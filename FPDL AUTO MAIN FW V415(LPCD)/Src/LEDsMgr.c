#include "Project.h"
#include "IO.h"
#include "Hardware.h"
#include "StdTypes.h"
#include "LEDsMgr.h"

static uint16_t LEDsWaterLightTimeCnt;
static uint16_t KeyLEDsFlashTimeCnt;

static uint16_t LEDRGBmgr_CNT;
static struct
{
	uint16_t RledTimeCnt;
	uint16_t BledTimeCnt;
}RBledMgr;


void SET_KEYLED_OFF(keycode_t key)
{
	switch (key)
	{
		case KEY_ZERO:
				SET_LED_KEY0_OFF;
			break;
		case KEY_ONE:
				SET_LED_KEY1_OFF;
			break;
		case KEY_TWO:
				SET_LED_KEY2_OFF;
			break;
		case KEY_THREE:
				SET_LED_KEY3_OFF;	
			break;
		case KEY_FOUR:
				SET_LED_KEY4_OFF;	
			break;
		case KEY_FIVE:
				SET_LED_KEY5_OFF;
			break;
		case KEY_SIX:
				SET_LED_KEY6_OFF;
			break;
		case KEY_SEVEN:
				SET_LED_KEY7_OFF;
			break;
		case KEY_EIGHT:
				SET_LED_KEY8_OFF;
			break;
		case KEY_NINE:
				SET_LED_KEY9_OFF;
			break;
		case KEY_ASTERISK:
				SET_LED_KEYASTERISK_OFF;
			break;
		case KEY_POUNDSIGN:
				SET_LED_KEYPOUNDSIGN_OFF;
			break;
		case KEY_DOORCLOSE:
				SET_LED_KEYDOORCLOSE_OFF;
			break;
		case KEY_DOORBELL:
				SET_LED_KEYDOORBELL_OFF;
			break;
		default:
				break;
        }
}
void SET_KEYLED_ON(keycode_t key)
{
	switch (key)
	{
		case KEY_ZERO:
				SET_LED_KEY0_ON;
			break;
		case KEY_ONE:
				SET_LED_KEY1_ON;
			break;
		case KEY_TWO:
				SET_LED_KEY2_ON;
			break;
		case KEY_THREE:
				SET_LED_KEY3_ON;	
			break;
		case KEY_FOUR:
				SET_LED_KEY4_ON;	
			break;
		case KEY_FIVE:
				SET_LED_KEY5_ON;
			break;
		case KEY_SIX:
				SET_LED_KEY6_ON;
			break;
		case KEY_SEVEN:
				SET_LED_KEY7_ON;
			break;
		case KEY_EIGHT:
				SET_LED_KEY8_ON;
			break;
		case KEY_NINE:
				SET_LED_KEY9_ON;
			break;
		case KEY_ASTERISK:
				SET_LED_KEYASTERISK_ON;
			break;
		case KEY_POUNDSIGN:
				SET_LED_KEYPOUNDSIGN_ON;
			break;
		case KEY_DOORCLOSE:
				SET_LED_KEYDOORCLOSE_ON;
			break;
		case KEY_DOORBELL:
				SET_LED_KEYDOORBELL_ON;
			break;

		default:
				break;
        }
}

