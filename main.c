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
void pin(void);
void arm(void);
void checkFlags(void);

static uint16_t greenPulse, redPulse, bluePulse;
static uint8_t isArmed = 1;
static uint8_t pinNum = 0000;

volatile uint8_t light_flag;
volatile uint8_t ss_flag;
volatile uint8_t isON;
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
//    NVIC_EnableIRQ(PORT4_IRQn);
    __enable_irq();

    uint8_t pressed = 0;

	while(1){
	    mainMenu();
	    while (!(pressed = readKeypad())){
	        checkFlags();
	    }
        if (pressed == 1){
            door();
        }
        else if (pressed == 2){
            motor();
        }
        else if (pressed == 3){
            lights();
        }
        else if (pressed == 4){
            pin();
        }
        else if (pressed == 5){
            arm();
        }
        else if (pressed == 6){
            smileMenu();
            delay_mS(10000);
            screenSaver();
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
         while (!(pos = readKeypad())){
             checkFlags();
         }
         if (pos == 1)
             if (isArmed){
                 alarmSoundHigh();
             }
             else
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
    uint8_t bl = 0;
    while (1){
        lightsMenu();
        while (!(color = readKeypad())){
            checkFlags();
        }
        if (color == 10)
            return;
        else if (color > 3)
            invalidKey();
        else{
            lightsPulseMenu();
            bl = collectPulse();
            if (color == 1 ){
                redPulse = bl;
                pwmRed(redPulse);
                display("Red is at       ",1);
                displayPulse(bl);
                delay_mS(3000);
            }
            else if (color == 2){
                greenPulse = bl;
                pwmGreen(greenPulse);
                display("Green is at     ",1);
                displayPulse(bl);
                delay_mS(3000);
            }
            else if (color == 3){
                bluePulse = bl;
                pwmBlue(bluePulse);
                display("Blue is at      ",1);
                displayPulse(bl);
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

void pin(){
    display("Currently       ",1);
    display("Unavailable     ",2);
    display("please wait for ",3);
    display("upgrades        ",4);
    delay_mS(3000);
    mainMenu();

//    int newPin;
//    passwordMenu();
//    if (pinNum == collectPIN()){
//        pinMenu();
//        newPin = collectPIN();
//        pinNum = newPin;
//    }
//    else
//        invalidPIN();
}

void arm(){
    while (1){
        uint8_t pos = 0;
        armMenu();
        while (!(pos = readKeypad())){
            checkFlags();
        }
        if (pos == 1)
            if (isArmed){
                display("Already Armed   ", 1);
                display("                ",2);
                display("                ",3);
                display("                ",4);
                delay_mS(3000);
            }
            else{
                passwordMenu();
                if (pinNum == collectPIN()){
                    isArmed = 1;
                    alarmSoundLow();
                    delay_mS(250);
                    alarmSoundHigh();
                    delay_mS(250);
                    alarmSoundOff();
                }
            }
        else if (pos == 2)
            if (!isArmed){
                display("Already Disarmed", 1);
                display("                ",2);
                display("                ",3);
                display("                ",4);
                delay_mS(3000);
            }
            else{
                passwordMenu();
                if (pinNum == collectPIN()){
                    isArmed = 0;
                    alarmSoundHigh();
                    delay_mS(250);
                    alarmSoundLow();
                    delay_mS(250);
                    alarmSoundOff();
                }
                else
                    invalidPIN();
            }
        else if (pos == 10)
            return;
        else{
            invalidKey();
        }
    }
}


/*****************************************************************************
 * Checks flags to check if buttons were pressed
 ****************************************************************************/
void checkFlags(){
    if (ss_flag){
        screenSaver();
        ss_flag;            //clear flag for screen saver
        }
    if(light_flag){
        toggleLights(isON, redPulse, bluePulse, greenPulse);
        light_flag = 0;     //clear flag for lights
    }
}

/*****************************************************************
 * Emergency stop for the Motor                   P3.7
 * Toggles the flags for the lights on and off    P3.2
 *****************************************************************/
 void PORT3_IRQHandler(){
     delay_uS(10);               //bounce to prevent multiple interrupts
     if (P3->IFG & BIT7){        //Checks to see if it was 3.7 flag
         TIMER_A2->CCR[3]  = 0;  //Stops Motor
         P3OUT &=~ BIT0;         //PCB LEDs OFF
         P3->IFG &=~ BIT7;       //Clear flag
     }
     else if (P3->IFG & BIT2){
         light_flag = 1;        //shows that light switch was pressed
         isON = !isON;          //boolean to know to turn off or turn on lights
         P3->IFG &=~ BIT2;      //Clear flag
     }
     else
         P3->IFG &=~ 0xFF;
 }

// /****************************************************************
//  *
//  ***************************************************************/
// void PORT4_IRQHandler(){
//     mainMenu();
//     P4->IFG &=~ 0x7F;
// }

// /****************************************************************
//  * turns the flag for the screen saver on
//  ***************************************************************/
// void T32_INT1_IRQHandler(){
//     TIMER32_1->INTCLR = 0;
//     ss_flag = 1;
// }
