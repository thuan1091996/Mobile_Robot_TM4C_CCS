/*
 * Delay_Systick.c
 *
 *  Created on: Jul 11, 2019
 *      Author: Itachi
 */

#include "Userlibs.h"
#include "Systick.h"

static unsigned long Tick=0;

/* SYSTICK Initialization
 * Function:- Initialization for Systick timer then load 80000  ->(80000 * 1/(80*10^6) = 1ms)
            - STAY IN LOOP until delay enough amount of time except when Systick interrupt execute
            - Use "HWREG" to change register value (more info in HW_type.h)
 * Input: No
 * Output: No
 * Change this function:
            + Change the Load value to create different time delay interval
            + Change the interrupt handler by "SystickIntRegister()" function
*/
void Systick_Init(){
//    SysTickIntRegister(Systick_ISR);    //Define  Systick ISR
    HWREG(NVIC_ST_CTRL)&=~0x07;         //Disable Systick Timer during initialization
    HWREG(NVIC_ST_RELOAD)=80000-1;      //(80 * 1/(80*10^6) = 1us)
    HWREG(NVIC_ST_CURRENT)=0;           //Write any value to clear the counter
    HWREG(NVIC_ST_CTRL)|=0x07;          //Enable Systick Timer and ARM for Systick interrupt
}

/* SYSTICK Overflow Interrupt Handler
 * The SYSTICK timer automatically load value from RELOAD_R so there no need to update new value
 * Interrupt after each 1ms
 * Input:  No
 * Output: No
*/
void Systick_ISR(){
    Tick++;                                         //Increase every 1 us corresponding to Reload value
}

/* Delay_ms second by SYSTICK
 * Function: Delay milliseconds corresponding to "milliseconds"
 * Input: The amount of milliseconds to delay
 * Output: No
 * Change: Use flag so the system is able to jump to another thread
           rather than stick in loop
-------------------------------------------------------*/
void Delay_ms(unsigned int miliseconds){
    Tick=0;                             //Reset count value
    while(Tick < miliseconds);
}

