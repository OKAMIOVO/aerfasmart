#ifndef IO_H
#define IO_H

/* ********************** FILE HEADER *************************************

*

*
*/
/* ***************************************************************************
*  INCLUDE FILES
*/
//#include "Hardware.h"
#include "Project.h"
/* MASS MICROELECTRONICS TARGET BOARD I/O SETTINGS */
/* Port Tri-state control (0 = out) */


#define SET_LCD_DS_H				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);
#define SET_LCD_DS_L				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);

#define SET_LCD_RST_H				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET);
#define SET_LCD_RST_L				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);

#define SET_LCD_CS_H				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET);
#define SET_LCD_CS_L				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET);

#define SET_MFC_RST_H				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_SET);
#define SET_MFC_RST_L				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_RESET);
#define SET_MFC_CS_L				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
#define SET_MFC_CS_H				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);

#define SET_FLASH_CS_H				//HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_SET);
#define SET_FLASH_CS_L				//HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_RESET);

#define SET_FM5114_TOUCH_RST_H		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
#define SET_FM5114_TOUCH_RST_L		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);

#define SET_OLEDPOWER_ON			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_SET);
#define SET_OLEDPOWER_OFF			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_RESET);


#define PINMACRO_TurnOnVbatToADC	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_SET);
#define PINMACRO_TurnOffVbatToADC	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);


#define SET_KB_I2C_EN_H				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_SET);
#define SET_KB_I2C_EN_L				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET);

#define PINMACRO_KB_IRQ_STATUS		(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_10))

#define PINMACRO_FPM_TOUCH_STATUS 		(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1))
#define PINMACRO_NFC_IRQ_STATUS 		(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_0))


#define SET_VOICEDATA_H 				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET);
#define SET_VOICEDATA_L 				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);
#define STATUS_PINMACRO_VOICEDATA 		(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14))
#define STATUS_PINMACRO_VOICEBUSY 		(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15))

#define SET_VOICEDATA2_H 				//HAL_GPIO_WritePin(GPIOD,GPIO_PIN_5,GPIO_PIN_SET);
#define SET_VOICEDATA2_L 				//HAL_GPIO_WritePin(GPIOD,GPIO_PIN_5,GPIO_PIN_RESET);
#define STATUS_PINMACRO_VOICEDATA2 		//(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_5))

#define TOGGLE_DEBUG_LED			HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_1);
#define SET_DEBUG_LED_ON			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_1,GPIO_PIN_SET);
#define SET_DEBUG_LED_OFF			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_1,GPIO_PIN_RESET);

#if defined (ProjectIs_AutoBarLock_S30Z03) || defined (ProjectIs_AutoBarLock_S32Z04) || defined (ProjectIs_AutoBarLock_S51Z02) \
	|| defined (ProjectIs_AutoBarLock_S51Z03) || defined (ProjectIs_AutoBarLock_S49Z05) || defined (ProjectIs_AutoBarLock_S49Z06) \
	|| defined (ProjectIs_AutoBarLock_S61Z03) || defined (ProjectIs_AutoBarLock_S70Z02) || defined (ProjectIs_AutoBarLock_S67Z01) \
	|| defined (ProjectIs_AutoBarLock_S64Z04) || defined (ProjectIs_AutoBarLock_S70Z01) || defined (ProjectIs_AutoBarLock_S08Z05) \
	|| defined (ProjectIs_AutoBarLock_S06Z04) || defined (ProjectIs_AutoBarLock_S51Z05) || defined (ProjectIs_AutoBarLock_S31Z01) \
	|| defined (ProjectIs_AutoBarLock_S67Z02) || defined (ProjectIs_AutoBarLock_S64Z08) || defined (ProjectIs_AutoBarLock_S70Z09)\
	|| defined (ProjectIs_AutoBarLock_S64Z07) || defined (ProjectIs_AutoBarLock_S64Z10) || defined (ProjectIs_AutoBarLock_S64Z09)\
	|| defined (ProjectIs_AutoBarLock_S51Z06) || defined (ProjectIs_AutoBarLock_S49Z15) || defined (ProjectIs_AutoBarLock_S20Z04)\
	|| defined ( ProjectIs_AutoBarLock_S11Z05)|| defined (ProjectIs_AutoBarLock_S61Z11) || defined (ProjectIs_AutoBarLock_S64Z19)\
	|| defined (ProjectIs_AutoBarLock_S78Z01) || defined (ProjectIs_AutoBarLock_S83Z01) || defined (ProjectIs_AutoBarLock_S72Z03)\
	|| defined (ProjectIs_AutoBarLock_S90Z01) || defined (ProjectIs_AutoBarLock_S83Z02) || defined (ProjectIs_AutoBarLock_S90Z11)\
	|| defined (ProjectIs_AutoBarLock_S90Z20) || defined (ProjectIs_AutoBarLock_S95Z01) || defined (ProjectIs_AutoBarLock_S86Z02)
