#include "Project.h"
#include "IO.h"
#include "Hardware.h"
#include "StdTypes.h"
#include "ISR.h"
#include "usart.h"
#include "FaceRecoginitionMgr.h"

#define byte_syncH 	0x00
#define byte_syncL 	0x01
#define byte_cmd 	0x02
#define byte_lenthH 0x03
#define byte_lenthL 0x04


#ifdef Function_FaceRecoginition
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
#else

#ifndef Function_YouzhiyunjiaWifi
status_t UART3_Write_TxBUFFER(unsigned char *buff,unsigned char len)
{
	return S_SUCCESS;
}

void UART3_TX_Task(void)
{
}
void UART3_CLEAR_TXBUFF(void)
{
}
#endif

#endif

void FaceRecognition_GetStatus(void)
{
 	FrmMgr.SendCmdFlag_Status = bTRUE;
	FrmMgr.PostFlag_Status = bFALSE;
}

void FRM_Cmd_Excute(void)
{
	uint16_t i;
	uint8_t RxBuff[210];
	uint8_t CmdLenth,checksum;

	
	if ( UART3_Mgr.Status == ReceivingData )
	{
		if ( UART3_Mgr.DataStreamTimeCnt > 0 )
		{
			UART3_Mgr.DataStreamTimeCnt--;
		}
		else	//reset UART2 receiving status
		{
			UART3_Mgr.Status = Idle;
			UART3_Mgr.RX_DataPoint = 0x00;
			/*
			for ( i=0;i<100;i++ )
			{
				UART3_Mgr.RX_Buffer[i]=0x00;
			}
			*/
		}
	}
	
	if ( UART3_Mgr.Status != GotNewCmd ){
		return;
	}

	CmdLenth = UART3_Mgr.RX_Buffer[3]*256+UART3_Mgr.RX_Buffer[4]+6;

	if ( CmdLenth <210 )
	{
		for (i=0;i<CmdLenth;i++)
		{
			RxBuff[i] = UART3_Mgr.RX_Buffer[i];
		}
	}
	UART3_Mgr.RX_DataPoint = 0x00;
	UART3_Mgr.Status = Idle;	
	
	checksum = 0x00;
	for (i=2;i<CmdLenth-1;i++)
	{
		checksum^=RxBuff[i];
	}
	
	DEBUG_MARK;
	
	if ( checksum!=RxBuff[CmdLenth-1] )
	{
		DEBUG_MARK;

		return;		//if check sum is failed, ignore this data string
	}

	if ( RxBuff[byte_cmd] == 0x01 )		//MID NOTE
	{
		if ( RxBuff[5] == 0x00 )		//note ID is 0, FRM is Ready
		{
			FrmMgr.PostFlag_Ready = bTRUE;
		}
		else if ( RxBuff[5] == 0x01 )		//note ID is 1, Face sate
		{
			FrmMgr.PostFlag_FaceState = bTRUE;
			FrmMgr.FaceState = RxBuff[6];
		}
	}
	else if ( RxBuff[byte_cmd] == 0x00 )	//REPLY
	{
		if ( RxBuff[5] == 0x10 )			//Reset
		{
			FrmMgr.PostFlag_ResetResult = bTRUE;
			FrmMgr.ErrorType = RxBuff[6];
		}
		else if ( RxBuff[5] == 0x11 )		//Status
		{
			FrmMgr.PostFlag_Status = bTRUE;
			FrmMgr.ErrorType = RxBuff[6];
		}
		else if ( RxBuff[5] == 0x12 )		//VERIFY
		{
			FrmMgr.PostFlag_VerifyResult = bTRUE;
			FrmMgr.ErrorType = RxBuff[6];
			FrmMgr.UserID = RxBuff[7]*256 + RxBuff[8];
		}
		else if ( RxBuff[5] == 0x13 )		//Register multi times
		{
			FrmMgr.PostFlag_RegisterResult = bTRUE;

			FrmMgr.ErrorType = RxBuff[6];
			FrmMgr.UserID = RxBuff[7]*256 + RxBuff[8];
		}
		else if ( RxBuff[5] == 0x1D )		//Register single times
		{
			FrmMgr.PostFlag_RegisterResult = bTRUE;

			FrmMgr.ErrorType = RxBuff[6];
			FrmMgr.UserID = RxBuff[7]*256 + RxBuff[8];
		}	
		else if ( RxBuff[5] == 0x20 )		//delete user
		{
			FrmMgr.PostFlag_DeleteTemplateResult = bTRUE;
			FrmMgr.ErrorType = RxBuff[6];
		}

		else if ( RxBuff[5] == 0x21 )		//delete all
		{
			FrmMgr.PostFlag_DeleteAllResult = bTRUE;
			FrmMgr.ErrorType = RxBuff[6];
		}
		
		else if ( RxBuff[5] == 0x23 )		//face reset
		{
			FrmMgr.PostFlag_FaceResetResult = bTRUE;
			FrmMgr.ErrorType = RxBuff[6];
		}
		
		else if ( RxBuff[5] == 0x24 )		//get all user ID
		{
			FrmMgr.PostFlag_GetAllUserIdResult = bTRUE;
			FrmMgr.ErrorType = RxBuff[6];
		}
		else if ( RxBuff[5] == 0xED )		//PowerDown
		{
			FrmMgr.PostFlag_PownDown = bTRUE;
			FaceRecognition_HardwarePowerOff();
		}
		else if ( RxBuff[5] == 0xFE )		//Enable Demo Mode
		{
			if ( FaceIdentifyMgr.Status == WaitForFrmEnableDemoModeAck )
			{
				FrmMgr.PostFlag_EnableDemoResult = bTRUE;
			}
			if ( FaceIdentifyMgr.Status == WaitForFrmDisableDemoModeAck )
			{
				FrmMgr.PostFlag_DisableDemoResult = bTRUE;
			}	
		}
		else if ( RxBuff[5] == 0x30 )		//version number
		{
			FrmMgr.PostFlag_VersionNumber = bTRUE;
			DEBUG_MARK;
		}
	}
}

