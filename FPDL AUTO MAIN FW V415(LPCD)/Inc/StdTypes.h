
#ifndef STDTYPES_H
#define STDTYPES_H
//* ********************** FILE HEADER *************************************

//* *************************************************************************
//*  BASIC TYPE DEFINTIONS
//*
typedef unsigned char 	  uint8_t;
//typedef unsigned int      uint16_t;
//typedef unsigned long int uint32_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;

typedef signed   char     sint8_t;
typedef signed int        sint16_t;
typedef signed long int   sint32_t;

typedef enum {bFALSE=0x00, bTRUE=0xFF} bool_t;
typedef enum {S_FAIL=0x00, S_SUCCESS=0xFF} status_t;
typedef enum {YES=0, NO=1} selection_t;

//+++++++++++++++++++++++++++ Bit Computing ++++++++++++++++++++++++++++++++++//
//SET BIT.    Example: a |= SETBIT0
enum
{
	SETBIT0 = 0x01,  SETBIT1 = 0x02,	SETBIT2 = 0x04,	 SETBIT3 = 0x08,
	SETBIT4 = 0x10,	 SETBIT5 = 0x20,	SETBIT6 = 0x40,	 SETBIT7 = 0x80,
	SETBIT8 = 0x0100,	 SETBIT9 = 0x0200,	SETBIT10 = 0x0400, SETBIT11 = 0x0800,
	SETBIT12 = 0x1000, SETBIT13 = 0x2000,	SETBIT14 = 0x4000, SETBIT15 = 0x8000		
};
//CLR BIT.    Example: a &= CLRBIT0
enum
{
	CLRBIT0 = 0xFFFE,  CLRBIT1 = 0xFFFD,	CLRBIT2 = 0xFFFB,	 CLRBIT3 = 0xFFF7,	
	CLRBIT4 = 0xFFEF,	 CLRBIT5 = 0xFFDF,	CLRBIT6 = 0xFFBF,	 CLRBIT7 = 0xFF7F,
	CLRBIT8 = 0xFEFF,	 CLRBIT9 = 0xFDFF,	CLRBIT10 = 0xFBFF, CLRBIT11 = 0xF7FF,
	CLRBIT12 = 0xEFFF, CLRBIT13 = 0xDFFF,	CLRBIT14 = 0xBFFF, CLRBIT15 = 0x7FFF
};
//CHOSE BIT.  Example: a = b&CHSBIT0
enum
{
	CHSBIT0 = 0x0001,  CHSBIT1 = 0x0002,	CHSBIT2 = 0x0004,	 CHSBIT3 = 0x0008,
	CHSBIT4 = 0x0010,	 CHSBIT5 = 0x0020,	CHSBIT6 = 0x0040,	 CHSBIT7 = 0x0080,
	CHSBIT8 = 0x0100,	 CHSBIT9 = 0x0200,	CHSBIT10 = 0x0400, CHSBIT11 = 0x0800,
	CHSBIT12 = 0x1000, CHSBIT13 = 0x2000,	CHSBIT14 = 0x4000, CHSBIT15 = 0x8000		
};

//+++++++++++++++++++++++++++++ Others ++++++++++++++++++++++++++++++++++++//
//TAST RUN STEPS.
enum
{
	STEP0 = 0,         STEP1 = 1,         STEP2 = 2,         STEP3 = 3,
	STEP4 = 4,         STEP5 = 5,         STEP6 = 6,         STEP7 = 7,
	STEP8 = 8,         STEP9 = 9,         STEP10 = 10,       STEP11 = 11,
	STEP12 = 12,       STEP13 = 13,       STEP14 = 14,       STEP15 = 15,
	STEP16 = 16,       STEP17 = 17,       STEP18 = 18,       STEP19 = 19,
	STEP20 = 20,       STEP21 = 21,       STEP22 = 22,       STEP23 = 23,
	STEP24 = 24,       STEP25 = 25,       STEP26 = 26,       STEP27 = 27,
	STEP28 = 28,       STEP29 = 29,       STEP30 = 30,       STEP31 = 31,
	STEP32 = 32,       STEP33 = 33,       STEP34 = 34,       STEP35 = 35
};

typedef struct
{
	uint8_t second;	///
	uint8_t minute;
	uint8_t hour;	
	uint8_t date;
	uint8_t day;
	uint8_t month;
	uint8_t year;
	uint8_t SavedDateWhileNetWorkTimeUpdated;
}systemtime_t;

typedef enum uint8_t
{
	Undefined=0,
	Master=1,
	User=2
}UserPriority_t;

typedef enum
{
	FPUSER = 0,
	PASSCODEUSER=1,
	CARDUSER,
	FACEUSER,
	REMOTEGUEST,
	ONLINEPASSCODEUSER,
	UNDEFINEDUSER,
	AGINGTESTUSER=0XFF
}UserType_t;

typedef enum
{
	KEY_ZERO=0,
	KEY_ONE=1,
	KEY_TWO,
	KEY_THREE,
	KEY_FOUR,
	KEY_FIVE,    
	KEY_SIX,
	KEY_SEVEN,
	KEY_EIGHT,
	KEY_NINE,
	KEY_ASTERISK,
	KEY_POUNDSIGN,
	KEY_INSIDEBUTTON,
	KEY_SLIDETOUCHKEY,
	KEY_DOORBELL,
	KEY_ONE_HOLD,
	KEY_TWO_HOLD,
	KEY_THREE_HOLD,
	KEY_FOUR_HOLD,
	KEY_FIVE_HOLD,   
	KEY_SIX_HOLD,
	KEY_SEVEN_HOLD,
	KEY_EIGHT_HOLD,
	KEY_NIGHT_HOLD,
	KEY_ZERO_HOLD,
	KEY_ASTERISK_HOLD,
	KEY_POUNDSIGN_HOLD,
	KEY_DOORCLOSE,
	KEY_INSIDEBUTTON_HOLD_LONG,
	KEY_NONE=255
}keycode_t;

typedef enum
{
	SCREEN_Main=0,
	SCREEN_MainMenu=1,
	SCREEN_UserManagementMenu,
	SCREEN_FaceMenu,
	SCREEN_UserFaceMenu,
	SCREEN_MasterFaceMenu,
	SCREEN_RegisterMasterFace,
	SCREEN_DeleteMasterFace,
	SCREEN_RegisterUserFace,
	SCREEN_DeleteUserFace,
	SCREEN_DeleteAllUserFace,
	SCREEN_FpMenu,
	SCREEN_UserFpMenu,
	SCREEN_StressUserFpMenu,
	SCREEN_MasterFpMenu,
	SCREEN_CardUserMenu,
	SCREEN_PasscodeMenu,
	SCREEN_PasscodeUserMenu,
	SCREEN_PasscodeMasterMenu,
	SCREEN_EventLogMenu,
	SCREEN_NetWorkLink,
	SCREEN_SystemConfigMenu,
	SCREEN_AutoMotorSettingMenu,
	SCREEN_RegisterUserFp,
	SCREEN_FpDeleteMenu,
	SCREEN_DeleteUserFp,
	SCREEN_DeleteAllUserFp,
	SCREEN_RegisterStressUserFp,
	SCREEN_DeleteStressUserFp,
	SCREEN_DeleteAllStressUserFp,
	SCREEN_RegisterMasterFp,
	SCREEN_DeleteMasterFp,
	SCREEN_DeleteAllMasterFp,	
	SCREEN_RegisterCardUser,
	SCREEN_DeleteCardUser,
	SCREEN_DeleteAllCardUser,
	SCREEN_RegisterPasscodeUser,
	SCREEN_DeletePasscodeUser,
	SCREEN_DeleteAllPasscodeUser,
	SCREEN_TimeSetting,
	SCREEN_ManagerPasscodeModify,
	SCREEN_RestoreFactoryDefault,
	SCREEN_VoiceSettingMenu,
	SCREEN_LanguageSetting,
	SCREEN_PickAlarmEnableSetting,
	SCREEN_VolumeSetting,
	SCREEN_MemoryUsage,
	SCREEN_ManagerIdentify,
	SCREEN_Initialization,
	SCREEN_LowBattery,
	SCREEN_IdentifySuccess,
	SCREEN_IdentifyFail,
	SCREEN_SelfTest,
	SCREEN_AgingTest,
	SCREEN_SystemLocked,
	SCREEN_UnlockingModeSetting,
	SCREEN_InfoInquiryMenu,
	SCREEN_CheckEventLogBySequence,
	SCREEN_CheckEventLogByDate,
	SCREEN_DeleteEventLog,
	SCREEN_PickLockAlarm,
	SCREEN_ContinueOpenSetting,
	SCREEN_DoorLockSettingMenu,
	SCREEN_BodyInductionSetting,
	SCREEN_SystemVersion,
	SCREEN_RemoteUnlockRequest,
	SCREEN_EngineeringModeMneu,
    SCREEN_AutoMotorUnlockTime,
    SCREEN_AutoMotorAutoLockTime,
    SCREEN_AutoMotorLockDirection,
    SCREEN_AutoMotorTorque,
    SCREEN_AutoMotorBoltLockTime,
    SCREEN_LockingTravel,
    SCREEN_AutoEject,
    SCREEN_AutoMotorSelfTest,
    SCREEN_WifiMFT,
    SCREEN_ErrorMessage,
	SCREEN_ManagerMenu,
	SCREEN_UserMenu,
	SCREEN_DeleteUserMenu,
	SCREEN_RegisterManagerFPC,
	SCREEN_RegisterManagerFace,
	SCREEN_RegisterNormalUserFPC,
	SCREEN_RegisterNormalUserFace,
	SCREEN_NumberDelete,
	SCREEN_AllDelete,
	SCREEN_OperateSuccess,
	SCREEN_AllDeleteSuccess
}screen_t;	


