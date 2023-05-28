/* 头文件包含 INCLUDES */
#include "stdtypes.h"
#include "Project.h"
#include "hardware.h"
#include "IO.h"
#include "usart.h"
#include "HostUart.h"
#include "FaceRecoginitionMgr.h"

#define DEF_VoiceBusyFilterTimesLimited 50

#define DEF_FRMFlowChartTimeOutDelay		64

static uint16_t TimeCunt1024Hz;
uint8_t UART1_RXBUFFER[10];
uint8_t UART2_RXBUFFER[10];
uint8_t UART3_RXBUFFER[10];


uint8_t tempvoicestatusvalue;

Uart3Mgr_t UART3_Mgr;


void SysTick_ISR(void)
{
		G_tflagbits.T1024Hz =1;
		TimeCunt1024Hz++;
	//TOGGLE_DEBUG_LED;
		if ( TimeCunt1024Hz%4==0 )
		{
			G_tflagbits.T256Hz =1;	
			if ( TimeCunt1024Hz%8 ==0 )
			{
				G_tflagbits.T128Hz =1;
				if ( TimeCunt1024Hz%16 ==0 )
				{
					G_tflagbits.T64Hz =1;
					if ( TimeCunt1024Hz%64 ==0 )
					{
						G_tflagbits.T16Hz =1;
						if ( TimeCunt1024Hz%128==0 )
						{
							G_tflagbits.T8Hz =1;
							if ( TimeCunt1024Hz%512==0 )
							{
								G_tflagbits.T2Hz =1;	
								if ( TimeCunt1024Hz==1024 )
								{	
									TimeCunt1024Hz = 0;
									G_tflagbits.T1Hz =1;
								}
							}
						}
					}	
				}
			}
		}
		DEBUG_MARK;
		return;
  /* USER CODE END SysTick_IRQn 1 */
}

void TIM3_ISR(void)
{
		DEBUG_MARK;
		//300us interval

		//TOGGLE_DEBUG_LED;

		#ifdef Function_MainBoardWithoutVoicePlayer
		
		SET_VOICEDATA_H;
		
		#else
		
		if ( VoiceDataTransferMgr.PulseWidthCnt > 0 )
		{
			VoiceDataTransferMgr.PulseWidthCnt--;
		}
		
		if ( VoiceDataTransferMgr.PulseWidthCnt == 0 )
		{
			tempvoicestatusvalue = VoiceDataTransferMgr.Status;
			
			switch ( VoiceDataTransferMgr.Status )
			{			
				case VoiceIdle:
					
					SET_VOICEDATA_H;

					if ( STATUS_PINMACRO_VOICEBUSY != 0x00 )
					{ 
						if ( VoiceDataTransferMgr.BusyPinStatusFilter < DEF_VoiceBusyFilterTimesLimited )
						{
							VoiceDataTransferMgr.BusyPinStatusFilter++;
						}
						else
						{
							VoiceDataTransferMgr.VoicePlayEnd = bTRUE;
							DEBUG_MARK;
						}
					}
					
					if (VoiceDataBufferMgr.VoiceDataReady == bTRUE )
					{
						VoiceDataTransferMgr.BitPoint = 0x00;
						VoiceDataTransferMgr.SendDataNum = 0;
						VoiceDataTransferMgr.TotalDataNum = VoiceDataBufferMgr.DataNum;
						memcpy(&VoiceDataTransferMgr.DataBuff[0],&VoiceDataBufferMgr.DataBuff[0],VoiceDataBufferMgr.DataNum);
						VoiceDataTransferMgr.Status = SendingDataStart;
						VoiceDataTransferMgr.VoicePlayEnd = bFALSE;			
						VoiceDataBufferMgr.VoiceDataReady = bFALSE;
					}

					VoiceDataTransferMgr.PulseWidthCnt = 10; //3MS
					
					break;	
						
				case SendingDataStart:
	
					if ( STATUS_PINMACRO_VOICEDATA != GPIO_PIN_RESET )
					{
						SET_VOICEDATA_L;
						VoiceDataTransferMgr.PulseWidthCnt = 17;	//5.1ms
					}
					else
					{
						VoiceDataTransferMgr.Status =  SendingData;//SendingData;
						VoiceDataTransferMgr.Data = VoiceDataTransferMgr.DataBuff[VoiceDataTransferMgr.SendDataNum];
					}
						
					break;
					
				case SendingData:
	
						if ( STATUS_PINMACRO_VOICEDATA == GPIO_PIN_RESET )	//send High level
						{
							SET_VOICEDATA_H;
							if ( (VoiceDataTransferMgr.Data&0x01) != 0x00)
							{
								VoiceDataTransferMgr.PulseWidthCnt = 3;	//900US
							}
							else
							{
								VoiceDataTransferMgr.PulseWidthCnt = 1;	//300US
							}
						}
						else		//send low level
						{
							SET_VOICEDATA_L;
							if ( (VoiceDataTransferMgr.Data&0x01) != 0x00)
							{
								VoiceDataTransferMgr.PulseWidthCnt = 1;	//300US
							}
							else
							{
								VoiceDataTransferMgr.PulseWidthCnt = 3;	//900US
							}
	
							VoiceDataTransferMgr.Data>>=1;
	
							if ( ++VoiceDataTransferMgr.BitPoint >=8 )
							{
								VoiceDataTransferMgr.Status = SendingDataEnd;
								DEBUG_MARK;
							}
						}
					break;
					
				case SendingDataEnd:	
						
						SET_VOICEDATA_H;
					
						VoiceDataTransferMgr.SendDataNum++;
					
						if (VoiceDataTransferMgr.SendDataNum < VoiceDataTransferMgr.TotalDataNum )
						{
							VoiceDataTransferMgr.Status = ContinueSendData;
							
							if ( VoiceDataTransferMgr.SendDataNum % 2 == 1 )		//First byte of command
							{
								VoiceDataTransferMgr.PulseWidthCnt = 7; //2.1ms
							}
							else
							{
								if ( VoiceDataTransferMgr.DataBuff[VoiceDataTransferMgr.SendDataNum-1] < 0xFF )		//data
								{
									VoiceDataTransferMgr.PulseWidthCnt = 34; //10.2ms
								}
								else //command
								{
									VoiceDataTransferMgr.PulseWidthCnt = 17; //5.1ms
								}
							}
						}
						else
						{
							VoiceDataTransferMgr.Status = WaitForVoicePlayerIdle;
							VoiceDataTransferMgr.PulseWidthCnt = 167; //50ms
						}
						
						DEBUG_MARK;
					break;	
					
				case ContinueSendData:	

						VoiceDataTransferMgr.Status = SendingDataStart;
						VoiceDataTransferMgr.BitPoint = 0x00;

						DEBUG_MARK;
					break;	


				case WaitForVoicePlayerIdle:
						VoiceDataTransferMgr.Status = VoiceIdle;
						VoiceDataTransferMgr.BusyPinStatusFilter = 0x00;
						VoiceDataTransferMgr.PulseWidthCnt = 1;
					break;

				default:
						VoiceDataTransferMgr.Status = VoiceIdle;
						DEBUG_MARK;
					break;
					
			}
		}
  #endif
  
  return;
  /* USER CODE END TIM3_IRQn 1 */
}
void TIM6_ISR(void)
{
		DEBUG_MARK;
	
		//TOGGLE_DEBUG_LED;
//300us interval
		UART1_TX_Task();


		UART2_TX_Task();

		if ( FrmMgr.FrmFunctionConfig == FRM_Enabled )
		{
			UART3_TX_Task();
		}
		
	return;
}