/********************************************************************************************************
*  Function: 		                                                           
*  Object: 
********************************************************************************************************/
void SET_12andAPled_ON(void)
{
	SET_LED_KEY1_ON;
	SET_LED_KEY2_ON;
	SET_LED_KEY3_OFF;	
	SET_LED_KEY4_OFF;	
	SET_LED_KEY5_OFF;
	SET_LED_KEY6_OFF;
	SET_LED_KEY7_OFF;
	SET_LED_KEY8_OFF;
	SET_LED_KEY9_OFF;
	SET_LED_KEYASTERISK_ON;
	SET_LED_KEY0_OFF;
	SET_LED_KEYPOUNDSIGN_ON;
	SET_LED_KEYDOORCLOSE_OFF;
	SET_LED_KEYDOORBELL_OFF;
}
/********************************************************************************************************
*  Function: 		                                                           
*  Object: 
********************************************************************************************************/
void SET_123andAPled_ON(void)
{
	SET_LED_KEY1_ON;
	SET_LED_KEY2_ON;
	SET_LED_KEY3_ON;	
	SET_LED_KEY4_OFF;	
	SET_LED_KEY5_OFF;
	SET_LED_KEY6_OFF;
	SET_LED_KEY7_OFF;
	SET_LED_KEY8_OFF;
	SET_LED_KEY9_OFF;
	SET_LED_KEYASTERISK_ON;
	SET_LED_KEY0_OFF;
	SET_LED_KEYPOUNDSIGN_ON;
	SET_LED_KEYDOORCLOSE_OFF;
	SET_LED_KEYDOORBELL_OFF;
}
/********************************************************************************************************
*  Function: 		                                                           
*  Object: 
********************************************************************************************************/
void SET_1234andAPled_ON(void)
{
	SET_LED_KEY1_ON;
	SET_LED_KEY2_ON;
	SET_LED_KEY3_ON;	
	SET_LED_KEY4_ON;	
	SET_LED_KEY5_OFF;
	SET_LED_KEY6_OFF;
	SET_LED_KEY7_OFF;
	SET_LED_KEY8_OFF;
	SET_LED_KEY9_OFF;
	SET_LED_KEYASTERISK_ON;
	SET_LED_KEY0_OFF;
	SET_LED_KEYPOUNDSIGN_ON;
	SET_LED_KEYDOORCLOSE_OFF;
	SET_LED_KEYDOORBELL_OFF;
}

/********************************************************************************************************
*  Function: 		                                                           
*  Object: 
********************************************************************************************************/
void SET_123456andAPled_ON(void)
{
	SET_LED_KEY1_ON;
	SET_LED_KEY2_ON;
	SET_LED_KEY3_ON;	
	SET_LED_KEY4_ON;	
	SET_LED_KEY5_ON;
	SET_LED_KEY6_ON;
	SET_LED_KEY7_OFF;
	SET_LED_KEY8_OFF;
	SET_LED_KEY9_OFF;
	SET_LED_KEYASTERISK_ON;
	SET_LED_KEY0_OFF;
	SET_LED_KEYPOUNDSIGN_ON;
	SET_LED_KEYDOORCLOSE_OFF;
	SET_LED_KEYDOORBELL_OFF;
}
/********************************************************************************************************
*  Function: 		                                                           
*  Object: 
********************************************************************************************************/
void SET_1234567andAPled_ON(void)
{
	SET_LED_KEY1_ON;
	SET_LED_KEY2_ON;
	SET_LED_KEY3_ON;	
	SET_LED_KEY4_ON;	
	SET_LED_KEY5_ON;
	SET_LED_KEY6_ON;
	SET_LED_KEY7_ON;
	SET_LED_KEY8_OFF;
	SET_LED_KEY9_OFF;
	SET_LED_KEYASTERISK_ON;
	SET_LED_KEY0_OFF;
	SET_LED_KEYPOUNDSIGN_ON;
	SET_LED_KEYDOORCLOSE_OFF;
	SET_LED_KEYDOORBELL_OFF;
}

/********************************************************************************************************
*  Function: 		                                                           
*  Object: 
********************************************************************************************************/
void SET_12345678andAPled_ON(void)
{
	SET_LED_KEY1_ON;
	SET_LED_KEY2_ON;
	SET_LED_KEY3_ON;	
	SET_LED_KEY4_ON;	
	SET_LED_KEY5_ON;
	SET_LED_KEY6_ON;
	SET_LED_KEY7_ON;
	SET_LED_KEY8_ON;
	SET_LED_KEY9_OFF;
	SET_LED_KEYASTERISK_ON;
	SET_LED_KEY0_OFF;
	SET_LED_KEYPOUNDSIGN_ON;
	SET_LED_KEYDOORCLOSE_OFF;
	SET_LED_KEYDOORBELL_OFF;
}


