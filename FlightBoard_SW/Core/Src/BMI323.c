/*
 * BMI323.c
 *
 *  Created on: Feb 12, 2024
 *      Author: willh
 */

#include "BMI323.h"

// sets the CS pin low
void BMI323_CSLow(BMI323* BMI){
	HAL_GPIO_WritePin(BMI->CS_Port, (int)BMI->CS_Pin, GPIO_PIN_RESET);
}

// sets the CS pin high
void BMI323_CSHigh(BMI323* BMI){
	HAL_GPIO_WritePin(BMI->CS_Port, (int)BMI->CS_Pin, GPIO_PIN_SET);
}

// writes to a specified register
void BMI323_WriteReg(BMI323* BMI, uint8_t addr, uint16_t data){

	BMI->TX_Buf[0] = 0x7F & addr; // set bit 7 low for write
	BMI->TX_Buf[1] = data & 0xFF; // lower byte first
	BMI->TX_Buf[2] = (data >> 8) & 0xFF; // upper byte next

	BMI323_CSLow(BMI);
	HAL_SPI_Transmit(&BMI->SPI, BMI->TX_Buf, 3, 100);
	BMI323_CSHigh(BMI);
}

// reads a specified register, returning the value
uint16_t BMI323_ReadReg(BMI323* BMI, uint8_t addr){

	BMI->TX_Buf[0] = 0x80 | addr; // set bit 7 high for read
	BMI->TX_Buf[1] = 0x00; // dummy read byte

	BMI323_CSLow(BMI);
	HAL_SPI_TransmitReceive(&BMI->SPI, BMI->TX_Buf, BMI->RX_Buf, 4, 100);
	BMI323_CSHigh(BMI);

	uint16_t val = BMI3_CONCAT_BYTES(BMI->RX_Buf[3], BMI->RX_Buf[2]);

	return val; // third byte is returned data
}

// reads multiple registers specified by a start/length, returning a pointer to the values
// WARNING - do not read more than 15 registers at a time or the pointer will go out of bounds
uint16_t* BMI323_ReadRegs(BMI323* BMI, uint8_t addr, uint8_t len){ // returns pointer

	BMI->TX_Buf[0] = 0x80 | addr; // set bit 7 high for read
	BMI->TX_Buf[1] = 0x00; // dummy read byte

	BMI323_CSLow(BMI);
	HAL_SPI_TransmitReceive(&BMI->SPI, BMI->TX_Buf, BMI->RX_Buf, (len*2)+2, 100); // two bytes per value + 2 bytes overhead
	BMI323_CSHigh(BMI);

	// convert uint8_t's to uint16_t
	for(int i = 0; i < len; i++){
		BMI->RX_Buf_16[i] = BMI3_CONCAT_BYTES(BMI->RX_Buf[(i*2)+3], BMI->RX_Buf[(i*2)+2]);
	}

	return BMI->RX_Buf_16;
}

// sets up the config registers
void BMI323_Init(BMI323* BMI){

	BMI323_WriteReg(BMI, BMI3_REG_ACC_CONF, 0b0111000100111000);
	//   0111 : performance mode
	//   0001 : 2 samples avg (0000 for no avg)
	//   0011 : Bandwidth= odr/4 & +/- 16g range
	//   1009 : ODR= 100hz

	BMI323_WriteReg(BMI, BMI3_REG_GYR_CONF, 0b0111000101001000);
	//   0111 : performance mode
	//   0001 : 2 samples avg (0000 for no avg)
	//   0100 : Bandwidth= odr/2 & +/- 2kdps range
	//   1000 : ODR= 100hz

	// NOTE - if range is changed, update BMI3_LSB_MG and
	// BMI3_LSB_DEG_S in BMI323_Definitions.h

	BMI323_WriteReg(BMI, BMI3_REG_FIFO_CONF, 0b0000000000000000);
	//   Disable FIFO

	BMI323_WriteReg(BMI, BMI3_REG_IO_INT_CTRL, 0b0000000000000101);
	//   0000 : reserved
	//   0000 : default for INT2
	//   0000 : reserved
	//   0011 : enable, push-pull, active high

	BMI323_WriteReg(BMI, BMI3_REG_INT_MAP2, 0b0000010000000000);
	//   0000 : no fifo interrupts
	//   0100 : map accel data ready to INT1 (0101 for accel and gyro on INT1)
	//   0000 : no temp or error interrupts
	//   0000 : no i2c or tap feature interrupts

}

// reads accel and gyro data
void BMI323_ReadData(BMI323* BMI){

	uint16_t *reg_data = BMI323_ReadRegs(BMI, BMI3_REG_ACC_DATA_X, BMI3_LEN_ACCEL_GYRO_DATA);

	BMI->Acc_X_Raw = (int16_t)reg_data[0];
	BMI->Acc_Y_Raw = (int16_t)reg_data[1];
	BMI->Acc_Z_Raw = (int16_t)reg_data[2];
	BMI->Gyro_X_Raw = (int16_t)reg_data[3];
	BMI->Gyro_Y_Raw = (int16_t)reg_data[4];
	BMI->Gyro_Z_Raw = (int16_t)reg_data[5];

	BMI->Acc_X_G = (double)BMI->Acc_X_Raw / BMI3_LSB_MG / 1000.0;
	BMI->Acc_Y_G = (double)BMI->Acc_Y_Raw / BMI3_LSB_MG / 1000.0;
	BMI->Acc_Z_G = (double)BMI->Acc_Z_Raw / BMI3_LSB_MG / 1000.0;

	BMI->Gyro_X_Deg_S = (double)BMI->Gyro_X_Raw / BMI3_LSB_DEG_S;
	BMI->Gyro_Y_Deg_S = (double)BMI->Gyro_Y_Raw / BMI3_LSB_DEG_S;
	BMI->Gyro_Z_Deg_S = (double)BMI->Gyro_Z_Raw / BMI3_LSB_DEG_S;
}

