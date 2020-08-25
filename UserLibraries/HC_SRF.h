// *****************************************************************************
//
//------------------------- 0.Driver information -------------------------------
// This library is used read data from multiple HC-SRF Sensors
// Created on: Dec, 29 2019
// Author : TRAN MINH THUAN
// Driver associate with TM4C123, CCS version 9.x
// HC - SR Sensors
//                |  Trigger |  Echo
//         -------+----------+--------
//          SR00  |  PB0     |  PA2
//          SR01  |  PB1     |  PA3
//          SR02  |  PB2     |  PA4
//          SR03  |  PB3     |  PA5
//          SR04  |  PA7     |  PA6
//******************************************************************************

#ifndef USERLIBRARIES_HC_SRF_H_
#define USERLIBRARIES_HC_SRF_H_


#define PORTA_IC_R      HWREG(0x4000441C)
#define PORTA_DATA_R    HWREG(0x400043FC)
#define ECHO_INT_MASK_R HWREG(0x40004410)
#define NUMB_SRF        5

#define TRIGGER0_PIN    GPIO_PIN_0
#define TRIGGER1_PIN    GPIO_PIN_1
#define TRIGGER2_PIN    GPIO_PIN_2
#define TRIGGER3_PIN    GPIO_PIN_3
#define TRIGGER4_PIN    GPIO_PIN_7


#define ECHO0_PIN       GPIO_PIN_2
#define ECHO1_PIN       GPIO_PIN_3
#define ECHO2_PIN       GPIO_PIN_4
#define ECHO3_PIN       GPIO_PIN_5
#define ECHO4_PIN       GPIO_PIN_6

#define TRIGGER0_PORT   GPIO_PORTB_BASE
#define TRIGGER1_PORT   GPIO_PORTB_BASE
#define TRIGGER2_PORT   GPIO_PORTB_BASE
#define TRIGGER3_PORT   GPIO_PORTB_BASE
#define TRIGGER4_PORT   GPIO_PORTA_BASE

#define ECHO_PORT       GPIO_PORTA_BASE
#define ECHO0_PORT      GPIO_PORTA_BASE
#define ECHO1_PORT      GPIO_PORTA_BASE
#define ECHO2_PORT      GPIO_PORTA_BASE
#define ECHO3_PORT      GPIO_PORTA_BASE
#define ECHO4_PORT      GPIO_PORTA_BASE

#define SRF0_INTFLAG	GPIO_INT_PIN_2
#define SRF1_INTFLAG	GPIO_INT_PIN_3
#define SRF2_INTFLAG	GPIO_INT_PIN_4
#define SRF3_INTFLAG	GPIO_INT_PIN_5
#define SRF4_INTFLAG	GPIO_INT_PIN_6

#define TIMER_VALUE     (*((volatile unsigned long *)0x40032050))

void SRF_Handler(void);
void SRF_Init(uint32_t ui32SRF_TriggerPort,
              uint32_t ui32SRF_TriggerPin,
              uint32_t ui32SRF_EchoPort,
              uint32_t ui32SRF_EchoPin);
void SRFs_Init(void);
uint32_t  SRF_GetDistance(uint8_t ui8SRF);


#endif /* USERLIBRARIES_HC_SRF_H_ */
