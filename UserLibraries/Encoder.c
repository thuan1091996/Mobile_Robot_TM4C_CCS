/****************************************************************************
* Title                 :   QEI encoder driver
* ProductLink           :   tdhshop.com.vn/dong-co-dc-servo-giam-toc-ga37
* Filename              :   Encoder.c
* Author                :   ItachiThuan
* Origin Date           :   Jul 8, 2019
* Version               :   1.0.1
* Target                :   TM4C123 with CCS IDE
* Notes                 :
        |  PhA  |  PhB
 -------+-------+--------
QEI0    |  PD6  |  PD7
QEI1    |  PC5  |  PC6
*****************************************************************************/

#ifndef USERLIBRARIES_ENCODER_C_
#define USERLIBRARIES_ENCODER_C_

/******************************************************************************
* Includes
*******************************************************************************/
#include "Userlibs.h"
#include "Encoder.h"
#include "UART.h"

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
int32_t i32Data_Velocity0=0;                    //Data Velocity Motor0 Semaphore
int32_t i32Data_Velocity1=0;                    //Data Velocity Motor1 Semaphore
extern uint8_t ui8Flag_FreshVel0;
extern uint8_t ui8Flag_FreshVel1;
/******************************************************************************
* Function Definitions
*******************************************************************************/

// *******************************************************************************
//
//! QEI0 Timer Expire INT Handler
//! When QEI's timer expires, Signal semaphore to indicate there is new speed data
//
//********************************************************************************
void QEI0_INTHandler(void)
{
    QEIIntClear(QEI0_BASE, QEI_INTTIMER);       //acknowledge interrupt
    ui8Flag_FreshVel0=1;
//        OS_Signal(&i32Data_Velocity0);
};

// *******************************************************************************
//
//! QEI1 Timer Expire INT Handler
//! When QEI's timer expires, Signal semaphore to indicate there is new speed data
//
//********************************************************************************
void QEI1_INTHandler(void)
{
    QEIIntClear(QEI1_BASE, QEI_INTTIMER);       //acknowledge interrupt
    ui8Flag_FreshVel1=1;
//        OS_Signal(&i32Data_Velocity1);
};



