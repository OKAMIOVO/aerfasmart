#include "Project.h"
#include "StdTypes.h"
#include "gpio.h"
#include "IO.h"
#include "Hardware.h"
#include "tim.h"
#include "iwdg.h"
#include "PIR.h"
#include "GUI.h"

BodyInductionMgr_t BodyInductionMgr;

#define DEF_IrBodyInductionFilterTimesLimited	7		//7 times,0.7*7 == 4.9s
#define DEF_IrBodyInductionFilterTimesInit		2		//7 times,0.7*2 == 1.4s

#define DEF_SensitivityAutoAdjustAsObstructionDetectedTimesLimited 257			//0.7s per cycle, for example, 257 = 180S


#if defined ProjectIs_AutoBarLock_S90Z01 || defined (ProjectIs_AutoBarLock_S90Z11)
#define DEF_IrPulseTxCnt    			9
#define DEF_IrPulseRxCnt_Trig   		9	
#define DEF_DefaultPIRduty_HighLevel	32
#define DEF_DefaultPIRduty_LowLevel		20
#define DEF_MinPIRduty_HighLevel		22
#define DEF_MinPIRduty_LowLevel			13
#else
#define DEF_IrPulseTxCnt    			9
#define DEF_IrPulseRxCnt_Trig   		9	
#define DEF_DefaultPIRduty_HighLevel	32
#define DEF_DefaultPIRduty_LowLevel		20
#define DEF_MinPIRduty_HighLevel		22
#define DEF_MinPIRduty_LowLevel			13
#endif


void PIR_IOs_Init(void)
{
	DisableTouch_Interrupt();
	SET_PIRPOWER_ON;
	MX_TIM1_Init();
	EnablePIR_Interrupt();
	Hardware_DelayX5us(400);	//Wait power stable, and IR Reciever output pin stable
}

void PIR_IOs_DeInit(void)
{
	DisablePIR_Interrupt();
	SET_PIRPOWER_OFF;
	MX_TIM1_DeInit();
	EnableTouch_Interrupt();
}

void PIR_PowerEnIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	SET_PIRPOWER_OFF;
}


void PIR_Init(void)
{
	BodyInductionMgr.IrBodyInductionFilterTimes = DEF_IrBodyInductionFilterTimesInit;
	BodyInductionMgr.BodyInductionInvalidTimes = 0;
	BodyInductionMgr.BodyInductionBeLimited = bFALSE;
	PIRmgr.HighLevelDuty = DEF_DefaultPIRduty_HighLevel;	// Period = 210,Duty Is 19%
	PIRmgr.LowLevelDuty = DEF_DefaultPIRduty_LowLevel;		// Period = 210,Duty Is 9%
	PIRmgr.ObstructionDetectedTimes = 0x0000;
	
	SET_PIRPOWER_OFF;
	DisablePIR_Interrupt();
}

void PIR_SensitivityDecrease(void)
{
	if ( BodyInductionMgr.SensingDistanceLevel == SensingDistanceL2 )
	{
		if ( PIRmgr.HighLevelDuty > DEF_MinPIRduty_HighLevel )
		{
			PIRmgr.HighLevelDuty--;
		}
	}
	else if ( BodyInductionMgr.SensingDistanceLevel == SensingDistanceL1 )
	{
		if ( PIRmgr.LowLevelDuty > DEF_MinPIRduty_LowLevel )
		{
			PIRmgr.LowLevelDuty--;
		}
	}
}

void PIR_Task(void)
{
	uint8_t i;

	//MX_TIM1_Init();
	CLRWDT();

	if (( IfSystemIsNoFaceUser() == bTRUE)
		&&(IfSystemIsInFactoryDefaultStatus()==bFALSE)
		)
	{
		PIRmgr.FaceDetected = bFALSE;
		return;
	}
		
							
	PIR_IOs_Init();

	if ( BodyInductionMgr.SensingDistanceLevel == SensingDistanceL2 )
	{
		SET_IR_PwmDuty(PIRmgr.HighLevelDuty);
		DEBUG_MARK;
	}
	else if ( BodyInductionMgr.SensingDistanceLevel == SensingDistanceL1 )
	{
		SET_IR_PwmDuty(PIRmgr.LowLevelDuty);
		DEBUG_MARK;
	}
	else
	{
		SET_IR_PwmDuty(0);
		DEBUG_MARK;
	}

	PIRmgr.RecPulse = 0;
	PIRmgr.SendPulse = DEF_IrPulseTxCnt;
	PIRmgr.FaceDetected = bFALSE;

	SET_IR_PwmStar();		//Send one pulse
	Hardware_DelayX5us(200);
	SET_IR_PwmStop();

	if ( PIRmgr.RecPulse > 0)
	{
		if ( BodyInductionMgr.IrBodyInductionFilterTimes == 0x00 )
		{
			PIRmgr.RecPulse = 0;
			Hardware_DelayX5us(100);
			
			for (i=0;i<PIRmgr.SendPulse;i++)
			{	
				SET_IR_PwmStar();
				Hardware_DelayX5us(200);
				SET_IR_PwmStop();
				Hardware_DelayX5us(200);
			}
			
			if ( PIRmgr.RecPulse >= ( DEF_IrPulseRxCnt_Trig ) )
			{
				PIRmgr.FaceDetected = bTRUE;
				BodyInductionMgr.IrBodyInductionFilterTimes = DEF_IrBodyInductionFilterTimesInit;
			}
			else 
			{
				if ( PIRmgr.ObstructionDetectedTimes < DEF_SensitivityAutoAdjustAsObstructionDetectedTimesLimited )
				{
					PIRmgr.ObstructionDetectedTimes++;
				}
				else
				{
					PIR_SensitivityDecrease();
					PIRmgr.ObstructionDetectedTimes = 0x0000;		//
					DEBUG_MARK;
				}
			}
		}
		else
		{
			if ( BodyInductionMgr.IrBodyInductionFilterTimes < DEF_IrBodyInductionFilterTimesLimited )
			{
				BodyInductionMgr.IrBodyInductionFilterTimes++;
			}
			
			if ( PIRmgr.ObstructionDetectedTimes < DEF_SensitivityAutoAdjustAsObstructionDetectedTimesLimited )
			{
				PIRmgr.ObstructionDetectedTimes++;
			}
			else
			{
				PIR_SensitivityDecrease();
				PIRmgr.ObstructionDetectedTimes = 0x0000;		//
				DEBUG_MARK;
			}
		}
	}
	else
	{
		if ( BodyInductionMgr.IrBodyInductionFilterTimes  > 0 )
		{
			BodyInductionMgr.IrBodyInductionFilterTimes--;
		}
		if ( PIRmgr.ObstructionDetectedTimes > 0 )
		{
			PIRmgr.ObstructionDetectedTimes--;
		}
	}
	
	
	PIR_IOs_DeInit();
	
	CLRWDT();
}
//#endif