#define PINMACRO_LED_KEYPOUNDSIGN_ON		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
#define PINMACRO_LED_KEYPOUNDSIGN_OFF		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
#define PINMACRO_LED_KEY0_ON				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET);
#define PINMACRO_LED_KEY0_OFF				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET);
#define PINMACRO_LED_KEYASTERISK_ON			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_SET);
#define PINMACRO_LED_KEYASTERISK_OFF		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_RESET);
#define PINMACRO_LED_KEY9_ON				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);
#define PINMACRO_LED_KEY9_OFF				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);
#define PINMACRO_LED_KEY8_ON				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
#define PINMACRO_LED_KEY8_OFF				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
#define PINMACRO_LED_KEY7_ON				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);
#define PINMACRO_LED_KEY7_OFF				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);
#define PINMACRO_LED_KEY5_ON				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET);
#define PINMACRO_LED_KEY5_OFF				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);
#define PINMACRO_LED_KEY4_ON				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_SET);
#define PINMACRO_LED_KEY4_OFF				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_RESET);
#define PINMACRO_LED_KEY3_ON				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_SET);
#define PINMACRO_LED_KEY3_OFF				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_RESET);
#define PINMACRO_LED_KEY2_ON				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_SET);
#define PINMACRO_LED_KEY2_OFF				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_RESET);
#define PINMACRO_LED_KEY1_ON				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);
#define PINMACRO_LED_KEY1_OFF				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_RESET); 
#define PINMACRO_LED_KEY6_ON				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
#define PINMACRO_LED_KEY6_OFF				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
#define PINMACRO_LED_KEYDOORCLOSE_ON		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_SET);
#define PINMACRO_LED_KEYDOORCLOSE_OFF		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_RESET);
#define PINMACRO_LED_KEYDOORBELL_ON			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);
#define PINMACRO_LED_KEYDOORBELL_OFF		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_RESET);

#elif defined (ProjectIs_AutoBarLock_S58Z07) || defined (ProjectIs_AutoBarLock_S68Z04)\
   || defined (ProjectIs_AutoBarLock_S68Z10)
#define PINMACRO_LED_KEYPOUNDSIGN_ON		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);
#define PINMACRO_LED_KEYPOUNDSIGN_OFF		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_RESET);
#define PINMACRO_LED_KEY0_ON				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_SET);
#define PINMACRO_LED_KEY0_OFF				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_RESET);
#define PINMACRO_LED_KEYASTERISK_ON			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_SET);
#define PINMACRO_LED_KEYASTERISK_OFF		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_RESET);
#define PINMACRO_LED_KEY9_ON				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_SET);
#define PINMACRO_LED_KEY9_OFF				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_RESET);
#define PINMACRO_LED_KEY8_ON				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET);
#define PINMACRO_LED_KEY8_OFF				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);
#define PINMACRO_LED_KEY7_ON				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
#define PINMACRO_LED_KEY7_OFF				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
#define PINMACRO_LED_KEY5_ON				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
#define PINMACRO_LED_KEY5_OFF				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
#define PINMACRO_LED_KEY4_ON				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);
#define PINMACRO_LED_KEY4_OFF				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);
#define PINMACRO_LED_KEY3_ON				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_SET);
#define PINMACRO_LED_KEY3_OFF				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_RESET);
#define PINMACRO_LED_KEY2_ON				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET);
#define PINMACRO_LED_KEY2_OFF				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET);
#define PINMACRO_LED_KEY1_ON				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
#define PINMACRO_LED_KEY1_OFF				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
#define PINMACRO_LED_KEY6_ON				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);
#define PINMACRO_LED_KEY6_OFF				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);
#define PINMACRO_LED_KEYDOORCLOSE_ON		//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_SET);
#define PINMACRO_LED_KEYDOORCLOSE_OFF		//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_RESET);
#define PINMACRO_LED_KEYDOORBELL_ON			//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);
#define PINMACRO_LED_KEYDOORBELL_OFF		//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_RESET);

