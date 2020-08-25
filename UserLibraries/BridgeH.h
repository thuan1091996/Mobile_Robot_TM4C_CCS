/*
 * Double_H_Bridges.h
 *
 *  Created on: Jul 8, 2019
 *      Author: Itachi
 */

#ifndef DOUBLE_H_BRIDGES_H_
#define DOUBLE_H_BRIDGES_H_

#define BRIDGEH_GPIO_PERIPH             SYSCTL_PERIPH_GPIOE
#define BRIDGEH_GPIO_PORT               GPIO_PORTE_BASE

#define BRIDGEH_PWM_GPIO_PERIPH         SYSCTL_PERIPH_GPIOE
#define BRIDGEH_PWM_PERIPH              SYSCTL_PERIPH_PWM0
#define BRIDGEH_PWM_GPIO_PORT           GPIO_PORTE_BASE
#define BRIDGEH_PWM_PORT                PWM0_BASE
#define BRIDGEH_PWM_GEN                 PWM_GEN_2

#define DATA_PE_R                       HWREG(0x400243FC)
#define IN1_PIN                         GPIO_PIN_0
#define IN2_PIN                         GPIO_PIN_1
#define IN3_PIN                         GPIO_PIN_2
#define IN4_PIN                         GPIO_PIN_3


#define ENA_PIN                         GPIO_PIN_4
#define ENB_PIN                         GPIO_PIN_5
#define GPIO_ENACONFIG                  GPIO_PE4_M0PWM4
#define GPIO_ENBCONFIG                  GPIO_PE5_M0PWM5

#define MOTOR1_FORWARD                  IN1_PIN
#define MOTOR1_BACKWARD                 IN2_PIN
#define MOTOR2_FORWARD                  IN3_PIN
#define MOTOR2_BACKWARD                 IN4_PIN

#define MOTOR1                          (IN1_PIN|IN2_PIN)
#define MOTOR2                          (IN3_PIN|IN4_PIN)

#define F_PWM                           5000
#define PWM_PRESCALER                   8
#define LOAD_PWM                        80000000/(PWM_PRESCALER*F_PWM)   //PWM value = F_CPU/(Prescaler *F_PWM) with F_CPU=80*10^6 MHz

void BridgeH_GPIO_Init(void);
void BridgeH_PWM_Init(void);
void Update_Speed(int8_t i8_M1Duty, int8_t i8_M2Duty);
void Motor1_Forward(void);
void Motor1_Backward(void);
void Motor1_Stop(void);
void Motor2_Forward(void);
void Motor2_Backward(void);
void Motor2_Stop(void);
void Motor3_Forward(void);
void Motor3_Backward(void);
void Motor3_Stop(void);
void Motor4_Forward(void);
void Motor4_Backward(void);
void Motor4_Stop(void);


#endif /* DOUBLE_H_BRIDGES_H_ */