/********************************************************************************************************
*  Function: 		                                                           
*  Object: 
*  输入： 无
*  输出： 无 	                                     
*  备注:  无
********************************************************************************************************/
void SET_LRandConfirmLED_ON(void)
{
	SET_LED_KEY1_OFF;
	SET_LED_KEY2_OFF;
	SET_LED_KEY3_OFF;	
	SET_LED_KEY4_ON;	
	SET_LED_KEY5_OFF;
	SET_LED_KEY6_ON;
	SET_LED_KEY7_OFF;
	SET_LED_KEY8_OFF;
	SET_LED_KEY9_OFF;
	SET_LED_KEYASTERISK_OFF;
	SET_LED_KEY0_OFF;
	SET_LED_KEYPOUNDSIGN_ON;
	SET_LED_KEYDOORCLOSE_OFF;
	SET_LED_KEYDOORBELL_OFF;
}
/********************************************************************************************************
*  Function: 		                                                           
*  Object: 
*  输入： 无
*  输出： 无 	                                     
*  备注:  无
********************************************************************************************************/
void SET_UDandConfirmLED_ON(void)
{
	SET_LED_KEY1_OFF;
	SET_LED_KEY2_ON;
	SET_LED_KEY3_OFF;	
	SET_LED_KEY4_OFF;	
	SET_LED_KEY5_OFF;
	SET_LED_KEY6_OFF;
	SET_LED_KEY7_OFF;
	SET_LED_KEY8_ON;
	SET_LED_KEY9_OFF;
	SET_LED_KEYASTERISK_OFF;
	SET_LED_KEY0_OFF;
	SET_LED_KEYPOUNDSIGN_ON;
	SET_LED_KEYDOORCLOSE_OFF;
	SET_LED_KEYDOORBELL_OFF;
}
	
/********************************************************************************************************
*  Function: 		                                                           
*  Object: 
*  输入： 无
*  输出： 无 	                                     
*  备注:  无
********************************************************************************************************/
void SET_UDandAPled_ON(void)
{
	SET_LED_KEY1_OFF;
	SET_LED_KEY2_ON;
	SET_LED_KEY3_OFF;	
	SET_LED_KEY4_OFF;	
	SET_LED_KEY5_OFF;
	SET_LED_KEY6_OFF;
	SET_LED_KEY7_OFF;
	SET_LED_KEY8_ON;
	SET_LED_KEY9_OFF;
	SET_LED_KEYASTERISK_ON;
	SET_LED_KEY0_OFF;
	SET_LED_KEYPOUNDSIGN_ON;
	SET_LED_KEYDOORCLOSE_OFF;
	SET_LED_KEYDOORBELL_OFF;
}
/********************************************************************************************************
*  Function: 		                                                           
*  Object: 
*  输入： 无
*  输出： 无 	                                     
*  备注:  无
********************************************************************************************************/
void SET_UDandLRandConfirmLED_ON(void)
{
	SET_LED_KEY1_OFF;
	SET_LED_KEY2_ON;
	SET_LED_KEY3_OFF;	
	SET_LED_KEY4_ON;	
	SET_LED_KEY5_OFF;
	SET_LED_KEY6_ON;
	SET_LED_KEY7_OFF;
	SET_LED_KEY8_ON;
	SET_LED_KEY9_OFF;
	SET_LED_KEYASTERISK_ON;
	SET_LED_KEY0_OFF;
	SET_LED_KEYPOUNDSIGN_ON;
	SET_LED_KEYDOORCLOSE_OFF;
	SET_LED_KEYDOORBELL_OFF;
}
/********************************************************************************************************
*  Function: 		                                                           
*  Object: 
*  输入： 无
*  输出： 无 	                                     
*  备注:  无
********************************************************************************************************/
void SET_UDandTHREEandConfirmLED_ON(void)
{
	SET_LED_KEY1_OFF;
	SET_LED_KEY2_ON;
	SET_LED_KEY3_ON;	
	SET_LED_KEY4_OFF;	
	SET_LED_KEY5_OFF;
	SET_LED_KEY6_OFF;
	SET_LED_KEY7_OFF;
	SET_LED_KEY8_ON;
	SET_LED_KEY9_OFF;
	SET_LED_KEYASTERISK_ON;
	SET_LED_KEY0_OFF;
	SET_LED_KEYPOUNDSIGN_ON;
	SET_LED_KEYDOORCLOSE_OFF;
	SET_LED_KEYDOORBELL_OFF;
}
/********************************************************************************************************
*  Function: 		                                                           
*  Object: 
*  输入： 无
*  输出： 无 	                                     
*  备注:  无
********************************************************************************************************/
void SET_APled_ON(void)
{
	SET_LED_KEY1_OFF;
	SET_LED_KEY2_OFF;
	SET_LED_KEY3_OFF;	
	SET_LED_KEY4_OFF;	
	SET_LED_KEY5_OFF;
	SET_LED_KEY6_OFF;
	SET_LED_KEY7_OFF;
	SET_LED_KEY8_OFF;
	SET_LED_KEY9_OFF;
	SET_LED_KEYASTERISK_ON;
	SET_LED_KEY0_OFF;
	SET_LED_KEYPOUNDSIGN_ON;
	SET_LED_KEYDOORCLOSE_OFF;
	SET_LED_KEYDOORBELL_OFF;
}