#elif defined (ProjectIs_AutoBarLock_S58Z02)
#define PINMACRO_LED_KEY4_ON		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
#define PINMACRO_LED_KEY4_OFF		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
#define PINMACRO_LED_KEY5_ON				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_SET);
#define PINMACRO_LED_KEY5_OFF				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_RESET);
#define PINMACRO_LED_KEY2_ON			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_SET);
#define PINMACRO_LED_KEY2_OFF		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_RESET);
#define PINMACRO_LED_KEY8_ON				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);
#define PINMACRO_LED_KEY8_OFF				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);
#define PINMACRO_LED_KEY0_ON				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
#define PINMACRO_LED_KEY0_OFF				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
#define PINMACRO_LED_KEY3_ON				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);
#define PINMACRO_LED_KEY3_OFF				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);
#define PINMACRO_LED_KEYASTERISK_ON				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
#define PINMACRO_LED_KEYASTERISK_OFF				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
#define PINMACRO_LED_KEY6_ON				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET);
#define PINMACRO_LED_KEY6_OFF				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);
#define PINMACRO_LED_KEY1_ON				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_SET);
#define PINMACRO_LED_KEY1_OFF				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_RESET);
#define PINMACRO_LED_KEY7_ON				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_SET);
#define PINMACRO_LED_KEY7_OFF				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_RESET);
#define PINMACRO_LED_KEYPOUNDSIGN_ON				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_SET);
#define PINMACRO_LED_KEYPOUNDSIGN_OFF				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_RESET);
#define PINMACRO_LED_KEY9_ON				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);
#define PINMACRO_LED_KEY9_OFF				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_RESET);
#define PINMACRO_LED_KEYDOORCLOSE_ON		//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_SET);
#define PINMACRO_LED_KEYDOORCLOSE_OFF		//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_RESET);
#define PINMACRO_LED_KEYDOORBELL_ON			//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);
#define PINMACRO_LED_KEYDOORBELL_OFF		//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_RESET);

#else 

#endif

