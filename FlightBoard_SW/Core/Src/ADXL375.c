/*
 * ADXL375.c
 *
 *  Created on: Feb 13, 2024
 *      Author: willh
 */

#include "ADXL375.h"


void ADXL375_WriteReg(ADXL375 * ADXL, uint8_t addr, uint8_t data){
	ADXL->TX_Buf[0] = data;
	HAL_I2C_Mem_Write(&ADXL->I2C, (ADXL3XX_ADDRESS << 1), addr, I2C_MEMADD_SIZE_8BIT, ADXL->TX_Buf, 1, 100);
}

uint8_t ADXL375_ReadReg(ADXL375 * ADXL, uint8_t addr){
	HAL_I2C_Mem_Read(&ADXL->I2C, (ADXL3XX_ADDRESS << 1), addr, I2C_MEMADD_SIZE_8BIT, ADXL->RX_Buf, 1, 100);
	return ADXL->RX_Buf[0];
}

uint8_t* ADXL375_ReadRegs(ADXL375 * ADXL, uint8_t addr, uint8_t len){
	HAL_I2C_Mem_Read(&ADXL->I2C, (ADXL3XX_ADDRESS << 1), addr, I2C_MEMADD_SIZE_8BIT, ADXL->RX_Buf, len, 100);
	return ADXL->RX_Buf;
}


void ADXL375_Init(ADXL375 * ADXL){
	ADXL375_WriteReg(ADXL, ADXL3XX_REG_BW_RATE, 0b00001010);
	// 000 - reserved
	// 0 - normal power mode
	// 1010 - 100hz

	ADXL375_WriteReg(ADXL, ADXL3XX_REG_INT_ENABLE, 0b10000000);
	// 1 - enable data ready interrupt
	// 0000000 - disable all other interupts

	ADXL375_WriteReg(ADXL, ADXL3XX_REG_DATA_FORMAT, 0b00001011);
	// 0 - no self test
	// 0 - 4 wire SPI
	// 0 - interrupt active high
	// 01 - reserved
	// 0 - right justified data
	// 11 - reserved

	ADXL375_WriteReg(ADXL, ADXL3XX_REG_POWER_CTL, 0b00001000);
	// 00 -reserved
	// 0 - no activity/inactivity function
	// 0 - no autosleep
	// 1 - enable measurements
	// 0 - disable sleep
	// 00 - no wakeup mode
}

void ADXL375_ReadData(ADXL375 * ADXL){
	uint8_t* regData = ADXL375_ReadRegs(ADXL, ADXL3XX_REG_DATAX0, ADXL3XX_DATA_LEN);

	ADXL->Acc_X_Raw = (int16_t)CONCAT_BYTES(regData[1], regData[0]);
	ADXL->Acc_Y_Raw = (int16_t)CONCAT_BYTES(regData[3], regData[2]);
	ADXL->Acc_Z_Raw = (int16_t)CONCAT_BYTES(regData[5], regData[4]);

	ADXL->Acc_X_G = ADXL->Acc_X_Raw*ADXL3XX_ACC_SCALE_FACTOR;
	ADXL->Acc_Y_G = ADXL->Acc_Y_Raw*ADXL3XX_ACC_SCALE_FACTOR;
	ADXL->Acc_Z_G = ADXL->Acc_Z_Raw*ADXL3XX_ACC_SCALE_FACTOR;
}
