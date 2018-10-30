#include "msp.h"
#include "timers.h"
#include "keypad.h"
#include "LCD.h"
#include "lights.h"
#include "fan.h"
#include "door.h"

void door(void);
void lights(void);
void motor(void);

/*****************************************************************
 * Authors: Richard Critchlow & Logan Dykstra
 * Version: October 2018
 *****************************************************************/

 int main(void){
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	initSystick();      //Systic Timer
	initTimerA2_50Hz(); //Timer A2
//	initTimer32();
	initKeypad();       //Keypad
	initLCD();          //LCD Display
	initLights();       //RGB Light
	initDoor();         //Servo
	initFan();          //Motor

    NVIC_EnableIRQ(PORT3_IRQn);
    NVIC_EnableIRQ(T32_INT1_IRQn);
//    __enable_interrupt();

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
        }
	}
}


 void door(){
     while (1){
         uint8_t pos = 0;
         doorMenu();
         while (!(pos = readKeypad()));
         if (pos == 1 )
             openDoor();
         else if (pos == 2)
             closeDoor();
         else if (pos == 10)
             return;
         else{
             invalidKey();
         }
     }

 }

 void lights(){
    uint8_t color = 0;
    uint8_t dc = 0;
    while (1){

        lightsMenu();
        while (!(color = readKeypad()));
        if (color == 10)
            return;
        else if (color > 3)
            invalidKey();
        else{
            lightsPulseMenu();
            dc = collectPulse();
            if (color == 1 ){
                pwmRed(dc);
                display("Red is at       ",1);
                displayPulse(dc);
                delay_mS(3000);
            }
            else if (color == 2){
                pwmGreen(dc);
                display("Green is at     ",1);
                displayPulse(dc);
                delay_mS(3000);
            }
            else if (color == 3){
                pwmBlue(dc);
                display("Blue is at      ",1);
                displayPulse(dc);
                delay_mS(3000);
            }
        }
    }
}

void motor(){
    uint8_t dc = 0;

    motorPulseMenu();
    dc = collectPulse();
    runFan(dc);
    display("Motor is at     ", 1);
    displayPulse(dc);
    delay_mS(3000);
 }

/*****************************************************************
 * Emergency stop for the Motor
 */
 void PORT3_IRQHandler(){
     if (P3->IFG & BIT7)        //Checks to see if it was 3.7 flag
         TIMER_A2->CCR[3]  = 0; //Stops Motor
     P3->IFG &=~ BIT7;          //Clear flag
 }

 void T32_INT1_IRQHandler(){
     TIMER32_1->INTCLR = 0;
     screenSaver();            //get stuck in ss
 }
