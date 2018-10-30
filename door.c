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

    initAlarm();

    //Start Condition
    closeDoor();


}

/**
 * 7.6 and 7.7 are speakers
 */
void initAlarm(){
        //Initializes the pins for the alarm

        P2SEL0 |=  BIT4;    //set P2 timerA
        P2SEL1 &=~ BIT4;
        P2DIR  |= BIT4;      //Output
        TIMER_A1->CTL = 0;
        TIMER_A1->CCR[0] = 2000-1;                     //1.5KHz
        TIMER_A1->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;     //speaker 1
        TIMER_A1->CCR[1] = 500;
        TIMER_A1->CCTL[2] = TIMER_A_CCTLN_OUTMOD_7;     //speaker 2
        TIMER_A1->CCR[2] = 500;
        TIMER_A1->CTL = 0x0214;
}


void alarmSoundHigh(){
    TIMER_A0->CCR[0] = 2000-1; //1.5Khz
}

void alarmSoundLow(){
    TIMER_A0->CCR[0] = 2500-1; //1.2Khz
}

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
    TIMER_A2->CCR[4] = 3100;    //Slight stall at 3000 (calibration error)
    P6OUT  |=  0x20;
    P6OUT  &=~ 0x10;
}