status_t  FRM_SendEnableDemoCmd(void)
{
	status_t ReturnValue;
	uint8_t BUFF[7];
	uint8_t i,CmdLenth,checksum;

	CmdLenth = 7;
	
	BUFF[0]=0xEF;
	BUFF[1]=0xAA;
	BUFF[2]=0xFE;
	BUFF[3]=0x00;
	BUFF[4]=0x01;
	BUFF[5]=0x01;
	checksum = 0x00;
	for (i=2;i<(CmdLenth-1);i++)
	{
		checksum^=BUFF[i];
	}
	BUFF[CmdLenth-1]=checksum;
	
	return (UART3_Write_TxBUFFER(BUFF,CmdLenth));
	
}

status_t  FRM_SendDisableDemoCmd(void)
{
	status_t ReturnValue;
	uint8_t BUFF[7];
	uint8_t i,CmdLenth,checksum;

	CmdLenth = 7;
	
	BUFF[0]=0xEF;
	BUFF[1]=0xAA;
	BUFF[2]=0xFE;
	BUFF[3]=0x00;
	BUFF[4]=0x01;
	BUFF[5]=0x00;
	checksum = 0x00;
	for (i=2;i<(CmdLenth-1);i++)
	{
		checksum^=BUFF[i];
	}
	BUFF[CmdLenth-1]=checksum;
	
	return (UART3_Write_TxBUFFER(BUFF,CmdLenth));
	
}


status_t FRM_SendRegisterMultiTimesCmd(void)
{
	status_t ReturnValue;
	uint8_t BUFF[41];
	uint8_t i,CmdLenth,checksum;
  	//uint8_t UserName[]={"JFDZ FPDL FACE USER 123456789012"};
  uint8_t FaceUserName[]={"Test"};

	for (i=0;i<41;i++)
 	{
		BUFF[i] = 0x00;
 	}

	CmdLenth =41;
	
	BUFF[0]=0xEF;
	BUFF[1]=0xAA;
	BUFF[2]=0x13;

	BUFF[3]=0x00;
	BUFF[4]=CmdLenth-6;

	BUFF[5] = FrmMgr.UserType;
	
	memcpy(&BUFF[6],FaceUserName,32);

	BUFF[38]=FrmMgr.FaceDirrect;
	BUFF[39]=FrmMgr.TimeLimited;

	checksum = 0x00;
	for (i=2;i<(CmdLenth-1);i++)
	{
		checksum^=BUFF[i];
	}
	BUFF[CmdLenth-1]=checksum;
	
	return (UART3_Write_TxBUFFER(BUFF,CmdLenth));
	
}


