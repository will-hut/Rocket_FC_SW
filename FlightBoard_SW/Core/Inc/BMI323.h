/*
 * BMI323.h
 *
 *  Created on: Feb 12, 2024
 *      Author: willh
 */

#ifndef INC_BMI323_H_
#define INC_BMI323_H_

#include "BMI323_Definitions.h"

typedef struct{
	// peripheral config
	SPI_HandleTypeDef		SPI;
	GPIO_TypeDef*			CS_Port;
	uint16_t				CS_Pin;

	uint8_t					TX_Buf[32]; // 32 is above max length spi transaction
	uint8_t					RX_Buf[32];
	uint16_t				RX_Buf_16[16]; // stores uint16_t's converted from RX_Buf

	// return values
	int16_t					Acc_X_Raw;
	int16_t					Acc_Y_Raw;
	int16_t					Acc_Z_Raw;
	int16_t					Gyro_X_Raw;
	int16_t					Gyro_Y_Raw;
	int16_t					Gyro_Z_Raw;

	double					Acc_X_G;
	double					Acc_Y_G;
	double					Acc_Z_G;
	double					Gyro_X_Deg_S;
	double					Gyro_Y_Deg_S;
	double					Gyro_Z_Deg_S;



} BMI323;

void BMI323_CSLow(BMI323*);
void BMI323_CSHigh(BMI323*);
void BMI323_WriteReg(BMI323 *, uint8_t, uint16_t);
uint16_t BMI323_ReadReg(BMI323 *, uint8_t);
uint16_t* BMI323_ReadRegs(BMI323 *, uint8_t, uint8_t);
void BMI323_Init(BMI323 *);
void BMI323_ReadData(BMI323 *);

#endif /* INC_BMI323_H_ */
