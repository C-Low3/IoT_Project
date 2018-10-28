#include "msp.h"
#include "timers.h"
#include "door.h"

/*
 * door.c
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
void initDoor(){

    //Initializes the pins for the fan and the servo
    P6SEL0 |=  BIT7;    //set both pins to timerA
    P6SEL1 &=~ BIT7;
    P6DIR  |=  BIT7;      //Output
    TIMER_A2->CCTL[4] = TIMER_A_CCTLN_OUTMOD_7;     //Servo
    TIMER_A2->CCR[4] = 0;

    //LEDs for door
    P6SEL0 &=~ 0x30;   //GPIO
    P6SEL1 &=~ 0x30;
    P6DIR  |=  0x30;    //Output
    P6OUT  &=~ 0x30;

 //   initAlarm();


}

///**
// * 5.6 and 5.7 are speakers (A
// */
//void initAlarm(){
//    //Initializes the pins for the alarm
//    P2SEL0 |=  BIT4;    //set P2 timerA
//    P2SEL1 &=~ BIT4;
//    P2DIR  |= BIT4;      //Output
//    TIMER_A0->CTL = 0;
//    TIMER_A0->CCR[0] = 50000-1;                     //500Hz
//    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;     //Blue
//    TIMER_A0->CCR[1] = 0;
//    TIMER_A0->CTL = 0x0214;
//}
//
//
//void alarmSound(uint8_t dc){
//    TIMER_A0->CCR[1] = 500*dc -1;
//
//
//}

/***********************************************************************
 * TimerA in P6.7 (A2.4)  For Door (servo)
 * 20ms period where 9000 cycles is 180
 **********************************************************************/
void openDoor(){
    TIMER_A2->CCR[4] = 3000 + 3000;
    P6OUT  &=~ 0x20;
    P6OUT  |=  0x10;
}

/***********************************************************************
 * TimerA in P6.7 (A2.4)  For Door (servo)
 * 20ms period where 1ms is 0 degrees
 **********************************************************************/
void closeDoor(){
    TIMER_A2->CCR[4] = 3000;
    P6OUT  |=  0x20;
    P6OUT  &=~ 0x10;
}