status_t FRM_SendRegisterSingleTimesCmd(void)
{
	status_t ReturnValue;
	uint8_t BUFF[41];
	uint8_t i,CmdLenth,checksum;
  uint8_t FaceUserName[]={"JFDZ FPDL FACE USER 123456789012"};
  //uint8_t FaceUserName[32]={"Test"};

	for (i=0;i<41;i++)
 	{
		BUFF[i] = 0x00;
 	}

	CmdLenth =41;
	
	BUFF[0]=0xEF;
	BUFF[1]=0xAA;
	BUFF[2]=0x1D;

	BUFF[3]=0x00;
	BUFF[4]=CmdLenth-6;

	BUFF[5] = FrmMgr.UserType;
	
	memcpy(&BUFF[6],FaceUserName,32);

	BUFF[38]=FrmMgr.FaceDirrect;
	BUFF[39]=FrmMgr.TimeLimited;

	checksum = 0x00;
	for (i=2;i<(CmdLenth-1);i++)
	{
		checksum^=BUFF[i];
	}
	BUFF[CmdLenth-1]=checksum;
	
	return (UART3_Write_TxBUFFER(BUFF,CmdLenth));
	
}


status_t FRM_SendVerifyCmd(void)
{
	status_t ReturnValue;
	uint8_t BUFF[8];
	uint8_t i,CmdLenth,checksum;

	CmdLenth =8;
	
	BUFF[0]=0xEF;
	BUFF[1]=0xAA;
	BUFF[2]=0x12;

	BUFF[3]=0x00;
	BUFF[4]=CmdLenth-6;

	BUFF[5] = 0x00;		//power down rightaway: 1=true,0=false

	BUFF[6]=FrmMgr.TimeLimited;

	checksum = 0x00;
	for (i=2;i<(CmdLenth-1);i++)
	{
		checksum^=BUFF[i];
	}
	BUFF[CmdLenth-1]=checksum;
	
	return (UART3_Write_TxBUFFER(BUFF,CmdLenth));
	
}

status_t FRM_SendDeleteUserCmd(void)
{
	status_t ReturnValue;
	uint8_t BUFF[8];
	uint8_t i,CmdLenth,checksum;

	CmdLenth =8;
	
	BUFF[0]=0xEF;
	BUFF[1]=0xAA;
	BUFF[2]=0x20;

	BUFF[3]=0x00;
	BUFF[4]=CmdLenth-6;

	BUFF[5] = FrmMgr.UserID/256;

	BUFF[6] = FrmMgr.UserID%256;

	checksum = 0x00;
	for (i=2;i<(CmdLenth-1);i++)
	{
		checksum^=BUFF[i];
	}
	BUFF[CmdLenth-1]=checksum;
	
	return (UART3_Write_TxBUFFER(BUFF,CmdLenth));
	
}

status_t FRM_SendDeleteAllCmd(void)
{
	status_t ReturnValue;
	uint8_t BUFF[6];
	uint8_t i,CmdLenth,checksum;

	CmdLenth =6;
	
	BUFF[0]=0xEF;
	BUFF[1]=0xAA;
	BUFF[2]=0x21;

	BUFF[3]=0x00;
	BUFF[4]=CmdLenth-6;

	checksum = 0x00;
	for (i=2;i<(CmdLenth-1);i++)
	{
		checksum^=BUFF[i];
	}
	BUFF[CmdLenth-1]=checksum;
	
	return (UART3_Write_TxBUFFER(BUFF,CmdLenth));
	
}

status_t FRM_SendGetAllUserIdCmd(void)
{
	status_t ReturnValue;
	uint8_t BUFF[6];
	uint8_t i,CmdLenth,checksum;

	CmdLenth =6;
	
	BUFF[0]=0xEF;
	BUFF[1]=0xAA;
	BUFF[2]=0x24;

	BUFF[3]=0x00;
	BUFF[4]=CmdLenth-6;

	checksum = 0x00;
	for (i=2;i<(CmdLenth-1);i++)
	{
		checksum^=BUFF[i];
	}
	BUFF[CmdLenth-1]=checksum;
	
	return (UART3_Write_TxBUFFER(BUFF,CmdLenth));
	
}

