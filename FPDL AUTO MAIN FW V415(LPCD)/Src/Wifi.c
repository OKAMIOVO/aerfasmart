#include "Project.h"
#include "IO.h"
#include "Hardware.h"
#include "ISR.h"
#include "StdTypes.h"
#include "usart.h"
#include "HostUart.h"
#include "RealTimeClock.h"

status_t UART2_Write_TxBUFFER(unsigned char *buff,unsigned char len)
{
	unsigned char i;
	if ( UART2_Mgr.TxLength !=0x00)		//if TX Buffer is not empty
	{
		return S_FAIL;
	}
	
	for ( i=0;i<len;i++)
	{
			UART2_Mgr.TxBUFFER[i] = *buff;
			buff++;
	}
	UART2_Mgr.TxLength = len;
	UART2_Mgr.TxPoint = 0;
	
	return S_SUCCESS;
}

void UART2_TX_Task(void)
{
		if ( UART2_Mgr.TxPoint < UART2_Mgr.TxLength )
		{
			if ( HAL_UART_Transmit(&huart2,&UART2_Mgr.TxBUFFER[UART2_Mgr.TxPoint],1,10) == HAL_OK )
			{
				UART2_Mgr.TxPoint++;
				DEBUG_MARK;
			}
		}
		else if ( (UART2_Mgr.TxPoint==UART2_Mgr.TxLength)&&(UART2_Mgr.TxLength > 0) ) //TX Finished
		{
			UART2_Mgr.TxPoint = 0;
			UART2_Mgr.TxLength = 0;	
		}
}

void UART2_CLEAR_TXBUFF(void)
{
	UART2_Mgr.TxPoint = 0;
	UART2_Mgr.TxLength = 0;
	UART2_Mgr.Status = Idle;
}


