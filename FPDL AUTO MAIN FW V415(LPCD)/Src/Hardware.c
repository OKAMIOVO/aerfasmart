#include "stm32g0xx_hal.h"
#include "stdtypes.h"
#include "Project.h"
#include "hardware.h"
#include "IO.h"
#include "I2cBus.h"
#include "BeepMgr.h"
#include "LEDsMgr.h"
#include "GUI.h"
#include "CSK14.h"
#include "TSM12S.h"
#include "adc.h"
#include "usart.h"
#include "tim.h"

//OPTION Byte Setting "00 00 01 00 00 00"
#define DELAY_1MS 1150  // for 40MIPS

#define a2d_databuffer_vbat	0

#ifdef ProjectIs_SlideLock_S0604		//≥¨≥£
#define DEF_SliderMotorProtectionTime 6144		//6S
#define DEF_SlideMotorPwmDutyLimited_CW		90
#define DEF_SlideMotorPwmDutyLimited_CCW	80
#define DEF_SlideMotorCurrentLimited_CW		200		//in mA
#define DEF_SlideMotorCurrentLimited_CCW	130		//in mA
#define DEF_SlideMotorOverCurrentValue_CW	180		//in mA
#define DEF_SlideMotorOverCurrentValue_CCW	110		//in mA
#define DEF_SlideMotorOverCurrenTimeLimited 128		//0.125s

#elif defined ProjectIs_SlideLock_S0603
#define DEF_SliderMotorProtectionTime 6144		//6S
#define DEF_SlideMotorPwmDutyLimited_CW		85
#define DEF_SlideMotorPwmDutyLimited_CCW	60
#define DEF_SlideMotorCurrentLimited_CW		200		//in mA
#define DEF_SlideMotorCurrentLimited_CCW	130		//in mA
#define DEF_SlideMotorOverCurrentValue_CW	180		//in mA
#define DEF_SlideMotorOverCurrentValue_CCW	110		//in mA
#define DEF_SlideMotorOverCurrenTimeLimited 128		//0.125s

#elif defined ProjectIs_SlideLock_S3301		//ÕÚ’π
#define DEF_SliderMotorProtectionTime 4096		//4S
#define DEF_SlideMotorPwmDutyLimited_CW		90
#define DEF_SlideMotorPwmDutyLimited_CCW	80
#define DEF_SlideMotorCurrentLimited_CW		250		//in mA
#define DEF_SlideMotorCurrentLimited_CCW	140		//in mA
#define DEF_SlideMotorOverCurrentValue_CW	200		//in mA
#define DEF_SlideMotorOverCurrentValue_CCW	130		//in mA
#define DEF_SlideMotorOverCurrenTimeLimited 128		//0.125s

#elif defined ProjectIs_SlideLock_S8888		//
#define DEF_SliderMotorProtectionTime 4096		//4S
#define DEF_SlideMotorPwmDutyLimited_CW		90
#define DEF_SlideMotorPwmDutyLimited_CCW	80
#define DEF_SlideMotorCurrentLimited_CW		250		//in mA
#define DEF_SlideMotorCurrentLimited_CCW	120		//in mA
#define DEF_SlideMotorOverCurrentValue_CW	200		//in mA
#define DEF_SlideMotorOverCurrentValue_CCW	110		//in mA
#define DEF_SlideMotorOverCurrenTimeLimited 128		//0.125s

#endif



uint32_t a2d_data[2];

//static uint16_t savedVbat;

enum
   {
   	ATask_Init_SetSw =0,
   	ATask_VBAT=1,
		ATask_SpeedSensor,
		ATask_speedsignal,
		ATask_HallSensor,
		ATask_Vout
   } analogTaskList;
 

	 // Changed from 16000/5 to /4 because loop runs in 4 cycle - see assy
