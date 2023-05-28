#include "Project.h"
#include "IO.h"
#include "Hardware.h"
#include "ISR.h"
#include "StdTypes.h"
#include "usart.h"
#include "HostUart.h"
#include "RealTimeClock.h"
#include "GUI.h"

#define DEF_HostUartTaskRunFre	128

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
				//WifiMgr.Link.Status = LinkFail;
				//WifiMgr.Link.errorcode = 0x01;
			}
			else if ( ComportMgr.Status == WaitRemoteUnlcokAck )
			{
				//WifiMgr.RemoteUnlockMgr.Status = RemoteUnlockFail;
			}
			else if ( ComportMgr.Status == WaitMFTAck )
			{
				//WifiMgr.MFT.Status = MFTFail;
			}
			else if ( ComportMgr.Status == WaitMotorSelftestAck )
			{
				//AutoMotorMgr.SelfTest.Status = SelfTestFail;
			}
			else if ( ComportMgr.Status == WaitOnlinePasswordVerifyCmdACK )
			{
				WifiMgr.WifiOnlinePasswordVerifyMgr.Status = OnlinePasswordVerifyFail;
			}
			else if ( ComportMgr.Status == WaitPlayVoiceAck )
			{
			
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

	if (ComportMgr.Flag_RxBuff1IsFull == bTRUE )
	{
		memcpy(RxBuff,ComportMgr.RxBuff1,12);
		ComportMgr.Flag_RxBuff1IsFull = bFALSE;
	}
	else if (ComportMgr.Flag_RxBuff2IsFull == bTRUE )
	{
		memcpy(RxBuff,ComportMgr.RxBuff2,12);
		ComportMgr.Flag_RxBuff2IsFull = bFALSE;
	} 
	else if ( UART2_Mgr.Status == GotNewCmd )
	{
		memcpy(RxBuff,UART2_Mgr.RX_Buffer,12);
		UART2_Mgr.RX_DataPoint = 0x00;
		UART2_Mgr.Status = Idle;
	}
	else
	{
		return;
	}

	CmdLenth=12;
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

	SelfTestMgr.ComOkFlag = bTRUE;		//for self test

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

		if ( (RxBuff[6]&0x01) == 0x00 )
		{
			WifiMgr.WifiConfigStatus.Status = NotConnectedToRooter;
		}
		else
		{
			WifiMgr.WifiConfigStatus.Status = ConnectedToRooter;
		}

		DriverBoardVersion.HWversion = RxBuff[8];
		DriverBoardVersion.FWversion = RxBuff[9];

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
				ComportMgr.TimeSyncWithNetwork = bTRUE;
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
		if ( RxBuff[3] == 0x00 )
		{
			ComportMgr.PostRemoteUnlock = 0x00;
			if ( RxBuff[10] == 0x00 )
			{
				//WifiMgr.RemoteUnlockMgr.Status = RemoteUnlockSuccess;
				WifiMgr.RemoteUnlockMgr.Result = IsSuccess;
			}
			else //if ( RxBuff[10] != 0x00)
			{
				//WifiMgr.RemoteUnlockMgr.Status = RemoteUnlockFail;
				WifiMgr.RemoteUnlockMgr.Result = IsFail;
				WifiMgr.RemoteUnlockMgr.errorcode = RxBuff[10];
			}
		}
		else  //RxBuff[3] = 0x01
		{
			ComportMgr.PostExitRemoteUnlock = 0x00;
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
			SystemTime.SavedDateWhileNetWorkTimeUpdated=SystemTime.date;
			ComportMgr.PostGetNetworkTime = 0x00;	//if wifi is not connected,don't post again
			DEBUG_MARK;
		}
		else if (RxBuff[10]==0x03)	//driver board will return UTC later
		{
			ComportMgr.PostGetNetworkTime = 0x00;	//don't post again
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
	else if ( RxBuff[2] == 0x0C )
	{
		if (RxBuff[3]==0x00)
		{
			WifiMgr.WifiOnlinePasswordVerifyMgr.Status = OnlinePasswordVerifySuccess;
			WifiMgr.WifiOnlinePasswordVerifyMgr.OnlinePasswordUserPriority = RxBuff[4];
			WifiMgr.WifiOnlinePasswordVerifyMgr.OnlinePasswordUserType = RxBuff[5];
			WifiMgr.WifiOnlinePasswordVerifyMgr.UserID = RxBuff[6]+RxBuff[7]*256;
		}
		else
		{
			WifiMgr.WifiOnlinePasswordVerifyMgr.Status = OnlinePasswordVerifyFail;
		}
		
		ComportMgr.PostOnlinePasswordVerify = 0x00;
	}
	else if ( RxBuff[2] == 0x0D )
	{
		ComportMgr.PostPositivePowerOnWifi = 0x00;
		WifiMgr.WifiOnlinePasswordVerifyMgr.WifiPowerIsOnForOnlinePassword = bTRUE;
	}
	else if ( RxBuff[2] == 0x20 )
	{
		if (RxBuff[3] == 0x00)
		{
			VoiceDataTransferMgr.VoicePlayEnd = bTRUE;
		}	
		ComportMgr.PostGetVoicePlayerStatus=0;	
		DEBUG_MARK;
	}
	else if ( RxBuff[2] == 0x21 )
	{
		DEBUG_MARK;
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
	buff[8] = AutoMotorMgr.LockingTravel<<4;
	buff[8] += AutoMotorMgr.AutoEject;	
	
	if ( SystemLanguage == Chinese )
	{
		buff[9] &=0xEF;	//CLEAR BIT4 
	}
	else
	{
		buff[9] |=0x10;	//SET BIT4 
	}
	buff[9] +=VoiceMgr.volume;//BIT3~BIT0, 代表音量，0000代表静音，0001~0003，代表低，中，高音量
		
		
	CKS=0x00;	
	for ( i=0;i<(CmdLenth-1);i++ )
	{
		CKS = CKS+buff[i];	
	}
	buff[CmdLenth-1] = CKS;
	
	UART2_Write_TxBUFFER(&buff[0],CmdLenth);

	ComportMgr.TimeOutCnt = 1*DEF_HostUartTaskRunFre;		//1s
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

	ComportMgr.TimeOutCnt = 1*DEF_HostUartTaskRunFre;		//1s
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

	ComportMgr.TimeOutCnt = 1*DEF_HostUartTaskRunFre;//185*DEF_HostUartTaskRunFre;		//185s
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

	ComportMgr.TimeOutCnt = 1*DEF_HostUartTaskRunFre;		//1s
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
	if ( usertype == FPUSER )
	{
		if ( userID > (DEF_MAX_FPMASTER+DEF_MAX_FPUSER) )
		{
			buff[4]=0x0A;
		}
		else
		{
			buff[4]=0x01;
		}
	}
	else if ( usertype == PASSCODEUSER ){
		buff[4]=0x02;
	}
	else if ( usertype == CARDUSER ){
		buff[4]=0x03;
	}
	else if ( usertype == FACEUSER ){
		buff[4]=0x06;	
	}
	else if ( usertype == ONLINEPASSCODEUSER ){
		buff[4]=0x09;	//OnlinePasscode
	}
	else if ( usertype == AGINGTESTUSER ){
		buff[4]=0xFF;
	}
	buff[5] = userID%256;
	buff[6] = userID/256;


	if ( StrongUnlockMgr.TrigStrongUnlocking == bTRUE )
	{
		buff[7] = 0x01;
	}
	else
	{
		buff[7] = 0x00;
	}
	
	CKS=0x00;
	for ( i=0;i<(CmdLenth-1);i++ )
	{
		CKS = CKS+buff[i];	
	}
	buff[CmdLenth-1] = CKS;
	
	UART2_Write_TxBUFFER(&buff[0],CmdLenth);
	
	ComportMgr.TimeOutCnt = 1*DEF_HostUartTaskRunFre;		//1s
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
	
	ComportMgr.TimeOutCnt = 1*DEF_HostUartTaskRunFre;		//1s
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
	
	ComportMgr.TimeOutCnt = 2*DEF_HostUartTaskRunFre;//65*DEF_HostUartTaskRunFre;		//65s
	ComportMgr.Status = WaitRemoteUnlcokAck;


	UART2_Mgr.RX_DataPoint = 0x00;
	UART2_Mgr.Status = Idle;
	

}
void ComPort_SendExitRemoteUnlockRequestCmd(void )
{ 
	uint8_t CKS,i,CmdLenth;
	uint8_t buff[12]={0xCA,0xAC,0x07,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF};
	CmdLenth = 12;
	
	CKS=0x00;
	for ( i=0;i<(CmdLenth-1);i++ )
	{
		CKS = CKS+buff[i];	
	}
	buff[CmdLenth-1] = CKS;
	
	UART2_Write_TxBUFFER(&buff[0],CmdLenth);
	
	ComportMgr.TimeOutCnt = 2*DEF_HostUartTaskRunFre;		//2s
	ComportMgr.Status = WaitExitRemoteUnlcokAck;

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
	else if ( usertype == FACEUSER ){
		buff[4]=0x06;	
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
	
	ComportMgr.TimeOutCnt = 1*DEF_HostUartTaskRunFre;		//1s
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
	
	ComportMgr.TimeOutCnt = 1*DEF_HostUartTaskRunFre;		//1s
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
	
	ComportMgr.TimeOutCnt = 2*DEF_HostUartTaskRunFre;		//2s
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

	ComportMgr.TimeOutCnt = 1*DEF_HostUartTaskRunFre; 	//1s
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

	ComportMgr.TimeOutCnt = 2*DEF_HostUartTaskRunFre;//15*DEF_HostUartTaskRunFre; 	//15s
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

	ComportMgr.TimeOutCnt = 2*DEF_HostUartTaskRunFre;//30*DEF_HostUartTaskRunFre; 	//30s
	ComportMgr.Status = WaitMotorSelftestAck;
	UART2_Mgr.RX_DataPoint = 0x00;
	UART2_Mgr.Status = Idle;

	//HAL_UART_Receive_IT(&huart2,&UART2_Mgr.RX_Buffer[0],WifiMgr.Ack.AckLenth);

}

void ComPort_SendOnlinePasswordVerifyCmd(void)
{
	uint32_t UTCtime;
	uint8_t CKS,i,CmdLenth;
	uint8_t buff[12]={0xCA,0xAC,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF};
	CmdLenth = 12;

		UTCtime = mktime(2000+BCDcodeToHEX(SystemTime.year), BCDcodeToHEX(SystemTime.month),
			BCDcodeToHEX(SystemTime.date), BCDcodeToHEX(SystemTime.hour),
			BCDcodeToHEX(SystemTime.minute), BCDcodeToHEX(SystemTime.second));

	buff[6]=UTCtime;
	UTCtime>>=8;
	buff[5]=UTCtime;
	UTCtime>>=8;
	buff[4]=UTCtime;
	UTCtime>>=8;
	buff[3]=UTCtime;

	buff[7]=0X50;	//Time Zone
	
	buff[8] = HEXtoBCDcode(WifiMgr.WifiOnlinePasswordVerifyMgr.OnlinePasswordBuff[0]*10+WifiMgr.WifiOnlinePasswordVerifyMgr.OnlinePasswordBuff[1]);
	buff[9] = HEXtoBCDcode(WifiMgr.WifiOnlinePasswordVerifyMgr.OnlinePasswordBuff[2]*10+WifiMgr.WifiOnlinePasswordVerifyMgr.OnlinePasswordBuff[3]);
	buff[10] = HEXtoBCDcode(WifiMgr.WifiOnlinePasswordVerifyMgr.OnlinePasswordBuff[4]*10+WifiMgr.WifiOnlinePasswordVerifyMgr.OnlinePasswordBuff[5]);
	
	CKS=0x00;
	for ( i=0;i<(CmdLenth-1);i++ )
	{
		CKS = CKS+buff[i];	
	}
	buff[CmdLenth-1] = CKS;
	
	UART2_Write_TxBUFFER(&buff[0],CmdLenth);

	ComportMgr.TimeOutCnt = 2*DEF_HostUartTaskRunFre; 	//2s
	ComportMgr.Status = WaitOnlinePasswordVerifyCmdACK;
	UART2_Mgr.RX_DataPoint = 0x00;
	UART2_Mgr.Status = Idle;


}

void ComPort_SendPositivePowerOnWifiCmd(void)
{
	uint8_t CKS,i,CmdLenth;
	uint8_t buff[12]={0xCA,0xAC,0x0D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF};
	CmdLenth = 12;
	CKS=0x00;
	for ( i=0;i<(CmdLenth-1);i++ )
	{
		CKS = CKS+buff[i];	
	}
	buff[CmdLenth-1] = CKS;
	
	UART2_Write_TxBUFFER(&buff[0],CmdLenth);

	ComportMgr.TimeOutCnt = 2*DEF_HostUartTaskRunFre;//30*DEF_HostUartTaskRunFre; 	//30s
	ComportMgr.Status = WaitPositivePowerOnWifiAck;
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

status_t ComPort_SendGetVoiceStatusCmd(void)
{
	uint8_t CKS,i,CmdLenth;
	uint8_t buff[12]={0xCA,0xAC,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF};
	CmdLenth = 12;
	CKS=0x00;
	for ( i=0;i<(CmdLenth-1);i++ )
	{
		CKS = CKS+buff[i];	
	}
	buff[CmdLenth-1] = CKS;
	
	if (UART2_Write_TxBUFFER(&buff[0],CmdLenth) == S_SUCCESS )
	{
		ComportMgr.TimeOutCnt = 1*DEF_HostUartTaskRunFre; 	//1s
		ComportMgr.Status = WaitGetVoiceStatusAck;
		UART2_Mgr.RX_DataPoint = 0x00;
		UART2_Mgr.Status = Idle;
	}
	else
	{
		return S_FAIL;
	}


	//HAL_UART_Receive_IT(&huart2,&UART2_Mgr.RX_Buffer[0],WifiMgr.Ack.AckLenth);

}


status_t ComPort_SendPlayVoiceCmd(uint8_t vicebuff[])
{
	
	uint8_t CKS,i,CmdLenth;
	uint8_t buff[35]={0xCA,0xAC,0x21,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		              0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		              0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
					  0x00,0x00,0x00,0x00,0xFF};

	for (i=0;i<31;i++)
	{
		buff[i+3] = vicebuff[i];
	}
	CmdLenth = 35;
	CKS=0x00;
	for ( i=0;i<(CmdLenth-1);i++ )
	{
		CKS = CKS+buff[i];	
	}
	buff[CmdLenth-1] = CKS;

	if (UART2_Write_TxBUFFER(&buff[0],CmdLenth) == S_SUCCESS )
	{
		ComportMgr.TimeOutCnt = 1*DEF_HostUartTaskRunFre; 	//1s
		ComportMgr.Status = WaitPlayVoiceAck;
		UART2_Mgr.RX_DataPoint = 0x00;
		UART2_Mgr.Status = Idle;
		VoiceDataTransferMgr.VoicePlayEnd = bFALSE;
		return S_SUCCESS;
	}
	else
	{
		return S_FAIL;
	}

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
void ComPort_SetPost_ExitRemoteUnlcok(void)
{
	ComportMgr.PostExitRemoteUnlock = 1;
	//ComportMgr.Status = AckIdle;	//interrupt wait for remote unlock ack
}

void ComPort_SetPost_WifiMFT(void)
{
	ComportMgr.PostWifiMFT = 1;
}
void ComPort_SetPost_MotorSelftest(void)
{
	ComportMgr.PostMotorSelftest = 1;
}

void ComPort_SetPost_PositivePowerOnWifi(void)
{
	ComportMgr.PostPositivePowerOnWifi = 3;
}

void ComPort_SetPost_OnlinePasswordVerify(uint8_t PasswordBUFF[])
{
	memcpy(&WifiMgr.WifiOnlinePasswordVerifyMgr.OnlinePasswordBuff[0],&PasswordBUFF[0],6);
	ComportMgr.PostOnlinePasswordVerify = 3;
}
void ComPort_SetPost_PlayVoice(uint8_t VOICEBUFF[])
{
	if ( ComportMgr.VoiceDataBuffIsFull_1 != bTRUE )
	{
		memcpy(&ComportMgr.VoiceBuff_1[0],&VOICEBUFF[0],31);
		ComportMgr.VoiceDataBuffIsFull_1 = bTRUE;
		ComportMgr.PostPlayVoice_1 = 1;
		if ( ComportMgr.VoiceDataID_2 < 255 )
		{
			ComportMgr.VoiceDataID_1 = ComportMgr.VoiceDataID_2 + 1;
		}
		else
		{
			ComportMgr.VoiceDataID_1=0;
		}
	}
	else if ( ComportMgr.VoiceDataBuffIsFull_2 != bTRUE )
	{
		memcpy(&ComportMgr.VoiceBuff_2[0],&VOICEBUFF[0],31);
		ComportMgr.VoiceDataBuffIsFull_2 = bTRUE;
		ComportMgr.PostPlayVoice_2 = 1;
		if ( ComportMgr.VoiceDataID_1 < 255 )
		{
			ComportMgr.VoiceDataID_2 = ComportMgr.VoiceDataID_1 + 1;
		}
		else
		{
			ComportMgr.VoiceDataID_2=0;
		}
	}
	else		//Voice data buff_1 and buff_2 are all full
	{
		if (((ComportMgr.VoiceDataID_1 == 255)&&(ComportMgr.VoiceDataID_2==0))
			||( ComportMgr.VoiceDataID_2  > ComportMgr.VoiceDataID_1 )
			)		//if VoiceBuff_1 is old, overwrite VoiceDataBuff_1
		{
			memcpy(&ComportMgr.VoiceBuff_1[0],&VOICEBUFF[0],31);
			ComportMgr.VoiceDataBuffIsFull_1 = bTRUE;
			ComportMgr.PostPlayVoice_1 = 1;
			if ( ComportMgr.VoiceDataID_2 < 255 )
			{
				ComportMgr.VoiceDataID_1 = ComportMgr.VoiceDataID_2 + 1;
			}
			else
			{
				ComportMgr.VoiceDataID_1=0;
			}
		}
		else if (((ComportMgr.VoiceDataID_2 == 255)&&(ComportMgr.VoiceDataID_1==0))
				||( ComportMgr.VoiceDataID_1  > ComportMgr.VoiceDataID_2 )
				)//if VoiceBuff_2 is old, overwrite VoiceDataBuff_2
		{
			memcpy(&ComportMgr.VoiceBuff_2[0],&VOICEBUFF[0],31);
			ComportMgr.VoiceDataBuffIsFull_2 = bTRUE;
			ComportMgr.PostPlayVoice_2 = 1;
			if ( ComportMgr.VoiceDataID_1 < 255 )
			{
				ComportMgr.VoiceDataID_2 = ComportMgr.VoiceDataID_1 + 1;
			}
			else
			{
				ComportMgr.VoiceDataID_2=0;
			}
		}
	}
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
	ComportMgr.PostPositivePowerOnWifi = 0x00;
	ComportMgr.PostPlayVoice_1=0;
	ComportMgr.VoiceDataBuffIsFull_1 = bFALSE;
	ComportMgr.PostPlayVoice_2=0;
	ComportMgr.VoiceDataBuffIsFull_2 = bFALSE;
	ComportMgr.TimeOutTimes = 0x00;
	ComportMgr.VoiceDataID_1 =  0x00;
	ComportMgr.VoiceDataID_2 =  0x00;
	
	WifiMgr.WifiOnlinePasswordVerifyMgr.WifiPowerIsOnForOnlinePassword = bFALSE;
	WifiMgr.WifiOnlinePasswordVerifyMgr.Status = OnlinePasswordVerifyInit;
		
	ComportMgr.DoorStatus = Standby;

	ComportMgr.Status = AckIdle;
}

void ComPort_Mgr_Task(void)
{

	ComportMgr.TimeCnt++;

	if (( ComportMgr.TimeCnt < 120 )
		&&( ComportMgr.TimeCnt%16==0)
		)
	{
		DEBUG_MARK;
		#ifdef Function_MainBoardWithoutVoicePlayer
		if (( VoiceDataTransferMgr.VoicePlayEnd == bFALSE )&&(ComportMgr.Status == AckIdle))
		{
			ComportMgr.PostGetVoicePlayerStatus=1;	
		}
		#endif
	}
	else if (ComportMgr.TimeCnt > 127)
	{
		ComportMgr.TimeCnt =0;
		if ( ComportMgr.Status == AckIdle )
		{
			ComportMgr.PostStatus=1;
		}
	}

	if ( (ComportMgr.PostGetVoicePlayerStatus > 0 )&&(ComportMgr.Status == AckIdle))
	{
		if ( ComPort_SendGetVoiceStatusCmd() == S_SUCCESS)
		{
			ComportMgr.PostGetVoicePlayerStatus--;
		}
	}
//	else if ( (ComportMgr.PostPlayVoice_1 > 0 )&&((ComportMgr.Status == AckIdle)||(ComportMgr.Status == WaitRemoteUnlcokAck)))
//	{	
//		if ( ComPort_SendPlayVoiceCmd(ComportMgr.VoiceBuff_1) == S_SUCCESS)
//		{
//			ComportMgr.PostPlayVoice_1--;
//			ComportMgr.VoiceDataBuffIsFull_1 = bFALSE;
//		}
//	}
//	else if ( (ComportMgr.PostPlayVoice_2 > 0 )&&((ComportMgr.Status == AckIdle)||(ComportMgr.Status == WaitRemoteUnlcokAck)))
//	{	
//		if ( ComPort_SendPlayVoiceCmd(ComportMgr.VoiceBuff_2) == S_SUCCESS)
//		{
//			ComportMgr.PostPlayVoice_2--;
//			ComportMgr.VoiceDataBuffIsFull_2 = bFALSE;
//		}
//	}
	else if ((ComportMgr.PostPlayVoice_1 > 0 )||(ComportMgr.PostPlayVoice_2 > 0 ))
	{
		if ((ComportMgr.PostPlayVoice_1 > 0 )&&(ComportMgr.PostPlayVoice_2 > 0 ))// if two voice buff are full, only post latest voice data
		{
			if (((ComportMgr.VoiceDataID_1 == 255)&&(ComportMgr.VoiceDataID_2==0))
				||( ComportMgr.VoiceDataID_2  > ComportMgr.VoiceDataID_1 )
				)
			{
				ComportMgr.PostPlayVoice_1 = 0;
				ComportMgr.VoiceDataBuffIsFull_1 = bFALSE;
			}
			else if (((ComportMgr.VoiceDataID_2 == 255)&&(ComportMgr.VoiceDataID_1==0))
					||( ComportMgr.VoiceDataID_1  > ComportMgr.VoiceDataID_2 )
					)
			{
				ComportMgr.PostPlayVoice_2 = 0;
				ComportMgr.VoiceDataBuffIsFull_2 = bFALSE;
			}
		}

		if ( (ComportMgr.PostPlayVoice_1 > 0 )&&(ComportMgr.Status == AckIdle))
		{	
			if ( ComPort_SendPlayVoiceCmd(ComportMgr.VoiceBuff_1) == S_SUCCESS)
			{
				ComportMgr.PostPlayVoice_1--;
				ComportMgr.VoiceDataBuffIsFull_1 = bFALSE;
			}
		}
		else if ( (ComportMgr.PostPlayVoice_2 > 0 )&&(ComportMgr.Status == AckIdle))
		{	
			if ( ComPort_SendPlayVoiceCmd(ComportMgr.VoiceBuff_2) == S_SUCCESS)
			{
				ComportMgr.PostPlayVoice_2--;
				ComportMgr.VoiceDataBuffIsFull_2 = bFALSE;
			}
		}
	}
	else if ((ComportMgr.PostParameter > 0 )&&(ComportMgr.Status == AckIdle))
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
	else if ( ( ComportMgr.PostExitWifiHostSpot > 0 )&&(ComportMgr.Status == AckIdle))
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
	else if ( (ComportMgr.PostExitRemoteUnlock > 0 )&&(ComportMgr.Status == AckIdle))
	{
		ComportMgr.PostExitRemoteUnlock--;
		ComPort_SendExitRemoteUnlockRequestCmd();
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
	else if ( (ComportMgr.PostPositivePowerOnWifi > 0 )&&(ComportMgr.Status == AckIdle))
	{
		ComportMgr.PostPositivePowerOnWifi--;
		ComPort_SendPositivePowerOnWifiCmd();
	}
	else if ( (ComportMgr.PostOnlinePasswordVerify > 0 )&&(ComportMgr.Status == AckIdle))
	{
		ComportMgr.PostOnlinePasswordVerify--;
		ComPort_SendOnlinePasswordVerifyCmd();
	}
	else if ( (ComportMgr.PostStatus > 0 )&&(ComportMgr.Status == AckIdle))
	{
		ComportMgr.PostStatus--;
		ComPort_SendStatusCmd();
	}

	
	ComPort_Cmd_Excute();
}

