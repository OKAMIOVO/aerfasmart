/**
  ******************************************************************************
  * File Name          : gpio.c
  * Description        : This file provides code for the configuration
  *                      of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"
#include "Project.h"
#include "IO.h"
#include "Hardware.h"
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{
	
	  GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	  /* GPIO Ports Clock Enable */
	  __HAL_RCC_GPIOC_CLK_ENABLE();
	  __HAL_RCC_GPIOF_CLK_ENABLE();
	  __HAL_RCC_GPIOA_CLK_ENABLE();
	  __HAL_RCC_GPIOB_CLK_ENABLE();
	  __HAL_RCC_GPIOD_CLK_ENABLE();
		
		HAL_SYSCFG_StrobeDBattpinsConfig(SYSCFG_CFGR1_UCPD1_STROBE);	//disable PA8,PB15 pull down
		HAL_SYSCFG_StrobeDBattpinsConfig(SYSCFG_CFGR1_UCPD2_STROBE);	//disable PD0,PD1 pull down
	
	  /*Configure GPIO pin Output Level */
	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_0 
							  |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6/*|GPIO_PIN_7*/ 
							  |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_RESET);
	
	  /*Configure GPIO pin Output Level */
	  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0|GPIO_PIN_1, GPIO_PIN_RESET);
	
	  /*Configure GPIO pin Output Level */
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4 
							  |GPIO_PIN_8|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_15, GPIO_PIN_RESET);
	
	  /*Configure GPIO pin Output Level */
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_12|GPIO_PIN_13 
							  |GPIO_PIN_14|GPIO_PIN_3|GPIO_PIN_5, GPIO_PIN_RESET);
	
	  /*Configure GPIO pin Output Level */
	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8/*|GPIO_PIN_9*/|GPIO_PIN_0|GPIO_PIN_1 
							  |GPIO_PIN_2|GPIO_PIN_4, GPIO_PIN_RESET);
	
	  /*Configure GPIO pins :  PC13 PC0 
							   PC4 PC5 PC6 PC7 
							   PC8 PC9 PC10 */
	  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_1 
							  |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7 
							  |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
		
	
	
	  /*Configure GPIO pins : PF0 PF1, spare set to output low for lower power comsuption */
	  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

	
	  /*Configure GPIO pin : PC0 */
//	  GPIO_InitStruct.Pin = GPIO_PIN_0;
//	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//	  GPIO_InitStruct.Pull = GPIO_PULLUP;
//	  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	  DisableNFCIRQ_Interrupt();
	
	  /*Configure GPIO pins : PA1 PA2 PA3 PA4 
							   PA8 PA11 PA12 PA15 */
	  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_8 
							  |GPIO_PIN_9|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_15;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
		/*Configure GPIO pins : PB0 PB11 PB12 PB13 
							   PB14 PB3 PB5 */
	  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_10|GPIO_PIN_12|GPIO_PIN_13 
							  |GPIO_PIN_14|GPIO_PIN_5;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);	

		  
	  
	  /*Configure GPIO pin : PB15  VOICE BUSY*/ 
	  GPIO_InitStruct.Pin = GPIO_PIN_15;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
	  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	
	  /*Configure GPIO pin : FPM INT */
	  GPIO_InitStruct.Pin = GPIO_PIN_1;
	  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	
	
#ifndef DEBUG_MODE
		/****** SET PA13/SWDIO to input with internal pull high, for lower power consumption *******/
		GPIO_InitStruct.Pin = GPIO_PIN_13;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
#endif
	
		
	  /*Configure GPIO pins : PA10 Touch INT */
	  GPIO_InitStruct.Pin = GPIO_PIN_10;
	  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	  /*Configure GPIO pins : PD8 PD9 PD0 PD1 
							   PD2 PD4 */
	  GPIO_InitStruct.Pin = /*GPIO_PIN_8|GPIO_PIN_9|*/GPIO_PIN_0|GPIO_PIN_1 
							  |GPIO_PIN_2|GPIO_PIN_4;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	 
	
	  Hardware_AwakeLine_WorkingStatus();
	
	
	  /* EXTI interrupt init*/
	  HAL_NVIC_SetPriority(EXTI0_1_IRQn, 3, 3);
	  HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);
		
	  HAL_NVIC_SetPriority(EXTI2_3_IRQn, 3, 3);
	  HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);
	
	  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 3, 3);
	  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

  


}
void EnableTouch_Interrupt(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	HAL_GPIO_DeInit(GPIOA,GPIO_PIN_10);
	
	GPIO_InitStruct.Pin = GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
void DisableTouch_Interrupt(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	HAL_GPIO_DeInit(GPIOA,GPIO_PIN_10);
	//__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_10);
	
	GPIO_InitStruct.Pin = GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;	
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


}

void EnablePIR_Interrupt(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	HAL_GPIO_DeInit(GPIOB,GPIO_PIN_11);
	
	GPIO_InitStruct.Pin = GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
void DisablePIR_Interrupt(void)
{
	//GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	HAL_GPIO_DeInit(GPIOB,GPIO_PIN_11);	//Analog input
	
//	GPIO_InitStruct.Pin = GPIO_PIN_11;
//	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//	GPIO_InitStruct.Pull = GPIO_PULLUP;	
//	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}
void EnableRADAR_Interrupt(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	HAL_GPIO_DeInit(GPIOB,GPIO_PIN_11);
	
	GPIO_InitStruct.Pin = GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
void DisableRADAR_Interrupt(void)
{
	//GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	HAL_GPIO_DeInit(GPIOB,GPIO_PIN_11);	//Analog input
	
//	GPIO_InitStruct.Pin = GPIO_PIN_11;
//	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//	GPIO_InitStruct.Pull = GPIO_PULLUP;	
//	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

void EnableWifiRx_Interrupt(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}
void DisableWifiRx_Interrupt(void)
{
	HAL_GPIO_DeInit(GPIOD,GPIO_PIN_9);
}




void EnableAntiPrying_Interrupt(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	HAL_GPIO_DeInit(GPIOB,GPIO_PIN_4);
	
	GPIO_InitStruct.Pin = GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
void DisableAntiPrying_Interrupt(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	HAL_GPIO_DeInit(GPIOB,GPIO_PIN_4);
	
	GPIO_InitStruct.Pin = GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;

	#ifdef ProjectIs_AutoBarLock_S58Z07
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	#else
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	#endif
	
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void EnableNFCIRQ_Interrupt(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	HAL_GPIO_DeInit(GPIOC,GPIO_PIN_0);
	
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}
void DisableNFCIRQ_Interrupt(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	HAL_GPIO_DeInit(GPIOC,GPIO_PIN_0);	//Analog input
	
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;	
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}


/* USER CODE BEGIN 2 */

/* USER CODE END 2 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
