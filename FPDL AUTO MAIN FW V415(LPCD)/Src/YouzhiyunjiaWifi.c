#include "Project.h"
#include "IO.h"
#include "Hardware.h"
#include "ISR.h"
#include "GUI.h"
#include "StdTypes.h"
#include "usart.h"
#include "YouzhiyunjiaWifi.h"

#ifdef Function_YouzhiyunjiaWifi 

//#define OFFSET_SECOND     946684800  /* ??1970/1/1/0/0/0??2000/1/1/0/0/0??????????  */
#define SECOND_OF_DAY     86400      

static uint8_t YZYJ_OnlinePasscodeBuff[6];

static uint8_t DayOfMon[12] = {31,28,31,30,31,30,31,31,30,31,30,31};


void GetDateTimeFromSecond(unsigned long lSec)
{
    uint16_t i,j,iDay;
    uint32_t lDay;
 
    lDay = lSec / SECOND_OF_DAY;        /* ???????????? */
    lSec = lSec % SECOND_OF_DAY;
 
    i = 1970;
    while(lDay > 365)
    {
      if(((i%4==0)&&(i%100!=0)) || (i%400==0))    /* ???? */
        lDay -= 366;
      else
        lDay -= 365;
      i++;
    }
    if((lDay == 365) && !(((i%4==0)&&(i%100!=0)) || (i%400==0)))  /* ??? */
    {
      lDay -= 365;
      i++;
    }
    WifiMgr.time.year = HEXtoBCDcode(i%100);     /* ?õ???? */
	
    for(j=0;j<12;j++)     /* ?????·? */
    {
      if((j==1) && (((i%4==0)&&(i%100!=0)) || (i%400==0)))
        iDay = 29;
      else
        iDay = DayOfMon[j];
      if(lDay >= iDay) lDay -= iDay;
      else break;
    }
	
    WifiMgr.time.month  = HEXtoBCDcode(j+1);
    WifiMgr.time.date  = HEXtoBCDcode(lDay+1);
    //WifiMgr.time.hour = HEXtoBCDcode(((lSec / 3600)+8)%24);//这里注意，世界时间已经加上北京时间差8，
	WifiMgr.time.hour = HEXtoBCDcode(((lSec / 3600))%24);
    WifiMgr.time.minute  = HEXtoBCDcode((lSec % 3600) / 60);
    WifiMgr.time.second  = HEXtoBCDcode((lSec % 3600) % 60);


	SystemTime.second 	= 	WifiMgr.time.second;		
	SystemTime.minute	= 	WifiMgr.time.minute;	
	SystemTime.hour   	= 	WifiMgr.time.hour;	
	SystemTime.date   	= 	WifiMgr.time.date;
	SystemTime.month 	= 	WifiMgr.time.month;		
	SystemTime.year  	=	WifiMgr.time.year;

	WriteSystemTime();
}

void WifiTimeTranslate(uint8_t timebuff[])
{
	uint8_t buff[4];
	uint8_t i;
	uint8_t TimeZone;
	uint32_t TimeInSeconds;
	uint32_t TimeZoneOffsetSeconds;

	for (i=0;i<4;i++)
	{
		buff[i]=timebuff[i];
	}
	TimeZone = timebuff[4];
	
	TimeInSeconds = buff[3];
	TimeInSeconds<<=8;
	TimeInSeconds+=buff[2];
	TimeInSeconds<<=8;
	TimeInSeconds+=buff[1];
	TimeInSeconds<<=8;
	TimeInSeconds+=buff[0];
	
	
	if ( TimeZone < 0x80 )		//非负数
	{
		TimeZoneOffsetSeconds =  ( TimeZone/10)*60*60+(TimeZone%10)*15*60;
		TimeInSeconds+=TimeZoneOffsetSeconds;
	}
	else
	{
		TimeZone-=1;
		TimeZone=~TimeZone;
		TimeZoneOffsetSeconds =  ( TimeZone/10)*60*60+(TimeZone%10)*15*60;
		TimeInSeconds-=TimeZoneOffsetSeconds;
	}
	
	GetDateTimeFromSecond(TimeInSeconds);
	WifiMgr.time.TimeAvailable = bTRUE;
}

status_t UART3_Write_TxBUFFER(unsigned char *buff,unsigned char len)
{
	unsigned char i;
	if ( UART3_Mgr.TxLength !=0x00)		//if TX Buffer is not empty
	{
		return S_FAIL;
	}
	
	for ( i=0;i<len;i++)
	{
			UART3_Mgr.TxBUFFER[i] = *buff;
			buff++;
	}
	UART3_Mgr.TxLength = len;
	UART3_Mgr.TxPoint = 0;
	
	return S_SUCCESS;
}

void UART3_TX_Task(void)
{
		if ( UART3_Mgr.TxPoint < UART3_Mgr.TxLength )
		{
			if ( HAL_UART_Transmit(&huart3,&UART3_Mgr.TxBUFFER[UART3_Mgr.TxPoint],1,10) == HAL_OK )
			{
				UART3_Mgr.TxPoint++;
				DEBUG_MARK;
			}
			else
			{
				HAL_UART_DeInit(&huart3);
				MX_USART3_UART_Init();
			}
		}
		else if ( (UART3_Mgr.TxPoint==UART3_Mgr.TxLength)&&(UART3_Mgr.TxLength > 0) ) //TX Finished
		{
			UART3_Mgr.TxPoint = 0;
			UART3_Mgr.TxLength = 0;	
		}
}

void UART3_CLEAR_TXBUFF(void)
{
	UART3_Mgr.TxPoint = 0;
	UART3_Mgr.TxLength = 0;
	UART3_Mgr.Status = Idle;
}


