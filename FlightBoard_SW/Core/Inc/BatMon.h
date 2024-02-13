/*
 * BatMon.h
 *
 *  Created on: Feb 13, 2024
 *      Author: willh
 */

#ifndef INC_BATMON_H_
#define INC_BATMON_H_

#include "main.h"

#define DIVIDER_VAL		(10.0/(10.0+22.0))
#define ADC_FULLSCALE	1024
#define ADC_VREF		3.3

typedef struct{
	// peripheral config
	ADC_HandleTypeDef		ADCHandle;

	// return values
	uint32_t				ADC_Raw;
	double					Voltage;

} BatMon;


void BatMon_Init(BatMon *);
void BatMon_ReadData(BatMon *);

#endif /* INC_BATMON_H_ */
