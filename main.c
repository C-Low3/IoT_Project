
#include <keypad.h>
#include "msp.h"
#include "LCD.h"
#include "timers.h"
#include "myIoT_devices.h"




/**
 * Authors: Richard Critchlow & Logan Dykstra
 * Version: October 2018
 */


int main(void){
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	initSystick();      //Systic Timer
	initKeypad();       //Keypad
	initLCD();          //LCD Display
	initTimerA_PWM();   //Fan and Servo
	initDoorLEDs();     //Door LEDs
	initE_Stop();       //Emergency stop for fan

	NVIC_EnableIRQ(PORT3_IRQn);
	__enable_interrupt();

	runFan(50);
	while (1){

	    mainMenu();
	    if (P3IN & BIT7)
	        printf("high\n");
	    else
	        printf("low\n");

	}

}

void PORT3_IRQHandler(){
     if (P3->IFG &=~ BIT7){
//       // TIMER_A2->CCR[3] = 1; //no pulse for fan
        printf("trigered\n");
      }
      P3->IFG &=~ BIT7;
      printf("cleared\n");
}
