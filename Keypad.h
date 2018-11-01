
/*
 * Keypad.h
 *
 *  Created on: Oct 14, 2018
 *      Author: Richard
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

void initKeypad(void);
void printKeypad(uint8_t num);
uint8_t readKeypad(void);
uint16_t collectPulse(void);
uint16_t collectPIN(void);

#endif /* KEYPAD_H_ */