void Wificmd_Init(void)
{
	UART3_Mgr.TxPoint = 0;
	UART3_Mgr.TxLength = 0;
	
	
}
void Wificmd_Excute(void)
{
	uint16_t i;
	uint8_t RxBuff[100];
	uint8_t CmdLenth,CKS;


	if ( WifiMgr.Ack.TimeOutCnt > 1 )
	{
		 WifiMgr.Ack.TimeOutCnt--;
	}
	else if ( WifiMgr.Ack.TimeOutCnt == 1 )
	{
		DEBUG_MARK;
		
		WifiMgr.Ack.TimeOutCnt = 0x0000;
		
		if (WifiMgr.Ack.Status == CountDownForSendOutRemoteUnlockSuccessCmd)
		{
			WifiMgr.Ack.Status = AckIdle;
			WifiMgr.RemoteUnlockMgr.Status = RemoteUnlockSuccess;
		}
		else if (WifiMgr.Ack.Status == WaitHostSpotCmdACK)
		{
			WifiMgr.Link.Status = LinkFail;
			WifiMgr.Ack.Status = AckIdle;
		}
		else if (WifiMgr.Ack.Status == WaitCheckStatusCmdACK)
		{	
			if (WifiMgr.PostMgr.PostFlag_CheckStatus == 0 )
			{
				WifiMgr.WifiConnection.Status = NotConnected;
				if ( WifiMgr.RemoteUnlockMgr.Status == RemoteUnlockRequestWait )
				{
					WifiMgr.RemoteUnlockMgr.Status = RemoteUnlockFail;
					WifiMgr.RemoteUnlockMgr.errorcode = 0x02;
				}
			}
			WifiMgr.Ack.Status = AckIdle;
		}
		else if ( WifiMgr.Ack.Status == WaitManufactureTestCmdACK )
		{
			WifiMgr.Ack.Status = AckIdle;
			WifiMgr.MFT.Status = MFTFail;
		}
		else if ( WifiMgr.Ack.Status == WaitOnlinePasswordVerifyCmdACK )
		{
			WifiMgr.Ack.Status = AckIdle;
			WifiMgr.WifiOnlinePasswordVerifyMgr.Status = OnlinePasswordVerifyFail;
		}
		else
		{
			WifiMgr.Ack.Status = AckIdle;
		}
	}


	if ( UART3_Mgr.Status == ReceivingData )
	{
		if ( UART3_Mgr.DataStreamTimeCnt > 0 )
		{
			UART3_Mgr.DataStreamTimeCnt--;
		}
		else	//reset UART2 receiving status
		{
			UART3_Mgr.RX_DataPoint = 0x00;
			UART3_Mgr.Status = Idle;
			UART3_RXBUFFER[0]=0x00;
			for ( i=0;i<100;i++ )
			{
				UART3_Mgr.RX_Buffer[i]=0x00;
			}
		}
	}
	
	if ( UART3_Mgr.Status != GotNewCmd ){
		return;
	}

	CmdLenth = WifiMgr.Ack.AckLenth;
	for (i=0;i<CmdLenth;i++)
	{
		RxBuff[i] = UART3_Mgr.RX_Buffer[i];
	}
	
	UART3_Mgr.RX_DataPoint = 0x00;
	UART3_Mgr.Status = Idle;	
	
	CKS = 0;
	for (i=0;i<(CmdLenth-1);i++)
	{
		CKS+= RxBuff[i];
	}
		
	DEBUG_MARK;
	
	if ( CKS!=RxBuff[CmdLenth-1] )
	{
		DEBUG_MARK;

		return;		//if check sum is failed, ignore this data strin
	}


	if ( RxBuff[2] == 0x13 )
	{
		WifiMgr.Ack.TimeOutCnt = 0x0000;		//Prevent unexpect ACK timeout
		WifiMgr.Ack.Status = AckIdle;
		WifiMgr.PostMgr.RemoteUnlockUserID = RxBuff[5]*256+RxBuff[4];
		GUI_WifiDrivingRemoteUnlockSuccess();
		Wifi_PostRemoteUnlockSuccessAck();
		
		DEBUG_MARK;
	}
	DEBUG_MARK;

	if ( WifiMgr.Ack.Status == WaitGeneralCmdACK )
	{
		
		if ( RxBuff[2] == 0x02 )
		{
			WifiMgr.Ack.TimeOutCnt = 0x0000;		//Prevent unexpect ACK timeout
			WifiMgr.Ack.Status = AckIdle;
			Wifi_PostPowerDown();
			DEBUG_MARK;
		}
	}
	else if  ( WifiMgr.Ack.Status == WaitHostSpotCmdACK )
	{
		if ( RxBuff[2] == 0x02 )
		{
			WifiMgr.Ack.TimeOutCnt = 0x0000;		//Prevent unexpect ACK timeout
			WifiMgr.Ack.Status = AckIdle;
			Wifi_PostPowerDown();
			DEBUG_MARK;
			
			if ( RxBuff[3] == 0x00 )
			{
				WifiMgr.Link.Status = LinkSuccess;
			}
			else
			{
				WifiMgr.Link.Status = LinkFail;
			}
		}
	}
	else if ( WifiMgr.Ack.Status == WaitRemoteUnlockRequestCmdACK)
	{
		if ( RxBuff[2] == 0x02 )
		{
			WifiMgr.Ack.Status = WaitRemoteUnlockSuccessCmdACK;
			WifiMgr.Ack.AckLenth = 10 ;
			WifiMgr.Ack.TimeOutCnt = 60*64;		//60s
			DEBUG_MARK;
		}
		DEBUG_MARK;
	}
	else if ( WifiMgr.Ack.Status == WaitRemoteUnlockSuccessCmdACK)
	{
		if ( RxBuff[2] == 0x13 )
		{
			WifiMgr.Ack.TimeOutCnt = 0x0000;		//Prevent unexpect ACK timeout
			WifiMgr.Ack.Status = AckIdle;
			WifiMgr.PostMgr.RemoteUnlockUserID = RxBuff[5]*256+RxBuff[4];
			Wifi_PostRemoteUnlockSuccessAck();
			DEBUG_MARK;
		}
		DEBUG_MARK;
	}
	else if ( WifiMgr.Ack.Status == WaitCheckStatusCmdACK )
	{
		if ( RxBuff[2] == 0x11 )
		{
			WifiMgr.Ack.TimeOutCnt = 0x0000;		//Prevent unexpect ACK timeout	
			WifiMgr.PostMgr.PostFlag_CheckStatus = 0;	
			WifiMgr.WifiConnection.Status = Connected;
			WifiMgr.Ack.Status = AckIdle;
			//Wifi_PostPowerDown();
			DEBUG_MARK;
		}	
	}
	else if ( WifiMgr.Ack.Status == WaitUtcCmdACK )
	{
		if ( RxBuff[2] == 0x11 )
		{
			WifiMgr.Ack.TimeOutCnt = 0x0000;		//Prevent unexpect ACK timeout
			if (( RxBuff[3]==0x00)
						&&(RxBuff[4]==0x00)
						&&(RxBuff[5]==0x00)
						&&(RxBuff[6]==0x00 ))
			{			
				WifiMgr.WifiNetWorkMgr.Status = NetWorkNG;
				DEBUG_MARK;
			}
			else
			{
				WifiTimeTranslate(&RxBuff[3]);
				WifiMgr.WifiNetWorkMgr.Status = NetWorkGood;	
			}
			
			WifiMgr.PostMgr.PostFlag_GetUTC = 0;	
			
			WifiMgr.WifiConnection.Status = Connected;
			WifiMgr.Ack.Status = AckIdle;
			Wifi_PostPowerDown();
			DEBUG_MARK;
		}
	}
	else if ( WifiMgr.Ack.Status == WaitManufactureTestCmdACK )
	{
		if ( RxBuff[2] == 0x0D )
		{
			WifiMgr.Ack.TimeOutCnt = 0x0000;		//Prevent unexpect ACK timeout
			if (RxBuff[3]==0x01)
			{
				WifiMgr.MFT.Status = MFTSuccess;
			}
			else
			{
				WifiMgr.MFT.Status = MFTFail;
			}
			WifiMgr.Ack.Status = AckIdle;
			Wifi_PostPowerDown();
			DEBUG_MARK;
		}
	}
	else if ( WifiMgr.Ack.Status == WaitPowerDownCmdACK )
	{
		WifiMgr.Ack.TimeOutCnt = 0x0000;		//Prevent unexpect ACK timeout
		WifiMgr.Ack.Status = AckIdle;
		WifiMgr.Power.Status = PowerShoulbeOff;
		DEBUG_MARK;
	}
	else if ( WifiMgr.Ack.Status == WaitCaptureImageCmdACK )
	{
		if ( RxBuff[2] == 0x1D )
		{
			WifiMgr.Ack.TimeOutCnt = 0x0000;		//Prevent unexpect ACK timeout
			WifiMgr.Ack.Status = AckIdle;
			DEBUG_MARK;
		}
	}
	else if ( WifiMgr.Ack.Status == WaitOnlinePasswordVerifyCmdACK )
	{
		if ( RxBuff[2] == 0x21 )
		{
			WifiMgr.Ack.TimeOutCnt = 0x0000;		//Prevent unexpect ACK timeout
			if (RxBuff[4]==0x00)
			{
				WifiMgr.WifiOnlinePasswordVerifyMgr.Status = OnlinePasswordVerifySuccess;
				WifiMgr.WifiOnlinePasswordVerifyMgr.OnlinePasswordUserPriority = RxBuff[5];
				WifiMgr.WifiOnlinePasswordVerifyMgr.OnlinePasswordUserType = RxBuff[6];
				WifiMgr.WifiOnlinePasswordVerifyMgr.UserID = RxBuff[7];
			}
			else
			{
				WifiMgr.WifiOnlinePasswordVerifyMgr.Status = OnlinePasswordVerifyFail;
			}
			DEBUG_MARK;
			WifiMgr.Ack.Status = AckIdle;		
			Wifi_PostPowerDown();
		}
	}
	else if ( WifiMgr.Ack.Status == WaitVideoConfigCmdACK )
	{
		if ( RxBuff[2] == 0x3D )
		{
			WifiMgr.Ack.TimeOutCnt = 0x0000;		//Prevent unexpect ACK timeout
			WifiMgr.Ack.Status = AckIdle;
			Wifi_PostPowerDown();
			DEBUG_MARK;
		}
	}
	else		//just protection
	{
		WifiMgr.Ack.TimeOutCnt = 0x0000;		//Prevent unexpect ACK timeout
		WifiMgr.Ack.Status = AckIdle;
		WifiMgr.Power.Status = PowerShoulbeOff;
		DEBUG_MARK;
	}

	DEBUG_MARK;
}

