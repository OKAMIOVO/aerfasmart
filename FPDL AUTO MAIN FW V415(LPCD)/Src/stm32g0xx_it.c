/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32g0xx_it.c
  * @brief   Interrupt Service Routines.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32g0xx_it.h"
#include "IO.h"
#include "ISR.h"
#include "usart.h"
#include "Project.h"
#include "CSK14.h"
#include "TSM12S.h"
#include "rtc.h"
#include "PIR.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim6;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M0+ Processor Interruption and Exception Handlers          */ 
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
		DEBUG_MARK;
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVC_IRQn 0 */

  /* USER CODE END SVC_IRQn 0 */
  /* USER CODE BEGIN SVC_IRQn 1 */

  /* USER CODE END SVC_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */
//???????
	SysTick_ISR();
}

/******************************************************************************/
/* STM32G0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32g0xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles RTC and TAMP interrupts through EXTI lines 19 and 21.
  */
void RTC_TAMP_IRQHandler(void)
{
  /* USER CODE BEGIN RTC_TAMP_IRQn 0 */

  /* USER CODE END RTC_TAMP_IRQn 0 */
  HAL_RTCEx_WakeUpTimerIRQHandler(&hrtc);
  /* USER CODE BEGIN RTC_TAMP_IRQn 1 */
  SystemPowerMgr.AwakeSource = RTCwakeupTimer;

	//TOGGLE_DEBUG_LED;
  /* USER CODE END RTC_TAMP_IRQn 1 */
}


/******************************************************************************/
/* STM32G0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32g0xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line 0 and line 1 interrupts.
  */
void EXTI0_1_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI0_1_IRQn 0 */
  if (__HAL_GPIO_EXTI_GET_FALLING_IT(GPIO_PIN_0) != 0x00u)
  {
	  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
	  if ((SystemPowerMgr.Status == SLEEP)){
		  SystemPowerMgr.AwakeSource = NFCIrq;
		  SystemPowerMgr.Status = RUN;
	  }
	  DEBUG_MARK;
  }


  /* USER CODE END EXTI0_1_IRQn 0 */
 
  /* USER CODE BEGIN EXTI0_1_IRQn 1 */
  else if (__HAL_GPIO_EXTI_GET_RISING_IT(GPIO_PIN_1) != 0x00u)
  {
	  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
	  if ((SystemPowerMgr.Status == SLEEP)){
		  SystemPowerMgr.AwakeSource = FingerTouch;
		  SystemPowerMgr.Status = RUN;
	  }
  }
  else{

  }

  /* USER CODE END EXTI0_1_IRQn 1 */
}

/**
  * @brief This function handles EXTI line 2 and line 3 interrupts.
  */
void EXTI2_3_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI2_3_IRQn 0 */

  /* USER CODE END EXTI2_3_IRQn 0 */
  if (__HAL_GPIO_EXTI_GET_FALLING_IT(GPIO_PIN_3) != 0x00u)
  {	
	  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);	
	  if ((SystemPowerMgr.Status == SLEEP)){
		  SystemPowerMgr.AwakeSource = DriverBoard;
		  SystemPowerMgr.Status = RUN;
	  }
  }
 
  /* USER CODE BEGIN EXTI2_3_IRQn 1 */

  /* USER CODE END EXTI2_3_IRQn 1 */
}
/**
  * @brief This function handles EXTI line 4 to 15 interrupts.
  */
