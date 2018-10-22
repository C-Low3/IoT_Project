#include "msp.h"
#include "myIoT_devices.h"
#include "timers.h"

/*
 * myIoT_devices.c
 *
 *  Created on: Oct 14, 2018
 *      Author: Richard
 */








void blueLED(uint8_t dc){
        TIMER_A0->CCR[1] = 40000;//FIXME
}
void greenLED(uint8_t dc){
        TIMER_A0->CCR[2] = 40000;
}
void redLED(uint8_t dc){
        TIMER_A0->CCR[3] = 40000;
}


/***********************************************************************
 * Runs fan at the % duty cycle
 * Error catches CCR > period
 **********************************************************************/
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

/**
 * LE
 */
void initLEDs(){
    P6SEL0 &=~ 0x30;   //GPIO
    P6SEL1 &=~ 0x30;
    P6DIR  |=  0x30;    //Output
    P6OUT  &=~ 0x30;

}


/***********************************************************************
 * TimerA in P6.7 (A2.4)  For Door (servo)
 * 20ms period where 9000 cycles is 180
 **********************************************************************/
void openDoor(){
    TIMER_A2->CCR[4] = 3000 + 3000;
    P6OUT  &=~ 0x10;
    P6OUT  |=  0x20;


}

/***********************************************************************
 * TimerA in P6.7 (A2.4)  For Door (servo)
 * 20ms period where 1ms is 0 degrees
 **********************************************************************/
void closeDoor(){
    TIMER_A2->CCR[4] = 3000;
    P6OUT  |=  0x10;
    P6OUT  &=~ 0x20;
}
