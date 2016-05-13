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
    float rollspeed;
    float pitchspeed;
    float yawspeed;
}euler_angles;

typedef struct{
	double x;
	double y;
}locationStruct;

typedef struct{
	double lat;
	double lon;
	double alt;
}gpsCoords;

typedef struct{
	double value[13];
    int32_t location[13];
    int32_t length = 13;
	double median;
}heightMedian;

#endif /* GLOBALS_H_ */



// location of the UAV
extern locationStruct currLocation;
extern locationStruct lastFlowStep;
extern locationStruct gpsLocation;
// struct of Euler angles
extern euler_angles eulerFromSensors;
// distance- input from sensor
extern float distanceSonar;
// flag- active operation
extern int active;
extern int init;
// GPS initial and current coordinates
extern gpsCoords currGPSCoords;
extern gpsCoords initGPSCoords;
// median filter for height
extern heightMedian height;
// flag- stop opticalFlow
extern int end_run;
// current running time
extern std::string currentTime;
