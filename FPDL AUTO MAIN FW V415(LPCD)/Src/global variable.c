#include "stm32g0xx_hal.h"
#include "StdTypes.h"
#include "Project.h"

Timebase_t G_tflagbits;
uint8_t G_BatLevel;
uint16_t G_BatVoltage;
systemtime_t SystemTime;
UartMgr_t UART1_Mgr;
UartMgr_t UART2_Mgr;

uint8_t G_DisRAM[1024];
uint8_t G_TEST;
screen_t CurrentScreen;

GUI_FaceTemplateRegisterMgr_t GUI_FaceTemplateRegisterMgr;
FaceUserRegisterMgr_t FaceUserRegisterMgr;
FaceUserDeleteMgr_t FaceUserDeleteMgr;
AllUserFaceDeleteMgr_t AllUserFaceDeleteMgr;
CheckHomManyRegisteredFACEuser_t CheckHomManyRegisteredFACEuser;

FpmAckMgr_t FpmAckMgr;
FpUserRegisterMgr_t FpUserRegisterMgr;
AllUserFpDeleteMgr_t	AllUserFpDeleteMgr;
FpUserDeleteMgr_t	FpUserDeleteMgr;
FpIdentifyMgr_t FpIdentifyMgr;
CardMemoryMgr_t CardMemoryMgr[DEF_MAX_CARDUSER];
CardRegisterMgr_t CardUserRegisterMgr;
CardIdentifyMgr_t CardIdentifyMgr;
CardUserDeleteMgr_t CardUserDeleteMgr;
AllCardUserDeleteMgr_t AllCardUserDeleteMgr;

FpRegisterMgr_t FpRegisterMgr;

PasscodeMemoryMgr_t PasscodeMemoryMgr[DEF_MAX_PASSCODEMASTER+DEF_MAX_PASSCODEUSER];
PasscodeUserRegisterMgr_t PasscodeUserRegisterMgr;
PasscodeUserDeleteMgr_t PasscodeUserDeleteMgr;
AllPasscodeUserDeleteMgr_t AllPasscodeUserDeleteMgr;
PasscodeUserIdentifyMgr_t PasscodeUserIdentifyMgr;
BatteryMgr_t BatteryMgr;

EMAGNETMgr_t EMAGNETMgr;


SystemPowerMgr_t SystemPowerMgr;

BeepMgr_t BeepMgr;


FpUserMemoryMgr_t FpUserMemoryMgr[DEF_MAX_FPMASTER+DEF_MAX_FPUSER+DEF_MAX_STRESSFPUSER];

RestoreFactoryDefaultMgr_t RestoreFactoryDefaultMgr;

VoiceMgr_t VoiceMgr;
PickAlarmEnableMgr_t PickAlarmEnableMgr;

CheckHomManyRegisteredFPuser_t CheckHomManyRegisteredFPuser;
CheckIfFpUserIDisRegistered_t CheckIfFpUserIDisRegistered;
CheckMemoryMgr_t CheckMemoryMgr;
ManagerIdentifyMgr_t ManagerIdentifyMgr;
DisplayMemoryUsageMgr_t DisplayMemoryUsageMgr;
InitializationMgr_t InitializationMgr;
FrmConnectionCheckMgr_t FrmConnectionCheckMgr;
ManagerPasscodeModifyMgr_t	ManagerPasscodeModifyMgr;
UserIdentifyResultMgr_t	UserIdentifyResultMgr;
SelfTestMgr_t SelfTestMgr;

SafetyMonitorMgr_t SafetyMonitorMgr;
UnlockingModeMgr_t UnlockingModeMgr;

VoiceDataTransferMgr_t VoiceDataTransferMgr; 
VoiceDataBufferMgr_t VoiceDataBufferMgr;


FPMpowerMgr_t FPMpowerMgr;

bool_t g_ASTERISK_PressedOnMainScreen;

AgingTestMgr_t AgingTestMgr;

LogMgr_t LogMgr;

LockBrand_t LockBrand;

CheckEventLogByDateMgr_t CheckEventLogByDateMgr;

CheckEventLogBySequenceMgr_t CheckEventLogBySequenceMgr;

VoiceReportLogMgr_t VoiceReportLogMgr;


TouchPowerMgr_t TouchPowerMgr;

SystemLanguage_t SystemLanguage;

AntiPryingMgr_t AntiPryingMgr;

LogDeleteMgr_t LogDeleteMgr;

FPMserialNumberManager_t FPMserialNumberManager;

LEDsMgr_t LEDsMgr;

WifiMgr_t WifiMgr;

ComportMgr_t ComportMgr;

AutoMotorMgr_t  AutoMotorMgr;

DisplayDoorStatusMgr_t DisplayDoorStatusMgr;

ErrorMessageMgr_t ErrorMessageMgr;

DoorBellMgr_t DoorBellMgr;


DriverBoardVersion_t DriverBoardVersion;

FrmAckMgr_t FrmAckMgr;
FaceUserMemoryMgr_t FaceUserMemoryMgr[DEF_MAX_FACEMASTER+DEF_MAX_FACEUSER];
FrmMgr_t FrmMgr;
FaceIdentifyMgr_t FaceIdentifyMgr;
DeleteAllFaceTemplateMgr_t DeleteAllFaceTemplateMgr;

PIRmgr_t PIRmgr;

FpmLEDsMgr_t FpmLEDsMgr;

AwakeDisplayMgr_t AwakeDisplayMgr;


//BodyInductionMgr_t BodyInductionMgr;

keycode_t Debug_Keycode;

StrongUnlockMgr_t StrongUnlockMgr;



// 阿尔法
UserResMsg userResMsg;
DelUserMsg delUserMsg;
AllDelUserMsg allDelUserMsg;