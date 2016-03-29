/*
 * feedback.cpp
 *
 *  Created on: Mar 16, 2016
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
#include "sensors.h"
#include "opticalFlow.h"
#include "globals.h"
#include "quadcopter.h"
#include "locationPlot.h"


/* Namespaces */
using namespace cv;
using namespace std;


int main(int argc, char** argv)
{
	active=1;

	// open sensors port
	Serial_Port* p_sensorsPort = open_port();

	// create and run thread of updating the angles from IMU
	pthread_t euler_thread;
	pthread_create(&euler_thread, NULL, updateEulerAngles, p_sensorsPort);

	// delay - waiting for angles and global variables to be stable
	sleep(10);

	// create and run thread for flight controller
//	pthread_t controller_thread;
//	pthread_create(&controller_thread, NULL, controller, NULL);

	// calculate location
	opticalFlow(1, NULL);

	// output plot
	plotLocation(NULL);

	// close sensors port
	close_port(p_sensorsPort);

    return 0;
}