/********************************************************************************************************
*  Function: 		                                                           
*  Object: 
*  输入： 无
*  输出： 无 	                                     
*  备注:  无
********************************************************************************************************/
void SET_AsteriskLED_ON(void)
{
	SET_LED_KEY1_OFF;
	SET_LED_KEY2_OFF;
	SET_LED_KEY3_OFF;	
	SET_LED_KEY4_OFF;	
	SET_LED_KEY5_OFF;
	SET_LED_KEY6_OFF;
	SET_LED_KEY7_OFF;
	SET_LED_KEY8_OFF;
	SET_LED_KEY9_OFF;
	SET_LED_KEYASTERISK_ON;
	SET_LED_KEY0_OFF;
	SET_LED_KEYPOUNDSIGN_OFF;
	SET_LED_KEYDOORCLOSE_OFF;
	SET_LED_KEYDOORBELL_OFF;
}

/********************************************************************************************************
*  Function: 		                                                           
*  Object: 
*  输入： 无
*  输出： 无 	                                     
*  备注:  无
********************************************************************************************************/
void SET_ALLKEYLED_ON(void)
{
	SET_LED_KEY1_ON;
	SET_LED_KEY2_ON;
	SET_LED_KEY3_ON;	
	SET_LED_KEY4_ON;	
	SET_LED_KEY5_ON;
	SET_LED_KEY6_ON;
	SET_LED_KEY7_ON;
	SET_LED_KEY8_ON;
	SET_LED_KEY9_ON;
	SET_LED_KEYASTERISK_ON;
	SET_LED_KEY0_ON;
	SET_LED_KEYPOUNDSIGN_ON;
	SET_LED_KEYDOORCLOSE_ON;
	SET_LED_KEYDOORBELL_ON;
}

