#include "msp.h"


/*
 * timers.c
 *
 * Sets up the Systick Timer and TimerA
 *
 *
 *  Created on: Oct 14, 2018
 *      Author: Richard
 */


/******************************************************************************
 * Initializes the Systick Timer
 *****************************************************************************/
void initSystick(){
    SysTick->CTRL = 0;
    SysTick->LOAD = 0x00FFFFFF;
    SysTick->VAL = 0;
    SysTick->CTRL = 0x00000005;
}

/******************************************************************************
 * Creates a delay using the Systick Timer
 * Param: msec - the number of milliseconds delayed
 *****************************************************************************/
void delay_mS(uint16_t msec){
    SysTick->LOAD = (msec * 3000)-1;
    SysTick->VAL = 0;
    while((SysTick->CTRL & 0x00010000)==0);
}

/******************************************************************************
 * Creates a delay using the Systick Timer
 * Param: usec - the number of microseconds delayed
 *****************************************************************************/
void delay_uS(uint16_t usec){
    SysTick->LOAD = (usec * 3)-1;
    SysTick->VAL = 0;
    while((SysTick->CTRL & 0x00010000)==0);
}

/************************************************************************************
 * Initilizes TimerA in P2.4 (A0.1)
 * If changing pin to timer Ax.y: Timer_Ax, CCTL[y], CCR[y]
 ***********************************************************************************/
void initTimerAPWMPin(){
    P2SEL0 |=  BIT4;    //TimerA(2.2)
    P2SEL1 &=~ BIT4;
    P2DIR |= BIT4;      //Output
    TIMER_A0->CTL = 0;
    TIMER_A0->CCR[0] = 60000-1;                     //20ms period
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;
    TIMER_A0->CCR[1] = 0;                       //initial 50% duty cycle
    TIMER_A0->CTL = 0x0214;
}

/************************************************************************************
 * PWM using timerA
 * Param: pulse number 0-100 representing power of motor in %
 ***********************************************************************************/
void timerA_PWM(uint16_t pulse, uint8_t subT){
    TIMER_A0->CCR[subT] = pulse*600;
}
