/*
 * BMP390.c
 *
 *  Created on: Feb 12, 2024
 *      Author: willh
 */

#include "BMP390.h"
#include <math.h>


// sets the CS pin low
void BMP390_CSLow(BMP390* BMP){
	HAL_GPIO_WritePin(BMP->CS_Port, (int)BMP->CS_Pin, GPIO_PIN_RESET);
}

// sets the CS pin high
void BMP390_CSHigh(BMP390* BMP){
	HAL_GPIO_WritePin(BMP->CS_Port, (int)BMP->CS_Pin, GPIO_PIN_SET);
}

// writes to a specified register
void BMP390_WriteReg(BMP390* BMP, uint8_t addr, uint8_t data){

	BMP->TX_Buf[0] = 0x7F & addr; // set bit 7 low for write
	BMP->TX_Buf[1] = data;

	BMP390_CSLow(BMP);
	HAL_SPI_Transmit(&BMP->SPI, BMP->TX_Buf, 2, 100);
	BMP390_CSHigh(BMP);
}

// reads a specified register, returning the value
uint8_t BMP390_ReadReg(BMP390* BMP, uint8_t addr){ // returns byte

	BMP->TX_Buf[0] = 0x80 | addr; // set bit 7 high for read
	BMP->TX_Buf[1] = 0x00; // dummy read byte

	BMP390_CSLow(BMP);
	HAL_SPI_TransmitReceive(&BMP->SPI, BMP->TX_Buf, BMP->RX_Buf, 3, 100);
	BMP390_CSHigh(BMP);

	return BMP->RX_Buf[2]; // third byte is returned data
}

// reads multiple registers specified by a start/length, returning a pointer to the values
// WARNING - do not read more than 30 registers at a time or the pointer will go out of bounds
uint8_t* BMP390_ReadRegs(BMP390* BMP, uint8_t addr, uint8_t len){ // returns pointer to first byte of data

	BMP->TX_Buf[0] = 0x80 | addr; // set bit 7 high for read
	BMP->TX_Buf[1] = 0x00; // dummy read byte

	BMP390_CSLow(BMP);
	HAL_SPI_TransmitReceive(&BMP->SPI, BMP->TX_Buf, BMP->RX_Buf, len+2, 100);
	BMP390_CSHigh(BMP);

	return BMP->RX_Buf + 2;
}

