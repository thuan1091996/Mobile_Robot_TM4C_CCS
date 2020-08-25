/*
 * Profile.c
 *
 *  Created on: Nov 20, 2019
 *      Author: Minh Thuan
 */

#include "Userlibs.h"
#include "Profile.h"

void Profile_Init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);      //Enable Profile pins GPIO port
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));
    GPIOPinTypeGPIOOutput(PROFILE_PORT, PROFILE_PIN0|PROFILE_PIN1|PROFILE_PIN2|PROFILE_PIN3|PROFILE_PIN4|PROFILE_PIN5|PROFILE_PIN6);
    DATA_Profile&=~(0xFF);      //clear all Profile port pins
}

void Profile_Toggle(uint8_t ui8Pin)
{
    DATA_Profile^=ui8Pin;
}




