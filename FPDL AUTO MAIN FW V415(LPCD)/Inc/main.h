/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32g0xx_hal.h"
#include "stm32g0xx_ll_system.h"

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
#define FPM_TOUCH_INPUT__ACTIVE_HIGH_Pin GPIO_PIN_1
#define FPM_TOUCH_INPUT__ACTIVE_HIGH_GPIO_Port GPIOB
#define FPM_TOUCH_INPUT__ACTIVE_HIGH_EXTI_IRQn EXTI0_1_IRQn
#define Hall_sensor_input_active_low_Pin GPIO_PIN_9
#define Hall_sensor_input_active_low_GPIO_Port GPIOA
#define Hall_sensor_input_active_low_EXTI_IRQn EXTI4_15_IRQn
#define Touch_Key_input_active_low_GPIO_Port GPIOA
#define Touch_Key_input_active_low_EXTI_IRQn EXTI4_15_IRQn
#define Anti_prying_input_active_low_Pin GPIO_PIN_4
#define Anti_prying_input_active_low_GPIO_Port GPIOB
#define Anti_prying_input_active_low_EXTI_IRQn EXTI4_15_IRQn
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