typedef struct
{
	uint8_t T1024Hz;	   
	uint8_t T256Hz;    /*                            */
	uint8_t T128Hz;    /*                            */
	uint8_t T64Hz;     /*                            */
	uint8_t T16Hz;     /*                            */
	uint8_t T8Hz;
	uint8_t T2Hz;
	uint8_t T1Hz;     /*                            */
}Timebase_t;

typedef	 enum 
{
	Idle =0,
	ReceivingData=1,
	GotNewCmd
}UART_RX_Status_t;

typedef struct
{
	uint8_t TxBUFFER[40];
	uint8_t TxPoint;
	uint8_t TxLength;
	
	uint8_t RX_Buffer[100];
	uint8_t RX_DataPoint;
	uint8_t DataStreamTimeCnt;
	UART_RX_Status_t Status;
}UartMgr_t;

typedef struct
{
	uint8_t TxBUFFER[100];
	uint8_t TxPoint;
	uint8_t TxLength;
	
	uint8_t RX_Buffer[220];
	uint8_t RX_DataPoint;
	uint8_t DataStreamTimeCnt;
	
	UART_RX_Status_t Status;
}Uart3Mgr_t;


typedef enum
{
	Error_NONE=0,						//		00H ：表示指令执行完毕或 
	Error_Fail=1,						//		01H ：表示数据包接收错误； 
	Error_NoFinger, 					//		02H ：表示传感器上没有手指；
	Error_GetImage, 					//		03H ：表示录入指纹图像失败；
	Error_FingerTooDry, 				//		04H ：表示指纹图像太干、淡而生不成特征；
	Error_FingerTooDamp,				//		05H ：表示指纹图像太湿、糊而生不成特征；
	Error_GenChar,						//		06H ：表示指纹图像太乱而生不成特征；
	Error_GenChar1, 					//		07H ：表示指纹图像正常，但特征点太少（或面积小）而生不成；
	Error_NotMatch, 					//		08H ：表示指纹不匹配
	Error_UnRegistered, 				//		09H ：表示 没搜索到指纹；
	Error_Combine,						//		0aH ：表示特征合并失败；
	Error_FpAddrOutRange,				//		0bH ：表示访问指纹库时地址序号超出范围；
	Error_ReadTemplate, 				//		0cH ：表示从指纹库读模板出错或无效；
	Error_UploadChar,					//		0dH ：表示上传特征失败； 
	Error_GetData,						//		0eH ：表示模块不能接后续数据包
	Error_UploadImage,					//		0fH ：表示上传图像失败
	Error_DeleteTemplate,				//		10H ：表示删除模板失败；
	Error_DeleteAll,					//		11H ：表示清空指纹库失败；
	Error_GoToSleep,					//		12H ：表示不能进入低功耗状态；
	Error_Passcode, 					//		13H ：表示口令不正确；
	Error_SystemReset,					//		14H ：表示系统复位失败；
	Error_BufferData,					//		15H ：表示缓冲区内没有效原始图而生不成像；
	Error_OnlineUpdate, 				//		16H ：表示在线升级失败；
	Error_FingerNotRelease, 			//		17H ：表示残留指纹或两次采集之间手没有移动过；
	Error_RWFlash,						//		18H ：表示读写 表示读写FLASH出错；
	Error_GenRandomNumber,				//		19H ：随机数生成失败
	Error_RegisterInvalid,				//		1aH ：无效寄存器号；
	Error_ValueOfRegister,				//		1bH ：寄存器设定内容错误号；
	Error_NotePage, 					//		1cH ：记事本页码指定错误；
	Error_PortOperation,				//		1dH ：端口操作失败；
	Error_AutoEnroll,					//		1eH ：自动注册（enroll）失败；
	Error_MemoryIsFull, 				//		1fH ：指纹库满；
	Error_DeviceAddress,				//		20H ：设备地址错误
	Error_Password, 					//		21H ：密码有误； 
	Error_TemplateIsNotEmpty,			//		22H ：指纹模板非空； 
	Error_TemplateIsEmpty,				//		23H ：指纹模板为空；
	Error_FpMemoryIsEmpty,				//		24H ：指纹库为空； 
	Error_EnrollTimes,					//		25H ：录入次数设置错误；
	Error_TimeOut,						//		26H ：超时；
	Error_TemplateIsRegistered, 		//		27H ：指纹已存在
	Error_TemplateIsUnion,				//		28H ：指纹模板有关联
	Error_Sensor,						//		29H ：传感器初始化失败；
	Error_Reserve,						//		2AH ：Reserved
	Error_SerialNumberMismatched,
	Error_DataPackageF0=0xF0,				//		f0H ：有后续数据包的指令，正确接收用0xf0应答
	Error_DataPackageF1=0xF1,				//		f1H ：有后续数据包的指令，命令包用0xf1应答；
	Error_FlashChecksum=0xF2,			//		f2H ：表示烧写内部FLASH时，校验和错误
	Error_FlashPackageHead=0xF3,		//		f3H ：表示烧写内部FLASH时，包标识错误
	Error_FlashPackageLength=0xF4,		//		f4H ：表示烧写内部FLASH时，包长度错误
	Error_FlashHexFileTooLong=0xF5, 	//		f5H ：表示烧写内部FLASH时，代码长度太长
	Error_FlashWriteFail=0xF6,			//		f6H ：表示烧写内部FLASH时，烧写时，烧写FLASH失败 ；
	Error_BadImage=0xF9,			//		f9H ：采集到的图像不清晰，魔力FPC指纹头
	Error_CheckSum
}FPMcmdErrorType_t;
	
typedef enum
{
		FPMcmdStart=0,
		SendFirstGetImageCmd,
		WaitForFirstGetImageCmdACK,
		SendFirstGenCharCmd,
		WaitForFirstGenCharCmdACK,
		SendDetectFingerFirstTimeRemoveCmd,
		WaitForDetectFingerFirstTimeRemoveCmdACK,
		SendSecondGetImageCmd,
		WaitForSecondGetImageCmdACK,
		SendSecondGenCharCmd,
		WaitForSecondGenCharCmdACK,
		SendDetectFingerSecondTimeRemoveCmd,
		WaitForDetectFingerSecondTimeRemoveCmdACK,
		SendThirdGetImageCmd,
		WaitForThirdGetImageCmdACK,
		SendThirdGenCharCmd,
		WaitForThirdGenCharCmdACK,
		SendDetectFingerThirdTimeRemoveCmd,
		WaitForDetectFingerThirdTimeRemoveCmdACK,
		SendFourthGetImageCmd,
		WaitForFourthGetImageCmdACK,
		SendFourthGenCharCmd,
		WaitForFourthGenCharCmdACK,		
		SendRegModelCmd,
		WaitForRegModelCmdACK,
		SendStoreCharCmd,
		WaitForStoreCharCmdACK,
		//StartFpUserDelete,		
		SendDeleteCharCmd,
		WaitForDeleteCharCmdACK,
		StartFpUserIdentify,
		SendGetImageCmd,
		WaitForGetImageCmdACK,
		SendGenCharCmd,
		WaitForGenCharCmdACK,
		SendDetectFingerRemoveCmd,
		WaitForDetectFingerRemoveCmdACK,
		SendSearchCmd,
		WaitForSearchCmdACK,
		SendGetSerialNumberCmd,
		WaitForGetSerialNumberCmdACK,
		//StartAllUserFpDelete,
//		SendDeleteAllUserFpCmd,
//		WaitForDeleteAllUserFpCmdACK,
		success,
		fail
}FPMcmdStatus_t;


