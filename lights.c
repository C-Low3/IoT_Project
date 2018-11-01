#include "msp.h"
#include "lights.h"

/*
 * light.c
 *
 *  Created on: Oct 27, 2018
 *      Author: Richard
 */

/****************************************************************
 * Initializes the RGB LED at a 125Hz PWM
 ****************************************************************/
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

    //Initializes switch for the lights toggle
    P3SEL0  &=~ BIT2;    //GPIO
    P3SEL1  &=~ BIT2;
    P3DIR   &=~ BIT2;     //Input
    P3REN   |=  BIT2;     //Enable internal Resistor
    P3OUT   |=  BIT2;     //Pull-Up Resistor

    P3->IE  |=  BIT2;     //Enable Interrupt
    P3->IES |=  BIT2;     //High to Low
    P3->IFG &=~ BIT2;     //Clear flag
}

/************************************************************************
 * Sets the PWM for the blue LED
 * @param duty cycle for the LED
 ************************************************************************/
void pwmBlue(uint8_t dc){
    if (dc < 100)
        TIMER_A0->CCR[1] = dc*240;
    else
        TIMER_A0->CCR[1] = 23999;
}

/************************************************************************
 * Sets the PWM for the red LED
 * @param duty cycle for the LED
 ************************************************************************/
void pwmRed(uint8_t dc){
    if (dc < 100)
        TIMER_A0->CCR[2] = dc*240;
    else
        TIMER_A0->CCR[2] = 23999;
}

/************************************************************************
 * Sets the PWM for the green LED
 * @param duty cycle for the LED
 ************************************************************************/
void pwmGreen(uint8_t dc){
    if (dc < 100)
        TIMER_A0->CCR[3] = dc*240;
    else
        TIMER_A0->CCR[3] = 23999;
}

/************************************************************************
 * Toggles the Lights
 * If the lights are on turns them off
 * If the lights are off turns them on to the most recent set duty cycle
 ************************************************************************/
void toggleLights(uint8_t isON, uint8_t rp,uint8_t bp,uint8_t gp){
    if (isON){
        TIMER_A0->CCR[1] = 0;
        TIMER_A0->CCR[2] = 0;
        TIMER_A0->CCR[3] = 0;
    }
    else{
        TIMER_A0->CCR[1] = bp*240;
        TIMER_A0->CCR[2] = rp*240;
        TIMER_A0->CCR[3] = gp*240;
    }
}



