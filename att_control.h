/*
 * att_control.h
 *
 *  Created on: 22 Mar 2016
 *      Author: osboxes
 */

#ifndef ATT_CONTROL_H_
#define ATT_CONTROL_H_

#include "pid.h"

class Attitude
{
    public:
		Attitude();
        ~Attitude();
        void calculate(double phi_des, double phi_mes, double the_des, double the_mes,
        		double psi_des, double psi_mes, double z_des, double z_mes);
        double phi_correct;
        double the_correct;
        double psi_correct;
        double z_correct;

    private:
        PID* phi_pid;
        PID* the_pid;
        PID* psi_pid;
        PID* z_pid;
};


#endif /* ATT_CONTROL_H_ */
