#include "Project.h"
#include "FM5114_Touch.h"
#include "Hardware.h"
#include "gpio.h"
#include "IO.h"
#include "I2cBus.h"

#ifdef Function_TouchUsed_FM5114



//unsigned char Touch_IRQ;

#if defined (ProjectIs_AutoBarLock_S70Z02) || defined (ProjectIs_AutoBarLock_S70Z09)

#define DEF_FM5114_ChannelConfig	0x3FFF	//from channel0~channel13
#define DEF_FM5114_TouchSens		0x01	//


#define FM5114_SENS_CH0 		40		//KEY 5
#define FM5114_SENS_CH1 		40		//KEY 7
#define FM5114_SENS_CH2 		70		//KEY 9
#define FM5114_SENS_CH3 		108		//KEY Doorbell
#define FM5114_SENS_CH4 		90		//KEY *
#define FM5114_SENS_CH5 		60		//KEY 3
#define FM5114_SENS_CH6 		60		//KEY 1
#define FM5114_SENS_CH7 		60		//KEY 2
#define FM5114_SENS_CH8 		60		//KEY 4
#define FM5114_SENS_CH9 		40		//KEY 6
#define FM5114_SENS_CH10 		90		//KEY Doorclose
#define FM5114_SENS_CH11 		90		//KEY #
#define FM5114_SENS_CH12 		70		//KEY 0
#define FM5114_SENS_CH13 		40		//KEY 8

#define FM5114_CS0_Pressed TouchKeyStatus.KEY_FIVE_Pressed 
#define FM5114_CS1_Pressed TouchKeyStatus.KEY_SEVEN_Pressed  
#define FM5114_CS2_Pressed TouchKeyStatus.KEY_NINE_Pressed 
#define FM5114_CS3_Pressed TouchKeyStatus.KEY_DOORBELL_Pressed 
#define FM5114_CS4_Pressed TouchKeyStatus.KEY_ASTERISK_Pressed 
#define FM5114_CS5_Pressed TouchKeyStatus.KEY_THREE_Pressed 
#define FM5114_CS6_Pressed TouchKeyStatus.KEY_ONE_Pressed 
#define FM5114_CS7_Pressed TouchKeyStatus.KEY_TWO_Pressed 
#define FM5114_CS8_Pressed TouchKeyStatus.KEY_FOUR_Pressed 
#define FM5114_CS9_Pressed TouchKeyStatus.KEY_SIX_Pressed 
#define FM5114_CS10_Pressed TouchKeyStatus.KEY_DOORCLOSE_Pressed 
#define FM5114_CS11_Pressed TouchKeyStatus.KEY_POUNDSIGN_Pressed 
#define FM5114_CS12_Pressed TouchKeyStatus.KEY_ZERO_Pressed
#define FM5114_CS13_Pressed TouchKeyStatus.KEY_EIGHT_Pressed


#elif defined ProjectIs_AutoBarLock_S83Z02

#define DEF_FM5114_ChannelConfig	0x3FFF	//from channel0~channel13
#define DEF_FM5114_TouchSens		0x01	//

#define FM5114_SENS_CH0 		40		//KEY 5
#define FM5114_SENS_CH1 		40		//KEY 7
#define FM5114_SENS_CH2 		70		//KEY 9
#define FM5114_SENS_CH3 		90		//KEY Doorbell
#define FM5114_SENS_CH4 		90		//KEY *
#define FM5114_SENS_CH5 		60		//KEY 3
#define FM5114_SENS_CH6 		60		//KEY 1
#define FM5114_SENS_CH7 		60		//KEY 2
#define FM5114_SENS_CH8 		60		//KEY 4
#define FM5114_SENS_CH9 		40		//KEY 6
#define FM5114_SENS_CH10 		90		//KEY Doorclose
#define FM5114_SENS_CH11 		90		//KEY #
#define FM5114_SENS_CH12 		70		//KEY 0
#define FM5114_SENS_CH13 		40		//KEY 8

#define FM5114_CS0_Pressed TouchKeyStatus.KEY_FIVE_Pressed 
#define FM5114_CS1_Pressed TouchKeyStatus.KEY_SEVEN_Pressed  
#define FM5114_CS2_Pressed TouchKeyStatus.KEY_NINE_Pressed 
#define FM5114_CS3_Pressed TouchKeyStatus.KEY_DOORCLOSE_Pressed 
#define FM5114_CS4_Pressed TouchKeyStatus.KEY_ASTERISK_Pressed 
#define FM5114_CS5_Pressed TouchKeyStatus.KEY_THREE_Pressed 
#define FM5114_CS6_Pressed TouchKeyStatus.KEY_ONE_Pressed 
#define FM5114_CS7_Pressed TouchKeyStatus.KEY_TWO_Pressed 
#define FM5114_CS8_Pressed TouchKeyStatus.KEY_FOUR_Pressed 
#define FM5114_CS9_Pressed TouchKeyStatus.KEY_SIX_Pressed 
#define FM5114_CS10_Pressed TouchKeyStatus.KEY_DOORBELL_Pressed
#define FM5114_CS11_Pressed TouchKeyStatus.KEY_POUNDSIGN_Pressed 
#define FM5114_CS12_Pressed TouchKeyStatus.KEY_ZERO_Pressed
#define FM5114_CS13_Pressed TouchKeyStatus.KEY_EIGHT_Pressed


#elif defined ProjectIs_AutoBarLock_S72Z03
#define DEF_FM5114_ChannelConfig	0x3FFF	//from channel0~channel13
#define DEF_FM5114_TouchSens		0x01	//


#define FM5114_SENS_CH0 		40		//KEY 5
#define FM5114_SENS_CH1 		40		//KEY 7
#define FM5114_SENS_CH2 		70		//KEY 9
#define FM5114_SENS_CH3 		90		//KEY Doorbell
#define FM5114_SENS_CH4 		90		//KEY *
#define FM5114_SENS_CH5 		60		//KEY 3
#define FM5114_SENS_CH6 		60		//KEY 1
#define FM5114_SENS_CH7 		60		//KEY 2
#define FM5114_SENS_CH8 		60		//KEY 4
#define FM5114_SENS_CH9 		40		//KEY 6
#define FM5114_SENS_CH10 		90		//KEY Doorclose
#define FM5114_SENS_CH11 		90		//KEY #
#define FM5114_SENS_CH12 		70		//KEY 0
#define FM5114_SENS_CH13 		40		//KEY 8

#define FM5114_CS0_Pressed TouchKeyStatus.KEY_FIVE_Pressed 
#define FM5114_CS1_Pressed TouchKeyStatus.KEY_SEVEN_Pressed  
#define FM5114_CS2_Pressed TouchKeyStatus.KEY_NINE_Pressed 
#define FM5114_CS3_Pressed TouchKeyStatus.KEY_DOORBELL_Pressed 
#define FM5114_CS4_Pressed TouchKeyStatus.KEY_ASTERISK_Pressed 
#define FM5114_CS5_Pressed TouchKeyStatus.KEY_THREE_Pressed 
#define FM5114_CS6_Pressed TouchKeyStatus.KEY_ONE_Pressed 
#define FM5114_CS7_Pressed TouchKeyStatus.KEY_TWO_Pressed 
#define FM5114_CS8_Pressed TouchKeyStatus.KEY_FOUR_Pressed 
#define FM5114_CS9_Pressed TouchKeyStatus.KEY_SIX_Pressed 
#define FM5114_CS10_Pressed TouchKeyStatus.KEY_DOORCLOSE_Pressed 
#define FM5114_CS11_Pressed TouchKeyStatus.KEY_POUNDSIGN_Pressed 
#define FM5114_CS12_Pressed TouchKeyStatus.KEY_ZERO_Pressed
#define FM5114_CS13_Pressed TouchKeyStatus.KEY_EIGHT_Pressed


#elif defined ProjectIs_AutoBarLock_S20Z04

#define DEF_FM5114_ChannelConfig	0x3F3F	//from channel0~channel11
#define DEF_FM5114_TouchSens		0x01	//

#define FM5114_SENS_CH0 		60		//KEY 
#define FM5114_SENS_CH1 		60		//
#define FM5114_SENS_CH2 		60		//
#define FM5114_SENS_CH3 		60		//
#define FM5114_SENS_CH4 		60		//
#define FM5114_SENS_CH5 		60		//
#define FM5114_SENS_CH6 		60		//
#define FM5114_SENS_CH7 		60		//
#define FM5114_SENS_CH8 		60		//
#define FM5114_SENS_CH9 		60		//
#define FM5114_SENS_CH10 		60		//
#define FM5114_SENS_CH11 		60		//
#define FM5114_SENS_CH12 		60		//
#define FM5114_SENS_CH13 		60		//

#define FM5114_CS5_Pressed TouchKeyStatus.KEY_NINE_Pressed 
#define FM5114_CS2_Pressed TouchKeyStatus.KEY_POUNDSIGN_Pressed  
#define FM5114_CS0_Pressed TouchKeyStatus.KEY_ASTERISK_Pressed
#define FM5114_CS1_Pressed TouchKeyStatus.KEY_ZERO_Pressed
#define FM5114_CS4_Pressed TouchKeyStatus.KEY_EIGHT_Pressed 
#define FM5114_CS3_Pressed TouchKeyStatus.KEY_SEVEN_Pressed 
#define FM5114_CS11_Pressed TouchKeyStatus.KEY_FOUR_Pressed
#define FM5114_CS10_Pressed TouchKeyStatus.KEY_FIVE_Pressed 
#define FM5114_CS8_Pressed TouchKeyStatus.KEY_SIX_Pressed 
#define FM5114_CS13_Pressed TouchKeyStatus.KEY_ONE_Pressed 
#define FM5114_CS12_Pressed TouchKeyStatus.KEY_TWO_Pressed 
#define FM5114_CS9_Pressed TouchKeyStatus.KEY_THREE_Pressed 
#define FM5114_CS6_Pressed TouchKeyStatus.KEY_SPARE_Pressed
#define FM5114_CS7_Pressed TouchKeyStatus.KEY_SPARE_Pressed

