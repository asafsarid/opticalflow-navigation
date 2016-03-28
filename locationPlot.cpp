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

void plotLocation(void *info)
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


