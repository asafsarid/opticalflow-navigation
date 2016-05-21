/*
 * opticalFlow.cpp
 *
 *  Created on: Mar 16, 2016
 */

/*************************************** Includes ***************************************/
// General
#include <iostream>
#include <pthread.h>
#include <time.h>
// OpenCV
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/highgui/highgui.hpp"
// Our files
#include "perspective.cpp"
#include "globals.h"
#include "mainwindow.h"

/*************************************** Namespaces ***************************************/
using namespace cv;
using namespace std;
/*************************************** Global Variables *********************************/
/* import global variables */
locationStruct currLocation;
locationStruct lastFlowStep;
locationStruct gpsLocation;
string currentTime;
int end_run;

/*************************************** Auxiliary functions ******************************/

/* this function finds the right location according yaw angle*/
locationStruct calculateNewLocationByYaw(locationStruct lastFlowStep){
    float yYaw = eulerFromSensors.yaw - M_PI/2;
    locationStruct outputLocation;

    outputLocation.x = lastFlowStep.x * cos(eulerFromSensors.yaw) + lastFlowStep.y * cos(yYaw);
    outputLocation.y = -(lastFlowStep.x * sin(eulerFromSensors.yaw) + lastFlowStep.y * sin(yYaw));

    return outputLocation;
}

/* this function draws the flow on the screen and accumulates the distance the UAV traveled */
#ifdef VIDEO_ACTIVE
void drawOptFlowMap(const Mat& flow, UMat& cflowmap, int step,
                    double, const Scalar& color, vector<Point2f> corners)
{
	// the distance each pixel traveled per two frames
	double distPixelx = 0, distPixely = 0;
	// count the number of pixels
	int counter = 0;
	// draw the optical flow and sum the distance that all the pixels have traveled
    for(int y = cflowmap.rows*0.25; y < cflowmap.rows*0.75; y += step)
        for(int x = cflowmap.cols*0.25; x < cflowmap.cols*0.75; x += step)
        {
            const Point2f& fxy = flow.at<Point2f>(y, x);
            line(cflowmap, Point(x,y), Point(cvRound(x+fxy.x), cvRound(y+fxy.y)), color);
            circle(cflowmap, Point(x,y), 2, color, -1);
            distPixelx += fxy.x;
            distPixely += fxy.y;
            counter++;
        }
    // update the global variable - location of the UAV
#ifdef YAW_ACTIVE
    locationStruct notUpdatedFlowStep;
    notUpdatedFlowStep.x = distPixelx/counter;
    notUpdatedFlowStep.y = distPixely/counter;
    lastFlowStep = calculateNewLocationByYaw(notUpdatedFlowStep);
#else
    lastFlowStep.x = distPixelx/counter;
    lastFlowStep.y = distPixely/counter;
#endif
}
#else
void calcAvgOpticalFlow(const Mat& flow, int step, vector<Point2f> corners)
{
    // the distance each pixel traveled per two frames
    double distPixelx = 0, distPixely = 0;
    // count the number of pixels
    int counter = 0;
    // draw the optical flow and sum the distance that all the pixels have traveled
    for(int y = flow.rows*0.25; y < flow.rows*0.75; y += step)
        for(int x = flow.cols*0.25; x < flow.cols*0.75; x += step)
        {
            const Point2f& fxy = flow.at<Point2f>(y, x);
            distPixelx += fxy.x;
            distPixely += fxy.y;
            counter++;
        }
    // update the global variable - location of the UAV
#ifdef YAW_ACTIVE
    locationStruct notUpdatedFlowStep;
    notUpdatedFlowStep.x = distPixelx/counter;
    notUpdatedFlowStep.y = distPixely/counter;
    lastFlowStep = calculateNewLocationByYaw(notUpdatedFlowStep);
#else
    lastFlowStep.x = distPixelx/counter;
    lastFlowStep.y = distPixely/counter;
#endif
}
#endif

