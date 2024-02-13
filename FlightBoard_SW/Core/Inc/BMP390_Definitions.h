/*
 * BMP390_Definitions.h
 *
 *  Created on: Feb 12, 2024
 *      Author: willh
 */

#ifndef INC_BMP390_DEFINITIONS_H_
#define INC_BMP390_DEFINITIONS_H_

#include "main.h"

// register values
#define BMP3_REG_CHIP_ID                        0x00
#define BMP3_REG_ERR                            0x02
#define BMP3_REG_SENS_STATUS                    0x03
#define BMP3_REG_DATA                           0x04
#define BMP3_REG_EVENT                          0x10
#define BMP3_REG_INT_STATUS                     0x11
#define BMP3_REG_FIFO_LENGTH                    0x12
#define BMP3_REG_FIFO_DATA                      0x14
#define BMP3_REG_FIFO_WM                        0x15
#define BMP3_REG_FIFO_CONFIG_1                  0x17
#define BMP3_REG_FIFO_CONFIG_2                  0x18
#define BMP3_REG_INT_CTRL                       0x19
#define BMP3_REG_IF_CONF                        0x1A
#define BMP3_REG_PWR_CTRL                       0x1B
#define BMP3_REG_OSR                            0X1C
#define BMP3_REG_ODR                            0x1D
#define BMP3_REG_CONFIG                         0x1F
#define BMP3_REG_CALIB_DATA                     0x31
#define BMP3_REG_CMD                            0x7E

// data field lengths
#define BMP3_LEN_CALIB_DATA						21
#define BMP3_LEN_P_T_DATA                       6

// data max/min vals
#define BMP3_MIN_TEMP_INT                       -4000
#define BMP3_MAX_TEMP_INT                       8500
#define BMP3_MIN_TEMP_DOUBLE                    -40.0f
#define BMP3_MAX_TEMP_DOUBLE                    85.0f
#define BMP3_MIN_PRES_INT                       3000000
#define BMP3_MAX_PRES_INT                       12500000
#define BMP3_MIN_PRES_DOUBLE                    30000.0f
#define BMP3_MAX_PRES_DOUBLE                    125000.0f

// helper macros
#define BMP3_CONCAT_BYTES(msb, lsb)             (((uint16_t)msb << 8) | (uint16_t)lsb)

// physics constants

#define SEALEVELPRESSURE_HPA 1013.25

// structs
typedef struct {
	// from table 24 of datasheet

    uint16_t par_t1;
    uint16_t par_t2;
    int8_t par_t3;
    int16_t par_p1;
    int16_t par_p2;
    int8_t par_p3;
    int8_t par_p4;
    uint16_t par_p5;
    uint16_t par_p6;
    int8_t par_p7;
    int8_t par_p8;
    int16_t par_p9;
    int8_t par_p10;
    int8_t par_p11;
    int64_t t_lin;
} BMP390_Calib_Data;

typedef struct {
    double par_t1;
    double par_t2;
    double par_t3;
    double par_p1;
    double par_p2;
    double par_p3;
    double par_p4;
    double par_p5;
    double par_p6;
    double par_p7;
    double par_p8;
    double par_p9;
    double par_p10;
    double par_p11;
    double t_lin;
} BMP390_Quant_Calib_Data;

#endif /* INC_BMP390_DEFINITIONS_H_ */
