/*
 * BMI323_Definitions.h
 *
 *  Created on: Feb 12, 2024
 *      Author: willh
 */

#ifndef INC_BMI323_DEFINITIONS_H_
#define INC_BMI323_DEFINITIONS_H_

#include "main.h"

// hardcoded constants
#define BMI3_LSB_MG									2.05
#define BMI3_LSB_DEG_S								16.4

// register values
// register values
#define BMI3_REG_CHIP_ID                            0x00
#define BMI3_REG_ERR_REG                            0x01
#define BMI3_REG_STATUS                             0x02
#define BMI3_REG_ACC_DATA_X                         0x03
#define BMI3_REG_ACC_DATA_Y                         0x04
#define BMI3_REG_ACC_DATA_Z                         0x05
#define BMI3_REG_GYR_DATA_X                         0x06
#define BMI3_REG_GYR_DATA_Y                         0x07
#define BMI3_REG_GYR_DATA_Z                         0x08
#define BMI3_REG_TEMP_DATA                          0x09
#define BMI3_REG_SENSOR_TIME_0                      0x0A
#define BMI3_REG_SENSOR_TIME_1                      0x0B
#define BMI3_REG_SAT_FLAGS                          0x0C
#define BMI3_REG_INT_STATUS_INT1                    0x0D
#define BMI3_REG_INT_STATUS_INT2                    0x0E
#define BMI3_REG_INT_STATUS_IBI                     0x0F
#define BMI3_REG_FEATURE_IO0                        0x10
#define BMI3_REG_FEATURE_IO1                        0x11
#define BMI3_REG_FEATURE_IO2                        0x12
#define BMI3_REG_FEATURE_IO3                        0x13
#define BMI3_REG_FEATURE_IO_STATUS                  0x14
#define BMI3_REG_FIFO_FILL_LEVEL                    0x15
#define BMI3_REG_FIFO_DATA                          0x16
#define BMI3_REG_ACC_CONF                           0x20
#define BMI3_REG_GYR_CONF                           0x21
#define BMI3_REG_ALT_ACC_CONF                       0x28
#define BMI3_REG_ALT_GYR_CONF                       0x29
#define BMI3_REG_ALT_CONF                           0x2A
#define BMI3_REG_ALT_STATUS                         0x2B
#define BMI3_REG_FIFO_WATERMARK                     0x35
#define BMI3_REG_FIFO_CONF                          0x36
#define BMI3_REG_FIFO_CTRL                          0x37
#define BMI3_REG_IO_INT_CTRL                        0x38
#define BMI3_REG_INT_CONF                           0x39
#define BMI3_REG_INT_MAP1                           0x3A
#define BMI3_REG_INT_MAP2                           0x3B
#define BMI3_REG_FEATURE_CTRL                       0x40
#define BMI3_REG_FEATURE_DATA_ADDR                  0x41
#define BMI3_REG_FEATURE_DATA_TX                    0x42
#define BMI3_REG_FEATURE_DATA_STATUS                0x43
#define BMI3_REG_FEATURE_ENGINE_STATUS              0x45
#define BMI3_REG_FEATURE_EVENT_EXT                  0x47
#define BMI3_REG_IO_PDN_CTRL                        0x4F
#define BMI3_REG_IO_SPI_IF                          0x50
#define BMI3_REG_IO_PAD_STRENGTH                    0x51
#define BMI3_REG_IO_I2C_IF                          0x52
#define BMI3_REG_IO_ODR_DEVIATION                   0x53
#define BMI3_REG_ACC_DP_OFF_X                       0x60
#define BMI3_REG_ACC_DP_DGAIN_X                     0x61
#define BMI3_REG_ACC_DP_OFF_Y                       0x62
#define BMI3_REG_ACC_DP_DGAIN_Y                     0x63
#define BMI3_REG_ACC_DP_OFF_Z                       0x64
#define BMI3_REG_ACC_DP_DGAIN_Z                     0x65
#define BMI3_REG_GYR_DP_OFF_X                       0x66
#define BMI3_REG_GYR_DP_DGAIN_X                     0x67
#define BMI3_REG_GYR_DP_OFF_Y                       0x68
#define BMI3_REG_GYR_DP_DGAIN_Y                     0x69
#define BMI3_REG_GYR_DP_OFF_Z                       0x6A
#define BMI3_REG_GYR_DP_DGAIN_Z                     0x6B
#define BMI3_REG_I3C_TC_SYNC_TPH                    0x70
#define BMI3_REG_I3C_TC_SYNC_TU                     0x71
#define BMI3_REG_I3C_TC_SYNC_ODR                    0x72
#define BMI3_REG_CMD                                0x7E
#define BMI3_REG_CFG_RES                            0x7F

// data field lengths
#define BMI3_LEN_ACCEL_GYRO_DATA					6

//helper macros
#define BMI3_CONCAT_BYTES(msb, lsb)             	(((uint16_t)msb << 8) | (uint16_t)lsb)


#endif /* INC_BMI323_DEFINITIONS_H_ */
