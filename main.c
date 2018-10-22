
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

	initSystick();
	initKeypad();
	initLCD();
	initTimerA_PWM();   //Fan and Servo
	initLEDs();


	while (1){


	    mainMenu();
	    openDoor();
	    delay_mS(3000);
	    closeDoor();
	    delay_mS(3000);

	}


}
