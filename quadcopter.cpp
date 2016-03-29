/*
 * quadcopter.cpp
 *
 *  Created on: 22 Mar 2016
 *      Author: osboxes
 */


#include <iostream>
#include <unistd.h>
#include "quadcopter.h"
#include "globals.h"

using namespace std;

void *controller(void *control) {
	//Initialize
	Position* position = new Position();
	Attitude* attitude = new Attitude();
	// Get X,Y From Optical Flow
	double y_mes;
	double x_mes;
	// Get Z From Sonar Sensor
	double z_mes;
	// Get Euler Angles From IMU
	double the_mes;
	double phi_mes;
	double psi_mes;
	double mc1,mc2,mc3,mc4;

	for(;;){
		// Get X,Y From Optical Flow
		y_mes = disty;
		x_mes = distx;
		// Get Z From Sonar Sensor
		z_mes = distanceFromGround;
		// Get Euler Angles From IMU
		the_mes = eulerFromSensors.pitch;
		phi_mes = eulerFromSensors.roll;
		psi_mes = eulerFromSensors.yaw;

		//Position Controller
		position->calculate(0, y_mes, 0, x_mes);
		//Attitude Contoller
		attitude->calculate(position->phi_command, phi_mes, position->the_command, the_mes, position->psi_command, psi_mes, 0, 0);

		//Convert corrections to motors
		mc1 = attitude->z_correct - attitude->psi_correct - attitude->the_correct;
		mc2 = attitude->z_correct + attitude->psi_correct + attitude->phi_correct;
		mc3 = attitude->z_correct - attitude->psi_correct + attitude->the_correct;
		mc4 = attitude->z_correct + attitude->psi_correct - attitude->phi_correct;

//		cout << "Motor 1 = " << mc1 << endl;
//		cout << "Motor 2 = " << mc2 << endl;
//		cout << "Motor 3 = " << mc3 << endl;
//		cout << "Motor 4 = " << mc4 << endl;
		usleep(500000);
	}
	return 0;
}
