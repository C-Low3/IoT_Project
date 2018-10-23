/*
 * myIoT_devices.h
 *
 *  Created on: Oct 14, 2018
 *      Author: Richard
 */

#ifndef MYIOT_DEVICES_H_
#define MYIOT_DEVICES_H_

void openDoor(void);
void closeDoor(void);

void runFan(uint16_t);
void stopFan(void);
void blueLED(uint8_t dc);
void greenLED(uint8_t dc);
void redLED(uint8_t dc);

void initDoorLEDs(void);
void initE_Stop();

#endif /* MYIOT_DEVICES_H_ */
