/*
 * locationPlot.cpp
 *
 *  Created on: Mar 26, 2016
 *      Author: Asaf Sarid, Ohad Cohen
 */
/* Includes */
// General
#include <iostream>
#include <pthread.h>
// OpenCV
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/highgui/highgui.hpp"
// Our files
#include "globals.h"
/* Namespaces */
using namespace cv;
using namespace std;

void* plotLocation(void *info)
{
	// create window
	namedWindow("Location", WINDOW_AUTOSIZE);
	// declare matrix and size variables
	Mat myplot = imread("blackBack.jpeg");
	int cols = myplot.cols/2;
	int rows = myplot.rows/2;
	// read location, plot it and show it
	while (active)
	{
		char TestStr[500];
		sprintf(TestStr,"Current Location: (%.3f, %.3f)", distx, disty);
		putText(myplot, TestStr, Point(10,25), CV_FONT_NORMAL, 0.5, Scalar(255,255,255),1,1);
		circle(myplot, Point(cols + 2, rows + 3), 2, Scalar(0, 255, 0), -1);
		imshow("Location", myplot);
		usleep(1);
	}

	cvWaitKey(0);
	return NULL;
}


