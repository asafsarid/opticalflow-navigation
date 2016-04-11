/*
 * globals.h
 *
 *  Created on: Mar 16, 2016
 *      Author: parallels
 */
#ifndef GLOBALS_H_
#define GLOBALS_H_

#define WIDTH_RES 320
#define HEIGHT_RES 240
#define PI 3.14159265

/* Structs Declaration */
typedef struct {
	float roll; /*< Roll angle (rad, -pi..+pi)*/
	float pitch; /*< Pitch angle (rad, -pi..+pi)*/
	float yaw; /*< Yaw angle (rad, -pi..+pi)*/
}euler_angles;


#endif /* GLOBALS_H_ */



// distance frame have traveled
extern double distx;
extern double disty;
// struct of Euler angles
extern euler_angles eulerFromSensors;
// distance- input from sensor
extern float distanceFromGround;
// flag- active operation
extern int active;
// msg id from sensors
extern int g_msgId;

extern int end_run;
// current running time
extern std::string currentTime;
