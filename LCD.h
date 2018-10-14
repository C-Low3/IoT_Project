/*
 * LCD.h
 *
 *
 *  Created on: Oct 12, 2018
 *      Author: Richard
 */

#ifndef LCD_H_
#define LCD_H_

void mainMenu(void); //displays main menu
void display(uint8_t String, uint8_t line); //displays a String[16]
void scroll(uint8_t String, uint8_t line);  //Scrolls a String[16]



#endif /* LCD_H_ */
