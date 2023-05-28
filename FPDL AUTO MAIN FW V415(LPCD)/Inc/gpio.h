/**
  ******************************************************************************
  * File Name          : gpio.h
  * Description        : This file contains all the functions prototypes for 
  *                      the gpio  
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __gpio_H
#define __gpio_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

extern void MX_GPIO_Init(void);
extern void EnableAntiPrying_Interrupt(void);
extern void DisableAntiPrying_Interrupt(void);
extern void EnableTouch_Interrupt(void);
extern void DisableTouch_Interrupt(void);
extern void EnablePIR_Interrupt(void);
extern void DisablePIR_Interrupt(void);
extern void EnableRADAR_Interrupt(void);
extern void DisableRADAR_Interrupt(void);
extern void EnableWifiRx_Interrupt(void);
extern void DisableWifiRx_Interrupt(void);
extern void EnableNFCIRQ_Interrupt(void);
extern void DisableNFCIRQ_Interrupt(void);



/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
