#include "Project.h"
#include "IO.h"
#include "Hardware.h"
#include "ISR.h"
#include "StdTypes.h"
#include "I2cBus.h"
#include "TSM12S.h"

#ifdef Function_TouchUsed_TMS12S


#define Def_KeyHoldTimeLimited 128			//2s
#define Def_KeyHoldLongTimeLimited 	320		//6s

#define TSM12S_SensitvityReg1_Addr  0x02
#define TSM12S_SensitvityReg2_Addr  0x03
#define TSM12S_SensitvityReg3_Addr  0x06
#define TSM12S_SensitvityReg4_Addr  0x07
#define TSM12S_SensitvityReg5_Addr  0x22
#define TSM12S_SensitvityReg6_Addr  0x23


//#define TSM12S_CONFIG_REG9 	0x03
#define TSM12S_CONFIG_REG10 	0x00
#define TSM12S_CONFIG_REG11 	0x00
#define TSM12S_CONFIG_REG12 	0x00
#define TSM12S_CONFIG_REG13 	0x00
#define TSM12S_CONFIG_REG14 	0x00
#define TSM12S_CONFIG_REG15 	0x00

#ifdef ProjectIs_AutoBarLock_S58Z02
#define TSM12S_CONFIG_REG2 	0x99		//BIT2~0 for KEY_9 ; BIT6~BIT4 for KEY_#
#define TSM12S_CONFIG_REG3 	0x98		//BIT2~0 for KEY_* ; BIT6~BIT4 for KEY_0
#define TSM12S_CONFIG_REG4 	0x99		//BIT2~0 for KEY_8 ; BIT6~BIT4 for KEY_7
#define TSM12S_CONFIG_REG5 	0x99		//BIT2~0 for KEY_4 ; BIT6~BIT4 for KEY_5
#define TSM12S_CONFIG_REG6 	0x99		//BIT2~0 for KEY_6 ; BIT6~BIT4 for KEY_1
#define TSM12S_CONFIG_REG7 	0x99		//BIT2~0 for KEY_2 ; BIT6~BIT4 for KEY_3

#define TSM12S_CONFIG_REG8 	0xA0


#define TSM12S_CONFIG_REG2_SLEEP 	0xAA		
#define TSM12S_CONFIG_REG3_SLEEP 	0xA8		
#define TSM12S_CONFIG_REG4_SLEEP 	0xAA		
#define TSM12S_CONFIG_REG5_SLEEP 	0xAA		
#define TSM12S_CONFIG_REG6_SLEEP 	0xAA		
#define TSM12S_CONFIG_REG7_SLEEP 	0xAA

#define TSM12S_CONFIG_REG8_SLEEP 	0x28


#elif defined (ProjectIs_AutoBarLock_S49Z06)

#define TSM12S_CONFIG_REG2 	0xCA		//BIT2~0 for KEY_9 ; BIT6~BIT4 for KEY_#
#define TSM12S_CONFIG_REG3 	0xCC		//BIT2~0 for KEY_* ; BIT6~BIT4 for KEY_0
#define TSM12S_CONFIG_REG4 	0xBC		//BIT2~0 for KEY_8 ; BIT6~BIT4 for KEY_7
#define TSM12S_CONFIG_REG5 	0xCB		//BIT2~0 for KEY_4 ; BIT6~BIT4 for KEY_5
#define TSM12S_CONFIG_REG6 	0xCB		//BIT2~0 for KEY_6 ; BIT6~BIT4 for KEY_1
#define TSM12S_CONFIG_REG7 	0xCC		//BIT2~0 for KEY_2 ; BIT6~BIT4 for KEY_3
#define TSM12S_CONFIG_REG8 	0xA0


#define TSM12S_CONFIG_REG2_SLEEP 	0xDB		
#define TSM12S_CONFIG_REG3_SLEEP 	0xDD		
#define TSM12S_CONFIG_REG4_SLEEP 	0xDD		
#define TSM12S_CONFIG_REG5_SLEEP 	0xDD		
#define TSM12S_CONFIG_REG6_SLEEP 	0xDD		
#define TSM12S_CONFIG_REG7_SLEEP 	0xDD

#define TSM12S_CONFIG_REG8_SLEEP 	0x2B


#endif

