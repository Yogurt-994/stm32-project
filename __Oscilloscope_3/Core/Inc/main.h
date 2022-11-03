/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "algorithm.h"
#include "base.h"
#include "stdio.h"
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
#define GND_Pin GPIO_PIN_2
#define GND_GPIO_Port GPIOE
#define GNDE3_Pin GPIO_PIN_3
#define GNDE3_GPIO_Port GPIOE
#define GNDE4_Pin GPIO_PIN_4
#define GNDE4_GPIO_Port GPIOE
#define GNDE5_Pin GPIO_PIN_5
#define GNDE5_GPIO_Port GPIOE
#define GNDE6_Pin GPIO_PIN_6
#define GNDE6_GPIO_Port GPIOE
#define GNDC13_Pin GPIO_PIN_13
#define GNDC13_GPIO_Port GPIOC
#define GNDF0_Pin GPIO_PIN_0
#define GNDF0_GPIO_Port GPIOF
#define GNDF1_Pin GPIO_PIN_1
#define GNDF1_GPIO_Port GPIOF
#define ADS8688_DAISY_Pin GPIO_PIN_15
#define ADS8688_DAISY_GPIO_Port GPIOA
#define ADS8688_SCK_Pin GPIO_PIN_10
#define ADS8688_SCK_GPIO_Port GPIOC
#define ADS8688_MISO_Pin GPIO_PIN_11
#define ADS8688_MISO_GPIO_Port GPIOC
#define ADS8688_MOSI_Pin GPIO_PIN_12
#define ADS8688_MOSI_GPIO_Port GPIOC
#define ADS8688_RST_Pin GPIO_PIN_2
#define ADS8688_RST_GPIO_Port GPIOD
#define ADS8688_CS_Pin GPIO_PIN_3
#define ADS8688_CS_GPIO_Port GPIOD
#define PGA00_Pin GPIO_PIN_9
#define PGA00_GPIO_Port GPIOG
#define PGA01_Pin GPIO_PIN_10
#define PGA01_GPIO_Port GPIOG
#define LED0_Pin GPIO_PIN_13
#define LED0_GPIO_Port GPIOG
#define VCC_Pin GPIO_PIN_4
#define VCC_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