typedef enum
{
	
	UserUnregistered=0,
	MasterMismatched=1,
	FPMserialNumberMismatched
}UserIdentifyErrorType_t;

typedef	enum
{
	WaitACK=0,
	GotACK=1
}ACK_Status_t;

typedef struct
{
	uint16_t Para1;
	uint16_t Para2;
	uint8_t Buff[100];
	uint8_t TimeCnt;
	uint16_t TimeOutCnt;
	ACK_Status_t Status;
	enum
	{
		GetImageCmd=0,
		GenCharCmd=1,
		RegModelCmd,
		StoreCharCmd
	}Type;
	FPMcmdErrorType_t ErrorCode;
}FpmAckMgr_t;


typedef enum
{
	ErrorNone=0,
	TimeOut=1,
	UserIDisRegistered,
	UserIDisNotRegistered,
	PasscodeIsBeUsed,
	CardCIDisBeUsed,
	MemoryIsFull,
	SystemNoMaster,
	ManagerHasLogin,
	TwoPasscodesDoNotMatch,
	FingerPrintIsRegistered,
	FpRegisterFail,
	FaceRegisterFail,
	RepeatFace,
	QUIT
}UserMgrErrorType_t;


		
typedef struct
{
	uint16_t UserID;
	uint16_t TimeCnt;
	uint8_t EnrollSuccessTimes;
	uint8_t EnrollFailTimes;
	bool_t DuplicateCheck;
	FPMcmdStatus_t Status;
	UserMgrErrorType_t ErrorType;
}FpRegisterMgr_t;


typedef enum
{
	StartFpUserRegister=0,
	InputUserID=1,	
	ReportFpUserID,
	CheckIfFpUserIDisBeUsed,
	AddUserToFPM,
	DeleteUserFromFPM,
	WaitForDeleteUserACKfromFPM,
	DeleteAllUserFromFPM,
	WaitForDeleteAllUserACKfromFPM,
	WaitForUserConfirmDeleteAllFP,
	SendDeleteAllUserFpCmdToFPM,
	WaitForDeleteAllUserFpCmdACKfromFPM,
	RegisterUserSuccess,
	RegisterUserFail,
	DeleteUserSuccess,
	DeleteUserFail,	
	StartFpIdentify,
	StartFpUserDelete,
	StartAllUserFpDelete,
	DeleteAllFpUserSuccess,
	DeleteAllFpUserFail,
	EXIT
}FpUserMgrStatus_t;

typedef enum
{
	FpMaster=0,		//管理员指纹
	FpUser=1,		//普通指纹
	StressFpUser	//胁迫指纹
}FpUserType_t;
	
typedef struct
{
	FpUserType_t FpUserType;
	uint16_t UserID;
	uint16_t UserID_Mini;
	uint16_t UserID_Max;
	uint16_t TimeCnt;
	FpUserMgrStatus_t Status;
	UserMgrErrorType_t ErrorType;
}FpUserRegisterMgr_t;

typedef struct
{
	FpUserType_t FpUserType;
	uint16_t UserID;
	uint16_t UserID_Mini;
	uint16_t UserID_Max;
	uint16_t TimeCnt;
	FpUserMgrStatus_t Status;
	UserMgrErrorType_t ErrorType;
}FpUserDeleteMgr_t;

typedef struct
{
	FpUserType_t FpUserType;
	FpUserMgrStatus_t Status;
	selection_t Selection;
	uint8_t TimeCnt;
}AllUserFpDeleteMgr_t;

typedef struct
{
	uint16_t UserID;
	uint16_t TimeCnt;
	FPMcmdStatus_t Status;
	FPMcmdErrorType_t ErrorType;
}FpIdentifyMgr_t;

typedef struct
{
	enum
	{
		StartCheckHowManyRegisteredFPuser = 0,
		WaitForCheckHowManyRegisteredFPuserCmdAck = 1,
		CheckHomManyRegisteredFPuserSuccess,
		CheckHomManyRegisteredFPuserFail
	}Status;
	uint8_t UserNum;
	uint8_t StressUserNum;
	uint8_t MasterNum;
	uint8_t TimeCnt;
	uint8_t FailTimes;
	uint8_t UserIdList[32];	
	uint8_t UserIdListChecksum;
}CheckHomManyRegisteredFPuser_t;

typedef struct
{
	enum
	{
		StartCheckIfFpUserIDisRegistered = 0,
		WaitForCheckIfFpUserIDisRegisteredCmdAck = 1,
		CheckIfFpUserIDisRegisteredSuccess,
		CheckIfFpUserIDisRegisteredFail
	}Status;
	bool_t UserIDisRegistered;
	uint8_t TimeCnt;
}CheckIfFpUserIDisRegistered_t;



typedef struct
{
	uint8_t UserID;
	uint8_t CID[4];
	enum
	{
		CIDisValid=0xAA,
		CIDisInvalid=0xFF
	}Status;
}CardMemoryMgr_t;


typedef enum
{
	StartCardUserRegister=0,
	InputCardUserID=1,
	ReportCardUserID,
	CheckCardUserIDisRegistered,
	SavedCardID,
	Success,
	Fail,
	StartCardUserIdentify,
	ReadingCardID,
	CheckCardIDifRegistered,
	StartCardUserDelete,
	StartAllCardUserDelete,
	WaitForUserConfirmDeleteAllCard,
	DeletingAllCardUser,
	CardUserEXIT
}CardUserMgrStatus_t;

typedef struct
{
	CardUserMgrStatus_t Status;
	UserMgrErrorType_t ErrorType;
	uint16_t TimeCnt;
	uint8_t UserID;
	uint8_t CID[5];
}CardRegisterMgr_t;

typedef struct
{
	CardUserMgrStatus_t Status;
	uint16_t TimeCnt;
	uint8_t CID[5];
	uint16_t UserID;
	uint8_t CardDetectIntervalTimeCnt; 
	uint8_t DelayTimeAfterReadCard; 
}CardIdentifyMgr_t;

typedef struct
{
	CardUserMgrStatus_t Status;
	UserMgrErrorType_t ErrorType;
	uint8_t TimeCnt;
	uint8_t CID[5];
	uint16_t UserID;
}CardUserDeleteMgr_t;

typedef struct
{
	CardUserMgrStatus_t Status;
	selection_t Selection;
	uint8_t TimeCnt;
}AllCardUserDeleteMgr_t;


typedef struct
{
	uint8_t UserID;
	uint8_t PasscodeBuff[12];
	enum
	{
		PasscodeIsValid=0xAA,
		PasscodeIsInvalid=0xFF
	}Status;
}PasscodeMemoryMgr_t;


typedef enum
{
	StartPasscodeUserRegister=0,
	InputPasscodeUserID=1,
	ReportPasscodeUserID,
	CheckIfPasscodeUserIDisRegistered,
	InputFirstPasscode,
	InputSecondPasscode,
	CompareTwoPasscode,
	SavePasscode,
	PasscodeIdentifyIdle,
	PasscodeIdentifyStart,
	PasscodeIdentifyPasscodeInput,
	PasscodeIdentifySuccess,
	PasscodeIdentifyFail,
	PasscodeIdentifyAgingTestSuccess,
	PasscodeIdentifySelfTestSuccess,
	PasscodeIdentifyFastVersionCheckSuccess,
	PasscodeIdentifyRemoteUnlockRequestSuccess,
	PasscodeIdentifyIntoMainMenuSuccess,
	PasscodeIdentifyEngineeringModeSuccess,
	PasscodeIdentifyWifiMFTSuccess,
	PasscodeIdentifyLanguageSetToEnglishSuccess,
	PasscodeIdentifyLanguageSetToChineseSuccess,
	StartPasscodeUserDelete,
	StartAllPasscodeUserDelete,
	WaitForDeleteAllPasscodeUserConfirm,
	DeletingAllPasscodeUser,
	RegisterPasscodeUserSuccess,
	RegisterPasscodeUserFail,
	DeletePasscodeUserSuccess,
	DeletePasscodeUserFail,
	DeleteAllPasscodeUserSuccess,
	DeleteAllPasscodeUserFail,
	PasscodeUserEXIT,
	ModifyManagerPasscodeStart,
	ModifyManagerPasscodeFail,
	ModifyManagerPasscodeSuccess
}PasscodeUserMgrStatus_t;

