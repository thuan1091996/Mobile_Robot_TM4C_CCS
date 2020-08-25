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
#include "Userlibs.h"
#include "HC_SRF.h"
struct SRF
{
   uint32_t Trigger_Port;
   uint32_t Trigger_Pin;
   uint32_t Echo_Port;
   uint32_t Echo_Pin;
   uint32_t Int_Flag;
};
typedef struct 	SRF SRF_Type;
SRF_Type    		SRFs[NUMB_SRF];
extern int32_t 	i32Data_SRF;
extern uint8_t 	Running_SRF;
extern uint32_t SRF_Data[NUMB_SRF];

static uint32_t	Pulse_Width;
static double   temp = 1.0/80.0;
uint8_t Echo_wait=0;
uint8_t Newdata=0;

// *****************************************************************************
//
//! Initialize a SRF module.
//!  void    SRF_Init(  uint32_t ui32SRF_TriggerPort,
//!                     uint32_t ui32SRF_TriggerPin,
//!                     uint32_t ui32SRF_EchoPort,
//!                     uint32_t ui32SRF_EchoPin)
//! \param:  ui32SRF_TriggerPort    :Trigger GPIO Port.
//! \param:  ui32SRF_TriggerPin     :Trigger GPIO Pin.
//! \param:  ui32SRF_EchoPort       :Echo GPIO Port.
//! \param:  ui32SRF_EchoPin        :Echo GPIO Pin.
//! This function is used to initialize the GPIO for SRF module
//! - Trigger pin output
//! - Echo pin input with interrupt on both edges
//
//******************************************************************************
void SRF_Init(uint32_t ui32SRF_TriggerPort,
              uint32_t ui32SRF_TriggerPin,
              uint32_t ui32SRF_EchoPort,
              uint32_t ui32SRF_EchoPin)
{
    //Trigger pin - output
    GPIOPinTypeGPIOOutput(ui32SRF_TriggerPort, ui32SRF_TriggerPin);
    //Echo pin - input interrupt both edges
    GPIOPinTypeGPIOInput(ui32SRF_EchoPort, ui32SRF_EchoPin);
    GPIOIntTypeSet(ui32SRF_EchoPort, ui32SRF_EchoPin,GPIO_BOTH_EDGES);
}

// *****************************************************************************
//
//! Initialize SRF modules.
//! void SRF_Init(void)
//! This function is used to initialize the SRF modules
//! Assign interrupt handler for SRF Echo pins
//! Setup a timer to calculate the distance
//
//******************************************************************************
void SRFs_Init(void)
{
    // Assign value to the SRFs
    uint8_t count;
    SRFs[0].Trigger_Port=TRIGGER0_PORT;
    SRFs[1].Trigger_Port=TRIGGER1_PORT;
    SRFs[2].Trigger_Port=TRIGGER2_PORT;
    SRFs[3].Trigger_Port=TRIGGER3_PORT;
    SRFs[4].Trigger_Port=TRIGGER4_PORT;

    SRFs[0].Trigger_Pin=TRIGGER0_PIN;
    SRFs[1].Trigger_Pin=TRIGGER1_PIN;
    SRFs[2].Trigger_Pin=TRIGGER2_PIN;
    SRFs[3].Trigger_Pin=TRIGGER3_PIN;
    SRFs[4].Trigger_Pin=TRIGGER4_PIN;

    SRFs[0].Echo_Port=ECHO0_PORT;
    SRFs[1].Echo_Port=ECHO1_PORT;
    SRFs[2].Echo_Port=ECHO2_PORT;
    SRFs[3].Echo_Port=ECHO3_PORT;
    SRFs[4].Echo_Port=ECHO4_PORT;

    SRFs[0].Echo_Pin=ECHO0_PIN;
    SRFs[1].Echo_Pin=ECHO1_PIN;
    SRFs[2].Echo_Pin=ECHO2_PIN;
    SRFs[3].Echo_Pin=ECHO3_PIN;
    SRFs[4].Echo_Pin=ECHO4_PIN;

	SRFs[0].Int_Flag=SRF0_INTFLAG;
	SRFs[1].Int_Flag=SRF1_INTFLAG;
	SRFs[2].Int_Flag=SRF2_INTFLAG;
	SRFs[3].Int_Flag=SRF3_INTFLAG;
	SRFs[4].Int_Flag=SRF4_INTFLAG;

    // Initialize for the GPIOs
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB));
    for(count=0;count<NUMB_SRF;count++)
    {
        SRF_Init(SRFs[count].Trigger_Port,										//Init the GPIO for SRF pins
                 SRFs[count].Trigger_Pin,
                 SRFs[count].Echo_Port,
                 SRFs[count].Echo_Pin);
        GPIOPinWrite(SRFs[count].Trigger_Port, SRFs[count].Trigger_Pin, 0);	    //Assign 0 as default value of trigger pins
    }
    //Assign Interrupt handler for Echo pins
    GPIOIntRegister(ECHO_PORT,SRF_Handler);
    IntPrioritySet(INT_GPIOA_TM4C123, 3);                                       //Set 3<-Priority of all echo pins (PortA)

    //Timer periodic up full width (32-bit)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER2));
    TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC_UP);
    TimerEnable(TIMER2_BASE,TIMER_A);
}