void UART1_ISR(void)
{
	if (	UART1_Mgr.Status == GotNewCmd )
	{
		//do nothing
	}
	else if ( (UART1_Mgr.Status == Idle )&&(UART1_RXBUFFER[0]==0xEF) )
	{
		UART1_Mgr.RX_Buffer[UART1_Mgr.RX_DataPoint] = UART1_RXBUFFER[0];
		UART1_Mgr.RX_DataPoint++;
		UART1_Mgr.Status = ReceivingData;
	}
	else if (UART1_Mgr.Status == ReceivingData)
	{
		UART1_Mgr.RX_Buffer[UART1_Mgr.RX_DataPoint] = UART1_RXBUFFER[0];
		UART1_Mgr.RX_DataPoint++;
		if ((UART1_Mgr.RX_Buffer[0]==0xEF)&&( UART1_Mgr.RX_DataPoint == (UART1_Mgr.RX_Buffer[8]+9))&&( UART1_Mgr.RX_DataPoint >9 ))
		{
			UART1_Mgr.Status = GotNewCmd;
		}
		else if ( UART1_Mgr.RX_DataPoint > 98)
		{
			UART1_Mgr.RX_DataPoint = 0x00;
			UART1_Mgr.Status = Idle;
		}
	}

	
	if ( HAL_UART_Receive_IT(&huart1,UART1_RXBUFFER,1) != HAL_OK )
	{
		HAL_UART_DeInit(&huart1);
		MX_USART1_UART_Init();
	}
			
	return;
}

