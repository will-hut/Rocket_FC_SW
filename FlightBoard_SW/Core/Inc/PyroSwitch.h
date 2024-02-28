/*
 * PyroSwitch.h
 *
 *  Created on: Feb 13, 2024
 *      Author: willh
 */

#ifndef INC_PYROSWITCH_H_
#define INC_PYROSWITCH_H_

#include "main.h"


typedef struct{
	// peripheral config
	TIM_HandleTypeDef				Timer;

	GPIO_TypeDef*					SW1_Port;
	uint16_t						SW1_Pin;
	GPIO_TypeDef*					SW2_Port;
	uint16_t						SW2_Pin;
	GPIO_TypeDef*					SW3_Port;
	uint16_t						SW3_Pin;
	GPIO_TypeDef*					SW4_Port;
	uint16_t						SW4_Pin;

	GPIO_TypeDef*					ST1_Port;
	uint16_t						ST1_Pin;
	GPIO_TypeDef*					ST2_Port;
	uint16_t						ST2_Pin;
	GPIO_TypeDef*					ST3_Port;
	uint16_t						ST3_Pin;
	GPIO_TypeDef*					ST4_Port;
	uint16_t						ST4_Pin;



} PyroSwitch;

int PyroSwitch_Check1(PyroSwitch *);
int PyroSwitch_Check2(PyroSwitch *);
int PyroSwitch_Check3(PyroSwitch *);
int PyroSwitch_Check4(PyroSwitch *);

void PyroSwitch_Fire1(PyroSwitch *);
void PyroSwitch_Fire2(PyroSwitch *);
void PyroSwitch_Fire3(PyroSwitch *);
void PyroSwitch_Fire4(PyroSwitch *);

void PyroSwitch_HandleCallback(PyroSwitch *, TIM_HandleTypeDef *);

#endif /* INC_PYROSWITCH_H_ */