// *****************************************************************************
//
//! Get distances data from SRF
//! uint32_t  SRF_GetDistance(void);
//! This function is used to initialize the SRF modules
//! Assign interrupt handler for SRF Echo pins
//! Setup a timer to calculate the distance
//
//******************************************************************************
uint32_t  SRF_GetDistance(uint8_t ui8SRF)
{
		uint32_t distance=0;
		if(Echo_wait==0)                //If not waiting for any SRF
		{
            GPIOIntEnable(SRFs[ui8SRF].Echo_Port, SRFs[ui8SRF].Int_Flag);
            SysCtlDelay(266);           //9.975us, very fast
            Running_SRF=ui8SRF;         //Pass this to SRF_Handler
            GPIOPinWrite(SRFs[ui8SRF].Trigger_Port,SRFs[ui8SRF].Trigger_Pin,SRFs[ui8SRF].Trigger_Pin);
            SysCtlDelay(266);   		//9.975us, very fast
            GPIOPinWrite(SRFs[ui8SRF].Trigger_Port, SRFs[ui8SRF].Trigger_Pin, 0);
            SysCtlDelay(1064000);       //40ms very fast

//            while(Echo_wait==1);
            if(Newdata==1)
            {
                if(Pulse_Width<1000000) //filter
                {
                    distance= (uint32_t) (temp*Pulse_Width);
                    distance/=58;
                    Newdata=0;
                    SRF_Data[Running_SRF]=distance;
                    return distance;
                }
            }
		}
		return distance;
}

void SRF_Handler(void)
{
		//
		//	Acknowledge the interrupt
		//
        static uint8_t valid=0;
        uint8_t echo_input=GPIOPinRead(SRFs[Running_SRF].Echo_Port, SRFs[Running_SRF].Echo_Pin);
        PORTA_IC_R|=0x7C;           //Clear all echo pins interrupt requests
//        GPIOIntClear(ECHO_PORT, SRFs[Running_SRF].Int_Flag);
	    if( echo_input == SRFs[Running_SRF].Echo_Pin)
        {
	        TIMER_VALUE=0;                             														      //Reset timer
            TimerEnable(TIMER2_BASE, TIMER_A);
            Echo_wait=1;
            valid=1;
	    }
	    if( (echo_input != SRFs[Running_SRF].Echo_Pin) && (valid==1) )
	    {
            Pulse_Width=TIMER_VALUE;
            TimerDisable(TIMER2_BASE, TIMER_A);
            Newdata=1;
            Echo_wait=0;
            valid=0;
            GPIOIntDisable(SRFs[Running_SRF].Echo_Port, SRFs[Running_SRF].Int_Flag);
	    }
}

