/*
 * lights.h
 *
 *  Created on: Oct 27, 2018
 *      Author: Richard
 */

#ifndef LIGHTS_H_
#define LIGHTS_H_

void initLights(void);
void pwmRed(uint8_t);
void pwmBlue(uint8_t);
void pwmGreen(uint8_t);
void toggleLights(uint8_t, uint8_t, uint8_t, uint8_t);

#endif /* LIGHTS_H_ */