#define SET_LED_KEYPOUNDSIGN_ON		LEDsMgr.KeyPoundSignShouldBeOn = bTRUE;
#define SET_LED_KEYPOUNDSIGN_OFF	LEDsMgr.KeyPoundSignShouldBeOn = bFALSE;
#define SET_LED_KEYASTERISK_ON		LEDsMgr.KeyAsteriskShouldBeOn  = bTRUE;
#define SET_LED_KEYASTERISK_OFF		LEDsMgr.KeyAsteriskShouldBeOn  = bFALSE;
#define SET_LED_KEY9_ON				LEDsMgr.Key9ShouldBeOn  = bTRUE;
#define SET_LED_KEY9_OFF			LEDsMgr.Key9ShouldBeOn  = bFALSE;
#define SET_LED_KEY8_ON				LEDsMgr.Key8ShouldBeOn  = bTRUE;
#define SET_LED_KEY8_OFF			LEDsMgr.Key8ShouldBeOn  = bFALSE;
#define SET_LED_KEY7_ON				LEDsMgr.Key7ShouldBeOn  = bTRUE;
#define SET_LED_KEY7_OFF			LEDsMgr.Key7ShouldBeOn  = bFALSE;
#define SET_LED_KEY6_ON				LEDsMgr.Key6ShouldBeOn  = bTRUE;
#define SET_LED_KEY6_OFF			LEDsMgr.Key6ShouldBeOn  = bFALSE;
#define SET_LED_KEY5_ON				LEDsMgr.Key5ShouldBeOn  = bTRUE;
#define SET_LED_KEY5_OFF			LEDsMgr.Key5ShouldBeOn  = bFALSE;
#define SET_LED_KEY4_ON				LEDsMgr.Key4ShouldBeOn  = bTRUE;
#define SET_LED_KEY4_OFF			LEDsMgr.Key4ShouldBeOn  = bFALSE;
#define SET_LED_KEY3_ON				LEDsMgr.Key3ShouldBeOn  = bTRUE;
#define SET_LED_KEY3_OFF			LEDsMgr.Key3ShouldBeOn  = bFALSE;
#define SET_LED_KEY2_ON				LEDsMgr.Key2ShouldBeOn  = bTRUE;
#define SET_LED_KEY2_OFF			LEDsMgr.Key2ShouldBeOn  = bFALSE;
#define SET_LED_KEY1_ON				LEDsMgr.Key1ShouldBeOn  = bTRUE;
#define SET_LED_KEY1_OFF			LEDsMgr.Key1ShouldBeOn  = bFALSE;
#define SET_LED_KEY0_ON				LEDsMgr.Key0ShouldBeOn  = bTRUE;
#define SET_LED_KEY0_OFF			LEDsMgr.Key0ShouldBeOn  = bFALSE;	
#define SET_LED_KEYDOORCLOSE_ON		LEDsMgr.KeyDoorCloseShouldBeOn = bTRUE;
#define SET_LED_KEYDOORCLOSE_OFF	LEDsMgr.KeyDoorCloseShouldBeOn = bFALSE;
#define SET_LED_KEYDOORBELL_ON		LEDsMgr.KeyDoorBellShouldBeOn = bTRUE;
#define SET_LED_KEYDOORBELL_OFF		LEDsMgr.KeyDoorBellShouldBeOn = bFALSE;


#ifdef ProjectIs_AutoBarLock_S08Z05
#define SET_LEDRGB_B_ON				HAL_GPIO_WritePin(GPIOF,GPIO_PIN_0,GPIO_PIN_SET);
#define SET_LEDRGB_B_OFF			HAL_GPIO_WritePin(GPIOF,GPIO_PIN_0,GPIO_PIN_RESET);
#define SET_LEDRGB_R_ON				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET);
#define SET_LEDRGB_R_OFF			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_RESET);
#define SET_LEDRGB_G_ON				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_SET);
#define SET_LEDRGB_G_OFF			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_RESET);

#elif defined (ProjectIs_AutoBarLock_S51Z05) || defined (ProjectIs_AutoBarLock_S51Z06)\
	|| defined (ProjectIs_AutoBarLock_S70Z02)|| defined (ProjectIs_AutoBarLock_S61Z11)\
	|| defined (ProjectIs_AutoBarLock_S64Z07)|| defined (ProjectIs_AutoBarLock_S64Z19)\
	|| defined (ProjectIs_AutoBarLock_S72Z03) || defined ProjectIs_AutoBarLock_S83Z02\
	|| defined (ProjectIs_AutoBarLock_S70Z09)
/******** IO Driver dirrectly without NPN *************/			
#define SET_LEDRGB_R_ON				HAL_GPIO_WritePin(GPIOF,GPIO_PIN_0,GPIO_PIN_RESET);
#define SET_LEDRGB_R_OFF			HAL_GPIO_WritePin(GPIOF,GPIO_PIN_0,GPIO_PIN_SET);
#define SET_LEDRGB_G_ON				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_RESET);
#define SET_LEDRGB_G_OFF			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET);
#define SET_LEDRGB_B_ON				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_RESET);
#define SET_LEDRGB_B_OFF			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_SET);

#elif defined (ProjectIs_AutoBarLock_S83Z01) || defined (ProjectIs_AutoBarLock_S90Z20)\
	||defined (ProjectIs_AutoBarLock_S86Z02)
#define SET_LEDRGB_G_OFF				HAL_GPIO_WritePin(GPIOF,GPIO_PIN_0,GPIO_PIN_RESET);
#define SET_LEDRGB_G_ON			HAL_GPIO_WritePin(GPIOF,GPIO_PIN_0,GPIO_PIN_SET);
#define SET_LEDRGB_R_OFF				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_RESET);
#define SET_LEDRGB_R_ON			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET);
#define SET_LEDRGB_B_OFF				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_RESET);
#define SET_LEDRGB_B_ON			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_SET);

