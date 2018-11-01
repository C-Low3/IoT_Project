/*
 * LCD.h
 *
 *
 *  Created on: Oct 12, 2018
 *      Author: Richard
 */

#ifndef LCD_H_
#define LCD_H_

void initLCD(void);
void pushNibble(uint8_t);
void pushByte(uint8_t);
void pulseEnable(void);
void writeCommand(uint8_t);
void writeData(uint8_t);


void display(char word[], uint8_t line); //displays a String[16]
void displayDC(uint8_t number);
void displayPulse(uint8_t pulse);
void displayPass(uint8_t pass);
void screenSaver();


void mainMenu(void); //displays main menu
void pinMenu(void);
void armMenu(void);
void smileMenu(void);
void doorMenu(void);
void motorPulseMenu(void);
void lightsMenu(void);
void lightsPulseMenu(void);
void passwordMenu(void);
void invalidKey(void);
void invalidPIN(void);


#endif /* LCD_H_ */
