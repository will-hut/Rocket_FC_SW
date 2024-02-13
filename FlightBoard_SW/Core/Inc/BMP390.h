/*
 * BMP390.h
 *
 *  Created on: Feb 12, 2024
 *      Author: willh
 */

#ifndef INC_BMP390_H_
#define INC_BMP390_H_

#include "BMP390_Definitions.h"


typedef struct{
	// peripheral config
	SPI_HandleTypeDef		SPI;
	HAL_StatusTypeDef		halReturn;
	GPIO_TypeDef*			CS_Port;
	uint16_t				CS_Pin;

	uint8_t					TX_Buf[32]; // 32 is above max length spi transaction
	uint8_t					RX_Buf[32];

	BMP390_Calib_Data		Calib_Data;
	BMP390_Quant_Calib_Data Quant_Calib_Data;

	// return values
	uint32_t				Pressure_Raw;
	uint32_t				Temp_Raw;
	double					Pressure_Pascal;
	double					Temp_C;
	double					Altitude_M;

} BMP390;

void BMP390_CSLow(BMP390*);
void BMP390_CSHigh(BMP390*);
void BMP390_WriteReg(BMP390 *, uint8_t, uint8_t);
uint8_t BMP390_ReadReg(BMP390 *, uint8_t);
uint8_t* BMP390_ReadRegs(BMP390 *, uint8_t, uint8_t);
void BMP390_ReadCal(BMP390 *);

uint32_t pow_bmp3(uint8_t, uint8_t);
void BMP390_Init(BMP390 *);

void BMP390_Quantize_Data(BMP390 *);
void BMP390_Compensate_Temp(BMP390 *);
void BMP390_Compensate_Pressure(BMP390 *);
void BMP390_ReadData(BMP390 *);


#endif /* INC_BMP390_H_ */