#else
/******** IO Driver with NPN *************/
#define SET_LEDRGB_R_ON				HAL_GPIO_WritePin(GPIOF,GPIO_PIN_0,GPIO_PIN_SET);
#define SET_LEDRGB_R_OFF			HAL_GPIO_WritePin(GPIOF,GPIO_PIN_0,GPIO_PIN_RESET);
#define SET_LEDRGB_G_ON				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET);
#define SET_LEDRGB_G_OFF			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_RESET);
#define SET_LEDRGB_B_ON				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_SET);
#define SET_LEDRGB_B_OFF			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_RESET);
#endif

#if defined (ProjectIs_AutoBarLock_S58Z02) || defined (ProjectIs_AutoBarLock_S49Z06)\
	|| defined (ProjectIs_AutoBarLock_S67Z01)|| defined ( ProjectIs_AutoBarLock_S11Z05)
#define SET_LED_RB_R_ON				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET);
#define SET_LED_RB_R_OFF			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_RESET);
#define SET_LED_RB_B_ON				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);
#define SET_LED_RB_B_OFF			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_RESET);
#elif defined (ProjectIs_AutoBarLock_S08Z05) || defined (ProjectIs_AutoBarLock_S06Z04)\
	|| defined (ProjectIs_AutoBarLock_S31Z01)|| defined (ProjectIs_AutoBarLock_S67Z02)\
	|| defined (ProjectIs_AutoBarLock_S51Z05) || defined (ProjectIs_AutoBarLock_S51Z06)\
	|| defined (ProjectIs_AutoBarLock_S61Z11)
#define SET_LED_RB_R_ON				//
#define SET_LED_RB_R_OFF			//
#define SET_LED_RB_B_ON				//
#define SET_LED_RB_B_OFF			//
#else
#define SET_LED_RB_R_ON				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);
#define SET_LED_RB_R_OFF			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_RESET);
#define SET_LED_RB_B_ON				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET);
#define SET_LED_RB_B_OFF			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_RESET);
#endif

#define SET_FPMPOWER_ON				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET);
#define SET_FPMPOWER_OFF			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET);

#define SET_SPEEKERPOWER_ON			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_SET);
#define SET_SPEEKERPOWER_OFF		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_RESET);

#define SET_DEBUG_TX_H 				
#define SET_DEBUG_TX_L 				
#define PINMACRO_DEBUG_RX_STATUS 	

#define PINMACRO_PICKLOCK_STATUS 	(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_4))

#define SET_AWAKE_INT_H			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_SET)
#define SET_AWAKE_INT_L			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_RESET)
#define SET_AWAKE_INT_Toggle    HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_3)
#define PINMACRO_AWAKE_INT_STATUS		HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_3)

#define SET_LEDPOWER_ON				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_SET);
#define SET_LEDPOWER_OFF			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_RESET);


#define SET_FRMPOWER_ON				HAL_GPIO_WritePin(GPIOF,GPIO_PIN_1,GPIO_PIN_SET);
#define SET_FRMPOWER_OFF			HAL_GPIO_WritePin(GPIOF,GPIO_PIN_1,GPIO_PIN_RESET);

#define SET_PIRPOWER_ON				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_SET);
#define SET_PIRPOWER_OFF			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_RESET);
#define PINMACRO_PIR_STATUS			(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11))

#define SET_WIFIPOWER_ON			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_SET);
#define SET_WIFIPOWER_OFF			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_RESET);
#define SET_DOORBELLAWAKE_L
#define SET_DOORBELLAWAKE_H


//#define SET_RADAR_CTRL1_H			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET);
//#define SET_RADAR_CTRL1_L			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET);
//#define SET_RADAR_CTRL2_H			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_SET);
//#define SET_RADAR_CTRL2_L			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_RESET);


#endif /* #ifndef IO_H */
/* ****************************** END OF FILE ****************************** */