status_t FRM_SendResetCmd(void)
{
	status_t ReturnValue;
	uint8_t BUFF[6];
	uint8_t i,CmdLenth,checksum;

	CmdLenth =6;
	
	BUFF[0]=0xEF;
	BUFF[1]=0xAA;
	BUFF[2]=0x10;

	BUFF[3]=0x00;
	BUFF[4]=CmdLenth-6;

	checksum = 0x00;
	for (i=2;i<(CmdLenth-1);i++)
	{
		checksum^=BUFF[i];
	}
	BUFF[CmdLenth-1]=checksum;
	
	return (UART3_Write_TxBUFFER(BUFF,CmdLenth));
	
}

status_t FRM_SendGetStatusCmd(void)
{
	status_t ReturnValue;
	uint8_t BUFF[6];
	uint8_t i,CmdLenth,checksum;

	CmdLenth =6;
	
	BUFF[0]=0xEF;
	BUFF[1]=0xAA;
	BUFF[2]=0x11;

	BUFF[3]=0x00;
	BUFF[4]=CmdLenth-6;

	checksum = 0x00;
	for (i=2;i<(CmdLenth-1);i++)
	{
		checksum^=BUFF[i];
	}
	BUFF[CmdLenth-1]=checksum;
	
	return (UART3_Write_TxBUFFER(BUFF,CmdLenth));
	
}


status_t FRM_SendFaceResetCmd(void)
{
	status_t ReturnValue;
	uint8_t BUFF[6];
	uint8_t i,CmdLenth,checksum;

	CmdLenth =6;
	
	BUFF[0]=0xEF;
	BUFF[1]=0xAA;
	BUFF[2]=0x23;

	BUFF[3]=0x00;
	BUFF[4]=CmdLenth-6;

	checksum = 0x00;
	for (i=2;i<(CmdLenth-1);i++)
	{
		checksum^=BUFF[i];
	}
	BUFF[CmdLenth-1]=checksum;
	
	return (UART3_Write_TxBUFFER(BUFF,CmdLenth));
	
}

status_t FRM_SendPownDownCmd(void)
{
	status_t ReturnValue;
	uint8_t BUFF[6];
	uint8_t i,CmdLenth,checksum;

	CmdLenth =6;
	
	BUFF[0]=0xEF;
	BUFF[1]=0xAA;
	BUFF[2]=0xED;

	BUFF[3]=0x00;
	BUFF[4]=CmdLenth-6;

	checksum = 0x00;
	for (i=2;i<(CmdLenth-1);i++)
	{
		checksum^=BUFF[i];
	}
	BUFF[CmdLenth-1]=checksum;
	
	return (UART3_Write_TxBUFFER(BUFF,CmdLenth));
	
}

status_t FRM_SendGetVersionCmd(void)
{
	status_t ReturnValue;
	uint8_t BUFF[6];
	uint8_t i,CmdLenth,checksum;

	CmdLenth =6;
	
	BUFF[0]=0xEF;
	BUFF[1]=0xAA;
	BUFF[2]=0x30;

	BUFF[3]=0x00;
	BUFF[4]=CmdLenth-6;

	checksum = 0x00;
	for (i=2;i<(CmdLenth-1);i++)
	{
		checksum^=BUFF[i];
	}
	BUFF[CmdLenth-1]=checksum;
	
	return (UART3_Write_TxBUFFER(BUFF,CmdLenth));
	
}

void FaceRecognition_EnableDemoMode(void)
{
	FrmMgr.SendCmdFlag_EnableDemo = bTRUE;
	FrmMgr.PostFlag_EnableDemoResult = bFALSE;
}
void FaceRecognition_DisableDemoMode(void)
{
	FrmMgr.SendCmdFlag_DisableDemo = bTRUE;
	FrmMgr.PostFlag_DisableDemoResult = bFALSE;
}

void FaceRecognition_RegisterTemplateMultiTimesStart(FaceDirrect_t facedirrect)
{
	FrmMgr.FaceDirrect = facedirrect;
	FrmMgr.TimeLimited = 20;
	FrmMgr.SendCmdFlag_RegisterMultiTimes = bTRUE;
	
	FrmMgr.PostFlag_RegisterResult = bFALSE;
	FrmMgr.PostFlag_FaceState = bFALSE;
}
void FaceRecognition_RegisterTemplateSingleTimesStart(void)
{
	FrmMgr.FaceDirrect = 0x00;
	FrmMgr.TimeLimited = 20;
	FrmMgr.SendCmdFlag_RegisterSingleTimes = bTRUE;
	
	FrmMgr.PostFlag_RegisterResult = bFALSE;
	FrmMgr.PostFlag_FaceState = bFALSE;
}