void rotateImage(const Mat &input, UMat &output, double roll, double pitch, double yaw,
                 double dx, double dy, double dz, double f, double cx, double cy)
  {
    // Camera Calibration Intrinsics Matrix
    Mat A2 = (Mat_<double>(3,4) <<
              f, 0, cx, 0,
              0, f, cy, 0,
              0, 0, 1,  0);
    // Inverted Camera Calibration Intrinsics Matrix
    Mat A1 = (Mat_<double>(4,3) <<
              1/f, 0,   -cx/f,
              0,   1/f, -cy/f,
              0,   0,   0,
              0,   0,   1);
    // Rotation matrices around the X, Y, and Z axis
    Mat RX = (Mat_<double>(4, 4) <<
              1, 0,         0,          0,
              0, cos(roll), -sin(roll), 0,
              0, sin(roll), cos(roll),  0,
              0, 0,         0,          1);
    Mat RY = (Mat_<double>(4, 4) <<
              cos(pitch),  0, sin(pitch), 0,
              0,           1, 0,          0,
              -sin(pitch), 0, cos(pitch), 0,
              0,           0, 0,          1);
    Mat RZ = (Mat_<double>(4, 4) <<
              cos(yaw), -sin(yaw), 0, 0,
              sin(yaw),  cos(yaw), 0, 0,
              0,          0,       1, 0,
              0,          0,       0, 1);
    // Translation matrix
    Mat T = (Mat_<double>(4, 4) <<
             1, 0, 0, dx,
             0, 1, 0, dy,
             0, 0, 1, dz,
             0, 0, 0, 1);
    // Compose rotation matrix with (RX, RY, RZ)
    Mat R = RZ * RY * RX;
    // Final transformation matrix
    Mat H = A2 * (T * (R * A1));
    // Apply matrix transformation
    warpPerspective(input, output, H, input.size(), INTER_LANCZOS4);
}

void rotateFrame(const Mat &input, UMat &output, Mat &A , double roll, double pitch, double yaw){
    Mat Rx = (Mat_<double>(3, 3) <<
              1,          0,           0,
              0, cos(roll), -sin(roll),
              0, sin(roll),  cos(roll));
    Mat Ry = (Mat_<double>(3, 3) <<
              cos(pitch), 0, sin(pitch),
              0, 1,          0,
              -sin(pitch), 0,  cos(pitch));
    Mat Rz = (Mat_<double>(3, 3) <<
              cos(yaw), -sin(yaw), 0,
              sin(yaw),  cos(yaw), 0,
              0,          0,           1);

    Mat R = Rz*Ry*Rx;
    Mat H = A*R*A.inv();

    Mat T = (Mat_<double>(1,3) << WIDTH_RES/2, HEIGHT_RES/2, 1);
    Mat W = H*T.t();

    double bound_h = HEIGHT_RES*abs(cos(roll));
    double bound_w = WIDTH_RES*abs(cos(pitch));

    double dx = bound_w/2 - W.at<double>(0,0);
    double dy = bound_h/2 - W.at<double>(0,1);

    Mat temp = (Mat_<double>(3,3) << 1, 0, dx,
                                     0, 1, dy,
                                     0, 0, 1);
    Mat trans = temp*H;


    warpPerspective(input, output, H, input.size());
}

/* calculate the location of the UAV according the input frame from the camera and the angles of the body*/
int opticalFlow(int source, MainWindow &w){

    int tempX = 0;
    int tempY = 0;

   end_run = 0;
   cout << "Capture from: " << endl << source << endl;

   // capture from camera
   VideoCapture cap(0);
    if( !cap.isOpened() )
		return -1;

    //get fps of video
//	double fps = cap.get(CV_CAP_PROP_FPS);

	// Set Resolution - The Default Resolution Is 640 x 480
	cap.set(CV_CAP_PROP_FRAME_WIDTH,WIDTH_RES);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT,HEIGHT_RES);
//    cap.set(CV_CAP_PROP_CONVERT_RGB, 0);

	/*  set calibration parameters and variables for storing the current location */
	/******************************************************************************/
	Mat flow, cflow, undistortFrame, processedFrame, origFrame, croppedFrame;
	Mat cameraMatrix = (Mat_<double>(3,3) <<
                        4.0902279881001898e+02, 0, 1.5975000000000000e+02,
                        0, 4.0902279881001898e+02, 1.1975000000000000e+02,
                        0, 0, 1
						);
	Mat distCoeffs = (Mat_<double>(5,1) <<
					   -3.4107530298867622e-02,
						3.8417307420301627e-01,
						0,
						0,
					   -1.0237742683067783e+00
					);

	UMat gray, prevgray, uflow;

#ifdef VIDEO_ACTIVE
    namedWindow("flow", WINDOW_NORMAL);