void Wifi_SendGetHotspotCmd(void)
{
	uint8_t CKS,i,CmdLenth;
	uint8_t buff[10]={0xAA,0x55,0x01,0x16,0x09,0x02,0x01,0x00,0x00,0xFF};
	CmdLenth = 10;
	CKS=0x00;
	for ( i=0;i<(CmdLenth-1);i++ )
	{
		CKS = CKS+buff[i];	
	}
	buff[CmdLenth-1] = CKS;
	
	UART3_Write_TxBUFFER(&buff[0],CmdLenth);

	WifiMgr.Ack.AckLenth = 10 ;
	WifiMgr.Ack.TimeOutCnt = 180*64;		//180s
	WifiMgr.Ack.Status = WaitHostSpotCmdACK;
	UART3_Mgr.RX_DataPoint = 0x00;
	UART3_Mgr.Status = Idle;

	//HAL_UART_Receive_IT(&huart2,&UART3_Mgr.RX_Buffer[0],WifiMgr.Ack.AckLenth);

}
void Wifi_SendlockStatusCmd(void )
{ 
	uint8_t CKS,i,CmdLenth;
	uint8_t buff[10]={0xAA,0x55,  0x03,0x00,  0x00,  0x02,0x01,0x00, 0x00,0xFF};
	CmdLenth = 10;
	if ( WifiMgr.PostMgr.UserType == FPUSER ){
		buff[5]=1;
	}else if ( WifiMgr.PostMgr.UserType == PASSCODEUSER ){
		buff[5]=2;
	}else if ( WifiMgr.PostMgr.UserType == CARDUSER ){
		buff[5]=3;
	}
	
	buff[6] = WifiMgr.PostMgr.UserID%256;
	buff[7] = WifiMgr.PostMgr.UserID/256;

	
	CKS=0x00;
	for ( i=0;i<(CmdLenth-1);i++ )
	{
		CKS = CKS+buff[i];	
	}
	buff[CmdLenth-1] = CKS;
	
	UART3_Write_TxBUFFER(&buff[0],CmdLenth);
	
	WifiMgr.Ack.AckLenth = 10 ;
	WifiMgr.Ack.TimeOutCnt = 12*64;		//12s
	WifiMgr.Ack.Status = WaitGeneralCmdACK;

	UART3_Mgr.RX_DataPoint = 0x00;
	UART3_Mgr.Status = Idle;
}

void Wifi_SendUnlockStatusCmd(void)
{ 
	uint8_t CKS,i,CmdLenth;
	uint8_t buff[10]={0xAA,0x55,  0x03,0x01,  0x00,  0x02,0x01,0x00, 0x00,0xFF};
	CmdLenth = 10;
	
	buff[5]=WifiMgr.PostMgr.UserType;
	buff[6] = WifiMgr.PostMgr.UserID%256;
	buff[7] = WifiMgr.PostMgr.UserID/256;

	CKS=0x00;
	for ( i=0;i<(CmdLenth-1);i++ )
	{
		CKS = CKS+buff[i];	
	}
	buff[CmdLenth-1] = CKS;
	
	UART3_Write_TxBUFFER(&buff[0],CmdLenth);
	
	WifiMgr.Ack.AckLenth = 10 ;
	WifiMgr.Ack.TimeOutCnt = 20*64;		//12s
	WifiMgr.Ack.Status = WaitGeneralCmdACK;

	UART3_Mgr.RX_DataPoint = 0x00;
	UART3_Mgr.Status = Idle;



}



void Wifi_SendRemoteUnlockRequestCmd(void )
{ 
	uint8_t CKS,i,CmdLenth;
	uint8_t buff[10]={0xAA,0x55,  0x03,0x03,  0x00,  0x00,0x00,0x00, 0x00,0xFF};
	CmdLenth = 10;
	
	CKS=0x00;
	for ( i=0;i<(CmdLenth-1);i++ )
	{
		CKS = CKS+buff[i];	
	}
	buff[CmdLenth-1] = CKS;
	
	UART3_Write_TxBUFFER(&buff[0],CmdLenth);
	
	WifiMgr.Ack.AckLenth = 10 ;
	WifiMgr.Ack.TimeOutCnt = 2*64;		//60s
	WifiMgr.Ack.Status = WaitRemoteUnlockRequestCmdACK;

	UART3_Mgr.RX_DataPoint = 0x00;
	UART3_Mgr.Status = Idle;

}

void Wifi_SendRemoteUnlockSuccessAck(void )
{ 
	uint8_t CKS,i,CmdLenth;
	uint8_t buff[10]={0xAA,0x55,0x12,0x00,  0x00,  0x00,0x00,0x00, 0x00,0xFF};
	CmdLenth = 10;
	
	CKS=0x00;
	for ( i=0;i<(CmdLenth-1);i++ )
	{
		CKS = CKS+buff[i];	
	}
	buff[CmdLenth-1] = CKS;
	
	UART3_Write_TxBUFFER(&buff[0],CmdLenth);
	
	WifiMgr.Ack.AckLenth = 10 ;
	WifiMgr.Ack.TimeOutCnt = 16;		//0.25s
	WifiMgr.Ack.Status = CountDownForSendOutRemoteUnlockSuccessCmd;

	UART3_Mgr.RX_DataPoint = 0x00;
	UART3_Mgr.Status = Idle;

}


