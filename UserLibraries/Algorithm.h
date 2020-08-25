/*
 * Algorithm.h
 *
 *  Created on: Oct 18, 2019
 *      Author: Minh Thuan
 */

#ifndef USERLIBRARIES_ALGORITHM_H_
#define USERLIBRARIES_ALGORITHM_H_

int8_t  PID_Controller(float Kp,
                       float Ki,
                       float Kd,
                       int16_t Error,
                       int16_t CurrentState);



#endif /* USERLIBRARIES_ALGORITHM_H_ */
