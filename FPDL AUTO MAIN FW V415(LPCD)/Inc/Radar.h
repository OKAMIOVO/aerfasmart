#ifndef RADAR_H
#define RADAR_H

#include "Project.h"

/*========================参数配置======================*/
#define	TRUE	1
#define	FALSE	0

#define	PWR_40UA_SWITCH			TRUE	// 配置功耗TRUE - 40uA；FALSE - 68uA
#define	SELF_CHECK_TIME			2000	// 上电自检时间 0~65536 ms 
#define	LIGHT_OFF_TIME			1000	// 关灯保护时间，建议1000ms
#define	LIGHT_ON_BASE_TIME		500		// 保持默认500ms
#define	LIGHT_ON_TIME			1500	// 总开灯时间=LIGHT_ON_BASE_TIME+LIGHT_ON_TIME

//#define	DELTA					200		// delta值：0~1023，值越大距离越短
//#define	GAIN					0x3B	// default = 0x3B  建议默认,值越大距离越短。如在近距离感应场景（2m内）使用，可配置为0x7B ~ 0x9B

#if defined (ProjectIs_AutoBarLock_S68Z04) || defined (ProjectIs_AutoBarLock_S68Z10)
#define	DELTA_L1				180		// delta值：0~1023，值越大距离越短
#define	GAIN_L1					0x6B//0x4B	// default = 0x3B  建议默认,值越大距离越短。如在近距离感应场景（2m内）使用，可配置为0x7B ~ 0x9B

#define	DELTA_L2				140		//60 delta值：0~1023，值越大距离越短
#define	GAIN_L2					0x6B//0x4B	// default = 0x3B  建议默认,值越大距离越短。如在近距离感应场景（2m内）使用，可配置为0x7B ~ 0x9B
#else
#define	DELTA_L1				250//80		// delta值：0~1023，值越大距离越短
#define	GAIN_L1					0x4B//0x4B	// default = 0x3B  建议默认,值越大距离越短。如在近距离感应场景（2m内）使用，可配置为0x7B ~ 0x9B

#define	DELTA_L2				200		//60 delta值：0~1023，值越大距离越短
#define	GAIN_L2					0x4B//0x4B	// default = 0x3B  建议默认,值越大距离越短。如在近距离感应场景（2m内）使用，可配置为0x7B ~ 0x9B
#endif

// 以下是光敏配置功能
#define	LIGHT_SENSOR_STATUS			0		// 0-关闭光敏 1-开启光敏
#define	LIGHT_SENSOR_VALUE_LOW   	500		// 光敏阈值 : 0~1023, 10bit
#define	LIGHT_SENSOR_VALUE_HIGH  	530		// 光敏阈值 : 0~1023, 10bit
#define	LIGHT_SENSOR_INIVERSE    	0		// 默认值0，不要改动

/*===================================================*/

#define PORTMACRO_RadarSDA_Port	GPIOB
#define PINMACRO_RadarSDA_Pin	GPIO_PIN_3
#define PORTMACRO_RadarSCL_Port	GPIOC
#define PINMACRO_RadarSCL_Pin	GPIO_PIN_4


#define SET_RADAR_SDA_H		HAL_GPIO_WritePin(PORTMACRO_RadarSDA_Port,PINMACRO_RadarSDA_Pin,GPIO_PIN_SET);
#define SET_RADAR_SDA_L		HAL_GPIO_WritePin(PORTMACRO_RadarSDA_Port,PINMACRO_RadarSDA_Pin,GPIO_PIN_RESET);
#define SET_RADAR_SCL_H		HAL_GPIO_WritePin(PORTMACRO_RadarSCL_Port,PINMACRO_RadarSCL_Pin,GPIO_PIN_SET);	
#define SET_RADAR_SCL_L		HAL_GPIO_WritePin(PORTMACRO_RadarSCL_Port,PINMACRO_RadarSCL_Pin,GPIO_PIN_RESET);

#define READ_RADAR_SDA_STATUS (HAL_GPIO_ReadPin(PORTMACRO_RadarSDA_Port,PINMACRO_RadarSDA_Pin))



#define SDA_OUT  {GPIO_InitStruct.Pin = PINMACRO_RadarSDA_Pin;GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;GPIO_InitStruct.Pull = GPIO_NOPULL;HAL_GPIO_Init(PORTMACRO_RadarSDA_Port, &GPIO_InitStruct);}
#define SDA_IN	 {GPIO_InitStruct.Pin = PINMACRO_RadarSDA_Pin;GPIO_InitStruct.Mode = GPIO_MODE_INPUT;GPIO_InitStruct.Pull = GPIO_PULLUP;HAL_GPIO_Init(PORTMACRO_RadarSDA_Port, &GPIO_InitStruct);}

#define SCL_OUT  {GPIO_InitStruct.Pin = PINMACRO_RadarSCL_Pin;GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;GPIO_InitStruct.Pull = GPIO_NOPULL;HAL_GPIO_Init(PORTMACRO_RadarSCL_Port, &GPIO_InitStruct);}
#define SCL_IN	 {GPIO_InitStruct.Pin = PINMACRO_RadarSCL_Pin;GPIO_InitStruct.Mode = GPIO_MODE_INPUT;GPIO_InitStruct.Pull = GPIO_PULLUP;HAL_GPIO_Init(PORTMACRO_RadarSCL_Port, &GPIO_InitStruct);}


extern status_t Radar_SelfTest(void);

extern status_t Radar_Init(void);

extern void RadarAwake_TurnOn(void);
extern void RadarAwake_TurnOff(void);

extern void Radar_Test(void);


#endif