// reads the calibration registers, parsing and quantizing them
void BMP390_ReadCal(BMP390* BMP){

	uint8_t *reg_data = BMP390_ReadRegs(BMP, BMP3_REG_CALIB_DATA, BMP3_LEN_CALIB_DATA);

	// parse raw, unquantized calibration data
	BMP->Calib_Data.par_t1 = BMP3_CONCAT_BYTES(reg_data[1], reg_data[0]);
	BMP->Calib_Data.par_t2 = BMP3_CONCAT_BYTES(reg_data[3], reg_data[2]);
	BMP->Calib_Data.par_t3 = (int8_t)reg_data[4];
	BMP->Calib_Data.par_p1 = (int16_t)BMP3_CONCAT_BYTES(reg_data[6], reg_data[5]);
	BMP->Calib_Data.par_p2 = (int16_t)BMP3_CONCAT_BYTES(reg_data[8], reg_data[7]);
	BMP->Calib_Data.par_p3 = (int8_t)reg_data[9];
	BMP->Calib_Data.par_p4 = (int8_t)reg_data[10];
	BMP->Calib_Data.par_p5 = BMP3_CONCAT_BYTES(reg_data[12], reg_data[11]);
	BMP->Calib_Data.par_p6 = BMP3_CONCAT_BYTES(reg_data[14], reg_data[13]);
	BMP->Calib_Data.par_p7 = (int8_t)reg_data[15];
	BMP->Calib_Data.par_p8 = (int8_t)reg_data[16];
	BMP->Calib_Data.par_p9 = (int16_t)BMP3_CONCAT_BYTES(reg_data[18], reg_data[17]);
	BMP->Calib_Data.par_p10 = (int8_t)reg_data[19];
	BMP->Calib_Data.par_p11 = (int8_t)reg_data[20];


	// quantize data

    /* Temporary variable */
    double temp_var;

    /* 1 / 2^8 */
    temp_var = 0.00390625f;
    BMP->Quant_Calib_Data.par_t1 = ((double)BMP->Calib_Data.par_t1 / temp_var);
    temp_var = 1073741824.0f;
    BMP->Quant_Calib_Data.par_t2 = ((double)BMP->Calib_Data.par_t2 / temp_var);
    temp_var = 281474976710656.0f;
    BMP->Quant_Calib_Data.par_t3 = ((double)BMP->Calib_Data.par_t3 / temp_var);
    temp_var = 1048576.0f;
    BMP->Quant_Calib_Data.par_p1 = ((double)(BMP->Calib_Data.par_p1 - (16384)) / temp_var);
    temp_var = 536870912.0f;
    BMP->Quant_Calib_Data.par_p2 = ((double)(BMP->Calib_Data.par_p2 - (16384)) / temp_var);
    temp_var = 4294967296.0f;
    BMP->Quant_Calib_Data.par_p3 = ((double)BMP->Calib_Data.par_p3 / temp_var);
    temp_var = 137438953472.0f;
    BMP->Quant_Calib_Data.par_p4 = ((double)BMP->Calib_Data.par_p4 / temp_var);

    /* 1 / 2^3 */
    temp_var = 0.125f;
    BMP->Quant_Calib_Data.par_p5 = ((double)BMP->Calib_Data.par_p5 / temp_var);
    temp_var = 64.0f;
    BMP->Quant_Calib_Data.par_p6 = ((double)BMP->Calib_Data.par_p6 / temp_var);
    temp_var = 256.0f;
    BMP->Quant_Calib_Data.par_p7 = ((double)BMP->Calib_Data.par_p7 / temp_var);
    temp_var = 32768.0f;
    BMP->Quant_Calib_Data.par_p8 = ((double)BMP->Calib_Data.par_p8 / temp_var);
    temp_var = 281474976710656.0f;
    BMP->Quant_Calib_Data.par_p9 = ((double)BMP->Calib_Data.par_p9 / temp_var);
    temp_var = 281474976710656.0f;
    BMP->Quant_Calib_Data.par_p10 = ((double)BMP->Calib_Data.par_p10 / temp_var);
    temp_var = 36893488147419103232.0f;
    BMP->Quant_Calib_Data.par_p11 = ((double)BMP->Calib_Data.par_p11 / temp_var);
}


// reads the calibration values and sets up the config registers
void BMP390_Init(BMP390* BMP){

	BMP390_ReadCal(BMP);

	BMP390_WriteReg(BMP, BMP3_REG_FIFO_CONFIG_1, 0b00000000); // Turn off FIFO
	BMP390_WriteReg(BMP, BMP3_REG_INT_CTRL, 0b01000010); // Set interrupt pin to data ready, active high, push/pull
	BMP390_WriteReg(BMP, BMP3_REG_PWR_CTRL, 0b00110011); // Turn on pressure/temp, normal mode
	BMP390_WriteReg(BMP, BMP3_REG_OSR, 0b00000001); // Pressure x2, temp x1 oversampling
	BMP390_WriteReg(BMP, BMP3_REG_ODR, 0b00000001); // 100hz
	BMP390_WriteReg(BMP, BMP3_REG_CONFIG, 0b00000001); // IIR filter off


}

// helper function to calculate the power
uint32_t pow_bmp3(uint8_t base, uint8_t power)
{
    uint32_t pow_output = 1;

    while (power != 0)
    {
        pow_output = base * pow_output;
        power--;
    }

    return pow_output;
}