typedef struct
{
	PasscodeUserMgrStatus_t Status;
	UserMgrErrorType_t ErrorType;
	uint16_t TimeCnt;
	uint8_t UserID;
	uint8_t Passcode[6];
	UserPriority_t UserPriority;
}PasscodeUserRegisterMgr_t;

typedef struct
{
	PasscodeUserMgrStatus_t Status;
	UserMgrErrorType_t ErrorType;
	uint16_t TimeCnt;
	//uint8_t Passcode[6];
	uint16_t UserID;
	UserPriority_t UserPriority;
}PasscodeUserDeleteMgr_t;

typedef struct
{
	PasscodeUserMgrStatus_t Status;
	selection_t Selection;
	uint8_t TimeCnt;
}AllPasscodeUserDeleteMgr_t;

typedef struct
{
	PasscodeUserMgrStatus_t Status;
	UserMgrErrorType_t ErrorType;
	uint16_t TimeCnt;
	uint8_t Passcode[10];
	uint8_t UserID;
}PasscodeUserIdentifyMgr_t;

typedef struct
{
	enum
	{
		SelfTestIdle=0,
		SelfTestStart=1,
		SelfTestWait,
		SelfTestSuccess,
		SelfTestFail,
		SelfTestDisplayResult
	}Status;
	enum
	{
		ErrorNone_SelfTest=0,
		ErrorNoHallSensor=1
	}ErrorCode;
	uint16_t TimeLimitedCnt;
}MotorSelfTestMgr_t;


typedef struct
{
	uint8_t UnlockTime;
	uint8_t AutoLockTime;
	enum
	{
		LEFTOPEN = 0x00,
		RIGHTOPEN=0x01,
	}LockDirection;
	enum
	{
		LowTorque=1,
		MiddleTorque=2,
		LargeTorque=3
	}TorqueLevel;
	uint8_t BoltLockTime;
	uint8_t LockingTravel;
	uint8_t AutoEject;
	MotorSelfTestMgr_t SelfTest;
}AutoMotorMgr_t;


typedef struct
{
	uint16_t SleepDelayTimerCnt;
	uint16_t AwakeTimerCnt;
	enum
	{
		RUN=0,
		SLEEP=1
	}Status;
	enum
	{
		SleepTimer=0,
		UserForced=1
	}SleepSource;
	enum
	{
		FingerTouch=0,
		KeyBoardTouch=1,
		CasingOpen,
		CasingClose,
		RTCwakeupTimer,
		SystemResetKey,
		AntiPrying,
		SliderTouchKey,
		DriverBoard,
		BodyInduction,
		WifiModule,
		NFCIrq,
		DoNotCare
	}AwakeSource;

	uint8_t	SavedCasingStatus;
//	bool_t SlideHallIsWorking;
}SystemPowerMgr_t;

typedef struct
{
	uint8_t BatteryVoltage;
	uint8_t ProtectVoltageTriggerTimes;
	bool_t LowBatteryProtectionEnabled;
	bool_t PostLowBattery;
	enum
	{
		LEVEL_0=0,
		LEVEL_1=1,
		LEVEL_2,
		LEVEL_3,
		LEVEL_4,
		LEVEL_5,
		LEVEL_6,
		LEVEL_7
	}BatteryLevel;
	enum
	{
		LiBattery=0,
		ZnMnBattery=1
	}BatteryType;
	uint8_t TimeCnt;
}BatteryMgr_t;


typedef struct
{
	enum{
		F1K=0,
		F2K,
		F4K
	}HZ;
	uint8_t TimeCnt;	
}BeepMgr_t;

typedef struct
{
	enum
	{
		VoiceIdle=0,
		SendingDataStart=1,
		SendingData=2,
		SendingDataEnd,
		ContinueSendData,
		StopCurrentDataTransfer,
		WaitForVoicePlayerIdle,
		Sleep
	}Status;	
	uint8_t BitPoint;
	uint8_t PulseWidthCnt;
	uint8_t Data;
	uint8_t DataBuff[100];
	uint8_t SendDataNum;
	uint8_t TotalDataNum;
	bool_t VoicePlayEnd;
	uint8_t CurrentVolume;
	uint8_t BusyPinStatusFilter;
}VoiceDataTransferMgr_t;

typedef struct
{
	uint8_t DataBuff[100];
	uint8_t DataNum;
	uint8_t CurrentVolume;
	bool_t VoiceDataReady;
}VoiceDataBufferMgr_t;


typedef enum
{
	UnRegistered=0,
	Registered=1
}RegisterStatus_t;



typedef struct
{
	uint16_t UserID;
	RegisterStatus_t RegisterStatus;
	UserPriority_t UserPriority;
}FpUserMemoryMgr_t;

typedef struct
{
	enum
	{
		MagnetIdle=0,
		MagnetBusy=1
	}Status;
	uint8_t TimeCnt;
}EMAGNETMgr_t;


typedef struct
{
	enum
	{
		StartRestoreFactoryDefault = 0,
		WaitForRestoreFactoryDefaultUserConfirm = 1,
		SaveFPMserialNumber,
		ConfirmedToRestoreFactoryDefault,
		ResetFPMuser,
		ResetFaceUser,
		ResetCardUser,
		ResetPasscodeUser,
		RestoreFactorySuccess,
		RestoreFactoryFail,
		RestoreFactoryDefaultEXIT
	}Status;
	selection_t Selection;
	uint8_t TimeCnt;
	uint8_t FailTimes;
}RestoreFactoryDefaultMgr_t;

typedef struct
{
	enum
	{
		StartVoiceEnableSetting = 0,
		WaitForVoiceEnableSettingUserConfirm = 1,
		VoiceEnableSettingSuccess,
		VoiceEnableSettingEXIT
	}Status;
	bool_t Enable;
	selection_t Selection;
	uint8_t volume;
	uint8_t TimeCnt;
}VoiceMgr_t;

typedef struct
{
	enum
	{
		StartPickAlarmEnableSetting = 0,
		WaitForPickAlarmEnableSettingUserConfirm = 1,
		PickAlarmEnableSettingSuccess,
		PickAlarmEnableSettingEXIT
	}Status;
	bool_t Enable;
	selection_t Selection;
	uint8_t TimeCnt;
}PickAlarmEnableMgr_t;


typedef struct
{
	enum
	{
		StartCheckMemory = 0,
		WaitForReadFPuserNum = 1,
		CheckMemorySuccess,
		CheckMemoryFail
	}Status;
	uint8_t TimeCnt;
	uint8_t FaceMasterNum;
	uint8_t FaceUserNum;
	uint8_t FpUserNum;
	uint8_t StressFpUserNum;	
	uint8_t FpMasterNum;
	uint8_t CardUserNum;
	uint8_t PasscodeUserNum;
	uint8_t PasscodeMasterNum;
	bool_t NeedRereadUserFromMemory;
}CheckMemoryMgr_t;

typedef struct
{
	enum
	{
		DisplayFpUserMemoryUsage=0,
		DisplayPasscodeUserMemoryUsage=1,
		DisplayCardUserMemoryUsage,
		DisplayMemoryUsageEnd	
	}Status;

	uint8_t TimeCnt;
}DisplayMemoryUsageMgr_t;

typedef struct
{
	enum
	{
		StartInitialization=0,
		MemoryCheck=1,
		MemoryCheckWait,
		FingerPrintCheck,
		FingerPrintCheckWait,
		CardReaderCheck,
		CardReaderCheckWait,
		TouchCheck,
		TouchCheckWait,
		CheckFinished
	}Status;
	bool_t MemoryCheckPass;
	bool_t FingerPrintCheckPass;
	bool_t CardReaderCheckPass;
	bool_t TouchCheckPass;
	uint8_t FrmCheckFailTimes;
	uint16_t TimeCnt;
}InitializationMgr_t;

