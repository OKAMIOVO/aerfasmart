#include "Project.h"
#include "IO.h"
#include "Hardware.h"
#include "StdTypes.h"
#include "rtc.h"
#include "I2cBus.h"
#include "RealTimeClock.h"
#include "HostUart.h"


/*************************************************************************************************		                                                           
*  Object:       
*  输入： 无
*  输出： 无								                         	                                     
*  备注：                                         
********************************************************************************************************/

#ifdef RTC_PCF8563
#define PCF8653_CONFIG_REG0 	0x00
#define PCF8653_CONFIG_REG1		0x00

#define PCF8653_CONFIG_REG2	 	0x00	//second
#define PCF8653_CONFIG_REG3		0x00	//minute
#define PCF8653_CONFIG_REG4		0x12	//hour
#define PCF8653_CONFIG_REG5		0x01	//day
#define PCF8653_CONFIG_REG6		0x01	//date,monday
#define PCF8653_CONFIG_REG7 	0x04	//month
#define PCF8653_CONFIG_REG8 	0x19	//year

#define PCF8653_CONFIG_REG9 	0x80
#define PCF8653_CONFIG_REG10 	0x80
#define PCF8653_CONFIG_REG11 	0x80
#define PCF8653_CONFIG_REG12 	0x80
#define PCF8653_CONFIG_REG13 	0x00
#define PCF8653_CONFIG_REG14 	0x00
#define PCF8653_CONFIG_REG15 	0x00

static uint8_t TestVal;
static uint8_t TestBuff1[20];

void RealTimeClock_Init(void) 
{
	uint8_t temp;

	I2C_ReadRandom(I2CADDR_RTC,0x02,&temp);
	
	DEBUG_MARK;
	
	I2C_WriteRandom(I2CADDR_RTC,0x00,PCF8653_CONFIG_REG0);
	I2C_WriteRandom(I2CADDR_RTC,0x01,PCF8653_CONFIG_REG1);

	if ((temp&0x80)!=0x00)		//if low voltage is trigger,reset the time
	{
	  	I2C_WriteRandom(I2CADDR_RTC,0x02,PCF8653_CONFIG_REG2);
	  	I2C_WriteRandom(I2CADDR_RTC,0x03,PCF8653_CONFIG_REG3);
		I2C_WriteRandom(I2CADDR_RTC,0x04,PCF8653_CONFIG_REG4);
	  	I2C_WriteRandom(I2CADDR_RTC,0x05,PCF8653_CONFIG_REG5);
		I2C_WriteRandom(I2CADDR_RTC,0x06,PCF8653_CONFIG_REG6);
	  	I2C_WriteRandom(I2CADDR_RTC,0x07,PCF8653_CONFIG_REG7);
		I2C_WriteRandom(I2CADDR_RTC,0x08,PCF8653_CONFIG_REG8);
	}

	Hardware_DelayMs(10);
}


void ReadSystemTime(void)
{
	uint8_t i;
	uint8_t BUFFER[7];
	/*
	for (i=0;i<16;i++)
	{
		BUFFER[i]=0x00;
	}
	
	DEBUG_MARK;
	*/
	//return;
	
	I2C_ReadSequential(I2CADDR_RTC,0x02,&BUFFER[0],7);

	SystemTime.second 	= 	BUFFER[0]&0x7F;		//Voltage Low bit is not care
	SystemTime.minute 	= 	BUFFER[1]&0x7F;	
	SystemTime.hour    	= 	BUFFER[2]&0x3F;	
	SystemTime.date    	= 	BUFFER[3]&0x3F;
	SystemTime.day		= 	BUFFER[4]&0x07;
	SystemTime.month 	= 	BUFFER[5]&0x1F;		//CENTURY BIT is not care 
	SystemTime.year    	=	BUFFER[6];

	DEBUG_MARK;
}


void WriteSystemTime(void)
{
	uint8_t i;
	uint8_t BUFF[7];
	
	BUFF[0]= SystemTime.second;	
	BUFF[1]= SystemTime.minute;	
	BUFF[2]= SystemTime.hour;
	BUFF[3]= SystemTime.date;
	BUFF[4]= SystemTime.day;
	BUFF[5]= SystemTime.month;
	BUFF[6]= SystemTime.year;

	I2C_WriteSequential(I2CADDR_RTC,0x02,&BUFF[0],7);

	DEBUG_MARK;
	
	//for (i=0;i<7;i++)
	//{
	//	BUFF[i]=0x88;;
	//}

	//I2C1_ReadSequential(I2CADDR_RTC,0x02,&BUFF[0],7);

	DEBUG_MARK;
	
}