// calculates BMP->Temp_C from the uncompensated value
void BMP390_Compensate_Temp(BMP390* BMP){
    int64_t uncomp_temp = BMP->Temp_Raw;
    double partial_data1;
    double partial_data2;

    partial_data1 = (double)(uncomp_temp - BMP->Quant_Calib_Data.par_t1);
    partial_data2 = (double)(partial_data1 * BMP->Quant_Calib_Data.par_t2);

    /* Update the compensated temperature in calib structure since this is
     * needed for pressure calculation */
    BMP->Quant_Calib_Data.t_lin = partial_data2 + (partial_data1 * partial_data1) *
                                             BMP->Quant_Calib_Data.par_t3;

    /* Returns compensated temperature */
    if (BMP->Quant_Calib_Data.t_lin < BMP3_MIN_TEMP_DOUBLE)
    {
        BMP->Quant_Calib_Data.t_lin = BMP3_MIN_TEMP_DOUBLE;
    }

    if (BMP->Quant_Calib_Data.t_lin > BMP3_MAX_TEMP_DOUBLE)
    {
        BMP->Quant_Calib_Data.t_lin = BMP3_MAX_TEMP_DOUBLE;
    }

    BMP->Temp_C = BMP->Quant_Calib_Data.t_lin;
}

// calculates BMP->Temp_C from the uncompensated value
void BMP390_Compensate_Pressure(BMP390* BMP){

    /* Variable to store the compensated pressure */
    double comp_press;

    /* Temporary variables used for compensation */
    double partial_data1;
    double partial_data2;
    double partial_data3;
    double partial_data4;
    double partial_out1;
    double partial_out2;

    partial_data1 = BMP->Quant_Calib_Data.par_p6 * BMP->Quant_Calib_Data.t_lin;
    partial_data2 = BMP->Quant_Calib_Data.par_p7 * pow_bmp3(BMP->Quant_Calib_Data.t_lin, 2);
    partial_data3 = BMP->Quant_Calib_Data.par_p8 * pow_bmp3(BMP->Quant_Calib_Data.t_lin, 3);
    partial_out1 = BMP->Quant_Calib_Data.par_p5 + partial_data1 + partial_data2 + partial_data3;

    partial_data1 = BMP->Quant_Calib_Data.par_p2 * BMP->Quant_Calib_Data.t_lin;
    partial_data2 = BMP->Quant_Calib_Data.par_p3 * pow_bmp3(BMP->Quant_Calib_Data.t_lin, 2);
    partial_data3 = BMP->Quant_Calib_Data.par_p4 * pow_bmp3(BMP->Quant_Calib_Data.t_lin, 3);
    partial_out2 = (double)BMP->Pressure_Raw *
                   (BMP->Quant_Calib_Data.par_p1 + partial_data1 + partial_data2 + partial_data3);

    partial_data1 = pow_bmp3((double)BMP->Pressure_Raw, 2);
    partial_data2 = BMP->Quant_Calib_Data.par_p9 + BMP->Quant_Calib_Data.par_p10 * BMP->Quant_Calib_Data.t_lin;
    partial_data3 = partial_data1 * partial_data2;
    partial_data4 = partial_data3 + pow_bmp3((double)BMP->Pressure_Raw, 3) * BMP->Quant_Calib_Data.par_p11;
    comp_press = partial_out1 + partial_out2 + partial_data4;

    if (comp_press < BMP3_MIN_PRES_DOUBLE)
    {
        comp_press = BMP3_MIN_PRES_DOUBLE;
    }

    if (comp_press > BMP3_MAX_PRES_DOUBLE)
    {
        comp_press = BMP3_MAX_PRES_DOUBLE;
    }

    BMP->Pressure_Pascal = comp_press;

}


// reads the raw data and calculates their values in C and Pascal
void BMP390_ReadData(BMP390* BMP){

	uint8_t *reg_data = BMP390_ReadRegs(BMP, BMP3_REG_DATA, BMP3_LEN_P_T_DATA);

	// read in raw data
	BMP->Pressure_Raw = ((uint32_t) reg_data[2] << 16) | ((uint32_t) reg_data[1] << 8) | ((uint32_t) reg_data[0]);
	BMP->Temp_Raw = 	((uint32_t) reg_data[5] << 16) | ((uint32_t) reg_data[4] << 8) | ((uint32_t) reg_data[3]);

	// run compensation functions
	BMP390_Compensate_Temp(BMP);
	BMP390_Compensate_Pressure(BMP);

	// calculate meters above sea level
	double atmospheric = BMP->Pressure_Pascal/100.0;
	BMP->Altitude_M = 44330.0 * (1.0 - pow(atmospheric / SEALEVELPRESSURE_HPA, 0.1903));
}