#elif defined (ProjectIs_AutoBarLock_S68Z04) || defined (ProjectIs_AutoBarLock_S68Z10)

#define DEF_FM5114_ChannelConfig	0x3F3F	//from channel0~channel11
#define DEF_FM5114_TouchSens		0x01	//

#define FM5114_SENS_CH0 		40		//KEY 
#define FM5114_SENS_CH1 		40		//
#define FM5114_SENS_CH2 		40		//
#define FM5114_SENS_CH3 		36		//
#define FM5114_SENS_CH4 		40		//
#define FM5114_SENS_CH5 		40		//
#define FM5114_SENS_CH6 		40		//
#define FM5114_SENS_CH7 		40		//
#define FM5114_SENS_CH8 		40		//
#define FM5114_SENS_CH9 		40		//
#define FM5114_SENS_CH10 		40		//
#define FM5114_SENS_CH11 		38		//
#define FM5114_SENS_CH12 		45		//
#define FM5114_SENS_CH13 		38		//

#define FM5114_CS12_Pressed TouchKeyStatus.KEY_ZERO_Pressed
#define FM5114_CS2_Pressed TouchKeyStatus.KEY_ONE_Pressed 
#define FM5114_CS1_Pressed TouchKeyStatus.KEY_TWO_Pressed 
#define FM5114_CS0_Pressed TouchKeyStatus.KEY_THREE_Pressed
#define FM5114_CS5_Pressed TouchKeyStatus.KEY_FOUR_Pressed
#define FM5114_CS4_Pressed TouchKeyStatus.KEY_FIVE_Pressed 
#define FM5114_CS3_Pressed TouchKeyStatus.KEY_SIX_Pressed 
#define FM5114_CS8_Pressed TouchKeyStatus.KEY_SEVEN_Pressed 
#define FM5114_CS10_Pressed TouchKeyStatus.KEY_EIGHT_Pressed 
#define FM5114_CS11_Pressed TouchKeyStatus.KEY_NINE_Pressed 
#define FM5114_CS9_Pressed TouchKeyStatus.KEY_ASTERISK_Pressed
#define FM5114_CS13_Pressed TouchKeyStatus.KEY_POUNDSIGN_Pressed  
#define FM5114_CS6_Pressed TouchKeyStatus.KEY_SPARE_Pressed
#define FM5114_CS7_Pressed TouchKeyStatus.KEY_SPARE_Pressed



#elif defined ProjectIs_AutoBarLock_S58Z07

#define DEF_FM5114_ChannelConfig	0x3F3F	//from 
#define DEF_FM5114_TouchSens		0x01	//

#define FM5114_SENS_CH0 		50		//KEY 3
#define FM5114_SENS_CH1 		50		// 2
#define FM5114_SENS_CH2 		50		// 1
#define FM5114_SENS_CH3 		50		// 6
#define FM5114_SENS_CH4 		50		// 5
#define FM5114_SENS_CH5 		35		// 4
#define FM5114_SENS_CH6 		60		// spare
#define FM5114_SENS_CH7 		60		// spare
#define FM5114_SENS_CH8 		35		// 7
#define FM5114_SENS_CH9 		68		// *
#define FM5114_SENS_CH10 		40		// 8
#define FM5114_SENS_CH11 		40		// 9
#define FM5114_SENS_CH12 		50		// 0
#define FM5114_SENS_CH13 		50		// #

#define FM5114_CS12_Pressed TouchKeyStatus.KEY_ZERO_Pressed
#define FM5114_CS2_Pressed TouchKeyStatus.KEY_ONE_Pressed 
#define FM5114_CS1_Pressed TouchKeyStatus.KEY_TWO_Pressed 
#define FM5114_CS0_Pressed TouchKeyStatus.KEY_THREE_Pressed
#define FM5114_CS5_Pressed TouchKeyStatus.KEY_FOUR_Pressed
#define FM5114_CS4_Pressed TouchKeyStatus.KEY_FIVE_Pressed 
#define FM5114_CS3_Pressed TouchKeyStatus.KEY_SIX_Pressed 
#define FM5114_CS8_Pressed TouchKeyStatus.KEY_SEVEN_Pressed 
#define FM5114_CS10_Pressed TouchKeyStatus.KEY_EIGHT_Pressed 
#define FM5114_CS11_Pressed TouchKeyStatus.KEY_NINE_Pressed 
#define FM5114_CS9_Pressed TouchKeyStatus.KEY_ASTERISK_Pressed
#define FM5114_CS13_Pressed TouchKeyStatus.KEY_POUNDSIGN_Pressed  
#define FM5114_CS6_Pressed TouchKeyStatus.KEY_SPARE_Pressed
#define FM5114_CS7_Pressed TouchKeyStatus.KEY_SPARE_Pressed



#endif





unsigned char FM5114_TOUCH_CS_LUT[50] = {1, 3, 4, 5, 7, 8, 10, 11, 12, 14, 15, 16, 18, 19, 20, 22, 23, 24, 26, 27, 29, 30, 31, 33, 34, 35, 37, 38, 39, 41, 42, 44, 45, 46, 48, 49, 50, 52, 53, 54, 56, 57, 58, 60};
unsigned char FM5114_TOUCH_SEN_LUT1[60] = {10, 11, 11, 12, 12, 12, 13, 13, 13, 13, 13, 13, 13, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15};
unsigned char FM5114_TOUCH_SEN_LUT2[60] = {9, 10, 10, 11, 11, 11, 12, 12, 12, 12, 12, 12, 12, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 15, 15, 15, 15, 15};
unsigned char FM5114_TOUCH_SEN_LUT3[60] = {9, 9, 10, 10, 10, 11, 11, 11, 11, 11, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14};
unsigned char FM5114_TOUCH_SEN_LUT4[60] = {9, 9, 9, 10, 10, 10, 11, 11, 11, 11, 11, 11, 11, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 14, 14, 14, 14, 14};

unsigned char FM5114_TOUCH_REGADR[] = {
	0x08, 0x09, 0x7E, 0x9D, 0x95, 0x87, 0x88, 0x9D, 0x95, 0xA6, //BUTTON0
	0x08, 0x09, 0x7E, 0x9D, 0x95, 0x87, 0x88, 0x9D, 0x95, 0xA7, //BUTTON1
	0x08, 0x09, 0x7E, 0x9E, 0x96, 0x87, 0x88, 0x9E, 0x96, 0xA8, //BUTTON2
	0x08, 0x09, 0x7E, 0x9E, 0x96, 0x87, 0x88, 0x9E, 0x96, 0xA9, //BUTTON3
	0x08, 0x09, 0x7E, 0x9F, 0x97, 0x87, 0x88, 0x9F, 0x97, 0xAA, //BUTTON4
	0x08, 0x09, 0x7E, 0x9F, 0x97, 0x87, 0x88, 0x9F, 0x97, 0xAB, //BUTTON5
	0x08, 0x09, 0x7E, 0xA0, 0x98, 0x87, 0x88, 0xA0, 0x98, 0xAC, //BUTTON6
	0x08, 0x09, 0x7E, 0xA0, 0x98, 0x87, 0x88, 0xA0, 0x98, 0xAD, //BUTTON7
	0x08, 0x09, 0x7E, 0xA1, 0x99, 0x87, 0x88, 0xA1, 0x99, 0xAE, //BUTTON8
	0x08, 0x09, 0x7E, 0xA1, 0x99, 0x87, 0x88, 0xA1, 0x99, 0xAF, //BUTTON9
	0x08, 0x09, 0x7E, 0xA2, 0x9A, 0x87, 0x88, 0xA2, 0x9A, 0xB0, //BUTTON10
	0x08, 0x09, 0x7E, 0xA2, 0x9A, 0x87, 0x88, 0xA2, 0x9A, 0xB1, //BUTTON11
	0x08, 0x09, 0x7E, 0xA3, 0x9B, 0x87, 0x88, 0xA3, 0x9B, 0xB2, //BUTTON12
	0x08, 0x09, 0x7E, 0xA3, 0x9B, 0x87, 0x88, 0xA3, 0x9B, 0xB3	//BUTTON13
};

unsigned char FM5114_TOUCH_REGVAL[] = {
	0x01, 0x00, 0x00, 0xF0, 0x03, 0xF0, 0x04, 0xF0, 0x01, 0x02, 0x03, 0x04, 0xF0, 0x00, //BUTTON0
	0x02, 0x00, 0x01, 0x0F, 0x30, 0x0F, 0x40, 0x0F, 0x10, 0x20, 0x30, 0x40, 0x0F, 0x04, //BUTTON1
	0x04, 0x00, 0x02, 0xF0, 0x03, 0xF0, 0x04, 0xF0, 0x01, 0x02, 0x03, 0x04, 0xF0, 0x00, //BUTTON2
	0x08, 0x00, 0x03, 0x0F, 0x30, 0x0F, 0x40, 0x0F, 0x10, 0x20, 0x30, 0x40, 0x0F, 0x04, //BUTTON3
	0x10, 0x00, 0x04, 0xF0, 0x03, 0xF0, 0x04, 0xF0, 0x01, 0x02, 0x03, 0x04, 0xF0, 0x00, //BUTTON4
	0x20, 0x00, 0x05, 0x0F, 0x30, 0x0F, 0x40, 0x0F, 0x10, 0x20, 0x30, 0x40, 0x0F, 0x04, //BUTTON5
	0x40, 0x00, 0x06, 0xF0, 0x03, 0xF0, 0x04, 0xF0, 0x01, 0x02, 0x03, 0x04, 0xF0, 0x00, //BUTTON6
	0x80, 0x00, 0x07, 0x0F, 0x30, 0x0F, 0x40, 0x0F, 0x10, 0x20, 0x30, 0x40, 0x0F, 0x04, //BUTTON7
	0x00, 0x01, 0x08, 0xF0, 0x03, 0xF0, 0x04, 0xF0, 0x01, 0x02, 0x03, 0x04, 0xF0, 0x00, //BUTTON8
	0x00, 0x02, 0x09, 0x0F, 0x30, 0x0F, 0x40, 0x0F, 0x10, 0x20, 0x30, 0x40, 0x0F, 0x04, //BUTTON9
	0x00, 0x04, 0x0A, 0xF0, 0x03, 0xF0, 0x04, 0xF0, 0x01, 0x02, 0x03, 0x04, 0xF0, 0x00, //BUTTON10
	0x00, 0x08, 0x0B, 0x0F, 0x30, 0x0F, 0x40, 0x0F, 0x10, 0x20, 0x30, 0x40, 0x0F, 0x04, //BUTTON11
	0x00, 0x10, 0x0C, 0xF0, 0x03, 0xF0, 0x04, 0xF0, 0x01, 0x02, 0x03, 0x04, 0xF0, 0x00, //BUTTON12
	0x00, 0x20, 0x0D, 0x0F, 0x30, 0x0F, 0x40, 0x0F, 0x10, 0x20, 0x30, 0x40, 0x0F, 0x04	//BUTTON13
};

