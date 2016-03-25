/*
 * opticalFlow.cpp
 *
 *  Created on: Mar 16, 2016
 *      Author: Asaf Sarid, Ohad Cohen
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
/*************************************** Namespaces ***************************************/
using namespace cv;
using namespace std;
/*************************************** Global Variables *********************************/
// distance frame have traveled
double distx;
double disty;


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
void drawOptFlowMap(const Mat& flow, Mat& cflowmap, int step,
                    double, const Scalar& color, double fps, vector<Point2f> corners)
{
	// the distance each pixel traveled per two frames
	double distPixelx = 0, distPixely = 0;
	// count the number of pixels
	int counter = 0;
	// draw the optical flow and sum the distance that all the pixels have traveled
    for(int y = 0; y < cflowmap.rows; y += step)
        for(int x = 0; x < cflowmap.cols; x += step)
        {
        	if(outOfROI(x,y, corners))
        		continue;
            const Point2f& fxy = flow.at<Point2f>(y, x);
            line(cflowmap, Point(x,y), Point(cvRound(x+fxy.x), cvRound(y+fxy.y)), color);
            circle(cflowmap, Point(x,y), 2, color, -1);
            distPixelx += fxy.x;
            distPixely += fxy.y;
            counter++;
        }
    // update the global variable - location of the UAV
    distx = distPixelx/counter;
    disty = distPixely/counter;
}


/* calculate the location */
int opticalFlow(int source, char* capturePath){
   cout << "Capture from: " << endl << source << endl;

   VideoCapture cap(0); // capture from camera 1

	if( !cap.isOpened() )
		return -1;
	double fps = cap.get(CV_CAP_PROP_FPS);
	//double numOfFrames = cap.get(CV_CAP_PROP_FRAME_COUNT);

	/*  set calibration parameters and variables for storing the current location */
	/******************************************************************************/
	Mat flow, cflow, undistortFrame, processedFrame, origFrame, croppedFrame;
	Mat cameraMatrix = (Mat_<double>(3,3) <<
						8.1782279881001898e+02,0, 3.1950000000000000e+02,
						0,8.1782279881001898e+02 ,2.3950000000000000e+02,
						0,0,1
						);
	Mat distCoeffs = (Mat_<double>(5,1) <<
					   -3.4107530298867622e-02,
						3.8417307420301627e-01,
						0,
						0,
					   -1.0237742683067783e+00
					);

	UMat gray, prevgray, uflow;


	namedWindow("flow", WINDOW_NORMAL);

	double location[2];
	int frame_counter = 0;
	int i;
	double rovX, rovY;		// range of view in both axis
	double dist=87; 		// distance from surface in cm
	rovX = 2*0.4*dist; 		// 2 * tan(43.6/2) * dist
	rovY = 2*0.3*dist;		// 2 * tan(33.7/2) * dist
	/*  open files for output data											***/
	/******************************************************************************/
	//FILE * pFile2Location = fopen ("./outputs/location.txt","w");

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
		// undistort the frame using the calibration parameters
		cv::undistort(origFrame, undistortFrame, cameraMatrix, distCoeffs, noArray());

		warpImage(undistortFrame, eulerFromSensors.yaw*(180/3.14), eulerFromSensors.pitch*(180/3.14), eulerFromSensors.roll*(180/3.14), 1, 30, processedFrame, warp, corners);
		// lower the process effort by transforming the picture to gray
		cvtColor(processedFrame, gray, COLOR_BGR2GRAY);

		if( !prevgray.empty() )
		{
			calcOpticalFlowFarneback(prevgray, gray, uflow, 0.5, 3/*def 3 */, 15/* def 15*/, 3, 5, 1.2 /* def 1.2*/, 0);
			cvtColor(prevgray, cflow, COLOR_GRAY2BGR);
			uflow.copyTo(flow);
			drawOptFlowMap(flow, cflow, 16, 1.5, Scalar(0, 255, 0), fps, corners);
			location[0] += (distx/640)*rovX;
			location[1] += (disty/480)*rovY;
			frame_counter++;

			char TestStr[500];
			sprintf(TestStr,"Frame: %d    Location:   "
					"X: %.3f, Y: %.3f.  Euler: %.3f, %.3f, %.3f. dist: %.3f", frame_counter, location[0], location[1],
					eulerFromSensors.pitch*(180/3.14), eulerFromSensors.roll*(180/3.14), eulerFromSensors.yaw*(180/3.14), distanceFromGround 		);

			putText(cflow, TestStr, Point(10,25), CV_FONT_NORMAL, 0.5, Scalar(255,255,255),1,1); //OutImg is Mat class;

			imshow("flow", cflow);
		}

		if(waitKey(1)>=0)
			break;
		std::swap(prevgray, gray);
	}

	// close the files
	//fclose(pFile2Location);

	return 0;
}
