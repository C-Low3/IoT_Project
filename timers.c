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
 * Initializes Timer A2 as a 50Hz Timer
 ***********************************************************************/
void initTimerA2_50Hz(){
    TIMER_A2->CTL = 0;
    TIMER_A2->CCR[0] = 60000-1;
    TIMER_A2->CTL = 0x0214;
}

/***********************************************************************
 * Timer A1 is used for a 1 minute interrupt.
 ***********************************************************************/
void initTimer32(){
    TIMER32_1->CONTROL = 0xC2;
    TIMER32_1->LOAD = 15000000-1;//10sec
    TIMER32_1->CONTROL |= 0x20;
}

/***********************************************************************
 * Restarts the timer32 Load Time
 ***********************************************************************/
void resetTimer32(){
    TIMER32_1->LOAD = 15000000-1;//10sec
}


