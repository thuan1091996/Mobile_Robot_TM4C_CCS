/*
 * Encoder.h
 *
 *  Created on: Oct 15, 2019
 *      Author: minht
 */

#ifndef USERLIBRARIES_ENCODER_H_
#define USERLIBRARIES_ENCODER_H_

#define ENCODER_RESOLUTION      1324
#define SPEED_SAMPLE_PERIOD     0.1

void QEI0_INTHandler();
void QEI_Init(void);
void QEI_VelocityInit(void);
void Update_Position1(float *Pt_Pos1);
void Update_Velocity1(int16_t *Pt_Vel1);
#endif /* USERLIBRARIES_ENCODER_H_ */