#if defined (ProjectIs_AutoBarLock_S58Z02) || defined (ProjectIs_AutoBarLock_S49Z06)
#define TSMC12_CS1_Pressed TouchKeyStatus.KEY_NINE_Pressed
#define TSMC12_CS2_Pressed TouchKeyStatus.KEY_POUNDSIGN_Pressed
#define TSMC12_CS3_Pressed TouchKeyStatus.KEY_ASTERISK_Pressed
#define TSMC12_CS4_Pressed TouchKeyStatus.KEY_ZERO_Pressed
#define TSMC12_CS5_Pressed TouchKeyStatus.KEY_EIGHT_Pressed
#define TSMC12_CS6_Pressed TouchKeyStatus.KEY_SEVEN_Pressed
#define TSMC12_CS7_Pressed TouchKeyStatus.KEY_FOUR_Pressed
#define TSMC12_CS8_Pressed TouchKeyStatus.KEY_FIVE_Pressed
#define TSMC12_CS9_Pressed TouchKeyStatus.KEY_SIX_Pressed
#define TSMC12_CS10_Pressed TouchKeyStatus.KEY_ONE_Pressed
#define TSMC12_CS11_Pressed TouchKeyStatus.KEY_TWO_Pressed
#define TSMC12_CS12_Pressed TouchKeyStatus.KEY_THREE_Pressed
#else

#endif

static struct
{
	bool_t KEY_ZERO_Pressed;
	bool_t KEY_ONE_Pressed;
	bool_t KEY_TWO_Pressed;
	bool_t KEY_THREE_Pressed;
	bool_t KEY_FOUR_Pressed;
	bool_t KEY_FIVE_Pressed;
	bool_t KEY_SIX_Pressed;
	bool_t KEY_SEVEN_Pressed;
	bool_t KEY_EIGHT_Pressed;
	bool_t KEY_NINE_Pressed;
	bool_t KEY_POUNDSIGN_Pressed;
	bool_t KEY_ASTERISK_Pressed;
	bool_t KEY_INSIDEBUTTON_Pressed;
	bool_t KEY_SLIDETOUCH_Pressed;
	bool_t KEY_DOORBELLTOUCH_Pressed;
	bool_t KEY_DOORCLOSE_Pressed;
	bool_t KEY_SPARE_Pressed;
}TouchKeyStatus;

static keycode_t NewKeycode,LastKeycode;
static uint16_t KeyHoldTimeCnt=0;

static uint8_t KEYSCANTIMECNT=0;
static uint8_t Buff[3];

bool_t g_TouchAwakeNeedReleaseKey;

//static uint8_t TestVal;
//static uint8_t TestBuff[20];

void TouchKeyStatus_Reset(void)
{
	TouchKeyStatus.KEY_ZERO_Pressed = bFALSE;
	TouchKeyStatus.KEY_ONE_Pressed = bFALSE;
	TouchKeyStatus.KEY_TWO_Pressed = bFALSE;
	TouchKeyStatus.KEY_THREE_Pressed = bFALSE;
	TouchKeyStatus.KEY_FOUR_Pressed = bFALSE;
	TouchKeyStatus.KEY_FIVE_Pressed = bFALSE;
	TouchKeyStatus.KEY_SIX_Pressed = bFALSE;
	TouchKeyStatus.KEY_SEVEN_Pressed = bFALSE;
	TouchKeyStatus.KEY_EIGHT_Pressed = bFALSE;
	TouchKeyStatus.KEY_NINE_Pressed = bFALSE;
	TouchKeyStatus.KEY_POUNDSIGN_Pressed = bFALSE;
	TouchKeyStatus.KEY_ASTERISK_Pressed = bFALSE;
	TouchKeyStatus.KEY_INSIDEBUTTON_Pressed = bFALSE;
	TouchKeyStatus.KEY_SLIDETOUCH_Pressed = bFALSE;
	TouchKeyStatus.KEY_DOORBELLTOUCH_Pressed = bFALSE;
	TouchKeyStatus.KEY_DOORCLOSE_Pressed = bFALSE;
	LastKeycode = NewKeycode = KEY_NONE;
}
void TOUCH_Init(void)
{
//	return;
	SET_KB_I2C_EN_H;
	Hardware_DelayMs(3);
	SET_KB_I2C_EN_L;
	Hardware_DelayMs(8);
	
	I2C_WriteRandom(I2CADDR_TSM12S,0x09,0x2B);		//Enable SOFT RESET
	Hardware_DelayMs(1);
	I2C_WriteRandom(I2CADDR_TSM12S,0x09,0x23);		//Disable SOFT RESET,Disable sleep mode
	Hardware_DelayMs(3);
        
  SET_TOUCH_AWAKE_SENSITIVITY();
        
	I2C_WriteRandom(I2CADDR_TSM12S,0x0C,TSM12S_CONFIG_REG12);		//
	I2C_WriteRandom(I2CADDR_TSM12S,0x0D,TSM12S_CONFIG_REG13);		//
	I2C_WriteRandom(I2CADDR_TSM12S,0x0E,TSM12S_CONFIG_REG14);		//
	I2C_WriteRandom(I2CADDR_TSM12S,0x0F,TSM12S_CONFIG_REG15);		//
        
    I2C_WriteRandom(I2CADDR_TSM12S,0x0A,TSM12S_CONFIG_REG10);		//
	I2C_WriteRandom(I2CADDR_TSM12S,0x0B,TSM12S_CONFIG_REG11);		//
        
	//SET_KB_I2C_EN_H;
	Hardware_DelayMs(5);

	TouchPowerMgr.Status = HighSensitivity;
	
	DEBUG_MARK;
}

