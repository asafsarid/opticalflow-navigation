/*
 * att_control.cpp

 *
 *  Created on: 22 Mar 2016
 *      Author: osboxes
 */

#include <iostream>
#include <cmath>
#include "pid.h"
#include "att_control.h"

#define Kp 2.1
#define Ki 1.1
#define Kd 1.2

using namespace std;




Attitude::Attitude()
{
    phi_pid = new PID(0.5, 1, -1, Kp, Kd, Ki);
    the_pid = new PID(0.5, 1, -1, Kp, Kd, Ki);
    psi_pid = new PID(0.5, 1, -1, Kp, Kd, Ki);
    z_pid = new PID(0.5, 1, -1, Kp, Kd, Ki);

    phi_correct = 0;
    the_correct = 0;
    psi_correct = 0;
    z_correct = 0;
}


void Attitude::calculate(double phi_des, double phi_mes, double the_des, double the_mes,
		double psi_des, double psi_mes, double z_des, double z_mes)
{
	phi_correct = phi_pid->calculate(phi_des, phi_mes);
	the_correct = the_pid->calculate(the_des, the_mes);
	psi_correct = psi_pid->calculate(psi_des, psi_mes);
	z_correct = z_pid->calculate(z_des, z_mes);
}


Attitude::~Attitude()
{
    delete phi_pid;
    delete psi_pid;
    delete the_pid;
    delete z_pid;
}




