#include "msp.h"
#include "lights.h"

/*
 * light.c
 *
 *  Created on: Oct 27, 2018
 *      Author: Richard
 */


void initLights(){

    //Initializes the Pins for the RGB Light
    P2SEL0 |=  0xF0;    //set P2 timerA
    P2SEL1 &=~ 0xF0;
    P2DIR  |= 0xF0;      //Output
    TIMER_A0->CTL = 0;
    TIMER_A0->CCR[0] = 24000-1;                     //125Hz
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;     //Blue
    TIMER_A0->CCR[1] = 0;
    TIMER_A0->CCTL[2] = TIMER_A_CCTLN_OUTMOD_7;     //Green
    TIMER_A0->CCR[2] = 0;
    TIMER_A0->CCTL[3] = TIMER_A_CCTLN_OUTMOD_7;     //Red
    TIMER_A0->CCR[3] = 0;
    TIMER_A0->CTL = 0x0214;
}


void pwmBlue(uint8_t dc){
    if (dc < 100)
        TIMER_A0->CCR[1] = dc*240;
    else
        TIMER_A0->CCR[1] = 23999;
}
void pwmRed(uint8_t dc){
    if (dc < 100)
        TIMER_A0->CCR[2] = dc*240;
    else
        TIMER_A0->CCR[2] = 23999;
}
void pwmGreen(uint8_t dc){
    if (dc < 100)
        TIMER_A0->CCR[3] = dc*240;
    else
        TIMER_A0->CCR[3] = 23999;
}