typedef struct
{
	enum
	{
		FrmCheckInit,
		FrmCheckWait,	
		FrmCheckPowerOff,
		FrmCheckPowerOn,
		FrmCheckReset,
		FrmCheckStop
	}Status;
	uint8_t FrmCheckFailTimes;
	uint16_t TimeCnt;
}FrmConnectionCheckMgr_t;


typedef struct
{
	enum
	{
		StartManagerIdentify = 0,
		IdentifyingManager = 1,
		ManagerIdentifySuccess,
		ManagerIdentifyFail,
		ManagerIdentifyExit
	}Status;

	enum
	{
		Invalid = 0xF0,
		Valid = 0xE8
	}PasscodeValidFlag;
	UserType_t MasterType;
	uint8_t	PasscodeBuff[6];
	uint8_t TimeCnt;
	uint8_t PasscodeUserID;
	uint8_t FpUserID;
	uint8_t FaceUserID;
	UserIdentifyErrorType_t ErrorType;
}ManagerIdentifyMgr_t;

typedef struct
{
	PasscodeUserMgrStatus_t Status;
	UserMgrErrorType_t ErrorType;
	uint16_t TimeCnt;
	uint8_t UserID;
	uint8_t Passcode[6];
}ManagerPasscodeModifyMgr_t;

typedef struct
{
	status_t CardIdentifyStatus;
	status_t FPIdentifyStatus;
	status_t PasscodeIdentifyStatus;
	status_t FaceIdentifyStatus;
	enum
	{
		LocalPasscode=0,
		AppPasscode=1,
		OnlinePasscode
	}PasscodeType;
	enum
	{
		CARD = 1,
		PASSCODE = 2,
		FINGERPRINT,
		FACE,
		FINGERPRINTandCARD,
		FINGERPRINTandPASSCODE,
		CARDandPASSCODE,
		RemoteUnlock,
		InsideCloseDoor,
		InsideOpenDoor,
		OutsideCloseDoor,
		ONLINEPASSCODE,
		INITIALSTATUS
	}IdentifyType;
	uint8_t FPUserID;
	uint8_t CardUserID;
	uint8_t PasscodeUserID;
	uint8_t FaceUserID;
	uint16_t TimeCnt;
	enum
	{
		SingalMode = 0,
		DoubleMode = 1
	}UnlockingMode;
	bool_t FlagContinuedOpenEnabled;
	UserIdentifyErrorType_t ErrorType;
}UserIdentifyResultMgr_t;

typedef struct
{
	enum
	{
		SELFTEST_START = 0,
		VoiceAndTouchAndLEDs_TEST=1,
		Display_TEST,
		FPM_TEST,
		FRM_TEST,
		BODYINDUCTION_TEST,
		EEPROM_TEST,
		RFID_TEST,
		ANTIPRYING_TEST,
		COMMUNICATION_TEST,
		VOLTAGE_TEST,
		LOWPOWER_TEST
	}Status;
	struct
	{
		bool_t KEY_ZERO_OK;
		bool_t KEY_ONE_OK;
		bool_t KEY_TWO_OK;
		bool_t KEY_THREE_OK;
		bool_t KEY_FOUR_OK;
		bool_t KEY_FIVE_OK;
		bool_t KEY_SIX_OK;
		bool_t KEY_SEVEN_OK;
		bool_t KEY_EIGHT_OK;
		bool_t KEY_NINE_OK;
		bool_t KEY_ASTERISK_OK;
		bool_t KEY_POUNDSIGN_OK;
		bool_t KEY_DOORCLOSE_OK;
		bool_t KEY_DOORBELL_OK;
	}TouchKeySelfTestStatus;
	enum
	{
		Unknow=0,
		Passed=1,
		Failed
	}TestResult;
	bool_t ComOkFlag;
	uint16_t TimeCnt;
}SelfTestMgr_t;



typedef struct
{
	uint8_t FpIdentifyFailedTimes;
	uint8_t CardIdentifyFailedTimes;
	uint8_t PasscodeIdentifyFailedTimes;
	uint8_t FaceIdentifyFailedTimes;
	uint8_t ManagerPasscodeIdentifyFailedTimes;		
	uint8_t ManagerFpIdentifyFailedTimes;
	bool_t SystemLocked;
	uint16_t SystemLockedTimeDelay;
}SafetyMonitorMgr_t;

typedef struct
{
	enum
	{
		UnlockingModeSettingStart = 0,
		WaitForUserConfirmUnlockingMode = 1,
		UserConfirmedUnlockingMode,
		UnlockingModeSettingSuccess
	}Status;
	uint16_t TimeCnt;
}UnlockingModeMgr_t;

typedef struct
{
	enum
	{
		FPMpowerDown = 0,
		FPMpowerOn=1
	}Status;
}FPMpowerMgr_t;

typedef struct
{
	enum
	{
		TouchPowerDown = 0,
		TouchPowerOn=1,
		LowSensitivity,
		HighSensitivity,
		NoSensing,
	}Status;
	bool_t NeedHardwareResetTouch;
}TouchPowerMgr_t;


typedef struct
{
	uint32_t MotorRunTimes;
	uint16_t SlideRunTimes;
	bool_t TestFlag;
	uint16_t TimeCnt;
}AgingTestMgr_t;


typedef enum
{
	OpenDoor = 0,
	ChangSystemConfig=1,
	AddFpUser,
	DeleteFpUser,
	DeleteAllFpUser,
	DeleteAllStressFpUser,
	AddPasscodeUser,
	DeletePasscodeUser,
	DeleteAllPasscodeUser,
	AddCardUser,
	DeleteCardUser,
	DeleteAllCardUser,
	AddFaceUser,
	DeleteFaceUser,
	DeleteAllFaceUser
}EventLogType_t;

typedef struct
{
	uint8_t FlagHighByte;
	uint8_t FlagLowByte;
	uint8_t LogIDmajor;
	//uint16_t LogIDminor;
	uint8_t year;
	uint8_t month;
	uint8_t date;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	
	UserType_t UserType;
		
	uint8_t UserID;
	
	EventLogType_t EventType;
	uint8_t EventParameter;
	uint8_t Remain1;
	uint8_t Remain2;
	uint8_t Remain3;
}Log_t;	//total 16 bytes

typedef struct
{
	uint16_t LastPoint;
	uint16_t DisplayPoint;
	uint16_t SavedDisplayPoint;
	Log_t NewLog;
	Log_t DisplayLog;
}LogMgr_t;

typedef struct
{
	uint16_t TimeCnt;
	uint16_t DeleteSectorNum;
	uint8_t Percentage;
	selection_t Selection;
	enum
	{
		LogDeleteIdle=0,
		LogDeleteStart=1,
		WaitforLogDeleteCofirm,
		LogDeleting,
		LogDeleteSuccess
	}Status;
}LogDeleteMgr_t;


typedef struct
{
	uint8_t year;
	uint8_t month;
	uint8_t date;
	uint16_t MatchedEventLogNum;
	uint16_t OffsetEventLogNum;
	uint16_t StartEventLogPoint;
	enum
	{
		SetYear = 0,
		SetMonth=1,
		SetDate,
		SearchEventLogWithDate,
		ShowEventLogWithDate
	}Status;
}CheckEventLogByDateMgr_t;

typedef struct
{
	uint16_t MatchedEventLogNum;
	uint16_t OffsetEventLogNum;
	uint16_t StartEventLogPoint;
	enum
	{
		SearchEventLogWithSequence=0,
		VoiceReportOperation = 1,
		ShowEventLogWithSequence
	}Status;
}CheckEventLogBySequenceMgr_t;

typedef struct
{
	enum
	{
		ReportLogIdle=0,
		ReportLogInit=1,	
		ReportLogID,
		ReportLogType,
		ReportLogYear,
		ReportLogMonth,
		ReportLogDate,
		ReportLogHour,
		ReportLogMinute,
		ReportLogSecond,
		ReportLogUserID,
		ReportLogEnd
	}Status;
	uint16_t TimeCnt;
}VoiceReportLogMgr_t;

typedef enum
{
	DefaultSmallBrand = 0x11,
	DefaultBigBrand = 0x22,	
	SmallBrand = 0x55,
	BigBrand = 0xAA
}BrandType_t;