void FaceRecognition_VerifyStart(void)
{
	FrmMgr.SendCmdFlag_Verify = bTRUE;
	FrmMgr.PostFlag_VerifyResult = bFALSE;
	FrmMgr.PostFlag_FaceState = bFALSE;
	FrmMgr.TimeLimited = 10;
}

void FaceRecognition_DeleteTemplateStart(uint16_t UserID)
{
	FrmMgr.SendCmdFlag_DeleteUser = bTRUE;
	FrmMgr.UserID = UserID;
	FrmMgr.PostFlag_DeleteTemplateResult = bFALSE;
}

void FaceRecognition_DeleteAllTemplateStart(void)
{
	FrmMgr.SendCmdFlag_DeleteAll = bTRUE;
	FrmMgr.PostFlag_DeleteAllResult = bFALSE;
}

void FaceRecognition_UpdateUserIdList(void)
{
 	FrmMgr.SendCmdFlag_GetAllUserID = bTRUE;
	FrmMgr.PostFlag_GetAllUserIdResult = bFALSE;
}

void FaceRecognition_FaceReset(void)
{
 	FrmMgr.SendCmdFlag_FaceReset = bTRUE;
	FrmMgr.PostFlag_FaceResetResult = bFALSE;
}

void FaceRecognition_Reset(void)
{
 	FrmMgr.SendCmdFlag_Reset = bTRUE;
	FrmMgr.PostFlag_ResetResult = bFALSE;
}
void FaceRecognition_PowerDown(void)
{
 	FrmMgr.SendCmdFlag_PownDown = bTRUE;
	FrmMgr.PostFlag_PownDown = bFALSE;
}

void FaceRecognition_GetVerionNumber(void)
{
 	FrmMgr.SendCmdFlag_GetVersion = bTRUE;
	FrmMgr.PostFlag_VersionNumber = bFALSE;
}

void FaceRecognition_HardwarePowerOn(void)
{
	SET_FRMPOWER_ON;
	FrmMgr.PowerStatus = FRM_PowerOn;
	FrmMgr.PostFlag_Ready = bFALSE;
	MX_USART3_UART_Init();
}
void FaceRecognition_HardwarePowerOff(void)
{
	HAL_UART_DeInit(&huart3);
	SET_FRMPOWER_OFF;
	FrmMgr.PowerStatus = FRM_PowerOff;
	FaceRecognitionMgr_Init();
	FaceIdentifyMgr.Status = FrmIdentifyStart;
}


void FaceRecognitionMgr_Init(void)
{
	UART3_Mgr.TxPoint = 0;
	UART3_Mgr.TxLength = 0;
	UART3_Mgr.Status = Idle;

	FrmMgr.SendCmdFlag_RegisterMultiTimes =bFALSE;
	FrmMgr.SendCmdFlag_RegisterSingleTimes =bFALSE;
	FrmMgr.SendCmdFlag_Verify =bFALSE;
	FrmMgr.SendCmdFlag_DeleteUser =bFALSE;
	FrmMgr.SendCmdFlag_DeleteAll =bFALSE;
	FrmMgr.SendCmdFlag_GetAllUserID =bFALSE;
	FrmMgr.SendCmdFlag_PownDown =bFALSE;
	FrmMgr.SendCmdFlag_FaceReset =bFALSE;
	FrmMgr.SendCmdFlag_Reset =bFALSE;
	FrmMgr.SendCmdFlag_Status =bFALSE;
	FrmMgr.SendCmdFlag_EnableDemo =bFALSE;
	FrmMgr.SendCmdFlag_GetVersion =bFALSE;
 
	FrmMgr.PostFlag_FaceState =bFALSE;
	FrmMgr.PostFlag_Ready =bFALSE;
	FrmMgr.PostFlag_Status =bFALSE;
	FrmMgr.PostFlag_ResetResult =bFALSE;
	FrmMgr.PostFlag_FaceResetResult =bFALSE;
	FrmMgr.PostFlag_VerifyResult =bFALSE;
	FrmMgr.PostFlag_RegisterResult =bFALSE;
	FrmMgr.PostFlag_DeleteTemplateResult =bFALSE;
	FrmMgr.PostFlag_DeleteAllResult =bFALSE;
	FrmMgr.PostFlag_GetAllUserIdResult =bFALSE;
	FrmMgr.PostFlag_EnableDemoResult =bFALSE;
	FrmMgr.PostFlag_VersionNumber =bFALSE;
	FrmMgr.PostFlag_PownDown = bFALSE;
	
}