/********************************************************************************************************
*  Function: 		                                                           
*  Object: 
*  输入： 无
*  输出： 无 	                                     
*  备注:  无
********************************************************************************************************/
void SET_ALLKEYLED_OFF(void)
{
	SET_LED_KEY1_OFF;
	SET_LED_KEY2_OFF;
	SET_LED_KEY3_OFF;	
	SET_LED_KEY4_OFF;	
	SET_LED_KEY5_OFF;
	SET_LED_KEY6_OFF;
	SET_LED_KEY7_OFF;
	SET_LED_KEY8_OFF;
	SET_LED_KEY9_OFF;
	SET_LED_KEYASTERISK_OFF;
	SET_LED_KEY0_OFF;
	SET_LED_KEYPOUNDSIGN_OFF;
	SET_LED_KEYDOORCLOSE_OFF;
	SET_LED_KEYDOORBELL_OFF;
}
/*******************************/
/*******************************/
void SET_ALLKEYLEDPIN_POWERDOWN(void)
{
	PINMACRO_LED_KEY1_OFF;
	PINMACRO_LED_KEY2_OFF;
	PINMACRO_LED_KEY3_OFF;	
	PINMACRO_LED_KEY4_OFF;	
	PINMACRO_LED_KEY5_OFF;
	PINMACRO_LED_KEY6_OFF;
	PINMACRO_LED_KEY7_OFF;
	PINMACRO_LED_KEY8_OFF;
	PINMACRO_LED_KEY9_OFF;
	PINMACRO_LED_KEYASTERISK_OFF;
	PINMACRO_LED_KEY0_OFF;
	PINMACRO_LED_KEYPOUNDSIGN_OFF;
	PINMACRO_LED_KEYDOORCLOSE_OFF;
	PINMACRO_LED_KEYDOORBELL_OFF;
}

void KEYLED_ASTERISK_Flash(void)
{
	if ( ++KeyLEDsFlashTimeCnt >80 )
	{
		KeyLEDsFlashTimeCnt=0;
	}
	if ( KeyLEDsFlashTimeCnt < 40 )
	{
		SET_ALLKEYLED_OFF();
	}
	else
	{
		SET_LED_KEYASTERISK_ON;
	}
}

#ifdef Function_TouchKeyIsTwoLine
void Enable_KEYLED_WATERLIGHT(void)
{
	LEDsWaterLightTimeCnt = 112;
	SET_RBLED_B_Flash(3);
}
void KEYLED_WATERLIGHT_Task(void)
{
	uint8_t i;
	if ( LEDsWaterLightTimeCnt > 110 )
	{
		SET_ALLKEYLED_OFF();
	}
	else if ( LEDsWaterLightTimeCnt > 0 )
	{
		i=LEDsWaterLightTimeCnt/10;
		if ( (i%6)==5)
		{
			SET_ALLKEYLED_OFF();
			SET_LED_KEYASTERISK_ON;
			SET_LED_KEYPOUNDSIGN_ON;
		}
		else if ( (i%6)==4)
		{
			SET_LED_KEY0_ON;
			SET_LED_KEY9_ON;
		}
		else if ( (i%6)==3)
		{
			SET_LED_KEY8_ON;
			SET_LED_KEY7_ON;
		}
		else if ( (i%6)==2)
		{
			SET_LED_KEY6_ON;
			SET_LED_KEY5_ON;
		}
		else if ( (i%6)==1)
		{
			SET_LED_KEY3_ON;
			SET_LED_KEY4_ON;
		}
		else if ( (i%6)==0)
		{
			SET_LED_KEY1_ON;
			SET_LED_KEY2_ON;
		}
	}
	else
	{
		SET_ALLKEYLED_OFF();
		//KEYLED_ASTERISK_Flash();
	}

	if ( LEDsWaterLightTimeCnt > 0 )
	{
		LEDsWaterLightTimeCnt--;

	}
}