void UART2_ISR(void)
{
	
	if (	UART2_Mgr.Status == GotNewCmd )
	{
		DEBUG_MARK;	//do nothing
	}
	else if ( (UART2_Mgr.Status == Idle )&&(UART2_RXBUFFER[0]==0xCA) )
	{
		UART2_Mgr.RX_Buffer[UART2_Mgr.RX_DataPoint] = UART2_RXBUFFER[0];
		UART2_Mgr.RX_DataPoint++;
		UART2_Mgr.DataStreamTimeCnt = DEF_WifiFlowChartTimeOutDelay;
		UART2_Mgr.Status = ReceivingData;
	}
	else if (UART2_Mgr.Status == ReceivingData)
	{
		UART2_Mgr.RX_Buffer[UART2_Mgr.RX_DataPoint] = UART2_RXBUFFER[0];
		UART2_Mgr.RX_DataPoint++;
		if ( UART2_Mgr.RX_DataPoint >= 12  )
		{
			UART2_Mgr.Status = GotNewCmd;
			
			if ( ComportMgr.Flag_RxBuff1IsFull == bFALSE )
			{
				memcpy(ComportMgr.RxBuff1, UART2_Mgr.RX_Buffer,12);
				ComportMgr.Flag_RxBuff1IsFull=bTRUE;
				UART2_Mgr.RX_DataPoint = 0x00;
				UART2_Mgr.Status = Idle;
			}
			else if ( ComportMgr.Flag_RxBuff2IsFull == bFALSE )
			{
				memcpy(ComportMgr.RxBuff2, UART2_Mgr.RX_Buffer,12);
				ComportMgr.Flag_RxBuff2IsFull=bTRUE;
				UART2_Mgr.RX_DataPoint = 0x00;
				UART2_Mgr.Status = Idle;
			}
			
		}
		else if ( UART2_Mgr.RX_DataPoint > 13)
		{
			UART2_Mgr.RX_DataPoint = 0x00;
			UART2_Mgr.Status = Idle;
		}
	}
	else{

	}
	
	if ( HAL_UART_Receive_IT(&huart2,UART2_RXBUFFER,1) != HAL_OK )
	{
		HAL_UART_DeInit(&huart2);
		MX_USART2_UART_Init();
	}
			
	return;
}

#ifdef Function_FaceRecoginition
void UART3_ISR(void)
{	

	if (UART3_Mgr.Status == GotNewCmd )
	{
		DEBUG_MARK;	//do nothing
	}
	else if ( (UART3_Mgr.Status == Idle )&&(UART3_RXBUFFER[0]==0xEF) )
	{
		UART3_Mgr.RX_DataPoint=0x00;
		UART3_Mgr.RX_Buffer[UART3_Mgr.RX_DataPoint] = UART3_RXBUFFER[0];
		UART3_Mgr.RX_DataPoint++;
		UART3_Mgr.DataStreamTimeCnt = DEF_FRMFlowChartTimeOutDelay;
		UART3_Mgr.Status = ReceivingData;
	}
	
	else if (UART3_Mgr.Status == ReceivingData)
	{
		UART3_Mgr.RX_Buffer[UART3_Mgr.RX_DataPoint] = UART3_RXBUFFER[0];

		if ((UART3_Mgr.RX_DataPoint == 0x01)&&(UART3_Mgr.RX_Buffer[UART3_Mgr.RX_DataPoint]!=0xAA))
		{
			UART3_Mgr.RX_DataPoint = 0x00;
			UART3_Mgr.Status = Idle;
		}
		else if (( UART3_Mgr.RX_DataPoint>4 )&&( UART3_Mgr.RX_DataPoint >= (UART3_Mgr.RX_Buffer[3]*256+UART3_Mgr.RX_Buffer[4]+5)))
		{
			UART3_Mgr.Status = GotNewCmd;
		}
		else if ( UART3_Mgr.RX_DataPoint > 210)
		{
			UART3_Mgr.RX_DataPoint = 0x00;
			UART3_Mgr.Status = Idle;
		}
		
		UART3_Mgr.RX_DataPoint++;
	}
	else{

	}

	if ( HAL_UART_Receive_IT(&huart3,UART3_RXBUFFER,1) != HAL_OK )
	{
		HAL_UART_DeInit(&huart3);
		MX_USART3_UART_Init();
	}
	
	return;
}

#elif defined Function_YouzhiyunjiaWifi
void UART3_ISR(void)
{	

	if (UART3_Mgr.Status == GotNewCmd )
	{
		DEBUG_MARK;	//do nothing
	}
	else if ( (UART3_Mgr.Status == Idle )&&(UART3_RXBUFFER[0]==0xAA) )
	{
		UART3_Mgr.RX_DataPoint=0x00;
		UART3_Mgr.RX_Buffer[UART3_Mgr.RX_DataPoint] = UART3_RXBUFFER[0];
		UART3_Mgr.RX_DataPoint++;
		UART3_Mgr.DataStreamTimeCnt = DEF_WifiFlowChartTimeOutDelay;
		UART3_Mgr.Status = ReceivingData;
	}
	
	else if (UART3_Mgr.Status == ReceivingData)
	{
		UART3_Mgr.RX_Buffer[UART3_Mgr.RX_DataPoint] = UART3_RXBUFFER[0];
		UART3_Mgr.RX_DataPoint++;

		if ( UART3_Mgr.RX_DataPoint >= WifiMgr.Ack.AckLenth )
		{
			UART3_Mgr.Status = GotNewCmd;
		}
		else if ( UART3_Mgr.RX_DataPoint > 99)
		{
			UART3_Mgr.RX_DataPoint = 0x00;
			UART3_Mgr.Status = Idle;
		}
	}
	else{

	}

	if ( HAL_UART_Receive_IT(&huart3,UART3_RXBUFFER,1) != HAL_OK )
	{
		HAL_UART_DeInit(&huart3);
		MX_USART3_UART_Init();
	}
	
	return;
}
#else
void UART3_ISR(void)
{	
	return;
}
#endif


