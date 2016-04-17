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
//#include "mainwindow.h"
#include <QApplication>

/* Namespaces */
using namespace cv;
using namespace std;

/* this function replace all spaces with underscores in string */
string space2underscore(string text)
{
    for(uint i = 0; i < text.length(); i++)
    {
           if( isspace(text[i]) )
                text[i] = '_';
    }
    return text;
}

int main(int argc, char** argv)
{

    QApplication a(argc, argv);
    MainWindow w;

    active=1;

    // current date/time based on current system
    time_t now = time(0);
    // convert now to string form
    char* dt = ctime(&now);
    string tempTime = (string)dt;
    currentTime = space2underscore(tempTime);

//    w.SavePlot();

    // open sensors port
    Serial_Port* p_sensorsPort = open_port();

    // create and run thread of updating the angles from IMU
    pthread_t euler_thread;
    pthread_create(&euler_thread, NULL, updateSensors, p_sensorsPort);

    // delay - waiting for angles and global variables to be stable
    sleep(10);

    w.show();
    // create and run thread for flight controller
 //	pthread_t controller_thread;
 //	pthread_create(&controller_thread, NULL, controller, NULL);

    // calculate location
    opticalFlow(1, NULL, w);

    // output plot
    //locationPlot(NULL);

    active = 0;

    // plot angles
//    anglesPlot(NULL);

    // close sensors port
    close_port(p_sensorsPort);

    return a.exec();
}



