/****************************************************************************
* Title                 :   Double Bridge H Driver
* ProductLink           :   banlinhkien.vn/goods-9856-mach-cau-h-7a-dc6-5-27v-mc-7a6527.html
* Filename              :   BridgeH.h
* Author                :   ItachiThuan
* Origin Date           :   Jul 8, 2019
* Version               :   1.0.1
* Target                :   TM4C123 with CCS IDE
* Notes                 :
  ENA     |  ENB     |  IN1    |  IN2   |  IN3   |  IN4
----------+----------+-------- +--------+--------+------
  PE4     |  PE5     |  PD0    |  PD1   |  PD2   |  PD3
*****************************************************************************/

/*************** INTERFACE CHANGE LIST **************************************
*    Date           + Software Version  +  Initials Description
*  Jul 8th, 2019      | v1.0.0            |  Interface Created.
*  Dec 5th, 2020      | v1.0.1            |  Change hardware pin to re-start the project
*****************************************************************************/
/** \BridgeH.h
 *  \brief: Contains API to drive 2 motors via bridge H 7A
 */
#ifndef DOUBLE_H_BRIDGES_H_
#define DOUBLE_H_BRIDGES_H_

/******************************************************************************
* Preprocessor
*******************************************************************************/
#define BRIDGEH_GPIO_PERIPH             SYSCTL_PERIPH_GPIOD
#define BRIDGEH_GPIO_PORT               GPIO_PORTD_BASE
#define BRIDGEH_GPIO_DATA               HWREG(GPIO_PORTD_BASE + 0x3FC)

#define BRIDGEH_PWM_GPIO_PERIPH         SYSCTL_PERIPH_GPIOE
#define BRIDGEH_PWM_PERIPH              SYSCTL_PERIPH_PWM0
#define BRIDGEH_PWM_GPIO_PORT           GPIO_PORTE_BASE
#define BRIDGEH_PWM_PORT                PWM0_BASE
#define BRIDGEH_PWM_GEN                 PWM_GEN_2

#define IN1_PIN                         GPIO_PIN_0
#define IN2_PIN                         GPIO_PIN_1
#define IN3_PIN                         GPIO_PIN_2
#define IN4_PIN                         GPIO_PIN_3

#define ENA_PIN                         GPIO_PIN_4
#define ENB_PIN                         GPIO_PIN_5

#define GPIO_ENACONFIG                  GPIO_PE4_M0PWM4
#define GPIO_ENBCONFIG                  GPIO_PE5_M0PWM5

#define MOTOR0_FORWARD                  IN3_PIN
#define MOTOR0_BACKWARD                 IN4_PIN
#define MOTOR1_FORWARD                  IN1_PIN
#define MOTOR1_BACKWARD                 IN2_PIN

#define MOTOR0                          (IN3_PIN|IN4_PIN)
#define MOTOR1                          (IN1_PIN|IN2_PIN)



/******************************************************************************
* Configuration Constants
*******************************************************************************/
#define F_PWM                           5000
#define PWM_PRESCALER                   8
#define LOAD_PWM                        80000000/(PWM_PRESCALER*F_PWM)   //PWM value = F_CPU/(Prescaler *F_PWM) with F_CPU=80*10^6 MHz

/******************************************************************************
* Function Prototypes
*******************************************************************************/
void BridgeH_GPIO_Init(void);
void BridgeH_PWM_Init(void);

void Motor0_UpdateSpeed(int8_t i8_M0Duty);
void Motor0_Forward(void);
void Motor0_Backward(void);
void Motor0_Stop(void);


void Motor1_UpdateSpeed(int8_t i8_M1Duty);
void Motor1_Forward(void);
void Motor1_Backward(void);
void Motor1_Stop(void);

void Motor2_UpdateSpeed(int8_t i8_M2Duty);
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
