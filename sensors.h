/*
 * sensors.h
 *
 *  Created on: 22 Mar 2016
 *      Author: osboxes
 */

#ifndef SENSORS_H_
#define SENSORS_H_

#include "c_library/common/mavlink.h"
#include "serial_port.h"


Serial_Port* open_port(/*char *path*/);
int close_port(Serial_Port* p_sensorsPort);
void *updateSensors(void *sensorsPort);
void updateGPSLocation();
double angleFromCoordinates();
double distanceFromCoordinates();
double toRadians(double angle);
double toDegrees(double angle);
void updateHeight();

#endif /* SENSORS_H_ */
