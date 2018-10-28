#include "msp.h"
#include "timers.h"
#include "fan.h"


/*
 * fan.c
 *
 *  Created on: Oct 27, 2018
 *      Author: Richard
 */

/***********************************************************************
 * Initilizes
 *      TimerA in P6.6 (A2.3)  For Motor
 *      TimerA in P6.7 (A2.4)  For Door (servo)
 *      TimerA in P2.4 - 2.7   (A0.1-A0.4)  For RGB Lights
 *      Both run at 50Hz
 * If changing timer pin Ax.y: Timer_Ax, CCTL[y], CCR[y]
 ***********************************************************************/
void initFan(){

    //Initializes the pins for the fan
    P6SEL0 |=  BIT6;    //set both pins to timerA
    P6SEL1 &=~ BIT6;
    P6DIR  |= BIT6;      //Output
    TIMER_A2->CCTL[3] = TIMER_A_CCTLN_OUTMOD_7;     //Fan
    TIMER_A2->CCR[3] = 0;

    //Initializes switch for the emergency stop  //FIXME switch 1.4 to something else
    P3SEL0  &=~ BIT7;    //GPIO
    P3SEL1  &=~ BIT7;
    P3DIR   &=~ BIT7;     //Input
    P3REN   |=  BIT7;     //Enable internal Resistor
    P3OUT   |=  BIT7;     //Pull-Up Resistor
    P3->IES |=  BIT7;     //High to Low
    P3->IE  |=  BIT7;     //Enable Interrupt
    P3->IFG &=~ BIT7;     //Clear flag
}

/************************************************************************
 * Runs fan at the % duty cycle
 * Error catches CCR > period
 ***********************************************************************/
void runFan(uint16_t pulse){
    if (pulse < 100)
        TIMER_A2->CCR[3] = pulse*600;
    else
        TIMER_A2->CCR[3] = 60000;
}

/***********************************************************************
 * Shuts off fan
 **********************************************************************/
void stopFan(void){
    TIMER_A2->CCR[3]  = 0;
}