// changed to /10 to add wdt drive & add stability
void  Hardware_DelayMs(uint16_t time)
{
    uint16_t delay;
	
	HAL_Delay(time);
  
}
/*********************************************************************
* Function:  void  DelayUs(uint16_t time)
*
* PreCondition: none
* Input: time - delay in microseconds
* Output: none
* Side Effects: 
* Overview: delays execution on time specified in uS
* Note: none
*
********************************************************************/
void Hardware_DelayX1us(uint16_t time)
 {
	while(time--)
	  {
	  }
 }

void  Hardware_DelayX5us(uint16_t time)
 {
    while(time--)
      {
		Hardware_DelayX1us(8);
      }
 }

 status_t UART1_Write_TxBUFFER(unsigned char *buff,unsigned char len)
{
	unsigned char i;
	if ( UART1_Mgr.TxLength !=0x00)		//if TX Buffer is not empty
	{
		return S_FAIL;
	}
	
	for ( i=0;i<len;i++)
	{
			UART1_Mgr.TxBUFFER[i] = *buff;
			buff++;
	}
	UART1_Mgr.TxLength = len;
	
	return S_SUCCESS;
}

void UART1_TX_Task(void)
{
		if ( UART1_Mgr.TxPoint < UART1_Mgr.TxLength )
		{
			//USART3_DR = UART1_Mgr.TxBUFFER[UART1_Mgr.TxPoint];
			if ( HAL_UART_Transmit(&huart1,&UART1_Mgr.TxBUFFER[UART1_Mgr.TxPoint],1,5) == HAL_OK )
			{
				UART1_Mgr.TxPoint++;
				DEBUG_MARK;
			}
		}
		else if ( (UART1_Mgr.TxPoint==UART1_Mgr.TxLength)&&(UART1_Mgr.TxLength > 0) )
		{
			UART1_Mgr.TxPoint = 0;
			UART1_Mgr.TxLength = 0;
		}
}
void UART1_CLEAR_TXBUFF(void)
{
	UART1_Mgr.TxPoint = 0;
	UART1_Mgr.TxLength = 0;
	UART1_Mgr.Status = Idle;
}


/* ***************************************************************************
*  Battery Voltage A2D  Filter
*
*/
void HardwareBatVoltageA2dFilter(uint16_t NewA2d)
{
	
	if ( NewA2d > a2d_data[a2d_databuffer_vbat] )
	{
		if ( NewA2d > (50+a2d_data[a2d_databuffer_vbat]) ){
			a2d_data[a2d_databuffer_vbat]+= 50;
		}
		else if ( NewA2d > (10+a2d_data[a2d_databuffer_vbat]) ){
			a2d_data[a2d_databuffer_vbat]+= 5;
		}
	}
	else if ( a2d_data[a2d_databuffer_vbat] > NewA2d )
	{
		if ( a2d_data[a2d_databuffer_vbat] > (NewA2d+50) ){
			a2d_data[a2d_databuffer_vbat]-= 50;
		}
		else if ( a2d_data[a2d_databuffer_vbat] > (NewA2d+10) )
		{
			a2d_data[a2d_databuffer_vbat]-= 5;
		}
	}
	
	//a2d_data[a2d_databuffer_vbat] = NewA2d;
}

