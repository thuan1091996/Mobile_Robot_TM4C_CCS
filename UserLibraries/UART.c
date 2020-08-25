/*
 * UART.c
 *
 *  Created on: Oct 17, 2019
 *      Author: Minh Thuan
 */
#include "Userlibs.h"
#include "UART.h"

/* -----------UART_Init ---------------
 * UART0 Initialization for PA0, PA1 (build-in connected with PC)
 * UART Configures: 115200, 8 bit, 1-bit stop, no parity check
 * Input: No
 * Output: No
 */
void UART_Init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0));
    GPIOPinConfigure(GPIO_PA0_U0RX);            //PIN mux for UART
    GPIOPinConfigure(GPIO_PA1_U0TX);            //PIN mux for UART
    GPIOPinTypeUART(GPIO_PORTA_BASE, RX0_PIN |TX0_PIN);
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
}

void UART_SendSpeed(int16_t i16_Speed)
{
    char b0,b1;
    b0 = i16_Speed & 0xFF;
    b1 = (i16_Speed>>8);
//    UARTCharPut(UART0_BASE, 99);
    UARTCharPut(UART0_BASE, b0);
    UARTCharPut(UART0_BASE, b1);
    UARTCharPut(UART0_BASE, 0x0A);          //Terminate
}