#else
void Enable_KEYLED_WATERLIGHT(void)
{
	LEDsWaterLightTimeCnt = 56;
	SET_RBLED_B_Flash(3);
}
void KEYLED_WATERLIGHT_Task(void)
{
	uint8_t i;
	if ( LEDsWaterLightTimeCnt > 55 )
	{
		SET_ALLKEYLED_OFF();
	}
	else if ( LEDsWaterLightTimeCnt > 0 )
	{
		i=LEDsWaterLightTimeCnt/5;
		if ( (i%12)==11)
		{
			SET_LED_KEY8_ON;
			SET_LED_KEY5_ON;
		}
		else if ( (i%12)==10)
		{
			SET_LED_KEY8_OFF;
			SET_LED_KEY6_ON;
		}
		else if ( (i%12)==9)
		{
			SET_LED_KEY9_ON;
			SET_LED_KEY5_OFF;
		}
		else if ( (i%12)==8)
		{
			SET_LED_KEYPOUNDSIGN_ON;
			SET_LED_KEY6_OFF;
		}
		else if ( (i%12)==7)
		{
			SET_LED_KEY0_ON;
			SET_LED_KEY9_OFF;
		}
		else if ( (i%12)==6)
		{
			SET_LED_KEYASTERISK_ON;
			SET_LED_KEYPOUNDSIGN_OFF;
		}
		else if ( (i%12)==5)
		{
			SET_LED_KEY7_ON;
			SET_LED_KEY0_OFF;
		}
		else if ( (i%12)==4)
		{
			SET_LED_KEY4_ON;
			SET_LED_KEYASTERISK_OFF;
		}
		else if ( (i%12)==3)
		{
			SET_LED_KEY1_ON;
			SET_LED_KEY7_OFF;
		}
		else if ( (i%12)==2)
		{
			SET_LED_KEY2_ON;
			SET_LED_KEY4_OFF;
		}
		else if ( (i%12)==1)
		{
			SET_LED_KEY3_ON;
			SET_LED_KEY1_OFF;
		}
		else if ( (i%12)==0)
		{
			SET_LED_KEY2_OFF;
			SET_LED_KEY3_OFF;
		}
	}
	else
	{
		SET_ALLKEYLED_OFF();
		//KEYLED_ASTERISK_Flash();
	}

	if ( LEDsWaterLightTimeCnt > 0 )
	{
		LEDsWaterLightTimeCnt--;

	}
}
#endif

void SET_RBLED_R_Flash(uint16_t times)
{
	RBledMgr.RledTimeCnt = times*32;
}

void SET_RBLED_B_Flash(uint16_t times)
{
	RBledMgr.BledTimeCnt = times*32;
}

void RB_LEDMgr_Task(void)
{
	if (CurrentScreen == SCREEN_IdentifySuccess)
	{
		SET_LED_RB_R_OFF;
		
		if (RBledMgr.BledTimeCnt > 0 )
		{
			RBledMgr.BledTimeCnt--;
			if (RBledMgr.BledTimeCnt%32==0)
			{
				SET_LED_RB_B_ON;
			}
			else if (RBledMgr.BledTimeCnt%32==16)
			{
				SET_LED_RB_B_OFF;
			}
		}
		else
		{
			SET_LED_RB_B_ON;
		}
	}
	else if (CurrentScreen == SCREEN_IdentifyFail)
	{
		SET_LED_RB_B_OFF;
		if (RBledMgr.RledTimeCnt > 0 )
		{
			RBledMgr.RledTimeCnt--;
			if (RBledMgr.RledTimeCnt%32==16)
			{
				SET_LED_RB_R_ON;
			}
			else if (RBledMgr.RledTimeCnt%32==0)
			{
				SET_LED_RB_R_OFF;
			}
		}
		else
		{
			SET_LED_RB_R_OFF;
			SET_LED_RB_B_ON;
		}
	}
	else
	{
		SET_LED_RB_R_OFF;
		SET_LED_RB_B_ON;
	}
	
}

