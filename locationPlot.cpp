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
#include <unistd.h>
// OpenCV
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/highgui/highgui.hpp"
// Our files
#include "globals.h"
#include "locationPlot.h"
/* Namespaces */
using namespace cv;
using namespace std;

void* plotLocation(void *info)
{
	// create window
	namedWindow("Location", WINDOW_AUTOSIZE);
	// declare matrix and size variables
	Mat myplot = imread("blackBack2.jpeg");
	int cols = myplot.cols/2;
	int rows = myplot.rows/2;

	int i=0;
	// read location, plot it and show it
	for(;;)
	{
		i++;
		char TestStr[500];
		float curr_x = distx;
		float curr_y = disty;
		cout << "DistanceX = " << distx << endl;
		cout << "DistanceY = " << disty << endl;
		sprintf(TestStr,"Current Location: (%.3f, %.3f)", curr_x, curr_y);
		putText(myplot, TestStr, Point(10,25), CV_FONT_NORMAL, 0.5, Scalar(255,255,255),1,1);
		circle(myplot, Point(cols + curr_x, rows + curr_y), 2, Scalar(0, 255, 0), -1);
//		if (i % 100 == 0)
		imshow("Location", myplot);
		if(waitKey(1000)>=0)
			break;
	}

	cvWaitKey(0);
	return NULL;
}