void Wifi_SendInfoCmd(void)
{ 
	uint8_t CKS,i,CmdLenth;
	uint8_t buff[10]={0xAA,0x55,  0x03,0x04,  0x00,  0x02,0x01,0x00, 0x00,0xFF};
	CmdLenth = 10;
	
	buff[4]=WifiMgr.PostMgr.InfoType;
	buff[5]=WifiMgr.PostMgr.UserType;
	
	buff[6] = WifiMgr.PostMgr.UserID%256;
	buff[7] = WifiMgr.PostMgr.UserID/256;

	CKS=0x00;
	for ( i=0;i<(CmdLenth-1);i++ )
	{
		CKS = CKS+buff[i];	
	}
	buff[CmdLenth-1] = CKS;
	
	UART3_Write_TxBUFFER(&buff[0],CmdLenth);
	
	WifiMgr.Ack.AckLenth = 10 ;
	WifiMgr.Ack.TimeOutCnt = 20*64; 	//12s
	WifiMgr.Ack.Status = WaitGeneralCmdACK;

	UART3_Mgr.RX_DataPoint = 0x00;
	UART3_Mgr.Status = Idle;

}

void Wifi_SendAlarmCmd(void)
{ 
	uint8_t CKS,i,CmdLenth;
	uint8_t buff[10]={0xAA,0x55,0x03,0x02,0x00,0x00,0x00,0x00,0x00,0xFF};
	CmdLenth = 10;

	buff[4]=WifiMgr.PostMgr.AlarmType;
	
	buff[5]=WifiMgr.PostMgr.UserType;
	
	buff[6] = WifiMgr.PostMgr.UserID%256;
	buff[7] = WifiMgr.PostMgr.UserID/256;
	
	CKS=0x00;
	for ( i=0;i<(CmdLenth-1);i++ )
	{
		CKS = CKS+buff[i];	
	}
	buff[CmdLenth-1] = CKS;
	
	UART3_Write_TxBUFFER(&buff[0],CmdLenth);
	
	WifiMgr.Ack.AckLenth = 10 ;
	WifiMgr.Ack.TimeOutCnt = 20*64; 	//12s
	WifiMgr.Ack.Status = WaitGeneralCmdACK;

	UART3_Mgr.RX_DataPoint = 0x00;
	UART3_Mgr.Status = Idle;

}

void Wifi_SendCheckStatusCmd(void)
{ 
	uint8_t CKS,i,CmdLenth;
	uint8_t buff[10]={0xAA,0x55, 0x10, 0x00,0x00,0x00,0x00, 0x00, 0x00,0xFF};
	CmdLenth = 10;
	
	CKS=0x00;
	for ( i=0;i<(CmdLenth-1);i++ )
	{
		CKS = CKS+buff[i];	
	}
	buff[CmdLenth-1] = CKS;
	
	UART3_Write_TxBUFFER(&buff[0],CmdLenth);
	
	WifiMgr.Ack.AckLenth = 10 ;
	WifiMgr.Ack.TimeOutCnt = 1*64; 	//1s
	WifiMgr.Ack.Status = WaitCheckStatusCmdACK;

	UART3_Mgr.RX_DataPoint = 0x00;
	UART3_Mgr.Status = Idle;

}

void Wifi_SendGetUTCCmd(void)
{ 
	uint8_t CKS,i,CmdLenth;
	uint8_t buff[10]={0xAA,0x55, 0x10, 0x00,0x00,0x00,0x00, 0x00, 0x00,0xFF};
	CmdLenth = 10;
	
	CKS=0x00;
	for ( i=0;i<(CmdLenth-1);i++ )
	{
		CKS = CKS+buff[i];	
	}
	buff[CmdLenth-1] = CKS;
	
	UART3_Write_TxBUFFER(&buff[0],CmdLenth);
	
	WifiMgr.Ack.AckLenth = 10 ;
	WifiMgr.Ack.TimeOutCnt = 4*64; 	//2s
	WifiMgr.Ack.Status = WaitUtcCmdACK;

	UART3_Mgr.RX_DataPoint = 0x00;
	UART3_Mgr.Status = Idle;

}
void Wifi_SendClearDataCmd(void )
{ 
	uint8_t CKS,i,CmdLenth;
	uint8_t buff[10]={0xAA,0x55,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0xFF};
	CmdLenth = 10;
	
	CKS=0x00;
	for ( i=0;i<(CmdLenth-1);i++ )
	{
		CKS = CKS+buff[i];	
	}
	buff[CmdLenth-1] = CKS;
	
	UART3_Write_TxBUFFER(&buff[0],CmdLenth);
	
	WifiMgr.Ack.AckLenth = 10 ;
	WifiMgr.Ack.TimeOutCnt = 20*64;		//10s
	WifiMgr.Ack.Status = WaitGeneralCmdACK;

	UART3_Mgr.RX_DataPoint = 0x00;
	UART3_Mgr.Status = Idle;

}
void Wifi_SendManufactureTestCmd(void)
{ 
	uint8_t CKS,i,CmdLenth;
	uint8_t buff[10]={0xAA,0x55,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0xFF};
	CmdLenth = 10;
	
	CKS=0x00;
	for ( i=0;i<(CmdLenth-1);i++ )
	{
		CKS = CKS+buff[i];	
	}
	buff[CmdLenth-1] = CKS;
	
	UART3_Write_TxBUFFER(&buff[0],CmdLenth);
	
	WifiMgr.Ack.AckLenth = 10 ;
	WifiMgr.Ack.TimeOutCnt = 20*64;		//2s
	WifiMgr.Ack.Status = WaitManufactureTestCmdACK;

	UART3_Mgr.RX_DataPoint = 0x00;
	UART3_Mgr.Status = Idle;

}

void Wifi_SendCaptureImageCmd(void)
{ 
	uint8_t CKS,i,CmdLenth;
	uint8_t buff[10]={0xAA,0x55,0x1C,0x00,0x00,0x00,0x00,0x00,0x00,0xFF};
	CmdLenth = 10;
	
	CKS=0x00;
	for ( i=0;i<(CmdLenth-1);i++ )
	{
		CKS = CKS+buff[i];	
	}
	buff[CmdLenth-1] = CKS;
	
	UART3_Write_TxBUFFER(&buff[0],CmdLenth);
	
	WifiMgr.Ack.AckLenth = 10 ;
	WifiMgr.Ack.TimeOutCnt = 20*64;		//12s
	WifiMgr.Ack.Status = WaitCaptureImageCmdACK;

	UART3_Mgr.RX_DataPoint = 0x00;
	UART3_Mgr.Status = Idle;

}


void Wifi_SendVideoConfigCmd(void)
{ 
	uint8_t CKS,i,CmdLenth;
	uint8_t buff[10]={0xAA,0x55,0x3C,0x00,0x00,0x00,0x00,0x00,0x00,0xFF};
	CmdLenth = 10;


	if ( SystemLanguage == Chinese ){
		buff[3] = 0x01;//Chinese
	}
	else{
		buff[3] = 0x02;//English
	}

	buff[4] = 24;	//speeker volume ,0~31

	buff[5] = 0x02;	// 0x00,image does not rote，0x02 Image rote 180 degree
	
	CKS=0x00;
	for ( i=0;i<(CmdLenth-1);i++ )
	{
		CKS = CKS+buff[i];	
	}
	buff[CmdLenth-1] = CKS;
	
	UART3_Write_TxBUFFER(&buff[0],CmdLenth);
	
	WifiMgr.Ack.AckLenth = 10 ;
	WifiMgr.Ack.TimeOutCnt = 20*64;		//12s
	WifiMgr.Ack.Status = WaitVideoConfigCmdACK;

	UART3_Mgr.RX_DataPoint = 0x00;
	UART3_Mgr.Status = Idle;

}


