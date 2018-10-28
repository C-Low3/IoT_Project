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
void scroll(char word[], uint8_t line);  //Scrolls a String[16]


void mainMenu(void); //displays main menu
void doorMenu(void);
void motorMenu(void);
void lightsMenu(void);
void lightsMenu2(void);
void invalidKey(void);


#endif /* LCD_H_ */
