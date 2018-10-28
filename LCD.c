#include "msp.h"
#include "LCD.h"
#include "timers.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * LCD.c
 *Controls the LCD Screen
 *  Created on: Oct 12, 2018
 *      Author: Richard
 */

#define RS0 P1OUT &=~ BIT7      //Any GPIO Pin
#define RS1 P1OUT |= BIT7
//#define RW0 P1OUT &=~ BIT4      //Any GPIO Pin
//#define RW1 P1OUT |= BIT4
#define E0 P1OUT &=~ BIT6       //Any GPIO Pin
#define E1 P1OUT |= BIT6
#define portout P5OUT           //Data pins D4-D7 to last 4 pins of port (GPIO)

/******************************************************************************
 * LCD Display
 * Displays a message to  for 5 seconds
 * Then scrolls a message across the LCD
 * To change pins just changes the #define and initLCD
 * Authors: Richard Critchlow & Logan Dykstra
 * Version: October 2018
 *****************************************************************************/


/******************************************************************************
 * Initializes the LCD
 * To change pins just changes the #define and initLCD
 *****************************************************************************/
void initLCD(){
    //Set Port 25.4-5.7 to GPIO
    P5SEL0 &=~ 0xF0;
    P5SEL1 &=~ 0xF0;
    //Set Port 1.6-1.7 to GPIO
    P1SEL0 &=~ 0xC0;
    P1SEL1 &=~ 0xC0;

    //Set ports to output low
    P5DIR |= 0xF0;
    P1DIR |= 0xC0;
    delay_uS(10);

    //initilization of LCD
    writeCommand(0x03);
    delay_mS(100);
    writeCommand(0x03);
    delay_uS(200);
    writeCommand(0x03);
    delay_mS(100);

    writeCommand(0x02);
    delay_uS(100);
    writeCommand(0x02);
    delay_uS(100);

    writeCommand(0x08);
    delay_uS(100);
    writeCommand(0x0F);
    delay_uS(100);
    writeCommand(0x01);
    delay_uS(100);
    writeCommand(0x06);
    delay_mS(100);
}
/******************************************************************************
 * Pulses the Enable Pin (Allows LCD to read pins)
 *****************************************************************************/
void pulseEnable(void){
    E0;
    delay_uS(10);
    E1;
    delay_uS(10);
    E0;
    delay_uS(10);
}

/******************************************************************************
 * Pushes a nibble to the LCD
 * Param: nib - nibble of bits to be sent to LCD
 *****************************************************************************/
void pushNibble(uint8_t nib){
    portout &=~ 0xF0;             //clearing data/cmd pins
    portout |= (nib & 0x0F)<<4;      //Placeing mask on pins
    pulseEnable();                //pulsing so LCD reads pins
}

/******************************************************************************
 * Pushes a full Byte of data to the LCD (when LCD is in nibble mode)
 * Param: byte - byte of data to be sent to LCD
 *****************************************************************************/
void pushByte(uint8_t byte){
    pushNibble((byte & 0xF0)>>4);   //MSB's
    pushNibble(byte & 0x0F);        //LSB's
    delay_uS(100);
}



/******************************************************************************
 * Writes data to the LCD (mostly used for character displays)
 * Param: data - usually character to be sent to LCD
 *****************************************************************************/
void writeData(uint8_t data){
    RS1;            //RS in data mode
    delay_uS(10);
    pushByte(data);
    delay_uS(10);
}

/******************************************************************************
 * Writes a command to the LCD (see data sheet for list of commands)
 * Param: cmd - cmd for the LCD
 *****************************************************************************/
void writeCommand(uint8_t cmd){
    RS0;            //RS in command mode
    delay_uS(10);
    pushByte(cmd);
    delay_uS(10);
}

/*****************************************************************************
 *Scrolls a size 16 string accross the top row of the LCD
 *Param: char[16] string to be scrolled
 ****************************************************************************/
void screenSaver(){

    char word[16] = "GVSU            ";
    uint8_t i=0;
    uint8_t c=0;
    uint8_t line = 0;

    writeCommand(1);  //Clears the Screen


    /* Intializes random number generator */
    //srand(time(NULL));

    line = rand() % 4;  //0 - 3
    c = rand() % 12 + 1;    //1-12

    printf("line: %d  c: %d\n",line,c);




    writeCommand(0x0C);  //turns off Curosr

    switch (line){       //moves cursor to the beginning of specified line
    case 0:
        line = 0x80; break;
    case 1:
        line = 0xC0; break;
    case 2:
        line = 0x90; break;
    case 3:
        line = 0xD0; break;
    default:
        break;
    }

    writeCommand(line + c);
    //prints word
        for (i=0;i<16;i++){
            writeData(word[i]);
            delay_uS(100);
        }
        delay_mS(1000);

}

/*****************************************************************************
 * Displays a message on the screen
 * Pram: word[] - 16 char message to be displayed
 *       line   - what line the message will be displayed on
 *****************************************************************************/
void display(char word[], uint8_t line){
    uint8_t i;
    writeCommand(0x0C);  //turns off Cursor
    switch (line){       //moves cursor to the beginning of specified line
    case 1:
        writeCommand(0x80); break;
    case 2:
        writeCommand(0xC0); break;
    case 3:
        writeCommand(0x90); break;
    case 4:
        writeCommand(0xD0); break;
    default:
        break;
    }

    //prints word
    for (i=0;i<16;i++){
        writeData(word[i]);
        delay_uS(100);
    }
}

void displayDC(uint8_t num){
    char text[16] = "                ";
    char sNum[2];
    sprintf(sNum, "%2d", num);
    strncpy(text,sNum, 2);
    display(text, 4);
}

void displayPulse(uint8_t pulse){
    char text[16] = "  % Power       ";
    char sNum[2];
    sprintf(sNum, "%2d", pulse);
    strncpy(text,sNum, 2);
    display(text, 2);
}

void mainMenu(){
    display("      Menu      ",1);
    display(" 1 Door         ",2);
    display(" 2 Lights       ",3);
    display(" 3 Motor        ",4);
}

void doorMenu(){
    display("    Select      ",1);
    display(" 1 Open         ",2);
    display(" 2 Close        ",3);
    display(" * Main Menu    ",4);
}
void motorPulseMenu(){
    display("Speed      0-99 ",1);
    display("Followed by '#' ",2);
    display("                ",3);
    display("                ",4);
}
void lightsMenu(){
    display("Color '*'to Exit",1);
    display(" 1 Red          ",2);
    display(" 2 Green        ",3);
    display(" 3 Blue         ",4);
}

void lightsPulseMenu(){
    display("Brightness 0-99 ",1);
    display("Followed by '#' ",2);
    display("                ",3);
    display("                ",4);
}

void invalidKey(){
    display("Invalid Key     ",1);
    display("                ",2);
    display("Please enter a  ",3);
    display("Valid Number    ",4);
    delay_mS(3000);
}