void Wifi_SendOnlinePasscodeVerifyCmd(uint8_t OnlinePasscodeBuff[])
{
	uint32_t UTCtime;
	uint8_t CKS,i,j,CmdLenth;
	uint8_t buff[14]={0xAA,0x55,0x20,0x09,0x11,0x22,0x33,0x44,0x50,0x02,0x12,0x34,0x56,0xFF};
	CmdLenth = 14;

	
	UTCtime = mktime(2000+BCDcodeToHEX(SystemTime.year), BCDcodeToHEX(SystemTime.month),
			BCDcodeToHEX(SystemTime.date), BCDcodeToHEX(SystemTime.hour),
			BCDcodeToHEX(SystemTime.minute), BCDcodeToHEX(SystemTime.second));

	buff[7]=UTCtime;
	UTCtime>>=8;
	buff[6]=UTCtime;
	UTCtime>>=8;
	buff[5]=UTCtime;
	UTCtime>>=8;
	buff[4]=UTCtime;

	buff[8]=0X50;	//Time Zone
	buff[9]=0X02;	//feature password
	
	buff[10] = HEXtoBCDcode(OnlinePasscodeBuff[0]*10+OnlinePasscodeBuff[1]);
	buff[11] = HEXtoBCDcode(OnlinePasscodeBuff[2]*10+OnlinePasscodeBuff[3]);
	buff[12] = HEXtoBCDcode(OnlinePasscodeBuff[4]*10+OnlinePasscodeBuff[5]);
	
	CKS=0x00;
	for ( i=0;i<(CmdLenth-1);i++ )
	{
		CKS = CKS+buff[i];	
	}
	buff[CmdLenth-1] = CKS;
	
	UART3_Write_TxBUFFER(&buff[0],CmdLenth);

	WifiMgr.Ack.AckLenth = 10 ;
	WifiMgr.Ack.TimeOutCnt = 1*64; 	//5s
	WifiMgr.Ack.Status = WaitOnlinePasswordVerifyCmdACK;

	UART3_Mgr.RX_DataPoint = 0x00;
	UART3_Mgr.Status = Idle;

}


void Wifi_SendPowerDownCmd(void)
{ 
	uint8_t CKS,i,CmdLenth;
	uint8_t buff[10]={0xAA,0x55, 0x04, 0x00,0x00,0x00,0x00, 0x00, 0x00,0xFF};
	CmdLenth = 10;
	
	CKS=0x00;
	for ( i=0;i<(CmdLenth-1);i++ )
	{
		CKS = CKS+buff[i];	
	}
	buff[CmdLenth-1] = CKS;
	
	UART3_Write_TxBUFFER(&buff[0],CmdLenth);
	
	WifiMgr.Ack.AckLenth = 10 ;
	WifiMgr.Ack.TimeOutCnt = 5*64; 	//2s
	WifiMgr.Ack.Status = WaitPowerDownCmdACK;

	UART3_Mgr.RX_DataPoint = 0x00;
	UART3_Mgr.Status = Idle;

}

void Wifi_SendAwakeWifiCmd(void)
{ 
	uint8_t CKS,i,CmdLenth;
	uint8_t buff[10]={0x00,0x000, 0x00, 0x00,0x00,0x00,0x00, 0x00, 0x00,0x00};
	CmdLenth = 10;
	
	
	UART3_Write_TxBUFFER(&buff[0],CmdLenth);
	
	WifiMgr.Ack.AckLenth = 10 ;
	WifiMgr.Ack.TimeOutCnt = 1*64; 	//1s
	WifiMgr.Ack.Status = WaitGeneralCmdACK;

	UART3_Mgr.RX_DataPoint = 0x00;
	UART3_Mgr.Status = Idle;

}

void Wifi_PostCaptureImageCmd(void)
{
	#ifdef Function_WifiWithImageCapture
	WifiMgr.DoorBellTimeCnt = 96;
	WifiMgr.PostMgr.PostFlag_CaptureImage = 0x01;
	#endif
}

void Wifi_PostVideoConfigCmd(void)
{
	#ifdef Function_WifiWithVideoCapture
	WifiMgr.PostMgr.PostFlag_VideoConfig = 0x01;
	#endif
}