void HardwareBatteryTypeAutoSwitch(void)
{
	#ifdef Function_LiBatteryOrZnMnBatteryAutoSwitch
  	if ( BatteryMgr.BatteryVoltage > 67 )
	{
		BatteryMgr.BatteryType = LiBattery;
	}
	else
	{
		BatteryMgr.BatteryType = ZnMnBattery;
	}
	#else
	BatteryMgr.BatteryType = LiBattery;
	#endif

	#ifdef DEBUG_MARK
	//BatteryMgr.BatteryType = ZnMnBattery;
	#endif

}
void HardwareBatteryMgr_Task(void)
{
	
	BatteryMgr.BatteryVoltage = (a2d_data[a2d_databuffer_vbat]*13)/100;	//in 0.1v, R1=30K, R2=10K
	
	BatteryMgr.BatteryVoltage+=3;		//Battery voltage will drop 0.3V as serial connected SS34

	if ( BatteryMgr.BatteryType == LiBattery )
	{
		if ( BatteryMgr.BatteryVoltage > 75 ){	
			BatteryMgr.BatteryLevel = LEVEL_4;	
		}
		else if ( BatteryMgr.BatteryVoltage > 73 ){ 	
			BatteryMgr.BatteryLevel = LEVEL_3;	
		}
		else if ( BatteryMgr.BatteryVoltage > 69 ){ 	
			BatteryMgr.BatteryLevel = LEVEL_2;	
		}
		else if ( BatteryMgr.BatteryVoltage > 67 ){ 	
			BatteryMgr.BatteryLevel = LEVEL_1;	//6.8V
		}
		else {		
			BatteryMgr.BatteryLevel = LEVEL_0;	
		}
	}
	else		//ZnMnBattery
	{		
	    if ( BatteryMgr.BatteryVoltage > 58 ){		
			BatteryMgr.BatteryLevel = LEVEL_4;	
		}
		else if ( BatteryMgr.BatteryVoltage > 54 ){		
			BatteryMgr.BatteryLevel = LEVEL_3;	
		}
		else if ( BatteryMgr.BatteryVoltage > 50 ){		
			BatteryMgr.BatteryLevel = LEVEL_2;	
		}
		else if ( BatteryMgr.BatteryVoltage > 47 ){		
			BatteryMgr.BatteryLevel = LEVEL_1;	//6.8V
		}
		else {		
			BatteryMgr.BatteryLevel = LEVEL_0;	
		}
	}
	
	if ( BatteryMgr.BatteryLevel == LEVEL_0 )
	{
		if ( BatteryMgr.ProtectVoltageTriggerTimes < 3 )
		{
			BatteryMgr.ProtectVoltageTriggerTimes++;	
		}
		else{
			//BatteryMgr.LowBatteryProtectionEnabled = bTRUE;
		}
	}
	else
	{
		if ( BatteryMgr.ProtectVoltageTriggerTimes > 0 )
		{
			BatteryMgr.ProtectVoltageTriggerTimes--;	
		}
		else{
			BatteryMgr.LowBatteryProtectionEnabled = bFALSE;
		}
	}	
}


/*************************************************************************
*   Hardware_Task_Analog_Battery() - Acquires A/D Data, processes touch          *
************************************************************************** 
*
*   Enter:      nothing                     
*   Calls:      Hardware_Touch_State(), Hardware_A2D_init()
*               Hardware_A2D_start(), Hardware_A2D_Fetch()                                                                                   
*   Returns:    data in local storage for call backs
*   Uses:       lots
*   General:    This task runs at the main loop 64 hertz task rate
*               and controls acquisition of the five channels of A/D 
*               data.
*               Touch screen are acquired fifteen times followed by
*               one acquisition of temp and humidity.  So temp and
*               humidity acquire twice per second, and touch acquires
*               30 times per second.
*/

void Hardware_Task_Analog_Battery( void)
{
	//return;
	uint32_t temp_a2d;
	
	SET_ADC_Channel_Battery();
	
	Hardware_DelayX1us(20);
	
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1,10);
	temp_a2d = HAL_ADC_GetValue(&hadc1);
	HardwareBatVoltageA2dFilter(temp_a2d );
	DEBUG_MARK;
}


void Hardware_AwakeLine_SleepStatus(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
		
	/*Configure GPIO pin : PD3 */
	HAL_GPIO_DeInit(GPIOD,GPIO_PIN_3);

	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_3);

	GPIO_InitStruct.Pin = GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	DEBUG_MARK;
}

void Hardware_AwakeLine_WorkingStatus(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
		
	/*Configure GPIO pin : PD3 */
	HAL_GPIO_DeInit(GPIOD,GPIO_PIN_3);

	GPIO_InitStruct.Pin = GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	SET_AWAKE_INT_L;
	
}

void Hardware_Awake_Driver(void)
{

		SET_AWAKE_INT_H;
		Hardware_DelayMs(2);
		SET_AWAKE_INT_L;
		Hardware_DelayMs(2);

}


	



