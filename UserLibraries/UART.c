// *****************************************************************************
//
//------------------------- 0.Driver information -------------------------------
// This library is used communication via COM port
// Created on: Oct 17, 2019
// Author : TRAN MINH THUAN
// Driver associate with TM4C123, Keil 5
//******************************************************************************
#include "Userlibs.h"
#include "UART.h"

// *****************************************************************************
//
//---------------------------- 1.Global variables ------------------------------
//
// *****************************************************************************




// *****************************************************************************
//
//--------------------------- 2.Static varaibles  ------------------------------
//
// *****************************************************************************




// *****************************************************************************
//
//-------------------------- 3. Function definition  ---------------------------
//
// *****************************************************************************

// *****************************************************************************
//
//! Initialize UART
//! void UART_Init(void)
//! \param ui32Speed - Commnucation speed
//! UART Initialization for TX, RX pins
//! UART Configures: 8 bit, 1-bit stop, no parity check
//
//******************************************************************************

void UART_Init(uint32_t ui32Speed)
{
    SysCtlPeripheralEnable(UART_GPIO_PERIPH);
    SysCtlPeripheralEnable(UART_PERIPH);
    while(!SysCtlPeripheralReady(UART_GPIO_PERIPH));
    while(!SysCtlPeripheralReady(UART_PERIPH));
    GPIOPinConfigure(GPIO_PA0_U0RX);            //PIN mux for UART
    GPIOPinConfigure(GPIO_PA1_U0TX);            //PIN mux for UART
    GPIOPinTypeUART(GPIO_PORTA_BASE, RX0_PIN |TX0_PIN);
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), ui32Speed,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
}

void UART_SendSpeed(int16_t i16_Speed)
{
    char b0,b1;
    b0 = i16_Speed & 0xFF;
    b1 = (i16_Speed>>8);
    UARTCharPut(UART0_BASE, 99);          //START
    UARTCharPut(UART0_BASE, b0);
    UARTCharPut(UART0_BASE, b1);
    UARTCharPut(UART0_BASE, 100);         //STOP
}