typedef struct
{
	BrandType_t BrandType;
	bool_t GotBrandData;
	uint8_t LockBrandDisDataBuff[512];
	uint8_t BrandChangeTimeDelay;
}LockBrand_t;


typedef struct
{
	bool_t AntiPryingTrigger;
	uint8_t FilterTimes;
	uint16_t TimeCnt;
}AntiPryingMgr_t;

typedef struct
{
	enum
	{ 
		IsNotMatched =0,
		IsMatched = 1,
		Unkonw
	}SNmatched;
	bool_t SNavariable;
	uint8_t SN[32];
}FPMserialNumberManager_t;



typedef enum
{
	Chinese=0,
	English=1
}SystemLanguage_t;

typedef struct
{
	bool_t Key0ShouldBeOn;
	bool_t Key1ShouldBeOn;
	bool_t Key2ShouldBeOn;
	bool_t Key3ShouldBeOn;
	bool_t Key4ShouldBeOn;
	bool_t Key5ShouldBeOn;
	bool_t Key6ShouldBeOn;
	bool_t Key7ShouldBeOn;	
	bool_t Key8ShouldBeOn;
	bool_t Key9ShouldBeOn;
	bool_t KeyAsteriskShouldBeOn;
	bool_t KeyPoundSignShouldBeOn;
	bool_t KeyDoorCloseShouldBeOn;
	bool_t KeyDoorBellShouldBeOn;
	keycode_t Keycode;
}LEDsMgr_t;


typedef	enum
{
	AckIdle = 0,
	WaitParameterAck=1,
	WaitStatusAck,
	WaitOpenDoorAck,
	WaitCloseDoorAck,
	WaitInfoAck,
	WaitAlarmAck,
	WaitWifiHostSpotAck,
	WaitExitWifiHostSpotAck,
	WaitClearWifiDataAck,
	WaitGetNetworkTimeAck,
	WaitRemoteUnlcokAck,
	WaitExitRemoteUnlcokAck,
	WaitMFTAck,
	WaitMotorSelftestAck,
	WaitOnlinePasswordVerifyCmdACK,
	WaitPositivePowerOnWifiAck,
	WaitPlayVoiceAck,
	WaitGetVoiceStatusAck,
	WaitAckTimeOut
}ComportStatus_t;

typedef struct
{
	uint8_t PostParameter;
	uint8_t PostStatus;
	uint8_t PostOpenDoor;
	uint8_t PostCloseDoor;
	uint8_t PostInfo;
	uint8_t PostAlarm;
	uint8_t PostWifiHostSpot;
	uint8_t PostExitWifiHostSpot;
	uint8_t PostClearWifiData;
	uint8_t PostGetNetworkTime;
	uint8_t PostRemoteUnlock;
	uint8_t PostExitRemoteUnlock;
	uint8_t PostWifiMFT;
	uint8_t PostMotorSelftest;
	uint8_t PostPositivePowerOnWifi;
	uint8_t PostOnlinePasswordVerify;
	uint8_t PostPlayVoice_1;
	uint8_t PostPlayVoice_2;
	uint8_t	PostGetVoicePlayerStatus;
	enum
	{
		Standby=0,
		Close=1,
		Closing,
		Openning,
		OpenedWaitClose,
		Open
	}DoorStatus;
	struct
	{
		uint8_t UserType;
		uint16_t UserID;
	}Unlock;

	struct
	{
		uint8_t Type;
		uint8_t UserType;
		uint16_t UserID;
	}Info;
	struct
	{
		uint8_t Type;
		uint8_t UserType;
		uint16_t UserID;
	}Alarm;

	uint8_t TimeOutTimes;
	uint16_t TimeOutCnt;
	ComportStatus_t Status;
	uint16_t TimeCnt;
	bool_t RestoreFactoryDefaultTrig;
	bool_t TimeSyncWithNetwork;
	bool_t VoiceDataBuffIsFull_1;
	uint8_t VoiceBuff_1[31];
	bool_t VoiceDataBuffIsFull_2;
	uint8_t VoiceBuff_2[31];
	uint8_t VoiceDataID_1;
	uint8_t VoiceDataID_2;

	uint8_t RxBuff1[20];
	bool_t Flag_RxBuff1IsFull;
	uint8_t RxBuff2[20];
	bool_t Flag_RxBuff2IsFull;
}ComportMgr_t;

typedef struct
{
	enum
	{
		OnlinePasswordVerifyInit=0,
		OnlinePasswordVerifyStart=1,
		OnlinePasswordVerifySuccess,
		OnlinePasswordVerifyFail
	}Status;

	enum
	{
		OnlinePasswordMater=0,
		OnlinePasswordUser=1,
		OnlinePasswordTempUser,
		OnlinePasswordStressUser
	}OnlinePasswordUserPriority;

	uint8_t OnlinePasswordUserType;
	uint16_t UserID;
	uint16_t TimeOutCnt;
	uint8_t OnlinePasswordBuff[6];
	bool_t WifiPowerIsOnForOnlinePassword;
}WifiOnlinePasswordVerifyMgr_t;

typedef struct
{
	uint16_t Para1;
	uint16_t Para2;
	uint8_t Buff[100];
	enum
	{
		WifiAckIdle = 0,
		WaitGeneralCmdACK=1,
		GotGeneralCmdACK,
		WaitHostSpotCmdACK,
		GotHostSpotCmdACK,		
		WaitPowerDownCmdACK,
		GotPowerDownCmdACK,
		WaitRemoteUnlockRequestCmdACK,
		WaitRemoteUnlockSuccessCmdACK,
		WaitCheckStatusCmdACK,
		GotCheckStatusCmdACK,		
		WaitUtcCmdACK,
		GotUtcCmdACK,
		WaitManufactureTestCmdACK,
		WaitCaptureImageCmdACK,
		WaitCaptureVideoCmdACK,
		WaitVideoConfigCmdACK,
		CountDownForSendOutRemoteUnlockSuccessCmd,
		WaitGetRemoteUnlockPasscodeCmdACK,
//		WaitOnlinePasswordVerifyCmdACK,
		WifiAckTimeOut
	}Status;
	uint8_t ErrorCode;
	uint8_t AckLenth;
	uint16_t TimeOutCnt;
}WifiAckMgr_t;

typedef struct
{
	enum
	{
		PowerOnDelay =0,
		PowerOn=1,
		PowerOffDelay,
		PowerOff,
		PowerShoulbeOff
	}Status;
	uint16_t PowerOnDelayTime;
	uint16_t PowerOnLimitedTimer;
	bool_t PowerOnForOnlinePassword;
}WifiPowerMgr_t;

typedef struct
{
	enum
	{
		LinkStart=0,
		LinkWait=1,
		LinkRouter,
		LinkSuccess,
		LinkFail,
		LinkIdle
	}Status;
	uint16_t Timer;
	uint16_t TimeLimitedCnt;
	uint16_t TimeCnt;
	uint8_t errorcode;
}WifiLinkMgr_t;

typedef struct
{
	enum
	{
		StartChecking=0,
		CheckingConnection=1,
		NotConnected,
		Connected
	}Status;
	uint8_t TryTimes;
	uint16_t TimeLimitedCnt;
}WifiConnection_t;
	
typedef struct
{
	enum
	{	
		NetWorkGood=0,
		NetWorkNG=1,
		NetWorkChecking,
		NetWorkNotChecked
	}Status;
	uint8_t NetWorkCheckingTryTimes;
	uint16_t NetWorkCheckingTimeLimited;
}WifiNetWorkMgr_t;

typedef struct
{
	bool_t TimeAvailable;
	bool_t TimeNeedRenew;
	uint8_t year;
	uint8_t month;
	uint8_t date;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
}WifiTimeMgr_t;

typedef struct
{
	enum
	{
		NotConnectedToRooter=0,
		ConnectedToRooter=1
	}Status;
}WifiConfigStatus_t;

