#include "msp.h"

/*
 * myIoT_devices.c
 *
 *  Created on: Oct 14, 2018
 *      Author: Richard
 */







/**
 * TimerA in P6.7 (A2.4)  For Door (servo)
 * 20ms period where 9000 cycles is 180
 */
void openDoor(){
    TIMER_A2->CCR[4] = 3000 + 3000;
}

/**
 * TimerA in P6.7 (A2.4)  For Door (servo)
 * 20ms period where 1ms is 0 degrees
 */
void closeDoor(){
    TIMER_A2->CCR[4] = 3000;
}