void SET_TOUCH_NO_SENSING(void)
{
	I2C_WriteRandom(I2CADDR_TSM12S,0x0C,0xFF); //
	I2C_WriteRandom(I2CADDR_TSM12S,0x0D,0xFF); //
	I2C_WriteRandom(I2CADDR_TSM12S,0x08,0x27); //
	TouchPowerMgr.Status = NoSensing;
}

void SET_TOUCH_SLEEP_SENSITIVITY(void)
{

	I2C_WriteRandom(I2CADDR_TSM12S,TSM12S_SensitvityReg1_Addr,TSM12S_CONFIG_REG2_SLEEP);	  //
	I2C_WriteRandom(I2CADDR_TSM12S,TSM12S_SensitvityReg2_Addr,TSM12S_CONFIG_REG3_SLEEP);	  //
	I2C_WriteRandom(I2CADDR_TSM12S,TSM12S_SensitvityReg3_Addr,TSM12S_CONFIG_REG4_SLEEP);	  //`
	I2C_WriteRandom(I2CADDR_TSM12S,TSM12S_SensitvityReg4_Addr,TSM12S_CONFIG_REG5_SLEEP);	  //
	I2C_WriteRandom(I2CADDR_TSM12S,TSM12S_SensitvityReg5_Addr,TSM12S_CONFIG_REG6_SLEEP);	  //
	I2C_WriteRandom(I2CADDR_TSM12S,TSM12S_SensitvityReg6_Addr,TSM12S_CONFIG_REG7_SLEEP);	  //
	I2C_WriteRandom(I2CADDR_TSM12S,0x08,TSM12S_CONFIG_REG8_SLEEP);	  //
	
	TouchPowerMgr.Status = LowSensitivity;

}
void SET_TOUCH_AWAKE_SENSITIVITY(void)
{
	uint8_t TempBuff[7];
	
//	if ( TouchPowerMgr.Status == NoSensing )	//if TSM12 Sensing be Disabled,turn on sensing
	{
		do
		{
		  I2C_WriteRandom(I2CADDR_TSM12S,0x0C,0x00); //
		  I2C_WriteRandom(I2CADDR_TSM12S,0x0D,0x00); //
		  I2C_ReadSequential(I2CADDR_TSM12S,0x0C,TempBuff,2);	
		}
		while(	(TempBuff[0]!=0x00)
				||(TempBuff[1]!=0x00)
			);
	}

	do
	{
		I2C_WriteRandom(I2CADDR_TSM12S,TSM12S_SensitvityReg1_Addr,TSM12S_CONFIG_REG2);		//
		I2C_WriteRandom(I2CADDR_TSM12S,TSM12S_SensitvityReg2_Addr,TSM12S_CONFIG_REG3);		//
		I2C_WriteRandom(I2CADDR_TSM12S,TSM12S_SensitvityReg3_Addr,TSM12S_CONFIG_REG4);		//`
		I2C_WriteRandom(I2CADDR_TSM12S,TSM12S_SensitvityReg4_Addr,TSM12S_CONFIG_REG5);		//
		I2C_WriteRandom(I2CADDR_TSM12S,TSM12S_SensitvityReg5_Addr,TSM12S_CONFIG_REG6);		//
		I2C_WriteRandom(I2CADDR_TSM12S,TSM12S_SensitvityReg6_Addr,TSM12S_CONFIG_REG7);		//
		I2C_WriteRandom(I2CADDR_TSM12S,0x08,TSM12S_CONFIG_REG8);		// 

		//read back to check
		I2C_ReadRandom(I2CADDR_TSM12S,TSM12S_SensitvityReg1_Addr,&TempBuff[0]);		//
		I2C_ReadRandom(I2CADDR_TSM12S,TSM12S_SensitvityReg2_Addr,&TempBuff[1]);		//
		I2C_ReadRandom(I2CADDR_TSM12S,TSM12S_SensitvityReg3_Addr,&TempBuff[2]);		//`
		I2C_ReadRandom(I2CADDR_TSM12S,TSM12S_SensitvityReg4_Addr,&TempBuff[3]);		//
		I2C_ReadRandom(I2CADDR_TSM12S,TSM12S_SensitvityReg5_Addr,&TempBuff[4]);		//
		I2C_ReadRandom(I2CADDR_TSM12S,TSM12S_SensitvityReg6_Addr,&TempBuff[5]);		//
		I2C_ReadRandom(I2CADDR_TSM12S,0x08,&TempBuff[6]);		// 	
			
	}
	while	(	(TempBuff[0]!=TSM12S_CONFIG_REG2)
			||	(TempBuff[1]!=TSM12S_CONFIG_REG3)
			||	(TempBuff[2]!=TSM12S_CONFIG_REG4)
			||	(TempBuff[3]!=TSM12S_CONFIG_REG5)
			||	(TempBuff[4]!=TSM12S_CONFIG_REG6)
			||	(TempBuff[5]!=TSM12S_CONFIG_REG7)
			||	(TempBuff[6]!=TSM12S_CONFIG_REG8)
			);

	TouchPowerMgr.Status = HighSensitivity;

	
}