void ComPort_Cmd_Excute(void)
{
	uint16_t i;
	uint8_t RxBuff[100];
	uint8_t CmdLenth,CKS,TempCKS;

	if ( (ComportMgr.Status != AckIdle))
		
	{
		if ( ComportMgr.TimeOutCnt > 0 )
		{
			 ComportMgr.TimeOutCnt--;
		}
		else
		{
			if ( ComportMgr.Status == WaitWifiHostSpotAck )
			{
				WifiMgr.Link.Status = LinkFail;
				WifiMgr.Link.errorcode = 0x01;
			}
			else if ( ComportMgr.Status == WaitRemoteUnlcokAck )
			{
				WifiMgr.RemoteUnlockMgr.Status = RemoteUnlockFail;
			}
			else if ( ComportMgr.Status == WaitMFTAck )
			{
				WifiMgr.MFT.Status = MFTFail;
			}
			else if ( ComportMgr.Status == WaitMotorSelftestAck )
			{
				AutoMotorMgr.SelfTest.Status = SelfTestFail;
			}

			ComportMgr.Status = AckIdle;//WaitAckTimeOut;
			if ( ComportMgr.TimeOutTimes < 10 ){
				ComportMgr.TimeOutTimes++;
			}
			Hardware_Awake_Driver();
		}
	}

	if ( UART2_Mgr.Status == ReceivingData )
	{
		if ( UART2_Mgr.DataStreamTimeCnt > 0 )
		{
			UART2_Mgr.DataStreamTimeCnt--;
		}
		else	//reset UART2 receiving status
		{
			UART2_Mgr.RX_DataPoint = 0x00;
			UART2_Mgr.Status = Idle;
			for ( i=0;i<100;i++ )
			{
				UART2_Mgr.RX_Buffer[i]=0x00;
			}
		}
	}
	
	if ( UART2_Mgr.Status != GotNewCmd ){
		return;
	}

	CmdLenth = 12;
	for (i=0;i<CmdLenth;i++)
	{
		RxBuff[i] = UART2_Mgr.RX_Buffer[i];
	}
	
	UART2_Mgr.RX_DataPoint = 0x00;
	UART2_Mgr.Status = Idle;	
	
	CKS = 0;
	for (i=0;i<(CmdLenth-1);i++)
	{
		CKS+= RxBuff[i];
	}
		
	DEBUG_MARK;
	
	if ( CKS!=RxBuff[CmdLenth-1] )
	{
		DEBUG_MARK;

		return;		//if check sum is failed, ignore this data string
	}

	ComportMgr.TimeOutTimes = 0x00;
	ComportMgr.Status = AckIdle;

	if ( RxBuff[2] == 0x01 )
	{
		ComportMgr.PostParameter = 0x00;
	}
	else if ( RxBuff[2] == 0x02 )
	{
		ComportMgr.PostStatus = 0x00;
		if ( RxBuff[3] == 0x01 )
		{
			ComportMgr.DoorStatus = Openning;
			DEBUG_MARK;
		}
		else if ( RxBuff[3] == 0x02 )
		{
			ComportMgr.DoorStatus = OpenedWaitClose;
			DEBUG_MARK;
		}
		else if ( RxBuff[3] == 0x03 )
		{
			ComportMgr.DoorStatus = Open;
			DEBUG_MARK;
		}
		else if ( RxBuff[3] == 0x04 )
		{
			ComportMgr.DoorStatus = Closing;
			DEBUG_MARK;
		}
		else if ( RxBuff[3] == 0x05 )
		{
			ComportMgr.DoorStatus = Close;
			DEBUG_MARK;
		}
		else
		{
			ComportMgr.DoorStatus = Standby;
			DEBUG_MARK;
		}

		
		if ( RxBuff[4] == 0x01 )
		{
			ComportMgr.RestoreFactoryDefaultTrig = bTRUE;
			DEBUG_MARK;
		}

		
		if ( RxBuff[5] == 0x01 )
		{
			ComPort_SetPost_Parameter();
		}

		DriverBoardVersion[4] = RxBuff[8]/100+0x30;
		DriverBoardVersion[6] = (RxBuff[8]%100)/10+0x30;
		DriverBoardVersion[7] = RxBuff[8]%10+0x30;
		
		DriverBoardVersion[10] = RxBuff[9]/100+0x30;
		DriverBoardVersion[12] = (RxBuff[9]%100)/10+0x30;
		DriverBoardVersion[13] = RxBuff[9]%10+0x30;
		
	}
	else if ( RxBuff[2] == 0x03 )
	{
		if (RxBuff[3] == 0x01)
		{
			ComportMgr.PostOpenDoor = 0x00;	
			//DisplayDoorStatusMgr.Status = GotOpenDoorACK;
		}
		else if (RxBuff[3] == 0x02) 
		{
			ComportMgr.PostCloseDoor = 0x00;
			//DisplayDoorStatusMgr.Status = GotCloseDoorACK;
		}
	}
	else if ( RxBuff[2] == 0x04 )
	{
		ComportMgr.PostInfo = 0x00;
	}
	else if ( RxBuff[2] == 0x05 )
	{
		ComportMgr.PostAlarm = 0x00;
	}
	else if ( RxBuff[2] == 0x06 )
	{
		if ( ( RxBuff[3] == 0x01 ))
		{
			ComportMgr.PostWifiHostSpot = 0x00; 
			if ( RxBuff[10] == 0x00 )
			{
				WifiMgr.Link.Status = LinkSuccess;
				ComPort_SetPost_GetNetWorkTime();
			}
			else
			{
				WifiMgr.Link.Status = LinkFail;
				WifiMgr.Link.errorcode = RxBuff[10];
			}
		}
		else if ( RxBuff[3] == 0x02 )
		{
			ComportMgr.PostExitWifiHostSpot= 0x00;
		}	
	}
	else if ( RxBuff[2] == 0x07 )
	{
		ComportMgr.PostRemoteUnlock = 0x00;
		if ( RxBuff[10] == 0x00 )
		{
			WifiMgr.RemoteUnlockMgr.Status = RemoteUnlockSuccess;
		}
		else
		{
			WifiMgr.RemoteUnlockMgr.Status = RemoteUnlockFail;
			WifiMgr.RemoteUnlockMgr.errorcode = RxBuff[10];
		}
	}
	else if ( RxBuff[2] == 0x08 )
	{
		ComportMgr.PostClearWifiData= 0x00;
	}
	else if ( RxBuff[2] == 0x09 )
	{
		if (RxBuff[10]==0x00)
		{
			SystemTime.year 	= RxBuff[3];
			SystemTime.month 	= RxBuff[4];
			SystemTime.date 	= RxBuff[5];
			SystemTime.hour 	= RxBuff[6];
			SystemTime.minute 	= RxBuff[7];
			SystemTime.second 	= RxBuff[8];	
			WriteSystemTime();
			ComportMgr.PostGetNetworkTime = 0x00;		//if get UTC,don't post again
		}
		else if (RxBuff[10]==0x02)	//wifi not connected;
		{
			SystemTime.SavedMonthWhileNetWorkTimeUpdated=SystemTime.month;
			ComportMgr.PostGetNetworkTime = 0x00;	//if wifi is not connected,don't post again
			DEBUG_MARK;
		}
		else
		{
			DEBUG_MARK;
		}
	}
	else if ( RxBuff[2] == 0x0A )
	{
		ComportMgr.PostWifiMFT = 0x00;
		if ( RxBuff[10] == 0x00 )
		{
			WifiMgr.MFT.Status = MFTSuccess;
		}
		else
		{
			WifiMgr.MFT.Status = MFTFail;
			WifiMgr.MFT.errorcode = RxBuff[10];
		}
	}
	else if ( RxBuff[2] == 0x0B )
	{
		AutoMotorMgr.SelfTest.ErrorCode = RxBuff[10];
		AutoMotorMgr.SelfTest.TimeLimitedCnt = 3*64;
		AutoMotorMgr.SelfTest.Status = SelfTestSuccess;
		ComportMgr.PostMotorSelftest = 0x00;
	}
		
	DEBUG_MARK;
}

