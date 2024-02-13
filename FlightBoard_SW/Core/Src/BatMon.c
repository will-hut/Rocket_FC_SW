/*
 * BatMon.c
 *
 *  Created on: Feb 13, 2024
 *      Author: willh
 */

#include "BatMon.h"

void BatMon_Init(BatMon * Bat){
	HAL_ADC_Start(&Bat->ADCHandle);
}


void BatMon_ReadData(BatMon* Bat){
	HAL_ADC_PollForConversion(&Bat->ADCHandle, 100);
	Bat->ADC_Raw = HAL_ADC_GetValue(&Bat->ADCHandle);
	Bat->Voltage = ((double)Bat->ADC_Raw/ADC_FULLSCALE)*ADC_VREF/DIVIDER_VAL;
}
