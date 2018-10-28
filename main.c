#include "msp.h"
#include "timers.h"
#include "keypad.h"
#include "LCD.h"
#include "lights.h"
#include "fan.h"
#include "door.h"

void testAll(void);

void door(void);
void lights(void);
void motor(void);

/**
 * Authors: Richard Critchlow & Logan Dykstra
 * Version: October 2018
 */

 int main(void){
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	initSystick();      //Systic Timer
	initTimerA2_50Hz(); //Timer A2
	initKeypad();       //Keypad
	initLCD();          //LCD Display
	initLights();       //RGB Light
	initDoor();         //Servo
	initFan();          //Motor

    NVIC_EnableIRQ(PORT3_IRQn);
    __enable_interrupt();

    uint8_t pressed = 0;


	while(1){

	    mainMenu();
	    while (!(pressed = readKeypad()));
        if (pressed == 1){
            door();
        }
        else if (pressed == 2){
            lights();
        }
        else if (pressed == 3){
            motor();
        }
        else{
            invalidKey();
            delay_mS(3000);
        }



	}


}


 void door(){
     int p = 0;
     doorMenu();
     while (!(p = readKeypad()));
     if (p == 1 )
         openDoor();
     else if (p == 2)
         closeDoor();
     else{
         invalidKey();
         door();

     }

 }

 void lights(){

 }

 void motor(){

 }

/*****************************************************************
 * Emergency stop for the Motor
 */
 void PORT3_IRQHandler(){
     //if (P3->IFG & BIT7)  //dont believe this is neccessary
         TIMER_A2->CCR[3]  = 0;     //Stops Motor
     P3->IFG &=~ BIT7;          //Clear flag
 }

 void testAll(){
     mainMenu();
     runFan(50);
     openDoor();
     pwmBlue(50);
     delay_mS(2000);
     pwmRed(50);
     doorMenu();
     pwmBlue(0);
     closeDoor();
     delay_mS(2000);
     pwmGreen(90);
     pwmRed(0);
     lightsMenu();
     delay_mS(2000);
     pwmGreen(50);
     delay_mS(2000);
     pwmGreen(20);
     lightsMenu2();
     delay_mS(2000);
     pwmGreen(0);
 }