void ComPort_SendParameterCmd(void)
{
	uint8_t CKS,i,CmdLenth;
	uint8_t buff[12]={0xCA,0xAC,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF};
	CmdLenth = 12;

	  if (AutoMotorMgr.TorqueLevel == LargeTorque)
	   {
				buff[3] = 70;  //=70*0.05 A
	   }
	  else if (AutoMotorMgr.TorqueLevel == MiddleTorque)
	   {
				buff[3] = 60;  //=60*0.05 A
	   }
	  else
	  {
				buff[3] = 50; //=50*0.05 A
	  }

	
	buff[4] = AutoMotorMgr.LockDirection;
	buff[5] = AutoMotorMgr.UnlockTime;
	buff[6] = AutoMotorMgr.AutoLockTime;
	buff[7] = AutoMotorMgr.BoltLockTime;

	buff[8] = (AutoMotorMgr.LockingTravel<<4)+(AutoMotorMgr.AutoEject&0x0F);
		
	CKS=0x00;	
	for ( i=0;i<(CmdLenth-1);i++ )
	{
		CKS = CKS+buff[i];	
	}
	buff[CmdLenth-1] = CKS;
	
	UART2_Write_TxBUFFER(&buff[0],CmdLenth);

	ComportMgr.TimeOutCnt = 1*64;		//1s
	ComportMgr.Status = WaitParameterAck;
	UART2_Mgr.RX_DataPoint = 0x00;
	UART2_Mgr.Status = Idle;

}

void ComPort_SendStatusCmd(void)
{
	uint8_t CKS,i,CmdLenth;
	uint8_t buff[12]={0xCA,0xAC,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF};
	CmdLenth = 12;
	
	buff[3] = BatteryMgr.BatteryVoltage;
		
	CKS=0x00;		
	for ( i=0;i<(CmdLenth-1);i++ )
	{
		CKS = CKS+buff[i];	
	}
	buff[CmdLenth-1] = CKS;
	
	UART2_Write_TxBUFFER(&buff[0],CmdLenth);

	ComportMgr.TimeOutCnt = 1*64;		//1s
	ComportMgr.Status = WaitStatusAck;
	UART2_Mgr.RX_DataPoint = 0x00;
	UART2_Mgr.Status = Idle;

}


