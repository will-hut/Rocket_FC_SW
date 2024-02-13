/*
 * ADXL375_Definitions.h
 *
 *  Created on: Feb 13, 2024
 *      Author: willh
 */

#ifndef INC_ADXL375_DEFINITIONS_H_
#define INC_ADXL375_DEFINITIONS_H_

#include "main.h"

// register values
#define ADXL3XX_REG_DEVID           0x00
#define ADXL3XX_REG_THRESH_TAP      0x1D
#define ADXL3XX_REG_OFSX            0x1E
#define ADXL3XX_REG_OFSY            0x1F
#define ADXL3XX_REG_OFSZ            0x20
#define ADXL3XX_REG_DUR             0x21
#define ADXL3XX_REG_LATENT          0x22
#define ADXL3XX_REG_WINDOW          0x23
#define ADXL3XX_REG_THRESH_ACT      0x24
#define ADXL3XX_REG_THRESH_INACT    0x25
#define ADXL3XX_REG_TIME_INACT      0x26
#define ADXL3XX_REG_ACT_INACT_CTL   0x27
#define ADXL3XX_REG_THRESH_FF       0x28
#define ADXL3XX_REG_TIME_FF         0x29
#define ADXL3XX_REG_TAP_AXES        0x2A
#define ADXL3XX_REG_ACT_TAP_STATUS  0x2B
#define ADXL3XX_REG_BW_RATE         0x2C
#define ADXL3XX_REG_POWER_CTL       0x2D
#define ADXL3XX_REG_INT_ENABLE      0x2E
#define ADXL3XX_REG_INT_MAP         0x2F
#define ADXL3XX_REG_INT_SOURCE      0x30
#define ADXL3XX_REG_DATA_FORMAT     0x31
#define ADXL3XX_REG_DATAX0          0x32
#define ADXL3XX_REG_DATAX1          0x33
#define ADXL3XX_REG_DATAY0          0x34
#define ADXL3XX_REG_DATAY1          0x35
#define ADXL3XX_REG_DATAZ0          0x36
#define ADXL3XX_REG_DATAZ1          0x37
#define ADXL3XX_REG_FIFO_CTL        0x38
#define ADXL3XX_REG_FIFO_STATUS     0x39

// address
#define ADXL3XX_ADDRESS 			0x53

// constants
#define ADXL3XX_ACC_SCALE_FACTOR 	0.049
#define ADXL3XX_DATA_LEN			6

// helper macros
#define CONCAT_BYTES(msb, lsb)             (((uint16_t)msb << 8) | (uint16_t)lsb)

#endif /* INC_ADXL375_DEFINITIONS_H_ */
