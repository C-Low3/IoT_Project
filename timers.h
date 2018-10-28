/*
 * timers.h
 *
 *  Created on: Oct 14, 2018
 *      Author: Richard
 */

#ifndef TIMERS_H_
#define TIMERS_H_

void initSystick(void);
void delay_mS(uint16_t);
void delay_uS(uint16_t);
void initTimerA2_50Hz(void);

#endif /* TIMERS_H_ */
