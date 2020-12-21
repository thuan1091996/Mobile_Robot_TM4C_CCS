/*
 * Delay_Systick.h
 *
 *  Created on: Jul 11, 2019
 *      Author: Itachi
 */

#ifndef USERLIBRARIES_SYSTICK_H_
#define USERLIBRARIES_SYSTICK_H_



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
void Systick_Init(void);

/* SYSTICK Overflow Interrupt Handler
 * The SYSTICK timer automatically load value from RELOAD_R so there no need to update new value
 * Interrupt after each 1ms
 * Input:  No
 * Output: No
*/
void Systick_ISR(void);

/* Delay_ms second by SYSTICK
 * Function: Delay milliseconds corresponding to "milliseconds"
 * Input: The amount of milliseconds to delay
 * Output: No
 * Change: Use flag so the system is able to jump to another thread
           rather than stick in loop
-------------------------------------------------------*/
void Delay_ms(unsigned int miliseconds);


#endif /* USERLIBRARIES_SYSTICK_H_ */
