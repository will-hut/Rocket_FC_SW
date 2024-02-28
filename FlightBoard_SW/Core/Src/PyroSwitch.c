/*
 * PyroSwitch.c
 *
 *  Created on: Feb 13, 2024
 *      Author: willh
 */

#include "PyroSwitch.h"

int PyroSwitch_Check1(PyroSwitch* SW){
	return HAL_GPIO_ReadPin(SW->SW1_Port, SW->SW1_Pin);
}

int PyroSwitch_Check2(PyroSwitch* SW){
	return HAL_GPIO_ReadPin(SW->SW2_Port, SW->SW2_Pin);
}

int PyroSwitch_Check3(PyroSwitch* SW){
	return HAL_GPIO_ReadPin(SW->SW3_Port, SW->SW3_Pin);
}

int PyroSwitch_Check4(PyroSwitch* SW){
	return HAL_GPIO_ReadPin(SW->SW4_Port, SW->SW4_Pin);
}

void PyroSwitch_Fire1(PyroSwitch* SW){
	HAL_GPIO_WritePin(SW->SW1_Port, SW->SW1_Pin, GPIO_PIN_SET);
	HAL_TIM_Base_Start_IT(&SW->Timer);
}

void PyroSwitch_Fire2(PyroSwitch* SW){
	HAL_GPIO_WritePin(SW->SW2_Port, SW->SW2_Pin, GPIO_PIN_SET);
	HAL_TIM_Base_Start_IT(&SW->Timer);
}

void PyroSwitch_Fire3(PyroSwitch* SW){
	HAL_GPIO_WritePin(SW->SW3_Port, SW->SW3_Pin, GPIO_PIN_SET);
	HAL_TIM_Base_Start_IT(&SW->Timer);
}

void PyroSwitch_Fire4(PyroSwitch* SW){
	HAL_GPIO_WritePin(SW->SW4_Port, SW->SW4_Pin, GPIO_PIN_SET);
	HAL_TIM_Base_Start_IT(&SW->Timer);
}




void PyroSwitch_HandleCallback(PyroSwitch* SW, TIM_HandleTypeDef *htim){
	// check if timer event is our timer
	if((&SW->Timer)->Instance == htim->Instance){
		HAL_GPIO_WritePin(SW->SW1_Port, SW->SW1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(SW->SW2_Port, SW->SW2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(SW->SW3_Port, SW->SW3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(SW->SW4_Port, SW->SW4_Pin, GPIO_PIN_RESET);
		HAL_TIM_Base_Stop(&SW->Timer);
	}
}
