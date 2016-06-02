/*
 * sensors.cpp
 *
 *  Created on: Feb 15, 2016
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

/* import global variable */
gpsCoords currGPSCoords;
gpsCoords initGPSCoords;
heightMedian height;
euler_angles eulerFromSensors;
euler_angles prevEulerFromSensors;
atomic<bool> eulerSpeedChanged;
float prevDistanceSonar;
float distanceSonar;
int active;

#define DISTANCE_CHANGES_THRESHOLD 0.5

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

    // 5. set the distance variables
	prevDistanceSonar = 0;
	distanceSonar = 0;

	// 6. return the port
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
    // 2. declare mavlink structs
	mavlink_message_t newMsg;
	mavlink_attitude_t attitudeMsg;
	mavlink_rangefinder_t rangeMsg;
    mavlink_gps_raw_int_t gpsIntMsg;

	// 3. loop runs until we lower the active flag
	while(active)
	{
			// 3.1.1. read from device
            int msgReceived = p_sensorsPort->read_message(newMsg);
            if (!(newMsg.msgid == MAVLINK_MSG_ID_RANGEFINDER) && !msgReceived)
                continue;
//            if (!msgReceived)
//                continue;
			// 3.1.2. handle only attitude data
			switch (newMsg.msgid) {
			// 3.1.2.1. Euler angles
			case MAVLINK_MSG_ID_ATTITUDE:
				mavlink_msg_attitude_decode(&newMsg, &attitudeMsg);
                if(attitudeMsg.pitchspeed != eulerFromSensors.pitchspeed ||
                   attitudeMsg.rollspeed != eulerFromSensors.rollspeed)
                {
                    prevEulerFromSensors.pitchspeed     = eulerFromSensors.pitchspeed;
                    prevEulerFromSensors.rollspeed      = eulerFromSensors.rollspeed;
                    prevEulerFromSensors.pitch          = eulerFromSensors.pitch;
                    prevEulerFromSensors.roll           = eulerFromSensors.roll;

                    eulerFromSensors.pitchspeed         = attitudeMsg.pitchspeed;
                    eulerFromSensors.rollspeed          = attitudeMsg.rollspeed;
                    eulerSpeedChanged.store(true);
                }
                    eulerFromSensors.pitch              = attitudeMsg.pitch;
                    eulerFromSensors.roll               = attitudeMsg.roll;
                    eulerFromSensors.yaw                = (-1) * attitudeMsg.yaw;

				break;
			// 3.1.2.2 Read Distance And Update
			case MAVLINK_MSG_ID_RANGEFINDER:
				mavlink_msg_rangefinder_decode(&newMsg, &rangeMsg);
				if(init)
				{
	                distanceSonar		= rangeMsg.distance*1.36;//*cos(eulerFromSensors.pitch)*cos(eulerFromSensors.roll);
				}
				// keep sonar changes filtered
				else
				{
					// if new sample is consider accurate
					if(abs(rangeMsg.distance*1.36 - distanceSonar) < DISTANCE_CHANGES_THRESHOLD)
					{
						//cout << "Sonar good. Old: " << distanceSonar << " , new: " << rangeMsg.distance << endl;
						distanceSonar		= rangeMsg.distance*1.36;
					}
					// new sample is noise- dont update. just consider the cos factor
//					else
//					{
//						cout << "Sonar not good. Old: " << distanceSonar << " , new: " << rangeMsg.distance << endl;
//					}
				}
//                  cout << "Sonar: " << distanceSonar << endl;
                //updateHeight();
                break;
			// 3.1.2.3 GPS
            case MAVLINK_MSG_ID_GPS_RAW_INT:
                mavlink_msg_gps_raw_int_decode(&newMsg, &gpsIntMsg);
                // if on init- update init coords, else- current
                if(init)
                {
                	initGPSCoords.lat = gpsIntMsg.lat / (double)10000000;
                	initGPSCoords.lon = gpsIntMsg.lon / (double)10000000;
                	initGPSCoords.alt = gpsIntMsg.alt / (double)10000000;
                }
                else
                {
                	currGPSCoords.lat = gpsIntMsg.lat / (double)10000000;
                	currGPSCoords.lon = gpsIntMsg.lon / (double)10000000;
                	currGPSCoords.alt = gpsIntMsg.alt / (double)10000000;
                	//updateGPSLocation();
                }
                break;
            // 3.1.2.3 Default Case
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
}

// find angles between 2 gps coords- in degrees
double angleFromCoordinates()
{

	// curr
	double lat1 = currGPSCoords.lat;
	double long1 = currGPSCoords.lon;
	// init
	double lat2 = initGPSCoords.lat;
	double long2 = initGPSCoords.lon;
    double dLon = (long2 - long1);

    double y = sin(dLon) * cos(lat2);
    double x = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(dLon);

    double brng = atan2(y, x);

    brng = toDegrees(brng);
    brng = (brng + 360);
    while(brng > 360)
    	brng -= 360;
    brng = 360 - brng;
    return brng;
}

// find distance between 2 gps coords
double distanceFromCoordinates()
{
	double R = 6371000; // metres
	// curr
	//printf("gps init: %.3f, %.3f. curr: %.3f, %.3f\n", initGPSCoords.lat, initGPSCoords.lon,
	//		currGPSCoords.lat, currGPSCoords.lon);
	double phi1 = toRadians(currGPSCoords.lat);
	double phi2 = toRadians(initGPSCoords.lat);
	double deltaPhi = toRadians(initGPSCoords.lat-currGPSCoords.lat);
	double deltaLambda = toRadians(initGPSCoords.lon-currGPSCoords.lon);

	double a = sin(deltaPhi/2) * sin(deltaPhi/2) + cos(phi1) * cos(phi2) * sin(deltaLambda/2) * sin(deltaLambda/2);
	double c = 2 * atan2(sqrt(a), sqrt(1-a));

	double d = R * c;
	//printf("gps distnace: %.3f\n", d);
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
	int lastIndex = 0;
	int i, j;
	double tempVal, tempLoc;

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
	height.value[lastIndex] = distanceSonar;

	// insetion sort
	// TODO: opt
	for (i = 0; i < height.length; i++){
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
