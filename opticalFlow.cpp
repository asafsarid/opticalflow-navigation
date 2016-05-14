/*
 * opticalFlow.cpp
 *
 *  Created on: Mar 16, 2016
 */

/*************************************** Includes ***************************************/
// General
#include <iostream>
#include <pthread.h>
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

/* determine if the pixel is in to desired trapezoid (changes according the angles of the capturing) */
int outOfROI(int x, int y, vector<Point2f> corners)
{
	uint   i, j=corners.size()-1 ;
	bool  oddNodes=0      ;

	for (i=0; i<corners.size(); i++) {
	  if ((corners[i].y < y && corners[j].y >= y) || (corners[j].y < y && corners[i].y >= y)) {
	    if (corners[i].x+(y-corners[i].y)/(corners[j].y-corners[i].y)*(corners[j].x-corners[i].x)<x) {
	      oddNodes=!oddNodes;
	    }
	  }
	  j=i;
	}

	return !oddNodes;
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
//        	if(outOfROI(x,y, corners))
//        		continue;
            const Point2f& fxy = flow.at<Point2f>(y, x);
            line(cflowmap, Point(x,y), Point(cvRound(x+fxy.x), cvRound(y+fxy.y)), color);
            circle(cflowmap, Point(x,y), 2, color, -1);
            distPixelx += fxy.x;
            distPixely += fxy.y;
            counter++;
        }
    // update the global variable - location of the UAV
    locationStruct notUpdatedFlowStep;
    notUpdatedFlowStep.x = distPixelx/counter;
    notUpdatedFlowStep.y = distPixely/counter;
    locationStruct lastFlowStep = calculateNewLocationByYaw(notUpdatedFlowStep);
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
//            if(outOfROI(x,y, corners))
//                continue;
            const Point2f& fxy = flow.at<Point2f>(y, x);
            distPixelx += fxy.x;
            distPixely += fxy.y;
            counter++;
        }
    // update the global variable - location of the UAV
    locationStruct notUpdatedFlowStep;
    notUpdatedFlowStep.x = distPixelx/counter;
    notUpdatedFlowStep.y = distPixely/counter;
    locationStruct lastFlowStep = calculateNewLocationByYaw(notUpdatedFlowStep);
}
#endif

void rotateImage(const Mat &input, UMat &output, double alpha, double beta, double gamma, double dx, double dy, double dz, double f)
  {
//    alpha = (alpha - 90.)*CV_PI/180.;
//    beta = (beta - 90.)*CV_PI/180.;
//    gamma = (gamma - 90.)*CV_PI/180.;
    // get width and height for ease of use in matrices
    double w = (double)input.cols;
    double h = (double)input.rows;
    // Projection 2D -> 3D matrix
    Mat A1 = (Mat_<double>(4,3) <<
              1/f, 0, -(w/2)/f,
              0, 1/f, -(h/2)/f,
              0, 0,    0,
              0, 0,    1);
    // Rotation matrices around the X, Y, and Z axis
    Mat RX = (Mat_<double>(4, 4) <<
              1,          0,           0, 0,
              0, cos(alpha), -sin(alpha), 0,
              0, sin(alpha),  cos(alpha), 0,
              0,          0,           0, 1);
    Mat RY = (Mat_<double>(4, 4) <<
              cos(beta), 0, sin(beta), 0,
              0, 1,          0, 0,
              -sin(beta), 0,  cos(beta), 0,
              0, 0,          0, 1);
    Mat RZ = (Mat_<double>(4, 4) <<
              cos(gamma), -sin(gamma), 0, 0,
              sin(gamma),  cos(gamma), 0, 0,
              0,          0,           1, 0,
              0,          0,           0, 1);
    // Composed rotation matrix with (RX, RY, RZ)
    Mat R = RZ * RY * RX;
    // Translation matrix
    Mat T = (Mat_<double>(4, 4) <<
             1, 0, 0, dx,
             0, 1, 0, dy,
             0, 0, 1, dz,
             0, 0, 0, 1);
    // 3D -> 2D matrix
    Mat A2 = (Mat_<double>(3,4) <<
              f, 0, w/2, 0,
              0, f, h/2, 0,
              0, 0,   1, 0);
    // Final transformation matrix
    Mat trans = A2 * (T * (R * A1));
    // Apply matrix transformation
    warpPerspective(input, output, trans, input.size(), INTER_LANCZOS4);
}

void rotateFrame(const Mat &input, Mat &output, Mat &A , double roll, double pitch, double yaw){
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
    Mat trans = A*R*A.inv();

    warpPerspective(input, output, trans, input.size());
}

/* this function finds the right location according yaw angle*/
locationStruct calculateNewLocationByYaw(locationStruct lastFlowStep){
	float yYaw = eulerFromSensors.yaw + 90;
	locationStruct outputLocation;

	outputLocation.x = lastFlowStep.x * cos(eulerFromSensors.yaw) + lastFlowStep.y * cos(yYaw);
	outputLocation.y = lastFlowStep.x * sin(eulerFromSensors.yaw) + lastFlowStep.y * sin(yYaw);

	return outputLocation;
}

/* calculate the location */
int opticalFlow(int source, /*char* capturePath,*/ MainWindow &w){

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
    double Xpred, Ypred;
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

        cout << "Yaw = " << eulerFromSensors.yaw*(180/M_PI) << endl;

//        rotateFrame(origFrame, processedFrame, cameraMatrix, eulerFromSensors.roll, 0, 0);

        rotateImage(processedFrame, gray, eulerFromSensors.roll, eulerFromSensors.pitch, 0, 0, 0, 1, 400);



		// undistort the frame using the calibration parameters
//		cv::undistort(origFrame, undistortFrame, cameraMatrix, distCoeffs, noArray());

//        // warpPerspective (using euler angles from IMU)
//        warpImage(undistortFrame, eulerFromSensors.yaw*(180/PI), eulerFromSensors.roll*(180/PI), eulerFromSensors.pitch*(180/PI), 1, 36, processedFrame, warp, corners);

		// lower the process effort by transforming the picture to gray


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
            rovX = 2*0.44523*distanceSonar*100; 	// 2 * tan(48/2) * dist(cm)
            rovY = 2*0.32492*distanceSonar*100;		// 2 * tan(36/2) * dist(cm)
//            cout << distanceSonar << endl;
#else
            double dist=87;             // distance from surface in cm
            rovX = 2*0.44523*dist; 		// 2 * tan(48/2) * dist
            rovY = 2*0.32492*dist;		// 2 * tan(36/2) * dist
#endif

            // predicted x,y change when rolling / pitching
            Xpred = (eulerFromSensors.pitchspeed*WIDTH_RES) / 48;
            Ypred = (eulerFromSensors.rollspeed*HEIGHT_RES) / 36;

            // calculate final x, y location
            currLocation.x -= ((lastFlowStep.x + Xpred)/WIDTH_RES)*rovX;
            currLocation.y += ((lastFlowStep.y - Ypred)/HEIGHT_RES)*rovY;

            // output to files
            fprintf(pLocationFile,"%f %f\n", currLocation.x, currLocation.y);
			fprintf(pAnglesFile,"%f %f %f\n", eulerFromSensors.pitch*(180/PI), eulerFromSensors.roll*(180/PI), eulerFromSensors.yaw*(180/PI));

            // Update Plots
            w.UpdatePlot(currLocation.x,currLocation.y);
            //w.AngleCorrectionUpdate(height.median, currLocation.y, distanceSonar, Ypred);
            w.AngleCorrectionUpdate(lastFlowStep.x, lastFlowStep.y, Xpred, Ypred);
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
