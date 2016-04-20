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
	int32_t lat;
	int32_t lon;
	int32_t alt;
}gpsCoords;

typedef struct{
	int32_t value[7];
	int32_t location[7];
	int32_t length;
	int32_t median;
}heightMedian;

#endif /* GLOBALS_H_ */



// location of the UAV
extern locationStruct currLocation;
extern locationStruct gpsLocation;
// struct of Euler angles
extern euler_angles eulerFromSensors;
// distance- input from sensor
extern float distanceSonar;
// flag- active operation
extern int active;
<<<<<<< HEAD
// flag- stop opticalFlow
=======
extern int init;
// msg id from sensors
extern int g_msgId;
// GPS initial and current coordinates
extern gpsCoords currGPSCoords;
extern gpsCoords initGPSCoords;
// median filter for height
extern heightMedian height;




>>>>>>> refs/remotes/origin/gpsHandle
extern int end_run;
// current running time
extern std::string currentTime;