void TOUCH_POWERDOWN(void)
{
//	return;
	SET_TOUCH_SLEEP_SENSITIVITY();    
	I2C_WriteRandom(I2CADDR_TSM12S,0x09,0x27);		//Disable SOFT RESET,Enable sleep mode
	Hardware_DelayMs(1);
	TouchKeyStatus_Reset();
	//SET_KB_I2C_EN_H;
}

void TOUCH_AWAKE(void)
{
//	return;
	SET_KB_I2C_EN_L;
	Hardware_DelayMs(3);
      
    SET_TOUCH_AWAKE_SENSITIVITY();
    I2C_WriteRandom(I2CADDR_TSM12S,0x09,0x23);       //Disable SOFT RESET,Disable sleep mode

}

void Key_Scan_Init(void)
{
	TouchKeyStatus.KEY_ZERO_Pressed = bFALSE;
	TouchKeyStatus.KEY_ONE_Pressed = bFALSE;
	TouchKeyStatus.KEY_TWO_Pressed = bFALSE;
	TouchKeyStatus.KEY_THREE_Pressed = bFALSE;
	TouchKeyStatus.KEY_FOUR_Pressed = bFALSE;
	TouchKeyStatus.KEY_FIVE_Pressed = bFALSE;
	TouchKeyStatus.KEY_SIX_Pressed = bFALSE;
	TouchKeyStatus.KEY_SEVEN_Pressed = bFALSE;
	TouchKeyStatus.KEY_EIGHT_Pressed = bFALSE;
	TouchKeyStatus.KEY_NINE_Pressed = bFALSE;
	TouchKeyStatus.KEY_POUNDSIGN_Pressed = bFALSE;
	TouchKeyStatus.KEY_ASTERISK_Pressed = bFALSE;
	TouchKeyStatus.KEY_INSIDEBUTTON_Pressed = bFALSE;
	TouchKeyStatus.KEY_SLIDETOUCH_Pressed = bFALSE;
}