void ComPort_SendGetHotspotCmd(void)
{
	uint8_t CKS,i,CmdLenth;
	uint8_t buff[12]={0xCA,0xAC,0x06,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF};
	CmdLenth = 12;
	CKS=0x00;
	for ( i=0;i<(CmdLenth-1);i++ )
	{
		CKS = CKS+buff[i];	
	}
	buff[CmdLenth-1] = CKS;
	
	UART2_Write_TxBUFFER(&buff[0],CmdLenth);

	ComportMgr.TimeOutCnt = 185*64;		//185s
	ComportMgr.Status = WaitWifiHostSpotAck;
	UART2_Mgr.RX_DataPoint = 0x00;
	UART2_Mgr.Status = Idle;


	//HAL_UART_Receive_IT(&huart2,&UART2_Mgr.RX_Buffer[0],WifiMgr.Ack.AckLenth);

}
void ComPort_SendExitHotspotCmd(void)
{
	uint8_t CKS,i,CmdLenth;
	uint8_t buff[12]={0xCA,0xAC,0x06,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF};
	CmdLenth = 12;
	CKS=0x00;
	for ( i=0;i<(CmdLenth-1);i++ )
	{
		CKS = CKS+buff[i];	
	}
	buff[CmdLenth-1] = CKS;
	
	UART2_Write_TxBUFFER(&buff[0],CmdLenth);

	ComportMgr.TimeOutCnt = 1*64;		//1s
	ComportMgr.Status = WaitExitWifiHostSpotAck;
	UART2_Mgr.RX_DataPoint = 0x00;
	UART2_Mgr.Status = Idle;


	//HAL_UART_Receive_IT(&huart2,&UART2_Mgr.RX_Buffer[0],WifiMgr.Ack.AckLenth);

}


void ComPort_SendOpenDoorCmd(UserType_t usertype, uint16_t userID )
{ 
	uint8_t CKS,i,CmdLenth;
	uint8_t buff[12]={0xCA,0xAC,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF};
	CmdLenth = 12;
	if ( usertype == FPUSER ){
		buff[4]=1;
	}else if ( usertype == PASSCODEUSER ){
		buff[4]=2;
	}else if ( usertype == CARDUSER ){
		buff[4]=3;
	}
	buff[5] = userID%256;
	buff[6] = userID/256;
	
	CKS=0x00;
	for ( i=0;i<(CmdLenth-1);i++ )
	{
		CKS = CKS+buff[i];	
	}
	buff[CmdLenth-1] = CKS;
	
	UART2_Write_TxBUFFER(&buff[0],CmdLenth);
	
	ComportMgr.TimeOutCnt = 1*64;		//1s
	ComportMgr.Status = WaitOpenDoorAck;

	UART2_Mgr.RX_DataPoint = 0x00;
	UART2_Mgr.Status = Idle;

}

void ComPort_SendCloseDoorCmd(void)
{ 
	uint8_t CKS,i,CmdLenth;
	uint8_t buff[12]={0xCA,0xAC,0x03,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF};
	CmdLenth = 12;
	
	CKS=0x00;
	for ( i=0;i<(CmdLenth-1);i++ )
	{
		CKS = CKS+buff[i];	
	}
	buff[CmdLenth-1] = CKS;
	
	UART2_Write_TxBUFFER(&buff[0],CmdLenth);
	
	ComportMgr.TimeOutCnt = 1*64;		//1s
	ComportMgr.Status = WaitCloseDoorAck;


	UART2_Mgr.RX_DataPoint = 0x00;
	UART2_Mgr.Status = Idle;

}


void ComPort_SendRemoteUnlockRequestCmd(void )
{ 
	uint8_t CKS,i,CmdLenth;
	uint8_t buff[12]={0xCA,0xAC,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF};
	CmdLenth = 12;
	
	CKS=0x00;
	for ( i=0;i<(CmdLenth-1);i++ )
	{
		CKS = CKS+buff[i];	
	}
	buff[CmdLenth-1] = CKS;
	
	UART2_Write_TxBUFFER(&buff[0],CmdLenth);
	
	ComportMgr.TimeOutCnt = 65*64;		//65s
	ComportMgr.Status = WaitRemoteUnlcokAck;


	UART2_Mgr.RX_DataPoint = 0x00;
	UART2_Mgr.Status = Idle;
	

}