void PCF8563_Test(void)
{
	//PCF8563_RegRead(&TestBuff1[0],10);
	
	DEBUG_MARK;
}


#else
void RealTimeClock_Init(void) 
{
	SystemTime.second = 	0x00;
	SystemTime.minute	= 	0x00;
	SystemTime.hour   = 	0x12;
	SystemTime.date   = 	0x01;
	SystemTime.day		= 	0x00;
	SystemTime.month 	= 	0x09;
	SystemTime.year   =		0x21;
	
	
	WriteSystemTime();
}
/*************************************************************************************************		                                                           
*  Object:       
*  输入： 无
*  输出： 无								                         	                                     
*  备注：                                         
********************************************************************************************************/
void ReadSystemTime(void)
{
	//return;
	RTC_TimeTypeDef Temp_RTC_Time;
	RTC_DateTypeDef Temp_RTC_Date;
	HAL_RTC_GetTime(&hrtc,&Temp_RTC_Time,RTC_FORMAT_BCD);
	HAL_RTC_GetDate(&hrtc,&Temp_RTC_Date,RTC_FORMAT_BCD);
	
	SystemTime.second = 	Temp_RTC_Time.Seconds;		
	SystemTime.minute	= 	Temp_RTC_Time.Minutes;	
	SystemTime.hour   = 	Temp_RTC_Time.Hours;	
	SystemTime.date   = 	Temp_RTC_Date.Date;
	SystemTime.day		= 	Temp_RTC_Date.WeekDay;
	SystemTime.month 	= 	Temp_RTC_Date.Month;		
	SystemTime.year   =	Temp_RTC_Date.Year;
	
}
/*************************************************************************************************		                                                           
*  Object:       
*  输入： 无
*  输出： 无								                         	                                     
*  备注：                                         
********************************************************************************************************/
void WriteSystemTime(void)
{
	//return;
	RTC_TimeTypeDef Temp_RTC_Time;
	RTC_DateTypeDef Temp_RTC_Date;
	
	Temp_RTC_Time.Hours = SystemTime.hour;
	Temp_RTC_Time.Minutes = SystemTime.minute;	
	Temp_RTC_Time.Seconds = SystemTime.second;	
	Temp_RTC_Time.TimeFormat = RTC_HOURFORMAT12_AM;	
	Temp_RTC_Time.SubSeconds = 0;
	Temp_RTC_Time.SecondFraction = 0;
	Temp_RTC_Time.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	Temp_RTC_Time.StoreOperation = RTC_STOREOPERATION_RESET ;
	
	Temp_RTC_Date.Date = SystemTime.date;	
	Temp_RTC_Date.WeekDay = SystemTime.day;	
	Temp_RTC_Date.Month = SystemTime.month;			
	Temp_RTC_Date.Year = SystemTime.year;	
	
	HAL_RTC_SetDate(&hrtc,&Temp_RTC_Date,RTC_FORMAT_BCD);
	HAL_RTC_SetTime(&hrtc,&Temp_RTC_Time,RTC_FORMAT_BCD);
}
/*************************************************************************************************		                                                           
*  Object:       
*  输入： 无
*  输出： 无								                         	                                     
*  备注：                                         
********************************************************************************************************/
void SystemTimeUpdate(void)
{
	ReadSystemTime();
	
	if (( SystemTime.SavedDateWhileNetWorkTimeUpdated!=SystemTime.date )
		||( ComportMgr.TimeSyncWithNetwork == bTRUE )
		)
	{
		ComPort_SetPost_GetNetWorkTime();
		SystemTime.SavedDateWhileNetWorkTimeUpdated=SystemTime.date;
		ComportMgr.TimeSyncWithNetwork = bFALSE;
	}

	#if defined (Function_YouzhiyunjiaWifi) || defined (Function_TuyaWifi)
	if ( SystemTime.SavedDateWhileNetWorkTimeUpdated!=SystemTime.date )
	{
		SystemTime.SavedDateWhileNetWorkTimeUpdated=SystemTime.date;
		Wifi_PostGetUTC(2);
	}
	#endif
}	

/*************************************************************************************************		                                                           
*  Object:       
*  输入： 无
*  输出： 无								                         	                                     
*  备注：                                         
********************************************************************************************************/
void EnableRTCWakeup(void)
{

}

/*************************************************************************************************		                                                           
*  Object:       
*  输入： 无
*  输出： 无								                         	                                     
*  备注：                                         
********************************************************************************************************/
void DisableRTCWakeup(void)
{

}
#endif


