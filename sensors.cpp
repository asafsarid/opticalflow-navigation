/*
 * sensors.cpp
 *
 *  Created on: Feb 15, 2016
 *      Author: parallels
 */
/* Includes */
// General
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <unistd.h>
#include <cmath>
#include <string.h>
#include <inttypes.h>
#include <fstream>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include "sensors.h"
#include "globals.h"
// Namespaces
using std::string;
using namespace std;
// Mavlink



/* Structs Declaration */



/* import global variable */
gpsCoords currGPSCoords;
gpsCoords initGPSCoords;
heightMedian height;
euler_angles eulerFromSensors;
float distanceFromGround;
int active;
int g_msgId;

/* Connect to the port of the sensors, configure the device and return the serial port descriptor */
Serial_Port* open_port(/*char *path*/)
{

	// 1. set path and baudrate
	char *uart_name = (char*)"/dev/ttyACM0";
	int baudrate = 115200;
	// 2. open the port and start it
	Serial_Port *p_sensorsPort = new Serial_Port(uart_name, baudrate);
	p_sensorsPort->start();
	// 3. configure the sensors to start stream
	mavlink_message_t msg;
	msg.checksum	= 0xb781;
	msg.magic 		= 254;
	msg.len			= 6;
	msg.seq			= 0;
	msg.sysid		= 255;
	msg.compid		= 1;
	msg.msgid		= 66;
	msg.payload64[0]= 20;
	msg.payload64[1]= 0;
	msg.payload64[2]= 1;
	msg.payload64[3]= 0;
	msg.payload64[4]= 0;
	msg.payload64[5]= 1;
	p_sensorsPort->write_message(msg);
	// 4. dummy read
	mavlink_message_t initMsg;
	p_sensorsPort->read_message(initMsg);

	// 4. return the port
	return p_sensorsPort;
}


/* close the port */
int close_port(Serial_Port* p_sensorsPort)
{
	p_sensorsPort->stop();
	return 0;
}


/* read the Euler angles from the IMU */
void *updateSensors(void *sensorsPort)
{
	// 1. cast the input pointer to the desired format
	Serial_Port *p_sensorsPort = (Serial_Port *)sensorsPort;
	// 2. open file to print the angles
//    FILE * euler_FD = fopen ("./outputs/eulerAngles.txt","a");
//    fprintf(euler_FD, "x is %d",0);
//    fclose(euler_FD);
    // 3. declare mavlink structs
	mavlink_message_t newMsg;
	mavlink_attitude_t attitudeMsg;
	mavlink_rangefinder_t rangeMsg;
	// 4. loop runs until we lower the active flag
	while(active)
	{
			// 4.1.1. read from device
			p_sensorsPort->read_message(newMsg);
			// 4.1.2. handle only attitude data
			switch (newMsg.msgid) {
			// 4.1.2.1. Euler angles
			case MAVLINK_MSG_ID_ATTITUDE:
				mavlink_msg_attitude_decode(&newMsg, &attitudeMsg);
				eulerFromSensors.pitch 	= attitudeMsg.pitch;
				eulerFromSensors.roll 	= attitudeMsg.roll;
				eulerFromSensors.yaw 	= attitudeMsg.yaw;
				break;
			// 4.1.2.2 Read Distance And Update
			case MAVLINK_MSG_ID_RANGEFINDER:
				mavlink_msg_rangefinder_decode(&newMsg, &rangeMsg);
				distanceFromGround		= rangeMsg.distance;
				break;
			// 4.1.2.3 Default Case
			default:
				break;
			}
	}
	return NULL;
}

void updateGPSLocation()
{
	double angle = angleFromCoordinates();
	double distance = distanceFromCoordinates();

	gpsLocation.x = distance * cos(toRadians(angle));
	gpsLocation.y = distance * sin(toRadians(angle));

	printf("GPS location: x- %.3f, y- %.3f\n", gpsLocation.x, gpsLocation.y);
}

// find angles between 2 gps coords- in degrees
double angleFromCoordinates()
{

	// curr
	double lat1 = currGPSCoords.lat / 10000000;
	double long1 = currGPSCoords.lon / 10000000;
	// init
	double lat2 = initGPSCoords.lat / 10000000;
	double long2 = initGPSCoords.lon / 10000000;

    double dLon = (long2 - long1);

    double y = sin(dLon) * cos(lat2);
    double x = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(dLon);

    double brng = atan2(y, x);

    brng = toDegrees(brng);
    brng = (brng + 360) % 360;
    brng = 360 - brng;

    return brng;
}

double distanceFromCoordinates()
{
	double R = 6371000; // metres
	// curr
	double φ1 = toRadians(currGPSCoords.lat);
	double φ2 = toRadians(initGPSCoords.lat);
	double Δφ = toRadians(initGPSCoords.lat-currGPSCoords.lat);
	double Δλ = toRadians(initGPSCoords.lon-currGPSCoords.lon);

	double a = sin(Δφ/2) * sin(Δφ/2) + cos(φ1) * cos(φ2) * sin(Δλ/2) * sin(Δλ/2);
	double c = 2 * atan2(sqrt(a), sqrt(1-a));

	double d = R * c;
	return d;
}

double toRadians(double angle)
{
	return 3.141592 * angle / 180;
}

double toDegrees(double angle)
{
	return 180 * angle / 3.141592;
}

void updateHeight(){
	int lastIndex;
	int i, j;
	uint32_t tempVal, tempLoc;

	// find the last sample (to switch with the new one)
	// increase location for the rest
	for(i=0; i< height.length; i++)
	{
		if(height.location[i] == height.length - 1)
			lastIndex = i;
		else
			height.location[i]++;
	}

	// insert new sample
	height.location[lastIndex] = 0;
	height.value[lastIndex] = distanceFromGround;

	// insetion sort
	// TODO: opt
	for (int i = 0; i < height.length; i++){
		j = i;

		while (j > 0 && height.value[j] < height.value[j-1]){
			  tempVal = height.value[j];
			  tempLoc = height.location[j];
			  height.value[j] = height.value[j-1];
			  height.location[j] = height.location[j-1];
			  height.value[j-1] = tempVal;
			  height.location[j-1] = tempLoc;
			  j--;
			  }
		}
	height.median = height.value[height.length/2];
}