#if 0
void RGB_LEDMgr_Task(void)
{
	if (CurrentScreen == SCREEN_IdentifySuccess)
	{
		if ( ++LEDRGBmgr_CNT > 96 )
		{
			LEDRGBmgr_CNT=0x0000;
		}

		if ( (ComportMgr.DoorStatus == Openning) 
			||(ComportMgr.DoorStatus == Open) 
			||(ComportMgr.DoorStatus == OpenedWaitClose) 
			)
		{
			if ( LEDRGBmgr_CNT <24 )
			{
				SET_LEDRGB_R_ON;
				SET_LEDRGB_B_OFF;
				SET_LEDRGB_G_OFF;	
			}
			else if ( LEDRGBmgr_CNT < 48 )
			{
				SET_LEDRGB_R_ON;
				SET_LEDRGB_B_ON;
				SET_LEDRGB_G_OFF;
			}
			else if ( LEDRGBmgr_CNT < 72 )
			{
				SET_LEDRGB_R_ON;
				SET_LEDRGB_G_ON;
				SET_LEDRGB_B_ON;
			}
			else
			{
				SET_LEDRGB_R_OFF;
				SET_LEDRGB_B_OFF;
				SET_LEDRGB_G_OFF;	
			}
		}
		else if ( (ComportMgr.DoorStatus == Closing) 
			||(ComportMgr.DoorStatus == Close) 
			)
		{
			if ( LEDRGBmgr_CNT <24 )
			{
				SET_LEDRGB_R_ON;
				SET_LEDRGB_B_ON;
				SET_LEDRGB_G_ON;	
			}
			else if ( LEDRGBmgr_CNT < 48 )
			{
				SET_LEDRGB_R_ON;
				SET_LEDRGB_B_ON;
				SET_LEDRGB_G_OFF;	

			}
			else if ( LEDRGBmgr_CNT < 72 )
			{
				SET_LEDRGB_R_ON;
				SET_LEDRGB_B_OFF;
				SET_LEDRGB_G_OFF;	
			}
			else
			{
				SET_LEDRGB_R_OFF;
				SET_LEDRGB_B_OFF;
				SET_LEDRGB_G_OFF;	
			}
		}
		else
		{
			SET_LEDRGB_R_ON;
			SET_LEDRGB_G_ON;
			SET_LEDRGB_B_ON;
		}
		
	}
	else{
		SET_LEDRGB_R_ON;
		SET_LEDRGB_G_ON;
		SET_LEDRGB_B_ON;
	}
	
}
#endif

void RGB_LEDMgr_Task(void)
{
	if ( CurrentScreen == SCREEN_IdentifySuccess )
	{
		SET_LEDRGB_G_ON;
		SET_LEDRGB_R_OFF;
		SET_LEDRGB_B_OFF;
	}
	else if ( CurrentScreen == SCREEN_IdentifyFail )
	{
		SET_LEDRGB_R_ON;
		SET_LEDRGB_B_OFF;
		SET_LEDRGB_G_OFF;	
	}
	else
	{
		SET_LEDRGB_B_ON;
		SET_LEDRGB_R_OFF;
		SET_LEDRGB_G_OFF;	
	}
}



void SET_LED_RGB_OFF(void)
{
	SET_LEDRGB_R_OFF;
	SET_LEDRGB_G_OFF;
	SET_LEDRGB_B_OFF;
}
void SET_LED_RB_OFF(void)
{
	SET_LED_RB_R_OFF;
	SET_LED_RB_B_OFF;
}