void EXTI4_15_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI4_15_IRQn 0 */
	if (__HAL_GPIO_EXTI_GET_FALLING_IT(GPIO_PIN_4) != 0x00u)
	{
		HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
		
		if ((SystemPowerMgr.Status == SLEEP)){
			SystemPowerMgr.AwakeSource = AntiPrying;
			SystemPowerMgr.Status = RUN;
		}
		AntiPryingMgr.AntiPryingTrigger = bTRUE;	
	}

	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);

	if (__HAL_GPIO_EXTI_GET_FALLING_IT(GPIO_PIN_10) != 0x00u)
	{	
		HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
		if ((SystemPowerMgr.Status == SLEEP)){
		SystemPowerMgr.AwakeSource = KeyBoardTouch;
		SystemPowerMgr.Status = RUN;
		}
	}

	if (__HAL_GPIO_EXTI_GET_FALLING_IT(GPIO_PIN_11) != 0x00u)
	{	
		HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);

		if ( BodyInductionMgr.BodyInductionDeviceType == PirModule )
		{
			if ( PIRmgr.RecPulse < 50 )
			{
				PIRmgr.RecPulse++;
			}
		}
		DEBUG_MARK;
	}
	if (__HAL_GPIO_EXTI_GET_RISING_IT(GPIO_PIN_11) != 0x00u)
	{	
		HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);

		if ( BodyInductionMgr.BodyInductionDeviceType == RadarModule )
		{	
			if (SystemPowerMgr.Status == SLEEP)
			{
				SystemPowerMgr.AwakeSource = BodyInduction;
			}
		}
		DEBUG_MARK;
	}
	if (__HAL_GPIO_EXTI_GET_FALLING_IT(GPIO_PIN_9) != 0x00u)
	{
		HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
		SystemPowerMgr.AwakeSource = WifiModule;
		MX_USART3_UART_Init();
		DEBUG_MARK;
	}
	if (__HAL_GPIO_EXTI_GET_FALLING_IT(GPIO_PIN_7) != 0x00u)
	{
		HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
	}
  /* USER CODE BEGIN EXTI4_15_IRQn 1 */

  /* USER CODE END EXTI4_15_IRQn 1 */
}

/**
  * @brief This function handles TIM1 break, update, trigger and commutation interrupts.
  */
void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_BRK_UP_TRG_COM_IRQn 0 */

  /* USER CODE END TIM1_BRK_UP_TRG_COM_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_BRK_UP_TRG_COM_IRQn 1 */

  /* USER CODE END TIM1_BRK_UP_TRG_COM_IRQn 1 */
}
/**
  * @brief This function handles TIM1 capture compare interrupt.
  */
void TIM1_CC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_CC_IRQn 0 */

  /* USER CODE END TIM1_CC_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_CC_IRQn 1 */

  /* USER CODE END TIM1_CC_IRQn 1 */
}

/**
  * @brief This function handles TIM3 global interrupt.
  */
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */

  /* USER CODE END TIM3_IRQn 0 */
  HAL_TIM_IRQHandler(&htim3);
  /* USER CODE BEGIN TIM3_IRQn 1 */
	
	TIM3_ISR();

  /* USER CODE END TIM3_IRQn 1 */
}
/**
  * @brief This function handles TIM3 global interrupt.
  */
void TIM6_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */

  /* USER CODE END TIM3_IRQn 0 */
  HAL_TIM_IRQHandler(&htim6);
  /* USER CODE BEGIN TIM3_IRQn 1 */
	
	TIM6_ISR();

  /* USER CODE END TIM3_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt / USART1 wake-up interrupt through EXTI line 25.
  */
void USART1_IRQHandler(void)
{
//	uint8_t temp;
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */
	UART1_ISR();
  /* USER CODE END USART1_IRQn 1 */
}
void USART2_IRQHandler(void)
{
//	uint8_t temp;
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART1_IRQn 1 */
	UART2_ISR();
  /* USER CODE END USART1_IRQn 1 */
}

void USART3_4_IRQHandler(void)
{
  /* USER CODE BEGIN USART3_4_IRQn 0 */

  /* USER CODE END USART3_4_IRQn 0 */
  HAL_UART_IRQHandler(&huart3);
  /* USER CODE BEGIN USART3_4_IRQn 1 */
  
  UART3_ISR();


  /* USER CODE END USART3_4_IRQn 1 */
}


/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