void Wifi_PostGetHotspotCmd(void)
{
	WifiMgr.PostMgr.PostFlag_GetHotspot = 0x01;
}
void Wifi_PostlockStatusCmd(UserType_t usertype, uint16_t userID )
{
	WifiMgr.PostMgr.PostFlag_lock = 0x01;
	WifiMgr.PostMgr.UserType = usertype;
	WifiMgr.PostMgr.UserID = userID;	
	Wifi_PostCaptureImageCmd();
}
void Wifi_PostUnlockStatusCmd(UserType_t usertype, uint16_t userID )
{
	if ( usertype == FPUSER )
	{
		WifiMgr.PostMgr.UserType = DEF_YouzhiyunUserType_FP;

		if ( userID > (DEF_MAX_FPMASTER+DEF_MAX_FPUSER) )
		{
			WifiMgr.PostMgr.AlarmType = DEF_YouzhiyunWifiAlarm_StressUnlock;
			WifiMgr.PostMgr.PostFlag_Alarm = 0x01;
			WifiMgr.PostMgr.UserID = userID;
			Wifi_PostCaptureImageCmd();
		}
		else
		{
			
			WifiMgr.PostMgr.PostFlag_Unlock = 0x01;
			WifiMgr.PostMgr.UserID = userID;
			Wifi_PostCaptureImageCmd();
		}
		
	}
	else if ( usertype == PASSCODEUSER )
	{
		WifiMgr.PostMgr.UserType = DEF_YouzhiyunUserType_PASSCODE;
		WifiMgr.PostMgr.PostFlag_Unlock = 0x01;
		WifiMgr.PostMgr.UserID = userID;
		Wifi_PostCaptureImageCmd();
	}
	else if ( usertype == CARDUSER )
	{
		WifiMgr.PostMgr.UserType = DEF_YouzhiyunUserType_CARD;
		WifiMgr.PostMgr.PostFlag_Unlock = 0x01;
		WifiMgr.PostMgr.UserID = userID;
		Wifi_PostCaptureImageCmd();
	}	
	else if ( usertype == REMOTEGUEST )
	{
		WifiMgr.PostMgr.UserType = DEF_YouzhiyunUserType_MOBILE;
		WifiMgr.PostMgr.PostFlag_Unlock = 0x01;
		WifiMgr.PostMgr.UserID = userID;
		Wifi_PostCaptureImageCmd();
	}
	else if ( usertype == ONLINEPASSCODEUSER )
	{
		if ( WifiMgr.WifiOnlinePasswordVerifyMgr.OnlinePasswordUserPriority == OnlinePasswordStressUser )
		{
			WifiMgr.PostMgr.AlarmType = DEF_YouzhiyunWifiAlarm_StressUnlock;
			WifiMgr.PostMgr.PostFlag_Alarm = 0x01;
			WifiMgr.PostMgr.UserType = DEF_YouzhiyunUserType_TEMPUSER;
			WifiMgr.PostMgr.UserID = userID;
			Wifi_PostCaptureImageCmd();
		}
		else
		{
			WifiMgr.PostMgr.UserType = DEF_YouzhiyunUserType_TEMPUSER;
			WifiMgr.PostMgr.PostFlag_Unlock = 0x01;
			WifiMgr.PostMgr.UserID = userID;
			Wifi_PostCaptureImageCmd();
		}
	}	
	else 
	{
		WifiMgr.PostMgr.UserType = DEF_YouzhiyunUserType_TEMPUSER;
		WifiMgr.PostMgr.PostFlag_Unlock = 0x01;
		WifiMgr.PostMgr.UserID = userID;
		Wifi_PostCaptureImageCmd();
	}
}
void Wifi_PostRemoteUnlockRequestCmd(void )
{
	WifiMgr.PostMgr.PostFlag_RemoteUnlockRequest = 0x01;
	Wifi_PostCaptureImageCmd();
}
void Wifi_PostRemoteUnlockSuccessAck(void )
{
	WifiMgr.PostMgr.PostFlag_RemoteUnlockSuccess = 0x01;
}
void Wifi_PostEvent(uint8_t EventType, UserType_t usertype, uint16_t userID)
{
	if (  (EventType == DEF_WifiEvent_LowBattery)
		||(EventType == DEF_WifiEvent_AddUser)
		||(EventType == DEF_WifiEvent_DeleteUser)
		||(EventType == DEF_WifiEvent_DoorBell)
	   )
	{
		if (EventType == DEF_WifiEvent_LowBattery)
		{
			WifiMgr.PostMgr.InfoType = DEF_YouzhiyunWifiInfo_LowBattery;
		}
		else if (EventType == DEF_WifiEvent_AddUser)
		{
			
			if ( usertype == FPUSER )
			{
				WifiMgr.PostMgr.UserType = DEF_YouzhiyunUserType_FP;
			}
			else if ( usertype == PASSCODEUSER )
			{
				WifiMgr.PostMgr.UserType = DEF_YouzhiyunUserType_PASSCODE;
			}
			else if ( usertype == CARDUSER )
			{
				WifiMgr.PostMgr.UserType = DEF_YouzhiyunUserType_CARD;
			}		
			WifiMgr.PostMgr.InfoType = DEF_YouzhiyunWifiInfo_AddUser;
			WifiMgr.PostMgr.UserID = userID;
		}
		else if (EventType == DEF_WifiEvent_DeleteUser)
		{
			if ( usertype == FPUSER )
			{
				WifiMgr.PostMgr.UserType = DEF_YouzhiyunUserType_FP;
			}
			else if ( usertype == PASSCODEUSER )
			{
				WifiMgr.PostMgr.UserType = DEF_YouzhiyunUserType_PASSCODE;
			}
			else if ( usertype == CARDUSER )
			{
				WifiMgr.PostMgr.UserType = DEF_YouzhiyunUserType_CARD;
			}	
			WifiMgr.PostMgr.InfoType = DEF_YouzhiyunWifiInfo_DeleteUser;
			WifiMgr.PostMgr.UserID = userID;
		}
		else if (EventType == DEF_WifiEvent_DoorBell)
		{
			WifiMgr.PostMgr.InfoType = DEF_YouzhiyunWifiInfo_DoorBell;
			Wifi_PostCaptureImageCmd();
		}
		WifiMgr.PostMgr.PostFlag_Info = 0x01;
		
	}
	else if (  (EventType == DEF_WifiEvent_FpUnlockAlarm)
				||(EventType == DEF_WifiEvent_PasswordUnlockAlarm)
				||(EventType == DEF_WifiEvent_CardUnlockAlarm)
				||(EventType == DEF_WifiEvent_AntiPryingAlarm)
			   )
	{
		if (EventType == DEF_WifiEvent_AntiPryingAlarm){
			WifiMgr.PostMgr.AlarmType =	DEF_YouzhiyunWifiAlarm_AntiPryingAlarm;
		}
		else if (EventType == DEF_WifiEvent_FpUnlockAlarm){
			WifiMgr.PostMgr.AlarmType = DEF_YouzhiyunWifiAlarm_FpUnlockAlarm;
		}
		else if (EventType == DEF_WifiEvent_PasswordUnlockAlarm){
			WifiMgr.PostMgr.AlarmType = DEF_YouzhiyunWifiAlarm_PasswordUnlockAlarm;
		}
		else if (EventType == DEF_WifiEvent_CardUnlockAlarm){
			WifiMgr.PostMgr.AlarmType = DEF_YouzhiyunWifiAlarm_CardUnlockAlarm;
		}
		WifiMgr.PostMgr.PostFlag_Alarm = 0x01;
		WifiMgr.PostMgr.UserType = usertype;
		WifiMgr.PostMgr.UserID = userID;
		Wifi_PostCaptureImageCmd();
	}

}


void Wifi_PostGetUTC(uint8_t Times)
{
	WifiMgr.PostMgr.PostFlag_GetUTC = Times;
}
void Wifi_PostCheckWifiStatus(uint8_t Times)
{
	WifiMgr.PostMgr.PostFlag_CheckStatus = Times;
}

void Wifi_PostOnlinePasswordVerify(uint8_t BUFF[])
{
	if ( WifiMgr.WifiConnection.Status == Connected )
	{
		memcpy(YZYJ_OnlinePasscodeBuff,BUFF,6);
		WifiMgr.PostMgr.PostFlag_OnlinePasswordVerify = 0x01;
	}
	else
	{
		WifiMgr.WifiOnlinePasswordVerifyMgr.Status = OnlinePasswordVerifyFail;
	}
}

void Wifi_PostPowerDown(void)
{
	//if ( WifiMgr.Power.PowerOnForOnlinePassword != bTRUE )
	{
		WifiMgr.PostMgr.PostFlag_PowerDown = 0x01;
	}
}

void Wifi_PostAwake(void)
{
	WifiMgr.PostMgr.PostFlag_AwakeWifi = 0x01;
}

void Wifi_PostClearData(void)
{
	WifiMgr.PostMgr.PostFlag_ClearData = 0x01;
}
void Wifi_PostManufactureTest(void)
{
	WifiMgr.PostMgr.PostFlag_ManufactureTest = 0x01;
}

void Wifi_ClearAllPostFlag(void)
{
	WifiMgr.PostMgr.PostFlag_Alarm = 0;
	WifiMgr.PostMgr.PostFlag_CaptureImage = 0;
	WifiMgr.PostMgr.PostFlag_CaptureVideo = 0;
	WifiMgr.PostMgr.PostFlag_ClearData = 0;
	WifiMgr.PostMgr.PostFlag_CheckStatus = 0;
	WifiMgr.PostMgr.PostFlag_GetUTC = 0;
	WifiMgr.PostMgr.PostFlag_Info = 0;
	WifiMgr.PostMgr.PostFlag_lock = 0;
	WifiMgr.PostMgr.PostFlag_ManufactureTest = 0;
	WifiMgr.PostMgr.PostFlag_PowerDown = 0;
	WifiMgr.PostMgr.PostFlag_RemoteUnlockRequest = 0;
	WifiMgr.PostMgr.PostFlag_RemoteUnlockSuccess = 0;
	WifiMgr.PostMgr.PostFlag_GetRemoteUnlockPasscode = 0;
	WifiMgr.PostMgr.PostFlag_SaveRemoteUnlockPasscode = 0;
	WifiMgr.PostMgr.PostFlag_Unlock = 0;
	WifiMgr.PostMgr.PostFlag_DoorBell = 0;
	WifiMgr.PostMgr.PostFlag_PowerLevel = 0;
	WifiMgr.PostMgr.PostFlag_ResetNetworkConfig = 0;
	WifiMgr.PostMgr.PostFlag_StressFP = 0;
	WifiMgr.PostMgr.PostFlag_OnlinePasswordVerify = 0;
	WifiMgr.Power.PowerOnForOnlinePassword = bFALSE;
}