keycode_t Key_Scan(void)
{

	//uint8_t Buff[2];
	keycode_t PostKeycode;
	

	I2C_ReadRandom(I2CADDR_TSM12S,0x10,&Buff[0]);
	I2C_ReadRandom(I2CADDR_TSM12S,0x12,&Buff[1]);
	I2C_ReadRandom(I2CADDR_TSM12S,0x13,&Buff[2]);

	if ( (Buff[0]==0x00 )
		&&(Buff[1]==0x00 )
		&&(Buff[2]==0x00 )
		)
	{
		if ( SystemPowerMgr.AwakeTimerCnt < 16 ){
			SystemPowerMgr.AwakeTimerCnt++;
		}
		else{
			g_TouchAwakeNeedReleaseKey = bFALSE;
		}
	}
	/*
	if ( (Buff[0]&0xC0)==0xC0 ){
		TSMC12_CS4_Pressed = bTRUE;
	}
	else if ( (Buff[0]&0xC0)<0x80 ){
		TSMC12_CS4_Pressed = bFALSE;
	}
	if ( (Buff[0]&0x30)== 0x30 ){
		TSMC12_CS3_Pressed = bTRUE;
	}
	else if ( (Buff[0]&0x30)<0x20 ){
		TSMC12_CS3_Pressed = bFALSE;
	}
	if ( (Buff[0]&0x0C)==0x0C ){
		TSMC12_CS2_Pressed = bTRUE;
	}
	else if ( (Buff[0]&0x0C)<0x08 ){
		TSMC12_CS2_Pressed = bFALSE;
	}
	if ( (Buff[0]&0x03)==0x03 ){
		TSMC12_CS1_Pressed = bTRUE;
	}
	else if ( (Buff[0]&0x03)<0x02 ){
		TSMC12_CS1_Pressed = bFALSE;
	}
	if ( (Buff[1]&0xC0)==0xC0 ){
		TSMC12_CS8_Pressed = bTRUE;
	}
	else if ( (Buff[1]&0xC0)<0x80 ){
		TSMC12_CS8_Pressed = bFALSE;
	}
	if ( (Buff[1]&0x30)==0x30 ){
		TSMC12_CS7_Pressed = bTRUE;
	}
	else if ( (Buff[1]&0x30)<0x20 ){
		TSMC12_CS7_Pressed = bFALSE;
	}
	if ( (Buff[1]&0x0C)==0x0C ){
		TSMC12_CS6_Pressed = bTRUE;
	}
	else if ( (Buff[1]&0x0C)<0x08 ){
		TSMC12_CS6_Pressed = bFALSE;
	}
	if ( (Buff[1]&0x03)==0x03 ){
		TSMC12_CS5_Pressed = bTRUE;
	}
	else if ( (Buff[1]&0x03)<0x02 ){
		TSMC12_CS5_Pressed = bFALSE;
	}
	if ( (Buff[2]&0xC0)==0xC0 ){
		TSMC12_CS12_Pressed = bTRUE;
	}
	else if ( (Buff[2]&0xC0)<0x80 ){
		TSMC12_CS12_Pressed = bFALSE;
	}
	if ( (Buff[2]&0x30)==0x30 ){
		TSMC12_CS11_Pressed = bTRUE;
	}
	else if ( (Buff[2]&0x30)<0x20 ){
		TSMC12_CS11_Pressed = bFALSE;
	}
	if ( (Buff[2]&0x0C)==0x0C ){
		TSMC12_CS10_Pressed = bTRUE;
	}
	else if ( (Buff[2]&0x0C)<0x08 ){
		TSMC12_CS10_Pressed = bFALSE;
	}
	if ( (Buff[2]&0x03)==0x03 ){
		TSMC12_CS9_Pressed = bTRUE;
	}
	else if ( (Buff[2]&0x03)<0x02 ){
		TSMC12_CS9_Pressed = bFALSE;
	}
	*/

	if ( (Buff[0]&0xC0)!=0x00 ){
		TSMC12_CS4_Pressed = bTRUE;
	}
	else{
		TSMC12_CS4_Pressed = bFALSE;
	}
	if ( (Buff[0]&0x30)!= 0x00 ){
		TSMC12_CS3_Pressed = bTRUE;
	}
	else{
		TSMC12_CS3_Pressed = bFALSE;
	}
	if ( (Buff[0]&0x0C)!= 0x00 ){
		TSMC12_CS2_Pressed = bTRUE;
	}
	else{
		TSMC12_CS2_Pressed = bFALSE;
	}
	if ( (Buff[0]&0x03)!= 0x00 ){
		TSMC12_CS1_Pressed = bTRUE;
	}
	else {
		TSMC12_CS1_Pressed = bFALSE;
	}
	if ( (Buff[1]&0xC0)!= 0x00 ){
		TSMC12_CS8_Pressed = bTRUE;
	}
	else {
		TSMC12_CS8_Pressed = bFALSE;
	}
	if ( (Buff[1]&0x30)!= 0x00 ){
		TSMC12_CS7_Pressed = bTRUE;
	}
	else {
		TSMC12_CS7_Pressed = bFALSE;
	}
	if ( (Buff[1]&0x0C)!= 0x00 ){
		TSMC12_CS6_Pressed = bTRUE;
	}
	else{
		TSMC12_CS6_Pressed = bFALSE;
	}
	if ( (Buff[1]&0x03)!= 0x00 ){
		TSMC12_CS5_Pressed = bTRUE;
	}
	else{
		TSMC12_CS5_Pressed = bFALSE;
	}
	if ( (Buff[2]&0xC0)!= 0x00 ){
		TSMC12_CS12_Pressed = bTRUE;
	}
	else{
		TSMC12_CS12_Pressed = bFALSE;
	}
	if ( (Buff[2]&0x30)!= 0x00 ){
		TSMC12_CS11_Pressed = bTRUE;
	}
	else{
		TSMC12_CS11_Pressed = bFALSE;
	}
	if ( (Buff[2]&0x0C)!= 0x00 ){
		TSMC12_CS10_Pressed = bTRUE;
	}
	else{
		TSMC12_CS10_Pressed = bFALSE;
	}
	if ( (Buff[2]&0x03)!= 0x00 ){
		TSMC12_CS9_Pressed = bTRUE;
	}
	else{
		TSMC12_CS9_Pressed = bFALSE;
	}	
		
	if ( (TouchKeyStatus.KEY_ZERO_Pressed == bTRUE )&&(LastKeycode == KEY_ZERO)){
		NewKeycode = KEY_ZERO;
	}
	else if ( (TouchKeyStatus.KEY_ONE_Pressed == bTRUE )&&(LastKeycode == KEY_ONE)){
		NewKeycode = KEY_ONE;
	}
	else if ( (TouchKeyStatus.KEY_TWO_Pressed == bTRUE )&&(LastKeycode == KEY_TWO)){
		NewKeycode = KEY_TWO;
	}
	else if ( (TouchKeyStatus.KEY_THREE_Pressed == bTRUE )&&(LastKeycode == KEY_THREE)){
		NewKeycode = KEY_THREE;
	}
	else if ( (TouchKeyStatus.KEY_FOUR_Pressed == bTRUE )&&(LastKeycode == KEY_FOUR)){
		NewKeycode = KEY_FOUR;
	}
	else if ( (TouchKeyStatus.KEY_FIVE_Pressed == bTRUE )&&(LastKeycode == KEY_FIVE)){
		NewKeycode = KEY_FIVE;
	}
	else if ( (TouchKeyStatus.KEY_SIX_Pressed == bTRUE )&&(LastKeycode == KEY_SIX)){
		NewKeycode = KEY_SIX;
	}
	else if ( (TouchKeyStatus.KEY_SEVEN_Pressed == bTRUE )&&(LastKeycode == KEY_SEVEN)){
		NewKeycode = KEY_SEVEN;
	}
	else if ( (TouchKeyStatus.KEY_EIGHT_Pressed == bTRUE )&&(LastKeycode == KEY_EIGHT)){
		NewKeycode = KEY_EIGHT;
	}
	else if ( (TouchKeyStatus.KEY_NINE_Pressed == bTRUE )&&(LastKeycode == KEY_NINE)){
		NewKeycode = KEY_NINE;
	}
	else if ( (TouchKeyStatus.KEY_POUNDSIGN_Pressed == bTRUE )&&(LastKeycode == KEY_POUNDSIGN)){
		NewKeycode = KEY_POUNDSIGN;
	}
	else if ( (TouchKeyStatus.KEY_ASTERISK_Pressed == bTRUE )&&(LastKeycode == KEY_ASTERISK)){
		NewKeycode = KEY_ASTERISK;
	}
	else if ( TouchKeyStatus.KEY_ZERO_Pressed == bTRUE ){
		NewKeycode = KEY_ZERO;
	}
	else if ( TouchKeyStatus.KEY_ONE_Pressed == bTRUE ){
		NewKeycode = KEY_ONE;
	}
	else if ( TouchKeyStatus.KEY_TWO_Pressed == bTRUE ){
		NewKeycode = KEY_TWO;
	}
	else if ( TouchKeyStatus.KEY_THREE_Pressed == bTRUE ){
		NewKeycode = KEY_THREE;
	}
	else if ( TouchKeyStatus.KEY_FOUR_Pressed == bTRUE ){
		NewKeycode = KEY_FOUR;
	}
	else if ( TouchKeyStatus.KEY_FIVE_Pressed == bTRUE ){
		NewKeycode = KEY_FIVE;
	}
	else if ( TouchKeyStatus.KEY_SIX_Pressed == bTRUE ){
		NewKeycode = KEY_SIX;
	}
	else if ( TouchKeyStatus.KEY_SEVEN_Pressed == bTRUE ){
		NewKeycode = KEY_SEVEN;
	}
	else if ( TouchKeyStatus.KEY_EIGHT_Pressed == bTRUE ){
		NewKeycode = KEY_EIGHT;
	}
	else if ( TouchKeyStatus.KEY_NINE_Pressed == bTRUE ){
		NewKeycode = KEY_NINE;
	}
	else if ( TouchKeyStatus.KEY_POUNDSIGN_Pressed == bTRUE ){
		NewKeycode = KEY_POUNDSIGN;
	}
	else if ( TouchKeyStatus.KEY_ASTERISK_Pressed == bTRUE ){
		NewKeycode = KEY_ASTERISK;
	}
	else {
		NewKeycode = KEY_NONE;
	}	

	LEDsMgr.Keycode = NewKeycode;

	if ( NewKeycode != KEY_NONE )
	{
		if ( NewKeycode !=LastKeycode )
		{
			if (NewKeycode != KEY_INSIDEBUTTON)
			{
				PostKeycode = NewKeycode;
			}
			else{
				PostKeycode = KEY_NONE; 
				DEBUG_MARK;
			}
			LastKeycode = NewKeycode;
			KeyHoldTimeCnt=0;
		}
		else
		{
			if ( KeyHoldTimeCnt < Def_KeyHoldLongTimeLimited )
			{
				KeyHoldTimeCnt++;
				PostKeycode = KEY_NONE;			
			}

			if ( KeyHoldTimeCnt > Def_KeyHoldTimeLimited )
			{
				if ((KeyHoldTimeCnt == Def_KeyHoldLongTimeLimited )&&( NewKeycode == KEY_INSIDEBUTTON))
				{
					PostKeycode = KEY_INSIDEBUTTON_HOLD_LONG;				
				}
				/*
				else if ( KEYSCANTIMECNT%4==0)
				{
					if ( NewKeycode == KEY_ASTERISK)
					{
						PostKeycode = KEY_ASTERISK_HOLD;				
					}
					else if ( NewKeycode == KEY_POUNDSIGN)
					{
						PostKeycode = KEY_POUNDSIGN_HOLD;
					}
					else if ( NewKeycode == KEY_TWO)
					{
						PostKeycode = KEY_TWO_HOLD;				
					}
					else if ( NewKeycode == KEY_EIGHT)
					{
						PostKeycode = KEY_EIGHT_HOLD;
					}
					else
					{
						PostKeycode = KEY_NONE;
					}
				}
				*/
				else{

					PostKeycode = KEY_NONE;
				}
				
				if ( ++KEYSCANTIMECNT > 127 ){
					KEYSCANTIMECNT=0;
				}
			}
			else{
				PostKeycode = KEY_NONE;
			}
		}
	}
	else
	{
		if (LastKeycode == KEY_INSIDEBUTTON){
			PostKeycode = LastKeycode;
		}
		else{
			PostKeycode = KEY_NONE;	
		}
		LastKeycode = NewKeycode;
		KeyHoldTimeCnt=0;
	}

	if (g_TouchAwakeNeedReleaseKey == bTRUE )
	{
		if (PostKeycode == KEY_ASTERISK)
		{
			PostKeycode = KEY_DOORCLOSE;
			DEBUG_MARK;
		}
		else if (PostKeycode == KEY_POUNDSIGN)
		{
			PostKeycode = KEY_POUNDSIGN;
			DEBUG_MARK;
		}
		else
		{
			PostKeycode = KEY_NONE;	
		}
	}

	return PostKeycode;

}

#endif