unsigned char FM5114_TOUCH_SENSEMODE_REGADR[] = {
	0xA6, 0xB7, 0x95, 0x9D, //BUTTON0
	0xA7, 0xB8, 0x95, 0x9D, //BUTTON1
	0xA8, 0xB9, 0x96, 0x9E, //BUTTON2
	0xA9, 0xBA, 0x96, 0x9E, //BUTTON3
	0xAA, 0xBB, 0x97, 0x9F, //BUTTON4
	0xAB, 0xBC, 0x97, 0x9F, //BUTTON5
	0xAC, 0xBD, 0x98, 0xA0, //BUTTON6
	0xAD, 0xBE, 0x98, 0xA0, //BUTTON7
	0xAE, 0xBF, 0x99, 0xA1, //BUTTON8
	0xAF, 0xC0, 0x99, 0xA1, //BUTTON9
	0xB0, 0xC1, 0x9A, 0xA2, //BUTTON10
	0xB1, 0xC2, 0x9A, 0xA2, //BUTTON11
	0xB2, 0xC3, 0x9B, 0xA3, //BUTTON12
	0xB3, 0xC4, 0x9B, 0xA3	//BUTTON13
};

unsigned char FM5114_TOUCH_SENSEMODE_REGVAL[] = {
	0x0F, 0x01, 0x08, //BUTTON0
	0xF0, 0x10, 0x80, //BUTTON1
	0x0F, 0x01, 0x08, //BUTTON2
	0xF0, 0x10, 0x80, //BUTTON3
	0x0F, 0x01, 0x08, //BUTTON4
	0xF0, 0x10, 0x80, //BUTTON5
	0x0F, 0x01, 0x08, //BUTTON6
	0xF0, 0x10, 0x80, //BUTTON7
	0x0F, 0x01, 0x08, //BUTTON8
	0xF0, 0x10, 0x80, //BUTTON9
	0x0F, 0x01, 0x08, //BUTTON10
	0xF0, 0x10, 0x80, //BUTTON11
	0x0F, 0x01, 0x08, //BUTTON12
	0xF0, 0x10, 0x80  //BUTTON13
};




uint8_t FM5114_TESTBUF[10];

static uint8_t testValue1;
static uint8_t testValue2;

unsigned char CsButton[14] = {0};
unsigned char Num;

//unsigned char Cs_prox;
//unsigned char keyflag;
//unsigned char touchnum=0;
//unsigned int i;

#define SENSE_MODE 1 /* 按键传感器增强模式配置 */

keycode_t GetNewTouchKey(void);


void FM5114_Touch_Reset(void) //FM5114 
{
	SET_FM5114_TOUCH_RST_L
	Hardware_DelayMs(50);
	SET_FM5114_TOUCH_RST_H;
	Hardware_DelayMs(350);
}

unsigned char FM5114_Reader_ReadOneByte(unsigned char address,unsigned char reg,unsigned char *reg_data)
{
	unsigned char RevData;
	I2C_ReadRandom(address,reg,&RevData);
	*reg_data = RevData;
	return RevData;
}
void FM5114_Reader_WriteOneByte(unsigned char address,unsigned char reg, unsigned char data)
{
	I2C_WriteRandom(address,reg,data);
}

//***********************************************
//函数名称：GetReg(unsigned char device_address,unsigned char reg_address,unsigned char *reg_data)
//函数功能：读取寄存器值
//入口参数：device_address：器件地址，reg_address：寄存器地址，reg_data：读取的值
//出口参数：FM5114_SUCCESS：成功   FM5114_COMM_ERR:失败
//***********************************************
unsigned char GetReg(unsigned char device_address, unsigned char reg_address, unsigned char *reg_data)
{

	//FM5114_Reader_ReadOneByte(device_address, reg_address, reg_data);

	I2C_ReadRandom(device_address,reg_address,reg_data);

	return FM5114_SUCCESS;
}

//***********************************************
//函数名称：SetReg(unsigned char device_address,unsigned char reg_address,unsigned char reg_data)
//函数功能：写寄存器
//入口参数：device_address：器件地址，reg_address：寄存器地址，reg_data：读取的值
//出口参数：FM5114_SUCCESS：成功   FM5114_COMM_ERR:失败
//***********************************************
unsigned char SetReg(unsigned char device_address, unsigned char reg_address, unsigned char reg_data)
{

	//FM5114_Reader_WriteOneByte(device_address, reg_address, reg_data);

	I2C_WriteRandom(device_address, reg_address, reg_data);

	return FM5114_SUCCESS;
}

//*******************************************************
//??????????ModifyReg(unsigned char addr,unsigned char* mask,unsigned char set)
//?????????????????÷
//??????????addr:??±ê?????÷???·   mask:????±?????
//         set:  0:±ê??????????   ???ü:±ê??????????
//??????????
//********************************************************
void ModifyReg(unsigned char device_address, unsigned char reg_address, unsigned char mask, unsigned char set)
{
	unsigned char reg_data;

	GetReg(device_address, reg_address, &reg_data);

	if (set)
	{
		reg_data |= mask;
	}
	else
	{
		reg_data &= ~mask;
	}

	SetReg(device_address, reg_address, reg_data);
	return;
}


unsigned char TrimLoad_PowerOn(void)
{
	unsigned char reg;

	Hardware_DelayMs(10); //Trim????????????LDO???????è?·±?VCC????=1.35V~1.65V

	if (GetReg(I2CADDR_FM5114_TOUCH, 0x5F, &reg) != FM5114_SUCCESS)
		return FM5114_COMM_ERR;
	if (SetReg(I2CADDR_FM5114_TOUCH, 0x5F, reg | 0x01) != FM5114_SUCCESS) //Trim????
		return FM5114_COMM_ERR;

	return FM5114_SUCCESS;
}

unsigned char TrimLoad_PowerOff(void)
{
	unsigned char reg;

	Hardware_DelayMs(10);

	if (GetReg(I2CADDR_FM5114_TOUCH, 0x5F, &reg) != FM5114_SUCCESS)
		return FM5114_COMM_ERR;
	if (SetReg(I2CADDR_FM5114_TOUCH, 0x5F, reg & 0xFE) != FM5114_SUCCESS) //TrimLoad????
		return FM5114_COMM_ERR;

	return FM5114_SUCCESS;
}
unsigned char FM5114_Touch_Trim_Load(void)
{
	unsigned char reg = 0;

	TrimLoad_PowerOn();

	Hardware_DelayMs(10);
	if (GetReg(I2CADDR_FM5114_TOUCH, 0x5E, &reg) != FM5114_SUCCESS)
		return FM5114_COMM_ERR;
	if (SetReg(I2CADDR_FM5114_TOUCH, 0x5E, reg | 0x80) != FM5114_SUCCESS) //????????Trim
		return FM5114_COMM_ERR;
	Hardware_DelayMs(1);
	if (GetReg(I2CADDR_FM5114_TOUCH, 0x5E, &reg) != FM5114_SUCCESS)
		return FM5114_COMM_ERR;
	//	Hardware_DelayMs(10);

	if ((reg & 0x01) != 0x00)
	{
		TrimLoad_PowerOff();

		//	if(GetReg(I2CADDR_FM5114_TOUCH, Scan_Ctrl , &reg) != FM5114_SUCCESS)
		//		return FM5114_COMM_ERR;

		return FM5114_SUCCESS;
	}
	else
	{
		TrimLoad_PowerOff();
		return FM5114_COMM_ERR;
	}
}

unsigned char FM5114_Touch_SoftReset(void)
{
	unsigned char reg_data, result;
	result = GetReg(I2CADDR_FM5114_TOUCH, Scan_Ctrl, &reg_data);

	result = SetReg(I2CADDR_FM5114_TOUCH, Scan_Ctrl, reg_data | 0x20);

	Hardware_DelayMs(1);
	return result;
}

char FM5114_Touch_Calibrate(void)
{
	unsigned char reg_data;
	if (GetReg(I2CADDR_FM5114_TOUCH, 0x7E, &reg_data) != FM5114_SUCCESS)
		return FM5114_COMM_ERR;
	if (SetReg(I2CADDR_FM5114_TOUCH, 0x7E, reg_data | 0x80) != FM5114_SUCCESS)
		return FM5114_COMM_ERR;
	return FM5114_SUCCESS;
}

