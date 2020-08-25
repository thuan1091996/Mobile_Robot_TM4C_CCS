/* --------0.Project information--------------------
 * Mobile Robot
 * Author : TRAN MINH THUAN
 * Date: August, 21th 2019
 * Project associate with TM4C123, CCS version 9.
---------------------------------------------------*/


/* --------1.System requirement---------------------
 * Programming H-bridge 160W to control 2 motors
 * Build driver to control robot direction
 *
---------------------------------------------------*/



////// ----------------2. Pre-processor Directives Section--------------------///////
#include "UserLibraries/Userlibs.h"
#include "UserLibraries/Systick.h"
#include "UserLibraries/BridgeH.h"
#include "UserLibraries/Encoder.h"
#include "UserLibraries/UART.h"
#include "UserLibraries/Algorithm.h"
//////------------------------------------------------------------------------///////

////// ----------------3.Global Declarations Section--------------------------///////
uint8_t ui8Flag_FreshVel=0;      //Flag indicator that new velocity available
int8_t  i8Duty1=100;               //Duty cycle of Motor 1
int8_t  i8Duty2=0;               //Duty cycle of Motor 2
int16_t Vel_Motor1=0;            //Current velocity of motor 1
int16_t Ref_Vel1=100;            //Command velocity of motor 1
float   Pos_Motor1=0;            //Current position of motor 1s

float kp=0.45;
float ki=0.17;
//Tuning PID
// kp -  0.45
// ki -  0.17
//////------------------------------------------------------------------------///////

////// ----------------4. Subroutines Section---------------------------------///////
void main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_2_5| SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ); //80MHz
//    Systick_Init();
//    BridgeH_GPIO_Init();
//    BridgeH_PWM_Init();
//    QEI_Init();
//    QEI_VelocityInit();
//    UART_Init();
    SRFs_Init();
    IntMasterEnable();                      //Enable Interrupt
    while(1)
    {
        int i=0;
        int temp;
//        if(ui8Flag_FreshVel==1)
//        {
//            Update_Velocity1(&Vel_Motor1);
//            UART_SendSpeed(Vel_Motor1);
//            i8Duty2=PID_Controller(kp, ki, 0, Ref_Vel1-Vel_Motor1, Vel_Motor1);
//            Update_Speed(i8Duty1, i8Duty2);
//            ui8Flag_FreshVel=0;
//        }

//        Update_Position1(&Pos_Motor1);
//        Update_Vel1(&Vel_Motor1);
        temp=SRF_GetDistance(i);
        Delay_ms(10);
    };
}
//////------------------------------------------------------------------------///////
