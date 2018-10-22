#include "msp.h"
#include "LCD.h"
#include "timers.h"

/*
 * LCD.c
 *Controls the LCD Screen
 *  Created on: Oct 12, 2018
 *      Author: Richard
 */

#define RS0 P1OUT &=~ BIT7
#define RS1 P1OUT |= BIT7
#define E0 P1OUT &=~ BIT6
#define E1 P1OUT |= BIT6

/******************************************************************************
 * LCD Display
 * Displays a message to  for 5 seconds
 * Then scrolls a message across the LCD
 * RS and E pins are #define
 * Authors: Richard Critchlow & Logan Dykstra
 * Version: October 2018
 *****************************************************************************/


/******************************************************************************
 * Initializes the LCD
 *****************************************************************************/
void initLCD(){
    //Set Port 5.4-5.7 to GPIO
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
    P5OUT &=~ 0xF0;             //clearing data/cmd pins
    P5OUT |= (nib & 0x0F)<<4;      //Placeing mask on pins
    pulseEnable();              //pulsing so LCD reads pins
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
void scroll(char word[],uint8_t line){
    uint8_t i=0;
    uint8_t c=0;         //Counter
    writeCommand(0x01);  //Clears the Screen
    writeCommand(0x0C);  //turns off Cursor


    //Prints the string repeatedly by removing the front most character
    while (c <= 15){
        writeCommand(0x80);
        for (i=c;i<16;i++){
            writeData(word[i]);
            delay_uS(100);
        }
        delay_mS(1000);
        c++;
    }
    //Prints the string by adding on character at the end and moving cursor
    while (c > 0){
        writeCommand(0x80 + c);
        for (i = 0; i < 16-c ; i++){
            writeData(word[i]);
            delay_uS(100);
        }
        delay_mS(1000);
        c--;
    }
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

void mainMenu(){
    display("      Menu      ",1);
    display(" 1 Door         ",2);
    display(" 2 Motor        ",3);
    display(" 3 Lights       ",4);
}