#endif

    currLocation.x = 0;
    currLocation.y = 0;
	int i;
    locationStruct predLocation;
	double rovX, rovY;		// range of view in both axis

	/*  open files for output data											***/
	/******************************************************************************/

	cout << "The local date and time is: " << currentTime << endl;
	string locationFileName = "./outputs/" + currentTime + "location.txt";
	string anglesFileName = "./outputs/" + currentTime + "angles.txt";

	FILE * pLocationFile = fopen (locationFileName.c_str(), "w");
	FILE * pAnglesFile	 = fopen (anglesFileName.c_str(), "w");

	/*  compare every two frames												***/
	/******************************************************************************/
	// the distorted function can't open the first frame- so ignore it
	for(i = 0; i < 2; i++);
	// for each frame calculate optical flow
	for(;;)
	{
		Mat m, disp, warp;
		vector<Point2f> corners;
		// take out frame- still distorted
		cap >> origFrame;

        cvtColor(origFrame, processedFrame, COLOR_BGR2GRAY);


//        rotateFrame(processedFrame, gray, cameraMatrix, eulerFromSensors.roll, eulerFromSensors.pitch, 0);

        rotateImage(processedFrame, gray, eulerFromSensors.roll, eulerFromSensors.pitch, 0, 0, 0, 1, cameraMatrix.at<double>(0,0),
                    cameraMatrix.at<double>(0,2),cameraMatrix.at<double>(1,2));

		// undistort the frame using the calibration parameters
//        cv::undistort(origFrame, undistortFrame, cameraMatrix, distCoeffs, noArray());

		if( !prevgray.empty() )
		{
            // calculate flow
			calcOpticalFlowFarneback(prevgray, gray, uflow, 0.5, 3/*def 3 */, 10/* def 15*/, 3, 3, 1.2 /* def 1.2*/, 0);
            uflow.copyTo(flow);

            // get average
#ifdef VIDEO_ACTIVE
            drawOptFlowMap(flow, prevgray, 16, 1.5, Scalar(0, 255, 0), corners);
            imshow("flow", prevgray);
#else
            calcAvgOpticalFlow(flow, 16, corners);
#endif

            // calculate range of view - 2*tan(fov/2)*distance
#ifdef SONAR_ACTIVE
            rovX = 2*0.44523*height.median*100; 	// 2 * tan(48/2) * dist(cm)
            rovY = 2*0.32492*height.median*100;		// 2 * tan(36/2) * dist(cm)
#else
            double dist=87/(cos(eulerFromSensors.roll)*cos(eulerFromSensors.pitch));             // distance from surface in cm
            rovX = 2*0.44523*dist; 		// 2 * tan(48/2) * dist
            rovY = 2*0.32492*dist;		// 2 * tan(36/2) * dist
#endif

            if(eulerSpeedChanged.load())
            {
                eulerSpeedChanged.store(false);

                predLocation.x = ((eulerFromSensors.pitch-prevEulerFromSensors.pitch)*(180/M_PI)*WIDTH_RES) / 48;
                predLocation.y = ((eulerFromSensors.roll-prevEulerFromSensors.roll)*(180/M_PI)*HEIGHT_RES) / 36;
#ifdef YAW_ACTIVE
                predLocation = calculateNewLocationByYaw(predLocation);
#endif
                cout << "Sonar with factor: " << distanceSonar*1.36 << endl;

                // calculate final x, y location
                currLocation.x -= ((lastFlowStep.x + predLocation.x)/WIDTH_RES)*rovX;
                currLocation.y += ((lastFlowStep.y - predLocation.y)/HEIGHT_RES)*rovY;

                tempX += lastFlowStep.x;
                tempY += lastFlowStep.y;

                w.AngleCorrectionUpdate(tempX, tempY, predLocation.x, predLocation.y);

                tempX = 0;
                tempY = 0;
            }
            else{
                // calculate final x, y location
                currLocation.x -= (lastFlowStep.x/WIDTH_RES)*rovX;
                currLocation.y += (lastFlowStep.y/HEIGHT_RES)*rovY;
                tempX += lastFlowStep.x;
                tempY += lastFlowStep.y;
            }

            // output to files
            fprintf(pLocationFile,"%f %f\n", currLocation.x, currLocation.y);
			fprintf(pAnglesFile,"%f %f %f\n", eulerFromSensors.pitch*(180/PI), eulerFromSensors.roll*(180/PI), eulerFromSensors.yaw*(180/PI));

            // Update Plots
            w.UpdatePlot(currLocation.x,currLocation.y);
//            w.AngleCorrectionUpdate(eulerFromSensors.roll*(180/M_PI), eulerFromSensors.pitch*(180/M_PI), 0, 0);
		}
        //break conditions
        if(waitKey(1)>=0)
            break;
        if(end_run)
            break;
		std::swap(prevgray, gray);
	}

#ifdef VIDEO_ACTIVE
    destroyWindow("flow");
#endif

	// close the files
	fclose(pLocationFile);
	fclose(pAnglesFile);

	return 0;
}