unsigned char FM5114_Touch_Button_Auto_Tuning(unsigned char Sensitivity, unsigned int Channel) //
{
	unsigned char Cs;
	unsigned char N_Sens;
	unsigned char reg, reg1;
	unsigned char Reg_out;
	unsigned char i;
	unsigned char Num;

	if (FM5114_Touch_SoftReset() != FM5114_SUCCESS) //soft_reset=1
		return FM5114_COMM_ERR;

	i = Channel;

	if (SetReg(I2CADDR_FM5114_TOUCH, FM5114_TOUCH_REGADR[(i * 10) + 0], FM5114_TOUCH_REGVAL[(i * 14) + 0]) != FM5114_SUCCESS) //s0_en--s7_en °??ü?????÷0????
		return FM5114_COMM_ERR;
	if (SetReg(I2CADDR_FM5114_TOUCH, FM5114_TOUCH_REGADR[(i * 10) + 1], FM5114_TOUCH_REGVAL[(i * 14) + 1]) != FM5114_SUCCESS) //s8_en--s15_en
		return FM5114_COMM_ERR;

	if (SetReg(I2CADDR_FM5114_TOUCH, FM5114_TOUCH_REGADR[(i * 10) + 2], FM5114_TOUCH_REGVAL[(i * 14) + 2]) != FM5114_SUCCESS)
		return FM5114_COMM_ERR;

	if (SetReg(I2CADDR_FM5114_TOUCH, 0x0B, 0x80) != FM5114_SUCCESS)
		return FM5114_COMM_ERR;

	if (GetReg(I2CADDR_FM5114_TOUCH, FM5114_TOUCH_REGADR[(i * 10) + 3], &reg) != FM5114_SUCCESS)
		return FM5114_COMM_ERR;
	if (SetReg(I2CADDR_FM5114_TOUCH, FM5114_TOUCH_REGADR[(i * 10) + 3], ((reg & FM5114_TOUCH_REGVAL[(i * 14) + 3]) | FM5114_TOUCH_REGVAL[(i * 14) + 4])) != FM5114_SUCCESS)
		return FM5114_COMM_ERR;

	if (GetReg(I2CADDR_FM5114_TOUCH, FM5114_TOUCH_REGADR[(i * 10) + 4], &reg) != FM5114_SUCCESS)
		return FM5114_COMM_ERR;
	if (SetReg(I2CADDR_FM5114_TOUCH, FM5114_TOUCH_REGADR[(i * 10) + 4], ((reg & FM5114_TOUCH_REGVAL[(i * 14) + 5]) | FM5114_TOUCH_REGVAL[(i * 14) + 6])) != FM5114_SUCCESS)
		return FM5114_COMM_ERR;

	if (SetReg(I2CADDR_FM5114_TOUCH, 0x8A, 0x0D) != FM5114_SUCCESS)
		return FM5114_COMM_ERR;
	if (SetReg(I2CADDR_FM5114_TOUCH, 0x8B, 0x9A) != FM5114_SUCCESS)
		return FM5114_COMM_ERR;

	if (FM5114_Touch_Calibrate() != FM5114_SUCCESS)
		return FM5114_COMM_ERR;
	Hardware_DelayMs(50);

	if (GetReg(I2CADDR_FM5114_TOUCH, FM5114_TOUCH_REGADR[(i * 10) + 5], &reg) != FM5114_SUCCESS)
		return FM5114_COMM_ERR;
	if ((reg & 0x80) == 0x80)
	{
	}
	else
	{
		return FM5114_CALI_ERR;
	}

	if (GetReg(I2CADDR_FM5114_TOUCH, FM5114_TOUCH_REGADR[(i * 10) + 6], &Num) != FM5114_SUCCESS)
		return FM5114_COMM_ERR;
	if (FM5114_Touch_SoftReset() != FM5114_SUCCESS) //soft_reset=1
		return FM5114_COMM_ERR;

	if (Num >= 0x2D) //±?????×é????
	{
		Num = 0X2C;
	}

	Cs = FM5114_TOUCH_CS_LUT[Num - 1];
	CsButton[i] = Cs;
	// Uart_Send_Hex(&i, 1);Uart_Send_Msg(" = ");Uart_Send_Hex(&Cs, 1);Uart_Send_Msg("\r\n");

	if (Cs <= 6)
	{
		if (GetReg(I2CADDR_FM5114_TOUCH, FM5114_TOUCH_REGADR[(i * 10) + 7], &reg) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
		if (SetReg(I2CADDR_FM5114_TOUCH, FM5114_TOUCH_REGADR[(i * 10) + 7], ((reg & FM5114_TOUCH_REGVAL[(i * 14) + 7]) | FM5114_TOUCH_REGVAL[(i * 14) + 8])) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
	}
	else if (Cs <= 27)
	{
		if (GetReg(I2CADDR_FM5114_TOUCH, FM5114_TOUCH_REGADR[(i * 10) + 7], &reg) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
		if (SetReg(I2CADDR_FM5114_TOUCH, FM5114_TOUCH_REGADR[(i * 10) + 7], ((reg & FM5114_TOUCH_REGVAL[(i * 14) + 7]) | FM5114_TOUCH_REGVAL[(i * 14) + 9])) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
	}
	else if (Cs > 27 && Cs <= 50)
	{
		if (GetReg(I2CADDR_FM5114_TOUCH, FM5114_TOUCH_REGADR[(i * 10) + 7], &reg) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
		if (SetReg(I2CADDR_FM5114_TOUCH, FM5114_TOUCH_REGADR[(i * 10) + 7], ((reg & FM5114_TOUCH_REGVAL[(i * 14) + 7]) | FM5114_TOUCH_REGVAL[(i * 14) + 10])) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
	}
	else if (Cs > 50 && Cs <= 60)
	{
		if (GetReg(I2CADDR_FM5114_TOUCH, FM5114_TOUCH_REGADR[(i * 10) + 7], &reg) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
		if (SetReg(I2CADDR_FM5114_TOUCH, FM5114_TOUCH_REGADR[(i * 10) + 7], ((reg & FM5114_TOUCH_REGVAL[(i * 14) + 7]) | FM5114_TOUCH_REGVAL[(i * 14) + 11])) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
	}

	if (Sensitivity == 0x01)
	{
		N_Sens = FM5114_TOUCH_SEN_LUT1[Cs - 1];
	}
	else if (Sensitivity == 0x02)
	{
		N_Sens = FM5114_TOUCH_SEN_LUT2[Cs - 1];
	}
	else if (Sensitivity == 0x03)
	{
		N_Sens = FM5114_TOUCH_SEN_LUT3[Cs - 1];
	}
	else if (Sensitivity == 0x04)
	{
		N_Sens = FM5114_TOUCH_SEN_LUT4[Cs - 1];
	}
	else
	{
		N_Sens = FM5114_TOUCH_SEN_LUT1[Cs - 1];
	}

	if (GetReg(I2CADDR_FM5114_TOUCH, FM5114_TOUCH_REGADR[(i * 10) + 8], &Reg_out) != FM5114_SUCCESS)
		return FM5114_COMM_ERR;
	if (SetReg(I2CADDR_FM5114_TOUCH, FM5114_TOUCH_REGADR[(i * 10) + 8], (Reg_out & FM5114_TOUCH_REGVAL[(i * 14) + 12]) + ((N_Sens - 8) << FM5114_TOUCH_REGVAL[(i * 14) + 13])) != FM5114_SUCCESS)
		return FM5114_COMM_ERR;

	if (N_Sens == 8)
	{
		if (SetReg(I2CADDR_FM5114_TOUCH, 0x8A, 0x00) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
		if (SetReg(I2CADDR_FM5114_TOUCH, 0x8B, 0xDA) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
	}
	else if (N_Sens == 9)
	{
		if (SetReg(I2CADDR_FM5114_TOUCH, 0x8A, 0x01) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
		if (SetReg(I2CADDR_FM5114_TOUCH, 0x8B, 0xB3) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
	}
	else if (N_Sens == 10)
	{
		if (SetReg(I2CADDR_FM5114_TOUCH, 0x8A, 0x03) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
		if (SetReg(I2CADDR_FM5114_TOUCH, 0x8B, 0x66) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
	}
	else if (N_Sens == 11)
	{
		if (SetReg(I2CADDR_FM5114_TOUCH, 0x8A, 0x06) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
		if (SetReg(I2CADDR_FM5114_TOUCH, 0x8B, 0xCD) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
	}
	else if (N_Sens == 12)
	{
		if (SetReg(I2CADDR_FM5114_TOUCH, 0x8A, 0x0D) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
		if (SetReg(I2CADDR_FM5114_TOUCH, 0x8B, 0x9A) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
	}
	else if (N_Sens == 13)
	{
		if (SetReg(I2CADDR_FM5114_TOUCH, 0x8A, 0x1B) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
		if (SetReg(I2CADDR_FM5114_TOUCH, 0x8B, 0x33) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
	}
	else if (N_Sens == 14)
	{
		if (SetReg(I2CADDR_FM5114_TOUCH, 0x8A, 0x36) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
		if (SetReg(I2CADDR_FM5114_TOUCH, 0x8B, 0x66) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
	}
	else if (N_Sens == 15)
	{
		if (SetReg(I2CADDR_FM5114_TOUCH, 0x8A, 0x6C) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
		if (SetReg(I2CADDR_FM5114_TOUCH, 0x8B, 0xCD) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
	}
	else if (N_Sens == 16)
	{
		if (SetReg(I2CADDR_FM5114_TOUCH, 0x8A, 0xD9) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
		if (SetReg(I2CADDR_FM5114_TOUCH, 0x8B, 0x9A) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
	}
	if (FM5114_Touch_Calibrate() != FM5114_SUCCESS)
		return FM5114_COMM_ERR;
	Hardware_DelayMs(50);

	if (GetReg(I2CADDR_FM5114_TOUCH, FM5114_TOUCH_REGADR[(i * 10) + 5], &reg) != FM5114_SUCCESS)
		return FM5114_COMM_ERR;
	if ((reg & 0x80) == 0x80)
	{
		if (GetReg(I2CADDR_FM5114_TOUCH, FM5114_TOUCH_REGADR[(i * 10) + 6], &reg) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
		if (GetReg(I2CADDR_FM5114_TOUCH, FM5114_TOUCH_REGADR[(i * 10) + 9], &reg1) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
		if (SetReg(I2CADDR_FM5114_TOUCH, FM5114_TOUCH_REGADR[(i * 10) + 9], ((reg1 & 0x80) + (reg & 0x7F))) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
	}
	else
	{
		return FM5114_CALI_ERR;
	}
	return FM5114_SUCCESS;
}

unsigned char FM5114_Touch_Scan_Start(void)
{
	unsigned char reg_data;
	if (GetReg(I2CADDR_FM5114_TOUCH, Scan_Ctrl, &reg_data) != FM5114_SUCCESS)
		return FM5114_COMM_ERR;
	if (SetReg(I2CADDR_FM5114_TOUCH, Scan_Ctrl, reg_data | 0x80) != FM5114_SUCCESS)
		return FM5114_COMM_ERR;
	else
		return FM5114_SUCCESS;
}

unsigned char FM5114_Touch_SenseModeInit(unsigned int channel)
{
	unsigned char i = channel;
	unsigned char reg_data, reg_data1;
	if (GetReg(I2CADDR_FM5114_TOUCH, FM5114_TOUCH_SENSEMODE_REGADR[(i * 4) + 0], &reg_data) != FM5114_SUCCESS)
		return FM5114_COMM_ERR;
	if (SetReg(I2CADDR_FM5114_TOUCH, FM5114_TOUCH_SENSEMODE_REGADR[(i * 4) + 0], ((reg_data & 0x80) + ((reg_data << 1) >> 2))) != FM5114_SUCCESS)
		return FM5114_COMM_ERR;
	if (GetReg(I2CADDR_FM5114_TOUCH, FM5114_TOUCH_SENSEMODE_REGADR[(i * 4) + 1], &reg_data1) != FM5114_SUCCESS)
		return FM5114_COMM_ERR;
	if (SetReg(I2CADDR_FM5114_TOUCH, FM5114_TOUCH_SENSEMODE_REGADR[(i * 4) + 1], ((reg_data1 & 0x80) + (reg_data & 0x7F) - ((reg_data << 1) >> 2))) != FM5114_SUCCESS)
		return FM5114_COMM_ERR;
	if (GetReg(I2CADDR_FM5114_TOUCH, FM5114_TOUCH_SENSEMODE_REGADR[(i * 4) + 2], &reg_data) != FM5114_SUCCESS)
		return FM5114_COMM_ERR;
	if ((reg_data & FM5114_TOUCH_SENSEMODE_REGVAL[(i * 3) + 0]) != 0)
	{
		reg_data = reg_data - FM5114_TOUCH_SENSEMODE_REGVAL[(i * 3) + 1];
		if (SetReg(I2CADDR_FM5114_TOUCH, FM5114_TOUCH_SENSEMODE_REGADR[(i * 4) + 2], reg_data) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
	}
	if (GetReg(I2CADDR_FM5114_TOUCH, FM5114_TOUCH_SENSEMODE_REGADR[(i * 4) + 3], &reg_data) != FM5114_SUCCESS)
		return FM5114_COMM_ERR;
	if (SetReg(I2CADDR_FM5114_TOUCH, FM5114_TOUCH_SENSEMODE_REGADR[(i * 4) + 3], reg_data | FM5114_TOUCH_SENSEMODE_REGVAL[(i * 3) + 2]) != FM5114_SUCCESS)
		return FM5114_COMM_ERR;

	return FM5114_SUCCESS;
}

unsigned char FM5114_Touch_Init(unsigned char Sensi, unsigned int ButtonChannel, unsigned char Lpcd_Timer_Irq_en)
{
	unsigned char reg = 0;
	unsigned int channel = ButtonChannel;
	unsigned char i;

	if (FM5114_Touch_SoftReset() != FM5114_SUCCESS) //soft_reset=1
		return FM5114_COMM_ERR;

	for (i = 0; i < 14; i++)
	{
		if (((channel >> i) & 0x01) != 0)
		{
			if (FM5114_Touch_Button_Auto_Tuning(Sensi, i) != FM5114_SUCCESS) //°??ü?÷?????ò
				return FM5114_COMM_ERR;
		}
	}

	if (FM5114_Touch_SoftReset() != FM5114_SUCCESS) //soft_reset=1
		return FM5114_COMM_ERR;

/* 按键传感器增强模式配置 */
	if (SENSE_MODE == 1)
	{
		for (i = 0; i < 14; i++)
		{
			if (((channel >> i) & 0x01) != 0)
			{
				if (FM5114_Touch_SenseModeInit(i) != FM5114_SUCCESS) //°??ü?÷?????ò
					return FM5114_COMM_ERR;
			}
		}
	}

	if (FM5114_Touch_SoftReset() != FM5114_SUCCESS) //soft_reset=1
		return FM5114_COMM_ERR;



	if (SetReg(I2CADDR_FM5114_TOUCH, Prox_Channel_Config1,0x00) != FM5114_SUCCESS) //s0~s7关闭接近感应功能
		return FM5114_COMM_ERR;
	if (SetReg(I2CADDR_FM5114_TOUCH, Prox_Channel_Config2,0x00) != FM5114_SUCCESS) //s8~s13关闭接近感应功能
		return FM5114_COMM_ERR;

	if (SetReg(I2CADDR_FM5114_TOUCH, Slide_Channel_Config, 0x00) != FM5114_SUCCESS) //s9~s13关闭滑块功能
		return FM5114_COMM_ERR;

/*  中断输出管脚配置 */	
	if (SetReg(I2CADDR_FM5114_TOUCH, 0x43, 0x02) != FM5114_SUCCESS)  //0x02代表推挽输出，0x03代表OC输出
		return FM5114_COMM_ERR;

	if (SetReg(I2CADDR_FM5114_TOUCH, Irq_Ctrl, 0x01) != FM5114_SUCCESS)  //脉冲中断，按键的release中断使能
		return FM5114_COMM_ERR;

	if (SetReg(I2CADDR_FM5114_TOUCH, Button_Channel_Config1, (channel & 0x00FF)) != FM5114_SUCCESS) //s0-s7_en
		return FM5114_COMM_ERR;
	if (SetReg(I2CADDR_FM5114_TOUCH, Button_Channel_Config2, ((channel & 0xFF00) >> 8)) != FM5114_SUCCESS) //s8-s11_en
		return FM5114_COMM_ERR;

	if (SetReg(I2CADDR_FM5114_TOUCH, Button0_Touch_Thrsh, FM5114_SENS_CH0) != FM5114_SUCCESS) //s0阈值, from 31~200, 越小越灵
		return FM5114_COMM_ERR;
	if (SetReg(I2CADDR_FM5114_TOUCH, Button1_Touch_Thrsh, FM5114_SENS_CH1) != FM5114_SUCCESS) //s1阈值, from 31~200, 越小越灵
		return FM5114_COMM_ERR;
	if (SetReg(I2CADDR_FM5114_TOUCH, Button2_Touch_Thrsh, FM5114_SENS_CH2) != FM5114_SUCCESS) //s2阈值, from 31~200, 越小越灵
		return FM5114_COMM_ERR;
	if (SetReg(I2CADDR_FM5114_TOUCH, Button3_Touch_Thrsh, FM5114_SENS_CH3) != FM5114_SUCCESS) //s3阈值, from 31~200, 越小越灵
		return FM5114_COMM_ERR;
	if (SetReg(I2CADDR_FM5114_TOUCH, Button4_Touch_Thrsh, FM5114_SENS_CH4) != FM5114_SUCCESS) //s4阈值, from 31~200, 越小越灵
		return FM5114_COMM_ERR;
	if (SetReg(I2CADDR_FM5114_TOUCH, Button5_Touch_Thrsh, FM5114_SENS_CH5) != FM5114_SUCCESS) //s5阈值, from 31~200, 越小越灵
		return FM5114_COMM_ERR;
	if (SetReg(I2CADDR_FM5114_TOUCH, Button6_Touch_Thrsh, FM5114_SENS_CH6) != FM5114_SUCCESS) //s6阈值, from 31~200, 越小越灵
		return FM5114_COMM_ERR;
	if (SetReg(I2CADDR_FM5114_TOUCH, Button7_Touch_Thrsh, FM5114_SENS_CH7) != FM5114_SUCCESS) //s7阈值, from 31~200, 越小越灵
		return FM5114_COMM_ERR;
	if (SetReg(I2CADDR_FM5114_TOUCH, Button8_Touch_Thrsh, FM5114_SENS_CH8) != FM5114_SUCCESS) //s8阈值, from 31~200, 越小越灵
		return FM5114_COMM_ERR;
	if (SetReg(I2CADDR_FM5114_TOUCH, Button9_Touch_Thrsh, FM5114_SENS_CH9) != FM5114_SUCCESS) //s9阈值, from 31~200, 越小越灵
		return FM5114_COMM_ERR;
	if (SetReg(I2CADDR_FM5114_TOUCH, Button10_Touch_Thrsh, FM5114_SENS_CH10) != FM5114_SUCCESS) //s10阈值, from 31~200, 越小越灵
		return FM5114_COMM_ERR;
	if (SetReg(I2CADDR_FM5114_TOUCH, Button11_Touch_Thrsh, FM5114_SENS_CH11) != FM5114_SUCCESS) //s11阈值, from 31~200, 越小越灵
		return FM5114_COMM_ERR;
	if (SetReg(I2CADDR_FM5114_TOUCH, Button12_Touch_Thrsh, FM5114_SENS_CH12) != FM5114_SUCCESS) //s12阈值, from 31~200, 越小越灵
		return FM5114_COMM_ERR;
	if (SetReg(I2CADDR_FM5114_TOUCH, Button13_Touch_Thrsh, FM5114_SENS_CH13) != FM5114_SUCCESS) //s13阈值, from 31~200, 越小越灵
		return FM5114_COMM_ERR;

	if (SetReg(I2CADDR_FM5114_TOUCH, Global_Hys_Thrsh, 0x44) != FM5114_SUCCESS) //按键传感器触摸阈值迟滞为8
		return FM5114_COMM_ERR;
	if (SetReg(I2CADDR_FM5114_TOUCH, Global_Noise_Thrsh, 0x1E) != FM5114_SUCCESS) //按键传感器噪声阈值为30
		return FM5114_COMM_ERR;
	if (SetReg(I2CADDR_FM5114_TOUCH, Global_NegNoise_Thrsh, 0x40) != FM5114_SUCCESS) //按键传感器负噪声阈值为40 0~127
		return FM5114_COMM_ERR;
	if (SetReg(I2CADDR_FM5114_TOUCH, Button_Debounce, 0x32) != FM5114_SUCCESS) //按键传感器防弹跳为2
		return FM5114_COMM_ERR;
	if (SetReg(I2CADDR_FM5114_TOUCH, Button_NegNoise_Num_Thrsh, 0x0A) != FM5114_SUCCESS) //按键传感器超时复位时间，10个扫描周期
		return FM5114_COMM_ERR;
	if (SetReg(I2CADDR_FM5114_TOUCH, 0x0B, 0x50) != FM5114_SUCCESS) //按键传感器其他参数配置,不开放
		return FM5114_COMM_ERR;
	if (SetReg(I2CADDR_FM5114_TOUCH, 0x0C, 0x20) != FM5114_SUCCESS) //按键传感器其他参数配置,不开放
		return FM5114_COMM_ERR;

	/*
		if (GetReg(I2CADDR_FM5114_TOUCH, Scan_Period_Config, &reg) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
		if (SetReg(I2CADDR_FM5114_TOUCH, Scan_Period_Config, (reg & 0xE0) | 0x10) != FM5114_SUCCESS) //低速按键扫描模式340ms扫描周期
			return FM5114_COMM_ERR;
	*/


	if (SetReg(I2CADDR_FM5114_TOUCH, Scan_Period_Config, 0x00) != FM5114_SUCCESS) //接近扫描模式及自动暂停扫描模式关闭，低速按键扫描模式20ms扫描周期
		return FM5114_COMM_ERR;

	//	if(GetReg(I2CADDR_FM5114_TOUCH, Scan_Ctrl , &reg) != FM5114_SUCCESS)
	//		return FM5114_COMM_ERR;
	//	if(SetReg(I2CADDR_FM5114_TOUCH, Scan_Ctrl, reg&0xEF) != FM5114_SUCCESS)        //按键互斥关闭
	//		return FM5114_COMM_ERR;

	if (GetReg(I2CADDR_FM5114_TOUCH, Scan_Ctrl, &reg) != FM5114_SUCCESS)
		return FM5114_COMM_ERR;
	if (SetReg(I2CADDR_FM5114_TOUCH, Scan_Ctrl, reg | 0x10) != FM5114_SUCCESS) //按键互斥开启
		return FM5114_COMM_ERR;

	if (SetReg(I2CADDR_FM5114_TOUCH, Timeout_Cfg, 0x59 != FM5114_SUCCESS)) //触摸超时配置
		return FM5114_COMM_ERR;

	if (Lpcd_Timer_Irq_en == 1)
	{
		if (GetReg(I2CADDR_FM5114_TOUCH, Irq_Repeat_Rate, &reg) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
		if (SetReg(I2CADDR_FM5114_TOUCH, Irq_Repeat_Rate, reg | 0x80) != FM5114_SUCCESS) //使能LPCD定时脉冲中断
			return FM5114_COMM_ERR;

		if (GetReg(I2CADDR_FM5114_TOUCH, LPCD_Timer_Config, &reg) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
		if (SetReg(I2CADDR_FM5114_TOUCH, LPCD_Timer_Config, reg | 0x38) != FM5114_SUCCESS) //使能LPCD定时脉冲
			return FM5114_COMM_ERR;
	}

	if (FM5114_Touch_Scan_Start() != FM5114_SUCCESS) //启动扫描
		return FM5114_COMM_ERR;
	// Uart_Send_Msg("-> Tuning OK!\r\n");
	return FM5114_SUCCESS;
}

unsigned char FM5114_Sensor_Debug(unsigned int channel)
{
	unsigned char reg1, reg2, reg3, reg4;
	unsigned char signal, base;
	unsigned long reg5, reg6, touchrise;

	switch (channel)
	{
	case 0: //'LOCK'
		if (SetReg(I2CADDR_FM5114_TOUCH, 0x7E, 0x00) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
		break;

	case 1: //'1'
		if (SetReg(I2CADDR_FM5114_TOUCH, 0x7E, 0x01) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
		break;

	case 2: //'4'
		if (SetReg(I2CADDR_FM5114_TOUCH, 0x7E, 0x02) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
		break;

	case 3: //'7'
		if (SetReg(I2CADDR_FM5114_TOUCH, 0x7E, 0x03) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
		break;

	case 4: //'*'
		if (SetReg(I2CADDR_FM5114_TOUCH, 0x7E, 0x04) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
		break;

	case 5: //'0'
		if (SetReg(I2CADDR_FM5114_TOUCH, 0x7E, 0x05) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
		break;

	case 6: //'8'
		if (SetReg(I2CADDR_FM5114_TOUCH, 0x7E, 0x06) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
		break;

	case 7: //'5'
		if (SetReg(I2CADDR_FM5114_TOUCH, 0x7E, 0x07) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
		break;

	case 8: //'2'
		if (SetReg(I2CADDR_FM5114_TOUCH, 0x7E, 0x08) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
		break;

	case 9: //'BELL'
		if (SetReg(I2CADDR_FM5114_TOUCH, 0x7E, 0x09) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
		break;

	case 10: //'3'
		if (SetReg(I2CADDR_FM5114_TOUCH, 0x7E, 0x0A) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
		break;

	case 11: //'6'
		if (SetReg(I2CADDR_FM5114_TOUCH, 0x7E, 0x0B) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
		break;

	case 12: //'9'
		if (SetReg(I2CADDR_FM5114_TOUCH, 0x7E, 0x0C) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
		break;

	case 13: //#
		if (SetReg(I2CADDR_FM5114_TOUCH, 0x7E, 0x0D) != FM5114_SUCCESS)
			return FM5114_COMM_ERR;
		break;
	}
	if (GetReg(I2CADDR_FM5114_TOUCH, 0x81, &reg1) != FM5114_SUCCESS)
		return FM5114_COMM_ERR;
	if (GetReg(I2CADDR_FM5114_TOUCH, 0x82, &reg2) != FM5114_SUCCESS)
		return FM5114_COMM_ERR;
	if (GetReg(I2CADDR_FM5114_TOUCH, 0x7F, &reg3) != FM5114_SUCCESS)
		return FM5114_COMM_ERR;
	if (GetReg(I2CADDR_FM5114_TOUCH, 0x80, &reg4) != FM5114_SUCCESS)
		return FM5114_COMM_ERR;

	// Uart_Send_Msg ("-> Signal = 0x");
	// signal = reg1;Uart_Send_Hex(&signal,1);
	// signal = reg2;Uart_Send_Hex(&signal,1);

	// Uart_Send_Msg(", Base = 0x");
	// base = reg3;Uart_Send_Hex(&base,1);
	// base = reg4;Uart_Send_Hex(&base,1);

	// reg5 = (reg1 << 8) + reg2;
	// reg6 = (reg3 << 8) + reg4;
	// Uart_Send_Msg(", TouchSignalRise = 0x");
	// if(reg5 > reg6)
	// {
	// 	touchrise = reg5 - reg6;
	// 	signal = (touchrise & 0xFF00) >> 8;
	// 	Uart_Send_Hex(&signal,1);
	// 	signal = touchrise & 0x00FF;
	// 	Uart_Send_Hex(&signal,1);
	// }
	// else
	// {
	// 	touchrise = reg6 - reg5;
	// 	signal = ((touchrise & 0xFF00) >> 8);
	// 	Uart_Send_Hex(&signal,1);
	// 	signal = touchrise & 0x00FF;
	// 	Uart_Send_Hex(&signal,1);

	// }
	// Uart_Send_Msg("\r\n");

	Hardware_DelayMs(1000);

	return FM5114_SUCCESS;
}
void FM5114_TEST(void)
{
	FM5114_Touch_Reset();
	//FM5114_TESTBUF[0] = 0x00;
	DEBUG_MARK;
	//FM5114_Reader_ReadOneByte(READER_I2C_Address,0x0C,&FM5114_TESTBUF[0]);
	//FM5114_Reader_ReadOneByte(I2CADDR_FM5114_TOUCH,0x42,&FM5114_TESTBUF[1]);
	DEBUG_MARK;
}


static keycode_t NewKeycode, LastKeycode;
static uint16_t KeyHoldTimeCnt = 0;

static uint8_t KEYSCANTIMECNT = 0;
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
	bool_t KEY_SPARE_Pressed;
} TouchKeyStatus;

AwakeSystemKeyMgr_t AwakeSystemKeyMgr;


#define Def_KeyHoldTimeLimited 128	   //2s
#define Def_KeyHoldLongTimeLimited 320 //6s


void TOUCH_Init(void)
{
	unsigned char result;
		
	FM5114_Touch_Reset();

	result = FM5114_Touch_SoftReset();

	result = FM5114_Touch_Trim_Load();

	result = FM5114_Touch_Init(DEF_FM5114_TouchSens, DEF_FM5114_ChannelConfig, 0); //0x3FFF±í??14???¨??????,bit0~bit13
	
	TouchPowerMgr.Status = HighSensitivity;
	
	if (result != FM5114_SUCCESS)
	{
		while (1);
	}

}

void TOUCH_SUSPEND(void)
{
	unsigned char reg;
	GetReg(I2CADDR_FM5114_TOUCH, Slide_Channel_Config, &reg);
	SetReg(I2CADDR_FM5114_TOUCH, Slide_Channel_Config, reg | 0x40); //Touch_suspend=1
}
void TOUCH_RESUME(void)
{
	unsigned char i,reg;
	i=4;
	do
	{
		GetReg(I2CADDR_FM5114_TOUCH, Slide_Channel_Config, &reg);
		SetReg(I2CADDR_FM5114_TOUCH, Slide_Channel_Config, reg & 0xBF); //Touch_suspend=0
		GetReg(I2CADDR_FM5114_TOUCH, Slide_Channel_Config, &reg);
		i--;
	}while(((reg&0x40)!=0x00)&&(i>0));
}

uint8_t SET_TOUCH_SLEEP_SENSITIVITY(void)
{
	unsigned char reg;

	TOUCH_SUSPEND();
	
	if (SetReg(I2CADDR_FM5114_TOUCH, Button_Debounce, 0x33) != FM5114_SUCCESS) //按键传感器防弹跳为3
		return FM5114_COMM_ERR;
	if (SetReg(I2CADDR_FM5114_TOUCH, Scan_Period_Config, 0x07) != FM5114_SUCCESS) //接近扫描模式及自动暂停扫描模式关闭，低速按键扫描模式160ms扫描周期
		return FM5114_COMM_ERR;

	TOUCH_RESUME();

	TouchPowerMgr.Status = LowSensitivity;
	return FM5114_SUCCESS;
	
}

uint8_t SET_TOUCH_AWAKE_SENSITIVITY(void)
{
	unsigned char reg;

	if (GetReg(I2CADDR_FM5114_TOUCH, Scan_Ctrl, &reg) != FM5114_SUCCESS)
	{
		//return FM5114_COMM_ERR;
		TOUCH_Init();
		return FM5114_SUCCESS;
	}

	if (SetReg(I2CADDR_FM5114_TOUCH, Scan_Ctrl, reg & 0xBF) != FM5114_SUCCESS) //normal Mode
	{
		//return FM5114_COMM_ERR;
		TOUCH_Init();
		return FM5114_SUCCESS;
	}
	
	TOUCH_SUSPEND();

	if (SetReg(I2CADDR_FM5114_TOUCH, Button_Debounce, 0x32) != FM5114_SUCCESS) //按键传感器防弹跳为1
	{
		TOUCH_Init();
	}
	if (SetReg(I2CADDR_FM5114_TOUCH, Scan_Period_Config, 0x00) != FM5114_SUCCESS) //接近扫描模式及自动暂停扫描模式关闭，低速按键扫描模式20ms扫描周期
	{
		TOUCH_Init();
	}

	TOUCH_RESUME();


	TouchPowerMgr.Status = HighSensitivity;
	return FM5114_SUCCESS;
	
}

uint8_t TOUCH_ERRORCHECK(void)
{
	uint8_t reg_data;
	
	if (GetReg(I2CADDR_FM5114_TOUCH, Button_Channel_Config1,&reg_data) != FM5114_SUCCESS )
		return FM5114_COMM_ERR;
	if (reg_data != (DEF_FM5114_ChannelConfig & 0x00FF)) //s0-s7_en
		return FM5114_COMM_ERR;
	
	if (GetReg(I2CADDR_FM5114_TOUCH, Button_Channel_Config2,&reg_data) != FM5114_SUCCESS )
		return FM5114_COMM_ERR;
	if (reg_data != ((DEF_FM5114_ChannelConfig & 0xFF00) >> 8))  //s8-s11_en
		return FM5114_COMM_ERR;

	if (GetReg(I2CADDR_FM5114_TOUCH, Button0_Touch_Thrsh,&reg_data) != FM5114_SUCCESS )
		return FM5114_COMM_ERR;
	if (reg_data != FM5114_SENS_CH0) 
		return FM5114_COMM_ERR;
	
	if (GetReg(I2CADDR_FM5114_TOUCH, Button1_Touch_Thrsh,&reg_data) != FM5114_SUCCESS )
		return FM5114_COMM_ERR;
	if (reg_data != FM5114_SENS_CH1)
		return FM5114_COMM_ERR;
	
	if (GetReg(I2CADDR_FM5114_TOUCH, Button2_Touch_Thrsh,&reg_data) != FM5114_SUCCESS )
		return FM5114_COMM_ERR;
	if (reg_data != FM5114_SENS_CH2)
		return FM5114_COMM_ERR;
	
	if (GetReg(I2CADDR_FM5114_TOUCH, Button3_Touch_Thrsh,&reg_data) != FM5114_SUCCESS )
		return FM5114_COMM_ERR;
	if (reg_data != FM5114_SENS_CH3)
		return FM5114_COMM_ERR;
	
	if (GetReg(I2CADDR_FM5114_TOUCH, Button4_Touch_Thrsh,&reg_data) != FM5114_SUCCESS )
		return FM5114_COMM_ERR;
	if (reg_data != FM5114_SENS_CH4)
		return FM5114_COMM_ERR;
	
	if (GetReg(I2CADDR_FM5114_TOUCH, Button5_Touch_Thrsh,&reg_data) != FM5114_SUCCESS )
		return FM5114_COMM_ERR;
	if (reg_data != FM5114_SENS_CH5)
		return FM5114_COMM_ERR;
	
	if (GetReg(I2CADDR_FM5114_TOUCH, Button6_Touch_Thrsh,&reg_data) != FM5114_SUCCESS )
		return FM5114_COMM_ERR;
	if (reg_data != FM5114_SENS_CH6)
		return FM5114_COMM_ERR;
	
	if (GetReg(I2CADDR_FM5114_TOUCH, Button7_Touch_Thrsh,&reg_data) != FM5114_SUCCESS )
		return FM5114_COMM_ERR;
	if (reg_data != FM5114_SENS_CH7)
		return FM5114_COMM_ERR;
	
	if (GetReg(I2CADDR_FM5114_TOUCH, Button8_Touch_Thrsh,&reg_data) != FM5114_SUCCESS )
		return FM5114_COMM_ERR;
	if (reg_data != FM5114_SENS_CH8)
		return FM5114_COMM_ERR;
	
	if (GetReg(I2CADDR_FM5114_TOUCH, Button9_Touch_Thrsh,&reg_data) != FM5114_SUCCESS )
		return FM5114_COMM_ERR;
	if (reg_data != FM5114_SENS_CH9)
		return FM5114_COMM_ERR;
	
	if (GetReg(I2CADDR_FM5114_TOUCH, Button10_Touch_Thrsh,&reg_data) != FM5114_SUCCESS )
		return FM5114_COMM_ERR;
	if (reg_data != FM5114_SENS_CH10)
		return FM5114_COMM_ERR;
	
	if (GetReg(I2CADDR_FM5114_TOUCH, Button11_Touch_Thrsh,&reg_data) != FM5114_SUCCESS )
		return FM5114_COMM_ERR;
	if (reg_data != FM5114_SENS_CH11)
		return FM5114_COMM_ERR;
	
	if (GetReg(I2CADDR_FM5114_TOUCH, Button12_Touch_Thrsh,&reg_data) != FM5114_SUCCESS )
		return FM5114_COMM_ERR;
	if (reg_data != FM5114_SENS_CH12)
		return FM5114_COMM_ERR;
	
	if (GetReg(I2CADDR_FM5114_TOUCH, Button13_Touch_Thrsh,&reg_data) != FM5114_SUCCESS )
		return FM5114_COMM_ERR;
	if (reg_data != FM5114_SENS_CH13)
		return FM5114_COMM_ERR;

	return FM5114_SUCCESS;

}


void TOUCH_AWAKE(void)
{
	keycode_t KEY;

	Hardware_DelayMs(3);

	if (SystemPowerMgr.AwakeSource == KeyBoardTouch)
	{
		g_TouchAwakeNeedReleaseKey = bTRUE;
		
		Key_Scan_Init();

		KEY = GetNewTouchKey();

		AwakeSystemKeyMgr.IsDoorColseKeyAwake = bFALSE;
		AwakeSystemKeyMgr.IsDoorBellKeyAwake = bFALSE;
		AwakeSystemKeyMgr.IsPoundsignKeyAwake = bFALSE;

		#if defined (ProjectIs_AutoBarLock_S20Z04) || defined (ProjectIs_AutoBarLock_S58Z07)\
		  ||defined (ProjectIs_AutoBarLock_S68Z04) || defined (ProjectIs_AutoBarLock_S68Z10)
			
		if (KEY == KEY_ASTERISK)
		{
			AwakeSystemKeyMgr.IsDoorColseKeyAwake = bTRUE;
			DEBUG_MARK;
		}
		else if (KEY == KEY_POUNDSIGN)
		{
			AwakeSystemKeyMgr.IsDoorBellKeyAwake = bTRUE;
			DEBUG_MARK;
		}
		else
		{
			DEBUG_MARK;
		}
			
		#else
		if (KEY == KEY_DOORBELL)
		{
			AwakeSystemKeyMgr.IsDoorBellKeyAwake = bTRUE;
		}
		else if (KEY == KEY_DOORCLOSE)
		{
			AwakeSystemKeyMgr.IsDoorColseKeyAwake = bTRUE;
		}
		else if (KEY == KEY_POUNDSIGN)
		{
			AwakeSystemKeyMgr.IsPoundsignKeyAwake = bTRUE;
		}
		#endif
		DEBUG_MARK;
	}
	else
	{
		//TouchPowerMgr.NeedHardwareResetTouch = bTRUE;
		DEBUG_MARK;
	}

	if ( TOUCH_ERRORCHECK() == FM5114_SUCCESS )
	{
		SET_TOUCH_AWAKE_SENSITIVITY();
		DEBUG_MARK;
	}
	else
	{
		TOUCH_Init();
		DEBUG_MARK;
	}
}


void TOUCH_POWERDOWN(void)
{
	if ( TouchPowerMgr.NeedHardwareResetTouch == bTRUE )
	{
		TouchPowerMgr.NeedHardwareResetTouch = bFALSE;
		TOUCH_Init();
		SET_TOUCH_SLEEP_SENSITIVITY();
	}
	else
	{
		if ( SET_TOUCH_SLEEP_SENSITIVITY()!=FM5114_SUCCESS)
		{
			TOUCH_Init();
			SET_TOUCH_SLEEP_SENSITIVITY();
		}
	}
	Hardware_DelayMs(1);
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
	TouchKeyStatus.KEY_DOORCLOSE_Pressed = bFALSE;
	TouchKeyStatus.KEY_DOORBELL_Pressed = bFALSE;
	
	LastKeycode = KEY_NONE;
}

keycode_t GetNewTouchKey(void)
{
	keycode_t Temp_NewKeycode;
	unsigned char reg_slide = 1, reg1, reg2, reg3, reg4, reg_data, polling_card;

	uint8_t ButtonStatus1,ButtonStatus2;



	GetReg(I2CADDR_FM5114_TOUCH, Button_Status1, &ButtonStatus1);
	GetReg(I2CADDR_FM5114_TOUCH, Button_Status2, &ButtonStatus2);

	GetReg(I2CADDR_FM5114_TOUCH, Special_Irq, &reg_slide);
	//GetReg(I2CADDR_FM5114_TOUCH, Button_Touch_Irq1, &reg1);
	//GetReg(I2CADDR_FM5114_TOUCH, Button_Touch_Irq2, &reg2);
	//GetReg(I2CADDR_FM5114_TOUCH, Button_Release_Irq1, &reg3);
	//GetReg(I2CADDR_FM5114_TOUCH, Button_Release_Irq2, &reg4);
	
	if ((reg2 & 0x80) != 0) //LPCD
	{
		SetReg(I2CADDR_FM5114_TOUCH, Button_Touch_Irq2, 0x7F); //

	}
	
	if ( reg_slide!=0x00 )
	{
		SetReg(I2CADDR_FM5114_TOUCH, Special_Irq, 0x00); //
	}
#if 0
	if ((reg_slide & 0x80) != 0) // Touch hardware error
	{
		SetReg(I2CADDR_FM5114_TOUCH, Special_Irq, 0x7F); //
	}

	if ((reg_slide & 0x20) != 0)	//
	{
		SetReg(I2CADDR_FM5114_TOUCH, Special_Irq, 0xDF); //
	}
	if ((reg_slide & 0x10) != 0)	//
	{
		SetReg(I2CADDR_FM5114_TOUCH, Special_Irq, 0xEF); //
	}
	if ((reg_slide & 0x04) != 0)	//
	{
		SetReg(I2CADDR_FM5114_TOUCH, Special_Irq, 0xFB); //
	}
	if ((reg_slide & 0x02) != 0)	//
	{
		SetReg(I2CADDR_FM5114_TOUCH, Special_Irq, 0xFD); //
	}
#endif


	if ((ButtonStatus1!=0x00) || (ButtonStatus2 != 0x00))
	{
			/******/
	}
	else
	{
		if (SystemPowerMgr.AwakeTimerCnt < 16)
		{
			SystemPowerMgr.AwakeTimerCnt++;
		}
		else
		{
			g_TouchAwakeNeedReleaseKey = bFALSE;
		}
	}


	if ( (ButtonStatus1 & 0x01)!=0x00)
	{
		FM5114_CS0_Pressed = bTRUE;
	}
	else
	{
		FM5114_CS0_Pressed = bFALSE;
	}
	
	if ( (ButtonStatus1 & 0x02)!=0x00)
	{
		FM5114_CS1_Pressed = bTRUE;
	}
	else
	{
		FM5114_CS1_Pressed = bFALSE;
	}

	if ( (ButtonStatus1 & 0x04)!=0x00)
	{
		FM5114_CS2_Pressed = bTRUE;
	}
	else
	{
		FM5114_CS2_Pressed = bFALSE;
	}

	if ( (ButtonStatus1 & 0x08)!=0x00)
	{
		FM5114_CS3_Pressed = bTRUE;
	}
	else
	{
		FM5114_CS3_Pressed = bFALSE;
	}

	if ( (ButtonStatus1 & 0x10)!=0x00)
	{
		FM5114_CS4_Pressed = bTRUE;
	}
	else
	{
		FM5114_CS4_Pressed = bFALSE;
	}
	
	if ( (ButtonStatus1 & 0x20)!=0x00)
	{
		FM5114_CS5_Pressed = bTRUE;
	}
	else
	{
		FM5114_CS5_Pressed = bFALSE;
	}

	if ( (ButtonStatus1 & 0x40)!=0x00)
	{
		FM5114_CS6_Pressed = bTRUE;
	}
	else
	{
		FM5114_CS6_Pressed = bFALSE;
	}

	if ( (ButtonStatus1 & 0x80)!=0x00)
	{
		FM5114_CS7_Pressed = bTRUE;
	}
	else
	{
		FM5114_CS7_Pressed = bFALSE;
	}

	if ( (ButtonStatus2 & 0x01)!=0x00)
	{
		FM5114_CS8_Pressed = bTRUE;
	}
	else
	{
		FM5114_CS8_Pressed = bFALSE;
	}
	
	if ( (ButtonStatus2 & 0x02)!=0x00)
	{
		FM5114_CS9_Pressed = bTRUE;
	}
	else
	{
		FM5114_CS9_Pressed = bFALSE;
	}

	if ( (ButtonStatus2 & 0x04)!=0x00)
	{
		FM5114_CS10_Pressed = bTRUE;
	}
	else
	{
		FM5114_CS10_Pressed = bFALSE;
	}

	if ( (ButtonStatus2 & 0x08)!=0x00)
	{
		FM5114_CS11_Pressed = bTRUE;
	}
	else
	{
		FM5114_CS11_Pressed = bFALSE;
	}

	if ( (ButtonStatus2 & 0x10)!=0x00)
	{
		FM5114_CS12_Pressed = bTRUE;
	}
	else
	{
		FM5114_CS12_Pressed = bFALSE;
	}
	
	if ( (ButtonStatus2 & 0x20)!=0x00)
	{
		FM5114_CS13_Pressed = bTRUE;
	}
	else
	{
		FM5114_CS13_Pressed = bFALSE;
	}


	if (TouchKeyStatus.KEY_ZERO_Pressed == bTRUE)
	{
		Temp_NewKeycode = KEY_ZERO;
	}
	else if (TouchKeyStatus.KEY_ONE_Pressed == bTRUE)
	{
		Temp_NewKeycode = KEY_ONE;
	}
	else if (TouchKeyStatus.KEY_TWO_Pressed == bTRUE)
	{
		Temp_NewKeycode = KEY_TWO;
	}
	else if (TouchKeyStatus.KEY_THREE_Pressed == bTRUE)
	{
		Temp_NewKeycode = KEY_THREE;
	}
	else if (TouchKeyStatus.KEY_FOUR_Pressed == bTRUE)
	{
		Temp_NewKeycode = KEY_FOUR;
	}
	else if (TouchKeyStatus.KEY_FIVE_Pressed == bTRUE)
	{
		Temp_NewKeycode = KEY_FIVE;
	}
	else if (TouchKeyStatus.KEY_SIX_Pressed == bTRUE)
	{
		Temp_NewKeycode = KEY_SIX;
	}
	else if (TouchKeyStatus.KEY_SEVEN_Pressed == bTRUE)
	{
		Temp_NewKeycode = KEY_SEVEN;
	}
	else if (TouchKeyStatus.KEY_EIGHT_Pressed == bTRUE)
	{
		Temp_NewKeycode = KEY_EIGHT;
	}
	else if (TouchKeyStatus.KEY_NINE_Pressed == bTRUE)
	{
		Temp_NewKeycode = KEY_NINE;
	}
	else if (TouchKeyStatus.KEY_POUNDSIGN_Pressed == bTRUE)
	{
		Temp_NewKeycode = KEY_POUNDSIGN;
	}
	else if (TouchKeyStatus.KEY_ASTERISK_Pressed == bTRUE)
	{
		Temp_NewKeycode = KEY_ASTERISK;
	}
	else if (TouchKeyStatus.KEY_DOORCLOSE_Pressed == bTRUE)
	{
		Temp_NewKeycode = KEY_DOORCLOSE;
	}
	else if (TouchKeyStatus.KEY_DOORBELL_Pressed == bTRUE)
	{
		Temp_NewKeycode = KEY_DOORBELL;
	}
	else
	{
		Temp_NewKeycode = KEY_NONE;
	}

	return Temp_NewKeycode;

}

keycode_t Key_Scan(void)
{

	keycode_t PostKeycode;

	NewKeycode = GetNewTouchKey();

	LEDsMgr.Keycode = NewKeycode;

	if (NewKeycode != KEY_NONE)
	{
		if (NewKeycode != LastKeycode)
		{
			PostKeycode = NewKeycode;
			LastKeycode = NewKeycode;
			KeyHoldTimeCnt = 0;
		}
		else
		{
			if (KeyHoldTimeCnt < Def_KeyHoldLongTimeLimited)
			{
				KeyHoldTimeCnt++;
				PostKeycode = KEY_NONE;
			}
			else
			{
				//Key_Scan_Init();
				PostKeycode = KEY_NONE;
			}
		}
	}
	else
	{
		PostKeycode = KEY_NONE;
		LastKeycode = NewKeycode;
		KeyHoldTimeCnt = 0;
	}


	if (AwakeSystemKeyMgr.IsDoorBellKeyAwake == bTRUE)
	{
		PostKeycode = KEY_DOORBELL;
		AwakeSystemKeyMgr.IsDoorColseKeyAwake = bFALSE;
		AwakeSystemKeyMgr.IsDoorBellKeyAwake = bFALSE;
		AwakeSystemKeyMgr.IsPoundsignKeyAwake = bFALSE;
	}
	else if (AwakeSystemKeyMgr.IsDoorColseKeyAwake == bTRUE)
	{
		PostKeycode = KEY_DOORCLOSE;
		AwakeSystemKeyMgr.IsDoorColseKeyAwake = bFALSE;
		AwakeSystemKeyMgr.IsDoorBellKeyAwake = bFALSE;
		AwakeSystemKeyMgr.IsPoundsignKeyAwake = bFALSE;
	}
	else if (AwakeSystemKeyMgr.IsPoundsignKeyAwake == bTRUE)
	{
		PostKeycode = KEY_POUNDSIGN;
		AwakeSystemKeyMgr.IsDoorColseKeyAwake = bFALSE;
		AwakeSystemKeyMgr.IsDoorBellKeyAwake = bFALSE;
		AwakeSystemKeyMgr.IsPoundsignKeyAwake = bFALSE;
	}
	else if ( g_TouchAwakeNeedReleaseKey == bTRUE)
	{
		PostKeycode = KEY_NONE;
	}

	return PostKeycode;
}
#endif
