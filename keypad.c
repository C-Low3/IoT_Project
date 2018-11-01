#include "msp.h"
#include "timers.h"
#include "keypad.h"
#include "LCD.h"
#include <stdio.h>

/***********************************************************************
 * keypad.c
 *
 *  Created on: Oct 14, 2018
 *      Author: Richard
 **********************************************************************/

/***********************************************************************
 * Initializes the Keypad
 * Rows are P4.0-4.3 Cols 4.4-4.6
 **********************************************************************/
void initKeypad(){
    //set Port 4 as GPIO
    P4SEL0 = 0x00;
    P4SEL1 = 0x00;

    //set all pins to inputs
    P4->DIR = 0x00;

    //set rows (4.0-4.3) with pull-up resistors
    P4->REN |= 0x0F;
    P4->OUT |= 0x0F;
}

/*********************************************************
 * Reads the keypad
 * stores the value of the key pressed into 'num'
 * returns 1 when key is pressed
 *********************************************************/
uint8_t readKeypad(){
    uint8_t col=0, row=0;
    int num = 0;

//    resetTimer32();

    for (col = 0; col < 3; col++){
        P4->DIR &= ~0x70;     //set all cols to input
        P4->DIR |= BIT(col+4);  //set current col to output
        P4->OUT &= ~BIT(col+4); //set current col to low

        delay_mS(10);            //bounce
        row = P4->IN & 0x0F;    //store row values

        while(!(P4IN & BIT0) | !(P4IN & BIT1) |
              !(P4IN & BIT2) | !(P4IN & BIT3));

        if(row != 0x0F)         //if one of the rows is low
            break;
    }

    P4->DIR &= ~0x70;  //returning all cols to inputs
    if (col >= 3)
        return 0;

    if (row == 0x0E)
        num = (col + 1);
    if (row == 0x0D)
        num = (3 + col + 1);
    if (row == 0x0B)
        num = (6 + col + 1);
    if (row == 0x07)
        num = (9 + col + 1);

    return num;
}

/*********************************************************
 * Displays the key pressed to the console
 * Used for Debugging
 *********************************************************/
void printKeypad(uint8_t num){
    if(num <= 9)
        printf("Output: %d\n",num);
    else if (num == 10)
        printf("Output: *\n");
    else if (num == 11)
        printf("Output: 0\n");
    else if (num == 12)
        printf("Output: #\n");
    else
        printf("unrecognized key");
}


/********************************************************************************
 * Collects a 3 digit pulse and returns it
 *******************************************************************************/
uint16_t collectPulse(){
    uint16_t pulse = 0;      //PIN
    uint8_t pressed = 0;  //key pressed

    while (1){
        if(pressed = readKeypad()){   //read number
            printKeypad(pressed);          //prints to console
//            resetTimer32();

            delay_mS(10);              //secondary bounce

            if (pressed < 10){
                pulse = pulse * 10 + pressed;
            }

            else if (pressed == 11){
                pulse = pulse * 10 + 0;
            }
            else if (pressed == 12){
                displayPulse(pulse);
                return pulse % 100;
            }
            else{
                invalidKey();
            }
            displayDC(pulse%100);
        }
    }
}


/************************************************************************
 * Collects a 4 digit pin and returns it
 ***********************************************************************/
uint16_t collectPIN(){
    uint16_t pin = 0;      //PIN
    uint16_t pressed = 0;  //key pressed
    uint8_t count = 0;     //flag

    while (1){
        if(pressed = readKeypad()){   //read number
//            resetTimer32();
            delay_mS(50);              //secondary bounce
            count++;

            if (pressed < 10){
                pin = pin * 10 + pressed;
            }
            else if (pressed == 10){
                display("Please enter    ",1);
                display("only digits 0-9 ",2);
                display("                ",3);
                display("                ",4);
                delay_mS(2000);
                count--;
            }
            else if (pressed == 11){
                pin = pin * 10 + 0;
                count++;
            }
            else if (pressed == 12){
                if (count < 5){
                    count = 0;
                    pressed = 0;
                    pin = 0;
                }
                else
                    return pin %= 10000;
            }
            displayPass(pin);
        }
    }
}




