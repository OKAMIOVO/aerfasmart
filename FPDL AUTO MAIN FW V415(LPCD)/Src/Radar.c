#include "StdTypes.h"
#include "gpio.h"
#include "IO.h"
#include "Hardware.h"
#include "Project.h"
#include "tim.h"
#include "iwdg.h"
#include "Radar.h"
#include "PIR.h"

static uint8_t RadarTestBuff[10];
	
void IIC_Stop(void);


/*-------------------------------------------------
 *  函数名IIC_GPIO_Init
 *	功能：  初始化IIC
  *  输入：  无
 *  输出：  无
 --------------------------------------------------*/
void IIC_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	SCL_OUT;
	SDA_OUT;
    IIC_Stop();
}

/*-------------------------------------------------
 *  函数名IC_Sleep
 *	功能：  进入睡眠状态，IIC配置为输入
  *  输入：  无
 *  输出：  无
 --------------------------------------------------*/
void IIC_Sleep(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	SCL_IN;
	SDA_IN;
}

/*-------------------------------------------------
 *  函数名：IIC_Start
 *	功能：  产生IIC起始信号
 *  输入：  无
 *  输出：  无
 --------------------------------------------------*/
void IIC_Start(void)
{	
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	SDA_OUT;
 	SET_RADAR_SDA_H;
    Hardware_DelayX1us(10);  	  
	SET_RADAR_SCL_H;
	Hardware_DelayX1us(10);
  	SET_RADAR_SDA_L;        //START:when CLK is high,DATA change form high to low 
	Hardware_DelayX1us(10);
	SET_RADAR_SCL_L;        //钳住I2C总线，准备发送或接收数据 
    Hardware_DelayX1us(10);          
}

/*-------------------------------------------------
 *  函数名：IIC_Stop
 *	功能：  产生IIC停止信号
 *  输入：  无
 *  输出：  无
 --------------------------------------------------*/
void IIC_Stop(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	SDA_OUT;          //SDA线输出
	SET_RADAR_SCL_L;
	SET_RADAR_SDA_L;        //STOP:when CLK is high DATA change form low to high
 	Hardware_DelayX1us(10);
	SET_RADAR_SCL_H; 
    Hardware_DelayX1us(10);
	SET_RADAR_SDA_H;        //发送I2C总线结束信号
	Hardware_DelayX1us(10);							   	
}

/*-------------------------------------------------
 *  函数名：IIC_Wait_Ack
 *	功能：  等待应答信号到来
 *  输入：  无
 *  输出：  返回值：1，接收应答失败
 *               0，接收应答成功
 --------------------------------------------------*/
unsigned char IIC_Wait_Ack(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	unsigned char ucErrTime=0;     
	
	SET_RADAR_SDA_H;
 	SDA_IN;               //SDA设置为输入 
	Hardware_DelayX1us(10);	   
	SET_RADAR_SCL_H;
	Hardware_DelayX1us(10);	 
	while(READ_RADAR_SDA_STATUS)
	{
		ucErrTime++;
		if(ucErrTime>250) //等待超时
		{
			IIC_Stop();
			return 1;
		}
	}
	SET_RADAR_SCL_L;            //时钟输出0 	   
	return 0;  
} 
/*-------------------------------------------------
 * 函数名：IIC_Ack
 *	功能：  产生ACK应答
 * 输入：  无
 * 输出：  无
 --------------------------------------------------*/
void IIC_Ack(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	SET_RADAR_SCL_L;
	SDA_OUT;              //SDA线输出
	SET_RADAR_SDA_L;
	Hardware_DelayX1us(10);	
	SET_RADAR_SCL_H;
	Hardware_DelayX1us(10);	
	SET_RADAR_SCL_L;
}

/*-------------------------------------------------
 *  函数名：IIC_NAck
 *	功能：  不产生ACK应答
 *  输入：  无
 *  输出：  无
 --------------------------------------------------*/	    
void IIC_NAck(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	SET_RADAR_SCL_L;
	SDA_OUT;              //SDA线输出
	SET_RADAR_SDA_H;
	Hardware_DelayX1us(10);	
	SET_RADAR_SCL_H;
	Hardware_DelayX1us(10);	
	SET_RADAR_SCL_L;
}	