void ComPort_SendInfoCmd(uint8_t InfoType, UserType_t usertype, uint16_t userID)
{ 
	uint8_t CKS,i,CmdLenth;
	uint8_t buff[12]={0xCA,0xAC,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF};
	CmdLenth = 12;
	
	buff[3]=InfoType;

	if ( usertype == FPUSER ){
		buff[4]=1;
	}else if ( usertype == PASSCODEUSER ){
		buff[4]=2;
	}else if ( usertype == CARDUSER ){
		buff[4]=3;
	}
	buff[5] = userID%256;
	buff[6] = userID/256;

	CKS=0x00;
	for ( i=0;i<(CmdLenth-1);i++ )
	{
		CKS = CKS+buff[i];	
	}
	buff[CmdLenth-1] = CKS;
	
	UART2_Write_TxBUFFER(&buff[0],CmdLenth);
	
	ComportMgr.TimeOutCnt = 1*64;		//1s
	ComportMgr.Status = WaitInfoAck;


	UART2_Mgr.RX_DataPoint = 0x00;
	UART2_Mgr.Status = Idle;


}

void ComPort_SendAlarmCmd(uint8_t AlarmType,UserType_t usertype, uint16_t userID)
{ 
	uint8_t CKS,i,CmdLenth;
	uint8_t buff[12]={0xCA,0xAC,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF};
	CmdLenth = 12;

	buff[3]=AlarmType;
	
	if ( usertype == FPUSER ){
		buff[4]=2;
	}else if ( usertype == PASSCODEUSER ){
		buff[4]=3;
	}else if ( usertype == CARDUSER ){
		buff[4]=4;
	}
	buff[5] = userID%256;
	buff[6] = userID/256;
	
	CKS=0x00;
	for ( i=0;i<(CmdLenth-1);i++ )
	{
		CKS = CKS+buff[i];	
	}
	buff[CmdLenth-1] = CKS;
	
	UART2_Write_TxBUFFER(&buff[0],CmdLenth);
	
	ComportMgr.TimeOutCnt = 1*64;		//1s
	ComportMgr.Status = WaitAlarmAck;


	UART2_Mgr.RX_DataPoint = 0x00;
	UART2_Mgr.Status = Idle;

}

void ComPort_SendGetNetworkTimeCmd(void)
{ 
	uint8_t CKS,i,CmdLenth;
	uint8_t buff[12]={0xCA,0xAC,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF};
	CmdLenth = 12;
	
	CKS=0x00;
	for ( i=0;i<(CmdLenth-1);i++ )
	{
		CKS = CKS+buff[i];	
	}
	buff[CmdLenth-1] = CKS;
	
	UART2_Write_TxBUFFER(&buff[0],CmdLenth);
	
	ComportMgr.TimeOutCnt = 2*64;		//2s
	ComportMgr.Status = WaitGetNetworkTimeAck;

	UART2_Mgr.RX_DataPoint = 0x00;
	UART2_Mgr.Status = Idle;
}

void ComPort_SendClearWifiDataCmd(void)
{
	uint8_t CKS,i,CmdLenth;
	uint8_t buff[12]={0xCA,0xAC,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF};
	CmdLenth = 12;
	CKS=0x00;
	for ( i=0;i<(CmdLenth-1);i++ )
	{
		CKS = CKS+buff[i];	
	}
	buff[CmdLenth-1] = CKS;
	
	UART2_Write_TxBUFFER(&buff[0],CmdLenth);

	ComportMgr.TimeOutCnt = 1*64; 	//1s
	ComportMgr.Status = WaitClearWifiDataAck;
	UART2_Mgr.RX_DataPoint = 0x00;
	UART2_Mgr.Status = Idle;


	//HAL_UART_Receive_IT(&huart2,&UART2_Mgr.RX_Buffer[0],WifiMgr.Ack.AckLenth);

}