typedef struct
{
	uint8_t PostFlag_PID;
	uint8_t PostFlag_CheckStatus;
	uint8_t PostFlag_ResetNetworkConfig;
	uint8_t PostFlag_GetHotspot;
	uint8_t PostFlag_NetworkConfigAck;
	uint8_t PostFlag_lock;	
	uint8_t PostFlag_Unlock;
	uint8_t PostFlag_Info;
	uint8_t PostFlag_Alarm;
	uint8_t PostFlag_GetUTC;
	uint8_t PostFlag_PowerDown;
	uint8_t PostFlag_AwakeWifi;
	uint8_t PostFlag_ClearData;
	uint8_t PostFlag_ManufactureTest;
	uint8_t PostFlag_CaptureImage;
	uint8_t PostFlag_CaptureVideo;
	uint8_t PostFlag_VideoConfig;
	uint8_t PostFlag_DoorBell;
	uint8_t PostFlag_PowerLevel;
	uint8_t PostFlag_RemoteUnlockRequest;
	uint8_t PostFlag_RemoteUnlockSuccess;
	uint8_t PostFlag_GetRemoteUnlockPasscode;
	uint8_t PostFlag_SaveRemoteUnlockPasscode;	
	uint8_t PostFlag_RemoteUnlockConfirmAck;
	uint8_t PostFlag_StressFP;
	uint8_t PostFlag_OnlinePasswordVerify;
	
	
	uint8_t InfoType;
	uint8_t AlarmType;
	UserType_t UserType;
	uint16_t UserID;
	uint16_t RemoteUnlockUserID;
	uint8_t PowerLevel;
}WifiPostMgr_t;

typedef struct
{
	enum
	{
		RemoteUnlockStandby=0,
		RemoteUnlockRequestInit=1,
		RemoteUnlcokDisplayDoorBell,
		RemoteUnlockRequestWait,
		RemoteUnlockStart,
		RemoteUnlockDisplayResult,
		RemoteUnlockExit
	}Status;
		
	enum
	{
		IsUnknow=0,
		IsSuccess=1,
		IsFail
	}Result;
	uint16_t TimeCnt;	
	uint8_t errorcode;	
}RemoteUnlockMgr_t;

typedef struct
{
	enum
	{
		BootIdle=0,
		BootStart=1,
		BootSuccess,
		BootFail
	}Status;
	uint16_t TimeLimitedCnt;	
}WifiBootMgr_t;

typedef struct
{
	enum
	{
		MFTStart=0,
		MFTWait=1,
		MFTSuccess,
		MFTFail,
		MFTIdle
	}Status;
	uint16_t TimeCnt;
	uint8_t errorcode;
}WifiMFTMgr_t;


typedef struct
{
	WifiPostMgr_t PostMgr;
	WifiConnection_t WifiConnection;
	WifiNetWorkMgr_t WifiNetWorkMgr;
	WifiBootMgr_t WifiBootMgr;
	WifiConfigStatus_t WifiConfigStatus;
	WifiTimeMgr_t time;
	WifiLinkMgr_t Link;
	WifiAckMgr_t Ack;
	WifiPowerMgr_t Power;
	RemoteUnlockMgr_t RemoteUnlockMgr;
	WifiOnlinePasswordVerifyMgr_t WifiOnlinePasswordVerifyMgr;
	WifiMFTMgr_t MFT;
	uint16_t RemoteUnlockUserID;
	uint16_t DoorBellTimeCnt;
}WifiMgr_t;


typedef struct
{
	enum
	{
		DoorOpenInit=0,
		OpenDoorWaitForMotorStatusChangeFromStandby=1,	
		GotOpenDoorACK,
		DoorOpenning,
		DoorOpenedWaitClose,
		DoorOpened,
		DoorCloseInit,
		CloseDoorWaitForMotorStatusChangeFromStandby,
		GotCloseDoorACK,
		DoorClosing,
		DoorClosed,
		DisplayOpenDoorEnd,
		DisplayCloseDoorEnd
	}Status;
	bool_t MotorStatusIsChangedFromStandby;
	uint16_t TimeCnt;
}DisplayDoorStatusMgr_t;

typedef struct
{
	enum
	{
		NoError=0,
		CommunicationError=1,
		SystemError
	}ErrorType;
		
	uint16_t TimeCnt;
}ErrorMessageMgr_t;

typedef struct
{
	uint16_t TimeCnt;
}DoorBellMgr_t;


typedef struct
{
	uint8_t FWversion;
	uint8_t HWversion;	
}DriverBoardVersion_t;

typedef enum
{
	FRM_Error_NONE=0,						//
}FRMcmdErrorType_t;

typedef struct
{
	uint16_t Para1;
	uint16_t Para2;
	uint8_t Buff[100];
	uint8_t TimeCnt;
	ACK_Status_t Status;
	FRMcmdErrorType_t ErrorCode;
}FrmAckMgr_t;

/*
typedef enum
{
	FRMcmdStart=0,
	FaceRegisterSuccess=1,
	FaceRegisterFail
}FRMcmdStatus_t;
*/

typedef enum
{
	StartFaceRegister=0,
	FaceResetForRegister=1,
	WaitForFaceResetFinishedForRegister,
	PowerOffForFrmForRegister,
	PowerOnForFrmForRegister,
	InfoUserPutFace,
	WaitForRegisterFaceResult,
	RegisterFaceTemplateSuccess,
	RegisterFaceTemplateFail
}GUI_FaceTemplateRegisterStatus_t;


typedef enum
{
	StartFaceUserRegister=0,
	InputFaceUserID=1,	
	ReportFaceUserID,
	WaitForConfirmAddFaceUser,
	CheckIfFaceUserIDisBeUsed,
	AddUserToFRM,
	DeleteUserFromFRM,
	WaitForDeleteUserACKfromFRM,
	DeleteAllUserFromFRM,
	WaitForDeleteAllUserACKfromFRM,
	WaitForUserConfirmDeleteAllFace,
	SendDeleteAllFaceUserCmdToFRM,
	WaitForDeleteAllFaceUserCmdACKfromFRM,
	RegisterFaceUserSuccess,
	RegisterFaceUserFail,
	DeleteFaceUserSuccess,
	DeleteFaceUserFail,	
	StartFaceIdentify,
	StartFaceUserDelete,
	FaceResetForDeleteTemplate,
	WaitForFaceResetFinishedForDeleteTemplate,	
	PowerOffForFrmForDeleteTemplate,	
	PowerOnForFrmForDeleteTemplate,		
	StartAllUserFaceDelete,
	StartFrmToDeleteAllFaceTemplate,
	WaitForFrmDeleteAllTemplateFinished,
	DeleteAllFaceUserSuccess,
	DeleteAllFaceUserFail,
	EXIT_FaceUserMgr
}FaceUserMgrStatus_t;

typedef enum
{
	FaceUser=0,		//普通人脸
	FaceMaster=1	//管理员人脸	
}FaceUserType_t;

typedef struct
{
	uint16_t UserID;
	uint16_t TimeCnt;
	uint16_t ReportFaceStatusTimeCnt;
	uint8_t EnrollSuccessTimes;
	uint8_t EnrollFailTimes;
	bool_t DuplicateCheck;
	GUI_FaceTemplateRegisterStatus_t Status;
	UserMgrErrorType_t ErrorType;
}GUI_FaceTemplateRegisterMgr_t;

typedef struct
{
	FaceUserType_t FaceUserType;
	uint16_t UserID;
	uint16_t UserID_Mini;
	uint16_t UserID_Max;
	uint16_t TimeCnt;
	FaceUserMgrStatus_t Status;
	UserMgrErrorType_t ErrorType;
}FaceUserRegisterMgr_t;

typedef struct
{
	FaceUserType_t FaceUserType;
	uint16_t UserID;
	uint16_t UserID_Mini;
	uint16_t UserID_Max;
	uint16_t TimeCnt;
	FaceUserMgrStatus_t Status;
	UserMgrErrorType_t ErrorType;
}FaceUserDeleteMgr_t;

typedef struct
{
	FaceUserType_t FaceUserType;
	FaceUserMgrStatus_t Status;
	selection_t Selection;
	uint8_t TimeCnt;
}AllUserFaceDeleteMgr_t;

typedef struct
{
	enum
	{
		StartCheckHowManyRegisteredFACEuser = 0,
		WaitForCheckHowManyRegisteredFACEuserCmdAck = 1,
		CheckHomManyRegisteredFACEuserSuccess,
		CheckHomManyRegisteredFACEuserFail
	}Status;
	uint8_t UserNum;
	uint8_t MasterNum;
	uint8_t TimeCnt;
	uint8_t FailTimes;
}CheckHomManyRegisteredFACEuser_t;

