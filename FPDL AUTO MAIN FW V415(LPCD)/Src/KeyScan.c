#include "StdTypes.h"
#include "IO.h"
#include "Hardware.h"
#include "Project.h"
#include "I2cBus.h"
#include "CSK14.h"

#define Def_KeyHoldTimeLimited 128			//2s
#define Def_KeyHoldLongTimeLimited 	320		//6s


#define CSK14_CS0_Pressed TouchKeyStatus.KEY_ZERO_Pressed
#define CSK14_CS1_Pressed TouchKeyStatus.KEY_ASTERISK_Pressed
#define CSK14_CS2_Pressed TouchKeyStatus.KEY_EIGHT_Pressed
#define CSK14_CS3_Pressed TouchKeyStatus.KEY_SEVEN_Pressed
#define CSK14_CS4_Pressed TouchKeyStatus.KEY_FIVE_Pressed
#define CSK14_CS5_Pressed TouchKeyStatus.KEY_FOUR_Pressed
#define CSK14_CS6_Pressed TouchKeyStatus.KEY_ONE_Pressed
#define CSK14_CS7_Pressed TouchKeyStatus.KEY_TWO_Pressed
#define CSK14_CS8_Pressed TouchKeyStatus.KEY_POUNDSIGN_Pressed
#define CSK14_CS9_Pressed TouchKeyStatus.KEY_NINE_Pressed
#define CSK14_CS10_Pressed TouchKeyStatus.KEY_SIX_Pressed
#define CSK14_CS11_Pressed TouchKeyStatus.KEY_THREE_Pressed
#define CSK14_CS12_Pressed TouchKeyStatus.KEY_DOORCLOSE_Pressed
#define CSK14_CS13_Pressed TouchKeyStatus.KEY_DOORBELL_Pressed



static keycode_t NewKeycode,LastKeycode;
static uint16_t KeyHoldTimeCnt=0;

static uint8_t KEYSCANTIMECNT=0;
static uint8_t TouchKeyStatusBuff[5];

static uint8_t ScanModeRegVlaue;
static uint8_t ChannelEnRegVlaue[2];

bool_t g_TouchAwakeNeedReleaseKey;



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
	bool_t KEY_DOORCLOSE_Pressed;
	bool_t KEY_DOORBELL_Pressed;	
	bool_t KEY_INSIDEBUTTON_Pressed;
	bool_t KEY_SLIDETOUCH_Pressed;
}TouchKeyStatus;


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
	TouchKeyStatus.KEY_DOORCLOSE_Pressed = bFALSE;
}


keycode_t Key_Scan(void)
{

	//uint8_t Buff[2];
	keycode_t PostKeycode;
	uint8_t CHECKSUM;

	I2C_ReadSequential(I2CADDR_CSK14S,0x22,&TouchKeyStatusBuff[0],4);	

	if ((TouchKeyStatusBuff[0]!=0x00)
		||(TouchKeyStatusBuff[1]!=0x00)
		)
	{
		I2C_WriteRandom(I2CADDR_CSK14S,0x1F,0x41);		//Clear IRQ
	}
	else
	{
		if ( SystemPowerMgr.AwakeTimerCnt < 16 ){
			SystemPowerMgr.AwakeTimerCnt++;
		}
		else{
			g_TouchAwakeNeedReleaseKey = bFALSE;
		}
	}

	CHECKSUM = TouchKeyStatusBuff[3]+TouchKeyStatusBuff[0]+TouchKeyStatusBuff[1]+0xC5;	
	if ( CHECKSUM != 0X00 )
	{
		DEBUG_MARK;
		//return KEY_NONE;
	}


	if ((TouchKeyStatusBuff[0]&0x01) != 0x00 ){
		CSK14_CS0_Pressed = bTRUE;
		DEBUG_MARK;
	}
	else{
		CSK14_CS0_Pressed = bFALSE;
	}
	if ((TouchKeyStatusBuff[0]&0x02) != 0x00 ){
		CSK14_CS1_Pressed = bTRUE;
	}
	else{
		CSK14_CS1_Pressed = bFALSE;
	}
	if ((TouchKeyStatusBuff[0]&0x04) != 0x00 ){
		CSK14_CS2_Pressed = bTRUE;
	}
	else{
		CSK14_CS2_Pressed = bFALSE;
	}
	if ((TouchKeyStatusBuff[0]&0x08) != 0x00 ){
		CSK14_CS3_Pressed = bTRUE;
	}
	else{
		CSK14_CS3_Pressed = bFALSE;
	}
	if ((TouchKeyStatusBuff[0]&0x10) != 0x00 ){
		CSK14_CS4_Pressed = bTRUE;
	}
	else{
		CSK14_CS4_Pressed = bFALSE;
	}
	if ((TouchKeyStatusBuff[0]&0x20) != 0x00 ){
		CSK14_CS5_Pressed = bTRUE;
	}
	else{
		CSK14_CS5_Pressed = bFALSE;
	}
	if ((TouchKeyStatusBuff[0]&0x40) != 0x00 ){
		CSK14_CS6_Pressed = bTRUE;
	}
	else{
		CSK14_CS6_Pressed = bFALSE;
	}
	if ((TouchKeyStatusBuff[0]&0x80) != 0x00 ){
		CSK14_CS7_Pressed = bTRUE;
	}
	else{
		CSK14_CS7_Pressed = bFALSE;
	}
	
	if ((TouchKeyStatusBuff[1]&0x01) != 0x00 ){
		CSK14_CS8_Pressed = bTRUE;
	}
	else{
		CSK14_CS8_Pressed = bFALSE;
	}
	if ((TouchKeyStatusBuff[1]&0x02) != 0x00 ){
		CSK14_CS9_Pressed = bTRUE;
	}
	else{
		CSK14_CS9_Pressed = bFALSE;
	}
	if ((TouchKeyStatusBuff[1]&0x04) != 0x00 ){
		CSK14_CS10_Pressed = bTRUE;
	}
	else{
		CSK14_CS10_Pressed = bFALSE;
	}
	if ((TouchKeyStatusBuff[1]&0x08) != 0x00 ){
		CSK14_CS11_Pressed = bTRUE;
	}
	else{
		CSK14_CS11_Pressed = bFALSE;
	}
	if ((TouchKeyStatusBuff[1]&0x10) != 0x00 ){
		CSK14_CS12_Pressed = bTRUE;
	}
	else{
		CSK14_CS12_Pressed = bFALSE;
	}
	if ((TouchKeyStatusBuff[1]&0x20) != 0x00 ){
		CSK14_CS13_Pressed = bTRUE;
	}
	else{
		CSK14_CS13_Pressed = bFALSE;
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
	else if ( (TouchKeyStatus.KEY_DOORCLOSE_Pressed == bTRUE )&&(LastKeycode == KEY_DOORCLOSE)){
		NewKeycode = KEY_DOORCLOSE;
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
	else if ( TouchKeyStatus.KEY_DOORCLOSE_Pressed == bTRUE ){
		NewKeycode = KEY_DOORCLOSE;
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
		if ((PostKeycode != KEY_DOORCLOSE)&&(PostKeycode != KEY_POUNDSIGN))
		{
			PostKeycode = KEY_NONE;	
		}
	}
	
	return PostKeycode;

}
