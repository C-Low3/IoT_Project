/*
 * door.h
 *
 *  Created on: Oct 27, 2018
 *      Author: Richard
 */

#ifndef DOOR_H_
#define DOOR_H_

void initDoor(void);
void openDoor(void);
void closeDoor(void);

void initAlarm(void);
void alarmSound(uint8_t);

#endif /* DOOR_H_ */
