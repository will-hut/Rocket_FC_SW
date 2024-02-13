/*
 * ADXL375.h
 *
 *  Created on: Feb 13, 2024
 *      Author: willh
 */

#ifndef INC_ADXL375_H_
#define INC_ADXL375_H_

#include "ADXL375_Definitions.h"


typedef struct{
	// peripheral config
	I2C_HandleTypeDef		I2C;

	uint8_t					TX_Buf[32]; // 32 is above max length i2c transaction
	uint8_t					RX_Buf[32];

	// return values
	int16_t					Acc_X_Raw;
	int16_t					Acc_Y_Raw;
	int16_t					Acc_Z_Raw;

	double					Acc_X_G;
	double					Acc_Y_G;
	double					Acc_Z_G;

} ADXL375;

void ADXL375_WriteReg(ADXL375 *, uint8_t, uint8_t);
uint8_t ADXL375_ReadReg(ADXL375 *, uint8_t);
uint8_t* ADXL375_ReadRegs(ADXL375 *, uint8_t, uint8_t);
void ADXL375_Init(ADXL375 *);
void ADXL375_ReadData(ADXL375 *);

#endif /* INC_ADXL375_H_ */