/*-------------------------------------------------
 *  函数名：IIC_Send_Byte
 *	功能：  IIC发送一个字节
 *  输入：  写入要发送的一个字节数据txd
 *  输出：  无
 --------------------------------------------------*/		  
void IIC_Send_Byte(unsigned char txd)
{   
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
    unsigned char t;   
	SDA_OUT;	          //SDA线输出   
    SET_RADAR_SCL_L;      //拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
		if(txd&0x80)
		{
			SET_RADAR_SDA_H;
		}
		else
		{
			SET_RADAR_SDA_L;
		}
		txd<<=1; 	  
		Hardware_DelayX1us(10);				  
		SET_RADAR_SCL_H;
		Hardware_DelayX1us(10);	
		SET_RADAR_SCL_L;	
		Hardware_DelayX1us(10);
    }	 
} 	 

/*-------------------------------------------------
 *  函数名：IIC_Read_Byte
 *	功能：  IIC读一个字节
 *  输入：  无
 *  输出：  读出存储器里面的数据并返回receive
 --------------------------------------------------*/
unsigned char IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	SDA_IN;               //SDA设置为输入
    for(i=0;i<8;i++ )
	{
        SET_RADAR_SCL_L; 
        Hardware_DelayX1us(10);	
     	SET_RADAR_SCL_H;
        receive<<=1;
        if(READ_RADAR_SDA_STATUS)receive++;   
		Hardware_DelayX1us(10);	
    }		
    
	if (!ack)
    {
        IIC_NAck(); //发送nACK
	}
    else
    {
        IIC_Ack(); //发送ACK  
    }
    
    return receive;
}

/*-------------------------------------------------
 *  函数名：AT5815_ReadOneByte
 *	功能：  IIC读出制定位置的数据
 *  输入：  address
 *  输出：  读出address存储器里面的数据iicdata
 --------------------------------------------------*/
unsigned char AT5815_ReadOneByte(unsigned char address)
{
	unsigned char iicdata = 0;
	uint8_t errortimes = 0;
	IIC_READ_Begin:
		IIC_Start();
		IIC_Send_Byte(0x50);
		if(IIC_Wait_Ack())
		{
			if (errortimes++<15)
			{
				goto IIC_READ_Begin;
			}
			else{
				return 0xE0;
			}
		}
		IIC_Send_Byte(address%256);				    //填要读的数据地址
		if(IIC_Wait_Ack())
		{
			if (errortimes++<15)
			{
				goto IIC_READ_Begin;
			}
			else{
				return 0xE0;
			}
		}
		IIC_Start();
		IIC_Send_Byte(0x51);
		if(IIC_Wait_Ack())
		{
			if (errortimes++<15)
			{
				goto IIC_READ_Begin;
			}
			else{
				return 0xE0;
			}
		}
		iicdata=IIC_Read_Byte(0);
		IIC_Stop();		
		return iicdata;
}
 /*-------------------------------------------------
 *  函数名：AT5815_WriteOneByte
 *	功能：  IIC把数据data写入制定的位置address
 *  输入：  address，data
 *  输出：  无
 --------------------------------------------------*/
void AT5815_WriteOneByte(unsigned char address,unsigned char data)
{
	uint8_t errortimes = 0;
	
	IIC_WRITE_Begin:
		IIC_Start();
		IIC_Send_Byte(0x50);
		if(IIC_Wait_Ack())
		{
			if (errortimes++<15)
			{
				goto IIC_WRITE_Begin;
			}
			else{
				return;
			}
		}

		IIC_Send_Byte(address%256);
		if(IIC_Wait_Ack())
		{
			if (errortimes++<15)
			{
				goto IIC_WRITE_Begin;
			}
			else{
				return;
			}
		}

		IIC_Send_Byte(data);
		if(IIC_Wait_Ack())
		{
			if (errortimes++<15)
			{
				goto IIC_WRITE_Begin;
			}
			else{
				return;
			}
		}

		IIC_Stop();	
    Hardware_DelayX5us(100);
}


/*
软件复位
*/
void soft_reset(void)
{
	AT5815_WriteOneByte(0x00, 0x00);
	AT5815_WriteOneByte(0x00, 0x01);
}

/*
设置自检时间
*/
void set_self_check_time(unsigned int self_check_time)
{
	AT5815_WriteOneByte(0x38, (unsigned char)(self_check_time));
	AT5815_WriteOneByte(0x39, (unsigned char)(self_check_time >> 8));
}

