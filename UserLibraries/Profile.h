/*
 * Profile.h
 *
 *  Created on: Nov 20, 2019
 *      Author: Minh Thuan
 */

#ifndef USERLIBRARIES_PROFILE_H_
#define USERLIBRARIES_PROFILE_H_

#define PROFILE_PIN0 GPIO_PIN_0
#define PROFILE_PIN1 GPIO_PIN_1
#define PROFILE_PIN2 GPIO_PIN_2
#define PROFILE_PIN3 GPIO_PIN_3
#define PROFILE_PIN4 GPIO_PIN_4
#define PROFILE_PIN5 GPIO_PIN_5
#define PROFILE_PIN6 GPIO_PIN_6

#define DATA_Profile HWREG(0x400253FC)

#define PROFILE_PORT GPIO_PORTF_BASE

void Profile_Init(void);

void Profile_Toggle(uint8_t ui8Pin);

#endif /* USERLIBRARIES_PROFILE_H_ */