void Wifi_Init(void)
{
	SET_DOORBELLAWAKE_L;
	
	UART3_Mgr.RX_DataPoint = 0x00;
	UART3_Mgr.Status = Idle;
	SET_WIFIPOWER_OFF;
	WifiMgr.Power.Status = PowerOff;
	WifiMgr.Ack.Status = AckIdle;
	
	WifiMgr.WifiConnection.Status = CheckingConnection;
	Wifi_PostCheckWifiStatus(3);
	
	WifiMgr.WifiNetWorkMgr.Status = NetWorkNG;
	WifiMgr.time.TimeAvailable = bFALSE;
	WifiMgr.RemoteUnlockMgr.Status = RemoteUnlockStandby;
	WifiMgr.Power.PowerOnForOnlinePassword = bFALSE;
	
	HAL_UART_DeInit(&huart3);
	
	Wifi_ClearAllPostFlag();

	Wifi_PostVideoConfigCmd();
}

void Wifi_PositiveClosing(void)
{
	Wifi_ClearAllPostFlag();
	WifiMgr.Power.PowerOnLimitedTimer = 0;
	HAL_UART_DeInit(&huart3);
	SET_WIFIPOWER_OFF;
	WifiMgr.Power.Status = PowerOff;	
	WifiMgr.Ack.Status = AckIdle;
	Wifi_ClearAllPostFlag();
}

void Wifi_PowerOffIfPowerOnForOnlinePasswordVerify(void)
{
	if ( WifiMgr.Power.PowerOnForOnlinePassword == bTRUE )
	{
		Wifi_PositiveClosing();
		WifiMgr.Power.PowerOnForOnlinePassword = bFALSE;
	}
}

void Wifi_PositiveExitRemoteUnlock(void)
{
	Wifi_ClearAllPostFlag();
	HAL_UART_DeInit(&huart3);
	SET_WIFIPOWER_OFF;
	WifiMgr.Power.Status = PowerOff;	
	WifiMgr.Ack.Status = AckIdle;
	WifiMgr.RemoteUnlockMgr.Status = RemoteUnlockStandby;
	WifiMgr.Power.PowerOnLimitedTimer = 0;
}


void Wifi_PowerMgr_Task(void)
{
	if 	( 	( WifiMgr.PostMgr.PostFlag_Alarm >0 )
		  ||( WifiMgr.PostMgr.PostFlag_CaptureImage >0 )
		  ||( WifiMgr.PostMgr.PostFlag_ClearData >0 )
		  ||( WifiMgr.PostMgr.PostFlag_GetHotspot >0 )
		  ||( WifiMgr.PostMgr.PostFlag_GetUTC >0 )
		  ||( WifiMgr.PostMgr.PostFlag_Info >0 )
		  ||( WifiMgr.PostMgr.PostFlag_lock >0 )
		  ||( WifiMgr.PostMgr.PostFlag_ManufactureTest >0 )
		  ||( WifiMgr.PostMgr.PostFlag_PowerDown >0 )
		  ||( WifiMgr.PostMgr.PostFlag_RemoteUnlockRequest >0 )
		  ||( WifiMgr.PostMgr.PostFlag_RemoteUnlockSuccess >0 )
		  ||( WifiMgr.PostMgr.PostFlag_Unlock >0 )
		  ||( WifiMgr.PostMgr.PostFlag_OnlinePasswordVerify >0 )
		  ||( WifiMgr.Power.PowerOnForOnlinePassword == bTRUE )
		)
	{
		if (WifiMgr.Power.Status == PowerOff )
		{
			WifiMgr.Power.Status = PowerOnDelay;

			MX_USART3_UART_Init();

			WifiMgr.Power.PowerOnDelayTime = DEF_WifiPowerDelayTime;
		}
	}
	else
	{
		if ( (WifiMgr.WifiConnection.Status != CheckingConnection)
			&&( WifiMgr.Ack.Status == AckIdle )
			&&( WifiMgr.Power.Status == PowerOn )
			&&( UART3_Mgr.TxLength == 0x00)
			)
		{
			WifiMgr.Power.Status = PowerShoulbeOff;
		}
	}
		

	if ( WifiMgr.Power.Status == PowerOn )
	{
		if ( WifiMgr.Power.PowerOnLimitedTimer > 1)
		{
			WifiMgr.Power.PowerOnLimitedTimer--;
		}
		else if ( WifiMgr.Power.PowerOnLimitedTimer == 1)
		{
		  WifiMgr.Power.PowerOnLimitedTimer = 0;
		  Wifi_ClearAllPostFlag();  
		  HAL_UART_DeInit(&huart3);
		  SET_WIFIPOWER_OFF;
		  WifiMgr.Power.Status = PowerOff;	  
		  WifiMgr.Ack.Status = AckIdle;
		}
	}
	
	else if ( WifiMgr.Power.Status == PowerOnDelay )
	{

		SET_WIFIPOWER_ON;
		
		if ( WifiMgr.Power.PowerOnDelayTime > 0 )
		{
			WifiMgr.Power.PowerOnDelayTime--;

			if ( WifiMgr.Power.PowerOnDelayTime == 32 )
			{
				
				
				//Wifi_SendAwakeWifiCmd();
			}
			else if ( WifiMgr.Power.PowerOnDelayTime == 1 )
			{
				//MX_USART3_UART_Init();
				
				WifiMgr.Power.PowerOnDelayTime=0;

				WifiMgr.WifiConnection.Status = CheckingConnection;
				
				Wifi_PostCheckWifiStatus(2); 
			}
		}
		else
		{
			WifiMgr.Power.Status = PowerOn;
			WifiMgr.Power.PowerOnLimitedTimer = 64*190;	//190 seconds
		}
	}
	
	else if ( WifiMgr.Power.Status == PowerShoulbeOff )
	{
		HAL_UART_DeInit(&huart3);
		SET_WIFIPOWER_OFF;
		WifiMgr.Power.Status = PowerOff;	
		WifiMgr.Ack.Status = AckIdle;
		WifiMgr.Power.PowerOnLimitedTimer = 0;
	}

}

void Wifi_DoorBellMgr_Task(void)
{
	if ( WifiMgr.DoorBellTimeCnt > 0 )
	{
		WifiMgr.DoorBellTimeCnt--;
		if ( WifiMgr.DoorBellTimeCnt > 64 ){
			SET_DOORBELLAWAKE_H;
		}
		else if ( WifiMgr.DoorBellTimeCnt > 32 ){
			SET_DOORBELLAWAKE_L;
		}
		else{
			SET_DOORBELLAWAKE_H;
		}
	}
	else
	{
		SET_DOORBELLAWAKE_L;
	}

}