/*
设置亮灯时间基底时间-最短为500ms
*/
void set_light_on_base_time(unsigned long light_on_base_time)
{
	AT5815_WriteOneByte(0x3d, (unsigned char)(light_on_base_time));
	AT5815_WriteOneByte(0x3e, (unsigned char)(light_on_base_time >> 8));
	AT5815_WriteOneByte(0x3f, (unsigned char)(light_on_base_time >> 16));
	AT5815_WriteOneByte(0x40, (unsigned char)(light_on_base_time >> 24));
}

/*
设置关灯保护时间
*/
void set_light_off_time(unsigned int light_off_time)
{
	AT5815_WriteOneByte(0x4e, (unsigned char)(LIGHT_OFF_TIME));
	AT5815_WriteOneByte(0x4f, (unsigned char)(LIGHT_OFF_TIME >> 8));
}

/*
设置感应距离
*/
void set_distance(unsigned char pwr_40uA_switch, unsigned int delta, unsigned char gain)
{
	// 配置功耗
    if (pwr_40uA_switch == 1)  // 40uA
    {
        AT5815_WriteOneByte(0x68, (0x48&0xc7)|0x38);
		AT5815_WriteOneByte(0x67, (0xf3&0xf0)|0x08);
	}
    else   // 70uA
    {
        AT5815_WriteOneByte(0x68, 0x48);
		AT5815_WriteOneByte(0x67, 0xf3);	
	}
    // 写入delta值
	AT5815_WriteOneByte(0x10, (unsigned char)(delta));
	AT5815_WriteOneByte(0x11, (unsigned char)(delta >> 8));	
	// 写入gain值
	AT5815_WriteOneByte(0x5C, gain);
}

/*
设置感应时间
*/
void set_light_on_time(unsigned long light_on_time)
{
	AT5815_WriteOneByte(0x41, 0x01);
	AT5815_WriteOneByte(0x42, (unsigned char)(light_on_time));
	AT5815_WriteOneByte(0x43, (unsigned char)(light_on_time >> 8));
	AT5815_WriteOneByte(0x44, (unsigned char)(light_on_time >> 16));
	AT5815_WriteOneByte(0x45, (unsigned char)(light_on_time >> 24));
}

/*
设置光敏功能
*/
void set_light_sensor_threshold(unsigned char onoff, 
								unsigned int light_sensor_value_high, 
								unsigned int light_sensor_value_low, 
                                unsigned int light_sensor_iniverse)
{
	if (onoff)
		AT5815_WriteOneByte(0x66, 0x42);
	else
		AT5815_WriteOneByte(0x66, 0x4a);
	
	AT5815_WriteOneByte(0x34, (unsigned char)(light_sensor_value_low));
	AT5815_WriteOneByte(0x35, (unsigned char)(light_sensor_value_high));
	AT5815_WriteOneByte(0x36, (unsigned char)((light_sensor_value_low >> 8) | ((light_sensor_value_high >> 8)<<2) | (light_sensor_iniverse << 4)));
}

/*
控制RF模块
关闭RF功能后，模块关闭感应功能，可节省约10uA功耗
*/
void control_rf_module(unsigned char onoff)
{
    if (onoff)
    {
		AT5815_WriteOneByte(0x5d, 0x45);
		AT5815_WriteOneByte(0x62, 0x55);
		AT5815_WriteOneByte(0x51, 0xa0);
	}
    else
    {
		AT5815_WriteOneByte(0x5d, 0x46);
		AT5815_WriteOneByte(0x62, 0xaa);
		AT5815_WriteOneByte(0x51, 0x50);
	}
}

/*
设置窗口检测长度&阈值
*/
void set_window_detect(unsigned char window_length, unsigned char window_threshold)
{
	AT5815_WriteOneByte(0x31, window_length);		// Default: detect Win = 4
	AT5815_WriteOneByte(0x32, window_threshold);	// Default: 3
}

