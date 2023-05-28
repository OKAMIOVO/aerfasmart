#ifndef _FM5114_Touch_H
#define _FM5114_Touch_H

#include "Project.h"

#ifdef Function_TouchUsed_FM5114

#include "StdTypes.h"


#define FM5114_SUCCESS			0x00		//
#define FM5114_RESET_ERR		0xF1		//
#define FM5114_PARAM_ERR 		0xF2		//
#define FM5114_TIMER_ERR		0xF3		//
#define FM5114_COMM_ERR			0xF4		//
#define FM5114_COLL_ERR			0xF5		//
#define FM5114_FIFO_ERR			0xF6		//
#define FM5114_CRC_ERR			0xF7
#define FM5114_PARITY_ERR		0xF8
#define FM5114_PROTOCOL_ERR	0xF9
#define FM5114_AUTH_ERR	0xE1
#define FM5114_RATS_ERR 	0xD1
#define FM5114_PPS_ERR 	0xD2
#define FM5114_PCB_ERR 	0xD3
#define FM5114_CALI_ERR 0xE1



#define Button_Status1                  	0x00 
#define Button_Status2                 		0x01 
#define Special_Status            			0x02
#define Button_Touch_Irq1                 	0x03
#define Button_Touch_Irq2                 	0x04
#define Button_Release_Irq1               	0x05
#define Button_Release_Irq2               	0x06
#define Special_Irq                       	0x07
#define Button_Channel_Config1            	0x08
#define Button_Channel_Config2           	0x09
#define Scan_Ctrl                         	0x0A
#define Button0_Touch_Thrsh               	0x0F


#define	Button1_Touch_Thrsh             	0x10
#define	Button2_Touch_Thrsh              	0x11
#define	Button3_Touch_Thrsh             	0x12
#define	Button4_Touch_Thrsh             	0x13
#define	Button5_Touch_Thrsh             	0x14
#define	Button6_Touch_Thrsh              	0x15
#define	Button7_Touch_Thrsh             	0x16
#define	Button8_Touch_Thrsh             	0x17
#define	Button9_Touch_Thrsh              	0x18
#define	Button10_Touch_Thrsh             	0x19
#define	Button11_Touch_Thrsh             	0x1A
#define	Button12_Touch_Thrsh             	0x1B
#define	Button13_Touch_Thrsh            	0x1C
#define	Global_Hys_Thrsh                	0x1F

#define	Global_Noise_Thrsh               	0x20
#define	Global_NegNoise_Thrsh           	0x21
#define	Button_NegNoise_Num_Thrsh        	0x28
#define	Button_Debounce                  	0x29
#define Timeout_Cfg				  							0x2D
#define	Prox_Channel_Config1             	0x2E
#define	Prox_Channel_Config2            	0x2F

#define	Prox_Filter_Config1             	0x30
#define	Prox_Thrsh                       	0x34
#define	Prox_Hys_Thrsh                   	0x35
#define	Prox_Noise_Thrsh                 	0x36
#define	Prox_NegNoise_Thrsh             	0x37
#define	Prox_NegNoise_Num_Thrsh         	0x39
#define	Prox_Debounce                    	0x3A
#define	Scan_Period_Config              	0x3E
#define	PressHold_Thrsh                 	0x3F

#define Slide_Channel_Config              	0x40
#define	Irq_Ctrl                          	0x41
#define Irq_Repeat_Rate                   	0x42
#define Gpio1_Mode_Config                 	0x45

#define Button9_Signal_MSB                	0x70
#define Button9_Signal_LSB                	0x71
#define Button10_Signal_MSB               	0x72
#define Button10_Signal_LSB               	0x73
#define Button11_Signal_MSB               	0x74
#define Button11_Signal_LSB               	0x75
#define Button12_Signal_MSB               	0x76
#define Button12_Signal_LSB               	0x77
#define Button13_Signal_MSB               	0x78
#define Button13_Signal_LSB               	0x79

#define LPCD_Timer_Config                 	0x86


#define I2C_TIMEOUT 0xFFFF

#define TX1_TX2_CW_DISABLE 0
#define TX1_CW_ENABLE 1
#define TX2_CW_ENABLE 2
#define TX1_TX2_CW_ENABLE 3
#define LPCD_ENABLE 1
//#define LPCD_DISABLE 0
#define  Reader_ENABLE 0
extern uint8_t FM5114_TESTBUF[10];

extern AwakeSystemKeyMgr_t AwakeSystemKeyMgr;
extern bool_t g_TouchAwakeNeedReleaseKey;

//extern unsigned char Touch_IRQ;
//extern unsigned char GetReg(unsigned char device_address,unsigned char reg_address,unsigned char *reg_data);
//extern unsigned char SetReg(unsigned char device_address,unsigned char reg_address,unsigned char reg_data);

extern void FM175XX_HPD_I2C(unsigned char mode);

extern void SetCW(unsigned char cw_mode);
extern void FM5114_Touch_Reset(void);
extern unsigned char FM5114_Reader_SoftReset(void);
extern void FM175XX_HPD(unsigned char mode);

extern void ModifyReg(unsigned char device_address,unsigned char reg_address,unsigned char mask,unsigned char set);

extern void Write_FIFO(unsigned char reglen,unsigned char * regbuf);
extern void Read_FIFO(unsigned char reglen,unsigned char * regbuf);
extern void Clear_FIFO(void);
extern unsigned char GetReg_Ext(unsigned char device_address,unsigned char ext_reg_address,unsigned char* ext_reg_data);
extern unsigned char SetReg_Ext(unsigned char device_address,unsigned char ext_reg_address,unsigned char ext_reg_data);
extern void ModifyReg(unsigned char device_address,unsigned char reg_address,unsigned char mask,unsigned char set);

extern unsigned char FM5114_Touch_SoftReset(void);
extern unsigned char FM5114_Touch_Trim_Load(void);
extern unsigned char  FM5114_Touch_Init(unsigned char Sensi ,unsigned int ButtonChannel, unsigned char Lpcd_Timer_Irq_en);
extern unsigned char FM5114_Sensor_Debug(unsigned int channel);

extern void TOUCH_AWAKE(void);
extern void TOUCH_POWERDOWN(void);
extern void TOUCH_SUSPEND(void);
extern void TOUCH_RESUME(void);


extern keycode_t Key_Scan(void);
extern void Key_Scan_Init(void);

#endif

#endif