void FaceRecognitionMgr_Task(void)
{

	FRM_Cmd_Excute();

	 if ( FrmMgr.SendCmdFlag_Reset == bTRUE )
	{
		if ( FRM_SendResetCmd() == S_SUCCESS)
		{
			FrmMgr.SendCmdFlag_Reset = bFALSE;
		}
	}
	else if ( FrmMgr.SendCmdFlag_Status == bTRUE )
	{
		if ( FRM_SendGetStatusCmd() == S_SUCCESS)
		{
			FrmMgr.SendCmdFlag_Status = bFALSE;
		}
	}
	else if ( FrmMgr.SendCmdFlag_RegisterMultiTimes == bTRUE )
	{
		if ( FRM_SendRegisterMultiTimesCmd() == S_SUCCESS)
		{
			FrmMgr.SendCmdFlag_RegisterMultiTimes = bFALSE;
		}
	}
	else if ( FrmMgr.SendCmdFlag_RegisterSingleTimes == bTRUE )
	{
		if ( FRM_SendRegisterSingleTimesCmd() == S_SUCCESS)
		{
			FrmMgr.SendCmdFlag_RegisterSingleTimes = bFALSE;
		}
	}
	else if ( FrmMgr.SendCmdFlag_Verify == bTRUE )
	{
		if ( FRM_SendVerifyCmd() == S_SUCCESS)
		{
			FrmMgr.SendCmdFlag_Verify = bFALSE;
		}
	}
	else if ( FrmMgr.SendCmdFlag_GetAllUserID == bTRUE )
	{
		if ( FRM_SendGetAllUserIdCmd() == S_SUCCESS)
		{
			FrmMgr.SendCmdFlag_GetAllUserID = bFALSE;
		}
	}
	else if ( FrmMgr.SendCmdFlag_DeleteUser == bTRUE )
	{
		if ( FRM_SendDeleteUserCmd() == S_SUCCESS)
		{
			FrmMgr.SendCmdFlag_DeleteUser = bFALSE;
		}
	}
	else if ( FrmMgr.SendCmdFlag_DeleteAll == bTRUE )
	{
		if ( FRM_SendDeleteAllCmd() == S_SUCCESS)
		{
			FrmMgr.SendCmdFlag_DeleteAll = bFALSE;
		}
	}
	else if ( FrmMgr.SendCmdFlag_FaceReset == bTRUE )
	{
		if ( FRM_SendFaceResetCmd() == S_SUCCESS)
		{
			FrmMgr.SendCmdFlag_FaceReset = bFALSE;
		}
	}
	else if ( FrmMgr.SendCmdFlag_EnableDemo == bTRUE )
	{
		if ( FRM_SendEnableDemoCmd() == S_SUCCESS)
		{
			FrmMgr.SendCmdFlag_EnableDemo = bFALSE;
		}
	}
	else if ( FrmMgr.SendCmdFlag_DisableDemo == bTRUE )
	{
		if ( FRM_SendDisableDemoCmd() == S_SUCCESS)
		{
			FrmMgr.SendCmdFlag_DisableDemo = bFALSE;
		}
	}
	else if ( FrmMgr.SendCmdFlag_GetVersion == bTRUE )
	{
		if ( FRM_SendGetVersionCmd() == S_SUCCESS)
		{
			FrmMgr.SendCmdFlag_GetVersion = bFALSE;
		}
	}
	else if ( FrmMgr.SendCmdFlag_PownDown == bTRUE )
	{
		if ( FRM_SendPownDownCmd() == S_SUCCESS)
		{
			FrmMgr.SendCmdFlag_PownDown = bFALSE;
		}
	}

}


