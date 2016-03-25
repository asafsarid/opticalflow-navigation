/*
 * pos_control.h
 *
 *  Created on: 22 Mar 2016
 *      Author: osboxes
 */

#ifndef POS_CONTROL_H_
#define POS_CONTROL_H_

#include "pid.h"

class Position
{
    public:
		Position();
        ~Position();
        void calculate(double y_des, double y_mes, double x_des, double x_mes);
        double phi_command;
        double the_command;
        double psi_command;

    private:
        PID* x_pos;
        PID* y_pos;
};

#endif /* POS_CONTROL_H_ */
