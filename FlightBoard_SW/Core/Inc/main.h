/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SW_EN1_Pin GPIO_PIN_0
#define SW_EN1_GPIO_Port GPIOC
#define SW_EN2_Pin GPIO_PIN_1
#define SW_EN2_GPIO_Port GPIOC
#define SW_EN3_Pin GPIO_PIN_2
#define SW_EN3_GPIO_Port GPIOC
#define SW_EN4_Pin GPIO_PIN_3
#define SW_EN4_GPIO_Port GPIOC
#define RADIO_RST_Pin GPIO_PIN_4
#define RADIO_RST_GPIO_Port GPIOC
#define RADIO_BUSY_Pin GPIO_PIN_5
#define RADIO_BUSY_GPIO_Port GPIOC
#define RADIO_DIO1_Pin GPIO_PIN_0
#define RADIO_DIO1_GPIO_Port GPIOB
#define BARO_INT_Pin GPIO_PIN_1
#define BARO_INT_GPIO_Port GPIOB
#define IMU_INT_Pin GPIO_PIN_2
#define IMU_INT_GPIO_Port GPIOB
#define SPI2_CS1_Pin GPIO_PIN_13
#define SPI2_CS1_GPIO_Port GPIOB
#define SPI2_CS2_Pin GPIO_PIN_6
#define SPI2_CS2_GPIO_Port GPIOC
#define ACCEL_INT_Pin GPIO_PIN_15
#define ACCEL_INT_GPIO_Port GPIOA
#define SW_ST4_Pin GPIO_PIN_4
#define SW_ST4_GPIO_Port GPIOB
#define SW_ST3_Pin GPIO_PIN_5
#define SW_ST3_GPIO_Port GPIOB
#define SW_ST2_Pin GPIO_PIN_6
#define SW_ST2_GPIO_Port GPIOB
#define SW_ST1_Pin GPIO_PIN_7
#define SW_ST1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
