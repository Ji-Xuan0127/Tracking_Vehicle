/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#define Track1_Pin GPIO_PIN_0
#define Track1_GPIO_Port GPIOA
#define Track2_Pin GPIO_PIN_1
#define Track2_GPIO_Port GPIOA
#define Track3_Pin GPIO_PIN_2
#define Track3_GPIO_Port GPIOA
#define Track4_Pin GPIO_PIN_3
#define Track4_GPIO_Port GPIOA
#define Speed_Pin GPIO_PIN_4
#define Speed_GPIO_Port GPIOA
#define Kp_Pin GPIO_PIN_5
#define Kp_GPIO_Port GPIOA
#define Ki_Pin GPIO_PIN_6
#define Ki_GPIO_Port GPIOA
#define Kd_Pin GPIO_PIN_7
#define Kd_GPIO_Port GPIOA
#define Start_in_Pin GPIO_PIN_5
#define Start_in_GPIO_Port GPIOC
#define Start_out_Pin GPIO_PIN_8
#define Start_out_GPIO_Port GPIOE
#define Motor1_Con1_Pin GPIO_PIN_13
#define Motor1_Con1_GPIO_Port GPIOE
#define Motor1_Con2_Pin GPIO_PIN_14
#define Motor1_Con2_GPIO_Port GPIOE
#define Motor2_Con1_Pin GPIO_PIN_10
#define Motor2_Con1_GPIO_Port GPIOB
#define Motor2_Con2_Pin GPIO_PIN_11
#define Motor2_Con2_GPIO_Port GPIOB
#define Motor2_Speed1_Pin GPIO_PIN_12
#define Motor2_Speed1_GPIO_Port GPIOD
#define Motor2_Speed2_Pin GPIO_PIN_13
#define Motor2_Speed2_GPIO_Port GPIOD
#define Motor1_Speed1_Pin GPIO_PIN_6
#define Motor1_Speed1_GPIO_Port GPIOC
#define Motor1_Speed2_Pin GPIO_PIN_7
#define Motor1_Speed2_GPIO_Port GPIOC
#define RES_Pin GPIO_PIN_5
#define RES_GPIO_Port GPIOD
#define DC_Pin GPIO_PIN_6
#define DC_GPIO_Port GPIOD
#define BLK_Pin GPIO_PIN_7
#define BLK_GPIO_Port GPIOD
#define PWM2_Pin GPIO_PIN_8
#define PWM2_GPIO_Port GPIOB
#define PWM1_Pin GPIO_PIN_9
#define PWM1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
