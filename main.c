
#include "msp.h"
#include "LCD.h"
#include "keypad.h"
#include "timers.h"
#include "myIoT_devices.h"




/**
 * Authors: Richard Critchlow & Logan Dykstra
 * Version: October 2018
 */


int main(void){
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	initKeypad();
	initSystick();
	initTimerA_PWM();

	int pressed;
	while (1){
	    if(pressed = readKeypad())
	        displayKeypad(pressed);

	    openDoor();
	    delay_mS(1000);
	    closeDoor();
	    delay_mS(3000);


	}


}
