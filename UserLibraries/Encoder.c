/*
 * Encoder.c
 *
 *  Created on: Oct 15, 2019
 *      Author: minht
 */

#ifndef USERLIBRARIES_ENCODER_C_
#define USERLIBRARIES_ENCODER_C_

#include "Userlibs.h"
#include "Encoder.h"
#include "UART.h"

extern uint8_t ui8Flag_FreshVel;            //Flag indicate new velocity data available

/* -----------QEI0_INTHandler---------------
 * Velocity Timer expire of QEI0 interrupt handler
 * Use global variable - " ui8Flag_FreshSpeed " to indicate that completed current period
 * Input: No
 * Output: No
 */
void QEI0_INTHandler()
{
    QEIIntClear(QEI1_BASE, QEI_INTTIMER);       //acknowledge interrupt
    ui8Flag_FreshVel=1;                         //New velocity data available
};

/* -----------QEI_Init ---------------
 * QEI Initialization for PD6 PD7
 * QEI Quadrature mode, no swap, no index, no Filter, "RESOLUTION" rising/falling edges per motor revolution
 * Input: No
 * Output: No
 * Change: Change the ENCODER_RESULUTION = Number of Rising/Falling edges per motor revolution on both channel A & B
 */
void QEI_Init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI1);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC));
//    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD));
//    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_QEI0));
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_QEI1));
    //Unlock PD7
//    HWREG(GPIO_PORTD_BASE+GPIO_O_LOCK) = GPIO_LOCK_KEY;
//    HWREG(GPIO_PORTD_BASE+GPIO_O_CR) |= GPIO_PIN_7;
		//GPIO Configure for QEI
    GPIOPinTypeQEI(GPIO_PORTC_BASE, GPIO_PIN_5);
    GPIOPinTypeQEI(GPIO_PORTC_BASE, GPIO_PIN_6);
//    GPIOPinTypeQEI(GPIO_PORTD_BASE, GPIO_PIN_6);
//    GPIOPinTypeQEI(GPIO_PORTD_BASE, GPIO_PIN_7);
    GPIOPinConfigure(GPIO_PC5_PHA1);
    GPIOPinConfigure(GPIO_PC6_PHB1);
//    GPIOPinConfigure(GPIO_PD6_PHA0);
//    GPIOPinConfigure(GPIO_PD7_PHB0);
		//QEI Configure
//    QEIDisable(QEI0_BASE);  //First disable  to configure
    QEIDisable(QEI1_BASE);
//    QEIIntDisable(QEI0_BASE, (QEI_INTERROR | QEI_INTDIR | QEI_INTTIMER | QEI_INTINDEX));
    QEIIntDisable(QEI1_BASE, (QEI_INTERROR | QEI_INTDIR | QEI_INTTIMER | QEI_INTINDEX));
//	QEIVelocityDisable(QEI0_BASE);
    QEIVelocityDisable(QEI1_BASE);

    // instruction below *100 because in case Sample Time >1s, QEIMAXPOS reg may not overload because there more than 1324 pulses /sec
    // no need cuz QEI_COUNT_R and QEI_MAXPOS work independently
//    QEIConfigure(QEI0_BASE, QEI_CONFIG_CAPTURE_A_B|QEI_CONFIG_NO_RESET|QEI_CONFIG_QUADRATURE|QEI_CONFIG_NO_SWAP, ENCODER0_RESOLUTION-1);
    QEIConfigure(QEI1_BASE, QEI_CONFIG_CAPTURE_A_B|QEI_CONFIG_NO_RESET|QEI_CONFIG_QUADRATURE|QEI_CONFIG_NO_SWAP, ENCODER_RESOLUTION-1);

//    QEIVelocityConfigure(QEI0_BASE, QEI_VELDIV_1, (SysCtlClockGet()*SPEED_SAMPLE_PERIOD));
    QEIVelocityConfigure(QEI1_BASE, QEI_VELDIV_1, (SysCtlClockGet()*SPEED_SAMPLE_PERIOD));

    //Interrupt configure
//    QEIIntRegister(QEI0_BASE, QEI0_INTHandler);
    QEIIntRegister(QEI1_BASE, QEI0_INTHandler);

//    IntPrioritySet(INT_QEI0_TM4C123, ui8Priority0);
//    IntPrioritySet(INT_QEI1_TM4C123, ui8Priority1);

//    QEIIntEnable(QEI0_BASE, QEI_INTTIMER);  //Velocity timer expires interrupt enable
    QEIIntEnable(QEI1_BASE, QEI_INTTIMER);  //Velocity timer expires interrupt enable

//    QEIPositionSet(QEI0_BASE, 0);       //Reset counter
    QEIPositionSet(QEI1_BASE, 0);

//    Filter
//    QEIFilterConfigure(QEI0_BASE, QEI_FILTCNT_7);
//    QEIFilterEnable(QEI0_BASE);

//    QEIVelocityEnable(QEI0_BASE);
    QEIVelocityEnable(QEI1_BASE);

//    QEIEnable(QEI0_BASE);
    QEIEnable(QEI1_BASE);
};

/* -----------QEI_VelocityInit ---------------
 * QEI Velocity Initialization with Timer clock = System clock
 * Setting for QEI Timer Expire interrupt to interrupt every period
 * Input:  No
 * Output: No
 */
void QEI_VelocityInit(void)
{
    QEIVelocityDisable(QEI1_BASE);  //Disable before configure
    QEIVelocityConfigure(QEI1_BASE, QEI_VELDIV_1, (SysCtlClockGet()*SPEED_SAMPLE_PERIOD));
    QEIVelocityEnable(QEI1_BASE);
    //Interrupt configure
    QEIIntRegister(QEI1_BASE, QEI0_INTHandler);
    QEIIntEnable(QEI1_BASE, QEI_INTTIMER);  //Velocity timer expires interrupt enable
};

/* -----------Update_Position ---------------
 * Update position by accumulated position overtime
 * Input: Pointer to variable contain "Current Position"
 * Output: No
 */
void Update_Position1(float *Pt_Pos1)
{
    static uint32_t pos_temp=0;                     //Falling/Rising edges detected in current period
    static int8_t   dir_temp=0;                     //Direction of rotation
    float pos_offset=0;                             //Position offset
    dir_temp=QEIDirectionGet(QEI0_BASE);
    pos_temp=QEIPositionGet(QEI0_BASE);
    if(dir_temp==1)                                 //CW direction
    {
        if(pos_temp!=0)
        {
            QEIPositionSet(QEI0_BASE, 0);           //Reset position
            pos_offset=(pos_temp*360.0)/(ENCODER_RESOLUTION*1.0);
            *Pt_Pos1 +=pos_offset;
        }
    }
    else if(dir_temp==-1)                           //CCW direction
    {
        if(pos_temp!=0)
        {
            QEIPositionSet(QEI0_BASE, 0);           //Reset position
            pos_offset=360.0-((pos_temp*360.0)/(ENCODER_RESOLUTION*1.0));
            *Pt_Pos1 -=pos_offset;
        }
    }
};

void Update_Velocity1(int16_t *Pt_Vel1)
{
    int8_t temp_dir=0;
    temp_dir=QEIDirectionGet(QEI1_BASE);
    *Pt_Vel1= (uint16_t)((QEIVelocityGet(QEI1_BASE)*60.0)/(1.0*ENCODER_RESOLUTION*SPEED_SAMPLE_PERIOD));
    *Pt_Vel1 *=temp_dir;
};

#endif /* USERLIBRARIES_ENCODER_C_ */
