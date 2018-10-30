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
 //           resetTimer32();

            delay_mS(10);              //secondary bounce

            if (pressed < 10){
                pulse = pulse * 10 + pressed;
            }

            else if (pressed == 11){
                pulse = pulse * 10 + 0;
            }
            else if (pressed == 12){
                printf("current pulse: %d \n",pulse % 100);
                displayPulse(pulse);
                return pulse % 100;
            }
            else{
                printf("Please enter only digit 0-9\n");
                invalidKey();
            }
            displayDC(pulse%100);
        }
    }
}