void Wifi_Mgr_Task(void)
{

	if ( WifiMgr.Power.Status == PowerOn )
	{
		if ((WifiMgr.PostMgr.PostFlag_CheckStatus > 0)&&(WifiMgr.Ack.Status == AckIdle))
		{
			if (WifiMgr.WifiConnection.Status != NotConnected )
			{
				WifiMgr.PostMgr.PostFlag_CheckStatus--;
				Wifi_SendCheckStatusCmd();
			}
			else
			{
				WifiMgr.PostMgr.PostFlag_CheckStatus =0;
			}
		}
		else if ((WifiMgr.PostMgr.PostFlag_GetUTC > 0)&&(WifiMgr.Ack.Status == AckIdle))
		{
			if (WifiMgr.WifiConnection.Status != NotConnected )
			{
				WifiMgr.PostMgr.PostFlag_GetUTC--;
				Wifi_SendGetUTCCmd();
			}
			else
			{
				WifiMgr.PostMgr.PostFlag_GetUTC =0;
			}
		}
		else if ((WifiMgr.PostMgr.PostFlag_CaptureImage > 0)&&(WifiMgr.Ack.Status == AckIdle))
		{
			if (WifiMgr.WifiConnection.Status == Connected )
			{
				Wifi_SendCaptureImageCmd();
				WifiMgr.PostMgr.PostFlag_CaptureImage--;
			}
			else if (WifiMgr.WifiConnection.Status == NotConnected )
			{
				WifiMgr.PostMgr.PostFlag_CaptureImage =0;
			}
		}
		else if ((WifiMgr.PostMgr.PostFlag_Alarm > 0)&&(WifiMgr.Ack.Status == AckIdle))
		{
			if (WifiMgr.WifiConnection.Status == Connected )
			{
				WifiMgr.PostMgr.PostFlag_Alarm--;
				Wifi_SendAlarmCmd();
			}
			else if (WifiMgr.WifiConnection.Status == NotConnected )
			{
				WifiMgr.PostMgr.PostFlag_Alarm=0;
			}
		}
		else if ((WifiMgr.PostMgr.PostFlag_ClearData > 0)&&(WifiMgr.Ack.Status == AckIdle))
		{
			if (WifiMgr.WifiConnection.Status == Connected )
			{
				WifiMgr.PostMgr.PostFlag_ClearData--;
				Wifi_SendClearDataCmd();	
			}
			else if (WifiMgr.WifiConnection.Status == NotConnected )
			{
				WifiMgr.PostMgr.PostFlag_ClearData=0;
			}
		}
		else if ((WifiMgr.PostMgr.PostFlag_GetHotspot > 0)&&(WifiMgr.Ack.Status == AckIdle))
		{
			if (WifiMgr.WifiConnection.Status == Connected )
			{
				WifiMgr.PostMgr.PostFlag_GetHotspot--;
				Wifi_SendGetHotspotCmd();
			}
			else if (WifiMgr.WifiConnection.Status == NotConnected )
			{
 				WifiMgr.PostMgr.PostFlag_GetHotspot = 0;
			}
		}
		else if ((WifiMgr.PostMgr.PostFlag_Info > 0)&&(WifiMgr.Ack.Status == AckIdle))
		{
			if (WifiMgr.WifiConnection.Status == Connected )
			{
				WifiMgr.PostMgr.PostFlag_Info--;
				Wifi_SendInfoCmd();
			}
			else if (WifiMgr.WifiConnection.Status == NotConnected )
			{
 				WifiMgr.PostMgr.PostFlag_Info = 0; 
			}
		}
		else if ((WifiMgr.PostMgr.PostFlag_lock > 0)&&(WifiMgr.Ack.Status == AckIdle))
		{
			if (WifiMgr.WifiConnection.Status == Connected )
			{
				WifiMgr.PostMgr.PostFlag_lock--;
				//Wifi_SendlockStatusCmd();	
			}
			else if (WifiMgr.WifiConnection.Status == NotConnected )
			{
 				WifiMgr.PostMgr.PostFlag_lock = 0;
			}
		}
		else if ((WifiMgr.PostMgr.PostFlag_ManufactureTest > 0)&&(WifiMgr.Ack.Status == AckIdle))
		{
			if (WifiMgr.WifiConnection.Status == Connected )
			{
				WifiMgr.PostMgr.PostFlag_ManufactureTest--;
				Wifi_SendManufactureTestCmd();
			}
			else if (WifiMgr.WifiConnection.Status == NotConnected )
			{
 				WifiMgr.PostMgr.PostFlag_ManufactureTest = 0; 
			}		
		}
		else if ((WifiMgr.PostMgr.PostFlag_RemoteUnlockRequest > 0)&&(WifiMgr.Ack.Status == AckIdle))
		{
			if (WifiMgr.WifiConnection.Status == Connected )
			{
				WifiMgr.PostMgr.PostFlag_RemoteUnlockRequest--;
				Wifi_SendRemoteUnlockRequestCmd();
			}
			else if (WifiMgr.WifiConnection.Status == NotConnected )
			{
 				WifiMgr.PostMgr.PostFlag_RemoteUnlockRequest = 0;			
			}	
		}
		else if ((WifiMgr.PostMgr.PostFlag_RemoteUnlockSuccess > 0)&&(WifiMgr.Ack.Status == AckIdle))
		{
			if (WifiMgr.WifiConnection.Status == Connected )
			{
				WifiMgr.PostMgr.PostFlag_RemoteUnlockSuccess--;
				Wifi_SendRemoteUnlockSuccessAck();
			}
			else if (WifiMgr.WifiConnection.Status == NotConnected )
			{
 				WifiMgr.PostMgr.PostFlag_RemoteUnlockSuccess = 0;	
			}
		}
		else if ((WifiMgr.PostMgr.PostFlag_Unlock > 0)&&(WifiMgr.Ack.Status == AckIdle))
		{
			if (WifiMgr.WifiConnection.Status == Connected )
			{
				WifiMgr.PostMgr.PostFlag_Unlock--;
				Wifi_SendUnlockStatusCmd();
			}
			else if (WifiMgr.WifiConnection.Status == NotConnected )
			{
 				WifiMgr.PostMgr.PostFlag_Unlock = 0;			
			}
		}
		else if ((WifiMgr.PostMgr.PostFlag_OnlinePasswordVerify > 0)&&(WifiMgr.Ack.Status == AckIdle))
		{
			if (WifiMgr.WifiConnection.Status == Connected )
			{
				WifiMgr.PostMgr.PostFlag_OnlinePasswordVerify--;
				Wifi_SendOnlinePasscodeVerifyCmd(YZYJ_OnlinePasscodeBuff);
			}
			else if (WifiMgr.WifiConnection.Status == NotConnected )
			{
				WifiMgr.PostMgr.PostFlag_OnlinePasswordVerify = 0;
			}
		}
		else if ((WifiMgr.PostMgr.PostFlag_VideoConfig > 0)&&(WifiMgr.Ack.Status == AckIdle))
		{
			if (WifiMgr.WifiConnection.Status == Connected )
			{
				WifiMgr.PostMgr.PostFlag_VideoConfig--;
				Wifi_SendVideoConfigCmd();
			}
			else if (WifiMgr.WifiConnection.Status == NotConnected )
			{
				WifiMgr.PostMgr.PostFlag_VideoConfig =0;
			}
		}
		else if ((WifiMgr.PostMgr.PostFlag_PowerDown > 0)&&(WifiMgr.Ack.Status == AckIdle))
		{
			if (WifiMgr.WifiConnection.Status == Connected )
			{
				WifiMgr.PostMgr.PostFlag_PowerDown--;
				Wifi_SendPowerDownCmd();
			}
			else if (WifiMgr.WifiConnection.Status == NotConnected )
			{
				WifiMgr.PostMgr.PostFlag_PowerDown =0;
			}
		}
	}
		
	Wifi_PowerMgr_Task();

	Wifi_DoorBellMgr_Task();
	
	Wificmd_Excute();
}

#endif

