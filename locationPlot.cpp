/*
 * locationPlot.cpp
 *
 *  Created on: Mar 26, 2016
 *      Author: Asaf Sarid, Ohad Cohen
 */

/* Includes */
// General
#include <iostream>
#include <fstream>
// OpenCV
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/highgui/highgui.hpp"
// Our files
#include "locationPlot.h"

/* Namespaces */
using namespace cv;
using namespace std;

void locationPlot(void *info)
{
	fstream in("./outputs/location.txt");
	string line;
	vector<vector<float> > coords;
	int i = 0;
	cout << "Starting Graph Thread" << endl;
	while (getline(in, line))
	{
		float value;
		stringstream ss(line);

		coords.push_back(vector<float>());

		while (ss >> value)
		{
			coords[i].push_back(value);
		}
		cout << "Got Line - " << line << endl;
		++i;
	}
	in.close();
	cout << "End Of Graph Thread" << endl;
	// create window
	namedWindow("Location", WINDOW_AUTOSIZE);

	// declare matrix and size variables
	Mat myplot = imread("blackBackGraph.jpeg");
	int cols = myplot.cols/2;
	int rows = myplot.rows/2;

	// read location and plot it
	for(vector<vector<float> >::iterator it = coords.begin(); it != coords.end(); ++it)
	{

		vector<float>::iterator it2 = (*it).begin();
		float tempX = *it2;
		++it2;
		float tempY = *it2;
		// draw point
		circle(myplot, Point(cols + tempX*10, rows + tempY*10), 2, Scalar(0, 255, 0), -1);
	}

	// show plot
	imshow("Location", myplot);

	cvWaitKey(0);
	destroyWindow("Location");
}

// plot the angles of the uav during the flight
void anglesPlot(void *info)
{
	fstream in("./outputs/angles.txt");
	string line;
	vector<vector<float> > coords;
	int i = 0;
	while (getline(in, line))
	{
		float value;
		stringstream ss(line);

		coords.push_back(vector<float>());

		while (ss >> value)
		{
			coords[i].push_back(value);
		}
		++i;
	}
	in.close();

	// create window
	namedWindow("Angles", WINDOW_AUTOSIZE);

	// declare matrix and size variables
	Mat myplot = imread("anglesBack.jpeg");
	int pitchCols 	= 28;
	int pitchRows 	= 310;
	int rollCols 	= 28;
	int rollRows 	= 107;
	int yawCols 	= 28;
	int yawRows 	= 604;
	int counter		= 0;

	// read location and plot it
	for(vector<vector<float> >::iterator it = coords.begin(); it != coords.end(); ++it)
	{

		vector<float>::iterator it2 = (*it).begin();
		// draw angles
		float tempPitch = *it2;
		circle(myplot, Point(pitchCols + counter, pitchRows + tempPitch/2), 2, Scalar(0, 255, 0), -1);
		++it2;
		float tempRoll 	= *it2;
		circle(myplot, Point(rollCols + counter, rollRows + tempRoll/2), 2, Scalar(0, 255, 0), -1);
		++it2;
		float tempYaw	= *it2;
		circle(myplot, Point(yawCols + counter, yawRows + tempYaw/2), 2, Scalar(0, 255, 0), -1);

		counter++;
	}

	// show plot
	imshow("Angles", myplot);

	cvWaitKey(0);
	destroyWindow("Angles");
}