void ComPort_SendWifiMFTCmd(void)
{
	uint8_t CKS,i,CmdLenth;
	uint8_t buff[12]={0xCA,0xAC,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF};
	CmdLenth = 12;
	CKS=0x00;
	for ( i=0;i<(CmdLenth-1);i++ )
	{
		CKS = CKS+buff[i];	
	}
	buff[CmdLenth-1] = CKS;
	
	UART2_Write_TxBUFFER(&buff[0],CmdLenth);

	ComportMgr.TimeOutCnt = 15*64; 	//15s
	ComportMgr.Status = WaitMFTAck;
	UART2_Mgr.RX_DataPoint = 0x00;
	UART2_Mgr.Status = Idle;

	//HAL_UART_Receive_IT(&huart2,&UART2_Mgr.RX_Buffer[0],WifiMgr.Ack.AckLenth);

}
void ComPort_SendMotorSelfTestCmd(void)
{
	uint8_t CKS,i,CmdLenth;
	uint8_t buff[12]={0xCA,0xAC,0x0B,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF};
	CmdLenth = 12;
	CKS=0x00;
	for ( i=0;i<(CmdLenth-1);i++ )
	{
		CKS = CKS+buff[i];	
	}
	buff[CmdLenth-1] = CKS;
	
	UART2_Write_TxBUFFER(&buff[0],CmdLenth);

	ComportMgr.TimeOutCnt = 30*64; 	//30s
	ComportMgr.Status = WaitMotorSelftestAck;
	UART2_Mgr.RX_DataPoint = 0x00;
	UART2_Mgr.Status = Idle;

	//HAL_UART_Receive_IT(&huart2,&UART2_Mgr.RX_Buffer[0],WifiMgr.Ack.AckLenth);

}


void ComPort_SendPowerDownCmd(void)
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
	
	UART2_Write_TxBUFFER(&buff[0],CmdLenth);
	
	UART2_Mgr.RX_DataPoint = 0x00;
	UART2_Mgr.Status = Idle;
	
}


void ComPort_SetPost_Parameter(void)
{
	ComportMgr.PostParameter = 3;
}
void ComPort_SetPost_Status(void)
{
	ComportMgr.PostStatus = 3;
}
void ComPort_SetPost_OpenDoor(uint8_t Usertype,uint16_t userID)
{
	ComportMgr.PostOpenDoor = 3;
	ComportMgr.Unlock.UserType = Usertype;
	ComportMgr.Unlock.UserID = userID;
}
void ComPort_SetPost_CloseDoor(void)
{
	ComportMgr.PostCloseDoor = 3;
	DEBUG_MARK;
}

void ComPort_SetPost_Info(uint8_t infotype,uint8_t Usertype,uint16_t userID)
{
	ComportMgr.PostInfo = 3;
	ComportMgr.Info.Type = infotype;
	ComportMgr.Info.UserType = Usertype;
	ComportMgr.Info.UserID = userID;
}
void ComPort_SetPost_Alarm(uint8_t alarmtype,uint8_t Usertype,uint16_t userID)
{
	ComportMgr.PostAlarm = 3;
	ComportMgr.Alarm.Type = alarmtype;
	ComportMgr.Alarm.UserType = Usertype;
	ComportMgr.Alarm.UserID = userID;
}
void ComPort_SetPost_WifiHotSpot(void)
{
	ComportMgr.PostWifiHostSpot = 1;
}
void ComPort_SetPost_ExitWifiHotSpot(void)
{
	ComportMgr.PostExitWifiHostSpot = 3;
}
void ComPort_SetPost_ClearWifiData(void)
{
	ComportMgr.PostClearWifiData = 3;
}
void ComPort_SetPost_GetNetWorkTime(void)
{
	ComportMgr.PostGetNetworkTime = 3;
}

void ComPort_SetPost_RemoteUnlcok(void)
{
	ComportMgr.PostRemoteUnlock = 1;
}
void ComPort_SetPost_WifiMFT(void)
{
	ComportMgr.PostWifiMFT = 1;
}
void ComPort_SetPost_MotorSelftest(void)
{
	ComportMgr.PostMotorSelftest = 1;
}