// *****************************************************************************
//
//! QEI Initialization.
//! QEI Quadrature mode, no swap, no index, no Filter
//! QEI Velocity Initialization with Timer clock = System clock
//! Setting for QEI Timer Expire interrupt to interrupt every period
//! \param:  ui32Resolution0     :Rising/falling edges per motor revolution
//! \param:  ui32Resolution1     :Rising/falling edges per motor revolution
//! \param:  ui8Priority0        :QEI Int handler priority
//! \param:  ui8Priority1        :QEI Int handler priority
//
//******************************************************************************
void QEI_Init(uint8_t ui8Priority0, uint8_t ui8Priority1)
{
    //Enable peripherals
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI1);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC));
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD));
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_QEI0));
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_QEI1));

    //Unlock PD7
    HWREG(GPIO_PORTD_BASE+GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTD_BASE+GPIO_O_CR) |= GPIO_PIN_7;

    //GPIO Configure for QEI
    GPIOPinTypeQEI(GPIO_PORTC_BASE, GPIO_PIN_5);
    GPIOPinTypeQEI(GPIO_PORTC_BASE, GPIO_PIN_6);
    GPIOPinTypeQEI(GPIO_PORTD_BASE, GPIO_PIN_6);
    GPIOPinTypeQEI(GPIO_PORTD_BASE, GPIO_PIN_7);
    GPIOPinConfigure(GPIO_PC5_PHA1);
    GPIOPinConfigure(GPIO_PC6_PHB1);
    GPIOPinConfigure(GPIO_PD6_PHA0);
    GPIOPinConfigure(GPIO_PD7_PHB0);

    //QEI Configure
    QEIDisable(QEI0_BASE);  //First disable  to configure
    QEIDisable(QEI1_BASE);
    QEIIntDisable(QEI0_BASE, (QEI_INTERROR | QEI_INTDIR | QEI_INTTIMER | QEI_INTINDEX));
    QEIIntDisable(QEI1_BASE, (QEI_INTERROR | QEI_INTDIR | QEI_INTTIMER | QEI_INTINDEX));
    QEIVelocityDisable(QEI0_BASE);
    QEIVelocityDisable(QEI1_BASE);

    // instruction below *100 because in case Sample Time >1s, QEIMAXPOS reg may not overload because there more than 1324 pulses /sec
    // no need cuz QEI_COUNT_R and QEI_MAXPOS work independently
    QEIConfigure(QEI0_BASE, QEI_CONFIG_CAPTURE_A_B|QEI_CONFIG_NO_RESET|QEI_CONFIG_QUADRATURE|QEI_CONFIG_NO_SWAP, ENCODER0_RESOLUTION-1);
    QEIConfigure(QEI1_BASE, QEI_CONFIG_CAPTURE_A_B|QEI_CONFIG_NO_RESET|QEI_CONFIG_QUADRATURE|QEI_CONFIG_NO_SWAP, ENCODER1_RESOLUTION-1);

    QEIVelocityConfigure(QEI0_BASE, QEI_VELDIV_1, (SysCtlClockGet()*SPEED_SAMPLE_PERIOD));
    QEIVelocityConfigure(QEI1_BASE, QEI_VELDIV_1, (SysCtlClockGet()*SPEED_SAMPLE_PERIOD));

    //Interrupt configure
    QEIIntRegister(QEI0_BASE, QEI0_INTHandler);
    QEIIntRegister(QEI1_BASE, QEI1_INTHandler);

    IntPrioritySet(INT_QEI0_TM4C123, ui8Priority0);
    IntPrioritySet(INT_QEI1_TM4C123, ui8Priority1);

    QEIIntEnable(QEI0_BASE, QEI_INTTIMER);  //Velocity timer expires interrupt enable
    QEIIntEnable(QEI1_BASE, QEI_INTTIMER);  //Velocity timer expires interrupt enable

    QEIPositionSet(QEI0_BASE, 0);       //Reset counter
    QEIPositionSet(QEI1_BASE, 0);

//    Filter
//    QEIFilterConfigure(QEI0_BASE, QEI_FILTCNT_7);
//    QEIFilterEnable(QEI0_BASE);

    QEIVelocityEnable(QEI0_BASE);
    QEIVelocityEnable(QEI1_BASE);

    QEIEnable(QEI0_BASE);
    QEIEnable(QEI1_BASE);
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
            pos_offset=(pos_temp*360.0)/(ENCODER0_RESOLUTION*1.0);
            *Pt_Pos1 +=pos_offset;
        }
    }
    else if(dir_temp==-1)                           //CCW direction
    {
        if(pos_temp!=0)
        {
            QEIPositionSet(QEI0_BASE, 0);           //Reset position
            pos_offset=360.0-((pos_temp*360.0)/(ENCODER1_RESOLUTION*1.0));
            *Pt_Pos1 -=pos_offset;
        }
    }
};


// *****************************************************************************
//
//! Update Velocity of encoder 0
//
//******************************************************************************
void Update_Velocity0(int16_t *Pt_Vel0)
{
    int8_t temp_dir=0;
    temp_dir=QEIDirectionGet(QEI0_BASE);
    *Pt_Vel0= (uint16_t)((QEIVelocityGet(QEI0_BASE)*60.0)/(1.0*ENCODER0_RESOLUTION*SPEED_SAMPLE_PERIOD));
    *Pt_Vel0 *=temp_dir;
};

// *****************************************************************************
//
//! Update Velocity of encoder 1
//
//******************************************************************************
void Update_Velocity1(int16_t *Pt_Vel1)
{
    int8_t temp_dir=0;
    temp_dir=QEIDirectionGet(QEI1_BASE);
    *Pt_Vel1= (uint16_t)((QEIVelocityGet(QEI1_BASE)*60.0)/(1.0*ENCODER1_RESOLUTION*SPEED_SAMPLE_PERIOD));
    *Pt_Vel1 *=temp_dir;
};

#endif /* USERLIBRARIES_ENCODER_C_ */
