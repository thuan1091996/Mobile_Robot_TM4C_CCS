/*
 * Algorithm.c
 *
 *  Created on: Oct 18, 2019
 *      Author: Minh Thuan
 */
#include "Userlibs.h"
#include "Algorithm.h"

int8_t  PID_Controller(float Kp,
                       float Ki,
                       float Kd,
                       int16_t Error,
                       int16_t CurrentState)
{
    static     int16_t intergrate_state=0;
    int16_t    intergrate_statemax=1000;
    int16_t    intergrate_statemin=-1000;
    int16_t    drive_temp;
    int8_t     pid_drive;
    intergrate_state+=Error;
    if(intergrate_state>intergrate_statemax)        intergrate_state=intergrate_statemax;
    else if (intergrate_state<intergrate_statemin)  intergrate_state=intergrate_statemin;
    drive_temp= (double)Error *Kp; //P term
    drive_temp+= (double)intergrate_state*Ki;

    //Bounded value
    if(drive_temp>100)
    {
        drive_temp=100;
    }
//    else if( (drive_temp>0) && (drive_temp<25) )
//    {
//        drive_temp=-25;
//    }
//    else if( (drive_temp<0) && (drive_temp>-25) )
//    {
//        drive_temp=0;
//    }
//    else if( (drive_temp<-25) && (drive_temp>-100) )
//    {
//        drive_temp=0;
//    }
    else if (drive_temp<-100)    drive_temp=-100;
    pid_drive=drive_temp;
    return pid_drive;
};