void ComPort_Init(void)
{

	UART2_Mgr.TxPoint = 0;
	UART2_Mgr.TxLength = 0;
	UART2_Mgr.RX_DataPoint = 0x00;
	UART2_Mgr.Status = Idle;

	ComportMgr.PostParameter = 0x03;
	ComportMgr.PostStatus = 0x00;
	ComportMgr.PostOpenDoor = 0x00;	
	ComportMgr.PostInfo = 0x00;
	ComportMgr.PostAlarm = 0x00;
	ComportMgr.PostWifiMFT = 0x00;
	ComportMgr.TimeOutTimes = 0x00;

	ComportMgr.DoorStatus = Standby;

	ComportMgr.Status = AckIdle;
}

void ComPort_Mgr_Task(void)
{

	if ((ComportMgr.TimeCnt++ > 32 )&&(ComportMgr.Status == AckIdle))
	{
		ComportMgr.TimeCnt =0;
		ComportMgr.PostStatus=1;	
	}

	if ((ComportMgr.PostParameter > 0 )&&(ComportMgr.Status == AckIdle))
	{
		ComportMgr.PostParameter--;
		ComPort_SendParameterCmd();
	}
	else if ( (ComportMgr.PostOpenDoor > 0 )&&(ComportMgr.Status == AckIdle))
	{
		ComportMgr.PostOpenDoor--;
		ComPort_SendOpenDoorCmd(ComportMgr.Unlock.UserType,ComportMgr.Unlock.UserID);
	}
	else if ( (ComportMgr.PostCloseDoor > 0 )&&(ComportMgr.Status == AckIdle))
	{
		ComportMgr.PostCloseDoor--;
		ComPort_SendCloseDoorCmd();
	}
	else if ( (ComportMgr.PostInfo > 0 )&&(ComportMgr.Status == AckIdle))
	{
		ComportMgr.PostInfo--;
		ComPort_SendInfoCmd(ComportMgr.Info.Type,ComportMgr.Info.UserType,ComportMgr.Info.UserID);
	}
	else if ( (ComportMgr.PostAlarm > 0 )&&(ComportMgr.Status == AckIdle))
	{
		ComportMgr.PostAlarm--;
		ComPort_SendAlarmCmd(ComportMgr.Alarm.Type,ComportMgr.Alarm.UserType,ComportMgr.Alarm.UserID);
	}
	else if ( (ComportMgr.PostWifiHostSpot > 0 )&&(ComportMgr.Status == AckIdle))
	{
		ComportMgr.PostWifiHostSpot--;
		ComPort_SendGetHotspotCmd();
	}
	else if ( ( ComportMgr.PostExitWifiHostSpot > 0 )&&((ComportMgr.Status == WaitWifiHostSpotAck)||(ComportMgr.Status == AckIdle)))
	{
		ComportMgr.PostExitWifiHostSpot--;
		ComPort_SendExitHotspotCmd();
	}
	else if ( (ComportMgr.PostClearWifiData > 0 )&&(ComportMgr.Status == AckIdle))
	{
		ComportMgr.PostClearWifiData--;
		ComPort_SendClearWifiDataCmd();
	}
	else if ( (ComportMgr.PostRemoteUnlock > 0 )&&(ComportMgr.Status == AckIdle))
	{
		ComportMgr.PostRemoteUnlock--;
		ComPort_SendRemoteUnlockRequestCmd();
	}
	else if ( (ComportMgr.PostGetNetworkTime > 0 )&&(ComportMgr.Status == AckIdle))
	{
		ComportMgr.PostGetNetworkTime--;
		ComPort_SendGetNetworkTimeCmd();
	}
	else if ( (ComportMgr.PostWifiMFT > 0 )&&(ComportMgr.Status == AckIdle))
	{
		ComportMgr.PostWifiMFT--;
		ComPort_SendWifiMFTCmd();
	}
	else if ( (ComportMgr.PostMotorSelftest > 0 )&&(ComportMgr.Status == AckIdle))
	{
		ComportMgr.PostMotorSelftest--;
		ComPort_SendMotorSelfTestCmd();
	}
	else if ( (ComportMgr.PostStatus > 0 )&&(ComportMgr.Status == AckIdle))
	{
		ComportMgr.PostStatus--;
		ComPort_SendStatusCmd();
	}

	
	ComPort_Cmd_Excute();
}

