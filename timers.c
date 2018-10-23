#include "msp.h"
#include "timers.h"

/***********************************************************************
 * timers.c
 *
 * Sets up the Systick Timer and TimerA
 *  Created on: Oct 14, 2018
 *      Author: Richard
 ***********************************************************************/


/***********************************************************************
 * Initializes the Systick Timer
 ***********************************************************************/
void initSystick(){
    SysTick->CTRL = 0;
    SysTick->LOAD = 0x00FFFFFF;
    SysTick->VAL = 0;
    SysTick->CTRL = 0x00000005;
}

/***********************************************************************
 * Creates a delay using the Systick Timer
 * Param: msec - the number of milliseconds delayed
 ***********************************************************************/
void delay_mS(uint16_t msec){
    SysTick->LOAD = (msec * 3000)-1;
    SysTick->VAL = 0;
    while((SysTick->CTRL & 0x00010000)==0);
}

/***********************************************************************
 * Creates a delay using the Systick Timer
 * Param: usec - the number of microseconds delayed
 ***********************************************************************/
void delay_uS(uint16_t usec){
    SysTick->LOAD = (usec * 3)-1;
    SysTick->VAL = 0;
    while((SysTick->CTRL & 0x00010000)==0);
}

/***********************************************************************
 * Initilizes
 *      TimerA in P6.6 (A2.3)  For Motor
 *      TimerA in P6.7 (A2.4)  For Door (servo)
 *      TimerA in P2.4 - 2.7   (A0.1-A0.4)  For RGB Lights
 *      Both run at 50Hz
 * If changing timer pin Ax.y: Timer_Ax, CCTL[y], CCR[y]
 ***********************************************************************/
void initTimerA_PWM(){

    //Initializes the pins for the fan and the servo
    P6SEL0 |=  0xC0;    //set both pins to timerA
    P6SEL1 &=~ 0xC0;
    P6DIR  |= 0xC0;      //Output
    TIMER_A2->CTL = 0;
    TIMER_A2->CCR[0] = 60000-1;                     //20ms period
    TIMER_A2->CCTL[3] = TIMER_A_CCTLN_OUTMOD_7;     //Fan
    TIMER_A2->CCR[3] = 0;
    TIMER_A2->CCTL[4] = TIMER_A_CCTLN_OUTMOD_7;     //Servo
    TIMER_A2->CCR[4] = 0;
    TIMER_A2->CTL = 0x0214;

    //Initializes the Pins for the RGB Light
    P2SEL0 |=  0xF0;    //set P2 timerA
    P2SEL1 &=~ 0xF0;
    P2DIR  |= 0xF0;      //Output
    TIMER_A0->CTL = 0;
    TIMER_A0->CCR[0] = 60000-1;                     //FIXME needs to be calculated somehow ug.
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;     //Blue
    TIMER_A0->CCR[1] = 0;
    TIMER_A0->CCTL[2] = TIMER_A_CCTLN_OUTMOD_7;     //Green
    TIMER_A0->CCR[2] = 0;
    TIMER_A0->CCTL[3] = TIMER_A_CCTLN_OUTMOD_7;     //Red
    TIMER_A0->CCR[3] = 0;
    TIMER_A0->CTL = 0x0214;
}

/***********************************************************************
 * PWM using timerA
 * Param: pulse number 0-100 representing power of motor in %
 ***********************************************************************/
void timerA_PWM(uint16_t pulse, uint8_t subT){
    TIMER_A2->CCR[subT] = pulse*60 + 3000;
}