typedef enum
{
	NORMAL =0, 	//人脸正常
	NOFACE =1,  		//未检测到人脸
	TOOUP =2, 		//人脸太靠近图片上边沿，未能录入
	TOODOWN	=3,		//人脸太靠近图片下边沿，未能录入
	TOOLEFT	=5,		//人脸太靠近图片左边沿，未能录入
	TOORIGHT =4,	//人脸太靠近图片右边沿，未能录入
	TOOFAR =6,		//人脸距离太远，未能录入
	TOOCLOSE =7		//人脸距离太近，未
}FaceState_t;

typedef enum
{
	FaceDirrect_MIDDLE=0x01,
	FaceDirrect_RIGH=0x02,
	FaceDirrect_LEFT=0x04,
	FaceDirrect_DOWN=0x08,
	FaceDirrect_UP=0x10,	
	FaceDirrect_UNDEFINED=0x00
}FaceDirrect_t;

/*
typedef enum
{
	FrmOperationSuccess=0,
	FrmUserIDisBeUsed=1,	
	FrmOperationFail,
	UnknownReason
}FaceResult_t;
*/

typedef enum
{
	MR_SUCCESS=0,					//  成功
	MR_REJECTED=1,					//  模块拒绝该命令
	MR_ABORTED=2,					//  录入/解锁算法已终止
	MR_FAILED4_CAMERA=4,			//  相机打开失败
	MR_FAILED4_UNKNOWNREASON=5,		//  未知错误
	MR_FAILED4_INVALIDPARAM=6,		//  无效的参数
	MR_FAILED4_NOMEMORY=7,			//  内存不足
	MR_FAILED4_UNKNOWNUSER=8,		//  没有已录入的用户
	MR_FAILED4_MAXUSER=9,			//  录入超过最大用户数量
	MR_FAILED4_FACEENROLLED=10,		//  人脸已录入
	MR_FAILED4_LIVENESSCHECK=12,	//  活体检测失败
	MR_FAILED4_TIMEOUT=13,			//  录入或解锁超时
	MR_FAILED4_AUTHORIZATION=14,	//  加密芯片授权失败
	MR_FAILED4_READ_FILE=19,		//  读文件失败
	MR_FAILED4_WRITE_FILE=20,		//  写文件失败
	MR_FAILED4_NO_ENCRYPT=21		//  通信协议未加密
}FrmErrorCode_t;

typedef enum
{
	FRM_IDLE=0,
	FRM_BUSY=1,
	FRM_ERROR,
	FRM_INVALID
}FrmStatus_t;

typedef enum
{
	FRM_PowerOff=0,
	FRM_PowerOn=1
}FrmPowerStatus_t;
	
typedef enum
{
	FRM_Enabled=0x55,
	FRM_Disabled=0xAA
}FrmFunctionConfig_t;

typedef struct
{	
	FrmFunctionConfig_t FrmFunctionConfig;
	UserType_t UserType;
	uint16_t UserID;
	uint8_t UserName[32];
	FaceDirrect_t FaceDirrect;
	FaceState_t FaceState;
	FrmStatus_t Status;
	FrmPowerStatus_t PowerStatus;
	uint16_t TimeLimited;
	uint16_t FrmFunctionSwitchTimeDelay;
	
	bool_t SendCmdFlag_RegisterMultiTimes;
	bool_t SendCmdFlag_RegisterSingleTimes;
	bool_t SendCmdFlag_Verify;
	bool_t SendCmdFlag_DeleteUser;
	bool_t SendCmdFlag_DeleteAll;
	bool_t SendCmdFlag_GetAllUserID;
	bool_t SendCmdFlag_PownDown;
	bool_t SendCmdFlag_FaceReset;
	bool_t SendCmdFlag_Reset;
	bool_t SendCmdFlag_Status;
	bool_t SendCmdFlag_EnableDemo;
	bool_t SendCmdFlag_DisableDemo;
	bool_t SendCmdFlag_GetVersion;
 
	bool_t PostFlag_FaceState;
	bool_t PostFlag_Ready;
	bool_t PostFlag_Status;
	bool_t PostFlag_ResetResult;
	bool_t PostFlag_FaceResetResult;
	bool_t PostFlag_VerifyResult;
	bool_t PostFlag_RegisterResult;
	bool_t PostFlag_DeleteTemplateResult;
	bool_t PostFlag_DeleteAllResult;
	bool_t PostFlag_GetAllUserIdResult;
	bool_t PostFlag_EnableDemoResult;
	bool_t PostFlag_DisableDemoResult;
	bool_t PostFlag_VersionNumber;
	bool_t PostFlag_PownDown;
	
	FrmErrorCode_t ErrorType;
}FrmMgr_t;

typedef struct
{
	UserPriority_t UserPriority;
	uint16_t FaceTemplateID;
	RegisterStatus_t RegisterStatus;
}FaceUserMemoryMgr_t;


typedef struct
{
	enum
	{
		FrmIdentifyStart = 0,
		GetFrmStatus=1,
		WaitForGetFrmStatusAck,
		PowerOffForFrmHardwareReset,
		PowerOnForFrmHardwareReset,
		ResetFrmForIdentify,
		WaitForFrmReadyForIdentify,
		SendFrmEnableDemoModeCmd,
		WaitForFrmEnableDemoModeAck,
		SendFrmDisableDemoModeCmd,
		WaitForFrmDisableDemoModeAck,
		SendFaceIdentifyCmd,
		WaitForFaceIdentifyResult,
		FaceIdentifySuccess,
		FaceIdentifySuccess_NoUser,
		FaceIdentifyFail
	}Status;

	uint16_t TimeCnt;

	uint16_t ReportFaceStatusTimeCnt;
	bool_t DemoModeIsEnabled;
	uint16_t IndentifyDelayTimeCnt;
}FaceIdentifyMgr_t;

typedef struct
{
	enum
	{
		StartAllFaceTemplateDelete=0,
		ResetFrmForDeleteAllFaceTemplate=1,
		WaitForFrmReadyForDeleteAllFaceTemplate,
		PowerOffFrmForDeleteAllTemplate,
		PowerOnFrmForDeleteAllTemplate,
		SendDeleteAllFaceTemplateCmdToFRM,
		WaitForDeleteAllFaceTemplateCmdACKfromFRM,
		DeleteAllFaceTemplateSuccess,
		DeleteAllFaceTemplateFail
	}Status;
		
	uint16_t TimeCnt;
}DeleteAllFaceTemplateMgr_t;

typedef struct
{
	uint8_t RecPulse;
	uint8_t SendPulse;
	uint16_t HighLevelDuty;
	uint16_t LowLevelDuty;
	bool_t FaceDetected;
	uint16_t ObstructionDetectedTimes;
}PIRmgr_t;

typedef struct
{
	enum
	{
		Absent=0,
		RadarModule=1,
		PirModule,
	}BodyInductionDeviceType;
	enum
	{
		SensingDistanceL0=0,
		SensingDistanceL1=1,
		SensingDistanceL2=2
	}SensingDistanceLevel;
		
	uint16_t BodyInductionDelayTimeCnt;	
	uint8_t BodyInductionInvalidTimes;
	bool_t 	BodyInductionBeLimited;
	uint32_t BodyInductionInvalidFirstTimesUTC;
	uint32_t BodyInductionInvalidLastTimesUTC;
	uint8_t IrBodyInductionFilterTimes;
	
}BodyInductionMgr_t;

typedef struct
{
	bool_t IsDoorColseKeyAwake;
	bool_t IsDoorBellKeyAwake;
	bool_t IsPoundsignKeyAwake;
}AwakeSystemKeyMgr_t;

typedef struct
{
	enum
	{
		DisplayCaseIgnore=0,
		DisplayHide=1,
		DisplayFinger,
		DisplayFace
	}DisplayType;
	uint16_t TimeCnt;
}AwakeDisplayMgr_t;

typedef struct
{
	uint8_t OperationMode;
	uint8_t StartColor;
	uint8_t StopColor;
	uint8_t Cycles;
}FpmLEDsMgr_t;

typedef struct
{
	uint32_t LastUTCtime; 
	uint32_t CurrentUTCtime;
	bool_t	TrigStrongUnlocking;
}StrongUnlockMgr_t;


#ifdef MAIN_C
#define GLOBAL extern /*********/
#else
#define GLOBAL extern 
#endif
	
	
#endif /* #ifndef STDTYPES_H */ 
/* *************** END OF FILE    STDTYPES_H ****************************** */




