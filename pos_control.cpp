/*
 * att_control.cpp

 *
 *  Created on: 22 Mar 2016
 */
#include <iostream>
#include <cmath>
#include "pid.h"
#include "pos_control.h"

//PID coefs
#define Kp 2
#define Ki 1.1
#define Kd 1.2

using namespace std;


Position::Position()
{
    //x pid
    x_pos = new PID(0.5, 45*3.14/180, -45*3.14/180, 1.2, 0.4, 0);
    //y pid
    y_pos = new PID(0.5, 45*3.14/180, -45*3.14/180, 1.2, 0.4, 0);

    //initial command
    phi_command = 0;
    the_command = 0;
    psi_command = 0;
}

// get command from desired and measured
void Position::calculate(double y_des, double y_mes, double x_des, double x_mes)
{
	phi_command = y_pos->calculate(y_des, y_mes);
	the_command = x_pos->calculate(x_des, x_mes);
	psi_command = 0;
}


Position::~Position()
{
    delete x_pos;
    delete y_pos;
}