void LEDsMgr_Init(void)
{
	LEDsMgr.Key0ShouldBeOn = bFALSE;
	LEDsMgr.Key1ShouldBeOn = bFALSE;
	LEDsMgr.Key2ShouldBeOn = bFALSE;
	LEDsMgr.Key3ShouldBeOn = bFALSE;
	LEDsMgr.Key4ShouldBeOn = bFALSE;
	LEDsMgr.Key5ShouldBeOn = bFALSE;
	LEDsMgr.Key6ShouldBeOn = bFALSE;
	LEDsMgr.Key7ShouldBeOn = bFALSE;
	LEDsMgr.Key8ShouldBeOn = bFALSE;
	LEDsMgr.Key9ShouldBeOn = bFALSE;
	LEDsMgr.KeyAsteriskShouldBeOn = bFALSE;
	LEDsMgr.KeyPoundSignShouldBeOn = bFALSE;
	LEDsMgr.KeyDoorCloseShouldBeOn = bFALSE;
	LEDsMgr.KeyDoorBellShouldBeOn = bFALSE;

	LEDsMgr.Keycode = KEY_NONE;

	LEDRGBmgr_CNT = 0X0000;
}
void KEYLEDsMgr_Task(void)
{
	if (( LEDsMgr.Key0ShouldBeOn  == bTRUE)&&(LEDsMgr.Keycode != KEY_ZERO)){
		PINMACRO_LED_KEY0_ON;
	}else{
		PINMACRO_LED_KEY0_OFF;
	}	
	if ((LEDsMgr.Key1ShouldBeOn  == bTRUE )&&(LEDsMgr.Keycode != KEY_ONE)){
		PINMACRO_LED_KEY1_ON;
	}else{
		PINMACRO_LED_KEY1_OFF;
	}
	if (( LEDsMgr.Key3ShouldBeOn  == bTRUE)&&(LEDsMgr.Keycode != KEY_THREE)){
		PINMACRO_LED_KEY3_ON;
	}else{
		PINMACRO_LED_KEY3_OFF;
	}

	if (( LEDsMgr.Key5ShouldBeOn  == bTRUE)&&(LEDsMgr.Keycode != KEY_FIVE)){
		PINMACRO_LED_KEY5_ON;
	}else{
		PINMACRO_LED_KEY5_OFF;
	}			
	if (( LEDsMgr.Key6ShouldBeOn  == bTRUE)&&(LEDsMgr.Keycode != KEY_SIX)){
		PINMACRO_LED_KEY6_ON;
	}else{
		PINMACRO_LED_KEY6_OFF;
	}

	if (( LEDsMgr.Key4ShouldBeOn  == bTRUE)&&(LEDsMgr.Keycode != KEY_FOUR)){
		PINMACRO_LED_KEY4_ON;
	}else{
		PINMACRO_LED_KEY4_OFF;
	}	
	if (( LEDsMgr.Key8ShouldBeOn  == bTRUE)&&(LEDsMgr.Keycode != KEY_EIGHT)){
		PINMACRO_LED_KEY8_ON;
	}else{
		PINMACRO_LED_KEY8_OFF;
	}		
	if (( LEDsMgr.Key2ShouldBeOn  == bTRUE)&&(LEDsMgr.Keycode != KEY_TWO)){
		PINMACRO_LED_KEY2_ON;
	}else{
		PINMACRO_LED_KEY2_OFF;
	}
	if (( LEDsMgr.Key9ShouldBeOn == bTRUE)&&(LEDsMgr.Keycode != KEY_NINE)){
		PINMACRO_LED_KEY9_ON;
	}else{
		PINMACRO_LED_KEY9_OFF;
	}	
	if (( LEDsMgr.Key7ShouldBeOn  == bTRUE)&&(LEDsMgr.Keycode != KEY_SEVEN)){
		PINMACRO_LED_KEY7_ON;
	}else{
		PINMACRO_LED_KEY7_OFF;
	}			
	if (( LEDsMgr.KeyAsteriskShouldBeOn  == bTRUE)&&(LEDsMgr.Keycode != KEY_ASTERISK)){
		PINMACRO_LED_KEYASTERISK_ON;
	}else{
		PINMACRO_LED_KEYASTERISK_OFF;
	}
	if (( LEDsMgr.KeyPoundSignShouldBeOn  == bTRUE)&&(LEDsMgr.Keycode != KEY_POUNDSIGN)){
		PINMACRO_LED_KEYPOUNDSIGN_ON;
	}else{
		PINMACRO_LED_KEYPOUNDSIGN_OFF;
	}	
	if (( LEDsMgr.KeyDoorCloseShouldBeOn  == bTRUE)&&(LEDsMgr.Keycode != KEY_DOORCLOSE)){
		PINMACRO_LED_KEYDOORCLOSE_ON;
	}else{
		PINMACRO_LED_KEYDOORCLOSE_OFF;
	}	
	if (( LEDsMgr.KeyDoorBellShouldBeOn  == bTRUE)&&(LEDsMgr.Keycode != KEY_DOORBELL)){
		PINMACRO_LED_KEYDOORBELL_ON;
	}else{
		PINMACRO_LED_KEYDOORBELL_OFF;
	}
}

void LEDsMgr_Task(void)
{
	KEYLEDsMgr_Task();
	RB_LEDMgr_Task();
	RGB_LEDMgr_Task();
}