/*
AT5815初始化
*/
void AT5815_REG_Init(uint8_t SensingDistance)
{
	control_rf_module(1);
		
	if ( SensingDistance == 2 )
	{
    	set_distance(PWR_40UA_SWITCH, DELTA_L2, GAIN_L2);
	}
	else
	{
    	set_distance(PWR_40UA_SWITCH, DELTA_L1, GAIN_L1);
	}
	
	set_light_on_time(LIGHT_ON_TIME);
    
	set_self_check_time(SELF_CHECK_TIME);
	
    set_light_on_base_time(LIGHT_ON_BASE_TIME);
	
	set_light_off_time(LIGHT_OFF_TIME);
	
	// 光敏功能，需要时再开启
	// set_light_sensor_threshold(LIGHT_SENSOR_STATUS, LIGHT_SENSOR_VALUE_LOW, LIGHT_SENSOR_VALUE_HIGH, LIGHT_SENSOR_INIVERSE);

	AT5815_WriteOneByte(0x55, 0x04);
	
    soft_reset();
} 

uint16_t AT5815_ReadDelta(void)
{
	uint16_t tempDelta;
	
	tempDelta = AT5815_ReadOneByte(0x11); 
	tempDelta <<=8;
	tempDelta +=AT5815_ReadOneByte(0x10);
	
	return tempDelta;
}

uint8_t AT5815_ReadGain(void)
{
	uint8_t tempGain;
	
	tempGain = AT5815_ReadOneByte(0x5C); 
	
	return tempGain;
}

/*
更新AT5815参数
*/
void AT5815_update_param(void)
{
    //这里更新参数，如感应距离、输出时间等
    
	// 复位使参数生效
	soft_reset();
}

void Radar_Test(void)
{

	RadarTestBuff[0] = AT5815_ReadOneByte(0x10);
	DEBUG_MARK;
	RadarTestBuff[1] = AT5815_ReadOneByte(0x31);
	DEBUG_MARK;
	RadarTestBuff[2] = AT5815_ReadOneByte(0x32);
	DEBUG_MARK;
	RadarTestBuff[3] = AT5815_ReadOneByte(0x10);
	DEBUG_MARK;
	
}

status_t Radar_SelfTest(void)
{
	IIC_GPIO_Init();  // 初始化IIC
	if ( AT5815_ReadOneByte(0x00) == 0x01)
	{
		return S_SUCCESS;
	}
	else
	{
		return S_FAIL;
	}
	IIC_Sleep();
}


status_t Radar_Init(void)
{
	uint16_t Read_Delta;
	uint8_t Read_Gain,FailTimes;
	status_t RadarInitResult;

	RadarInitResult = S_FAIL;
	FailTimes = 0;

	while(1)
	{
		IIC_GPIO_Init();  // 初始化IIC
		
		AT5815_REG_Init(BodyInductionMgr.SensingDistanceLevel);

		Read_Delta = AT5815_ReadDelta();
		Read_Gain = AT5815_ReadGain();

		if ( BodyInductionMgr.SensingDistanceLevel == SensingDistanceL2 )
		{
			if ((Read_Delta == DELTA_L2)&&(Read_Gain == GAIN_L2))
			{
				DEBUG_MARK;
				RadarInitResult = S_SUCCESS;
				break;
			}
			else
			{
				FailTimes++;
			}
		}
		else if ( BodyInductionMgr.SensingDistanceLevel == SensingDistanceL1 )
		{
			if ((Read_Delta == DELTA_L1)&&(Read_Gain == GAIN_L1))
			{
				DEBUG_MARK;
				RadarInitResult = S_SUCCESS;
				break;			
			}
			else
			{
				FailTimes++;
			}
		}
		else
		{
			if ((Read_Delta == DELTA_L1)&&(Read_Gain == GAIN_L1))
			{
				DEBUG_MARK;
				RadarInitResult = S_SUCCESS;
				break;			
			}
			else
			{
				FailTimes++;
			}
			BodyInductionMgr.SensingDistanceLevel = SensingDistanceL0;
		}

		if ( FailTimes > 3 )
		{
			//BodyInductionMgr.SensingDistanceLevel = SensingDistanceL0;		//if write parameter failed, disable Radar
			DEBUG_MARK;
			break;			
		}
	}

	IIC_Sleep();

	return RadarInitResult;
	
}


void RadarAwake_TurnOn(void)
{
	EnableRADAR_Interrupt();
}
void RadarAwake_TurnOff(void)
{
	DisableRADAR_Interrupt();
}


