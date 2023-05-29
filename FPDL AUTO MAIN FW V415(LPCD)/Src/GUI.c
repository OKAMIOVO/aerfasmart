#include "StdTypes.h"
#include "gpio.h"
#include "spi.h"
#include "i2c.h"
#include "adc.h"
#include "rtc.h"
#include "tim.h"
#include "iwdg.h"
#include "IO.h"
#include "ISR.h"
#include "RealTimeClock.h"
#include "Hardware.h"
#include "Project.h"
#include "GUI.h"
#include "I2cBus.h"
#include "CSK14.h"
#include "TSM12S.h"
#include "FM5114_Touch.h"
#include "Fingerprint.h"
#include "Font.h"
#include "MFC.h"
#include "EEPROM.h"
#include "BeepMgr.h"
#include "RealTimeClock.h"
#include "Flash.h"
#include "OLEDdriver.h"
#include "LCD_HFG12864.h"
#include "LEDsMgr.h"
#include "AppUnlock.h"
#include "usart.h"
#include "HostUart.h"
#include "FaceRecoginitionMgr.h"
#include "PIR.h"
#include "Radar.h"
#ifdef Function_YouzhiyunjiaWifi 
#include "YouzhiyunjiaWifi.h"
#endif


#ifdef Function_FPM_IS_ML383C
#define DEF_FPM_EnrollTimes 8
#define DEF_FPM_EnrollFailTimes_Limited 7
#else
#define DEF_FPM_EnrollTimes 5
#define DEF_FPM_EnrollFailTimes_Limited 3
#endif


#define Function_Network

/*
#ifdef Function_Network
#define DEF_MAX_MainMenuNum 6
#define DEF_MAX_FpMenuNum 3
#else
#define DEF_MAX_MainMenuNum 5
#define DEF_MAX_FpMenuNum 2
#endif
*/

#define DEF_MenuSwitchDelayTime //Hardware_DelayMs(100);

uint16_t AllDeleteCnt = 300;

static uint16_t MainMenuVoiceBuff[][6]=
{	
	{VOICE_Press,VOICE_One,VOICE_User,VOICE_Management,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Two,VOICE_SystemSetting,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Three,VOICE_InfomationInquiry,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Four,VOICE_NetworkConfig,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ExitMenu,DEF_VoiceSegmentEndFlag},
	{VOICE_PressPoundKey,VOICE_ToRepeatListening,DEF_VoiceSegmentEndFlag}
};
	
static uint16_t UserManagementMenuVoiceBuff_WithFRM[][6]=
{
	{VOICE_Press,VOICE_One,VOICE_Face,VOICE_User,VOICE_Management,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Two,VOICE_Fingerprint,VOICE_User,VOICE_Management,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Three,VOICE_Password,VOICE_User,VOICE_Management,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Four,VOICE_Card,VOICE_User,VOICE_Management,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ExitMenu,DEF_VoiceSegmentEndFlag},
	{VOICE_PressPoundKey,VOICE_ToRepeatListening,DEF_VoiceSegmentEndFlag}
};

static uint16_t UserManageMenuVoiceBuff[][6]=
{
	{VOICE_Press,VOICE_One,VOICE_Add,VOICE_Administrator,VOICE_User,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Two,VOICE_User,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Three,VOICE_Delete,VOICE_User,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ExitMenu,DEF_VoiceSegmentEndFlag},
	{VOICE_PressPoundKey,VOICE_ToRepeatListening,DEF_VoiceSegmentEndFlag}
};

static uint16_t UserManagementMenuVoiceBuff[][6]=
{	
	{VOICE_Press,VOICE_One,VOICE_Fingerprint,VOICE_User,VOICE_Management,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Two,VOICE_Password,VOICE_User,VOICE_Management,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Three,VOICE_Card,VOICE_User,VOICE_Management,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ExitMenu,DEF_VoiceSegmentEndFlag},
	{VOICE_PressPoundKey,VOICE_ToRepeatListening,DEF_VoiceSegmentEndFlag}
};


static uint16_t FaceMenuVoiceBuff[][7]=
{	
	{VOICE_Press,VOICE_One,VOICE_Add,VOICE_Administrator,VOICE_Face,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Two,VOICE_Add,VOICE_User,VOICE_Face,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Three,VOICE_Delete,VOICE_Singal,VOICE_Face,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag},
	{VOICE_PressPoundKey,VOICE_ToRepeatListening,DEF_VoiceSegmentEndFlag}
};

static uint16_t AddAdminUserVoiceBuff[][7]=
{	
	{VOICE_Press,VOICE_One,VOICE_Add,VOICE_Fingerprint,VOICE_Password,VOICE_Card,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Two,VOICE_Add,VOICE_Face,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag},
	{VOICE_PressPoundKey,VOICE_ToRepeatListening,DEF_VoiceSegmentEndFlag}
};

static uint16_t DeleteUserVoiceBuff[][7]=
{	
	{VOICE_Press,VOICE_One,VOICE_Add,VOICE_Fingerprint,VOICE_Delete,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Two,VOICE_All,VOICE_Delete,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag},
	{VOICE_PressPoundKey,VOICE_ToRepeatListening,DEF_VoiceSegmentEndFlag}
};

static uint16_t RegisterManagerFPCVoiceBuff[][3]=
{	
	{VOICE_PleaseEnter,VOICE_ID,DEF_VoiceSegmentEndFlag},
};

//static uint16_t UserIdVoiceBuff[][2]=
//{	
//	{VOICE_ID,DEF_VoiceSegmentEndFlag},
//};

//static uint16_t UserIdResFPCVoiceBuff[][5]=
//{	
//	{VOICE_PleaseEnter,VOICE_Fingerprint,VOICE_Password,VOICE_Card,DEF_VoiceSegmentEndFlag},
//};

static uint16_t AllDeleteUserVoiceBuff[][3]=
{	
	{VOICE_All,VOICE_Delete,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag},
	{VOICE_PressPoundKey,VOICE_ConfirmToDelete,DEF_VoiceSegmentEndFlag}
};

static uint16_t OperateSucessVoiceBuff[][3]=
{	
	{VOICE_Operation,VOICE_Success,DEF_VoiceSegmentEndFlag},
};

static uint16_t FpMenuVoiceBuff[][8]=
{		
	{VOICE_Press,VOICE_One,VOICE_Add,VOICE_Administrator,VOICE_Fingerprint,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Two,VOICE_Add,VOICE_User,VOICE_Fingerprint,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Three,VOICE_Add,VOICE_Stress,VOICE_Fingerprint,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Four,VOICE_Delete,VOICE_Fingerprint,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag},
	{VOICE_PressPoundKey,VOICE_ToRepeatListening,DEF_VoiceSegmentEndFlag}	
};
	
static uint16_t FpDeleteMenuVoiceBuff[][8]=
{		
	{VOICE_Press,VOICE_One,VOICE_Delete,VOICE_Singal,VOICE_User,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Two,VOICE_Delete,VOICE_All,VOICE_User,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Three,VOICE_Delete,VOICE_All,VOICE_Stress,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag},
	{VOICE_PressPoundKey,VOICE_ToRepeatListening,DEF_VoiceSegmentEndFlag}	
};	

static uint16_t PasscodeMenuVoiceBuff[][7]=
{	
	{VOICE_Press,VOICE_One,VOICE_Add,VOICE_Administrator,VOICE_Password,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Two,VOICE_Add,VOICE_User,VOICE_Password,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Three,VOICE_Delete,VOICE_Singal,VOICE_Password,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Four,VOICE_Delete,VOICE_All,VOICE_User,VOICE_Password,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag},
	{VOICE_PressPoundKey,VOICE_ToRepeatListening,DEF_VoiceSegmentEndFlag}
};

static uint16_t CardMenuVoiceBuff[][7]=
{	
	{VOICE_Press,VOICE_One,VOICE_Add,VOICE_Card,VOICE_User,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Two,VOICE_Delete,VOICE_Singal,VOICE_Card,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Three,VOICE_Delete,VOICE_All,VOICE_Card,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag},
	{VOICE_PressPoundKey,VOICE_ToRepeatListening,DEF_VoiceSegmentEndFlag},
};


static uint16_t InfoInquiryMenuVoiceBuff[][7]=
{	
	{VOICE_Press,VOICE_One,VOICE_Log,VOICE_Query,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Two,VOICE_MemoryUsage,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Three,VOICE_SystemVersion,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Four,VOICE_RestoreFactoryDefault,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag},
	{VOICE_PressPoundKey,VOICE_ToRepeatListening,DEF_VoiceSegmentEndFlag},
};

static uint16_t MemoryUsageVoiceBuff[][10]=
{	
	{VOICE_Face,VOICE_Used,VOICE_One,VOICE_Two,VOICE_Three,VOICE_Remained,VOICE_One,VOICE_One,VOICE_One,DEF_VoiceSegmentEndFlag},
	{VOICE_Fingerprint,VOICE_Used,VOICE_One,VOICE_Two,VOICE_Three,VOICE_Remained,VOICE_One,VOICE_One,VOICE_One,DEF_VoiceSegmentEndFlag},
	{VOICE_Password,VOICE_Used,VOICE_One,VOICE_Two,VOICE_Remained,VOICE_One,VOICE_One,DEF_VoiceSegmentEndFlag},
	{VOICE_Card,VOICE_Used,VOICE_One,VOICE_Two,VOICE_Three,VOICE_Remained,VOICE_One,VOICE_One,VOICE_One,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag},
	{VOICE_PressPoundKey,VOICE_ToRepeatListening,DEF_VoiceSegmentEndFlag},
};

static uint16_t SystemConfigMenuVoiceBuff[][6]=
{	
	{VOICE_Press,VOICE_One,VOICE_Time,VOICE_Setting,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Two,VOICE_Voice,VOICE_Setting,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Three,VOICE_DoorLock,VOICE_Setting,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Four,VOICE_Motor,VOICE_Setting,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag},
	{VOICE_PressPoundKey,VOICE_ToRepeatListening,DEF_VoiceSegmentEndFlag}
};

static uint16_t VoiceSettingMenuVoiceBuff[][6]=
{	
	{VOICE_Press,VOICE_One,VOICE_Language,VOICE_Setting,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Two,VOICE_Volume,VOICE_Setting,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag},
	{VOICE_PressPoundKey,VOICE_ToRepeatListening,DEF_VoiceSegmentEndFlag}
};

static uint16_t LanguageSetVoiceBuff[][4]=
{	
	{VOICE_Press,VOICE_One,VOICE_Chinese,DEF_VoiceSegmentEndFlag},
	{VOICE_Press+1,VOICE_Two+1,VOICE_English,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag},
	{VOICE_PressPoundKey,VOICE_ToRepeatListening,DEF_VoiceSegmentEndFlag}
};

static uint16_t VolumeSetVoiceBuff[][5]=
{	
	{VOICE_Press,VOICE_One,VOICE_Big,VOICE_Volume,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Two,VOICE_Middle,VOICE_Volume,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Three,VOICE_Small,VOICE_Volume,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Four,VOICE_Mute,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag},
	{VOICE_PressPoundKey,VOICE_ToRepeatListening,DEF_VoiceSegmentEndFlag}
};
	
static uint16_t LogMenuVoiceBuff[][5]=
{	
	{VOICE_Press,VOICE_One,VOICE_Query,VOICE_Log,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Two,VOICE_Delete,VOICE_Log,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag},
	{VOICE_PressPoundKey,VOICE_ToRepeatListening,DEF_VoiceSegmentEndFlag}
};

	
static uint16_t VoiceStr_VoiceOpened[] = {VOICE_Voice,VOICE_Opened,DEF_VoiceSegmentEndFlag};
static uint16_t VoiceStr_VoiceClosed[] = {VOICE_Voice,VOICE_Closed,DEF_VoiceSegmentEndFlag};

static uint16_t VoiceStr_NormallyOpenModeOpened[] = {VOICE_NormallyOpen,VOICE_Opened,DEF_VoiceSegmentEndFlag};
static uint16_t VoiceStr_NormallyOpenModeClosed[] = {VOICE_NormallyOpen,VOICE_Closed,DEF_VoiceSegmentEndFlag};

/*
static uint16_t DoorLockSettingVoiceBuff[][7]=
{	
	{VOICE_Press,VOICE_One,VOICE_UnlockingMode,VOICE_Setting,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Two,VOICE_AntiSkidAlarm,VOICE_Setting,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Three,VOICE_BodyInduction,VOICE_Setting,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag},
	{VOICE_PressPoundKey,VOICE_ToRepeatListening,DEF_VoiceSegmentEndFlag}
};
*/

static uint16_t DoorLockSettingVoiceBuff_WithFRM[][7]=
{	
	{VOICE_Press,VOICE_One,VOICE_UnlockingMode,VOICE_Setting,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Two,VOICE_AntiSkidAlarm,VOICE_Setting,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Three,VOICE_BodyInduction,VOICE_Setting,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag},
	{VOICE_PressPoundKey,VOICE_ToRepeatListening,DEF_VoiceSegmentEndFlag}
};

static uint16_t DoorLockSettingVoiceBuff[][7]=
{	
	{VOICE_Press,VOICE_One,VOICE_UnlockingMode,VOICE_Setting,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Two,VOICE_AntiSkidAlarm,VOICE_Setting,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag},
	{VOICE_PressPoundKey,VOICE_ToRepeatListening,DEF_VoiceSegmentEndFlag}
};


static uint16_t UnlockModeSetVoiceBuff[][4]=
{	
	{VOICE_Press,VOICE_One,VOICE_SingleUnlock,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Two,VOICE_CombinationUnlock,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag},
	{VOICE_PressPoundKey,VOICE_ToRepeatListening,DEF_VoiceSegmentEndFlag}
};

static uint16_t BodyInductionVoiceBuff[][5]=
{	
	{VOICE_Press,VOICE_One,VOICE_BodyInduction,VOICE_Close,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Two,VOICE_BodyInduction,VOICE_Small,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Three,VOICE_BodyInduction,VOICE_Big,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag},
	{VOICE_PressPoundKey,VOICE_ToRepeatListening,DEF_VoiceSegmentEndFlag}
};

	
static uint16_t VoiceStr_SingleUnlockModeOpened[] = {VOICE_SingleUnlock,VOICE_Opened,DEF_VoiceSegmentEndFlag};
static uint16_t VoiceStr_CombinationUnlockModeOpened[] = {VOICE_CombinationUnlock,VOICE_Opened,DEF_VoiceSegmentEndFlag};

static uint16_t AntiPryingSettingVoiceBuff[][5]=
{	
	{VOICE_Press,VOICE_One,VOICE_AntiSkidAlarm,VOICE_Open,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Two,VOICE_AntiSkidAlarm,VOICE_Close,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag},
	{VOICE_PressPoundKey,VOICE_ToRepeatListening,DEF_VoiceSegmentEndFlag}
};

static uint16_t VoiceStr_AntiPryingOpened[] = {VOICE_AntiSkidAlarm,VOICE_Opened,DEF_VoiceSegmentEndFlag};
static uint16_t VoiceStr_AntiPryingClosed[] = {VOICE_AntiSkidAlarm,VOICE_Closed,DEF_VoiceSegmentEndFlag};


static uint16_t PickAlarmSetVoiceBuff[][5]=
{	
	{VOICE_Press,VOICE_One,VOICE_AntiSkidAlarm,VOICE_Open,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Two,VOICE_AntiSkidAlarm,VOICE_Close,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag},
	{VOICE_PressPoundKey,VOICE_ToRepeatListening,DEF_VoiceSegmentEndFlag}
};

	
static uint16_t RestoreFactoryDefaultVoiceBuff[][4]=
{	
	{VOICE_PressPoundKey,VOICE_Confirm,VOICE_RestoreFactoryDefault,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag}
};

static uint16_t EngineeringModeMenuVoiceBuff_WithFRM[][8]=
{	
	{VOICE_Press,VOICE_One,VOICE_UnlockingDirect,VOICE_Setting,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Two,VOICE_Motor,VOICE_Torque,VOICE_Setting,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Three,VOICE_LockTongueReturn,VOICE_Time,VOICE_Setting,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Four,VOICE_LockDistance,VOICE_Setting,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Five,VOICE_SelfBounce,VOICE_Setting,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Six,VOICE_Motor,VOICE_SelfTest,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Seven,VOICE_BodyInduction,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag},
	{VOICE_PressPoundKey,VOICE_ToRepeatListening,DEF_VoiceSegmentEndFlag}
};

static uint16_t EngineeringModeMenuVoiceBuff[][8]=
{	
	{VOICE_Press,VOICE_One,VOICE_UnlockingDirect,VOICE_Setting,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Two,VOICE_Motor,VOICE_Torque,VOICE_Setting,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Three,VOICE_LockTongueReturn,VOICE_Time,VOICE_Setting,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Four,VOICE_LockDistance,VOICE_Setting,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Five,VOICE_SelfBounce,VOICE_Setting,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Six,VOICE_Motor,VOICE_SelfTest,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag},
	{VOICE_PressPoundKey,VOICE_ToRepeatListening,DEF_VoiceSegmentEndFlag}
};


static uint16_t MotorSettingMenuVoiceBuff[][8]=
{	
	{VOICE_Press,VOICE_One,VOICE_Lock,VOICE_Time,VOICE_Setting,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Two,VOICE_AutoLock,VOICE_Time,VOICE_Setting,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Three,VOICE_Motor,VOICE_SelfTest,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Four,VOICE_Motor,VOICE_Torque,VOICE_Setting,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag},
	{VOICE_PressPoundKey,VOICE_ToRepeatListening,DEF_VoiceSegmentEndFlag}
};


static uint16_t UnlockDirrectionSettingVoiceBuff[][4]=
{
	{VOICE_Press,VOICE_One,VOICE_LeftOpen,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Two,VOICE_RightOpen,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag},
	{VOICE_PressPoundKey,VOICE_ToRepeatListening,DEF_VoiceSegmentEndFlag}
};

static uint16_t MotorTorqueSettingVoiceBuff[][5]=
{
	{VOICE_Press,VOICE_One,VOICE_Small,VOICE_Torque,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Two,VOICE_Middle,VOICE_Torque,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Three,VOICE_Big,VOICE_Torque,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag},
	{VOICE_PressPoundKey,VOICE_ToRepeatListening,DEF_VoiceSegmentEndFlag}
};

static uint16_t UnlockTimeSettingVoiceBuff[][7]=
{
	{VOICE_Press,VOICE_Two,VOICE_Eight,VOICE_Adjust,VOICE_Lock,VOICE_Time,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag},
	{VOICE_PressPoundKey,VOICE_ToRepeatListening,DEF_VoiceSegmentEndFlag}
};
	
static uint16_t AutoLockDelayTimeSettingVoiceBuff[][7]=
{
	{VOICE_Press,VOICE_Two,VOICE_Eight,VOICE_Adjust,VOICE_AutoLock,VOICE_Time,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag},
	{VOICE_PressPoundKey,VOICE_ToRepeatListening,DEF_VoiceSegmentEndFlag}
};
	
static uint16_t BoltLockTimeSettingVoiceBuff[][5]=
{
	{VOICE_Press,VOICE_Two,VOICE_Eight,VOICE_Adjust,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag},
	{VOICE_PressPoundKey,VOICE_ToRepeatListening,DEF_VoiceSegmentEndFlag}
};

static uint16_t LockingTravelSettingVoiceBuff[][5]=
{
	{VOICE_Press,VOICE_Two,VOICE_Eight,VOICE_Adjust,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag},
	{VOICE_PressPoundKey,VOICE_ToRepeatListening,DEF_VoiceSegmentEndFlag}
};

static uint16_t AutoEjectSettingVoiceBuff[][5]=
{
	{VOICE_Press,VOICE_One,VOICE_Open,VOICE_SelfBounce,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Two,VOICE_Close,VOICE_SelfBounce,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag},
	{VOICE_PressPoundKey,VOICE_ToRepeatListening,DEF_VoiceSegmentEndFlag}
};

static uint16_t MotorSelfTestVoiceBuff[][4]=
{
	{VOICE_Motor,VOICE_SelfTest,VOICE_PleaseWait,DEF_VoiceSegmentEndFlag}
};

static uint16_t SystemVersionVoiceBuff[][10]=
{
	{VOICE_CharS,VOICE_Zero,VOICE_Zero,VOICE_Zero,VOICE_CharZ,VOICE_Zero,VOICE_Zero,VOICE_Zero,DEF_VoiceSegmentEndFlag},
	{VOICE_Zero,VOICE_Zero,VOICE_Zero,VOICE_Dot,VOICE_Zero,VOICE_Zero,VOICE_Zero,VOICE_Dot,DEF_VoiceSegmentEndFlag},
	{VOICE_Zero,VOICE_Zero,VOICE_Zero,VOICE_Dot,VOICE_Zero,VOICE_Zero,VOICE_Zero,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag},
	{VOICE_PressPoundKey,VOICE_ToRepeatListening,DEF_VoiceSegmentEndFlag}
};


	
static uint16_t VoiceStr_ConfirmOrExitDelete[]={VOICE_PressPoundKey,VOICE_ConfirmToDelete,VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag};
static uint16_t VoiceStr_ConfirmOrExitRestorFactoryDefault[]={VOICE_PressPoundKey,VOICE_RestoreFactoryDefault,VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag};
static uint16_t VoiceStr_RestorFactoryDefaultPleaseWait[]={VOICE_RestoreFactoryDefault,VOICE_PleaseWait,DEF_VoiceSegmentEndFlag};
static uint16_t VoiceStr_OperationSuccess[]={VOICE_Operation,VOICE_Success,DEF_VoiceSegmentEndFlag};
static uint16_t VoiceStr_OperationFail[]={VOICE_Operation,VOICE_Fail,DEF_VoiceSegmentEndFlag};
static uint16_t VoiceStr_HardwareNotSupportOperationFail[]={VOICE_HardwareNotSupported,VOICE_Operation,VOICE_Fail,DEF_VoiceSegmentEndFlag};
static uint16_t VoiceStr_RepeatedFingerprint[]={VOICE_Duplicate,VOICE_Fingerprint,DEF_VoiceSegmentEndFlag};
static uint16_t VoiceStr_RepeatedCard[]={VOICE_Duplicate,VOICE_Card,DEF_VoiceSegmentEndFlag};
static uint16_t VoiceStr_RepeatedPassword[]={VOICE_Duplicate,VOICE_Password,DEF_VoiceSegmentEndFlag};
static uint16_t VoiceStr_DeleteFail[]={VOICE_Delete,VOICE_Fail,DEF_VoiceSegmentEndFlag};
static uint16_t VoiceStr_DeleteSuccess[]={VOICE_Delete,VOICE_Success,DEF_VoiceSegmentEndFlag};
static uint16_t VoiceStr_PleaseInputPassword[]={VOICE_PleaseEnter,VOICE_Password,DEF_VoiceSegmentEndFlag};
static uint16_t VoiceStr_PleaseInputPasswordAgain[]={VOICE_Again,VOICE_Password,DEF_VoiceSegmentEndFlag};
static uint16_t VoiceStr_TimeSetting[]={VOICE_Time,VOICE_Setting,DEF_VoiceSegmentEndFlag};
static uint16_t VoiceStr_PleaseInputYear[]={VOICE_PleaseEnter,VOICE_Year,DEF_VoiceSegmentEndFlag};
static uint16_t VoiceStr_PleaseInputMonth[]={VOICE_PleaseEnter,VOICE_Month,DEF_VoiceSegmentEndFlag};
static uint16_t VoiceStr_PleaseInputDate[]={VOICE_PleaseEnter,VOICE_Date,DEF_VoiceSegmentEndFlag};
static uint16_t VoiceStr_PleaseInputHour[]={VOICE_PleaseEnter,VOICE_Hour,DEF_VoiceSegmentEndFlag};
static uint16_t VoiceStr_PleaseInputMinute[]={VOICE_PleaseEnter,VOICE_Minute,DEF_VoiceSegmentEndFlag};
static uint16_t VoiceStr_PleaseInputSecond[]={VOICE_PleaseEnter,VOICE_Second,DEF_VoiceSegmentEndFlag};
static uint16_t VoiceStr_PleaseInputSecondIdentity[]={VOICE_Beep,VOICE_PleaseInputSecondIdentity,DEF_VoiceSegmentEndFlag};
static uint16_t VoiceStr_FrmFunctionEnabled[]={VOICE_Face,VOICE_Opened,DEF_VoiceSegmentEndFlag};
static uint16_t VoiceStr_FrmFunctionDisbled[]={VOICE_Face,VOICE_Closed,DEF_VoiceSegmentEndFlag};
static uint16_t VoiceStr_UserIdIsNotExist[]={VOICE_User,VOICE_ID,VOICE_NotExist,DEF_VoiceSegmentEndFlag};

static uint16_t AutoLockOffVoiceStr[]={VOICE_AutoLock,VOICE_Close,DEF_VoiceSegmentEndFlag};
static uint16_t AutoEjectOnVoiceStr[]={VOICE_SelfBounce,VOICE_Opened,DEF_VoiceSegmentEndFlag};
static uint16_t AutoEjectOffVoiceStr[]={VOICE_SelfBounce,VOICE_Closed,DEF_VoiceSegmentEndFlag};
static uint16_t NoLogVoiceStr[]={VOICE_Not,VOICE_Log,DEF_VoiceSegmentEndFlag};

static uint16_t VoiceStr_RequestingRemoteUnlock[]={VOICE_RequestingRemoteUnlock,VOICE_PleaseWait,DEF_VoiceSegmentEndFlag};

static uint16_t VoiceStr_VerifySuccess[]={VOICE_Validation,VOICE_Success,DEF_VoiceSegmentEndFlag};

static uint16_t VoiceStr_AddNewMasterPleaseInputPasscode[]={VOICE_Add,VOICE_New,VOICE_Password,VOICE_Administrator,VOICE_PleaseEnter,VOICE_Password,DEF_VoiceSegmentEndFlag};



static struct
{
	uint8_t TotalMenuNum;
	uint8_t MenuPoint;
}VoiceMenuMgr;



const uint8_t InputUserIDStr[]={HZ_qing,HZ_shu,HZ_ru,HZ_bian,HZ_hao,HZ_end};
const uint8_t InputUserIDStrEn[]={"PLS input ID"};
const uint8_t IDisNotRegisteredStr[]={HZ_yong,HZ_hu,HZ_bian,HZ_hao,HZ_bu,HZ_cun,HZ_zaima,HZ_end};
const uint8_t IDisNotRegisteredStrEn[]={"ID is Invalid"};
const uint8_t IDisRegisteredStr[]={HZ_gai,HZ_bian,HZ_hao,HZ_yi,HZ_bei,HZ_shiyong,HZ_yong,HZ_end};
const uint8_t IDisRegisteredStrEn[]={"ID is be used"};

const uint8_t PleaseAddMasterStr[]={HZ_qing,HZ_xian,HZ_tian,HZ_jia,HZ_guan,HZ_li,HZ_yuan,HZ_end};
const uint8_t PleaseAddMasterStrEn[]={"PLS Add Admin."};


const uint8_t InputPasscodeStr[]={HZ_qing,HZ_shu,HZ_ru,HZ_mi,HZ_ma,HZ_end};
const uint8_t InputPasscodeStrEn[]={"PLS input PW"};
const uint8_t ConfirmPasscode[]={HZ_qing,HZ_que,HZ_ding,HZ_mi,HZ_ma,HZ_end};
const uint8_t ConfirmPasscodeEn[]={"Confirm PW"};


const uint8_t AutoEjectEnableStr[]={ZF_1,ZF_xiaoshudian,HZ_dakai,HZ_kai,HZ_zhidong,HZ_tan,HZ_end};
const uint8_t AutoEjectEnableStrEn[]={"1.Enable"};

const uint8_t AutoEjectDisableStr[]={ZF_2,ZF_xiaoshudian,HZ_guanbi,HZ_bi,HZ_zhidong,HZ_tan,HZ_end};
const uint8_t AutoEjectDisableStrEn[]={"2.Disable"};

const uint8_t FpUserStr[]={HZ_zhi,HZ_wen,HZ_yong,HZ_hu,HZ_end};
const uint8_t FpUserStrEn[]={"FP user"};

const uint8_t CardUserStr[]={HZ_ka,HZ_pian,HZ_yong,HZ_hu,HZ_end};
const uint8_t CardUserStrEn[]={"CD user"};

const uint8_t PasscodeUserStr[]={HZ_mi,HZ_ma,HZ_yong,HZ_hu,HZ_end};
const uint8_t PasscodeUserStrEn[]={"PW user"};

const uint8_t TemporaryPasswordStr[]={HZ_lin,HZ_shi,HZ_mi,HZ_ma,HZ_end};
const uint8_t TemporaryPasswordStrEn[]={"Temp PW"};

const uint8_t FaceUserStr[]={HZ_ren,HZ_lianbu,HZ_yong,HZ_hu,HZ_end};
const uint8_t FaceUserStrEn[]={"FC user"};


const uint8_t IDStr[]={HZ_bian,HZ_hao,ZF_maohao,HZ_end};
const uint8_t IDStrEn[]={"ID:"};

const uint8_t UserIDStr[]={HZ_yong,HZ_hu,HZ_bian,HZ_hao,ZF_maohao,HZ_end};
const uint8_t UserIDStrEn[]={"User ID:"};

const uint8_t NoEventLogStr[]={HZ_wu,HZ_ji,HZ_lu,HZ_end};
const uint8_t NoEventLogStrEn[]={"No Log"};

const uint8_t UserIsFullStr[]={HZ_yong,HZ_hu,HZ_yi,HZ_man,HZ_end};
const uint8_t UserIsFullStrEn[]={"User is full"};	


/*
const uint8_t TimeOutStr[]={HZ_chao,HZ_shi,HZ_end};
const uint8_t TimeOutStrEn[]={"Time out"};
*/

const uint8_t OperationSuccessStr[]={HZ_cao,HZ_zhuo,HZ_cheng,HZ_gong,HZ_end};
const uint8_t OperationSuccessStrEn[]={"Success"};

const uint8_t OperationFailStr[]={HZ_cao,HZ_zhuo,HZ_shibai,HZ_bai,HZ_end};
const uint8_t OperationFailStrEn[]={"Fail"};

const uint8_t Yes[]={HZ_kongge,HZ_shifou,HZ_kongge,HZ_end};
const uint8_t YesEn[]={"YES"};

const uint8_t No[]={HZ_kongge,HZ_fou,HZ_kongge,HZ_end};
const uint8_t NoEn[]={"NO"};

const uint8_t FaceRepeated[]={HZ_chong,HZ_fu,HZ_ren,HZ_lianbu,HZ_end};
const uint8_t FaceRepeatedEn[]={"Repeated Face"};

const uint8_t FingerprintRepeated[]={HZ_chong,HZ_fu,HZ_zhi,HZ_wen,HZ_end};
const uint8_t FingerprintRepeatedEn[]={"Repeated FP"};


const uint8_t CanNotDeleteLastOneMasterStr1[]={HZ_gai,HZ_guan,HZ_li,HZ_yuan,HZ_yi,HZ_dengru,HZ_ru,HZ_end};
const uint8_t CanNotDeleteLastOneMasterStr1En[]={"Admin. is login"};

const uint8_t CanNotDeleteLastOneMasterStr2[]={HZ_bu,HZ_neng,HZ_shan,HZ_chufa,HZ_end};
const uint8_t CanNotDeleteLastOneMasterStr2En[]={"Can't delete"};

const uint8_t ConfirmDeleteStr[]={HZ_anya,ZF_jinghao,HZ_que,HZ_renzhen,HZ_shan,HZ_chufa,HZ_end};	//按#确认删除
const uint8_t ConfirmDeleteStrEn[]={"Press # to cfm"}; //确认删除
const uint8_t AbortDeleteStr[]={HZ_anya,ZF_xinghao,HZ_fang,HZ_huijia,HZ_end};	//按*返回
const uint8_t AbortDeleteStrEn[]={"Press * to RTN"};	//按*返回


const uint8_t ConfirmRestoreFactoryDefaultStr1[]={HZ_anya,ZF_jinghao,HZ_hui,HZ_fu,HZ_chu,HZ_chang,HZ_end};
const uint8_t ConfirmRestoreFactoryDefaultStr2[]={HZ_anya,ZF_xinghao,HZ_fang,HZ_huijia,HZ_end};
const uint8_t ConfirmRestoreFactoryDefaultStr1En[]={"Press # to cfm"}; //确认删除
const uint8_t ConfirmRestoreFactoryDefaultStr2En[]={"Press * to RTN"};	//按*返回


const uint8_t RestoreFactoryDoingStr1[]={HZ_hui,HZ_fu,HZ_chu,HZ_chang,ZF_xiaoshudian,ZF_xiaoshudian,ZF_xiaoshudian,HZ_end};
const uint8_t RestoreFactoryDoingStr2[]={HZ_qing,HZ_shao,HZ_houniao,HZ_end};
const uint8_t RestoreFactoryDoingStr1En[]={"Restoring..."};
const uint8_t RestoreFactoryDoingStr2En[]={"Please Wait"};


const uint8_t UnlockLogStr[]={HZ_kai,HZ_suomen,HZ_ji,HZ_lu,HZ_end};
const uint8_t UnlockLogStrEn[]={"Log"};	

const uint8_t SingalModeStr[]={ZF_1,ZF_xiaoshudian,HZ_dan,HZ_yier,HZ_kai,HZ_suomen,HZ_kongge,HZ_end};
const uint8_t SingalModeStrEn[]={"1.Single"}; 
const uint8_t DoubleModeStr[]={ZF_2,ZF_xiaoshudian,HZ_zu,HZ_he,HZ_kai,HZ_suomen,HZ_kongge,HZ_end};
const uint8_t DoubleModeStrEn[]={"2.Combination"};	

const uint8_t AntiPryingEnableStr[]={ZF_1,ZF_xiaoshudian,HZ_fangzi,HZ_qiao,HZ_dakai,HZ_kai,HZ_kongge,HZ_end};
const uint8_t AntiPryingEnableStrEn[]={"1.Enable"}; 
const uint8_t AntiPryingDisableStr[]={ZF_2,ZF_xiaoshudian,HZ_fangzi,HZ_qiao,HZ_guanbi,HZ_bi,HZ_end};
const uint8_t AntiPryingDisableStrEn[]={"2.Disable"};


const uint8_t SystemNoSecondIdentityStr[]={HZ_xi,HZ_tong,HZ_wu,HZ_di,HZ_er,HZ_shen,HZ_fen,HZ_end};
const uint8_t SystemNoSecondIdentityStrEn[]={"No 2nd Identity"};

const uint8_t FpMasterIDRangeStrEn[]={"<1-  >:"};
const uint8_t FpUserIDRangeStrEn[]={"<  -   >:"};
const uint8_t CardUserIDRangeStrEn[]={"<1-   >:"};
const uint8_t PasscodeIDRangeStrEn[]={"< -  >:"};

const uint8_t SystemLockedStr[]={HZ_xi,HZ_tong,HZ_suomen,HZ_ding,HZ_end};
const uint8_t SystemLockedStrEn[]={"System Lock"};

const uint8_t PleaseSwingCardStr[]={HZ_qing,HZ_shua,HZ_ka,HZ_end};//请刷卡
const uint8_t PleaseSwingCardStrEn[]={"PLS swing card"};	
const uint8_t CardCIDisBeUsedStr[]={HZ_chong,HZ_fu,HZ_ka,HZ_hao,HZ_end};
const uint8_t CardCIDisBeUsedStrEn[]={"Repeated card"}; 

const uint8_t AntiPryingAlarmStr[]={HZ_fangzi,HZ_qiao,HZ_bao,HZ_jing,HZ_end};
const uint8_t AntiPryingAlarmStrEn[]={"Alarm"};
const uint8_t RemovalAlarmStr1[]={HZ_qing,HZ_shu,HZ_ru,HZ_zhengque,HZ_que,HZ_shen,HZ_fen,HZ_end}; 
const uint8_t RemovalAlarmStr1En[]={"Enter Identity"};	
const uint8_t RemovalAlarmStr2[]={HZ_tui,HZ_chu,HZ_end};		
const uint8_t RemovalAlarmStr2En[]={"to Removal"};

const uint8_t ReplaceBatteryStr1En[]={"Low Power,Pls"};
const uint8_t ReplaceBatteryStr2En[]={"Replace Battery"};

const uint8_t PleaseWaitStr[]={HZ_qing,HZ_shao,HZ_houniao,HZ_end};
const uint8_t PleaseWaitStrEn[]={"Please Wait"};

const uint8_t IdentifyFailStr[]={HZ_yan,HZ_zheng,HZ_shibai,HZ_bai,HZ_end};
const uint8_t IdentifyFailStrEn[]={"Identify Fail"};

const uint8_t PressPoundKeyToConfirmStr[]={HZ_anya,ZF_jinghao,HZ_que,HZ_renzhen,HZ_end};
const uint8_t PressAsteriskKeyToReturnStr[]={HZ_anya,ZF_xinghao,HZ_fang,HZ_huijia,HZ_end};
const uint8_t PressPoundKeyToConfirmStrEn[]={"Press # to cfm"}; //确认删除
const uint8_t PressAsteriskKeyToReturnStrEn[]={"Press * to RTN"};	//按*返回

const uint8_t InputErrorStr[]={HZ_shu,HZ_ru,HZ_cuo,HZ_wuhui,HZ_end};
const uint8_t InputErrorStrEn[]={"Input Error"};



const uint8_t MainMenuStr[][10]= 
{ 
	
	{ZF_1,ZF_xiaoshudian,HZ_yong,HZ_hu,HZ_guan,HZ_li,HZ_kongge,HZ_end}, 					//用户管理		
	{ZF_2,ZF_xiaoshudian,HZ_xi,HZ_tong,HZ_she,HZ_zhiwei,HZ_kongge,HZ_kongge,HZ_end},				//系统设定	
	{ZF_3,ZF_xiaoshudian,HZ_xinxi,HZ_xishiningren,HZ_cha,HZ_xun,HZ_kongge,HZ_kongge,HZ_end},		//信息查询
	{ZF_4,ZF_xiaoshudian,HZ_pei,HZ_wang,HZ_kongge,HZ_kongge,HZ_kongge,HZ_end}						//配网				
};
	
const uint8_t MainMenuStrEn[][16]= 
{ 
	{"1.User MGMT"},				//用户管理		
	{"2.System config "},			//系统设定	
	{"3.Info. Inquiry"},			//信息查询
	{"4.NetWork    "}				//配网
	
};

const uint8_t UserManagementMenuStr_WithFRM[][10]= 
{ 
	{ZF_1,ZF_xiaoshudian,HZ_ren,HZ_lianbu,HZ_yong,HZ_hu,HZ_guan,HZ_li,HZ_kongge,HZ_end}, 			//人脸用户管理	
	{ZF_2,ZF_xiaoshudian,HZ_zhi,HZ_wen,HZ_yong,HZ_hu,HZ_guan,HZ_li,HZ_kongge,HZ_end}, 					//指纹用户管理		
	{ZF_3,ZF_xiaoshudian,HZ_mi,HZ_ma,HZ_yong,HZ_hu,HZ_guan,HZ_li,HZ_kongge,HZ_end},						//密码用户管理		
	{ZF_4,ZF_xiaoshudian,HZ_ka,HZ_pian,HZ_yong,HZ_hu,HZ_guan,HZ_li,HZ_kongge,HZ_end}, 					//卡片用户管理 				
};

const uint8_t UserManagementMenuStrEn_WithFRM[][16]= 
{ 
	{"1.Face user  "},				//人脸用户管理				
	{"2.FP user    "},				//指纹用户管理		
	{"3.Password user"},			//密码用户管理		
	{"4.Card user   "},				//卡片用户管理 
};

const uint8_t UserManageMenuStr[][11]= 
{ 
	{ZF_1,ZF_xiaoshudian,HZ_zeng,HZ_jia,HZ_guan,HZ_li,HZ_yuan,HZ_yong,HZ_hu,HZ_kongge,HZ_end}, 			//增加管理员用户	
	{ZF_2,ZF_xiaoshudian,HZ_zeng,HZ_jia,HZ_pu,HZ_tongyong,HZ_yong,HZ_hu,HZ_kongge,HZ_end}, 				//增加普通用户		
	{ZF_3,ZF_xiaoshudian,HZ_shan,HZ_chufa,HZ_yong,HZ_hu,HZ_kongge,HZ_end},								//删除用户			
};

const uint8_t UserManageMenuStrEn[][24]= 
{ 
	{"1.Add administrator user"},				//增加管理员用户				
	{"2.Add user    "},				//增加普通用户		
	{"3.Delete user  "},			//删除用户		
};

const uint8_t UserManagementMenuStr[][10]= 
{ 
	{ZF_1,ZF_xiaoshudian,HZ_zhi,HZ_wen,HZ_yong,HZ_hu,HZ_guan,HZ_li,HZ_kongge,HZ_end}, 					//指纹用户管理		
	{ZF_2,ZF_xiaoshudian,HZ_mi,HZ_ma,HZ_yong,HZ_hu,HZ_guan,HZ_li,HZ_kongge,HZ_end},						//密码用户管理		
	{ZF_3,ZF_xiaoshudian,HZ_ka,HZ_pian,HZ_yong,HZ_hu,HZ_guan,HZ_li,HZ_kongge,HZ_end}, 					//卡片用户管理 				
};
const uint8_t UserManagementMenuStrEn[][16]= 
{ 
	{"1.FP user    "},				//指纹用户管理		
	{"2.Password user "},			//密码用户管理		
	{"3.Card user   "},				//卡片用户管理 
};


const uint8_t FaceMenuStr[][11]= 
{ 
	{ZF_1,ZF_xiaoshudian,HZ_tian,HZ_jia,HZ_guan,HZ_li,HZ_yuan,HZ_ren,HZ_lianbu,HZ_end},		//添加管理员人脸
	{ZF_2,ZF_xiaoshudian,HZ_tian,HZ_jia,HZ_yong,HZ_hu,HZ_ren,HZ_lianbu,HZ_end}, 			//添加新用户人脸
	{ZF_3,ZF_xiaoshudian,HZ_shan,HZ_chufa,HZ_dan,HZ_yier,HZ_ren,HZ_lianbu,HZ_end}, 			//删除单个人脸				  
};	
const uint8_t FaceMenuStrEn[][18]= 
{ 
	{"1.Add Admin."},		//添加管理员人脸
	{"2.Add user"},			//添加用户人脸
	{"3.Del one user"},		//删除单个人脸
};

const uint8_t AdminMenuStr[][11]= 
{ 
	{ZF_1,ZF_xiaoshudian,HZ_lu,HZ_ru,HZ_zhi,HZ_wen,HZ_mi,HZ_ma,HZ_ka,HZ_pian,HZ_end},		//录入指纹，密码，卡片	
	{ZF_2,ZF_xiaoshudian,HZ_lu,HZ_ru,HZ_ren,HZ_lianbu,HZ_end}, 			//录入人脸			  
};	
const uint8_t AdminMenuStrEn[][27]= 
{ 
	{"1.Add FP,Password,Card"},		//录入指纹，密码，卡片
	{"2.Add Face"},			//录入人脸
};

const uint8_t RegisterFPCPageStr[][14]= 
{ 
	{HZ_kongge,HZ_kongge,HZ_qing,HZ_shu,HZ_ru,HZ_bian,HZ_hao,HZ_end},		//请输入编号
	{HZ_kongge,HZ_kongge,HZ_kongge,HZ_kongge,ZF_0,ZF_0,ZF_1,HZ_end},							//001
	{HZ_kongge,HZ_end},
	{HZ_kongge,HZ_fanwei,HZ_wei,ZF_maohao,ZF_zhongkuohaozuo,ZF_0,ZF_0,ZF_1,ZF_hengan,ZF_0,ZF_0,ZF_9,ZF_zhongkuohaoyou,HZ_end},	//范围[001-009]
			  
};	
const uint8_t RegisterFPCPageStrEn[][20]= 
{ 
	{" Please input numer "},		//请输入编号
	{"         001        "},			//001
	{"                    "},	
	{"Scope:[001-009]"},			//范围[001-009]	
};

const uint8_t RegisterUserFPCPageStr[][14]= 
{ 
	{HZ_kongge,HZ_kongge,HZ_qing,HZ_shu,HZ_ru,HZ_bian,HZ_hao,HZ_end},		//请输入编号
	{HZ_kongge,HZ_kongge,HZ_kongge,HZ_kongge,ZF_0,ZF_1,ZF_0,HZ_end},							//010
	{HZ_kongge,HZ_end},
	{HZ_kongge,HZ_fanwei,HZ_wei,ZF_maohao,ZF_zhongkuohaozuo,ZF_0,ZF_1,ZF_0,ZF_hengan,ZF_2,ZF_0,ZF_0,ZF_zhongkuohaoyou,HZ_end},	//范围[010-200]
			  
};	
const uint8_t RegisterUserFPCPageStrEn[][20]= 
{ 
	{" Please input numer "},		//请输入编号
	{"         010        "},			//010
	{"                    "},	
	{"Scope:[010-200]"},			//范围[010-200]	
};

const uint8_t DeleteUserPageStr[][14]= 
{ 
	{HZ_kongge,HZ_kongge,HZ_qing,HZ_shu,HZ_ru,HZ_bian,HZ_hao,HZ_end},		//请输入编号
	{HZ_kongge,HZ_kongge,HZ_kongge,HZ_kongge,ZF_0,ZF_0,ZF_1,HZ_end},							//001
	{HZ_kongge,HZ_end},
	{HZ_kongge,HZ_fanwei,HZ_wei,ZF_maohao,ZF_zhongkuohaozuo,ZF_0,ZF_0,ZF_1,ZF_hengan,ZF_2,ZF_0,ZF_0,ZF_zhongkuohaoyou,HZ_end},	//范围[001-200]
			  
};	
const uint8_t DeleteUserPageStrEn[][20]= 
{ 
	{" Please input numer "},		//请输入编号
	{"         001        "},			//010
	{"                    "},	
	{"Scope:[001-200]"},			//范围[010-200]	
};

const uint8_t DeleteAllUserPageStr[][16]= 
{ 
	{HZ_kongge,HZ_kongge,HZ_kongge,HZ_quan,HZ_bufen,HZ_shan,HZ_chufa,HZ_end},		//请输入编号
	{HZ_kongge,HZ_end},
	{HZ_kongge,HZ_end},
	{ZF_xinghao,HZ_qu,HZ_xiao,HZ_kongge,HZ_kongge,HZ_kongge,HZ_kongge,ZF_jinghao,HZ_que,HZ_renzhen,HZ_end},	//范围[001-200]
			  
};	
const uint8_t DeleteAllUserPageStrEn[][30]= 
{ 
	{" All Delete "},		//请输入编号
	{"                    "},
	{"                    "},	
	{"                    "},
	{"*Cancle              #Confirm"},
};

const uint8_t OperateSuccessStr[][16]= 
{ 
	{HZ_kongge,HZ_end},
	{HZ_kongge,HZ_kongge,HZ_cao,HZ_zhuo,HZ_cheng,HZ_gong,HZ_end},		//操作成功
			  
};	
const uint8_t OperateSuccessStrEn[][30]= 
{ 
	{"                    "},
	{"                    "},
	{"  Operate  Success  "},	
};

const uint8_t DeleteUserMenuStr[][8]= 
{ 
	{ZF_1,ZF_xiaoshudian,HZ_anya,HZ_bian,HZ_hao,HZ_shan,HZ_chufa,HZ_end},		//按编号删除
	{ZF_2,ZF_xiaoshudian,HZ_quan,HZ_bufen,HZ_shan,HZ_chufa,HZ_end}, 			//全部删除			  
};	
const uint8_t DeleteUserMenuStrEn[][18]= 
{ 
	{"1.Delete by number"},		//按编号删除
	{"2.All Delete"},			//全部删除	
};

const uint8_t FpMenuStr[][11]= 
{ 
	{ZF_1,ZF_xiaoshudian,HZ_tian,HZ_jia,HZ_guan,HZ_li,HZ_yuan,HZ_zhi,HZ_wen,HZ_end},		//添加管理员指纹
	{ZF_2,ZF_xiaoshudian,HZ_tian,HZ_jia,HZ_yong,HZ_hu,HZ_zhi,HZ_wen,HZ_end},						//添加用户指纹
	{ZF_3,ZF_xiaoshudian,HZ_tian,HZ_jia,HZ_xie,HZ_po,HZ_zhi,HZ_wen,HZ_end},							//添加胁迫指纹
	{ZF_4,ZF_xiaoshudian,HZ_shan,HZ_chufa,HZ_zhi,HZ_wen,HZ_end}, 				//删除指纹
};
	
const uint8_t FpMenuStrEn[][18]= 
{ 
	{"1.Add Admin. FP"},		//添加新管理员
	{"2.Add user FP"},			//添加新用户
	{"3.Add SU FP"},				//添加胁迫指纹	
	{"4.Del user"},			//删除用户
};

const uint8_t FpDeleteMenuStr[][11]= 
{ 
	{ZF_1,ZF_xiaoshudian,HZ_shan,HZ_chufa,HZ_dan,HZ_yier,HZ_yong,HZ_hu,HZ_end}, 				//删除单一用户
	{ZF_2,ZF_xiaoshudian,HZ_shan,HZ_chufa,HZ_suo,HZ_you,HZ_yong,HZ_hu,HZ_end},					//删除所有用户		
	{ZF_3,ZF_xiaoshudian,HZ_shan,HZ_chufa,HZ_suo,HZ_you,HZ_xie,HZ_po,HZ_end}					//删除所有胁迫	
};
	
const uint8_t FpDeleteMenuStrEn[][18]= 
{ 	
	{"1.Del one user"},			//删除已有用户
	{"2.Del all user"}, 		//删除所有用户		
	{"3.Del all SU FP"} 	   //删除所有胁迫指纹	
};


const uint8_t CardUserMenuStr[][11]= 
{ 
	{ZF_1,ZF_xiaoshudian,HZ_tian,HZ_jia,HZ_ka,HZ_pian,HZ_yong,HZ_hu,HZ_kongge,HZ_kongge,HZ_end}, 		//添加新用户
	{ZF_2,ZF_xiaoshudian,HZ_shan,HZ_chufa,HZ_dan,HZ_yier,HZ_ka,HZ_pian,HZ_end}, 										//删除已有用户				   
	{ZF_3,ZF_xiaoshudian,HZ_shan,HZ_chufa,HZ_suo,HZ_you,HZ_ka,HZ_pian,HZ_end},											//删除所有用户				   
};	
const uint8_t CardUserMenuStrEn[][18]= 
{ 
	{"1.Add card user"},					//添加新用户
	{"2.Del card user"},					//删除已有用户				   
	{"3.Del all cards"},					//删除所有用户				   
};	


const uint8_t PassCodeMenuStr[][11]= 
{ 
	{ZF_1,ZF_xiaoshudian,HZ_tian,HZ_jia,HZ_guan,HZ_li,HZ_yuan,HZ_mi,HZ_ma,HZ_end},	//添加管理员密码
	{ZF_2,ZF_xiaoshudian,HZ_tian,HZ_jia,HZ_yong,HZ_hu,HZ_mi,HZ_ma,HZ_end}, 					//添加新用户密码
	{ZF_3,ZF_xiaoshudian,HZ_shan,HZ_chufa,HZ_dan,HZ_yier,HZ_mi,HZ_ma,HZ_end}, 			//删除已有用户密码
	{ZF_4,ZF_xiaoshudian,HZ_shan,HZ_suo,HZ_you,HZ_yong,HZ_hu,HZ_mi,HZ_ma,HZ_end}		//删除所有用户密码							  
};	
const uint8_t PassCodeMenuStrEn[][18]= 
{ 
	{"1.Add PW Admin."},		//添加新管理员
	{"2.Add PW user"},			//添加新用户
	{"3.Del one user"},			//删除已有用户	
	{"4.Del all user"},			//删除所有用户
};	



const uint8_t InfoInquiryMenuStr[][9]= 
{ 
		{ZF_1,ZF_xiaoshudian,HZ_ji,HZ_lu,HZ_cha,HZ_xun,HZ_end},					//记录查询
		{ZF_2,ZF_xiaoshudian,HZ_nei,HZ_cun,HZ_cha,HZ_xun,HZ_kongge,HZ_end}, 	//内存用量
		{ZF_3,ZF_xiaoshudian,HZ_ban,HZ_ben,HZ_xinxi,HZ_xishiningren,HZ_end},		//版本信息
		{ZF_4,ZF_xiaoshudian,HZ_hui,HZ_fu,HZ_chu,HZ_chang,HZ_she,HZ_zhiwei,HZ_end}	//恢复出厂设置
};	
uint8_t InfoInquiryMenuStrEn[][16]= 
{ 
		{"1.Event Log"},			//
		{"2.Memory Usage"},			//
		{"3.System Version"},		//
		{"4.Factory DFLT."}       //恢复出厂设置
};		


/*
const uint8_t EventLogMenuStr[][10]= 
{ 
		{ZF_1,ZF_xiaoshudian,HZ_anya,HZ_shun,HZ_xu,HZ_cha,HZ_xun,HZ_end},			//按顺序查询
		{ZF_2,ZF_xiaoshudian,HZ_anya,HZ_ri,HZ_qi,HZ_cha,HZ_xun,HZ_end},				//按日期查询
		{ZF_3,ZF_xiaoshudian,HZ_shan,HZ_chufa,HZ_ji,HZ_lu,HZ_end}							//删除记录
};	
uint8_t EventLogMenuStrEn[][16]= 
{ 
		{"1.Sequence check"},			//按顺序查询
		{"2.Check by date "},			//按日期查询
		{"3.Delete all log"}			//删除记录
};		
*/
const uint8_t EventLogMenuStr[][10]= 
{ 
		{ZF_1,ZF_xiaoshudian,HZ_cha,HZ_xun,HZ_ji,HZ_lu,HZ_end},					//按顺序查询
		{ZF_2,ZF_xiaoshudian,HZ_shan,HZ_chufa,HZ_ji,HZ_lu,HZ_end}				//删除记录
};	
uint8_t EventLogMenuStrEn[][16]= 
{ 
		{"1.Query Log"},			//按顺序查询
		{"2.Delete log"}			//删除记录
};


const uint8_t SytemConfigMenuStr[][9]= 
{ 
		{ZF_1,ZF_xiaoshudian,HZ_shi,HZ_jian,HZ_she,HZ_zhiwei,HZ_end}, 		//时间设置
		{ZF_2,ZF_xiaoshudian,HZ_yuyan,HZ_yin,HZ_she,HZ_zhiwei,HZ_end}, 		//语音设置
		{ZF_3,ZF_xiaoshudian,HZ_men,HZ_suomen,HZ_she,HZ_zhiwei,HZ_end},		//门锁设置
		{ZF_4,ZF_xiaoshudian,HZ_dian,HZ_jiqi,HZ_she,HZ_zhiwei,HZ_end},		//电机设置??
};	
const uint8_t SytemConfigMenuStrEn[][18]= 
{ 
		{"1.Time setting   "},			//时间设置
		{"2.Voice setting  "},			//语言设置
		{"3.Lock setting   "},			//门锁设置
		{"4.Motor Setting  "}, 			//电机设置
};	

const uint8_t VoiceSettingMenuStr[][9]= 
{ 
		{ZF_1,ZF_xiaoshudian,HZ_yuyan,HZ_yanyu,HZ_she,HZ_zhiwei,HZ_end}, 			//语言设置
		{ZF_2,ZF_xiaoshudian,HZ_yin,HZ_liang,HZ_tiao,HZ_jie,HZ_end},				//音量调节
};	
const uint8_t VoiceSettingMenuStrEn[][18]= 
{ 
		{"1.Language       "},			//语言设置
		{"2.Volume         "},			//音量调节
	
};

/*		
const uint8_t DoorLockSettingMenuStr[][10]= 
{ 
	{ZF_1,ZF_xiaoshudian,HZ_kai,HZ_suomen,HZ_mo,HZ_si,HZ_she,HZ_zhiwei,HZ_end},				// 开锁模式设置
	{ZF_2,ZF_xiaoshudian,HZ_fangzi,HZ_qiao,HZ_bao,HZ_jing,HZ_she,HZ_zhiwei,HZ_end},		// 防撬报警设置
	{ZF_3,ZF_xiaoshudian,HZ_ren,HZ_tiyan,HZ_gan,HZ_yingda,HZ_she,HZ_zhiwei,HZ_end},		// 人体感应设置
};
const uint8_t DoorLockSettingMenuStrEn[][18]= 
{ 

	{"1.Unlock mode   "},			    // 开锁模式设置
	{"2.Anti prying   "},					// 防撬报警设置
	{"3.Body induction"},	       	// 人体感应设置
};
*/

const uint8_t VolumeAdjustStr[][7]={	
								{ZF_1,ZF_xiaoshudian,HZ_da,HZ_yin,HZ_liang,ZF_kongge,HZ_end},
								{ZF_2,ZF_xiaoshudian,HZ_zhong,HZ_yin,HZ_liang,HZ_kongge,HZ_end},
								{ZF_3,ZF_xiaoshudian,HZ_xiao,HZ_yin,HZ_liang,HZ_kongge,HZ_end},
								{ZF_4,ZF_xiaoshudian,HZ_jingzhi,HZ_yin,ZF_kongge,HZ_end}
							  };
const uint8_t VolumeAdjustStrEn[][9]={
								{"1.High  "},
								{"2.Middle"},
								{"3.Low   "},
								{"4.Mute  "}
							  };

const uint8_t DoorLockSettingMenuStr[][10]= 
{ 
	{ZF_1,ZF_xiaoshudian,HZ_kai,HZ_suomen,HZ_mo,HZ_si,HZ_she,HZ_zhiwei,HZ_end},			// 开锁模式设置
	{ZF_2,ZF_xiaoshudian,HZ_fangzi,HZ_qiao,HZ_bao,HZ_jing,HZ_she,HZ_zhiwei,HZ_end},		// 防撬报警设置
	{ZF_3,ZF_xiaoshudian,HZ_ren,HZ_tiyan,HZ_gan,HZ_yingda,HZ_she,HZ_zhiwei,HZ_end}		// 人体感应设置
};
	
const uint8_t DoorLockSettingMenuStrEn[][18]= 
{ 
	{"1.Unlock mode"},			    // 开锁模式设置
	{"2.Anti prying"},				// 防撬报警设置
	{"3.Body Induction"},			// 人体感应设置
};
	
const uint8_t BodyInductionAdjustStr[][7]={	
	{ZF_1,ZF_xiaoshudian,HZ_guanbi,HZ_bi,HZ_end},
	{ZF_2,ZF_xiaoshudian,HZ_xiao,HZ_end},
	{ZF_3,ZF_xiaoshudian,HZ_da,HZ_end}
  };
const uint8_t BodyInductionAdjustStrEn[][9]={
	{"1.Off"},
	{"2.Low"},
	{"3.High"}
  };

const uint8_t EngineeringModeMenuStr_WithFRM[][10]= 
{ 
	{ZF_1,ZF_xiaoshudian,HZ_kai,HZ_suomen,HZ_fangx,HZ_xiang,HZ_end},							//开锁方向
	{ZF_2,ZF_xiaoshudian,HZ_dian,HZ_jiqi,HZ_niu,HZ_niuli,HZ_end},								//电机扭力
	{ZF_3,ZF_xiaoshudian,HZ_xielv,HZ_se,HZ_fanzheng,HZ_suomen,HZ_shi,HZ_jian,HZ_end},			//斜舌反锁时间
	{ZF_4,ZF_xiaoshudian,HZ_fanzheng,HZ_suomen,HZ_xing,HZ_chengxu,HZ_kongge,HZ_kongge,HZ_end},	//反锁行程
	{ZF_5,ZF_xiaoshudian,HZ_zhidong,HZ_tan,HZ_end},												//自弹
	{ZF_6,ZF_xiaoshudian,HZ_dian,HZ_jiqi,HZ_zhidong,HZ_xue,HZ_xiguan,HZ_end},					//电机自学习
	{ZF_7,ZF_xiaoshudian,HZ_ren,HZ_tiyan,HZ_gan,HZ_yingda,HZ_she,HZ_zhiwei,HZ_end}				//人体感应
};	
const uint8_t EngineeringModeMenuStrEN_WithFRM[][18]= 
{ 

	{"1.UnLock Direct "},			  	//开锁方向
	{"2.Motor Torque  "},				//电机扭力
	{"3.lock tongue   "},				//斜舌反锁时间
	{"4.Locking DIS   "},				//反锁行程
	{"5.Self Bounce   "},				//自弹
	{"6.Motor Selftest"},				//电机自学习
	{"7.Body Induction"}				//人体感应
};

const uint8_t EngineeringModeMenuStr[][10]= 
{ 
	{ZF_1,ZF_xiaoshudian,HZ_kai,HZ_suomen,HZ_fangx,HZ_xiang,HZ_end},							//开锁方向
	{ZF_2,ZF_xiaoshudian,HZ_dian,HZ_jiqi,HZ_niu,HZ_niuli,HZ_end},								//电机扭力
	{ZF_3,ZF_xiaoshudian,HZ_xielv,HZ_se,HZ_fanzheng,HZ_suomen,HZ_shi,HZ_jian,HZ_end},			//斜舌反锁时间
	{ZF_4,ZF_xiaoshudian,HZ_fanzheng,HZ_suomen,HZ_xing,HZ_chengxu,HZ_kongge,HZ_kongge,HZ_end},	//反锁行程
	{ZF_5,ZF_xiaoshudian,HZ_zhidong,HZ_tan,HZ_end},												//自弹
	{ZF_6,ZF_xiaoshudian,HZ_dian,HZ_jiqi,HZ_zhidong,HZ_xue,HZ_xiguan,HZ_end}					//电机自学习
};	
const uint8_t EngineeringModeMenuStrEN[][18]= 
{ 

	{"1.UnLock Direct "},			  	//开锁方向
	{"2.Motor Torque  "},				//电机扭力
	{"3.lock tongue   "},				//斜舌反锁时间
	{"4.Locking DIS   "},				//反锁行程
	{"5.Self Bounce   "},				//自弹
	{"6.Motor Selftest"}				//电机自学习
};


const uint8_t AutoMotorSetingMenuStr[][10]= 
{ 
	{ZF_1,ZF_xiaoshudian,HZ_guanbi,HZ_men,HZ_shi,HZ_jian,HZ_end},	    					//关门时间	
	{ZF_2,ZF_xiaoshudian,HZ_zhidong,HZ_dong,HZ_shang,HZ_suomen,HZ_shi,HZ_jian,HZ_end},		//自动上锁时间
	{ZF_3,ZF_xiaoshudian,HZ_dian,HZ_jiqi,HZ_zhidong,HZ_xue,HZ_xiguan,HZ_end},				//电机自学习
	{ZF_4,ZF_xiaoshudian,HZ_dian,HZ_jiqi,HZ_niu,HZ_niuli,HZ_end}								//电机扭力
};	
const uint8_t AutoMotorSetingMenuStrEN[][18]= 
{ 
	{"1.Unlock time   "},	       		//关门时间
	{"2.Auto Lock Time"},				//自动关锁时间
	{"3.Motor Selftest"},				//电机自学习
	{"4.Motor Torque  "}				//电机扭力
};



static struct
{
	uint8_t TotalMenuNum;
	uint8_t StartMenuLine;
	uint8_t MenuPoint;
	enum
	{
		MoveDown=0,
		MoveUp=1
	}MoveDirect;
}MenuMgr;

struct
{
	enum
	{
		SetYearMajor=0,
		SetYearMinor=1,
		SetMonthMajor,
		SetMonthMinor,
		SetDateMajor,
		SetDateMinor,
		SetHourMajor,
		SetHourMinor,
		SetMinuteMajor,
		SetMinuteMinor,
		SetSecondMajor,
		SetSecondMinor
	}Status;

}TimeSettingMgr;

struct
{
	//uint8_t StartPage;
	//uint8_t StartColumn;
	uint16_t Value;
	uint8_t InputPoint;
	uint8_t InputNum;
	enum
	{
		InputIdle=0,
		InputStart=1,
		InputEnd,
		InputExit
	}Status;

}DataInputMgr;

static struct
{
	uint8_t InputBuff[16];
	uint8_t PasscodeLen;
	uint8_t Point;
	enum
		{
			PasscodeInputIdle=0,
			PasscodeInputStart=1,
			PasscodeInputEnd,
			PasscodeInputExit
		}Status;
}PasscodeInputMgr;

screen_t LastScreen;

static systemtime_t TempSystemTime;

static uint8_t GUI_TimeCnt;

static uint8_t GUI_ToggleFlag_1s;
static uint8_t GUI_ToggleFlag_05s;
static bool_t GUI_Flag_RefreshLCD;

static keycode_t gui_keycode;

//static uint8_t GUI_CardDetectCntDownTimer;

static uint8_t PasscodeBUFF1[12];



void CardUserIdentify(void);

uint8_t CompareCardIDwithMemory(uint8_t *Point);
uint8_t PasscodeIdendify(uint8_t *BUFF1);
void CheckIfFpUserIsRegistered(uint16_t USERID);
void GUI_CreatAndSaveLog(EventLogType_t LogType);
uint8_t AppPasscodeIdentify(uint8_t BUFF[]);
void DeleteAllPasscodeUserfromMemory(void);
void DeleteAllPasscodeMasterfromMemory(void);
status_t SaveFPMserialNumberToMemory(void);
status_t Get_FPM_SerialNumber(uint8_t buff[]);
bool_t CheckFPMserialNumberIsMatch(uint8_t buff[]);
void GoIntoMotorSettingMenu_Init(void);
void GoIntoEngineeringModeMenu_Init(void);
uint8_t TranslateNumberToVoice(uint8_t value);
uint8_t GetUserIDbyFaceTemplateID(uint16_t TemplateID);
void FaceUserIdentify(void);
void ShowEventLogBySequence(void);

void ReadFPuserIdListFromEEPROM(void);
void WriteFPuserIdListToEEPROM(void);
void ResetFPuserIdListInEEPROM(void);
void AddFPuserIdToList(uint16_t FPuserID);
void DeleteFPuserIdFromList(uint16_t FPuserID);
void DeleteAllFPuserIdFromList(void);
void DeleteAllStressFPuserIdFromList(void);

void VoiceReportCurrentLanguage(void);

void Config_AntiPrying_Interrupt(void)
{
	if (PickAlarmEnableMgr.Enable == bTRUE)
	{
		EnableAntiPrying_Interrupt();
	}
	else{
		DisableAntiPrying_Interrupt();
	}
}

uint8_t BCDcodeToHEX(uint8_t Indata)
{
	uint8_t result;
	result = ((Indata/16)*10)+Indata%16;
	return result;
}

uint8_t HEXtoBCDcode(uint8_t Indata)
{
	uint8_t result;
	result = ((Indata/10)<<4)+Indata%10; 
	return result;
}


uint8_t BcdCode_Plus(uint8_t BcdCode)
{
	uint8_t temp;
	if ((BcdCode&0x0F)==9)
	{
		temp = (BcdCode&0xF0)+0x10;
	}
	else
	{
		temp = BcdCode+1;
	}
	return temp;
}

uint8_t BcdCode_Minus(uint8_t BcdCode)
{
	uint8_t temp;
	if (BcdCode==0)
	{
		return 0;
	}
	if ((BcdCode&0x0F)==0)
	{
		temp = (BcdCode&0xF0)-0x10 + 9;
	}
	else
	{
		temp = BcdCode-1;
	}
	return temp;
}

uint8_t BcdMaxDays_in_Month(uint8_t year, uint8_t month)
{
       	uint8_t monttbuffer[12] = {0x31, 0x28, 0x31, 0x30, 0x31, 0x30, 0x31, 0x31, 0x30, 0x31, 0x30, 0x31};
		uint16_t y;
		uint8_t m;
		y=((year/16)*10)+(year%16)+2000;
		m = ((month/16)*10)+(month%16);
        if (((y % 4 == 0) && (y % 100 != 0)) 
			|| (y % 400 == 0)
			)
        {
                monttbuffer[1] = 0x29; 
		}
			
        return monttbuffer[m-1];
}

bool_t IfSystemIsInFactoryDefaultStatus(void)
{
	if ( FrmMgr.FrmFunctionConfig == FRM_Enabled )
	{
		if ( ( CheckMemoryMgr.FpUserNum == 0x00 )
				&&(CheckMemoryMgr.FpMasterNum == 0x00 )
				&&(CheckMemoryMgr.CardUserNum == 0x00 )
				&&(CheckMemoryMgr.PasscodeMasterNum == 0x00 )
				&&(CheckMemoryMgr.PasscodeUserNum == 0x00 )
				&&(CheckMemoryMgr.FaceMasterNum == 0x00 )
				&&(CheckMemoryMgr.FaceUserNum == 0x00 )
				)
		{
			return bTRUE;
		}
		else
		{
			return bFALSE;
		}
	}
	else
	{
		if ( ( CheckMemoryMgr.FpUserNum == 0x00 )
			&&(CheckMemoryMgr.FpMasterNum == 0x00 )
			&&(CheckMemoryMgr.CardUserNum == 0x00 )
			&&(CheckMemoryMgr.PasscodeMasterNum == 0x00 )
			&&(CheckMemoryMgr.PasscodeUserNum == 0x00 )
			)
		{
			return bTRUE;
		}
		else
		{
			return bFALSE;
		}
	}
}
bool_t IfSystemIsNoMaster(void)
{
	if ( FrmMgr.FrmFunctionConfig == FRM_Enabled )
	{
		if ((CheckMemoryMgr.FpMasterNum == 0x00 )
			&&(CheckMemoryMgr.PasscodeMasterNum == 0x00 )
			&&(CheckMemoryMgr.FaceMasterNum == 0x00 )
			)
		{
			return bTRUE;
		}
		else
		{
			return bFALSE;
		}
	}
	else
	{
		if ( (CheckMemoryMgr.FpMasterNum == 0x00 )
			&&(CheckMemoryMgr.PasscodeMasterNum == 0x00 )
			)
		{
			return bTRUE;
		}
		else
		{
			return bFALSE;
		}
	}
}

bool_t IfSystemWithoutSecondIdentity(void)
{
	if  ( (((CheckMemoryMgr.FpUserNum+CheckMemoryMgr.FpMasterNum+CheckMemoryMgr.StressFpUserNum) == 0x00 )&&((CheckMemoryMgr.PasscodeMasterNum + CheckMemoryMgr.PasscodeUserNum)== 0x00))
		||(((CheckMemoryMgr.FpUserNum+CheckMemoryMgr.FpMasterNum+CheckMemoryMgr.StressFpUserNum) == 0x00 )&&(CheckMemoryMgr.CardUserNum == 0x00 ))
		||(((CheckMemoryMgr.PasscodeMasterNum + CheckMemoryMgr.PasscodeUserNum)== 0x00)&&(CheckMemoryMgr.CardUserNum == 0x00 ))
		)
	{
		return bTRUE;
	}
	else
	{
		return bFALSE;
	}
}
bool_t CompareTwoArrayIsSame(uint8_t Point1[],uint8_t Point2[],uint8_t Lenth)
{
	uint8_t i;
	for (i=0;i<Lenth;i++)
	{
		if (Point1[i]!=Point2[i]){
			return bFALSE;
		}
	}
	return bTRUE;
}

void UnlockModeAutoCalculate(void)
{
	if	( IfSystemWithoutSecondIdentity() == bTRUE )
	{
		UserIdentifyResultMgr.UnlockingMode = SingalMode;
		EEPROM_WriteSequential(UnlockModeStartAddr,&UserIdentifyResultMgr.UnlockingMode,1);
	}
}
bool_t is_valid_date(uint8_t year, uint8_t month, uint8_t date)
{
       	uint8_t monttbuffer[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
		uint16_t y;
		uint8_t m,d;
		y=((year/16)*10)+(year%16)+2000;
		m = ((month/16)*10)+(month%16);
		d = ((date/16)*10) + (date%16);
        if (((y % 4 == 0) && (y % 100 != 0)) 
			|| (y % 400 == 0)
			)
        {
                monttbuffer[1] = 29; 
		}
        if ( (m > 0) 
			&& (m < 13)
			&& (d > 0) 
			&& (d <= monttbuffer[m - 1])
			)
        {
			return bTRUE;
		}
		else{
			return bFALSE;
		}
}

unsigned long mktime(unsigned int year0, unsigned int mon0,
        unsigned int day, unsigned int hour,
        unsigned int min, unsigned int sec)
{
    unsigned int mon = mon0, year = year0;

    /* 1..12 -> 11,12,1..10 */
    if (0 >= (int) (mon -= 2)) {
        mon += 12;    /* Puts Feb last since it has leap day */
        year -= 1;
    }

    return ((((unsigned long)
          (year/4 - year/100 + year/400 + 367*mon/12 + day) +
          year*365 - 719499
        )*24 + hour /* now have hours */
      )*60 + min /* now have minutes */
    )*60 + sec; /* finally seconds */
}


void GoIntoShowSystemVersion(void)
{
	VoiceMenuMgr.MenuPoint = 0;
	VoiceMenuMgr.TotalMenuNum = 5;
	CurrentScreen = SCREEN_SystemVersion;
	
	SystemVersionVoiceBuff[0][1] = TranslateNumberToVoice(DEF_CustomerNumber/100);
	SystemVersionVoiceBuff[0][2] = TranslateNumberToVoice(DEF_CustomerNumber%100/10);
	SystemVersionVoiceBuff[0][3] = TranslateNumberToVoice(DEF_CustomerNumber%10);
	SystemVersionVoiceBuff[0][5] = TranslateNumberToVoice(DEF_ModelNumber/100);
	SystemVersionVoiceBuff[0][6] = TranslateNumberToVoice(DEF_ModelNumber%100/10);
	SystemVersionVoiceBuff[0][7] = TranslateNumberToVoice(DEF_ModelNumber%10);	

	//Main Board Hardware Version
	SystemVersionVoiceBuff[1][0] = TranslateNumberToVoice(DEF_HardwareVerion/100);
	SystemVersionVoiceBuff[1][1] = TranslateNumberToVoice(DEF_HardwareVerion%100/10);
	SystemVersionVoiceBuff[1][2] = TranslateNumberToVoice(DEF_HardwareVerion%10);
	//Main Board Firmware Version
	SystemVersionVoiceBuff[1][4] = TranslateNumberToVoice(DEF_FirmwareVerion/100);
	SystemVersionVoiceBuff[1][5] = TranslateNumberToVoice(DEF_FirmwareVerion%100/10);
	SystemVersionVoiceBuff[1][6] = TranslateNumberToVoice(DEF_FirmwareVerion%10);

	//Driver Board Hardware Version
	SystemVersionVoiceBuff[2][0]= TranslateNumberToVoice(DriverBoardVersion.HWversion/100);
	SystemVersionVoiceBuff[2][1]= TranslateNumberToVoice(DriverBoardVersion.HWversion%100/10);
	SystemVersionVoiceBuff[2][2]= TranslateNumberToVoice(DriverBoardVersion.HWversion%10);
	//Driver Board Firmware Version
	SystemVersionVoiceBuff[2][4]= TranslateNumberToVoice(DriverBoardVersion.FWversion/100);
	SystemVersionVoiceBuff[2][5]= TranslateNumberToVoice(DriverBoardVersion.FWversion%100/10);
	SystemVersionVoiceBuff[2][6]= TranslateNumberToVoice(DriverBoardVersion.FWversion%10);	
}
void ShowSystemVersion(void)
{

	const uint8_t SystemVersionStr[]={HZ_xi,HZ_tong,HZ_ban,HZ_ben,HZ_end};
	const uint8_t SystemVersionStrEn[]={"System Version"};
	const uint8_t FPM_Version[]={"FPM:126"};
	uint8_t ModelStr[]={"SxxxZxxx"};
	uint8_t VersionStr[]={"100.100.100.100"};


	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,SystemVersionVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}

	if (SystemLanguage == Chinese){
		DisHZ16x14Str(0,32,SystemVersionStr,NormalDisplay);
	}
	else{
		DisEN16x8Str(0,10,SystemVersionStrEn,NormalDisplay);
	}

	ModelStr[1]=DEF_CustomerNumber/100+0x30;
	ModelStr[2]=DEF_CustomerNumber%100/10+0x30;
	ModelStr[3]=DEF_CustomerNumber%10+0x30;
	ModelStr[5]=DEF_ModelNumber/100+0x30;
	ModelStr[6]=DEF_ModelNumber%100/10+0x30;
	ModelStr[7]=DEF_ModelNumber%10+0x30;	
	DisEN16x8Str(3,24,ModelStr,NormalDisplay);
	
	//Main Board Hardware Version
	VersionStr[0]=DEF_HardwareVerion/100+0x30;
	VersionStr[1]=DEF_HardwareVerion%100/10+0x30;
	VersionStr[2]=DEF_HardwareVerion%10+0x30;
	//Main Board Firmware Version
	VersionStr[4]=DEF_FirmwareVerion/100+0x30;
	VersionStr[5]=DEF_FirmwareVerion%100/10+0x30;
	VersionStr[6]=DEF_FirmwareVerion%10+0x30;

	//Driver Board Hardware Version
	VersionStr[8]=DriverBoardVersion.HWversion/100+0x30;
	VersionStr[9]=DriverBoardVersion.HWversion%100/10+0x30;
	VersionStr[10]=DriverBoardVersion.HWversion%10+0x30;
	//Driver Board Firmware Version
	VersionStr[12]=DriverBoardVersion.FWversion/100+0x30;
	VersionStr[13]=DriverBoardVersion.FWversion%100/10+0x30;
	VersionStr[14]=DriverBoardVersion.FWversion%10+0x30;
	DisEN16x8Str(5,0,VersionStr,NormalDisplay);

	

	//DisEN16x8Str(6,0,FPM_Version,NormalDisplay);
	
  
}

/****************************************************************/
/****************************************************************/
/****************************************************************/
/****************************************************************/
void SystemConfigSave(void)
{

	EEPROM_WriteSequential(BodyInductionStartAddr,&BodyInductionMgr.SensingDistanceLevel,1);
	EEPROM_WriteSequential(SystemLanguageStartAddr,&SystemLanguage,1);
	EEPROM_WriteSequential(UnlockModeStartAddr,&UserIdentifyResultMgr.UnlockingMode,1);
	EEPROM_WriteSequential(VolumeValueStartAddr,&VoiceMgr.volume,1);
	EEPROM_WriteSequential(PickAlarmSwitchStartAddr,&PickAlarmEnableMgr.Enable,1);
	
}
/****************************************************************/
/****************************************************************/
/****************************************************************/
/****************************************************************/
void AutoMotorMgrConfigSave(void)
{
	EEPROM_WriteSequential(AutoMotorMgrUnlockTimeAddr,&AutoMotorMgr.UnlockTime,1);
	EEPROM_WriteSequential(AutoMotorMgrAutoLockTimeAddr,&AutoMotorMgr.AutoLockTime,1);
	EEPROM_WriteSequential(AutoMotorMgrLockDirectionAddr,&AutoMotorMgr.LockDirection,1);
	EEPROM_WriteSequential(AutoMotorMgrTorqueAddr,&AutoMotorMgr.TorqueLevel,1);
	EEPROM_WriteSequential(AutoMotorMgrBoltLockTimeAddr,&AutoMotorMgr.BoltLockTime,1);
}


void ReadAutoMotorMgrConfig(void)
{
	uint8_t temp;
	
	EEPROM_ReadSequential(AutoMotorMgrUnlockTimeAddr,&temp,1);
	if ((temp<2)||(temp>10))
	{
		#ifdef ProjectIs_AutoBarLock_S58Z07
		AutoMotorMgr.UnlockTime=0x03;//default is 5s
		#else
		AutoMotorMgr.UnlockTime=0x05;//default is 5s
		#endif
		
	}else{
		AutoMotorMgr.UnlockTime = temp;
	}	
	EEPROM_ReadSequential(AutoMotorMgrAutoLockTimeAddr,&temp,1);
	if (((temp>0x00)&&(temp<5))||(temp>20)){
		AutoMotorMgr.AutoLockTime=0x00;//default is 5s
		DEBUG_MARK;
	}else{
		AutoMotorMgr.AutoLockTime = temp;
		DEBUG_MARK;
	}
	EEPROM_ReadSequential(AutoMotorMgrLockDirectionAddr,&temp,1);
	if ((temp!=LEFTOPEN)&&(temp!=RIGHTOPEN)){
		AutoMotorMgr.LockDirection=LEFTOPEN;//default is 
	}else{
		AutoMotorMgr.LockDirection = temp;
	}
	EEPROM_ReadSequential(AutoMotorMgrTorqueAddr,&temp,1);
	if ((temp<LowTorque)||(temp>LargeTorque)){
		AutoMotorMgr.TorqueLevel=MiddleTorque;//default is 5s
	}else{
		AutoMotorMgr.TorqueLevel = temp;
	}
	EEPROM_ReadSequential(AutoMotorMgrBoltLockTimeAddr,&temp,1);
	if ((temp<2)||(temp>24)){
		AutoMotorMgr.BoltLockTime=0x06;//default is 300ms
	}else{
		AutoMotorMgr.BoltLockTime = temp;
	}

	EEPROM_ReadSequential(AutoMotorMgrLockingTravelAddr,&temp,1);
	if ((temp>0)&&(temp<9)){
		AutoMotorMgr.LockingTravel=temp;
	}else{
		AutoMotorMgr.LockingTravel = 5;	//default is 8
	}

	EEPROM_ReadSequential(AutoMotorMgrAutoEjectAddr,&temp,1);
	if (temp == 0x01){
		AutoMotorMgr.AutoEject=temp;
	}else{
		AutoMotorMgr.AutoEject = 0x00;	//default is bfalse
	}
	
	
}
/****************************************************************/
/****************************************************************/
/****************************************************************/
/****************************************************************/

void AutoMotorMgrInitConfigReset(void)
{
    AutoMotorMgr.UnlockTime=2;
	AutoMotorMgr.AutoLockTime=5;
	AutoMotorMgr.LockDirection=LEFTOPEN;
	AutoMotorMgr.TorqueLevel = MiddleTorque;
	AutoMotorMgr.BoltLockTime=6;	

	AutoMotorMgrConfigSave();

	ComPort_SetPost_Parameter();
}

void BodyInductionConfigLoad(void)
{
	static uint8_t temp;
	EEPROM_ReadSequential(BodyInductionStartAddr,&temp,1);
	if (temp < 3 )
	{
		BodyInductionMgr.SensingDistanceLevel = temp;
	}
	else
	{
	  	BodyInductionMgr.SensingDistanceLevel = SensingDistanceL1;
	}
}

void FrmFunctionConfigLoad(void)
{
	uint8_t temp;
	
	EEPROM_ReadSequential(FrmFunctionConfigAddr,&temp,1);

	#ifdef Function_FaceRecoginition
		#ifdef Function_FaceRecoginitionSwitchedByNFC
		if (temp == FRM_Disabled )
		{
			FrmMgr.FrmFunctionConfig = FRM_Disabled;
		}
		else
		{
		  	FrmMgr.FrmFunctionConfig = FRM_Enabled;
		}
		#endif
	#else
	
		FrmMgr.FrmFunctionConfig = FRM_Disabled;
	
	#endif
}
void FrmFunctionConfigSave(void)
{
	EEPROM_WriteSequential(FrmFunctionConfigAddr,&FrmMgr.FrmFunctionConfig,1);
}

	
/****************************************************************/
/****************************************************************/
/****************************************************************/
/****************************************************************/
void SystemConfigLoad(void)
{
	uint8_t temp;

	FrmFunctionConfigLoad();
  	BodyInductionConfigLoad();
	
	EEPROM_ReadSequential(SystemLanguageStartAddr,&temp,1);
	if (temp == English)
	{
        SystemLanguage = English;
	}
    else
    {
      SystemLanguage = Chinese;
    }
	
	EEPROM_ReadSequential(UnlockModeStartAddr,&temp,1);
	if (temp == DoubleMode)
	{
            UserIdentifyResultMgr.UnlockingMode = DoubleMode;
	}
    else
    {
      UserIdentifyResultMgr.UnlockingMode = SingalMode;
    }
	
	EEPROM_ReadSequential(VolumeValueStartAddr,&temp,1);
	if (temp < 4 )		//from 0~4
	{
    	VoiceMgr.volume = temp;
	}
    else
    {
    	#ifdef ProjectIs_AutoBarLock_S70Z01
		VoiceMgr.volume = 1;
		#else
      	VoiceMgr.volume = 3;
		#endif
    }

	if ( VoiceMgr.volume > 0 ){
		VoiceMgr.Enable = bTRUE;
	}
	else{
		VoiceMgr.Enable = bFALSE;
	}
		
	EEPROM_ReadSequential(PickAlarmSwitchStartAddr,&temp,1);
	
	#ifdef Function_AntiPryingDefaultDisabled
	if (temp == bTRUE)
	{
    	PickAlarmEnableMgr.Enable = bTRUE;
	}
    else
    {
      	PickAlarmEnableMgr.Enable = bFALSE;
    }
	#else
	if (temp == bFALSE)
	{
    	PickAlarmEnableMgr.Enable = bFALSE;
	}
    else
    {
      	PickAlarmEnableMgr.Enable = bTRUE;
    }
	#endif

	Config_AntiPrying_Interrupt();
	
}
/****************************************************************/
/****************************************************************/
/****************************************************************/
/****************************************************************/
void ReadLockBrand(void)
{
	BrandType_t tempBrand;
	
	EEPROM_ReadSequential(LockBrandStartAddr,&tempBrand,1);
	
	if ( tempBrand == SmallBrand )
	{
		EEPROM_ReadSequential(LockBrandStartAddr+1,&LockBrand.LockBrandDisDataBuff[0],224);
		LockBrand.BrandType = SmallBrand;
	}
	else if ( tempBrand == BigBrand )
	{
		EEPROM_ReadSequential(LockBrandStartAddr+1,&LockBrand.LockBrandDisDataBuff[0],512);
		LockBrand.BrandType = BigBrand;
	}
	else if ( tempBrand == DefaultBigBrand )
	{
		LockBrand.BrandType = DefaultBigBrand;
	}
	else
	{
		LockBrand.BrandType = DefaultSmallBrand;
	}	
	
}
/****************************************************************/
/****************************************************************/
/****************************************************************/
/****************************************************************/
void WriteLockBrand(void)
{	
	uint8_t Flag;
	LockBrand.GotBrandData = bFALSE;
	if ( MFC_ReadLockBrandData(&LockBrand.LockBrandDisDataBuff[0]) == S_SUCCESS )
	{
		EEPROM_WriteSequential(LockBrandStartAddr,&LockBrand.BrandType,1);
		
		if ( LockBrand.BrandType == SmallBrand )
		{
			EEPROM_WriteSequential(LockBrandStartAddr+1,&LockBrand.LockBrandDisDataBuff[0],224);
		}
		else if ( LockBrand.BrandType == BigBrand )
		{
			EEPROM_WriteSequential(LockBrandStartAddr+1,&LockBrand.LockBrandDisDataBuff[0],512);
		}
		LockBrand.GotBrandData = bTRUE;
	}
}


/****************************************************************/
/****************************************************************/
/****************************************************************/
/****************************************************************/
void SystemConfigReset(void)
{
	UserIdentifyResultMgr.UnlockingMode = SingalMode;
	
	VoiceMgr.Enable = bTRUE;
	
	AntiPryingMgr.AntiPryingTrigger = bFALSE;		//Refresh Anti-Prying Trigger

	#ifdef Function_AntiPryingDefaultDisabled
		PickAlarmEnableMgr.Enable = bFALSE;
	#else
		PickAlarmEnableMgr.Enable = bTRUE;
	#endif

	BodyInductionMgr.SensingDistanceLevel = SensingDistanceL1;

	#ifdef ProjectIs_AutoBarLock_S70Z01
	VoiceMgr.volume = 1;
	#else
  	VoiceMgr.volume = 3;
	#endif
	
	Config_AntiPrying_Interrupt();

	SystemConfigSave();
	
	//AutoMotorMgrInitConfigReset();

}
/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowLockBrand(void)
{
	const uint8_t Welcome_Str[8]={HZ_huan,HZ_yingjie,HZ_shiyong,HZ_yong,HZ_zhineng,HZ_neng,HZ_suomen,HZ_end};
	const uint8_t Welcome_StrEn[]={"WELCOME"};

	if ( LockBrand.BrandType == SmallBrand )
	{
		DisImage_RAM(6,8,112,16,&LockBrand.LockBrandDisDataBuff[0],NormalDisplay);
	}
	else if ( LockBrand.BrandType == BigBrand )
	{
		DisImage_RAM(2,0,128,32,&LockBrand.LockBrandDisDataBuff[0],NormalDisplay);
	}
	else if ( LockBrand.BrandType == DefaultBigBrand )
	{
		if ( SystemLanguage == Chinese ) 
		{
			DisImage(2,10,108,32,&Icon_Welcome[0],NormalDisplay);
		}
		else
		{
			DisImage(2,15,98,32,&Icon_WelcomeEn[0],NormalDisplay);
		}
	}	
	else //if ( LockBrand.BrandType == DefaultSmallBrand )
	{
		if ( SystemLanguage == Chinese ) 
		{
			DisHZ16x14Str(6,16,Welcome_Str,NormalDisplay);
		}
		else
		{
			DisEN16x8Str(6,36,Welcome_StrEn,NormalDisplay);
		}
	}


}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void GoIntoMainScreen_WithIdentifyInit(void)
{
	PasscodeInputMgr.Point = 0x00;
	PasscodeInputMgr.PasscodeLen = 16;
	PasscodeUserIdentifyMgr.Status = PasscodeIdentifyIdle;
	
	FpIdentifyMgr.Status = FPMcmdStart;
	
	CardIdentifyMgr.Status = ReadingCardID;
	CardIdentifyMgr.CardDetectIntervalTimeCnt = 0;

	
	FaceIdentifyMgr.Status = FrmIdentifyStart;
	CurrentScreen = SCREEN_Main;
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void GUI_RefreshSleepTime(void)
{
	if (( CurrentScreen == SCREEN_Main )||( CurrentScreen == SCREEN_LowBattery ))
	{
		SystemPowerMgr.SleepDelayTimerCnt = Def_GuiTimeDelayCnt10s;
	}
	else
	{
		SystemPowerMgr.SleepDelayTimerCnt = Def_GuiTimeDelayCnt30s;
	}
}


void GUI_SetFPM_LED(uint8_t mode,uint8_t startcolor,uint8_t stopcolor,uint8_t cycles )
{
	#ifdef Function_FPMBreathingLed
		uint8_t i;
		DEBUG_MARK;
		if (( FpmLEDsMgr.OperationMode != mode )
			||(FpmLEDsMgr.StartColor != startcolor )
			||(FpmLEDsMgr.StopColor != stopcolor )
			||(FpmLEDsMgr.Cycles != cycles )
			)
		{
			if ( FpmAckMgr.Status == WaitACK )		//wait for last command ACK
			{
				for (i=0;i<25;i++)
				{
					Hardware_DelayMs(10);
					FPM_Mgr_Task();
					if ( FpmAckMgr.Status == GotACK )
					{
						break;
					}
				}
			}
			FpmAckMgr.Status = WaitACK;
			FpmAckMgr.TimeOutCnt = DEF_FpmAckTimeoutTime;
			FPM_SetBreathingLED(mode,startcolor,stopcolor,cycles); 	////Green LED Breathing
			for (i=0;i<25;i++)
			{
				Hardware_DelayMs(10);
				FPM_Mgr_Task();
				if ( FpmAckMgr.Status == GotACK )
				{

					break;
				}
			}
			FpmLEDsMgr.OperationMode = mode;
			FpmLEDsMgr.StartColor = startcolor;
			FpmLEDsMgr.StopColor = stopcolor;
			FpmLEDsMgr.Cycles = cycles;
		}
	#endif
}


/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowIdentifySuccessPage(void)
{
	const uint8_t DoorLockedStr[]={ZF_xiaoyuhao,HZ_yi,HZ_guanbi,HZ_suomen,ZF_dayuhao,HZ_end};
	const uint8_t DoorLockedStrEn[]={"<Closed>"};	
	const uint8_t DoorUnlockedStr[]={ZF_xiaoyuhao,HZ_yi,HZ_kai,HZ_suomen,ZF_dayuhao,HZ_end};
	const uint8_t DoorUnlockedStrEn[]={"<Opened>"}; 
	
	const uint8_t DoorLockingStr[]={HZ_zhengque,HZ_zaima,HZ_shang,HZ_suomen,HZ_end};
	const uint8_t DoorLockingStrEn[]={"<Closing>"}; 
	const uint8_t DoorUnlockingStr[]={HZ_zhengque,HZ_zaima,HZ_kai,HZ_suomen,HZ_end};
	const uint8_t DoorUnlockingStrEn[]={"Opening"};	
	
	const uint8_t InitialStatusStr[]={HZ_chuzhong,HZ_shizhong,HZ_zhuang,HZ_tai,HZ_end};
	const uint8_t InitialStatusStrEn[]={"Initial"}; 

	const uint8_t RemoteUnlockStr[]={HZ_yuanchu,HZ_chengxu,HZ_kai,HZ_suomen,HZ_end};
	const uint8_t RemoteUnlockStrEn[]={"Remote"};

	const uint8_t ButtonUnlockStr[]={HZ_kai,HZ_suomen,HZ_anya,HZ_niu,HZ_end};
	const uint8_t ButtonUnlockStrEn[]={"Button"};
	
	uint8_t i;

	
	if ( DisplayDoorStatusMgr.Status == DoorOpenInit )
	{	
		if (UserIdentifyResultMgr.IdentifyType != InsideOpenDoor)
		{
			StrongUnlockMgr.CurrentUTCtime = mktime(2000+BCDcodeToHEX(SystemTime.year), BCDcodeToHEX(SystemTime.month),
			BCDcodeToHEX(SystemTime.date), BCDcodeToHEX(SystemTime.hour),
			BCDcodeToHEX(SystemTime.minute), BCDcodeToHEX(SystemTime.second));

			if ((StrongUnlockMgr.CurrentUTCtime > StrongUnlockMgr.LastUTCtime )
				&&((StrongUnlockMgr.CurrentUTCtime-StrongUnlockMgr.LastUTCtime)<DEF_StrongUnlockingIntervalLimited)
				)
			{
				StrongUnlockMgr.TrigStrongUnlocking = bTRUE;
				DEBUG_MARK;
			}
			else
			{
				StrongUnlockMgr.TrigStrongUnlocking = bFALSE;
				DEBUG_MARK;
			}
		}
		else
		{
			StrongUnlockMgr.TrigStrongUnlocking = bFALSE;
		}
		
		if ( FrmMgr.FrmFunctionConfig == FRM_Enabled )
		{
			FaceRecognition_PowerDown();
			//FaceRecognition_HardwarePowerOff();
		}
	
		#if defined (Function_YouzhiyunjiaWifi) || defined (Function_TuyaWifi)
		
		if (UserIdentifyResultMgr.IdentifyType != InsideOpenDoor)
		{
			DEBUG_MARK;
			
			GUI_CreatAndSaveLog(OpenDoor);	

			if (LogMgr.NewLog.UserType == ONLINEPASSCODEUSER )
			{
				ComPort_SetPost_OpenDoor(LogMgr.NewLog.UserType,WifiMgr.WifiOnlinePasswordVerifyMgr.UserID);
			}
			else
			{
				ComPort_SetPost_OpenDoor(LogMgr.NewLog.UserType,LogMgr.NewLog.UserID);
			}	
			Wifi_PostUnlockStatusCmd(LogMgr.NewLog.UserType,LogMgr.NewLog.UserID);
		}

		#else
		if (  (UserIdentifyResultMgr.IdentifyType != RemoteUnlock )
		&&(UserIdentifyResultMgr.IdentifyType != InsideOpenDoor)
		)
		{
			DEBUG_MARK;
			
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_VerifySuccess);
			
			GUI_CreatAndSaveLog(OpenDoor);	

			if (LogMgr.NewLog.UserType == ONLINEPASSCODEUSER )
			{
				ComPort_SetPost_OpenDoor(LogMgr.NewLog.UserType,WifiMgr.WifiOnlinePasswordVerifyMgr.UserID);
			}
			else
			{
				ComPort_SetPost_OpenDoor(LogMgr.NewLog.UserType,LogMgr.NewLog.UserID);
			}
		}
		#endif		

		DisplayDoorStatusMgr.Status = OpenDoorWaitForMotorStatusChangeFromStandby;
		DisplayDoorStatusMgr.MotorStatusIsChangedFromStandby = bFALSE;
		DisplayDoorStatusMgr.TimeCnt = Def_GuiTimeDelayCnt4s;
		UserIdentifyResultMgr.TimeCnt = Def_IdentifySuccessScreenTimeLimited;

		//GUI_SetFPM_LED(DEF_FpmLedMode_Breath,DEF_FpmLedColor_Green,DEF_FpmLedColor_Green,255);
		
	}

	
	else if (DisplayDoorStatusMgr.Status == DoorCloseInit)
	{
		if ( FrmMgr.FrmFunctionConfig == FRM_Enabled )
		{
			FaceRecognition_PowerDown();
			//FaceRecognition_HardwarePowerOff();
		}
		
		if ( UserIdentifyResultMgr.IdentifyType == OutsideCloseDoor )
		{
			ComPort_SetPost_CloseDoor();
		}
		DisplayDoorStatusMgr.Status = CloseDoorWaitForMotorStatusChangeFromStandby;
		DisplayDoorStatusMgr.MotorStatusIsChangedFromStandby = bFALSE;
		DisplayDoorStatusMgr.TimeCnt = Def_GuiTimeDelayCnt4s;
		UserIdentifyResultMgr.TimeCnt = Def_IdentifySuccessScreenTimeLimited;
	}
	
	else if (( DisplayDoorStatusMgr.Status == OpenDoorWaitForMotorStatusChangeFromStandby )
		||( DisplayDoorStatusMgr.Status == CloseDoorWaitForMotorStatusChangeFromStandby ))
	{
		if ( ComportMgr.DoorStatus != Standby)
		{
			DisplayDoorStatusMgr.MotorStatusIsChangedFromStandby = bTRUE;
		}
		if ( DisplayDoorStatusMgr.TimeCnt >0 )
		{
			DisplayDoorStatusMgr.TimeCnt--;
		}
		else
		{
			DisplayDoorStatusMgr.MotorStatusIsChangedFromStandby = bTRUE;
			if ( DisplayDoorStatusMgr.Status == OpenDoorWaitForMotorStatusChangeFromStandby )
			{
				DisplayDoorStatusMgr.Status = DoorOpenning;
			}
			else
			{
				DisplayDoorStatusMgr.Status = DoorClosing;
			}
		}
	}
		
	if (DisplayDoorStatusMgr.MotorStatusIsChangedFromStandby == bTRUE)
	{
		if (ComportMgr.DoorStatus == Openning) 
		{
			DisplayDoorStatusMgr.Status = DoorOpenning;
		}
		else if (ComportMgr.DoorStatus == Open) 
		{
			if ( DisplayDoorStatusMgr.Status == DoorOpenning )
			{
				PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_DoorOpened);
				GUI_SetFPM_LED(DEF_FpmLedMode_Off,DEF_FpmLedColor_All,DEF_FpmLedColor_All,1);
			}
			DisplayDoorStatusMgr.Status = DoorOpened;
		}
		else if (ComportMgr.DoorStatus == OpenedWaitClose) 
		{
			if ( DisplayDoorStatusMgr.Status == DoorOpenning )
			{
				PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_DoorOpened);
				GUI_SetFPM_LED(DEF_FpmLedMode_Off,DEF_FpmLedColor_All,DEF_FpmLedColor_All,1);
			}
			DisplayDoorStatusMgr.Status = DoorOpenedWaitClose;
		}
		else if (ComportMgr.DoorStatus == Closing) 
		{
			if ( DisplayDoorStatusMgr.Status != DoorClosing )
			{
				DisplayDoorStatusMgr.Status = DoorClosing;
				if (BatteryMgr.BatteryType == LiBattery)
				{
					PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_DoingLock);
				}
			}
		}
		else if (ComportMgr.DoorStatus == Close) 
		{
			DisplayDoorStatusMgr.Status = DoorClosed;
		}
		else if (ComportMgr.DoorStatus == Standby) 
		{
			if ( (DisplayDoorStatusMgr.Status == DoorClosing)
				||( DisplayDoorStatusMgr.Status == DoorClosed))
			{
				if (DisplayDoorStatusMgr.Status == DoorClosing)
				{
					PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_DoorClosed);
				}
				UserIdentifyResultMgr.TimeCnt = Def_GuiTimeDelayCnt3s;
				SystemPowerMgr.SleepDelayTimerCnt = Def_GuiTimeDelayCnt3s;			
				DisplayDoorStatusMgr.Status = DisplayCloseDoorEnd;
	
			}
			else if (( DisplayDoorStatusMgr.Status == DoorOpenning)
					||( DisplayDoorStatusMgr.Status == DoorOpened)
					||( DisplayDoorStatusMgr.Status == DoorOpenedWaitClose)
					)
			{
				//PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_DoorOpened);
				UserIdentifyResultMgr.TimeCnt = Def_GuiTimeDelayCnt3s;
				SystemPowerMgr.SleepDelayTimerCnt = Def_GuiTimeDelayCnt3s;	
				DisplayDoorStatusMgr.Status = DisplayOpenDoorEnd;
			}
			else
			{
				DEBUG_MARK;
			}
			
		}
		else
		{
			DEBUG_MARK;
		}
	}

	if (( AwakeDisplayMgr.DisplayType != DisplayHide)
			&&(BatteryMgr.BatteryType == LiBattery)
		)
	{
		GUI_SetFPM_LED(DEF_FpmLedMode_Breath,DEF_FpmLedColor_Green,DEF_FpmLedColor_Green,255);
	}
	else
	{
		GUI_SetFPM_LED(DEF_FpmLedMode_Off,DEF_FpmLedColor_All,DEF_FpmLedColor_All,255);
	}

	if (( AwakeDisplayMgr.DisplayType != DisplayHide)
		||(BatteryMgr.BatteryType == LiBattery)
		)
	{	
		if ( ( DisplayDoorStatusMgr.Status == OpenDoorWaitForMotorStatusChangeFromStandby )
			||( DisplayDoorStatusMgr.Status == DoorOpenning )
			||( DisplayDoorStatusMgr.Status == DoorOpened )
			||( DisplayDoorStatusMgr.Status == DoorOpenedWaitClose )
			||( DisplayDoorStatusMgr.Status == DisplayOpenDoorEnd )
			)
		{

			DisImage(0,72,52,40,Icon_Unlocked,NormalDisplay);
			
			if ( UserIdentifyResultMgr.IdentifyType == CARD )
			{
				if (SystemLanguage == Chinese){
					DisHZ16x14Str(2,0,CardUserStr,NormalDisplay);
					DisHZ16x14Str(4,0,IDStr,NormalDisplay);
				}
				else{
					DisEN16x8Str(2,0,CardUserStrEn,NormalDisplay);
					DisEN16x8Str(4,0,IDStrEn,NormalDisplay);
				}
			
				DisDigital16x8Str(4,36,UserIdentifyResultMgr.CardUserID,NormalDisplay);
			}
			else if ( UserIdentifyResultMgr.IdentifyType == FINGERPRINT )
			{
				if (SystemLanguage == Chinese){
					DisHZ16x14Str(2,0,FpUserStr,NormalDisplay);
					DisHZ16x14Str(4,0,IDStr,NormalDisplay);
				}
				else{
					DisEN16x8Str(2,0,FpUserStrEn,NormalDisplay);
					DisEN16x8Str(4,0,IDStrEn,NormalDisplay);
				}
			
				DisDigital16x8Str(4,36,UserIdentifyResultMgr.FPUserID,NormalDisplay);
			}
			else if (UserIdentifyResultMgr.IdentifyType == PASSCODE )
			{

				if (SystemLanguage == Chinese){
					DisHZ16x14Str(2,0,PasscodeUserStr,NormalDisplay);
					DisHZ16x14Str(4,0,IDStr,NormalDisplay);
				}
				else{
					DisEN16x8Str(2,0,PasscodeUserStrEn,NormalDisplay);
					DisEN16x8Str(4,0,IDStrEn,NormalDisplay);
				}
				
				DisDigital16x8Str(4,36,UserIdentifyResultMgr.PasscodeUserID,NormalDisplay);
			}
			else if (UserIdentifyResultMgr.IdentifyType == FACE )
			{

				if (SystemLanguage == Chinese){
					DisHZ16x14Str(2,0,FaceUserStr,NormalDisplay);
					DisHZ16x14Str(4,0,IDStr,NormalDisplay);
				}
				else{
					DisEN16x8Str(2,0,FaceUserStrEn,NormalDisplay);
					DisEN16x8Str(4,0,IDStrEn,NormalDisplay);
				}
				
				DisDigital16x8Str(4,36,UserIdentifyResultMgr.FaceUserID,NormalDisplay);
			}
			else if (UserIdentifyResultMgr.IdentifyType == ONLINEPASSCODE )
			{
				if (SystemLanguage == Chinese){
					DisHZ16x14Str(2,0,TemporaryPasswordStr,NormalDisplay);
					DisHZ16x14Str(4,0,IDStr,NormalDisplay);
				}
				else{
					DisEN16x8Str(2,0,TemporaryPasswordStrEn,NormalDisplay);
					DisEN16x8Str(4,0,IDStrEn,NormalDisplay);
				}

				DisDigital16x8Str(4,36,UserIdentifyResultMgr.PasscodeUserID,NormalDisplay);
			}
			
			else if (UserIdentifyResultMgr.IdentifyType == FINGERPRINTandCARD ){
				if (SystemLanguage == Chinese){
					DisHZ16x14Str(0,0,FpUserStr,NormalDisplay);
					DisHZ16x14Str(2,0,IDStr,NormalDisplay);
				}
				else{
					DisEN16x8Str(0,0,FpUserStrEn,NormalDisplay);
					DisEN16x8Str(2,0,IDStrEn,NormalDisplay);
				}
				DisDigital16x8Str(2,36,UserIdentifyResultMgr.FPUserID,NormalDisplay);
				
				if (SystemLanguage == Chinese){
					DisHZ16x14Str(4,0,CardUserStr,NormalDisplay);
					DisHZ16x14Str(6,0,IDStr,NormalDisplay);
				}
				else{
					DisEN16x8Str(4,0,CardUserStrEn,NormalDisplay);
					DisEN16x8Str(6,0,IDStrEn,NormalDisplay);
				}
				DisDigital16x8Str(6,36,UserIdentifyResultMgr.CardUserID,NormalDisplay); 	
			}
			else if (UserIdentifyResultMgr.IdentifyType == FINGERPRINTandPASSCODE ){
				if (SystemLanguage == Chinese){
					DisHZ16x14Str(0,0,FpUserStr,NormalDisplay);
					DisHZ16x14Str(2,0,IDStr,NormalDisplay);
				}
				else{
					DisEN16x8Str(0,0,FpUserStrEn,NormalDisplay);
					DisEN16x8Str(2,0,IDStrEn,NormalDisplay);
				}
				DisDigital16x8Str(2,36,UserIdentifyResultMgr.FPUserID,NormalDisplay);
				
				if (SystemLanguage == Chinese){
					DisHZ16x14Str(4,0,PasscodeUserStr,NormalDisplay);
					DisHZ16x14Str(6,0,IDStr,NormalDisplay);
				}
				else{
					DisEN16x8Str(4,0,PasscodeUserStrEn,NormalDisplay);
					DisEN16x8Str(6,0,IDStrEn,NormalDisplay);
				}
				DisDigital16x8Str(6,36,UserIdentifyResultMgr.PasscodeUserID,NormalDisplay); 
			}
			else if (UserIdentifyResultMgr.IdentifyType == CARDandPASSCODE ){
				if (SystemLanguage == Chinese){
					DisHZ16x14Str(0,0,CardUserStr,NormalDisplay);
					DisHZ16x14Str(2,0,IDStr,NormalDisplay);
				}
				else{
					DisEN16x8Str(0,0,CardUserStrEn,NormalDisplay);
					DisEN16x8Str(2,0,IDStrEn,NormalDisplay);
				}
				DisDigital16x8Str(2,36,UserIdentifyResultMgr.CardUserID,NormalDisplay);
				
				if (SystemLanguage == Chinese){
					DisHZ16x14Str(4,0,PasscodeUserStr,NormalDisplay);
					DisHZ16x14Str(6,0,IDStr,NormalDisplay);
				}
				else{
					DisEN16x8Str(4,0,PasscodeUserStrEn,NormalDisplay);
					DisEN16x8Str(6,0,IDStrEn,NormalDisplay);
				}
				DisDigital16x8Str(6,36,UserIdentifyResultMgr.PasscodeUserID,NormalDisplay); 
			}
			else if (UserIdentifyResultMgr.IdentifyType == INITIALSTATUS ){
				if (SystemLanguage == Chinese){
					DisHZ16x14Str(2,0,InitialStatusStr,NormalDisplay);
				}
				else{
					DisEN16x8Str(2,0,InitialStatusStrEn,NormalDisplay);
				}
			}
			else if (UserIdentifyResultMgr.IdentifyType == RemoteUnlock )
			{
				if (SystemLanguage == Chinese){
					DisHZ16x14Str(2,0,RemoteUnlockStr,NormalDisplay);
				}
				else{
					DisEN16x8Str(2,0,RemoteUnlockStrEn,NormalDisplay);
				}
			}
			else //if (UserIdentifyResultMgr.IdentifyType == InsideOpenDoor)
			{
				if (SystemLanguage == Chinese){
					DisHZ16x14Str(2,0,ButtonUnlockStr,NormalDisplay);
				}
				else{
					DisEN16x8Str(2,0,ButtonUnlockStrEn,NormalDisplay);
				}
			}
			
			if (  ( DisplayDoorStatusMgr.Status == DoorOpened )  
				||( DisplayDoorStatusMgr.Status == DoorOpenedWaitClose )
				||( DisplayDoorStatusMgr.Status == DisplayOpenDoorEnd )
				)
			{
				if (SystemLanguage == Chinese){
					DisHZ16x14Str(6,64,DoorUnlockedStr,NormalDisplay);
				}
				else{
					DisEN16x8Str(6,60,DoorUnlockedStrEn,NormalDisplay);
				}
			}
			else
			{
				if (SystemLanguage == Chinese){
					DisHZ16x14Str(6,64,DoorUnlockingStr,NormalDisplay);
				}
				else{
					DisEN16x8Str(6,60,DoorUnlockingStrEn,NormalDisplay);
				}
			}		
		}
		else if (( DisplayDoorStatusMgr.Status == CloseDoorWaitForMotorStatusChangeFromStandby) 
				||( DisplayDoorStatusMgr.Status == DoorClosing )	
				)
		{
			GUI_Flag_RefreshLCD = bTRUE;
			DisImage(0,44,40,40,Icon_Locked,NormalDisplay);
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(6,36,DoorLockingStr,NormalDisplay);
			}
			else{
				DisEN16x8Str(6,32,DoorLockingStrEn,NormalDisplay);
			}
		}
		else if (( DisplayDoorStatusMgr.Status == DoorClosed )
				||( DisplayDoorStatusMgr.Status == DisplayCloseDoorEnd )
				)
		{	
			GUI_Flag_RefreshLCD = bTRUE;
			DisImage(0,44,40,40,Icon_Locked,NormalDisplay);

			if (SystemLanguage == Chinese){
				DisHZ16x14Str(6,36,DoorLockedStr,NormalDisplay);
			}
			else{
				DisEN16x8Str(6,32,DoorLockedStrEn,NormalDisplay);
			}	
		}
	}
	
	if ( UserIdentifyResultMgr.TimeCnt > 0 )
	{
		UserIdentifyResultMgr.TimeCnt--;
	}
	else 
	{
		if (UserIdentifyResultMgr.IdentifyType != InsideOpenDoor)
		{
			StrongUnlockMgr.LastUTCtime = mktime(2000+BCDcodeToHEX(SystemTime.year), BCDcodeToHEX(SystemTime.month),
			BCDcodeToHEX(SystemTime.date), BCDcodeToHEX(SystemTime.hour),
			BCDcodeToHEX(SystemTime.minute), BCDcodeToHEX(SystemTime.second));
		}
				
		SystemPowerMgr.SleepDelayTimerCnt = 0x0000; //go to sleep

		BodyInductionMgr.BodyInductionDelayTimeCnt = Def_BodyInductionDelayTime_AntiTurningBack;
		BodyInductionMgr.BodyInductionInvalidTimes = 0;
		BodyInductionMgr.BodyInductionBeLimited = bFALSE;
		
		UserIdentifyResultMgr.CardIdentifyStatus = S_FAIL;
		UserIdentifyResultMgr.FPIdentifyStatus = S_FAIL;
		UserIdentifyResultMgr.PasscodeIdentifyStatus = S_FAIL;
		UserIdentifyResultMgr.FaceIdentifyStatus = S_FAIL;
		
		GoIntoMainScreen_WithIdentifyInit();	

		GUI_SetFPM_LED(DEF_FpmLedMode_Breath,DEF_FpmLedColor_Blue,DEF_FpmLedColor_Blue,255);

	}

}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowIdentifyFailPage(void)
{
	uint8_t i;

	const uint8_t TitleStr1[]={HZ_zhi,HZ_wen,HZ_ying,HZ_jianshu,HZ_bu,HZ_pi,HZ_pei,HZ_end};
	const uint8_t TitleStr2[]={HZ_qing,HZ_hui,HZ_fu,HZ_chu,HZ_chang,HZ_end};

	DisImage(0,52,24,24,Icon_Incorrect,NormalDisplay);
	
	if ( UserIdentifyResultMgr.IdentifyType == CARD )
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(5,36,IdentifyFailStr,NormalDisplay);
		}
		else{
			DisEN16x8Str(5,12,IdentifyFailStrEn,NormalDisplay);
		}
	}
	else if ( UserIdentifyResultMgr.IdentifyType == FINGERPRINT )
	{
		if (SystemLanguage == Chinese)
		{
			if ( UserIdentifyResultMgr.ErrorType == FPMserialNumberMismatched ){
				DisHZ16x14Str(4,14,TitleStr1,NormalDisplay);
				DisHZ16x14Str(6,20,TitleStr2,NormalDisplay);
			}
			else{
				DisHZ16x14Str(5,36,IdentifyFailStr,NormalDisplay);
			}
		}
		else{
			DisEN16x8Str(5,12,IdentifyFailStrEn,NormalDisplay);
		}
	}
	else if (UserIdentifyResultMgr.IdentifyType == PASSCODE )
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(5,36,IdentifyFailStr,NormalDisplay);
		}
		else{
			DisEN16x8Str(5,12,IdentifyFailStrEn,NormalDisplay);
		}
	}
	
	if ( UserIdentifyResultMgr.TimeCnt > 0 )
	{
		if ( UserIdentifyResultMgr.TimeCnt == Def_MessageBoxTimeDelay )
		{
			GUI_SetFPM_LED(DEF_FpmLedMode_Breath,DEF_FpmLedColor_Red,DEF_FpmLedColor_Red,255);
		}
		
		UserIdentifyResultMgr.TimeCnt--;
	}
	else
	{
		GoIntoMainScreen_WithIdentifyInit();
		GUI_SetFPM_LED(DEF_FpmLedMode_Breath,DEF_FpmLedColor_Blue,DEF_FpmLedColor_Blue,255);
	}
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowErrorMessage(void)
{
	const uint8_t CommunicationErrorStr[]={HZ_tongyong,HZ_xunfei,HZ_shibai,HZ_bai,HZ_end};
	const uint8_t CommunicationErrorStr1En[]={"Communication"};
	const uint8_t CommunicationErrorStr2En[]={"Fail"};

	const uint8_t SystemErrorStr[]={"System Error"};
	const uint8_t UnkownErrorStr[]={"ERROR"};

	if (ErrorMessageMgr.ErrorType == CommunicationError)
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(3,36,CommunicationErrorStr,NormalDisplay);
		}
		else{
			DisEN16x8Str(3,12,CommunicationErrorStr1En,NormalDisplay);
			DisEN16x8Str(5,12,CommunicationErrorStr2En,NormalDisplay);
		}
		
	}
	else if  ( ErrorMessageMgr.ErrorType == SystemError )
	{
		DisEN16x8Str(3,12,SystemErrorStr,NormalDisplay);
	}
	else
	{
		DisEN16x8Str(3,12,UnkownErrorStr,NormalDisplay);
	}

	if ( ErrorMessageMgr.TimeCnt % 32 == 0 )
	{
		SET_ALLKEYLED_ON();
	}
	else if ( ErrorMessageMgr.TimeCnt % 32 == 16 )
	{
		SET_ALLKEYLED_OFF();
	}
	if ( ErrorMessageMgr.TimeCnt % 64 == 0 )
	{
		DEF_Fail_Beep;
	}
	
	if ( ErrorMessageMgr.TimeCnt > 1 )
	{
		ErrorMessageMgr.TimeCnt--;
	}
	else
	{
		GoIntoMainScreen_WithIdentifyInit();
	}

}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowDoorBell(void)
{
	DisImage(33,12,61,40,Icon_DoorBell,NormalDisplay);
	
	if ( DoorBellMgr.TimeCnt > 0 )
	{
		DoorBellMgr.TimeCnt--;
	}
	else
	{
		GoIntoMainScreen_WithIdentifyInit();
	}

}

void GUI_DataInputCreat(uint8_t StartPage,uint8_t StartColumn,uint8_t InputNum,uint16_t DefaultValue)
{
	uint8_t i,temp;
	uint16_t value;


	if( DataInputMgr.Status == InputIdle )
	{
		DataInputMgr.Status = InputStart;
		DataInputMgr.Value = DefaultValue;
		DataInputMgr.InputNum = InputNum;

		if ( DefaultValue == 0x0000 ){
			DataInputMgr.InputPoint = 0;
		}
		else{
			DataInputMgr.InputPoint = InputNum;
		}
	}
		
	else if ( DataInputMgr.Status == InputStart )
	{
		if (DataInputMgr.InputPoint > DataInputMgr.InputNum ){
			return;
		}

		value = DataInputMgr.Value;
		for (i=0;i<DataInputMgr.InputPoint;i++)
		{	
			temp = value%10;
			DisOneDigital16x8(StartPage,StartColumn+(8*(DataInputMgr.InputPoint-i-1)),temp,NormalDisplay);
			value/=10;
		}

		for (i=DataInputMgr.InputPoint;i<(DataInputMgr.InputNum+1);i++)		//"+1" for clear underline
		{
			DisZF16x8(StartPage,StartColumn+(8*i),ZF_kongge,NormalDisplay);
		}
		
		if ( GUI_ToggleFlag_05s == 1 )
		{
			DisZF16x8(StartPage,StartColumn+(8*DataInputMgr.InputPoint),ZF_kongge,NormalDisplay);
		}
		else{
			DisZF16x8(StartPage,StartColumn+(8*DataInputMgr.InputPoint),ZF_underline,NormalDisplay);
		}
	}

}




void GUI_UserIDinputButtonMonitor(keycode_t keycode)
{
	if ( (keycode < KEY_NINE)||(keycode == KEY_NINE) )
	{
		DEF_ButtonPress_Voice;
		if ( DataInputMgr.InputPoint < DataInputMgr.InputNum )
		{
			DataInputMgr.Value = DataInputMgr.Value*10+gui_keycode;
			DataInputMgr.InputPoint++;
		}
	}
	else if ( keycode == KEY_ASTERISK )
	{
		DEF_ButtonPress_Voice;
		if ( DataInputMgr.InputPoint > 0 )
		{
			//DataInputMgr.InputPoint--;
			//DataInputMgr.Value/=10;
			DataInputMgr.InputPoint=0;
			DataInputMgr.Value=0;
		}
		else
		{
			DataInputMgr.Status = InputExit;
		}
	}
	else if ( keycode == KEY_POUNDSIGN )
	{
		DEF_ButtonPress_Voice;
		if ( DataInputMgr.InputPoint == 0 )
		{
			DataInputMgr.Status = InputExit;
		}
		else
		{
			DataInputMgr.Status = InputEnd;
		}
	}
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void GUI_PasscodeInputCreat(uint8_t StartPage,uint8_t StartColumn)
{
	uint8_t i;

	if (PasscodeInputMgr.Point > 16){
		return;
	}
	
	Clear_Screen_Page(StartPage);
	Clear_Screen_Page(StartPage+1);
	for (i=0;i<PasscodeInputMgr.Point;i++)
	{
		DisZF16x8(StartPage,(8*i)+(64-(PasscodeInputMgr.Point*4)),ZF_xinghao,NormalDisplay);
	}

}


void GUI_PasscodeInputButtonMonitor(keycode_t keycode)
{
	uint8_t i;
	
	if ( (keycode < KEY_NINE)||(keycode == KEY_NINE) )
	{
		if ( CurrentScreen != SCREEN_PickLockAlarm ){
			DEF_ButtonPress_Voice;
		}
		if ( PasscodeInputMgr.Point <  PasscodeInputMgr.PasscodeLen )
		{
			PasscodeInputMgr.InputBuff[PasscodeInputMgr.Point] = gui_keycode;
			PasscodeInputMgr.Point++;
		}
	}
	else if ( keycode == KEY_ASTERISK )
	{
		if ( CurrentScreen != SCREEN_PickLockAlarm ){
			DEF_ButtonPress_Voice;
		}		
		if ( PasscodeInputMgr.Point > 0 )
		{
			for (i=0;i<16;i++)
			{
				PasscodeInputMgr.InputBuff[i]=0xff;
			}
			PasscodeInputMgr.Point = 0;
		}
		else
		{
			PasscodeInputMgr.Status = PasscodeInputExit;
		}
	}
	else if ( keycode == KEY_POUNDSIGN )
	{
		if ( CurrentScreen != SCREEN_PickLockAlarm ){
			DEF_ButtonPress_Voice;
		}
		if ( PasscodeInputMgr.Point == 0 )
		{
			PasscodeInputMgr.Status = PasscodeInputExit;
		}
		else
		{
			PasscodeInputMgr.Status = PasscodeInputEnd;
		}
	}
}

void FpUserIdentify(void)
{

	if (FpIdentifyMgr.Status == FPMcmdStart )
	{
		FpIdentifyMgr.Status = SendGetImageCmd;
	}
	else if (FpIdentifyMgr.Status == SendGetImageCmd)
	{
		FpIdentifyMgr.Status = WaitForGetImageCmdACK;
		FpmAckMgr.Status = WaitACK;
		FpmAckMgr.TimeOutCnt = DEF_FpmAckTimeoutTime;
		FPM_SendGetImageCmd();
		FpIdentifyMgr.TimeCnt = Def_FPMcmdTimeOutDelay;
		DEBUG_MARK;
	}
	else if ( FpIdentifyMgr.Status == WaitForGetImageCmdACK)
	{
		if (FpmAckMgr.Status == GotACK)
		{
			if ( FpmAckMgr.ErrorCode == Error_NONE)
			{
				#ifdef Function_FPMserialNumberCheck
				if  ( (CheckMemoryMgr.FpUserNum+CheckMemoryMgr.FpMasterNum+CheckMemoryMgr.StressFpUserNum) > 0x00 )
				{
					FpIdentifyMgr.Status = SendGetSerialNumberCmd;
				}
				else
				{
					FpIdentifyMgr.Status = SendGenCharCmd;
				}
				#else
				FpIdentifyMgr.Status = SendGenCharCmd;
				#endif
				GUI_RefreshSleepTime();
			}
			else
			{
				if (FpmAckMgr.ErrorCode == Error_NoFinger)
				{
					FpIdentifyMgr.Status = FPMcmdStart;
				}
				else
				{
					FpIdentifyMgr.Status = FPMcmdStart;
					DEBUG_MARK;
				}
			}
		}
		else 
		{
			if (--FpIdentifyMgr.TimeCnt < 1)
				{
					FpIdentifyMgr.Status = FPMcmdStart;
                                        DEBUG_MARK;
				}
		}
	}
	else if ( FpIdentifyMgr.Status == SendGetSerialNumberCmd )
	{
		FpIdentifyMgr.Status = WaitForGetSerialNumberCmdACK;
		FpmAckMgr.Status = WaitACK;
		FpmAckMgr.TimeOutCnt = DEF_FpmAckTimeoutTime;
		FPM_SendGetChipSerialNumberCmd();
		FpIdentifyMgr.TimeCnt = Def_FPMcmdTimeOutDelay;
		DEBUG_MARK;
	}
	else if ( FpIdentifyMgr.Status == WaitForGetSerialNumberCmdACK)
	{
		if (FpmAckMgr.Status == GotACK)
		{
			if ( FpmAckMgr.ErrorCode == Error_NONE)
			{
				if ( CheckFPMserialNumberIsMatch(&FpmAckMgr.Buff[10]) == bTRUE )
				{
					FpIdentifyMgr.Status = SendGenCharCmd;
					//GUI_Flag_RefreshLCD = bTRUE;
					FpIdentifyMgr.TimeCnt=Def_IdendtifySuccessScreenTimeDelay;	
				}
				else
				{
					FpIdentifyMgr.Status = fail;
					FpIdentifyMgr.ErrorType = Error_SerialNumberMismatched;
					FpIdentifyMgr.TimeCnt=Def_IdendtifyFailScreenTimeDelay;
					DEBUG_MARK;
				}
			}
			else
			{
				FpIdentifyMgr.Status = fail;
				FpIdentifyMgr.ErrorType = Error_TimeOut;
				FpIdentifyMgr.TimeCnt=Def_IdendtifyFailScreenTimeDelay;
				DEBUG_MARK;
			}
		}
		else 
		{
			if (--FpIdentifyMgr.TimeCnt < 1)
				{
					FpIdentifyMgr.Status = fail;
					FpIdentifyMgr.ErrorType = Error_TimeOut;
					FpIdentifyMgr.TimeCnt=Def_IdendtifyFailScreenTimeDelay;
					DEBUG_MARK;
				}
		}
	}
	else if ( FpIdentifyMgr.Status == SendGenCharCmd )
	{
		
		FpIdentifyMgr.Status = WaitForGenCharCmdACK;
		FpmAckMgr.Status = WaitACK;
		FpmAckMgr.TimeOutCnt = DEF_FpmAckTimeoutTime;
		FPM_SendGenCharCmd(0x01);		//BuffID is 0x01
		FpIdentifyMgr.TimeCnt = Def_FPMcmdTimeOutDelay;
		DEBUG_MARK;
	}
	else if ( FpIdentifyMgr.Status == WaitForGenCharCmdACK)
	{
		if (FpmAckMgr.Status == GotACK)
		{
			if ( FpmAckMgr.ErrorCode == Error_NONE)
			{
				FpIdentifyMgr.Status = SendSearchCmd;
				//GUI_Flag_RefreshLCD = bTRUE;
			}
			else
			{
				FpIdentifyMgr.Status = fail;
				FpIdentifyMgr.ErrorType = Error_GenChar;
				FpIdentifyMgr.TimeCnt=Def_IdendtifyFailScreenTimeDelay;
				DEBUG_MARK;
			}
			DEBUG_MARK;
		}
		else 
		{
			if (--FpIdentifyMgr.TimeCnt < 1)
				{
					FpIdentifyMgr.Status = fail;
					FpIdentifyMgr.ErrorType = Error_TimeOut;
					FpIdentifyMgr.TimeCnt=Def_IdendtifyFailScreenTimeDelay;
					DEBUG_MARK;
				}
		}
	}
	else if ( FpIdentifyMgr.Status == SendSearchCmd )
	{
		FpIdentifyMgr.Status = WaitForSearchCmdACK;
		FpmAckMgr.Status = WaitACK;
		FpmAckMgr.TimeOutCnt = DEF_FpmAckTimeoutTime;
		FPM_SendSearchCmd(0x01,0x0000,(DEF_MAX_FPMASTER+DEF_MAX_FPUSER+DEF_MAX_STRESSFPUSER));		//BuffID is 0x01,from Page 1~100
		FpIdentifyMgr.TimeCnt = Def_FPMcmdTimeOutDelay;
		DEBUG_MARK;
	}
	else if ( FpIdentifyMgr.Status == WaitForSearchCmdACK)
	{
		if (FpmAckMgr.Status == GotACK)
		{
			if ( FpmAckMgr.ErrorCode == Error_NONE)
			{
				FpIdentifyMgr.Status = success;
				FpIdentifyMgr.UserID = FpmAckMgr.Para1;
				//GUI_Flag_RefreshLCD = bTRUE;
				FpIdentifyMgr.TimeCnt=Def_IdendtifySuccessScreenTimeDelay;	
			}
			else if (FpmAckMgr.ErrorCode == Error_FingerNotRelease )
			{
				FpIdentifyMgr.Status = FPMcmdStart;
			}
			else{
				FpIdentifyMgr.Status = fail;
				FpIdentifyMgr.ErrorType = Error_NotMatch;
				FpIdentifyMgr.TimeCnt=Def_IdendtifyFailScreenTimeDelay;
				//FpIdentifyMgr.ErrorType = FpmAckMgr.ErrorCode;
			}
			DEBUG_MARK;
		}
		else 
		{
			if (--FpIdentifyMgr.TimeCnt < 1)
				{
					FpIdentifyMgr.Status = fail;
					FpIdentifyMgr.ErrorType = Error_TimeOut;
					FpIdentifyMgr.TimeCnt=Def_IdendtifyFailScreenTimeDelay;
				}
		}
	}
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void PasscodeUserIdentify(void)
{
	//uint8_t PasscodeBUFF[16];
	//uint8_t i;

	if ( PasscodeUserIdentifyMgr.Status != PasscodeIdentifyIdle )
	{
		if (PasscodeUserIdentifyMgr.Status == PasscodeIdentifyStart )
		{
			/*
			PasscodeInputMgr.Point = 0x00;
			PasscodeInputMgr.PasscodeLen = 16;
			PasscodeInputMgr.Status = PasscodeInputStart;
			for (i=0;i<PasscodeInputMgr.PasscodeLen;i++)
			{
				PasscodeInputMgr.InputBuff[i]=0xff;
			}
			*/
			GUI_Flag_RefreshLCD = bTRUE;	
			PasscodeUserIdentifyMgr.TimeCnt = 240;	//
			PasscodeUserIdentifyMgr.Status = PasscodeIdentifyPasscodeInput;
		}
		else if (PasscodeUserIdentifyMgr.Status == PasscodeIdentifyPasscodeInput)
		{
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(3,28,InputPasscodeStr,NormalDisplay);
			}
			else{
				DisEN16x8Str(3,16,InputPasscodeStrEn,NormalDisplay);
			}
			GUI_PasscodeInputCreat(6,0);
			if (PasscodeInputMgr.Status == PasscodeInputEnd)
			{
				#if defined (ProjectIs_AutoBarLock_S58Z02) || defined (ProjectIs_AutoBarLock_S49Z06)\
					|| defined (ProjectIs_AutoBarLock_S67Z01) || defined (ProjectIs_AutoBarLock_S31Z01)\
					|| defined (ProjectIs_AutoBarLock_S67Z02) || defined (ProjectIs_AutoBarLock_S20Z04)\
					|| defined ( ProjectIs_AutoBarLock_S11Z05)|| defined (ProjectIs_AutoBarLock_S58Z07)\
					|| defined (ProjectIs_AutoBarLock_S68Z04) || defined (ProjectIs_AutoBarLock_S68Z10)
				if ((PasscodeInputMgr.InputBuff[0]==0x01)&&(PasscodeInputMgr.Point == 1))
				{
					PasscodeUserIdentifyMgr.Status = PasscodeIdentifyIntoMainMenuSuccess;
				}
				#else
				if (0)
				{
					
				}
				#endif
				else
				{
					if ( IfSystemIsInFactoryDefaultStatus()==bTRUE )
					{
						if( ( (PasscodeInputMgr.InputBuff[0]==0x01)
							&&(PasscodeInputMgr.InputBuff[1]==0x01)
							&&(PasscodeInputMgr.InputBuff[2]==0x01)
							&&(PasscodeInputMgr.InputBuff[3]==0x01)
							&&(PasscodeInputMgr.InputBuff[4]==0x01)
							&&(PasscodeInputMgr.InputBuff[5]==0x01)
							&&(PasscodeInputMgr.InputBuff[6]==0x01)
							&&(PasscodeInputMgr.InputBuff[8]==0x01)
							&&(PasscodeInputMgr.InputBuff[9]==0x01)
						  )
						  ||((PasscodeInputMgr.InputBuff[0]==0x03)&&(PasscodeInputMgr.Point == 1))
						  )
						{
							PasscodeUserIdentifyMgr.Status = PasscodeIdentifyAgingTestSuccess;
						}
						else if ((PasscodeInputMgr.InputBuff[0]==0x08)
								&&(PasscodeInputMgr.InputBuff[1]==0x08)
								&&(PasscodeInputMgr.InputBuff[2]==0x08)
								&&(PasscodeInputMgr.Point == 3)
								)
						{
							PasscodeUserIdentifyMgr.Status = PasscodeIdentifyEngineeringModeSuccess;
						}
						else if ((PasscodeInputMgr.InputBuff[0]==0x07)
								&&(PasscodeInputMgr.InputBuff[1]==0x06)
								&&(PasscodeInputMgr.InputBuff[2]==0x04)
								&&(PasscodeInputMgr.InputBuff[3]==0x03)
								&&(PasscodeInputMgr.Point == 4)
								)
						{
							PasscodeUserIdentifyMgr.Status = PasscodeIdentifyWifiMFTSuccess;
						}
						else if ((PasscodeInputMgr.InputBuff[0]==0x01)
								&&(PasscodeInputMgr.InputBuff[1]==0x04)
								&&(PasscodeInputMgr.InputBuff[2]==0x05)
								&&(PasscodeInputMgr.InputBuff[3]==0x08)
								&&(PasscodeInputMgr.Point == 4)
								)
						{
							PasscodeUserIdentifyMgr.Status = PasscodeIdentifySelfTestSuccess;
						}
						else if ((PasscodeInputMgr.InputBuff[0]==0x01)
						&&(PasscodeInputMgr.InputBuff[1]==0x03)
						&&(PasscodeInputMgr.InputBuff[2]==0x05)
						&&(PasscodeInputMgr.InputBuff[3]==0x07)
						&&(PasscodeInputMgr.Point == 4)
						)
						{
							PasscodeUserIdentifyMgr.Status = PasscodeIdentifyFastVersionCheckSuccess;
						}
						else if ((PasscodeInputMgr.InputBuff[0]==0x02)
						&&(PasscodeInputMgr.InputBuff[1]==0x05)
						&&(PasscodeInputMgr.InputBuff[2]==0x06)
						&&(PasscodeInputMgr.InputBuff[3]==0x08)
						&&(PasscodeInputMgr.Point == 4)
						)
						{
							PasscodeUserIdentifyMgr.Status = PasscodeIdentifyLanguageSetToEnglishSuccess;
						}
						else if ((PasscodeInputMgr.InputBuff[0]==0x02)
						&&(PasscodeInputMgr.InputBuff[1]==0x05)
						&&(PasscodeInputMgr.InputBuff[2]==0x06)
						&&(PasscodeInputMgr.InputBuff[3]==0x09)
						&&(PasscodeInputMgr.Point == 4)
						)
						{
							PasscodeUserIdentifyMgr.Status = PasscodeIdentifyLanguageSetToChineseSuccess;
						}
						else
						{
							PasscodeUserIdentifyMgr.Status = PasscodeIdentifyFail;
						}
					}
					else
					{
						PasscodeUserIdentifyMgr.UserID = PasscodeIdendify( PasscodeInputMgr.InputBuff);
						if ( PasscodeUserIdentifyMgr.UserID != 0x00 )//Identify success
						{
							PasscodeUserIdentifyMgr.Status = PasscodeIdentifySuccess; 
							PasscodeUserIdentifyMgr.TimeCnt =Def_IdendtifySuccessScreenTimeDelay;
							UserIdentifyResultMgr.PasscodeType = LocalPasscode;
							GUI_Flag_RefreshLCD = bTRUE;
							#if defined (Function_YouzhiyunjiaWifi) || defined (Function_TuyaWifi) 
								Wifi_PowerOffIfPowerOnForOnlinePasswordVerify();
							#endif
						}
						else
						{
							if ( PasscodeInputMgr.Point == 12 )
							{
								PasscodeUserIdentifyMgr.UserID = AppPasscodeIdentify(PasscodeInputMgr.InputBuff);
								if ( PasscodeUserIdentifyMgr.UserID != 0x00 )//App unlock success
								{
									PasscodeUserIdentifyMgr.Status = PasscodeIdentifySuccess;
									PasscodeUserIdentifyMgr.TimeCnt =Def_IdendtifySuccessScreenTimeDelay;
									UserIdentifyResultMgr.PasscodeType = AppPasscode;
									GUI_Flag_RefreshLCD = bTRUE;
								}
								else
								{
									PasscodeUserIdentifyMgr.Status = PasscodeIdentifyFail;
									PasscodeUserIdentifyMgr.TimeCnt =Def_IdendtifyFailScreenTimeDelay;
									GUI_Flag_RefreshLCD = bTRUE;
								}
								#if defined (Function_YouzhiyunjiaWifi) || defined (Function_TuyaWifi) 
								Wifi_PowerOffIfPowerOnForOnlinePasswordVerify();
								#endif
							}
							else if  (( PasscodeInputMgr.Point == 7 )&&(PasscodeInputMgr.InputBuff[0] == 1 ))
							{
								if ( WifiMgr.WifiOnlinePasswordVerifyMgr.Status == OnlinePasswordVerifyInit )
								{
									WifiMgr.WifiOnlinePasswordVerifyMgr.Status = OnlinePasswordVerifyStart;
									#if defined (Function_YouzhiyunjiaWifi) || defined (Function_TuyaWifi) 
									Wifi_PostOnlinePasswordVerify(&PasscodeInputMgr.InputBuff[1]);
									#else
									ComPort_SetPost_OnlinePasswordVerify(&PasscodeInputMgr.InputBuff[1]);
									#endif
								}
								else if ( WifiMgr.WifiOnlinePasswordVerifyMgr.Status == OnlinePasswordVerifySuccess )
								{
									PasscodeUserIdentifyMgr.Status = PasscodeIdentifySuccess;
									PasscodeUserIdentifyMgr.TimeCnt =Def_IdendtifySuccessScreenTimeDelay;
									UserIdentifyResultMgr.PasscodeType = OnlinePasscode;
									PasscodeUserIdentifyMgr.UserID = 0x00;	//(WifiMgr.WifiOnlinePasswordVerifyMgr.UserID-1023)%256;	//online passcode user ID from 1024 to 2048
									WifiMgr.WifiOnlinePasswordVerifyMgr.Status = OnlinePasswordVerifyInit;
									GUI_Flag_RefreshLCD = bTRUE;
									#if defined (Function_YouzhiyunjiaWifi) || defined (Function_TuyaWifi) 
									Wifi_PowerOffIfPowerOnForOnlinePasswordVerify();
									#endif

								}
								else if ( WifiMgr.WifiOnlinePasswordVerifyMgr.Status == OnlinePasswordVerifyFail )
								{
									PasscodeUserIdentifyMgr.Status = PasscodeIdentifyFail;
									PasscodeUserIdentifyMgr.TimeCnt =Def_IdendtifyFailScreenTimeDelay;
									WifiMgr.WifiOnlinePasswordVerifyMgr.Status = OnlinePasswordVerifyInit;
									GUI_Flag_RefreshLCD = bTRUE; 
									#if defined (Function_YouzhiyunjiaWifi) || defined (Function_TuyaWifi) 
									Wifi_PowerOffIfPowerOnForOnlinePasswordVerify();
									#endif
								}
							}
							else
							{			
								PasscodeUserIdentifyMgr.Status = PasscodeIdentifyFail;
								PasscodeUserIdentifyMgr.TimeCnt =Def_IdendtifyFailScreenTimeDelay;
								GUI_Flag_RefreshLCD = bTRUE;
								#if defined (Function_YouzhiyunjiaWifi) || defined (Function_TuyaWifi) 
								Wifi_PowerOffIfPowerOnForOnlinePasswordVerify();
								#endif
								
							}
						}
					}
				}
			}
			else if (PasscodeInputMgr.Status == PasscodeInputExit)
			{
				PasscodeUserIdentifyMgr.Status = PasscodeIdentifyIdle;
				PasscodeUserRegisterMgr.TimeCnt = 0;	
				GUI_Flag_RefreshLCD = bTRUE;
			}
		}
	}
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void CardUserIdentify(void)
{
	status_t GetCardID;
	uint8_t UserID;

	if (CardIdentifyMgr.Status == ReadingCardID)
	{
		GetCardID = MFC_Auto_Reader(CardIdentifyMgr.CID);
		if ( GetCardID == S_SUCCESS )
		{
			UserID = CompareCardIDwithMemory(CardIdentifyMgr.CID); 
			if ( UserID == 0x00 )		//Card ID is not registered
			{
				CardIdentifyMgr.Status = Fail;
				CardIdentifyMgr.TimeCnt = Def_IdendtifyFailScreenTimeDelay;
			}
			else
			{
				CardIdentifyMgr.UserID = UserID;
				CardIdentifyMgr.Status = Success;	
				CardIdentifyMgr.TimeCnt = Def_IdendtifySuccessScreenTimeDelay;
			}
		}
	}
}


/*******************************************************/
/*******************************************************/
/*******************************************************/
void DisplayMainPage(void)
{
	const uint8_t YearMonthDay[]={ZF_2,ZF_0,ZF_1,ZF_8,ZF_hengan,ZF_0,ZF_1,ZF_hengan,ZF_0,ZF_1,HZ_end};
	const uint8_t Frame[]={ZF_1,ZF_8,ZF_hengan,ZF_0,ZF_1,ZF_hengan,ZF_0,ZF_1,HZ_end};
	const uint8_t Str1[]={HZ_weihe,HZ_le,HZ_nin,HZ_de,HZ_an,HZ_quan,HZ_end};
	const uint8_t Str1En[]={"Sys without"};
	const uint8_t Str2[]={HZ_qing,HZ_tian,HZ_jia,HZ_guan,HZ_li,HZ_yuan,HZ_end};
	const uint8_t Str2En[]={"admin."};
	const uint8_t Str3[]={HZ_qing,HZ_shu,HZ_ru,HZ_di,HZ_er,HZ_shen,HZ_fen,HZ_end};
	const uint8_t Str3En[]={"Input 2nd ID"};
	const uint8_t Str4[]={HZ_anya,ZF_xiaoyuhao,ZF_jinghao,ZF_dayuhao,HZ_jinru,HZ_ru,HZ_guan,HZ_li,HZ_mo,HZ_si,HZ_end};
	const uint8_t Str4En[]={"Press # to menu"};

	const uint8_t PleaseAddAdminStr[]={HZ_qing,HZ_tian,HZ_jia,HZ_guan,HZ_li,HZ_yuan,HZ_end};
	const uint8_t PleaseAddAdminStrEn[]={"Pls add admin."};

//	if (AwakeDisplayMgr.TimeCnt > 0 )
//	{
//		AwakeDisplayMgr.TimeCnt--;
//	}

	if ((AwakeDisplayMgr.DisplayType == DisplayHide)
		)
	{
		SET_ALLKEYLED_OFF();
		Clear_Screen();
		
		#ifdef Function_DisplayUsed_HFG12864
		SET_OLEDPOWER_OFF;
		#endif
		
		return;
	}

	SET_ALLKEYLED_ON();
	GUI_SetFPM_LED(DEF_FpmLedMode_On,DEF_FpmLedColor_Blue,DEF_FpmLedColor_Blue,255);

	DisImage(0,92,25,16,IconBetteryLevel[BatteryMgr.BatteryLevel],NormalDisplay);

	if ( ( LockBrand.BrandType == BigBrand )
		||( LockBrand.BrandType == DefaultBigBrand )
		)
	{
	
		DisBcdDigital16x8(0,8,SystemTime.hour,NormalDisplay);
		if (GUI_ToggleFlag_05s == 0 )
		{
			DisZF16x8(0,24,ZF_kongge,NormalDisplay);
		}
		else
		{
			DisZF16x8(0,24,ZF_maohao,NormalDisplay);
		}
		DisBcdDigital16x8(0,32,SystemTime.minute,NormalDisplay);

		if ( WifiMgr.WifiConfigStatus.Status == ConnectedToRooter )
		{
			DisImage(0,66,16,16,Icon_WifiConnected,NormalDisplay);
		}
		else
		{
			Clear_PartyScreen(0,66,16,16);
			//DisImage(0,66,16,16,Icon_WifiNoSignal,NormalDisplay);
		}
		
		if (	( PasscodeUserIdentifyMgr.Status == PasscodeIdentifyIdle )
			)
		{
			if	( (UserIdentifyResultMgr.UnlockingMode == DoubleMode )&&
					((UserIdentifyResultMgr.FPIdentifyStatus == S_SUCCESS )
					||(UserIdentifyResultMgr.CardIdentifyStatus == S_SUCCESS )
					||(UserIdentifyResultMgr.PasscodeIdentifyStatus == S_SUCCESS ))
				)
			{
				if (SystemLanguage == Chinese){
					DisHZ16x14Str(3,16,Str3,NormalDisplay); //please input sencond identity
				}
				else{
					DisEN16x8Str(3,8,Str3En,NormalDisplay); //please input sencond identity
				}
				
				GUI_Flag_RefreshLCD = bTRUE;
			}
				
			#if defined (ProjectIs_AutoBarLock_S58Z02)    || defined (ProjectIs_AutoBarLock_S49Z06) \
				|| defined (ProjectIs_AutoBarLock_S67Z01) || defined (ProjectIs_AutoBarLock_S31Z01)\
				|| defined (ProjectIs_AutoBarLock_S67Z02) || defined ( ProjectIs_AutoBarLock_S11Z05)\
				|| defined ( ProjectIs_AutoBarLock_S20Z04)|| defined ( ProjectIs_AutoBarLock_S58Z07)\
				|| defined (ProjectIs_AutoBarLock_S68Z04) || defined (ProjectIs_AutoBarLock_S68Z10)
			#else
			else if ( g_ASTERISK_PressedOnMainScreen == bTRUE )
			{
				GUI_Flag_RefreshLCD = bTRUE;
				if (SystemLanguage == Chinese){
					DisHZ16x14Str(6,4,Str4,NormalDisplay);
				}
				else{
					DisEN16x8Str(6,4,Str4En,NormalDisplay);
				}						
			}
			#endif

			else if ( IfSystemIsInFactoryDefaultStatus()==bTRUE )
			{
				if (SystemLanguage == Chinese)
				{
					DisHZ16x14Str(6,20,PleaseAddAdminStr,NormalDisplay);
				}
				else
				{			
					DisEN16x8Str(6,8,PleaseAddAdminStrEn,NormalDisplay);
				}	
				GUI_Flag_RefreshLCD = bTRUE;
			}
			else
			{
				GUI_Flag_RefreshLCD = bTRUE;

				DisHZ16x14Str(6,24,YearMonthDay,NormalDisplay);
				DisBcdDigital16x8(6,40,SystemTime.year,NormalDisplay);
				DisBcdDigital16x8(6,64,SystemTime.month,NormalDisplay);
				DisBcdDigital16x8(6,88,SystemTime.date,NormalDisplay);
			}
			//DisHZ16x14Str(6,16,FPDLLog,NormalDisplay);
			ShowLockBrand();
		}
	}
	else
	{
		DisHZ16x14Str(0,8,YearMonthDay,NormalDisplay);
		DisBcdDigital16x8(0,24,SystemTime.year,NormalDisplay);
		DisBcdDigital16x8(0,48,SystemTime.month,NormalDisplay);
		DisBcdDigital16x8(0,72,SystemTime.date,NormalDisplay);
		
		if (	( PasscodeUserIdentifyMgr.Status == PasscodeIdentifyIdle )
			)
		{
			if 	( (UserIdentifyResultMgr.UnlockingMode == DoubleMode )&&
					((UserIdentifyResultMgr.FPIdentifyStatus == S_SUCCESS )
					||(UserIdentifyResultMgr.CardIdentifyStatus == S_SUCCESS )
					||(UserIdentifyResultMgr.PasscodeIdentifyStatus == S_SUCCESS ))
				)
			{
				if (SystemLanguage == Chinese){
					DisHZ16x14Str(3,16,Str3,NormalDisplay);	//please input sencond identity
				}
				else{
					DisEN16x8Str(3,8,Str3En,NormalDisplay);	//please input sencond identity
				}
				GUI_Flag_RefreshLCD = bTRUE;
			}
				
				
			#if defined (ProjectIs_AutoBarLock_S58Z02) || defined (ProjectIs_AutoBarLock_S49Z06) \
				|| defined (ProjectIs_AutoBarLock_S67Z01)|| defined (ProjectIs_AutoBarLock_S31Z01)\
				|| defined (ProjectIs_AutoBarLock_S67Z02)|| defined ( ProjectIs_AutoBarLock_S11Z05)\
				|| defined ( ProjectIs_AutoBarLock_S20Z04)|| defined ( ProjectIs_AutoBarLock_S58Z07)\
				|| defined (ProjectIs_AutoBarLock_S68Z04) || defined (ProjectIs_AutoBarLock_S68Z10)
			#else
			else if ( g_ASTERISK_PressedOnMainScreen == bTRUE )
			{
				GUI_Flag_RefreshLCD = bTRUE;
				if (SystemLanguage == Chinese){
					DisHZ16x14Str(3,4,Str4,NormalDisplay);
				}
				else{
					DisEN16x8Str(3,4,Str4En,NormalDisplay);
				}						
			}
			#endif
			
			else if ( IfSystemIsInFactoryDefaultStatus()==bTRUE )
			{
				DisImage(3,2,27,24,Icon_Warning,NormalDisplay);
				if (SystemLanguage == Chinese){
					DisHZ16x14Str(2,40,Str1,NormalDisplay);
					DisHZ16x14Str(4,40,Str2,NormalDisplay);
				}
				else{			
					DisEN16x8Str(2,38,Str1En,NormalDisplay);
					DisEN16x8Str(4,38,Str2En,NormalDisplay);
				}
				
				GUI_Flag_RefreshLCD = bTRUE;
			}
			else
			{
				GUI_Flag_RefreshLCD = bTRUE;

				DisBcdDigital32x20(2,14,SystemTime.hour,NormalDisplay);
				DisBcdDigital32x20(2,74,SystemTime.minute,NormalDisplay);
				DisImage(2,54,20,32,ZF32x20[10],NormalDisplay);	
			}
			
			//DisHZ16x14Str(6,16,FPDLLog,NormalDisplay);
			ShowLockBrand();
		}
	}

}
/*******************************************************/
/*******************************************************/
/*******************************************************/
void GUI_UpadteMain(void)
{
	uint8_t i;

	if ( (ComportMgr.DoorStatus == Closing )
		||(ComportMgr.DoorStatus == Close)
		)
	{
		UserIdentifyResultMgr.IdentifyType = InsideCloseDoor;	
		Enable_KEYLED_WATERLIGHT();	
		CurrentScreen = SCREEN_IdentifySuccess;
		DisplayDoorStatusMgr.Status = DoorCloseInit;
		UserIdentifyResultMgr.TimeCnt = Def_IdentifySuccessScreenTimeLimited;
	}
	else if (( ComportMgr.DoorStatus == Openning )
			||( ComportMgr.DoorStatus == Open )
			||( ComportMgr.DoorStatus == OpenedWaitClose )
			)
	{
		UserIdentifyResultMgr.IdentifyType = InsideOpenDoor;	
		Enable_KEYLED_WATERLIGHT();	
		CurrentScreen = SCREEN_IdentifySuccess;
		DisplayDoorStatusMgr.Status = DoorOpenInit;
		UserIdentifyResultMgr.TimeCnt = Def_IdentifySuccessScreenTimeLimited;
	}

	if ( ComportMgr.TimeOutTimes > 5 )
	{
		ErrorMessageMgr.ErrorType = CommunicationError;
		ErrorMessageMgr.TimeCnt = Def_GuiTimeDelayCnt3s;
		CurrentScreen = SCREEN_ErrorMessage;
	}

	if ( SafetyMonitorMgr.SystemLocked == bTRUE )
	{
		CurrentScreen = SCREEN_SystemLocked;
		GUI_SetFPM_LED(DEF_FpmLedMode_Off,DEF_FpmLedColor_All,DEF_FpmLedColor_All,255);
		SET_LED_RGB_OFF();
		SET_LED_RB_OFF();
	}
	
	if ( CardIdentifyMgr.CardDetectIntervalTimeCnt == 0  )	//FPM Cmd is sent out
	{
		CardIdentifyMgr.CardDetectIntervalTimeCnt = Def_CardDetectIntervalTime;
		
		CardUserIdentify();
	}
	
	if ( CardIdentifyMgr.Status == Success)
	{
		UserIdentifyResultMgr.CardIdentifyStatus = S_SUCCESS;
		UserIdentifyResultMgr.CardUserID = CardIdentifyMgr.UserID;
		GUI_RefreshSleepTime();
		SafetyMonitorMgr.FpIdentifyFailedTimes = 0x00;
        SafetyMonitorMgr.CardIdentifyFailedTimes = 0x00;
        SafetyMonitorMgr.PasscodeIdentifyFailedTimes = 0x00;
		SafetyMonitorMgr.FaceIdentifyFailedTimes = 0x00;

		if (UserIdentifyResultMgr.UnlockingMode == SingalMode )
		{
			UserIdentifyResultMgr.IdentifyType = CARD;
			Enable_KEYLED_WATERLIGHT();	
			CurrentScreen = SCREEN_IdentifySuccess;
			DisplayDoorStatusMgr.Status = DoorOpenInit;
			UserIdentifyResultMgr.TimeCnt = Def_IdentifySuccessScreenTimeLimited;	
			//SafetyMonitorMgr.CardIdentifyFailedTimes = 0x00;
		}
		else if (UserIdentifyResultMgr.UnlockingMode == DoubleMode )
		{
			if (( UserIdentifyResultMgr.PasscodeIdentifyStatus == S_SUCCESS ))
			{	
				UserIdentifyResultMgr.IdentifyType = CARDandPASSCODE;	
				Enable_KEYLED_WATERLIGHT();	
				CurrentScreen = SCREEN_IdentifySuccess;
				DisplayDoorStatusMgr.Status = DoorOpenInit;
				UserIdentifyResultMgr.TimeCnt = Def_IdentifySuccessScreenTimeLimited;	
				//SafetyMonitorMgr.CardIdentifyFailedTimes = 0x00;
			}
			else if (( UserIdentifyResultMgr.FPIdentifyStatus == S_SUCCESS ))
			{	
				UserIdentifyResultMgr.IdentifyType = FINGERPRINTandCARD;	
				Enable_KEYLED_WATERLIGHT();	
				CurrentScreen = SCREEN_IdentifySuccess;
				DisplayDoorStatusMgr.Status = DoorOpenInit;
				UserIdentifyResultMgr.TimeCnt = Def_IdentifySuccessScreenTimeLimited;	
				//SafetyMonitorMgr.CardIdentifyFailedTimes = 0x00;
			}
			else{	
				PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_PleaseInputSecondIdentity);
				CardIdentifyMgr.Status = ReadingCardID;
			}
		}
	}
	else if ( CardIdentifyMgr.Status == Fail)
	{
		if ( IfSystemIsInFactoryDefaultStatus()==bTRUE )
		{
			if ( (CardIdentifyMgr.CID[0] == 0x16)
				&&(CardIdentifyMgr.CID[1] == 0xD4)
				&&(CardIdentifyMgr.CID[2] == 0xDB)
				&&(CardIdentifyMgr.CID[3] == 0x3C)
				)
			{
				DEBUG_MARK;
				if ( LockBrand.BrandChangeTimeDelay == 0 )
				{
					WriteLockBrand();
					if ( LockBrand.GotBrandData == bTRUE )
					{
						DEBUG_MARK;
						PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_OperationSuccess);
						LockBrand.BrandChangeTimeDelay = Def_GuiTimeDelayCnt3s;
						GUI_RefreshSleepTime();
					}
				}
				CardIdentifyMgr.Status = ReadingCardID;
			}
				
			#ifdef Function_FaceRecoginitionSwitchedByNFC
			else if ( (CardIdentifyMgr.CID[0] == 0x20)
				&&(CardIdentifyMgr.CID[1] == 0xA8)
				&&(CardIdentifyMgr.CID[2] == 0x35)
				&&(CardIdentifyMgr.CID[3] == 0x7B)
				)
			{
				if ( FrmMgr.FrmFunctionSwitchTimeDelay == 0 )
				{
					if ( MFC_FrmFunctionConfigSwitch() == S_SUCCESS )
					{
						FrmMgr.FrmFunctionSwitchTimeDelay = Def_GuiTimeDelayCnt3s;
						GUI_RefreshSleepTime();
						if ( FrmMgr.FrmFunctionConfig == FRM_Enabled )
						{
							FrmMgr.FrmFunctionConfig = FRM_Disabled;
							PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_FrmFunctionDisbled);
						}
						else
						{
							FrmMgr.FrmFunctionConfig = FRM_Enabled;
							PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_FrmFunctionEnabled);
						}
						FrmFunctionConfigSave();	
					}
				}
			}
			#endif
			else
			{
				UserIdentifyResultMgr.IdentifyType = INITIALSTATUS;
				Enable_KEYLED_WATERLIGHT(); 
				CurrentScreen = SCREEN_IdentifySuccess;
				DisplayDoorStatusMgr.Status = DoorOpenInit;
				GUI_RefreshSleepTime();
				UserIdentifyResultMgr.TimeCnt = Def_IdentifySuccessScreenTimeLimited;
			}		
		}
		else
		{
			GUI_RefreshSleepTime();		//refresh to 10s
			UserIdentifyResultMgr.CardIdentifyStatus = S_FAIL;
			UserIdentifyResultMgr.IdentifyType = CARD;
			UserIdentifyResultMgr.CardUserID = 0x00;
			CurrentScreen = SCREEN_IdentifyFail;
			UserIdentifyResultMgr.TimeCnt = Def_MessageBoxTimeDelay;
			SET_RBLED_R_Flash(3);
			PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_IdentifyFail);	
			if ( SafetyMonitorMgr.CardIdentifyFailedTimes < DEF_CardIdentifyFailedTimesLimited )
			{
				SafetyMonitorMgr.CardIdentifyFailedTimes++;
			}
			else
			{
				SafetyMonitorMgr.SystemLocked = bTRUE;
				SafetyMonitorMgr.SystemLockedTimeDelay = DEF_SystemLockedTime;
				ComPort_SetPost_Alarm(DEF_WifiAlarm_CardUnlockAlarm,CARDUSER,0x00);
				#if defined (Function_YouzhiyunjiaWifi) || defined (Function_TuyaWifi)
				Wifi_PostEvent(DEF_WifiEvent_CardUnlockAlarm,CARDUSER,0x00);
				#endif
			}
		}
	}

	FpUserIdentify();
	if ( FpIdentifyMgr.Status == success)
	{

		UserIdentifyResultMgr.FPIdentifyStatus = S_SUCCESS;
		UserIdentifyResultMgr.FPUserID = FpIdentifyMgr.UserID+1;
		SafetyMonitorMgr.FpIdentifyFailedTimes = 0x00;
        SafetyMonitorMgr.CardIdentifyFailedTimes = 0x00;
        SafetyMonitorMgr.PasscodeIdentifyFailedTimes = 0x00;
		SafetyMonitorMgr.FaceIdentifyFailedTimes = 0x00;

		if (UserIdentifyResultMgr.UnlockingMode == SingalMode )
		{
			UserIdentifyResultMgr.IdentifyType = FINGERPRINT;
			Enable_KEYLED_WATERLIGHT();	
			CurrentScreen = SCREEN_IdentifySuccess;
			DisplayDoorStatusMgr.Status = DoorOpenInit;
			UserIdentifyResultMgr.TimeCnt = Def_IdentifySuccessScreenTimeLimited;	
			//SafetyMonitorMgr.IdentifyFailedTimes = 0x00;
		}
		else if (UserIdentifyResultMgr.UnlockingMode == DoubleMode )
		{
			if (( UserIdentifyResultMgr.CardIdentifyStatus == S_SUCCESS))
			{	
				UserIdentifyResultMgr.IdentifyType = FINGERPRINTandCARD;
				Enable_KEYLED_WATERLIGHT();	
				CurrentScreen = SCREEN_IdentifySuccess;
				DisplayDoorStatusMgr.Status = DoorOpenInit;
				UserIdentifyResultMgr.TimeCnt = Def_IdentifySuccessScreenTimeLimited;	
				//SafetyMonitorMgr.IdentifyFailedTimes = 0x00;
			}
			else if (( UserIdentifyResultMgr.PasscodeIdentifyStatus == S_SUCCESS ))
			{	
				UserIdentifyResultMgr.IdentifyType = FINGERPRINTandPASSCODE;	
				Enable_KEYLED_WATERLIGHT();	
				CurrentScreen = SCREEN_IdentifySuccess;
				DisplayDoorStatusMgr.Status = DoorOpenInit;
				UserIdentifyResultMgr.TimeCnt = Def_IdentifySuccessScreenTimeLimited;	
				//SafetyMonitorMgr.IdentifyFailedTimes = 0x00;
			}
			else{	
				PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_PleaseInputSecondIdentity);
				FpIdentifyMgr.Status = FPMcmdStart;
			}
		}
		
	}
	else if ( FpIdentifyMgr.Status == fail)
	{
		if ( FpIdentifyMgr.ErrorType == Error_SerialNumberMismatched )
		{
			UserIdentifyResultMgr.ErrorType = FPMserialNumberMismatched;
		}
		else{
			UserIdentifyResultMgr.ErrorType = UserUnregistered;
		}
		
		if ( ( IfSystemIsInFactoryDefaultStatus()==bTRUE )
			//&& (UserIdentifyResultMgr.ErrorType != FPMserialNumberMismatched)
			)
		{
			//UserIdentifyResultMgr.FPIdentifyStatus = S_SUCCESS;
			UserIdentifyResultMgr.IdentifyType = INITIALSTATUS;
			Enable_KEYLED_WATERLIGHT();	
			CurrentScreen = SCREEN_IdentifySuccess;
			DisplayDoorStatusMgr.Status = DoorOpenInit;
			UserIdentifyResultMgr.TimeCnt = Def_IdentifySuccessScreenTimeLimited;
			
		}
		else
		{
			UserIdentifyResultMgr.FPIdentifyStatus = S_FAIL;
			UserIdentifyResultMgr.IdentifyType = FINGERPRINT;
			UserIdentifyResultMgr.FPUserID = 0x00;
			CurrentScreen = SCREEN_IdentifyFail;
			UserIdentifyResultMgr.TimeCnt = Def_MessageBoxTimeDelay;
			SET_RBLED_R_Flash(3);
			PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_IdentifyFail);
			
			if ( SafetyMonitorMgr.FpIdentifyFailedTimes < DEF_FpIdentifyFailedTimesLimited )
			{
				SafetyMonitorMgr.FpIdentifyFailedTimes++;
			}
			else
			{
				SafetyMonitorMgr.SystemLocked = bTRUE;
				SafetyMonitorMgr.SystemLockedTimeDelay = DEF_SystemLockedTime;
				ComPort_SetPost_Alarm(DEF_WifiAlarm_FpUnlockAlarm,FPUSER,0x00);
				#if defined (Function_YouzhiyunjiaWifi) || defined (Function_TuyaWifi)
				Wifi_PostEvent(DEF_WifiEvent_FpUnlockAlarm,FPUSER,0x00);
				#endif
			}
		
		}

	}

	PasscodeUserIdentify();
	if (PasscodeUserIdentifyMgr.Status == PasscodeIdentifySuccess)
	{
		UserIdentifyResultMgr.PasscodeIdentifyStatus = S_SUCCESS;
		UserIdentifyResultMgr.PasscodeUserID = PasscodeUserIdentifyMgr.UserID;
		SafetyMonitorMgr.FpIdentifyFailedTimes = 0x00;
        SafetyMonitorMgr.CardIdentifyFailedTimes = 0x00;
        SafetyMonitorMgr.PasscodeIdentifyFailedTimes = 0x00;
		SafetyMonitorMgr.FaceIdentifyFailedTimes = 0x00;

		if ( UserIdentifyResultMgr.PasscodeType == AppPasscode )
		{
			UserIdentifyResultMgr.IdentifyType = PASSCODE;
			Enable_KEYLED_WATERLIGHT(); 
			CurrentScreen = SCREEN_IdentifySuccess;
			DisplayDoorStatusMgr.Status = DoorOpenInit;
			UserIdentifyResultMgr.TimeCnt = Def_IdentifySuccessScreenTimeLimited;	
		}
		else if ( UserIdentifyResultMgr.PasscodeType == OnlinePasscode )
		{
			UserIdentifyResultMgr.IdentifyType = ONLINEPASSCODE;
			Enable_KEYLED_WATERLIGHT(); 
			CurrentScreen = SCREEN_IdentifySuccess;
			DisplayDoorStatusMgr.Status = DoorOpenInit;
			UserIdentifyResultMgr.TimeCnt = Def_IdentifySuccessScreenTimeLimited;
		}
		else
		{
			if (UserIdentifyResultMgr.UnlockingMode == SingalMode )
			{
				UserIdentifyResultMgr.IdentifyType = PASSCODE;
				Enable_KEYLED_WATERLIGHT();	
				CurrentScreen = SCREEN_IdentifySuccess;
				DisplayDoorStatusMgr.Status = DoorOpenInit;
				UserIdentifyResultMgr.TimeCnt = Def_IdentifySuccessScreenTimeLimited;	
			}
			else if (UserIdentifyResultMgr.UnlockingMode == DoubleMode )
			{
				if (( UserIdentifyResultMgr.CardIdentifyStatus == S_SUCCESS ))
				{	
					UserIdentifyResultMgr.IdentifyType = CARDandPASSCODE;
					Enable_KEYLED_WATERLIGHT();	
					CurrentScreen = SCREEN_IdentifySuccess;
					DisplayDoorStatusMgr.Status = DoorOpenInit;
					UserIdentifyResultMgr.TimeCnt = Def_IdentifySuccessScreenTimeLimited;	
					//SafetyMonitorMgr.IdentifyFailedTimes = 0x00;
				}
				else if (( UserIdentifyResultMgr.FPIdentifyStatus == S_SUCCESS ))
				{	
					UserIdentifyResultMgr.IdentifyType = FINGERPRINTandPASSCODE;	
					Enable_KEYLED_WATERLIGHT();	
					CurrentScreen = SCREEN_IdentifySuccess;
					DisplayDoorStatusMgr.Status = DoorOpenInit;
					UserIdentifyResultMgr.TimeCnt = Def_IdentifySuccessScreenTimeLimited;	
					//SafetyMonitorMgr.IdentifyFailedTimes = 0x00;
				}
	            else{	
	                PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_PleaseInputSecondIdentity);
	                PasscodeUserIdentifyMgr.Status = PasscodeIdentifyIdle;
	            }
			}
		}
	}
	else if ( PasscodeUserIdentifyMgr.Status == PasscodeIdentifyFail )
	{
		if ( IfSystemIsInFactoryDefaultStatus()==bTRUE )
		{
			//UserIdentifyResultMgr.FPIdentifyStatus = S_SUCCESS;
			UserIdentifyResultMgr.IdentifyType = INITIALSTATUS;
			Enable_KEYLED_WATERLIGHT(); 
			CurrentScreen = SCREEN_IdentifySuccess;
			DisplayDoorStatusMgr.Status = DoorOpenInit;
			UserIdentifyResultMgr.TimeCnt = Def_IdentifySuccessScreenTimeLimited;
		}
		else
		{
			UserIdentifyResultMgr.PasscodeIdentifyStatus = S_FAIL;
			UserIdentifyResultMgr.IdentifyType = PASSCODE;
			UserIdentifyResultMgr.PasscodeUserID = 0x00;
			CurrentScreen = SCREEN_IdentifyFail;
			UserIdentifyResultMgr.TimeCnt = Def_MessageBoxTimeDelay;
			SET_RBLED_R_Flash(3);
			PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_IdentifyFail);	
			if ( SafetyMonitorMgr.PasscodeIdentifyFailedTimes < DEF_PasscodeIdentifyFailedTimesLimited )
			{
				SafetyMonitorMgr.PasscodeIdentifyFailedTimes++;
			}
			else
			{
				SafetyMonitorMgr.SystemLocked = bTRUE;
				SafetyMonitorMgr.SystemLockedTimeDelay = DEF_SystemLockedTime;
				ComPort_SetPost_Alarm(DEF_WifiAlarm_PasswordUnlockAlarm,PASSCODEUSER,0x00);
				#if defined (Function_YouzhiyunjiaWifi) || defined (Function_TuyaWifi)
				Wifi_PostEvent(DEF_WifiEvent_PasswordUnlockAlarm,PASSCODEUSER,0x00);
				#endif
			}
		}
	}
	else if (PasscodeUserIdentifyMgr.Status == PasscodeIdentifyAgingTestSuccess )
	{
		AgingTestMgr.MotorRunTimes=0x00000000;		
		AgingTestMgr.SlideRunTimes=0x0000;	
		AgingTestMgr.TestFlag = bFALSE;
		
		AutoMotorMgr.UnlockTime=2;
		AutoMotorMgr.AutoLockTime=5;
		ComPort_SetPost_Parameter();
		
		CurrentScreen = SCREEN_AgingTest;
		
	}
	else if ( PasscodeUserIdentifyMgr.Status == PasscodeIdentifyEngineeringModeSuccess )
	{
		GoIntoEngineeringModeMenu_Init();	
		GUI_RefreshSleepTime();
	}
	else if ( PasscodeUserIdentifyMgr.Status == PasscodeIdentifyWifiMFTSuccess)
	{
		WifiMgr.MFT.Status = MFTStart;
		CurrentScreen = SCREEN_WifiMFT;	
		PasscodeUserIdentifyMgr.Status = PasscodeIdentifyIdle;
	}
	else if (PasscodeUserIdentifyMgr.Status == PasscodeIdentifyRemoteUnlockRequestSuccess)
	{
		CurrentScreen = SCREEN_RemoteUnlockRequest;
		WifiMgr.RemoteUnlockMgr.Status = RemoteUnlockRequestInit;
		PasscodeUserIdentifyMgr.Status = PasscodeIdentifyIdle;
	}
	else if ( PasscodeUserIdentifyMgr.Status == PasscodeIdentifySelfTestSuccess )
	{
		SelfTestMgr.Status = SELFTEST_START;
		CurrentScreen = SCREEN_SelfTest;	
		PasscodeUserIdentifyMgr.Status = PasscodeIdentifyIdle;
	}
	else if ( PasscodeUserIdentifyMgr.Status == PasscodeIdentifyFastVersionCheckSuccess )
	{
		GoIntoShowSystemVersion();	
		PasscodeUserIdentifyMgr.Status = PasscodeIdentifyIdle;
		GUI_RefreshSleepTime();
	}
	else if ( PasscodeUserIdentifyMgr.Status == PasscodeIdentifyLanguageSetToEnglishSuccess )
	{
		PasscodeInputMgr.Point = 0x00;
		PasscodeInputMgr.PasscodeLen = 16;
		PasscodeUserIdentifyMgr.Status = PasscodeIdentifyIdle;
		
		SystemLanguage = English;
		SystemConfigSave();
		VoiceReportCurrentLanguage();
	}
	else if ( PasscodeUserIdentifyMgr.Status == PasscodeIdentifyLanguageSetToChineseSuccess )
	{
		PasscodeInputMgr.Point = 0x00;
		PasscodeInputMgr.PasscodeLen = 16;
		PasscodeUserIdentifyMgr.Status = PasscodeIdentifyIdle;
	
		SystemLanguage = Chinese;
		SystemConfigSave();
		VoiceReportCurrentLanguage();			
	}

	else if ( PasscodeUserIdentifyMgr.Status == PasscodeIdentifyIntoMainMenuSuccess )
	{
		CurrentScreen = SCREEN_ManagerIdentify;
		ManagerIdentifyMgr.Status = StartManagerIdentify;
	}

	if ( FrmMgr.FrmFunctionConfig == FRM_Enabled )
	{
		if ( PasscodeInputMgr.Point > 0 )
		{
			FaceIdentifyMgr.IndentifyDelayTimeCnt = Def_GuiTimeDelayCnt1s;
		}
		
		if ( FaceIdentifyMgr.IndentifyDelayTimeCnt > 0x0000 )
		{
			 FaceIdentifyMgr.IndentifyDelayTimeCnt--;	 
		}
		
		if ((( IfSystemIsInFactoryDefaultStatus() == bTRUE )		//Face identify Demo mode is enabled
			||( IfSystemIsNoFaceUser() == bFALSE ))		//system is has face user
			&&( FaceIdentifyMgr.IndentifyDelayTimeCnt == 0x0000 )
			&&( g_ASTERISK_PressedOnMainScreen == bFALSE )		//wait user press # into manager identify screen
			)
		{
			FaceUserIdentify();
			if ( FaceIdentifyMgr.Status == FaceIdentifySuccess )
			{
				UserIdentifyResultMgr.FaceIdentifyStatus = S_SUCCESS;
				UserIdentifyResultMgr.FaceUserID = GetUserIDbyFaceTemplateID(FrmMgr.UserID);
				SafetyMonitorMgr.FpIdentifyFailedTimes = 0x00;
		        SafetyMonitorMgr.CardIdentifyFailedTimes = 0x00;
		        SafetyMonitorMgr.PasscodeIdentifyFailedTimes = 0x00;
				SafetyMonitorMgr.FaceIdentifyFailedTimes = 0x00;
				//if (UserIdentifyResultMgr.UnlockingMode == SingalMode )
				{
					UserIdentifyResultMgr.IdentifyType = FACE;
					Enable_KEYLED_WATERLIGHT();	
					CurrentScreen = SCREEN_IdentifySuccess;
					DisplayDoorStatusMgr.Status = DoorOpenInit;
					UserIdentifyResultMgr.TimeCnt = Def_IdentifySuccessScreenTimeLimited;	
				}
			}
			else if ( FaceIdentifyMgr.Status == FaceIdentifySuccess_NoUser)
			{
				UserIdentifyResultMgr.IdentifyType = INITIALSTATUS;
				Enable_KEYLED_WATERLIGHT(); 
				CurrentScreen = SCREEN_IdentifySuccess;
				DisplayDoorStatusMgr.Status = DoorOpenInit;
				UserIdentifyResultMgr.TimeCnt = Def_IdentifySuccessScreenTimeLimited;	
			}
			else if ( FaceIdentifyMgr.Status == FaceIdentifyFail )
			{
				FaceIdentifyMgr.Status = FrmIdentifyStart;
				DEBUG_MARK;
			}
		}
		else
		{
			if ( FrmMgr.PowerStatus != FRM_PowerOff )
			{
				FaceRecognition_HardwarePowerOff();
			}	
		}
	}

	DisplayMainPage();

	if (SystemPowerMgr.SleepDelayTimerCnt == 0x0000)
	{
		Clear_Screen();
		SET_ALLKEYLED_OFF();
	}

}


void GoIntoMainMenu_Init(void)
{
	VoiceMenuMgr.MenuPoint = 0;
	VoiceMenuMgr.TotalMenuNum = 6;
	CurrentScreen = SCREEN_MainMenu;
	DEF_MenuSwitchDelayTime;
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowMainMenu(void)
{
	uint8_t i;

	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,MainMenuVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}
	for (i=0;i<4;i++)
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(2*i,0,MainMenuStr[i],NormalDisplay);
		}
		else{
			DisEN16x8Str(2*i,0,MainMenuStrEn[i],NormalDisplay);
		}
	}		
	GUI_Flag_RefreshLCD = bTRUE;
}


void GoIntoUserManagementMenu_Init(void)
{
	if ( FrmMgr.FrmFunctionConfig == FRM_Enabled )
	{
		VoiceMenuMgr.TotalMenuNum = 6;
	}
	else
	{
		VoiceMenuMgr.TotalMenuNum = 5;
	}
	VoiceMenuMgr.MenuPoint = 0;
	CurrentScreen = SCREEN_UserManagementMenu;
	DEF_MenuSwitchDelayTime;
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowUserManagementMenu(void)
{
	uint8_t i,j,StartMenuNum,StopMenuNum;
	// if ( FrmMgr.FrmFunctionConfig == FRM_Enabled )
	// {
		if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
		{
			if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
			{
				PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,UserManageMenuVoiceBuff[VoiceMenuMgr.MenuPoint]);
				VoiceMenuMgr.MenuPoint++;
			}
		}
		for (i=0;i<3;i++)
		{
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(2*i,0,UserManageMenuStr[i],NormalDisplay);
			}
			else{
				DisEN16x8Str(2*i,0,UserManageMenuStrEn[i],NormalDisplay);
			}
		}
	// }
	/* else
	{
		if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
		{
			if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
			{
				PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,UserManagementMenuVoiceBuff[VoiceMenuMgr.MenuPoint]);
				VoiceMenuMgr.MenuPoint++;
			}
		}
		for (i=0;i<3;i++)
		{
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(2*i,0,UserManagementMenuStr[i],NormalDisplay);
			}
			else{
				DisEN16x8Str(2*i,0,UserManagementMenuStrEn[i],NormalDisplay);
			}
		}
	} */
	GUI_Flag_RefreshLCD = bTRUE;
}


/*******************************************************/
/*******************************************************/
/*******************************************************/
void GoIntoFaceMenu_Init(void)
{
	VoiceMenuMgr.MenuPoint = 0;
	VoiceMenuMgr.TotalMenuNum = 5;
	CurrentScreen = SCREEN_FaceMenu;
	DEF_MenuSwitchDelayTime;
}

void AddManagerMenu_Init(void)
{
	VoiceMenuMgr.MenuPoint = 0;
	VoiceMenuMgr.TotalMenuNum = 4;
	CurrentScreen = SCREEN_ManagerMenu;
	DEF_MenuSwitchDelayTime;
}

void AddUserMenu_Init(void)
{
	VoiceMenuMgr.MenuPoint = 0;
	VoiceMenuMgr.TotalMenuNum = 4;
	CurrentScreen = SCREEN_UserMenu;
	DEF_MenuSwitchDelayTime;
}

void DeleteUserMenu_Init(void)
{
	VoiceMenuMgr.MenuPoint = 0;
	VoiceMenuMgr.TotalMenuNum = 4;
	CurrentScreen = SCREEN_DeleteUserMenu;
	DEF_MenuSwitchDelayTime;
}

void GoInToSCREEN_RegisterManagerFPCInit(void){
	VoiceMenuMgr.MenuPoint = 0;
	VoiceMenuMgr.TotalMenuNum = 1;
	CurrentScreen = SCREEN_RegisterManagerFPC;
	DEF_MenuSwitchDelayTime;
}

void GoInToSCREEN_RegisterManagerFaceInit(void){
	VoiceMenuMgr.MenuPoint = 0;
	VoiceMenuMgr.TotalMenuNum = 1;
	CurrentScreen = SCREEN_RegisterManagerFace;
	DEF_MenuSwitchDelayTime;
}

void GoInToSCREEN_RegisterUserFPCInit(void){
	VoiceMenuMgr.MenuPoint = 0;
	VoiceMenuMgr.TotalMenuNum = 1;
	CurrentScreen = SCREEN_RegisterNormalUserFPC;
	DEF_MenuSwitchDelayTime;
}

void GoInToSCREEN_RegisterUserFace(void){
	VoiceMenuMgr.MenuPoint = 0;
	VoiceMenuMgr.TotalMenuNum = 1;
	CurrentScreen = SCREEN_RegisterNormalUserFace;
	DEF_MenuSwitchDelayTime;
}

void GoInToSCREEN_DeleteUser(void){
	VoiceMenuMgr.MenuPoint = 0;
	VoiceMenuMgr.TotalMenuNum = 1;
	CurrentScreen = SCREEN_NumberDelete;
	DEF_MenuSwitchDelayTime;
}

void GoInToSCREEN_AllDeleteUser(void){
	VoiceMenuMgr.MenuPoint = 0;
	VoiceMenuMgr.TotalMenuNum = 1;
	CurrentScreen = SCREEN_AllDelete;
	DEF_MenuSwitchDelayTime;
}

void ShowOperateSuccess(void){
	VoiceMenuMgr.MenuPoint = 0;
	VoiceMenuMgr.TotalMenuNum = 1;
	CurrentScreen = SCREEN_OperateSuccess;
	DEF_MenuSwitchDelayTime;
}

void GoInTo_ResManagerIdFPCInit(void){
	VoiceMenuMgr.MenuPoint = 0;
	VoiceMenuMgr.TotalMenuNum = 3;
	CurrentScreen = SCREEN_ResManagerIdFPC;
	DEF_MenuSwitchDelayTime;
}

void GoInTo_ResManagerIdFaceInit(void){
	VoiceMenuMgr.MenuPoint = 0;
	VoiceMenuMgr.TotalMenuNum = 3;
	CurrentScreen = SCREEN_ResManagerIdFace;
	DEF_MenuSwitchDelayTime;
}

void GoInTo_ResNormalIdFPCInit(void){
	VoiceMenuMgr.MenuPoint = 0;
	VoiceMenuMgr.TotalMenuNum = 3;
	CurrentScreen = SCREEN_ResNormalUserIdFPC;
	DEF_MenuSwitchDelayTime;
}

void GoInTo_ResNormalIdFaceInit(void){
	VoiceMenuMgr.MenuPoint = 0;
	VoiceMenuMgr.TotalMenuNum = 3;
	CurrentScreen = SCREEN_ResNormalUserIdFace;
	DEF_MenuSwitchDelayTime;
}

void GoInTo_DelUserIdInit(void){
	VoiceMenuMgr.MenuPoint = 0;
	VoiceMenuMgr.TotalMenuNum = 3;
	CurrentScreen = SCREEN_DelUserId;
	DEF_MenuSwitchDelayTime;
}

void ShowManagerMenu(void){
	uint8_t i;

	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,AddAdminUserVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}
	
	uint8_t j = 1;
	for (i=0;i<2;i++)
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(2*i,0,AdminMenuStr[i],NormalDisplay);
		}
		else{
			DisEN16x8Str(2*i,0,AdminMenuStrEn[i],NormalDisplay);
		}

	}	
}

void ShowNormalMenu(void){
	uint8_t i;

	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,AddAdminUserVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}

	for (i=0;i<2;i++)
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(2*i,0,AdminMenuStr[i],NormalDisplay);
		}
		else{
			DisEN16x8Str(2*i,0,AdminMenuStrEn[i],NormalDisplay);
		}

	}	
}

void ShowDeleteUserMenu(void){
	uint8_t i;

	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,DeleteUserVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}

	for (i=0;i<2;i++)
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(2*i,0,DeleteUserMenuStr[i],NormalDisplay);
		}
		else{
			DisEN16x8Str(2*i,0,DeleteUserMenuStrEn[i],NormalDisplay);
		}

	}
}

void ShowRegisterManagerFPCPage(void){
	uint8_t i;

	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,RegisterManagerFPCVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}


	if (SystemLanguage == Chinese){
		DisHZ16x14Str(0,0,RegisterFPCPageStr[0],NormalDisplay);
		DisHZ16x14Str(6,0,RegisterFPCPageStr[3],NormalDisplay);
	}
	else{
		DisEN16x8Str(0,0,RegisterFPCPageStrEn[0],NormalDisplay);
		DisEN16x8Str(6,0,RegisterFPCPageStrEn[3],NormalDisplay);
	}
	
	GUI_DataInputCreat(2,53,3,0x0001);

	
}

void ShowResManagerIDFPC(void){
	
	uint16_t value = DataInputMgr.Value;
	
	const uint8_t TitleStr[]={HZ_bian,HZ_hao,ZF_maohao,HZ_end};	// 编号
	const uint8_t TitleStrEn[]={"ID:"};
	
	const uint8_t ContentStr[][7]={
	
		{HZ_qing,HZ_shu,HZ_ru,HZ_end},
		{HZ_zhi,HZ_wen,HZ_mi,HZ_ma,HZ_ka,HZ_pian,HZ_end}
	};
	
	const uint8_t ContentStrEn[][20]={
		
		{"Please Enter"},
		{"Fprint Password Card"}
		
	};
	
	uint16_t voiceArray[4];
	voiceArray[0]=TranslateNumberToVoice(value%1000/100);
	voiceArray[1]=TranslateNumberToVoice(value%100/10);
	voiceArray[2]=TranslateNumberToVoice(value%10);
	voiceArray[3] = DEF_VoiceSegmentEndFlag;
	
	static uint16_t UserIdVoiceBuff[][3]=
		{	
			{VOICE_ID,DEF_VoiceSegmentEndFlag},
		};
		
	static uint16_t UserIdResFPCVoiceBuff[][5]=
		{	
			{VOICE_PleaseEnter,VOICE_Fingerprint,VOICE_Password,VOICE_Card,DEF_VoiceSegmentEndFlag},
		};

	static uint16_t* ResManagerIDFPCVoiceBuffer[3];
	
	ResManagerIDFPCVoiceBuffer[0] = UserIdVoiceBuff[0];
	ResManagerIDFPCVoiceBuffer[1] = &(voiceArray[0]);
	ResManagerIDFPCVoiceBuffer[2] = UserIdResFPCVoiceBuff[0];
	
	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE ){
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum ){
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,ResManagerIDFPCVoiceBuffer[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}
	
	if (SystemLanguage == Chinese){
		DisHZ16x14Str(0,0,TitleStr,NormalDisplay);
		DisDigital16x8Str(0,35,value,NormalDisplay);
		DisHZ16x14Str(2,53,ContentStr[0],NormalDisplay);
		DisHZ16x14Str(4,30,ContentStr[1],NormalDisplay);
	}
	else{
		DisEN16x8Str(0,0,TitleStrEn,NormalDisplay);
		DisDigital16x8Str(0,35,value,NormalDisplay);
		DisHZ16x14Str(2,53,ContentStrEn[0],NormalDisplay);
		DisHZ16x14Str(4,0,ContentStrEn[1],NormalDisplay);
	}
	
}



void ShowResManagerIDFace(void){
	uint16_t value = DataInputMgr.Value;
	
	const uint8_t TitleStr[]={HZ_bian,HZ_hao,ZF_maohao,HZ_end};	// 编号
	const uint8_t TitleStrEn[]={"ID:"};
	
	const uint8_t ContentStr[][7]={
	
		{HZ_qing,HZ_zhengque,HZ_shi,HZ_she,HZ_xiang,HZ_tou,HZ_end}
	};
	
	const uint8_t ContentStrEn[][24]={
		
		{"Please stare at camera"}
		
	};
	
	uint16_t voiceArray[4];
	voiceArray[0]=TranslateNumberToVoice(value%1000/100);
	voiceArray[1]=TranslateNumberToVoice(value%100/10);
	voiceArray[2]=TranslateNumberToVoice(value%10);
	voiceArray[3] = DEF_VoiceSegmentEndFlag;
	
	static uint16_t UserIdVoiceBuff[][3]=
		{	
			{VOICE_ID,DEF_VoiceSegmentEndFlag},
		};
		
	static uint16_t UserIdResFPCVoiceBuff[][2]=
		{	
			{VOICE_PleaseInputFace,DEF_VoiceSegmentEndFlag},
		};

	static uint16_t* ResManagerIDFPCVoiceBuffer[3];
	
	ResManagerIDFPCVoiceBuffer[0] = UserIdVoiceBuff[0];
	ResManagerIDFPCVoiceBuffer[1] = &(voiceArray[0]);
	ResManagerIDFPCVoiceBuffer[2] = UserIdResFPCVoiceBuff[0];
	
	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE ){
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum ){
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,ResManagerIDFPCVoiceBuffer[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}
	
	if (SystemLanguage == Chinese){
		DisHZ16x14Str(0,0,TitleStr,NormalDisplay);
		DisDigital16x8Str(0,35,value,NormalDisplay);
		DisHZ16x14Str(4,23,ContentStr[0],NormalDisplay);
	}
	else{
		DisEN16x8Str(0,0,TitleStrEn,NormalDisplay);
		DisDigital16x8Str(0,35,value,NormalDisplay);
		DisHZ16x14Str(4,23,ContentStrEn[0],NormalDisplay);
	}
}

void ShowRegisterManagerFacePage(void){
	uint8_t i;

	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,RegisterManagerFPCVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}

	if (SystemLanguage == Chinese){
		DisHZ16x14Str(0,0,RegisterFPCPageStr[0],NormalDisplay);
		DisHZ16x14Str(6,0,RegisterFPCPageStr[3],NormalDisplay);
	}
	else{
		DisEN16x8Str(0,0,RegisterFPCPageStrEn[0],NormalDisplay);
		DisEN16x8Str(6,0,RegisterFPCPageStrEn[3],NormalDisplay);
	}
	
	GUI_DataInputCreat(2,53,3,0x0001);
}

void ShowRegisterUserFPCPage(void){
	uint8_t i;

	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,RegisterManagerFPCVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}

	for (i=0;i<4;i++)
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(2*i,0,RegisterUserFPCPageStr[i],NormalDisplay);
		}
		else{
			DisEN16x8Str(2*i,0,RegisterUserFPCPageStrEn[i],NormalDisplay);
		}

	}
	
	GUI_DataInputCreat(2,53,3,0x000a);
}

void ShowResNormalIDFPC(void){
	uint16_t value = DataInputMgr.Value;
	
	const uint8_t TitleStr[]={HZ_bian,HZ_hao,ZF_maohao,HZ_end};	// 编号
	const uint8_t TitleStrEn[]={"ID:"};
	
	const uint8_t ContentStr[][7]={
	
		{HZ_qing,HZ_shu,HZ_ru,HZ_end},
		{HZ_zhi,HZ_wen,HZ_mi,HZ_ma,HZ_ka,HZ_pian,HZ_end}
	};
	
	const uint8_t ContentStrEn[][20]={
		
		{"Please Enter"},
		{"Fprint Password Card"}
		
	};
	
	uint16_t voiceArray[4];
	voiceArray[0]=TranslateNumberToVoice(value%1000/100);
	voiceArray[1]=TranslateNumberToVoice(value%100/10);
	voiceArray[2]=TranslateNumberToVoice(value%10);
	voiceArray[3] = DEF_VoiceSegmentEndFlag;
	
	static uint16_t UserIdVoiceBuff[][3]=
		{	
			{VOICE_ID,DEF_VoiceSegmentEndFlag},
		};
		
	static uint16_t UserIdResFPCVoiceBuff[][5]=
		{	
			{VOICE_PleaseEnter,VOICE_Fingerprint,VOICE_Password,VOICE_Card,DEF_VoiceSegmentEndFlag},
		};

	static uint16_t* ResManagerIDFPCVoiceBuffer[3];
	
	ResManagerIDFPCVoiceBuffer[0] = UserIdVoiceBuff[0];
	ResManagerIDFPCVoiceBuffer[1] = &(voiceArray[0]);
	ResManagerIDFPCVoiceBuffer[2] = UserIdResFPCVoiceBuff[0];
	
	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE ){
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum ){
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,ResManagerIDFPCVoiceBuffer[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}
	
	if (SystemLanguage == Chinese){
		DisHZ16x14Str(0,0,TitleStr,NormalDisplay);
		DisDigital16x8Str(0,35,value,NormalDisplay);
		DisHZ16x14Str(2,53,ContentStr[0],NormalDisplay);
		DisHZ16x14Str(4,30,ContentStr[1],NormalDisplay);
	}
	else{
		DisEN16x8Str(0,0,TitleStrEn,NormalDisplay);
		DisDigital16x8Str(0,35,value,NormalDisplay);
		DisHZ16x14Str(2,53,ContentStrEn[0],NormalDisplay);
		DisHZ16x14Str(4,0,ContentStrEn[1],NormalDisplay);
	}
}

void ShowResNormalIDFace(void){
	uint16_t value = DataInputMgr.Value;
	
	const uint8_t TitleStr[]={HZ_bian,HZ_hao,ZF_maohao,HZ_end};	// 编号
	const uint8_t TitleStrEn[]={"ID:"};
	
	const uint8_t ContentStr[][7]={
	
		{HZ_qing,HZ_zhengque,HZ_shi,HZ_she,HZ_xiang,HZ_tou,HZ_end}
	};
	
	const uint8_t ContentStrEn[][24]={
		
		{"Please stare at camera"}
		
	};
	
	uint16_t voiceArray[4];
	voiceArray[0]=TranslateNumberToVoice(value%1000/100);
	voiceArray[1]=TranslateNumberToVoice(value%100/10);
	voiceArray[2]=TranslateNumberToVoice(value%10);
	voiceArray[3] = DEF_VoiceSegmentEndFlag;
	
	static uint16_t UserIdVoiceBuff[][3]=
		{	
			{VOICE_ID,DEF_VoiceSegmentEndFlag},
		};
		
	static uint16_t UserIdResFPCVoiceBuff[][2]=
		{	
			{VOICE_PleaseInputFace,DEF_VoiceSegmentEndFlag},
		};

	static uint16_t* ResManagerIDFPCVoiceBuffer[3];
	
	ResManagerIDFPCVoiceBuffer[0] = UserIdVoiceBuff[0];
	ResManagerIDFPCVoiceBuffer[1] = &(voiceArray[0]);
	ResManagerIDFPCVoiceBuffer[2] = UserIdResFPCVoiceBuff[0];
	
	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE ){
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum ){
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,ResManagerIDFPCVoiceBuffer[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}
	
	if (SystemLanguage == Chinese){
		DisHZ16x14Str(0,0,TitleStr,NormalDisplay);
		DisDigital16x8Str(0,35,value,NormalDisplay);
		DisHZ16x14Str(4,23,ContentStr[0],NormalDisplay);
	}
	else{
		DisEN16x8Str(0,0,TitleStrEn,NormalDisplay);
		DisDigital16x8Str(0,35,value,NormalDisplay);
		DisHZ16x14Str(4,23,ContentStrEn[0],NormalDisplay);
	}
}

void ShowRegisterUserFacePage(void){
	uint8_t i;

	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,RegisterManagerFPCVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}

	if (SystemLanguage == Chinese){
		DisHZ16x14Str(0,0,RegisterUserFPCPageStr[0],NormalDisplay);
		DisHZ16x14Str(6,0,RegisterUserFPCPageStr[3],NormalDisplay);
	}
	else{
		DisEN16x8Str(0,0,RegisterUserFPCPageStrEn[0],NormalDisplay);
		DisEN16x8Str(6,0,RegisterUserFPCPageStrEn[3],NormalDisplay);
	}
	
	GUI_DataInputCreat(2,53,3,0x000a);
}

void ShowDeleteUserPage(void){
	uint8_t i;

	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,RegisterManagerFPCVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}

	if (SystemLanguage == Chinese){
		DisHZ16x14Str(0,0,DeleteUserPageStr[0],NormalDisplay);
		DisHZ16x14Str(6,0,DeleteUserPageStr[3],NormalDisplay);
	}
	else{
		DisEN16x8Str(0,0,DeleteUserPageStrEn[0],NormalDisplay);
		DisEN16x8Str(6,0,DeleteUserPageStrEn[3],NormalDisplay);
	}
	
	GUI_DataInputCreat(2,53,3,0x000a);
	
}

void ShowDelUserId(void){
	uint16_t value = DataInputMgr.Value;
	#ifdef 0
	const uint8_t Content1Str[]={HZ_cao,HZ_zhuo,HZ_shibai,HZ_bai,HZ_end};	// 编号
	const uint8_t Content1strEn[]={"Operate failure"};
	
	const uint8_t Content2Str[]{HZ_yong,HZ_hu,HZ_bu,HZ_cun,HZ_zaima,HZ_end};	// 用户不存在
	const uint8_t Content2strEn[]={"The user does not exist"};
	#endif
	const uint8_t Content3Str[]={HZ_shan,HZ_chufa,HZ_cheng,HZ_gong,HZ_end};
	const uint8_t Content3StrEn[]={"successfully delete"};
	
	#ifdef 0
	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE ){
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum ){
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,ResManagerIDFPCVoiceBuffer[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}
	#endif
	if (SystemLanguage == Chinese){
		DisHZ16x14Str(2,23,Content3Str,NormalDisplay);
	}
	else{
		DisHZ16x14Str(2,23,Content3StrEn,NormalDisplay);
	}
	
}

void ShowAllDeleteUserPage(void){
	uint8_t i;

	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,AllDeleteUserVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}

	for (i=0;i<4;i++)
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(2*i,0,DeleteAllUserPageStr[i],NormalDisplay);
		}
		else{
			DisEN16x8Str(2*i,0,DeleteAllUserPageStrEn[i],NormalDisplay);
		}

	}
}

void ShowOperateSuccessPage(void){
	uint8_t i;
	
		if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
		{
			if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
			{
				PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,OperateSucessVoiceBuff[VoiceMenuMgr.MenuPoint]);
				VoiceMenuMgr.MenuPoint++;
			}
		}
		for (i=0;i<2;i++)
		{
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(2*i,0,OperateSuccessStr[i],NormalDisplay);
			}
			else{
				DisEN16x8Str(2*i,0,OperateSuccessStr[i],NormalDisplay);
			}

		}
		
	if(AllDeleteCnt==0){
		CurrentScreen = SCREEN_AllDeleteSuccess;
		AllDeleteCnt = 300;
	}
		
}

void ShowFaceMenu(void)
{
	uint8_t i;

	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,FaceMenuVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}

	for (i=0;i<3;i++)
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(2*i,0,FaceMenuStr[i],NormalDisplay);
		}
		else{
			DisEN16x8Str(2*i,0,FaceMenuStrEn[i],NormalDisplay);
		}

	}		
}

void GoIntoFpMenu_Init(void)
{
	VoiceMenuMgr.MenuPoint = 0;
	VoiceMenuMgr.TotalMenuNum = 6;
	CurrentScreen = SCREEN_FpMenu;
	DEF_MenuSwitchDelayTime;
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowFpMenu(void)
{
	uint8_t i;

	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,FpMenuVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}


	for (i=0;i<4;i++)
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(2*i,0,FpMenuStr[i],NormalDisplay);
		}
		else{
			DisEN16x8Str(2*i,0,FpMenuStrEn[i],NormalDisplay);
		}
	}


	
	GUI_Flag_RefreshLCD = bTRUE;
}

void GoIntoFpDeleteMenu_Init(void)
{
	VoiceMenuMgr.MenuPoint = 0;
	VoiceMenuMgr.TotalMenuNum = 5;
	CurrentScreen = SCREEN_FpDeleteMenu;
	DEF_MenuSwitchDelayTime;
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowFpDeleteMenu(void)
{
	uint8_t i;

	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,FpDeleteMenuVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}

	for (i=0;i<3;i++)
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(2*i,0,FpDeleteMenuStr[i],NormalDisplay);
		}
		else{
			DisEN16x8Str(2*i,0,FpDeleteMenuStrEn[i],NormalDisplay);
		}
	}

	GUI_Flag_RefreshLCD = bTRUE;
}



/*******************************************************/
/*******************************************************/
/*******************************************************/
void GoIntoPasscodeMenu_Init(void)
{
	VoiceMenuMgr.MenuPoint = 0;
	VoiceMenuMgr.TotalMenuNum = 6;
	CurrentScreen = SCREEN_PasscodeMenu;
	DEF_MenuSwitchDelayTime;
}

void ShowPasscodeMenu(void)
{
	uint8_t i;

	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,PasscodeMenuVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}

	for (i=0;i<4;i++)
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(2*i,0,PassCodeMenuStr[i],NormalDisplay);
		}
		else{
			DisEN16x8Str(2*i,0,PassCodeMenuStrEn[i],NormalDisplay);
		}

	}		
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void GoIntoCardMenu_Init(void)
{
	VoiceMenuMgr.MenuPoint = 0;
	VoiceMenuMgr.TotalMenuNum = 5;
	CurrentScreen = SCREEN_CardUserMenu;
	DEF_MenuSwitchDelayTime;
}
void ShowCardMenu(void)
{
	uint8_t i;
	
	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,CardMenuVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}

	for (i=0;i<3;i++)
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(2*i,0,CardUserMenuStr[i],NormalDisplay);
		}
		else{
			DisEN16x8Str(2*i,0,CardUserMenuStrEn[i],NormalDisplay);
		}
	}				
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void GoIntoInfoInquiryMenu_Init(void)
{
	VoiceMenuMgr.MenuPoint = 0;
	VoiceMenuMgr.TotalMenuNum = 6;
	CurrentScreen = SCREEN_InfoInquiryMenu;
	DEF_MenuSwitchDelayTime;
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowInfoInquiryMenu(void)
{
	uint8_t i;
	

	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,InfoInquiryMenuVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}
	
	for (i=0;i<4;i++)
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(2*i,0,InfoInquiryMenuStr[MenuMgr.StartMenuLine+i],NormalDisplay);
		}
		else{
			DisEN16x8Str(2*i,0,InfoInquiryMenuStrEn[MenuMgr.StartMenuLine+i],NormalDisplay);
		}
	}			
}


/*******************************************************/
/*******************************************************/
/*******************************************************/
void GoIntoLogMenu_Init(void)
{
	VoiceMenuMgr.MenuPoint = 0;
	VoiceMenuMgr.TotalMenuNum = 4;
	CurrentScreen = SCREEN_EventLogMenu;
	DEF_MenuSwitchDelayTime;
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowEventLogMenu(void)
{
	uint8_t i;

	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,LogMenuVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}

	for (i=0;i<2;i++)
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(2*i,0,EventLogMenuStr[MenuMgr.StartMenuLine+i],NormalDisplay);
		}
		else{
			DisEN16x8Str(2*i,0,EventLogMenuStrEn[MenuMgr.StartMenuLine+i],NormalDisplay);
		}
	}			
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void GoIntoSystemConfigMenu_Init(void)
{
	VoiceMenuMgr.MenuPoint = 0;
	VoiceMenuMgr.TotalMenuNum = 6;
	CurrentScreen = SCREEN_SystemConfigMenu;
	DEF_MenuSwitchDelayTime;
}

void ShowSystemConfigMenu(void)
{

	uint8_t i;
	
	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,SystemConfigMenuVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}

	for (i=0;i<4;i++)
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(2*i,0,SytemConfigMenuStr[i],NormalDisplay);
		}
		else{
			DisEN16x8Str(2*i,0,SytemConfigMenuStrEn[i],NormalDisplay);
		}
	}		
	GUI_Flag_RefreshLCD = bTRUE;
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void GoIntoVoiceSettingMenu_Init(void)
{
	VoiceMenuMgr.MenuPoint = 0;
	VoiceMenuMgr.TotalMenuNum = 4;
	CurrentScreen = SCREEN_VoiceSettingMenu;
	DEF_MenuSwitchDelayTime;
}
void ShowVoiceSettingMenu(void)
{
	uint8_t i;
	
	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceSettingMenuVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}

	for (i=0;i<2;i++)
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(2*i,0,VoiceSettingMenuStr[i],NormalDisplay);
		}
		else{
			DisEN16x8Str(2*i,0,VoiceSettingMenuStrEn[i],NormalDisplay);
		}
	}				
}


/*******************************************************/
/*******************************************************/
/*******************************************************/
void GoIntoDoorLockSettingMenu_Init(void)
{
	VoiceMenuMgr.MenuPoint = 0;
	CurrentScreen = SCREEN_DoorLockSettingMenu;

	if ( FrmMgr.FrmFunctionConfig == FRM_Enabled )
	{
		VoiceMenuMgr.TotalMenuNum = 5;
	}
	else
	{
		VoiceMenuMgr.TotalMenuNum = 4;
	}
	DEF_MenuSwitchDelayTime;
	
}
void ShowDoorLockSettingMenu(void)
{
	uint8_t i,MenuNum;

	
	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			if ( FrmMgr.FrmFunctionConfig == FRM_Enabled )
			{
				PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,DoorLockSettingVoiceBuff_WithFRM[VoiceMenuMgr.MenuPoint]);
			}
			else
			{
				PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,DoorLockSettingVoiceBuff[VoiceMenuMgr.MenuPoint]);
			}
			VoiceMenuMgr.MenuPoint++;
		}
	}

	if ( FrmMgr.FrmFunctionConfig == FRM_Enabled )
	{
		MenuNum = 3;
	}
	else
	{
		MenuNum = 2;
	}
	
	for (i=0;i<MenuNum;i++)
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(2*i,0,DoorLockSettingMenuStr[i],NormalDisplay);
		}
		else{
			DisEN16x8Str(2*i,0,DoorLockSettingMenuStrEn[i],NormalDisplay);
		}
	}
				
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void GoIntoMotorSettingMenu_Init(void)
{
	VoiceMenuMgr.MenuPoint = 0;
	VoiceMenuMgr.TotalMenuNum = 6;
	CurrentScreen = SCREEN_AutoMotorSettingMenu;
	DEF_MenuSwitchDelayTime;
}
void ShowAutoMotorSettingMenu(void)
{
	uint8_t i,j,StartMenuNum,StopMenuNum;
	
	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,MotorSettingMenuVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}

	for (i=0;i<4;i++)
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(2*i,0,AutoMotorSetingMenuStr[i],NormalDisplay);
		}
		else{
			DisEN16x8Str(2*i,0,AutoMotorSetingMenuStrEN[i],NormalDisplay);
		}
		j++;
	}		
	GUI_Flag_RefreshLCD = bTRUE;
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void GoIntoEngineeringModeMenu_Init(void)
{
	VoiceMenuMgr.MenuPoint = 0;
	if ( FrmMgr.FrmFunctionConfig == FRM_Enabled )
		{
	VoiceMenuMgr.TotalMenuNum = 9;
		}
	else{
	VoiceMenuMgr.TotalMenuNum = 8;
		}
	CurrentScreen = SCREEN_EngineeringModeMneu;
	DEF_MenuSwitchDelayTime;
}
void ShowEngineeringModeMenu(void)
{
	uint8_t i,j,StartMenuNum,StopMenuNum;
	
	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			if ( FrmMgr.FrmFunctionConfig == FRM_Enabled )
			{
				PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,EngineeringModeMenuVoiceBuff_WithFRM[VoiceMenuMgr.MenuPoint]);
			}
			else
			{
				PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,EngineeringModeMenuVoiceBuff[VoiceMenuMgr.MenuPoint]);
			}
			VoiceMenuMgr.MenuPoint++;
		}
	}

	if ( VoiceMenuMgr.MenuPoint < 5 )
	{
		StartMenuNum = 0;
		StopMenuNum = 4;
	}
	else
	{
		StartMenuNum = 4;
		if ( FrmMgr.FrmFunctionConfig == FRM_Enabled )
			{
		StopMenuNum = 7;
			}
		else{
		StopMenuNum = 6;
			}
	}

	j=0;
	for (i=StartMenuNum;i<StopMenuNum;i++)
	{
		if (SystemLanguage == Chinese)
		{
			if ( FrmMgr.FrmFunctionConfig == FRM_Enabled )
			{
				DisHZ16x14Str(2*j,0,EngineeringModeMenuStr_WithFRM[i],NormalDisplay);
			}
			else{
				DisHZ16x14Str(2*j,0,EngineeringModeMenuStr[i],NormalDisplay);
			}
		}
		else
		{	if ( FrmMgr.FrmFunctionConfig == FRM_Enabled )
			{
				DisEN16x8Str(2*j,0,EngineeringModeMenuStrEN_WithFRM[i],NormalDisplay);
			}
			else{
				DisEN16x8Str(2*j,0,EngineeringModeMenuStrEN[i],NormalDisplay);
			}
		}
		j++;
	}		
	GUI_Flag_RefreshLCD = bTRUE;
}


/*******************************************************/
/*******************************************************/
/*******************************************************/
uint8_t Get_Availabe_FPmasterID(void)
{
	uint8_t i;
	for (i=0;i<(DEF_MAX_FPMASTER);i++)
	{
		if ( FpUserMemoryMgr[i].RegisterStatus == UnRegistered ){
			return (i+1);
		}
	}
	return 0;
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
uint8_t Get_Availabe_FPuserID(void)
{
	uint8_t i;
	for (i=(DEF_MAX_FPMASTER);i<(DEF_MAX_FPUSER+DEF_MAX_FPMASTER);i++)
	{
		if ( FpUserMemoryMgr[i].RegisterStatus == UnRegistered ){
			return (i+1);
		}
	}
	return 0;
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
uint8_t Get_Availabe_StressFPuserID(void)
{
	uint8_t i;
	for (i=(DEF_MAX_FPMASTER+DEF_MAX_FPUSER);i<(DEF_MAX_FPMASTER+DEF_MAX_FPUSER+DEF_MAX_STRESSFPUSER);i++)
	{
		if ( FpUserMemoryMgr[i].RegisterStatus == UnRegistered ){
			return (i+1);
		}
	}
	return 0;
}

uint8_t TranslateNumberToVoice(uint8_t value)
{
	return 12+(2*value);
}
void VoiceReportUserID(uint16_t Value)
{
	uint16_t VoiceStr[7];
	VoiceStr[0]=VOICE_Current;
	VoiceStr[1]=VOICE_User;
	VoiceStr[2]=VOICE_ID;
	VoiceStr[3]=TranslateNumberToVoice(Value%1000/100);
	VoiceStr[4]=TranslateNumberToVoice(Value%100/10);
	VoiceStr[5]=TranslateNumberToVoice(Value%10);
	VoiceStr[6]=DEF_VoiceSegmentEndFlag;
	PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr);
}

void VoiceReportUserIDWithUserConfirm(uint16_t Value)
{
	uint16_t VoiceStr[11];
	VoiceStr[0]=VOICE_Current;
	VoiceStr[1]=VOICE_User;
	VoiceStr[2]=VOICE_ID;
	VoiceStr[3]=TranslateNumberToVoice(Value%1000/100);
	VoiceStr[4]=TranslateNumberToVoice(Value%100/10);
	VoiceStr[5]=TranslateNumberToVoice(Value%10);
	VoiceStr[6]=VOICE_PressPoundKey;
	VoiceStr[7]=VOICE_Confirm;
	VoiceStr[8]=VOICE_PressAsteriskKey;
	VoiceStr[9]=VOICE_ReturnPreviousMenu;
	VoiceStr[10]=DEF_VoiceSegmentEndFlag;
	PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr);
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
uint8_t Get_Availabe_FACEmasterID(void)
{
	uint8_t i;
	for (i=0;i<(DEF_MAX_FACEMASTER);i++)
	{
		if ( FaceUserMemoryMgr[i].RegisterStatus != Registered ){
			return (i+1);
		}
	}
	return 0;
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
uint8_t Get_Availabe_FACEuserID(void)
{
	uint8_t i;
	for (i=(DEF_MAX_FACEMASTER);i<(DEF_MAX_FACEUSER+DEF_MAX_FACEMASTER);i++)
	{
		if ( FaceUserMemoryMgr[i].RegisterStatus != Registered ){
			return (i+1);
		}
	}
	return 0;
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
bool_t CheckIfFaceUserIsRegistered(uint16_t USERID)
{
	//uint8_t FPindexTable[32];
	uint8_t ByteOffset,BitOffset;
	
	if ( FaceUserMemoryMgr[USERID-1].RegisterStatus == Registered )
	{
		return bTRUE;
	}
	else
	{
		return bFALSE;
	}
}
/*******************************************************/
/*****		Input CID[5]; 					   *********/
/*****      OUTPUT S_FAIL- MemoryIsFull, SUCCESS-be saved ****************/
/*******************************************************/

status_t SaveFaceUserToMemory(void)
{
	return S_SUCCESS;

}
void WriteFaceUserMemoryToEEPROM(void)
{
	EEPROM_WriteSequential(FaceUserMemoryStartAddr,&FaceUserMemoryMgr[0].UserPriority, sizeof(FaceUserMemoryMgr[0])*(DEF_MAX_FACEMASTER+DEF_MAX_FACEUSER));
}
void ReadFaceUserMemoryFromEEPROM(void)
{
	EEPROM_ReadSequential(FaceUserMemoryStartAddr,&FaceUserMemoryMgr[0].UserPriority, sizeof(FaceUserMemoryMgr[0])*(DEF_MAX_FACEMASTER+DEF_MAX_FACEUSER));
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void DeleteFaceUserfromMemory(uint8_t UserID)
{

	FaceUserMemoryMgr[UserID-1].RegisterStatus = UnRegistered;
	FaceUserMemoryMgr[UserID-1].FaceTemplateID = 0xFFFF;
	FaceUserMemoryMgr[UserID-1].UserPriority = Undefined;
	WriteFaceUserMemoryToEEPROM();
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void DeleteAllFaceUserfromMemory(void)
{
	uint8_t i;

	for (i=0;i<(DEF_MAX_FACEMASTER+DEF_MAX_FACEUSER);i++)
	{
		FaceUserMemoryMgr[i].RegisterStatus = UnRegistered;
		FaceUserMemoryMgr[i].FaceTemplateID = 0xFFFF;
		FaceUserMemoryMgr[i].UserPriority = Undefined;
	}
	WriteFaceUserMemoryToEEPROM();
}

uint8_t GetUserIDbyFaceTemplateID(uint16_t TemplateID)
{
	uint8_t i;
	for (i=0;i<(DEF_MAX_FACEMASTER+DEF_MAX_FACEUSER);i++)
	{
		if ( (FaceUserMemoryMgr[i].FaceTemplateID == TemplateID)
			&&(FaceUserMemoryMgr[i].RegisterStatus == Registered)
			)
		{
			return i+1;
		}
	}
	return 0;
}

uint8_t CheckHowManyRegisteredFaceMaster(void)
{
	uint8_t i,FaceMasterNum;
	
	FaceMasterNum=0;
	
	for (i=0;i<(DEF_MAX_FACEMASTER);i++)
	{
		if ( FaceUserMemoryMgr[i].RegisterStatus == Registered)
		{
			FaceMasterNum++;
		}
	}
	return FaceMasterNum;
}
uint8_t CheckHowManyRegisteredFaceUser(void)
{
	uint8_t i,FaceUserNum;
	
	FaceUserNum=0;
	
	for (i=DEF_MAX_FACEMASTER;i<(DEF_MAX_FACEMASTER+DEF_MAX_FACEUSER);i++)
	{
		if ( FaceUserMemoryMgr[i].RegisterStatus == Registered)
		{
			FaceUserNum++;
		}
	}
	return FaceUserNum;
}

bool_t IfSystemIsNoFaceUser(void)
{
	uint8_t i;
	for (i=0;i<(DEF_MAX_FACEMASTER+DEF_MAX_FACEUSER);i++)
	{
		if ( FaceUserMemoryMgr[i].RegisterStatus == Registered)
		{
			return bFALSE;
		}
	}
	return bTRUE;
}


/*******************************************************/
/*******************************************************/
/*******************************************************/
void RegisterFace(void)
{
	//uint8_t i;

	FaceDirrect_t FaceDirrect;
			
	const uint8_t PleasePutMiddleFace[]={HZ_qing,HZ_shu,HZ_ru,HZ_zhengque,HZ_lianbu,HZ_end};
	const uint8_t PleasePutMiddleFaceEn[]={"PLS put face"};	
	
	const uint8_t PleasePutRightFace[]={HZ_qing,HZ_xiang,HZ_youbian,HZ_weiwei,HZ_weiwei,HZ_qingxie,HZ_xielv,HZ_end};
	const uint8_t PleasePutRightFaceEn[]={"PLS put Right Face"};	

	const uint8_t PleasePutLeftFace[]={HZ_qing,HZ_xiang,HZ_zuo,HZ_weiwei,HZ_weiwei,HZ_qingxie,HZ_xielv,HZ_end};
	const uint8_t PleasePutLeftFaceEn[]={"PLS put Left Face"};

	const uint8_t PleasePutLowerFace[]={HZ_qing,HZ_xiang,HZ_xia,HZ_weiwei,HZ_weiwei,HZ_di,HZ_tou,HZ_end};
	const uint8_t PleasePutLowerFaceEn[]={"PLS put Lower Face"};
	
	const uint8_t PleasePutUpperFace[]={HZ_qing,HZ_xiang,HZ_shang,HZ_weiwei,HZ_weiwei,HZ_taitou,HZ_tou,HZ_end};
	const uint8_t PleasePutUpperFaceEn[]={"PLS put Upper Face"};

	
	if ( GUI_FaceTemplateRegisterMgr.Status == StartFaceRegister )
	{		

		GUI_RefreshSleepTime();
		
		PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_PleaseInputFace);
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(3,32,PleasePutMiddleFace,NormalDisplay);
		}
		else{
			DisEN16x8Str(3,4,PleasePutMiddleFaceEn,NormalDisplay);
		}

		GUI_FaceTemplateRegisterMgr.EnrollSuccessTimes = 0;
		GUI_FaceTemplateRegisterMgr.EnrollFailTimes=0;
		GUI_FaceTemplateRegisterMgr.DuplicateCheck = bTRUE;
		if ( FrmMgr.PowerStatus == FRM_PowerOff ) 
		{
			FaceRecognition_HardwarePowerOn();
			GUI_FaceTemplateRegisterMgr.Status = PowerOnForFrmForRegister;
			GUI_FaceTemplateRegisterMgr.TimeCnt = Def_GuiTimeDelayCnt2s; 	
		}
		else
		{
			GUI_FaceTemplateRegisterMgr.Status = FaceResetForRegister;
			GUI_FaceTemplateRegisterMgr.TimeCnt = Def_GuiTimeDelayCnt1s; 
		}

		
	}
	else if (GUI_FaceTemplateRegisterMgr.Status == FaceResetForRegister)
	{
		FaceRecognition_Reset();
		GUI_FaceTemplateRegisterMgr.Status = WaitForFaceResetFinishedForRegister;
		GUI_FaceTemplateRegisterMgr.TimeCnt = Def_GuiTimeDelayCnt4s; 
	}
	else if (GUI_FaceTemplateRegisterMgr.Status == WaitForFaceResetFinishedForRegister)
	{
		if ( FrmMgr.PostFlag_ResetResult == bTRUE)
		{
			GUI_FaceTemplateRegisterMgr.Status = InfoUserPutFace;
		}
		if (--GUI_FaceTemplateRegisterMgr.TimeCnt < 1 )
		{
			if (GUI_FaceTemplateRegisterMgr.EnrollFailTimes < 3 )
			{
				GUI_FaceTemplateRegisterMgr.EnrollFailTimes++;
				GUI_FaceTemplateRegisterMgr.Status = PowerOffForFrmForRegister;
				FaceRecognition_HardwarePowerOff();
				GUI_FaceTemplateRegisterMgr.TimeCnt = Def_GuiTimeDelayCnt01s; 
			}
			else
			{
				GUI_FaceTemplateRegisterMgr.Status = RegisterFaceTemplateFail;
				GUI_FaceTemplateRegisterMgr.ErrorType = TimeOut;
			}
		}
	}
	else if (GUI_FaceTemplateRegisterMgr.Status == PowerOffForFrmForRegister)
	{
		if (--GUI_FaceTemplateRegisterMgr.TimeCnt < 1 )
		{
			GUI_FaceTemplateRegisterMgr.Status = PowerOnForFrmForRegister;
			GUI_FaceTemplateRegisterMgr.TimeCnt = Def_GuiTimeDelayCnt1s5; 
			FaceRecognition_HardwarePowerOn();
		}
	}
	else if (GUI_FaceTemplateRegisterMgr.Status == PowerOnForFrmForRegister)
	{
		if (--GUI_FaceTemplateRegisterMgr.TimeCnt < 1 )
		{
			GUI_FaceTemplateRegisterMgr.Status = FaceResetForRegister;
		}
	}
	else if (GUI_FaceTemplateRegisterMgr.Status == InfoUserPutFace)
	{

		GUI_FaceTemplateRegisterMgr.Status = WaitForRegisterFaceResult;
		GUI_FaceTemplateRegisterMgr.TimeCnt = Def_GuiTimeDelayCnt25s;
		GUI_FaceTemplateRegisterMgr.ReportFaceStatusTimeCnt=0x0000;	
		
		if (GUI_FaceTemplateRegisterMgr.EnrollSuccessTimes == 0)
		{
			FaceDirrect = FaceDirrect_MIDDLE;	
		}
		else if (GUI_FaceTemplateRegisterMgr.EnrollSuccessTimes == 1)
		{
			FaceDirrect = FaceDirrect_RIGH;
			PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_PleaseMoveFaceToRight);
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(3,16,PleasePutRightFace,NormalDisplay);
			}
			else{
				DisEN16x8Str(3,4,PleasePutRightFaceEn,NormalDisplay);
			}	
		}
		else if (GUI_FaceTemplateRegisterMgr.EnrollSuccessTimes == 2)
		{
			FaceDirrect = FaceDirrect_LEFT;
			PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_PleaseMoveFaceToLeft);
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(3,16,PleasePutLeftFace,NormalDisplay);
			}
			else{
				DisEN16x8Str(3,4,PleasePutLeftFaceEn,NormalDisplay);
			}
		}
		else if (GUI_FaceTemplateRegisterMgr.EnrollSuccessTimes == 3)
		{
			FaceDirrect = FaceDirrect_DOWN;
			PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_PleaseMoveFaceToDown);
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(3,16,PleasePutLowerFace,NormalDisplay);
			}
			else{
				DisEN16x8Str(3,4,PleasePutLowerFaceEn,NormalDisplay);
			}
		}
		else if (GUI_FaceTemplateRegisterMgr.EnrollSuccessTimes == 4)
		{
			FaceDirrect = FaceDirrect_UP;
			PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_PleaseMoveFaceToUp);
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(3,16,PleasePutUpperFace,NormalDisplay);
			}
			else{
				DisEN16x8Str(3,4,PleasePutUpperFaceEn,NormalDisplay);
			}
		}	
		#ifdef Function_FaceRegisterMultiTimes
			FaceRecognition_RegisterTemplateMultiTimesStart(FaceDirrect);
		#elif defined Function_FaceRegisterSingleTimes
			FaceRecognition_RegisterTemplateSingleTimesStart();
		#else
			//DEFINE FAIL
		#endif
		
		DEBUG_MARK;
	}
	else if ( GUI_FaceTemplateRegisterMgr.Status == WaitForRegisterFaceResult)
	{
		GUI_RefreshSleepTime();

		if ( GUI_FaceTemplateRegisterMgr.ReportFaceStatusTimeCnt < 1000 )
		{
			GUI_FaceTemplateRegisterMgr.ReportFaceStatusTimeCnt++;
		}
		
		if (FrmMgr.PostFlag_FaceState == bTRUE)
		{
			FrmMgr.PostFlag_FaceState = bFALSE;
			
			if ( FrmMgr.FaceState == NOFACE )
			{
				if ( GUI_FaceTemplateRegisterMgr.ReportFaceStatusTimeCnt > Def_GuiTimeDelayCnt4s )
				{
					GUI_FaceTemplateRegisterMgr.ReportFaceStatusTimeCnt=0x0000;				
					PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_NoFaceDetected);
				}
			}
			else if ( FrmMgr.FaceState == TOOUP )	//浜鸿劯澶?闈犺繎鍥剧墖涓婅竟娌?
			{
				if ( GUI_FaceTemplateRegisterMgr.ReportFaceStatusTimeCnt > Def_GuiTimeDelayCnt2s )
				{
					GUI_FaceTemplateRegisterMgr.ReportFaceStatusTimeCnt=0x0000;	
					PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_FaceTooUp);
				}
			}
			else if ( FrmMgr.FaceState == TOODOWN )//浜鸿劯澶?闈犺繎鍥剧墖涓嬭竟娌?
			{
				if ( GUI_FaceTemplateRegisterMgr.ReportFaceStatusTimeCnt > Def_GuiTimeDelayCnt2s )
				{
					GUI_FaceTemplateRegisterMgr.ReportFaceStatusTimeCnt=0x0000;
					PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_FaceTooLow);
				}
			}	
			else if ( FrmMgr.FaceState == TOOLEFT )//浜鸿劯澶?闈犺繎鍥剧墖宸﹁竟娌?
			{
				if ( GUI_FaceTemplateRegisterMgr.ReportFaceStatusTimeCnt > Def_GuiTimeDelayCnt2s )
				{
					GUI_FaceTemplateRegisterMgr.ReportFaceStatusTimeCnt=0x0000;
					PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_FaceTooLeft);
				}
			}
			else if ( FrmMgr.FaceState == TOORIGHT )//浜鸿劯澶?闈犺繎鍥剧墖鍙宠竟娌?
			{
				if ( GUI_FaceTemplateRegisterMgr.ReportFaceStatusTimeCnt > Def_GuiTimeDelayCnt2s )
				{
					GUI_FaceTemplateRegisterMgr.ReportFaceStatusTimeCnt=0x0000;
					PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_FaceTooRight);
				}
			}
			else if ( FrmMgr.FaceState == TOOFAR )//浜鸿劯璺濈?诲お杩?
			{
				if ( GUI_FaceTemplateRegisterMgr.ReportFaceStatusTimeCnt > Def_GuiTimeDelayCnt2s )
				{
					GUI_FaceTemplateRegisterMgr.ReportFaceStatusTimeCnt=0x0000;
					PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_FaceTooAway);
				}
			}
			else if ( FrmMgr.FaceState == TOOCLOSE )//浜鸿劯璺濈?诲お杩?
			{
				if ( GUI_FaceTemplateRegisterMgr.ReportFaceStatusTimeCnt > Def_GuiTimeDelayCnt2s )
				{
					GUI_FaceTemplateRegisterMgr.ReportFaceStatusTimeCnt=0x0000;
					PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_FaceTooClose);
				}
			}
			
			DEBUG_MARK;

		}
		if (FrmMgr.PostFlag_RegisterResult == bTRUE)
		{
			FrmMgr.PostFlag_RegisterResult = bFALSE;

			if (FrmMgr.ErrorType == MR_SUCCESS )
			{
				#ifdef Function_FaceRegisterMultiTimes
				if (++GUI_FaceTemplateRegisterMgr.EnrollSuccessTimes < 5 )
				#elif defined Function_FaceRegisterSingleTimes
				if (++GUI_FaceTemplateRegisterMgr.EnrollSuccessTimes < 1 )
				#else
				if (1)
				#endif
				{
					GUI_FaceTemplateRegisterMgr.Status = InfoUserPutFace;
					GUI_FaceTemplateRegisterMgr.TimeCnt = Def_GuiTimeDelayCnt1s; 
				}
				else
				{
					GUI_FaceTemplateRegisterMgr.Status = RegisterFaceTemplateSuccess;
					GUI_FaceTemplateRegisterMgr.UserID = FrmMgr.UserID;
					DEBUG_MARK;
				}
			}
			else
			{
				GUI_FaceTemplateRegisterMgr.Status = RegisterFaceTemplateFail;
				if ( FrmMgr.ErrorType == MR_FAILED4_FACEENROLLED )
				{
					GUI_FaceTemplateRegisterMgr.ErrorType = RepeatFace;
				}
				else
				{
					GUI_FaceTemplateRegisterMgr.ErrorType = FaceRegisterFail;
				}
			}
		}
		
		if (--GUI_FaceTemplateRegisterMgr.TimeCnt < 1 )
		{
			GUI_FaceTemplateRegisterMgr.Status = RegisterFaceTemplateFail;
			GUI_FaceTemplateRegisterMgr.ErrorType = TimeOut;
		}
	}

}


void FaceUserIdentify(void)
{
	if ( FaceIdentifyMgr.Status == FrmIdentifyStart )
	{
		if ( FrmMgr.PowerStatus == FRM_PowerOff )
		{
			FaceRecognition_HardwarePowerOn();
		}
		FaceIdentifyMgr.Status = GetFrmStatus;
		FaceIdentifyMgr.TimeCnt = Def_GuiTimeDelayCnt1s; 
	}
	else if (FaceIdentifyMgr.Status == GetFrmStatus)
	{
		if (--FaceIdentifyMgr.TimeCnt < 1 )
		{	
			FaceRecognition_GetStatus();
			FaceIdentifyMgr.Status = WaitForGetFrmStatusAck;
			FaceIdentifyMgr.TimeCnt = Def_GuiTimeDelayCnt05s; 
		}
		if (FrmMgr.PostFlag_Ready == bTRUE )
		{
			if ( IfSystemIsInFactoryDefaultStatus()==bTRUE )
			{
				FaceIdentifyMgr.Status = SendFrmEnableDemoModeCmd;	
			}
			else if ( FaceIdentifyMgr.DemoModeIsEnabled == bTRUE )
			{
				FaceIdentifyMgr.Status = SendFrmDisableDemoModeCmd;	
			}
			else
			{
				FaceIdentifyMgr.Status = SendFaceIdentifyCmd;
			}
		}
	}
	else if (FaceIdentifyMgr.Status == WaitForGetFrmStatusAck)
	{
		if ( FrmMgr.PostFlag_Status == bTRUE)
		{
			if ( FrmMgr.ErrorType == MR_SUCCESS )
			{
				if ( IfSystemIsInFactoryDefaultStatus()==bTRUE )
				{
					FaceIdentifyMgr.Status = SendFrmEnableDemoModeCmd;	
				}
				else if ( FaceIdentifyMgr.DemoModeIsEnabled == bTRUE )
				{
					FaceIdentifyMgr.Status = SendFrmDisableDemoModeCmd;	
				}
				else
				{
					FaceIdentifyMgr.Status = SendFaceIdentifyCmd;
				}
			}
			else
			{
				FaceIdentifyMgr.Status = PowerOffForFrmHardwareReset;
				FaceIdentifyMgr.TimeCnt = Def_GuiTimeDelayCnt01s;
			}
		}
		if (--FaceIdentifyMgr.TimeCnt < 1 )
		{
				FaceIdentifyMgr.Status = PowerOffForFrmHardwareReset;
				FaceIdentifyMgr.TimeCnt = Def_GuiTimeDelayCnt01s; 
				DEBUG_MARK;
		}
	} 
	else if (FaceIdentifyMgr.Status == PowerOffForFrmHardwareReset)
	{
		FaceRecognition_HardwarePowerOff();
		if (--FaceIdentifyMgr.TimeCnt < 1 )
		{
			FaceIdentifyMgr.Status = PowerOnForFrmHardwareReset;
		}
	} 
	else if (FaceIdentifyMgr.Status == PowerOnForFrmHardwareReset)
	{
		FaceRecognition_HardwarePowerOn();
		FaceIdentifyMgr.Status = WaitForFrmReadyForIdentify;
		FaceIdentifyMgr.TimeCnt = Def_GuiTimeDelayCnt2s; 
	} 
	else if (FaceIdentifyMgr.Status == WaitForFrmReadyForIdentify)
	{
		if ( FrmMgr.PostFlag_Ready == bTRUE)
		{
			if ( IfSystemIsInFactoryDefaultStatus()==bTRUE )
			{
				FaceIdentifyMgr.Status = SendFrmEnableDemoModeCmd;	
			}
			else if ( FaceIdentifyMgr.DemoModeIsEnabled == bTRUE )
			{
				FaceIdentifyMgr.Status = SendFrmDisableDemoModeCmd;	
			}
			else
			{
				FaceIdentifyMgr.Status = SendFaceIdentifyCmd;
			}
		}
		if (--FaceIdentifyMgr.TimeCnt < 1 )
		{
			FaceIdentifyMgr.Status = FaceIdentifyFail;
		}
	}
	else if ( FaceIdentifyMgr.Status == SendFrmEnableDemoModeCmd )
	{
		FaceIdentifyMgr.DemoModeIsEnabled = bTRUE;
		FaceRecognition_EnableDemoMode();
		FaceIdentifyMgr.TimeCnt = Def_GuiTimeDelayCnt2s; 
		FaceIdentifyMgr.Status = WaitForFrmEnableDemoModeAck;
	}	
	else if ( FaceIdentifyMgr.Status == WaitForFrmEnableDemoModeAck )
	{
		if (--FaceIdentifyMgr.TimeCnt < 1)
		{
			DEBUG_MARK;
			FaceIdentifyMgr.Status = FrmIdentifyStart;
		}
		if (FrmMgr.PostFlag_EnableDemoResult == bTRUE )
		{
			FaceIdentifyMgr.Status = SendFaceIdentifyCmd;
			FaceIdentifyMgr.DemoModeIsEnabled = bTRUE;
		}
	}	
	else if ( FaceIdentifyMgr.Status == SendFrmDisableDemoModeCmd )
	{
		FaceRecognition_DisableDemoMode();
		FaceIdentifyMgr.TimeCnt = Def_GuiTimeDelayCnt2s; 
		FaceIdentifyMgr.Status = WaitForFrmDisableDemoModeAck;
	}	
	else if ( FaceIdentifyMgr.Status == WaitForFrmDisableDemoModeAck )
	{
		if (--FaceIdentifyMgr.TimeCnt < 1)
		{
			DEBUG_MARK;
			FaceIdentifyMgr.Status = FrmIdentifyStart;
		}
		if (FrmMgr.PostFlag_DisableDemoResult == bTRUE )
		{
			FaceIdentifyMgr.Status = SendFaceIdentifyCmd;
			FaceIdentifyMgr.DemoModeIsEnabled = bFALSE;
		}
	}
	else if ( FaceIdentifyMgr.Status == SendFaceIdentifyCmd )
	{
		FaceIdentifyMgr.Status = WaitForFaceIdentifyResult;
		FaceIdentifyMgr.TimeCnt = Def_GuiTimeDelayCnt10s;
		FaceIdentifyMgr.ReportFaceStatusTimeCnt = Def_GuiTimeDelayCnt3s;
		FaceRecognition_VerifyStart();
		DEBUG_MARK;
	}
	else if ( FaceIdentifyMgr.Status == WaitForFaceIdentifyResult)
	{
		if ( FaceIdentifyMgr.ReportFaceStatusTimeCnt > 0 )
		{
			FaceIdentifyMgr.ReportFaceStatusTimeCnt--;
		}

		#ifdef Function_VoiceReportFaceStatusWhileIdendify
		if (FrmMgr.PostFlag_FaceState == bTRUE)
		{
			FrmMgr.PostFlag_FaceState = bFALSE;

			if (  FaceIdentifyMgr.ReportFaceStatusTimeCnt == 0x0000)
			{
				if (( FrmMgr.FaceState == NOFACE )&&(SystemPowerMgr.SleepDelayTimerCnt > Def_GuiTimeDelayCnt2s) )
				{
					FaceIdentifyMgr.ReportFaceStatusTimeCnt = Def_GuiTimeDelayCnt3s;	
					if ( CurrentScreen != SCREEN_PickLockAlarm )
					{
						PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_NoFaceDetected);
					}
				}
				else if ( FrmMgr.FaceState == TOOUP )	//
				{
					FaceIdentifyMgr.ReportFaceStatusTimeCnt = Def_GuiTimeDelayCnt2s;	
					PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_FaceTooUp);
				}
				else if ( FrmMgr.FaceState == TOODOWN )//
				{
					FaceIdentifyMgr.ReportFaceStatusTimeCnt = Def_GuiTimeDelayCnt2s;
					PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_FaceTooLow);
				}	
				else if ( FrmMgr.FaceState == TOOLEFT )//
				{
					FaceIdentifyMgr.ReportFaceStatusTimeCnt = Def_GuiTimeDelayCnt2s;
					PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_FaceTooLeft);
				}
				else if ( FrmMgr.FaceState == TOORIGHT )//
				{
					FaceIdentifyMgr.ReportFaceStatusTimeCnt = Def_GuiTimeDelayCnt2s;
					PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_FaceTooRight);
				}
				else if ( FrmMgr.FaceState == TOOFAR )//
				{
					FaceIdentifyMgr.ReportFaceStatusTimeCnt = Def_GuiTimeDelayCnt2s;
					PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_FaceTooAway);
				}
				else if ( FrmMgr.FaceState == TOOCLOSE )//
				{
					FaceIdentifyMgr.ReportFaceStatusTimeCnt = Def_GuiTimeDelayCnt2s;
					PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_FaceTooClose);
				}
			}
			
			DEBUG_MARK;
		}
		#endif
		if (FrmMgr.PostFlag_VerifyResult == bTRUE)
		{
			if ( FrmMgr.ErrorType == MR_SUCCESS )
			{
				if ( IfSystemIsInFactoryDefaultStatus()==bTRUE )
				{
					FaceIdentifyMgr.Status = FaceIdentifySuccess_NoUser;
				}
				else
				{
					FaceIdentifyMgr.Status = FaceIdentifySuccess;
				}
			}
			else
			{
				FaceIdentifyMgr.Status = FaceIdentifyFail;
				DEBUG_MARK;
			}
		}
		if (--FaceIdentifyMgr.TimeCnt < 1)
		{
				DEBUG_MARK;
				FaceIdentifyMgr.Status = FaceIdentifyFail;
		}
	}
}


/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowRegisterUserFace(void)
{
	//uint8_t i;
	//uint16_t UserID;
	const uint8_t AddUserFaceStr[]={HZ_tian,HZ_jia,HZ_ren,HZ_lianbu,HZ_yong,HZ_hu,HZ_end};  	
	const uint8_t AddUserFaceStrEn[]={"Add Face user"};  	
	const uint8_t AddMasterFaceStr[]={HZ_tian,HZ_jia,HZ_guan,HZ_li,HZ_yuan,HZ_ren,HZ_lianbu,HZ_end};  	
	const uint8_t AddMasterFaceStrEn[]={"Add Face Admin."};  

		
	if (SystemLanguage == Chinese)
	{
		if (FaceUserRegisterMgr.FaceUserType == FaceMaster){
			DisHZ16x14Str(0,16,AddMasterFaceStr,NormalDisplay);
		}else{
			DisHZ16x14Str(0,20,AddUserFaceStr,NormalDisplay);
		}
	}
	else
	{
		if (FaceUserRegisterMgr.FaceUserType == FaceMaster){
			DisEN16x8Str(0,12,AddMasterFaceStrEn,NormalDisplay);
		}else{
			DisEN16x8Str(0,12,AddUserFaceStrEn,NormalDisplay);
		}
	}

	if ( FaceUserRegisterMgr.Status == StartFaceUserRegister )
	{	
		if (((FaceUserRegisterMgr.FaceUserType == FaceMaster)&&(CheckMemoryMgr.FaceMasterNum >= DEF_MAX_FACEMASTER))
			||((FaceUserRegisterMgr.FaceUserType == FaceUser)&&(CheckMemoryMgr.FaceUserNum >= DEF_MAX_FACEUSER))
				)
		{
			FaceUserRegisterMgr.Status = RegisterUserFail;
			FaceUserRegisterMgr.ErrorType = MemoryIsFull;
			FaceUserRegisterMgr.TimeCnt = Def_MessageBoxTimeDelay;
			PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_UsersAreFull);
		}
		else 
		{
			FaceUserRegisterMgr.Status = InputFaceUserID;
		}
	}
	else if ( FaceUserRegisterMgr.Status == InputFaceUserID )
	{
		if ( FaceUserRegisterMgr.FaceUserType == FaceMaster )
		{
			FaceUserRegisterMgr.UserID = Get_Availabe_FACEmasterID();
		}
		else if ( FaceUserRegisterMgr.FaceUserType == FaceUser )
		{
			FaceUserRegisterMgr.UserID = Get_Availabe_FACEuserID();
		}

		if (SystemLanguage == Chinese)
		{
			DisHZ16x14Str(3,20,UserIDStr,NormalDisplay);
			DisOneDigital16x8(3,84,FaceUserRegisterMgr.UserID/100,NormalDisplay);
			DisOneDigital16x8(3,92,FaceUserRegisterMgr.UserID%100/10,NormalDisplay);
			DisOneDigital16x8(3,100,FaceUserRegisterMgr.UserID%10,NormalDisplay);
		}
		else
		{
			DisEN16x8Str(2,20,UserIDStrEn,NormalDisplay);
			DisOneDigital16x8(2,84,FaceUserRegisterMgr.UserID/100,NormalDisplay);
			DisOneDigital16x8(2,92,FaceUserRegisterMgr.UserID%100/10,NormalDisplay);
			DisOneDigital16x8(2,100,FaceUserRegisterMgr.UserID%10,NormalDisplay);
		}
		
		

		if (SystemLanguage == Chinese)
		{
			DisHZ16x14Str(6,0,PressAsteriskKeyToReturnStr,NormalDisplay);
			DisHZ16x14Str(6,76,PressPoundKeyToConfirmStr,NormalDisplay);
		}
		else
		{
			DisEN16x8Str(4,0,PressPoundKeyToConfirmStrEn,NormalDisplay);
			DisEN16x8Str(6,0,PressAsteriskKeyToReturnStrEn,NormalDisplay);
		}
		
		FaceUserRegisterMgr.Status = ReportFaceUserID;
	
		VoiceReportUserIDWithUserConfirm(FaceUserRegisterMgr.UserID);
		
		FaceUserRegisterMgr.TimeCnt = Def_GuiTimeDelayCnt15s;
			
	}
		
	else if ( FaceUserRegisterMgr.Status == ReportFaceUserID )
	{
		if ( FaceUserRegisterMgr.TimeCnt-- <1 )
		{
			//FaceRecognition_FaceReset();
			FaceRecognition_HardwarePowerOff();
			GoIntoFaceMenu_Init();
		}
	}
//	else if ( FaceUserRegisterMgr.Status == WaitForConfirmAddFaceUser )
//	{
//		if ( FaceUserRegisterMgr.TimeCnt-- <1 )
//		{
//			FaceUserRegisterMgr.Status = RegisterUserFail;	
//			FaceUserRegisterMgr.ErrorType = TimeOut;
//			GUI_Flag_RefreshLCD = bTRUE;
//		}
//	}
	else if (FaceUserRegisterMgr.Status == AddUserToFRM)
	{
		RegisterFace();
		
		if (GUI_FaceTemplateRegisterMgr.Status == RegisterFaceTemplateSuccess)
		{
			
			FaceUserRegisterMgr.Status = RegisterUserSuccess;
			FaceUserRegisterMgr.TimeCnt = Def_MessageBoxTimeDelay;
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_OperationSuccess);
			GUI_Flag_RefreshLCD = bTRUE;
			
			ComPort_SetPost_Info(DEF_WifiInfo_AddUser,FACEUSER,FaceUserRegisterMgr.UserID);

			FaceRecognition_UpdateUserIdList();
			
			if ( FaceUserRegisterMgr.FaceUserType == FaceMaster )
			{
				CheckMemoryMgr.FaceMasterNum+=1;
				FaceUserMemoryMgr[FaceUserRegisterMgr.UserID-1].FaceTemplateID = GUI_FaceTemplateRegisterMgr.UserID;
				FaceUserMemoryMgr[FaceUserRegisterMgr.UserID-1].RegisterStatus = Registered;
				FaceUserMemoryMgr[FaceUserRegisterMgr.UserID-1].UserPriority = Master;
			}
			else
			{
				CheckMemoryMgr.FaceUserNum+=1;
				FaceUserMemoryMgr[FaceUserRegisterMgr.UserID-1].FaceTemplateID = GUI_FaceTemplateRegisterMgr.UserID;
				FaceUserMemoryMgr[FaceUserRegisterMgr.UserID-1].RegisterStatus = Registered;
				FaceUserMemoryMgr[FaceUserRegisterMgr.UserID-1].UserPriority = User;
			}

			WriteFaceUserMemoryToEEPROM();


			GUI_SetFPM_LED(DEF_FpmLedMode_Breath,DEF_FpmLedColor_Green,DEF_FpmLedColor_Green,255);

		}
		else if (GUI_FaceTemplateRegisterMgr.Status == RegisterFaceTemplateFail )
		{
			FaceRecognition_PowerDown();
			
			FaceUserRegisterMgr.Status = RegisterUserFail;
			FaceUserRegisterMgr.ErrorType = GUI_FaceTemplateRegisterMgr.ErrorType;
			FaceUserRegisterMgr.TimeCnt = Def_MessageBoxTimeDelay;
			if ( FaceUserRegisterMgr.ErrorType == RepeatFace ){
				PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_RepeatedFace);
			}
			else{
				PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_OperationFail);
			}
			GUI_Flag_RefreshLCD = bTRUE;

			GUI_SetFPM_LED(DEF_FpmLedMode_Breath,DEF_FpmLedColor_Red,DEF_FpmLedColor_Red,255);
		}
	}

	else if ( FaceUserRegisterMgr.Status == RegisterUserSuccess )
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(4,36,OperationSuccessStr,NormalDisplay);
		}
		else{
			DisEN16x8Str(4,36,OperationSuccessStrEn,NormalDisplay);
		}

		if ( FaceUserRegisterMgr.TimeCnt > 0 )
		{
			FaceUserRegisterMgr.TimeCnt--;
			if ( FaceUserRegisterMgr.TimeCnt == Def_GuiTimeDelayCnt1s )
			{
				FaceRecognition_PowerDown();
			}
			else if ( FaceUserRegisterMgr.TimeCnt == Def_GuiTimeDelayCnt01s )
			{
				FaceRecognition_HardwarePowerOff();
			}
		}
		else	
		{		
			FaceUserRegisterMgr.Status = StartFaceUserRegister;
			GUI_Flag_RefreshLCD = bTRUE;
			GUI_RefreshSleepTime();
			GUI_CreatAndSaveLog(AddFaceUser);
			GUI_SetFPM_LED(DEF_FpmLedMode_Breath,DEF_FpmLedColor_Blue,DEF_FpmLedColor_Blue,255);
		}
		DEBUG_MARK;
	}
	else if ( FaceUserRegisterMgr.Status == RegisterUserFail )
	{		
		if ( FaceUserRegisterMgr.ErrorType == UserIDisRegistered )
		{
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(4,10,IDisRegisteredStr,NormalDisplay);
			}
			else{
				DisEN16x8Str(4,10,IDisRegisteredStrEn,NormalDisplay);
			}
		}
		else if ( FaceUserRegisterMgr.ErrorType == RepeatFace )
		{
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(4,36,FaceRepeated,NormalDisplay);
			}
			else{
				DisEN16x8Str(4,12,FaceRepeatedEn,NormalDisplay);
			}
		}
		else if ( FaceUserRegisterMgr.ErrorType == MemoryIsFull )
		{
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(4,32,UserIsFullStr,NormalDisplay);
			}
			else{
				DisEN16x8Str(4,4,UserIsFullStrEn,NormalDisplay);
			}
		}
		else
		{
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(4,36,OperationFailStr,NormalDisplay);
			}
			else{
				DisEN16x8Str(4,48,OperationFailStrEn,NormalDisplay);
			}
		}
		if (FaceUserRegisterMgr.TimeCnt-- < 1 )		
		{
			FaceRecognition_HardwarePowerOff();
			GoIntoFaceMenu_Init();
			GUI_SetFPM_LED(DEF_FpmLedMode_Breath,DEF_FpmLedColor_Blue,DEF_FpmLedColor_Blue,255);
		}
		DEBUG_MARK;
	}
}


/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowDeleteUserFace(void)
{
	//uint8_t i;
	const uint8_t DelUserFaceStr[]={HZ_shan,HZ_chufa,HZ_ren,HZ_lianbu,HZ_yong,HZ_hu,HZ_end};  	
	const uint8_t DelUserFaceStrEn[]={"Del Face user"};  
 
	//const uint8_t DelMasterFaceStr[]={HZ_shan,HZ_chufa,HZ_guan,HZ_li,HZ_yuan,HZ_ren,HZ_lianbu,HZ_end};  	
	//const uint8_t DelMasterFaceStrEn[]={"Del Face Admin."};  

	if (SystemLanguage == Chinese)
	{
		DisHZ16x14Str(0,20,DelUserFaceStr,NormalDisplay);
	}
	else
	{
		DisEN16x8Str(0,12,DelUserFaceStrEn,NormalDisplay);
	}

	if ( FaceUserDeleteMgr.Status == StartFaceUserDelete )
	{
		FaceUserDeleteMgr.Status = InputFaceUserID;
		DataInputMgr.Status = InputIdle;
		PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_PleaseInputID);
	}
	
	else if ( FaceUserDeleteMgr.Status == InputFaceUserID )
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(3,32,InputUserIDStr,NormalDisplay);
		}
		else{
			DisEN16x8Str(3,32,InputUserIDStrEn,NormalDisplay);
		}

		GUI_DataInputCreat(5,56,3,0x0000);

		if (DataInputMgr.Status == InputEnd)
		{
			FaceUserDeleteMgr.UserID = DataInputMgr.Value;			
			FaceUserDeleteMgr.Status = ReportFaceUserID;
			DataInputMgr.Status = InputIdle;
			VoiceReportUserID(FaceUserDeleteMgr.UserID);
			FaceUserDeleteMgr.TimeCnt = Def_GuiTimeDelayCnt4s;
		}			
		else if  (DataInputMgr.Status == InputExit)
		{
			FaceUserDeleteMgr.Status = DeleteFaceUserFail;
			FaceUserDeleteMgr.ErrorType = QUIT;
			FaceUserDeleteMgr.TimeCnt = 0;
		}
	}

	else if ( FaceUserDeleteMgr.Status == ReportFaceUserID )
	{
		if (--FaceUserDeleteMgr.TimeCnt < 1 )
		{
			FaceUserDeleteMgr.Status = CheckIfFaceUserIDisBeUsed;
			GUI_Flag_RefreshLCD = bTRUE;
		}
	}
	else if (FaceUserDeleteMgr.Status == CheckIfFaceUserIDisBeUsed)
	{
		if ( CheckIfFaceUserIsRegistered(FaceUserDeleteMgr.UserID) == bTRUE )
		{
			if ( (ManagerIdentifyMgr.MasterType == FACEUSER )
				&&(ManagerIdentifyMgr.FaceUserID == FaceUserDeleteMgr.UserID)
				)
				
			{
				FaceUserDeleteMgr.Status = DeleteFaceUserFail;
				//PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_DeleteFail);
				PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_AdminIsLoginCannotbeDelete);
				FaceUserDeleteMgr.ErrorType = ManagerHasLogin;
				FaceUserDeleteMgr.TimeCnt = Def_GuiTimeDelayCnt3s;
				GUI_Flag_RefreshLCD = bTRUE;
			}
			else
			{
				if (SystemLanguage == Chinese)
				{
					DisHZ16x14Str(4,40,PleaseWaitStr,NormalDisplay);
				}
				else
				{
					DisEN16x8Str(4,12,PleaseWaitStrEn,NormalDisplay);
				}
				PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_PleaseWait);

				if ( FrmMgr.PowerStatus == FRM_PowerOff ) 
				{
					FaceUserDeleteMgr.Status = PowerOnForFrmForDeleteTemplate;
					FaceRecognition_HardwarePowerOn();
					FaceUserDeleteMgr.TimeCnt = Def_GuiTimeDelayCnt2s; 
				} 
				else
				{
					FaceUserDeleteMgr.Status = FaceResetForDeleteTemplate;	
					DataInputMgr.Status = InputIdle;	
					FaceUserDeleteMgr.TimeCnt = Def_GuiTimeDelayCnt05s;	
				}
			}
		}
		else
		{
			FaceUserDeleteMgr.Status = DeleteFaceUserFail;
			FaceUserDeleteMgr.TimeCnt = Def_MessageBoxTimeDelay;
			FaceUserDeleteMgr.ErrorType = UserIDisNotRegistered;
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_UserIdIsNotExist);
			GUI_Flag_RefreshLCD = bTRUE;
		}
	}
	else if ( FaceUserDeleteMgr.Status == FaceResetForDeleteTemplate)
	{
		FaceRecognition_Reset();

		FaceUserDeleteMgr.Status = WaitForFaceResetFinishedForDeleteTemplate;
		FaceUserDeleteMgr.TimeCnt = Def_GuiTimeDelayCnt4s; 
	}
	else if (FaceUserDeleteMgr.Status == WaitForFaceResetFinishedForDeleteTemplate)
	{
		if ( FrmMgr.PostFlag_ResetResult == bTRUE)
		{
			FaceUserDeleteMgr.Status = DeleteUserFromFRM;
		}
		if (--FaceUserDeleteMgr.TimeCnt < 1 )
		{
			FaceUserDeleteMgr.Status = PowerOffForFrmForDeleteTemplate;
			FaceRecognition_HardwarePowerOff();
			FaceUserDeleteMgr.TimeCnt = Def_GuiTimeDelayCnt05s; 
		}
	}

	else if ( FaceUserDeleteMgr.Status == PowerOffForFrmForDeleteTemplate)
	{
		if (--FaceUserDeleteMgr.TimeCnt < 1 )
		{
			FaceUserDeleteMgr.Status = PowerOnForFrmForDeleteTemplate;
			FaceRecognition_HardwarePowerOn();
			FaceUserDeleteMgr.TimeCnt = Def_GuiTimeDelayCnt2s; 
		} 
	}
	else if ( FaceUserDeleteMgr.Status == PowerOnForFrmForDeleteTemplate)
	{
		if ( FrmMgr.PostFlag_Ready == bTRUE)
		{
			FaceUserDeleteMgr.Status = DeleteUserFromFRM;
		} 
		if (--FaceUserDeleteMgr.TimeCnt < 1 )
		{
			FaceUserDeleteMgr.Status = DeleteFaceUserFail;
			FaceUserDeleteMgr.ErrorType = TimeOut;
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_DeleteFail);
		} 
	}
	else if ( FaceUserDeleteMgr.Status == DeleteUserFromFRM)
	{
		FaceRecognition_DeleteTemplateStart(FaceUserMemoryMgr[FaceUserDeleteMgr.UserID-1].FaceTemplateID);
		FaceUserDeleteMgr.Status = WaitForDeleteUserACKfromFRM;
		FaceUserDeleteMgr.TimeCnt = Def_GuiTimeDelayCnt3s;
		FrmAckMgr.Status = WaitACK;
		FpmAckMgr.TimeOutCnt = DEF_FpmAckTimeoutTime;
	}
	else if (FaceUserDeleteMgr.Status == WaitForDeleteUserACKfromFRM)
	{
		if (FrmMgr.PostFlag_FaceState == bTRUE)
		{
			FrmMgr.PostFlag_FaceState = bFALSE;
			DEBUG_MARK;
		}
		
		if (FrmMgr.PostFlag_DeleteTemplateResult == bTRUE)
		{
			FrmMgr.PostFlag_DeleteTemplateResult = bFALSE;
			FaceUserDeleteMgr.TimeCnt = Def_MessageBoxTimeDelay;
			
			if (FrmMgr.ErrorType == MR_SUCCESS )
			{
				FaceUserDeleteMgr.Status = DeleteFaceUserSuccess;

				ComPort_SetPost_Info(DEF_WifiInfo_DeleteUser,FACEUSER,FaceUserDeleteMgr.UserID);

				if ( FaceUserDeleteMgr.UserID <= DEF_MAX_FACEMASTER )
				{
					if ( CheckMemoryMgr.FaceMasterNum > 0 )
					{
						CheckMemoryMgr.FaceMasterNum-=1;
						FaceUserMemoryMgr[FaceUserDeleteMgr.UserID-1].FaceTemplateID = 0xFFFF;
						FaceUserMemoryMgr[FaceUserDeleteMgr.UserID-1].RegisterStatus = UnRegistered;
						FaceUserMemoryMgr[FaceUserDeleteMgr.UserID-1].UserPriority = Undefined;
					}
				}
				else  
				{
					if ( CheckMemoryMgr.FaceUserNum > 0 )
					{
						CheckMemoryMgr.FaceUserNum-=1;
						FaceUserMemoryMgr[FaceUserDeleteMgr.UserID-1].FaceTemplateID = 0xFFFF;
						FaceUserMemoryMgr[FaceUserDeleteMgr.UserID-1].RegisterStatus = UnRegistered;
						FaceUserMemoryMgr[FaceUserDeleteMgr.UserID-1].UserPriority = Undefined;
					}
				}
				PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_DeleteSuccess);
				WriteFaceUserMemoryToEEPROM();
			}
			else
			{
				FaceUserDeleteMgr.Status = DeleteFaceUserFail;
				PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_DeleteFail);
			}
		}
		
		if (--FaceUserDeleteMgr.TimeCnt < 1 )
		{
			FaceUserDeleteMgr.Status = DeleteFaceUserFail;
			FrmAckMgr.ErrorCode = Error_TimeOut;
			GUI_Flag_RefreshLCD = bTRUE;
			FaceUserDeleteMgr.TimeCnt = Def_MessageBoxTimeDelay;
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_DeleteFail);
		}

	}
	else if ( FaceUserDeleteMgr.Status == DeleteFaceUserSuccess )
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(4,36,OperationSuccessStr,NormalDisplay);
		}
		else{
			DisEN16x8Str(4,36,OperationSuccessStrEn,NormalDisplay);
		}
		if (--FaceUserDeleteMgr.TimeCnt < 1 )		
		{
			FaceUserDeleteMgr.Status = StartFaceUserDelete;
			GUI_RefreshSleepTime();
			GUI_Flag_RefreshLCD = bTRUE;
			UnlockModeAutoCalculate();
			GUI_CreatAndSaveLog(DeleteFaceUser);
		}
		DEBUG_MARK;
	}
	else if ( FaceUserDeleteMgr.Status == DeleteFaceUserFail )
	{
		if ( FaceUserDeleteMgr.ErrorType == UserIDisNotRegistered )
		{
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(4,20,IDisNotRegisteredStr,NormalDisplay);
			}
			else{
				DisEN16x8Str(4,12,IDisNotRegisteredStrEn,NormalDisplay);
			}
		}
		else if ( FaceUserDeleteMgr.ErrorType == ManagerHasLogin )
		{
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(3,15,CanNotDeleteLastOneMasterStr1,NormalDisplay);
				DisHZ16x14Str(5,32,CanNotDeleteLastOneMasterStr2,NormalDisplay);
			}
			else
			{
				DisEN16x8Str(3,4,CanNotDeleteLastOneMasterStr1En,NormalDisplay);
				DisEN16x8Str(5,16,CanNotDeleteLastOneMasterStr2En,NormalDisplay);
			}
		}
		else
		{
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(4,36,OperationFailStr,NormalDisplay);
			}
			else{
				DisEN16x8Str(4,48,OperationFailStrEn,NormalDisplay);
			}
		}
		if (FaceUserDeleteMgr.TimeCnt-- < 1 )		
		{
			GoIntoFaceMenu_Init();
		}
		DEBUG_MARK;
	}
	
}

#if 0
void DeleteAllFaceTemplate(void)
{
	if ( DeleteAllFaceTemplateMgr.Status == StartAllFaceTemplateDelete)
	{
		if ( FrmMgr.PowerStatus == FRM_PowerOff )
		{
			FaceRecognition_HardwarePowerOn();
			DeleteAllFaceTemplateMgr.Status = PowerOnFrmForDeleteAllTemplate;
			DeleteAllFaceTemplateMgr.TimeCnt  = Def_GuiTimeDelayCnt2s;
		}
		else
		{
			DeleteAllFaceTemplateMgr.Status = ResetFrmForDeleteAllFaceTemplate;
			DeleteAllFaceTemplateMgr.TimeCnt  = Def_GuiTimeDelayCnt05s;	
		}
	}
	else if (DeleteAllFaceTemplateMgr.Status == ResetFrmForDeleteAllFaceTemplate)
	{
		if ( DeleteAllFaceTemplateMgr.TimeCnt-- < 1 )
		{
			FaceRecognition_Reset();
			DeleteAllFaceTemplateMgr.Status = WaitForFrmReadyForDeleteAllFaceTemplate;
			DeleteAllFaceTemplateMgr.TimeCnt = Def_GuiTimeDelayCnt4s; 
		}
	}
	else if (DeleteAllFaceTemplateMgr.Status == WaitForFrmReadyForDeleteAllFaceTemplate)
	{
		if ( FrmMgr.PostFlag_ResetResult == bTRUE)
		{
			DeleteAllFaceTemplateMgr.Status = SendDeleteAllFaceTemplateCmdToFRM;
		}
		if ( DeleteAllFaceTemplateMgr.TimeCnt-- < 1 )
		{
			DeleteAllFaceTemplateMgr.Status = PowerOffFrmForDeleteAllTemplate;
			FaceRecognition_HardwarePowerOff();
			DeleteAllFaceTemplateMgr.TimeCnt  = Def_GuiTimeDelayCnt05s;
		}
	}
	else if (DeleteAllFaceTemplateMgr.Status == PowerOffFrmForDeleteAllTemplate)
	{
		if ( DeleteAllFaceTemplateMgr.TimeCnt-- < 1 )
		{
			DeleteAllFaceTemplateMgr.Status = PowerOnFrmForDeleteAllTemplate;
			FaceRecognition_HardwarePowerOn();
			DeleteAllFaceTemplateMgr.TimeCnt  = Def_GuiTimeDelayCnt2s;
		}
	}
	else if (DeleteAllFaceTemplateMgr.Status == PowerOnFrmForDeleteAllTemplate)
	{
		if ( FrmMgr.PostFlag_Ready == bTRUE )
		{
			DeleteAllFaceTemplateMgr.Status = SendDeleteAllFaceTemplateCmdToFRM;
		}
		if ( DeleteAllFaceTemplateMgr.TimeCnt-- < 1 )
		{
			DeleteAllFaceTemplateMgr.Status = DeleteAllFaceTemplateFail;
		}
	}
	
    else if ( DeleteAllFaceTemplateMgr.Status == SendDeleteAllFaceTemplateCmdToFRM )
	{
		FaceRecognition_DeleteAllTemplateStart();
		DeleteAllFaceTemplateMgr.Status = WaitForDeleteAllFaceTemplateCmdACKfromFRM;
		DeleteAllFaceTemplateMgr.TimeCnt = Def_GuiTimeDelayCnt4s;
	}
	
	else if ( DeleteAllFaceTemplateMgr.Status == WaitForDeleteAllFaceTemplateCmdACKfromFRM )
	{
		if (FrmMgr.PostFlag_DeleteAllResult == bTRUE)
		{
			FrmMgr.PostFlag_DeleteAllResult = bFALSE;

			if (FrmMgr.ErrorType == MR_SUCCESS )
			{
				DeleteAllFaceTemplateMgr.Status = DeleteAllFaceTemplateSuccess;
				DeleteAllFaceUserfromMemory();
			}
			else
			{
				DeleteAllFaceTemplateMgr.Status = DeleteAllFaceTemplateFail;
			}
		}
		
		if ( --DeleteAllFaceTemplateMgr.TimeCnt < 1 )
		{
			DeleteAllFaceTemplateMgr.Status = DeleteAllFaceTemplateFail;
			DEBUG_MARK;
		}
	}

}
#endif
void DeleteAllFaceTemplate(void)
{
	if ( DeleteAllFaceTemplateMgr.Status == StartAllFaceTemplateDelete)
	{
		DeleteAllFaceTemplateMgr.Status = PowerOffFrmForDeleteAllTemplate;
		FaceRecognition_HardwarePowerOff();
		DeleteAllFaceTemplateMgr.TimeCnt  = Def_GuiTimeDelayCnt05s;
	}
	else if (DeleteAllFaceTemplateMgr.Status == PowerOffFrmForDeleteAllTemplate)
	{
		if ( DeleteAllFaceTemplateMgr.TimeCnt-- < 1 )
		{
			DeleteAllFaceTemplateMgr.Status = PowerOnFrmForDeleteAllTemplate;
			FaceRecognition_HardwarePowerOn();
			DeleteAllFaceTemplateMgr.TimeCnt  = Def_GuiTimeDelayCnt2s;
		}
	}
	else if (DeleteAllFaceTemplateMgr.Status == PowerOnFrmForDeleteAllTemplate)
	{
		if ( FrmMgr.PostFlag_Ready == bTRUE )
		{
			DeleteAllFaceTemplateMgr.Status = SendDeleteAllFaceTemplateCmdToFRM;
		}
		if ( DeleteAllFaceTemplateMgr.TimeCnt-- < 1 )
		{
			DeleteAllFaceTemplateMgr.Status = SendDeleteAllFaceTemplateCmdToFRM;
			DEBUG_MARK;
		}
	}
	
    else if ( DeleteAllFaceTemplateMgr.Status == SendDeleteAllFaceTemplateCmdToFRM )
	{
		FaceRecognition_DeleteAllTemplateStart();
		DeleteAllFaceTemplateMgr.Status = WaitForDeleteAllFaceTemplateCmdACKfromFRM;
		DeleteAllFaceTemplateMgr.TimeCnt = Def_GuiTimeDelayCnt4s;
	}
	
	else if ( DeleteAllFaceTemplateMgr.Status == WaitForDeleteAllFaceTemplateCmdACKfromFRM )
	{
		if (FrmMgr.PostFlag_DeleteAllResult == bTRUE)
		{
			FrmMgr.PostFlag_DeleteAllResult = bFALSE;

			if (FrmMgr.ErrorType == MR_SUCCESS )
			{
				DeleteAllFaceTemplateMgr.Status = DeleteAllFaceTemplateSuccess;
				DeleteAllFaceUserfromMemory();
			}
			else
			{
				DeleteAllFaceTemplateMgr.Status = DeleteAllFaceTemplateFail;
				DEBUG_MARK;
			}
		}
		
		if ( DeleteAllFaceTemplateMgr.TimeCnt-- < 1 )
		{
			DeleteAllFaceTemplateMgr.Status = DeleteAllFaceTemplateFail;
			DEBUG_MARK;
		}
	}

}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowDeleteAllUserFace(void)
{
	uint16_t i;
	
	
	if ( AllUserFaceDeleteMgr.Status == StartAllUserFaceDelete )
		{
			AllUserFaceDeleteMgr.Status = WaitForUserConfirmDeleteAllFace;
			AllUserFaceDeleteMgr.Selection = NO;
			PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_ConfirmToDelete);
		}
	else if ( AllUserFaceDeleteMgr.Status == WaitForUserConfirmDeleteAllFace )
		{
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(1,40,ConfirmDeleteStr,NormalDisplay);	
			}
			else{
				DisEN16x8Str(1,0,ConfirmDeleteStrEn,NormalDisplay);
			}
				
			if (AllUserFaceDeleteMgr.Selection == YES)
				{
					if (SystemLanguage == Chinese){
						DisHZ16x14Str(4,48,Yes,InverseDisplay);
						DisHZ16x14Str(6,48,No,NormalDisplay);
					}
					else
					{
						DisEN16x8Str(4,48,YesEn,InverseDisplay);
						DisEN16x8Str(6,48,NoEn,NormalDisplay);						
					}
				}
			else
				{
					if (SystemLanguage == Chinese){
						DisHZ16x14Str(4,48,Yes,NormalDisplay);
						DisHZ16x14Str(6,48,No,InverseDisplay);
					}
					else
					{
						DisEN16x8Str(4,48,YesEn,NormalDisplay);	
						DisEN16x8Str(6,48,NoEn,InverseDisplay);					
					}
				}
		}
	else if (AllUserFaceDeleteMgr.Status == StartFrmToDeleteAllFaceTemplate)
	{
		DeleteAllFaceTemplateMgr.Status = StartAllFaceTemplateDelete;
		AllUserFaceDeleteMgr.Status = WaitForFrmDeleteAllTemplateFinished;
	}
	
	else if (AllUserFaceDeleteMgr.Status == WaitForFrmDeleteAllTemplateFinished)
	{
		DeleteAllFaceTemplate();
		if ( DeleteAllFaceTemplateMgr.Status == DeleteAllFaceTemplateSuccess )
		{
			AllUserFaceDeleteMgr.Status = DeleteAllFaceUserSuccess;
		}
		else if ( DeleteAllFaceTemplateMgr.Status == DeleteAllFaceTemplateFail )
		{
			AllUserFaceDeleteMgr.Status = DeleteAllFaceUserFail;
			GUI_Flag_RefreshLCD = bTRUE;
			AllUserFaceDeleteMgr.TimeCnt = Def_MessageBoxTimeDelay;
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_DeleteFail);
		}
	}
	else if ( AllUserFaceDeleteMgr.Status == DeleteAllFaceUserSuccess )
		{
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(4,36,OperationSuccessStr,NormalDisplay);
			}
			else{
				DisEN16x8Str(4,36,OperationSuccessStrEn,NormalDisplay);
			}
			if (--AllUserFaceDeleteMgr.TimeCnt < 1 )
			{
				GoIntoFaceMenu_Init();
				GUI_CreatAndSaveLog(DeleteAllFaceUser);
			}
		}
	else if ( AllUserFaceDeleteMgr.Status == DeleteAllFaceUserFail )
		{
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(4,36,OperationFailStr,NormalDisplay);
			}
			else{
				DisEN16x8Str(4,48,OperationFailStrEn,NormalDisplay);
			}
			if (--AllUserFaceDeleteMgr.TimeCnt < 1 )
			{
				GoIntoFaceMenu_Init();
			}
		}
	else if ( AllUserFaceDeleteMgr.Status == EXIT )
		{
				GoIntoFaceMenu_Init();
		}
	
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void RegisterFp(uint16_t UserID)
{
	uint8_t i;
		
	const uint8_t PleasePutFinger[]={HZ_qing,HZ_fan,HZ_ru,HZ_shou,HZ_zhi,HZ_end};
	const uint8_t PleasePutFingerEn[]={"PLS put finger"};	
	const uint8_t PleasePutFingerSecondTimes[]={HZ_di,HZ_er,HZ_ci,HZ_fan,HZ_ru,HZ_shou,HZ_zhi,HZ_end};
	const uint8_t PleasePutFingerAgainStr1En[]={"Put finger"};
	const uint8_t PleasePutFingerAgainStr2En[]={"again"};
	const uint8_t PleasePutFingerThirdTimes[]={HZ_di,HZ_san,HZ_ci,HZ_fan,HZ_ru,HZ_shou,HZ_zhi,HZ_end};
	const uint8_t PleasePutFingerFourthTimes[]={HZ_di,HZ_sige,HZ_ci,HZ_fan,HZ_ru,HZ_shou,HZ_zhi,HZ_end};
	const uint8_t PleasePutFingerAgain[]={HZ_qing,HZ_zai,HZ_ci,HZ_fan,HZ_ru,HZ_shou,HZ_zhi,HZ_end};
	

	FpRegisterMgr.UserID = UserID;
	
	if ( FpRegisterMgr.Status == FPMcmdStart )
	{		
		if  ( (CheckMemoryMgr.FpUserNum+CheckMemoryMgr.FpMasterNum+CheckMemoryMgr.StressFpUserNum) == 0x00 )
		{
			FpRegisterMgr.Status = SendGetSerialNumberCmd;
			DEBUG_MARK;
		}
		else
		{
			FpRegisterMgr.Status = SendGetImageCmd;
			FpRegisterMgr.TimeCnt = Def_WaitUserPutFingerTimeDelay; 	
			//FpRegisterMgr.EnrollSuccessTimes = 1;
			FpRegisterMgr.EnrollFailTimes=0;
			#ifdef Function_DisableFpmDuplicateCheck
				FpRegisterMgr.DuplicateCheck = bFALSE;
			#else
				FpRegisterMgr.DuplicateCheck = bTRUE;
			#endif
		}
		
		FpRegisterMgr.EnrollSuccessTimes = 1;
		
		PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_PleasePutFinger);
		
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(4,28,PleasePutFinger,NormalDisplay);
		}
		else{
			DisEN16x8Str(4,4,PleasePutFingerEn,NormalDisplay);
		}	
		DisOneDigital16x8(6,50,FpRegisterMgr.EnrollSuccessTimes,NormalDisplay);
		DisEN16x8Str(6,60,"/",NormalDisplay);
		DisOneDigital16x8(6,70,DEF_FPM_EnrollTimes,NormalDisplay);
	}

	else if ( FpRegisterMgr.Status == SendGetSerialNumberCmd )
	{
		
		FpRegisterMgr.Status = WaitForGetSerialNumberCmdACK;
		FpmAckMgr.Status = WaitACK;
		FpmAckMgr.TimeOutCnt = DEF_FpmAckTimeoutTime;
		FPM_SendGetChipSerialNumberCmd();
		FpRegisterMgr.TimeCnt = Def_FPMcmdTimeOutDelay;
		DEBUG_MARK;
	}
	else if ( FpRegisterMgr.Status == WaitForGetSerialNumberCmdACK)
	{
		if (FpmAckMgr.Status == GotACK)
		{
			if ( FpmAckMgr.ErrorCode == Error_NONE)
			{
				for (i=0;i<32;i++)
				{
					FPMserialNumberManager.SN[i] = FpmAckMgr.Buff[10+i];
				}
				EEPROM_WriteSequential(FPMserialNumberStartAddr,FPMserialNumberManager.SN,32);
				
				FpRegisterMgr.Status = SendGetImageCmd;
				FpRegisterMgr.TimeCnt = Def_WaitUserPutFingerTimeDelay; 	
				FpRegisterMgr.EnrollSuccessTimes = 1;
				FpRegisterMgr.EnrollFailTimes=0;
				#ifdef Function_DisableFpmDuplicateCheck
					FpRegisterMgr.DuplicateCheck = bFALSE;
				#else
					FpRegisterMgr.DuplicateCheck = bTRUE;
				#endif
			}
		}
		else 
		{
			if (--FpRegisterMgr.TimeCnt < 1)
			{
				FpRegisterMgr.Status = fail;
				FpRegisterMgr.ErrorType = FpRegisterFail;
			}
		}
	}
	else if (FpRegisterMgr.Status == SendGetImageCmd)
	{
		
		FpRegisterMgr.Status = WaitForGetImageCmdACK;
		FpmAckMgr.TimeCnt = Def_GuiTimeDelayCnt1s;
		FpmAckMgr.Status = WaitACK;
		FpmAckMgr.TimeOutCnt = DEF_FpmAckTimeoutTime;
		FPM_SendGetEnrollImageCmd();
		DEBUG_MARK;
	}
	else if ( FpRegisterMgr.Status == WaitForGetImageCmdACK)
	{
		if (FpmAckMgr.Status == GotACK)
		{
			DEBUG_MARK;
			if ( FpmAckMgr.ErrorCode == Error_NONE)
			{
				FpRegisterMgr.Status = SendGenCharCmd;
				GUI_RefreshSleepTime();
			}
			else
			{
				if ( (FpmAckMgr.ErrorCode == Error_GetImage)
					||(FpmAckMgr.ErrorCode == Error_NoFinger)
					||(FpmAckMgr.ErrorCode == Error_BadImage)
					)
				{
					FpRegisterMgr.Status = SendGetImageCmd;
				}
				else
				{
					FpRegisterMgr.Status = fail;
					FpRegisterMgr.ErrorType = FpRegisterFail;
					DEBUG_MARK;
				}
			}
			DEBUG_MARK;
		}
		else 
		{
			if (--FpRegisterMgr.TimeCnt < 1 )
			{
				FpRegisterMgr.Status = fail;
				FpRegisterMgr.ErrorType = TimeOut;
			}
			/*
			if (--FpmAckMgr.TimeCnt < 1)
			{
				FpRegisterMgr.Status = SendGetImageCmd;
				DEBUG_MARK;
			}
			*/
		}

	}
	else if ( FpRegisterMgr.Status == SendGenCharCmd )
	{
		
		FpRegisterMgr.Status = WaitForGenCharCmdACK;
		FpmAckMgr.Status = WaitACK;
		FpmAckMgr.TimeOutCnt = DEF_FpmAckTimeoutTime;
		FPM_SendGenCharCmd(FpRegisterMgr.EnrollSuccessTimes);		//BuffID start from 0x01
		FpRegisterMgr.TimeCnt = Def_FPMcmdTimeOutDelay; 	
		DEBUG_MARK;
	}

	else if ( FpRegisterMgr.Status == WaitForGenCharCmdACK)
	{
		if (FpmAckMgr.Status == GotACK)
		{
			if ( FpmAckMgr.ErrorCode == Error_NONE)
			{	
				if ((FpRegisterMgr.EnrollSuccessTimes == 1 )&&(FpRegisterMgr.DuplicateCheck == bTRUE ))
				{
					FpRegisterMgr.EnrollSuccessTimes+=1;
					FpRegisterMgr.Status = SendSearchCmd;
					FpRegisterMgr.DuplicateCheck = bFALSE;
					FpRegisterMgr.TimeCnt =  Def_FPMcmdTimeOutDelay; 
				}
				else if (FpRegisterMgr.EnrollSuccessTimes < DEF_FPM_EnrollTimes )
				{
					FpRegisterMgr.Status = SendDetectFingerRemoveCmd;
					FpRegisterMgr.EnrollSuccessTimes+=1;
					FpRegisterMgr.TimeCnt =  Def_WaitUserRemoveFingerTimeDelay; 
					PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_PleasePutFingerAgain);
				}
				else 
				{
					FpRegisterMgr.Status = SendRegModelCmd;
					FpRegisterMgr.TimeCnt =  Def_FPMcmdTimeOutDelay; 
				}
					
				GUI_Flag_RefreshLCD = bTRUE;
				GUI_RefreshSleepTime();
			}
			else if ((FpmAckMgr.ErrorCode == Error_GenChar)
								||(FpmAckMgr.ErrorCode == Error_GetImage)	//???????????
							)
			{
				if ( FpRegisterMgr.EnrollFailTimes < DEF_FPM_EnrollFailTimes_Limited)
				{
					FpRegisterMgr.Status = SendDetectFingerRemoveCmd;
					FpRegisterMgr.TimeCnt = Def_WaitUserRemoveFingerTimeDelay;	
					GUI_Flag_RefreshLCD = bTRUE;
					GUI_RefreshSleepTime();
					PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_PleasePutFingerAgain);
				}
				else
				{
					FpRegisterMgr.Status = fail;
					FpRegisterMgr.ErrorType = FpRegisterFail;
				}
				FpRegisterMgr.EnrollFailTimes+=1;
			}
			else
			{
				FpRegisterMgr.Status = fail;
				FpRegisterMgr.ErrorType = FpRegisterFail;
			}
			DEBUG_MARK;
		}
		else if (--FpRegisterMgr.TimeCnt < 1 )
		{
			FpRegisterMgr.Status = fail;
			FpRegisterMgr.ErrorType = TimeOut;
		}
	}

	else if ( FpRegisterMgr.Status == SendSearchCmd )
	{
		FpRegisterMgr.Status = WaitForSearchCmdACK;
		FpmAckMgr.Status = WaitACK;
		FpmAckMgr.TimeOutCnt = DEF_FpmAckTimeoutTime;
		FPM_SendSearchCmd(0x01,0x0000,(DEF_MAX_FPMASTER+DEF_MAX_FPUSER+DEF_MAX_STRESSFPUSER));		//BuffID is 0x01,from Page 1~100
		DEBUG_MARK;
	}

	else if ( FpRegisterMgr.Status == WaitForSearchCmdACK)
	{
		if (FpmAckMgr.Status == GotACK)
		{
			if ( FpmAckMgr.ErrorCode == Error_NONE)		//if fingerprint is registered
			{
				FpRegisterMgr.Status = fail;
				FpRegisterMgr.ErrorType = FingerPrintIsRegistered;
			}
			else if ( FpmAckMgr.ErrorCode == Error_UnRegistered)	
			{
				FpRegisterMgr.Status = SendDetectFingerRemoveCmd;
				FpRegisterMgr.TimeCnt = Def_WaitUserRemoveFingerTimeDelay;	
				GUI_Flag_RefreshLCD = bTRUE;
				GUI_RefreshSleepTime();
				PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_PleasePutFingerAgain);
			}
			else
			{
				FpRegisterMgr.Status = fail;
				FpRegisterMgr.ErrorType = FpRegisterFail;
			}
			DEBUG_MARK;
		}
		else if (--FpRegisterMgr.TimeCnt < 1 )
		{
			FpRegisterMgr.Status = fail;
			FpRegisterMgr.ErrorType = TimeOut;
		}
	}

	else if ( FpRegisterMgr.Status == SendDetectFingerRemoveCmd )
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(3,16,PleasePutFingerAgain,NormalDisplay);
			DisOneDigital16x8(6,50,FpRegisterMgr.EnrollSuccessTimes,NormalDisplay);
			DisEN16x8Str(6,60,"/",NormalDisplay);
			DisOneDigital16x8(6,70,DEF_FPM_EnrollTimes,NormalDisplay);
		}
		else{
			DisEN16x8Str(3,24,PleasePutFingerAgainStr1En,NormalDisplay);
			DisEN16x8Str(5,24,PleasePutFingerAgainStr2En,NormalDisplay);
		}
		FpRegisterMgr.Status = WaitForDetectFingerRemoveCmdACK;
		FpmAckMgr.Status = WaitACK;
		FpmAckMgr.TimeOutCnt = DEF_FpmAckTimeoutTime;
		FPM_SendGetEnrollImageCmd();
		DEBUG_MARK;
	}
	else if ( FpRegisterMgr.Status == WaitForDetectFingerRemoveCmdACK )
	{
		if (FpmAckMgr.Status == GotACK)
		{
			if ( FpmAckMgr.ErrorCode != Error_NoFinger)
			{
				FpRegisterMgr.Status = SendDetectFingerRemoveCmd;
				GUI_RefreshSleepTime();
			}
			else
			{
				FpRegisterMgr.Status =  SendGetImageCmd;
				FpRegisterMgr.TimeCnt = Def_WaitUserPutFingerTimeDelay;				
				//GUI_Flag_RefreshLCD = bTRUE;
			}
			DEBUG_MARK;
		}
		else if (--FpRegisterMgr.TimeCnt < 1 )
		{
			FpRegisterMgr.Status = fail;
			FpRegisterMgr.ErrorType = TimeOut;
		}
	}


	
	else if ( FpRegisterMgr.Status == SendRegModelCmd )
	{
		FpRegisterMgr.Status = WaitForRegModelCmdACK;
		FpmAckMgr.Status = WaitACK;
		FpmAckMgr.TimeOutCnt = DEF_FpmAckTimeoutTime;
		FPM_SendRegModelCmd();	
		FpRegisterMgr.TimeCnt = Def_FPMcmdTimeOutDelay;		
		DEBUG_MARK;
	}
	else if ( FpRegisterMgr.Status == WaitForRegModelCmdACK)
	{
		if (FpmAckMgr.Status == GotACK)
		{
			DEBUG_MARK;
			if ( FpmAckMgr.ErrorCode == Error_NONE)
			{
				FpRegisterMgr.Status = SendStoreCharCmd;
			}
			else
			{
				DEBUG_MARK;
				FpRegisterMgr.Status = fail;
				FpRegisterMgr.ErrorType = FpRegisterFail;
			}
			DEBUG_MARK;
		}
		else if (--FpRegisterMgr.TimeCnt < 1 )
		{
			FpRegisterMgr.Status = fail;
			FpRegisterMgr.ErrorType = TimeOut;
		}
	}
	else if ( FpRegisterMgr.Status == SendStoreCharCmd )
	{
		FpRegisterMgr.Status = WaitForStoreCharCmdACK;
		FpmAckMgr.Status = WaitACK;
		FpmAckMgr.TimeOutCnt = DEF_FpmAckTimeoutTime;
		FPM_SendStoreCharCmd(0x01,FpRegisterMgr.UserID);		//BuffID is 0x01 ,Page ID is UserID
		FpRegisterMgr.TimeCnt = Def_FPMcmdTimeOutDelay;	
		DEBUG_MARK;
	}
	else if ( FpRegisterMgr.Status == WaitForStoreCharCmdACK )
	{
		if (FpmAckMgr.Status == GotACK)
		{
			if ( FpmAckMgr.ErrorCode == Error_NONE)
			{
				FpRegisterMgr.Status = success;
			}
			else
			{
				FpRegisterMgr.Status = fail;
				FpRegisterMgr.ErrorType = FpRegisterFail;
			}
			DEBUG_MARK;
		}
		else if (--FpRegisterMgr.TimeCnt < 1 )
		{
			FpRegisterMgr.Status = fail;
			FpRegisterMgr.ErrorType = TimeOut;
		}
	}
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowRegisterUserFp(void)
{
	//uint8_t i;
	//uint16_t UserID;
	const uint8_t AddUserFpStr[]={HZ_tian,HZ_jia,HZ_zhi,HZ_wen,HZ_yong,HZ_hu,HZ_end};  	
	const uint8_t AddUserFpStrEn[]={"Add FP user"};  	
	const uint8_t AddMasterFpStr[]={HZ_tian,HZ_jia,HZ_guan,HZ_li,HZ_yuan,HZ_zhi,HZ_wen,HZ_end};  	
	const uint8_t AddMasterFpStrEn[]={"Add FP Admin."};  
	const uint8_t AddStressUserFpStr[]={HZ_tian,HZ_jia,HZ_xie,HZ_po,HZ_zhi,HZ_wen,HZ_end};  	
	const uint8_t AddStressUserFpStrEn[]={"Add Special FP"}; 


		
	if (SystemLanguage == Chinese)
	{
		if (FpUserRegisterMgr.FpUserType == FpMaster){
			DisHZ16x14Str(0,16,AddMasterFpStr,NormalDisplay);
		}else if (FpUserRegisterMgr.FpUserType == FpUser){
			DisHZ16x14Str(0,20,AddUserFpStr,NormalDisplay);
		}else {
			DisHZ16x14Str(0,20,AddStressUserFpStr,NormalDisplay);
		}
	}
	else
	{
		if (FpUserRegisterMgr.FpUserType == FpMaster){
			DisEN16x8Str(0,12,AddMasterFpStrEn,NormalDisplay);
		}else if (FpUserRegisterMgr.FpUserType == FpUser){
			DisEN16x8Str(0,12,AddUserFpStrEn,NormalDisplay);
		}else {
			DisEN16x8Str(0,12,AddStressUserFpStrEn,NormalDisplay);
		}
	}

	if ( FpUserRegisterMgr.Status == StartFpUserRegister )
	{		
		if (((FpUserRegisterMgr.FpUserType == FpMaster)&&(CheckMemoryMgr.FpMasterNum >= DEF_MAX_FPMASTER))
			||((FpUserRegisterMgr.FpUserType == FpUser)&&(CheckMemoryMgr.FpUserNum >= DEF_MAX_FPUSER))
			||((FpUserRegisterMgr.FpUserType == StressFpUser)&&(CheckMemoryMgr.StressFpUserNum >= DEF_MAX_STRESSFPUSER))
				)
		{
			FpUserRegisterMgr.Status = RegisterUserFail;
			FpUserRegisterMgr.ErrorType = MemoryIsFull;
			FpUserRegisterMgr.TimeCnt = Def_MessageBoxTimeDelay;
			PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_UsersAreFull);
		}
		else 
		{
			FpUserRegisterMgr.Status = InputUserID;
		}
	}
	else if ( FpUserRegisterMgr.Status == InputUserID )
	{

		if ( FpUserRegisterMgr.FpUserType == FpMaster )
		{
			FpUserRegisterMgr.UserID = Get_Availabe_FPmasterID();
		}
		else if ( FpUserRegisterMgr.FpUserType == FpUser )
		{
			FpUserRegisterMgr.UserID = Get_Availabe_FPuserID();
		}
		else if ( FpUserRegisterMgr.FpUserType == StressFpUser )
		{
			FpUserRegisterMgr.UserID = Get_Availabe_StressFPuserID();
		}


		if (SystemLanguage == Chinese)
		{
			DisHZ16x14Str(3,20,UserIDStr,NormalDisplay);
			DisOneDigital16x8(3,84,FpUserRegisterMgr.UserID/100,NormalDisplay);
			DisOneDigital16x8(3,92,FpUserRegisterMgr.UserID%100/10,NormalDisplay);
			DisOneDigital16x8(3,100,FpUserRegisterMgr.UserID%10,NormalDisplay);
		}
		else
		{
			DisEN16x8Str(2,20,UserIDStrEn,NormalDisplay);
			DisOneDigital16x8(2,84,FpUserRegisterMgr.UserID/100,NormalDisplay);
			DisOneDigital16x8(2,92,FpUserRegisterMgr.UserID%100/10,NormalDisplay);
			DisOneDigital16x8(2,100,FpUserRegisterMgr.UserID%10,NormalDisplay);
		}



		if (SystemLanguage == Chinese)
		{
			DisHZ16x14Str(6,0,PressAsteriskKeyToReturnStr,NormalDisplay);
			DisHZ16x14Str(6,76,PressPoundKeyToConfirmStr,NormalDisplay);
		}
		else
		{
			DisEN16x8Str(4,0,PressPoundKeyToConfirmStrEn,NormalDisplay);
			DisEN16x8Str(6,0,PressAsteriskKeyToReturnStrEn,NormalDisplay);
		}

		
		
		FpUserRegisterMgr.Status = ReportFpUserID;

		VoiceReportUserIDWithUserConfirm(FpUserRegisterMgr.UserID);
		
		FpUserRegisterMgr.TimeCnt = Def_GuiTimeDelayCnt15s;
		
		DataInputMgr.Status = InputIdle;
	}

	else if ( FpUserRegisterMgr.Status == ReportFpUserID )
	{
		if ( FpUserRegisterMgr.TimeCnt-- <1 )
		{
			GoIntoFpMenu_Init();
		}
	}
	
	else if (FpUserRegisterMgr.Status == AddUserToFPM)
	{
		RegisterFp(FpUserRegisterMgr.UserID-1);
		
		if (FpRegisterMgr.Status == success)
		{
			FpUserRegisterMgr.Status = RegisterUserSuccess;
			FpUserRegisterMgr.TimeCnt = Def_MessageBoxTimeDelay;
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_OperationSuccess);
			GUI_Flag_RefreshLCD = bTRUE;

			GUI_SetFPM_LED(DEF_FpmLedMode_Breath,DEF_FpmLedColor_Green,DEF_FpmLedColor_Green,255);
				
			ComPort_SetPost_Info(DEF_WifiInfo_AddUser,FPUSER,FpUserRegisterMgr.UserID);

			#if defined (Function_YouzhiyunjiaWifi) || defined (Function_TuyaWifi)
			Wifi_PostEvent(DEF_WifiEvent_AddUser,FPUSER,FpUserRegisterMgr.UserID);
			#endif
			
			if ( FpUserRegisterMgr.FpUserType == FpMaster )
			{
				CheckMemoryMgr.FpMasterNum+=1;
				FpUserMemoryMgr[FpUserRegisterMgr.UserID-1].UserID = FpUserRegisterMgr.UserID;
				FpUserMemoryMgr[FpUserRegisterMgr.UserID-1].RegisterStatus = Registered;
				FpUserMemoryMgr[FpUserRegisterMgr.UserID-1].UserPriority = Master;
			}
			else
			{
				if( FpUserRegisterMgr.FpUserType == FpUser ){
					CheckMemoryMgr.FpUserNum+=1;
				}else{
					CheckMemoryMgr.StressFpUserNum+=1;
				}
				FpUserMemoryMgr[FpUserRegisterMgr.UserID-1].UserID = FpUserRegisterMgr.UserID;
				FpUserMemoryMgr[FpUserRegisterMgr.UserID-1].RegisterStatus = Registered;
				FpUserMemoryMgr[FpUserRegisterMgr.UserID-1].UserPriority = User;
			}
			AddFPuserIdToList(FpUserRegisterMgr.UserID);	
		}
		else if (FpRegisterMgr.Status == fail)
		{
			FpUserRegisterMgr.Status = RegisterUserFail;
			FpUserRegisterMgr.ErrorType = FpRegisterMgr.ErrorType;
			FpUserRegisterMgr.TimeCnt = Def_MessageBoxTimeDelay;
			if ( FpUserRegisterMgr.ErrorType == FingerPrintIsRegistered ){
				PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_RepeatedFingerprint);
			}
			else{
				PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_OperationFail);
			}
			GUI_Flag_RefreshLCD = bTRUE;
			GUI_SetFPM_LED(DEF_FpmLedMode_Breath,DEF_FpmLedColor_Red,DEF_FpmLedColor_Red,255);
		}
	}

	else if ( FpUserRegisterMgr.Status == RegisterUserSuccess )
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(4,36,OperationSuccessStr,NormalDisplay);
		}
		else{
			DisEN16x8Str(4,36,OperationSuccessStrEn,NormalDisplay);
		}
		
		if (--FpUserRegisterMgr.TimeCnt < 1 )		
		{
			GUI_SetFPM_LED(DEF_FpmLedMode_Breath,DEF_FpmLedColor_Blue,DEF_FpmLedColor_Blue,255);
			FpUserRegisterMgr.Status = StartFpUserRegister;
			GUI_Flag_RefreshLCD = bTRUE;
			GUI_RefreshSleepTime();
			GUI_CreatAndSaveLog(AddFpUser);			
		}
		DEBUG_MARK;
	}
	else if ( FpUserRegisterMgr.Status == RegisterUserFail )
	{		
		if ( FpUserRegisterMgr.ErrorType == UserIDisRegistered )
		{
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(4,10,IDisRegisteredStr,NormalDisplay);
			}
			else{
				DisEN16x8Str(4,10,IDisRegisteredStrEn,NormalDisplay);
			}
		}
		else if ( FpUserRegisterMgr.ErrorType == SystemNoMaster )
		{
			Clear_Screen();
			DisImage(2,50,27,24,Icon_Warning,NormalDisplay);
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(6,14,PleaseAddMasterStr,NormalDisplay);
			}
			else{
				DisEN16x8Str(6,10,PleaseAddMasterStrEn,NormalDisplay);
			}
		}
		else if ( FpUserRegisterMgr.ErrorType == FingerPrintIsRegistered )
		{
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(4,36,FingerprintRepeated,NormalDisplay);
			}
			else{
				DisEN16x8Str(4,12,FingerprintRepeatedEn,NormalDisplay);
			}
		}
		else if ( FpUserRegisterMgr.ErrorType == MemoryIsFull )
		{
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(4,32,UserIsFullStr,NormalDisplay);
			}
			else{
				DisEN16x8Str(4,4,UserIsFullStrEn,NormalDisplay);
			}
		}
		else
		{
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(4,36,OperationFailStr,NormalDisplay);
			}
			else{
				DisEN16x8Str(4,48,OperationFailStrEn,NormalDisplay);
			}
		}
		if (FpUserRegisterMgr.TimeCnt-- < 1 )		
		{
			GUI_SetFPM_LED(DEF_FpmLedMode_Breath,DEF_FpmLedColor_Blue,DEF_FpmLedColor_Blue,255);
			
			if (FpUserRegisterMgr.FpUserType == FpMaster)
			{
				GoIntoFpMenu_Init();
			}
			else if (FpUserRegisterMgr.FpUserType == FpUser)
			{
				GoIntoFpMenu_Init();	
			}
			else 
			{
				GoIntoFpMenu_Init();
			}

		}
		DEBUG_MARK;
	}
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowDeleteUserFp(void)
{
	//uint8_t i;
	const uint8_t DelUserFpStr[]={HZ_shan,HZ_chufa,HZ_zhi,HZ_wen,HZ_end};  	
	const uint8_t DelUserFpStrEn[]={"Delete FP"};  	


	if (SystemLanguage == Chinese)
	{
		DisHZ16x14Str(0,32,DelUserFpStr,NormalDisplay);
	}
	else
	{
		DisEN16x8Str(0,12,DelUserFpStrEn,NormalDisplay);
	}

	if ( FpUserDeleteMgr.Status == StartFpUserDelete )
	{
		FpUserDeleteMgr.Status = InputUserID;
		DataInputMgr.Status = InputIdle;
		PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_PleaseInputID);
	}
	else if ( FpUserDeleteMgr.Status == InputUserID )
	{

		if (SystemLanguage == Chinese){
			DisHZ16x14Str(3,24,InputUserIDStr,NormalDisplay);
		}
		else{
			DisEN16x8Str(3,24,InputUserIDStrEn,NormalDisplay);
		}

		GUI_DataInputCreat(6,42,3,0x0000);

		if (DataInputMgr.Status == InputEnd)
		{
			FpUserDeleteMgr.UserID = DataInputMgr.Value;	
			FpUserDeleteMgr.Status = ReportFpUserID;
			FpUserDeleteMgr.TimeCnt = Def_GuiTimeDelayCnt4s;
			CheckIfFpUserIDisRegistered.Status = StartCheckIfFpUserIDisRegistered;
			DataInputMgr.Status = InputIdle;
			VoiceReportUserID(FpUserDeleteMgr.UserID);
		}			
		else if  (DataInputMgr.Status == InputExit)
		{
			FpUserDeleteMgr.Status = DeleteUserFail;
			FpUserDeleteMgr.ErrorType = QUIT;
			FpUserDeleteMgr.TimeCnt = 0;
		}
	}
	else if ( FpUserDeleteMgr.Status == ReportFpUserID )
	{
		if (--FpUserDeleteMgr.TimeCnt < 1 )
		{
			FpUserDeleteMgr.Status = CheckIfFpUserIDisBeUsed;
		}
	}
	else if (FpUserDeleteMgr.Status == CheckIfFpUserIDisBeUsed)
	{
		CheckIfFpUserIsRegistered(FpUserDeleteMgr.UserID-1);
		if ( CheckIfFpUserIDisRegistered.Status == CheckIfFpUserIDisRegisteredSuccess )
		{
			if ( CheckIfFpUserIDisRegistered.UserIDisRegistered == bTRUE )
			{
				if ( (ManagerIdentifyMgr.MasterType == FPUSER )
					&&(ManagerIdentifyMgr.FpUserID == FpUserDeleteMgr.UserID)
					)
					
				{
					FpUserDeleteMgr.Status = DeleteUserFail;
					FpUserDeleteMgr.ErrorType = ManagerHasLogin;
					FpUserDeleteMgr.TimeCnt = Def_GuiTimeDelayCnt3s;
					GUI_Flag_RefreshLCD = bTRUE;
					//PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_DeleteFail);
					PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_AdminIsLoginCannotbeDelete);
				}
				else{

					FpUserDeleteMgr.Status = DeleteUserFromFPM;	
					DataInputMgr.Status = InputIdle;	
					GUI_Flag_RefreshLCD = bTRUE;
				}
			}
			else
			{
				FpUserDeleteMgr.Status = DeleteUserFail;
				FpUserDeleteMgr.TimeCnt = Def_MessageBoxTimeDelay;
				FpUserDeleteMgr.ErrorType = UserIDisNotRegistered;
				PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_UserIdIsNotExist);
				GUI_Flag_RefreshLCD = bTRUE;
			}
		}
		else if ( CheckIfFpUserIDisRegistered.Status == CheckIfFpUserIDisRegisteredFail )
		{
			FpUserDeleteMgr.Status = DeleteUserFail;
			FpUserDeleteMgr.TimeCnt = Def_MessageBoxTimeDelay;
			FpUserDeleteMgr.ErrorType = QUIT;
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_DeleteFail);
			GUI_Flag_RefreshLCD = bTRUE;
		}
	}
	else if ( FpUserDeleteMgr.Status == DeleteUserFromFPM)
	{
		FPM_DeleteCharCmd(FpUserDeleteMgr.UserID-1,1);
		FpUserDeleteMgr.Status = WaitForDeleteUserACKfromFPM;
		FpUserDeleteMgr.TimeCnt = Def_FPMcmdTimeOutDelay;
		FpmAckMgr.Status = WaitACK;
		FpmAckMgr.TimeOutCnt = DEF_FpmAckTimeoutTime;
	}
	else if (FpUserDeleteMgr.Status == WaitForDeleteUserACKfromFPM)
	{
		if (FpmAckMgr.Status == GotACK)
		{
			if ( FpmAckMgr.ErrorCode == Error_NONE)
			{
				FpUserDeleteMgr.Status = DeleteUserSuccess;
				GUI_Flag_RefreshLCD = bTRUE;
				FpUserDeleteMgr.TimeCnt = Def_MessageBoxTimeDelay;
				PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_DeleteSuccess);
				
				ComPort_SetPost_Info(DEF_WifiInfo_DeleteUser,FPUSER,FpUserDeleteMgr.UserID);
				
				#if defined (Function_YouzhiyunjiaWifi) || defined (Function_TuyaWifi)
				Wifi_PostEvent(DEF_WifiEvent_DeleteUser,FPUSER,FpUserDeleteMgr.UserID);
				#endif

				if ( FpUserDeleteMgr.UserID <= DEF_MAX_FPMASTER )
				{
					if ( CheckMemoryMgr.FpMasterNum > 0 )
					{
						CheckMemoryMgr.FpMasterNum-=1;
						FpUserMemoryMgr[FpUserDeleteMgr.UserID-1].UserID = 0xFFFF;
						FpUserMemoryMgr[FpUserDeleteMgr.UserID-1].RegisterStatus = UnRegistered;
						FpUserMemoryMgr[FpUserDeleteMgr.UserID-1].UserPriority = Undefined;
					}
				}
				else if( FpUserDeleteMgr.UserID <= (DEF_MAX_FPMASTER+DEF_MAX_FPUSER) )
				{
					if ( CheckMemoryMgr.FpUserNum > 0 )
					{
						CheckMemoryMgr.FpUserNum-=1;
						FpUserMemoryMgr[FpUserDeleteMgr.UserID-1].UserID = 0xFFFF;
						FpUserMemoryMgr[FpUserDeleteMgr.UserID-1].RegisterStatus = UnRegistered;
						FpUserMemoryMgr[FpUserDeleteMgr.UserID-1].UserPriority = Undefined;
					}
				}
				else
				{
					if ( CheckMemoryMgr.StressFpUserNum > 0 )
					{
						CheckMemoryMgr.StressFpUserNum-=1;
						FpUserMemoryMgr[FpUserDeleteMgr.UserID-1].UserID = 0xFFFF;
						FpUserMemoryMgr[FpUserDeleteMgr.UserID-1].RegisterStatus = UnRegistered;
						FpUserMemoryMgr[FpUserDeleteMgr.UserID-1].UserPriority = Undefined;
					}
				}
				DeleteFPuserIdFromList(FpUserDeleteMgr.UserID);
			}
			else
			{
				FpUserDeleteMgr.Status = DeleteUserFail;
				GUI_Flag_RefreshLCD = bTRUE;
				FpUserDeleteMgr.TimeCnt = Def_MessageBoxTimeDelay;
				PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_DeleteFail);
			}
			DEBUG_MARK;
		}
		else if (--FpUserDeleteMgr.TimeCnt < 1 )
			{
				FpUserDeleteMgr.Status = DeleteUserFail;
				FpmAckMgr.ErrorCode = Error_TimeOut;
				GUI_Flag_RefreshLCD = bTRUE;
				FpUserDeleteMgr.TimeCnt = Def_MessageBoxTimeDelay;
				PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_DeleteFail);
			}

	}
	else if ( FpUserDeleteMgr.Status == DeleteUserSuccess )
		{
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(4,36,OperationSuccessStr,NormalDisplay);
			}
			else{
				DisEN16x8Str(4,36,OperationSuccessStrEn,NormalDisplay);
			}
			if (--FpUserDeleteMgr.TimeCnt < 1 )		
			{
				FpUserDeleteMgr.Status = StartFpUserDelete;
				GUI_RefreshSleepTime();
				GUI_Flag_RefreshLCD = bTRUE;
				UnlockModeAutoCalculate();
				GUI_CreatAndSaveLog(DeleteFpUser);
			}
			DEBUG_MARK;
		}
	else if ( FpUserDeleteMgr.Status == DeleteUserFail )
		{
			if ( FpUserDeleteMgr.ErrorType == UserIDisNotRegistered )
			{
				if (SystemLanguage == Chinese){
					DisHZ16x14Str(4,20,IDisNotRegisteredStr,NormalDisplay);
				}
				else{
					DisEN16x8Str(4,12,IDisNotRegisteredStrEn,NormalDisplay);
				}
			}
			else if ( FpUserDeleteMgr.ErrorType == ManagerHasLogin )
			{
				if (SystemLanguage == Chinese){
					DisHZ16x14Str(3,15,CanNotDeleteLastOneMasterStr1,NormalDisplay);
					DisHZ16x14Str(5,32,CanNotDeleteLastOneMasterStr2,NormalDisplay);
				}
				else
				{
					DisEN16x8Str(3,4,CanNotDeleteLastOneMasterStr1En,NormalDisplay);
					DisEN16x8Str(5,16,CanNotDeleteLastOneMasterStr2En,NormalDisplay);
				}
			}
			else
			{
				if (SystemLanguage == Chinese){
					DisHZ16x14Str(4,36,OperationFailStr,NormalDisplay);
				}
				else{
					DisEN16x8Str(4,48,OperationFailStrEn,NormalDisplay);
				}
			}
			if (FpUserDeleteMgr.TimeCnt-- < 1 )		
			{
				GoIntoFpDeleteMenu_Init();
			}
			DEBUG_MARK;
		}
	
}


/*******************************************************/
/*******************************************************/
/*******************************************************/
void ReadRegisteredFPuserFromLocalList(void)
{
	uint8_t i,j,USERID,Checksum,UserIdByte;
	
	
	ReadFPuserIdListFromEEPROM();

	Checksum = 0x00;

	for ( i=0;i<32;i++)
	{
		Checksum+=CheckHomManyRegisteredFPuser.UserIdList[i];
	}

	if (Checksum!=CheckHomManyRegisteredFPuser.UserIdListChecksum)
	{
		ResetFPuserIdListInEEPROM();
		CheckHomManyRegisteredFPuser.UserNum = 0x00;
		CheckHomManyRegisteredFPuser.StressUserNum = 0x00;
		CheckHomManyRegisteredFPuser.MasterNum = 0x00;
	}
	else
	{		
		USERID = 0x00;
		CheckHomManyRegisteredFPuser.UserNum = 0x00;
		CheckHomManyRegisteredFPuser.StressUserNum = 0x00;
		CheckHomManyRegisteredFPuser.MasterNum = 0x00;
		for ( i=0;i<32;i++)
		{
			UserIdByte = CheckHomManyRegisteredFPuser.UserIdList[i];
			for (j=0;j<8;j++)
			{	
				if ( (UserIdByte&0x01) !=0 )
				{
					if ( USERID < DEF_MAX_FPMASTER )
					{
						CheckHomManyRegisteredFPuser.MasterNum++;
						FpUserMemoryMgr[USERID].UserID = USERID;
						FpUserMemoryMgr[USERID].RegisterStatus = Registered;
						FpUserMemoryMgr[USERID].UserPriority = Master;
					}
					else if ( USERID < (DEF_MAX_FPMASTER+DEF_MAX_FPUSER))
					{
						CheckHomManyRegisteredFPuser.UserNum++;
						FpUserMemoryMgr[USERID].UserID = USERID;
						FpUserMemoryMgr[USERID].RegisterStatus = Registered;
						FpUserMemoryMgr[USERID].UserPriority = User;
					}
					else if ( USERID < (DEF_MAX_FPMASTER+DEF_MAX_FPUSER+DEF_MAX_STRESSFPUSER))
					{
						CheckHomManyRegisteredFPuser.StressUserNum++;
						FpUserMemoryMgr[USERID].UserID = USERID;
						FpUserMemoryMgr[USERID].RegisterStatus = Registered;
						FpUserMemoryMgr[USERID].UserPriority = User;
					}
					
				}
				UserIdByte>>=1;
				USERID++;
			}
		}
	}
}

void CheckHowManyRegisteredFPuserFromFPM(void)
{
	//uint8_t FPindexTable[32];
	uint8_t i,j,USERID;
	
	if ( CheckHomManyRegisteredFPuser.Status == StartCheckHowManyRegisteredFPuser )
		{
			FPM_SendReadIndexTableCmd();
			CheckHomManyRegisteredFPuser.TimeCnt = Def_GuiTimeDelayCnt05s;//Def_FPMcmdTimeOutDelay;
			CheckHomManyRegisteredFPuser.Status = WaitForCheckHowManyRegisteredFPuserCmdAck;
			FpmAckMgr.Status = WaitACK;
			FpmAckMgr.TimeOutCnt = DEF_FpmAckTimeoutTime;
		}
	else if ( CheckHomManyRegisteredFPuser.Status == WaitForCheckHowManyRegisteredFPuserCmdAck)
		{
			if (FpmAckMgr.Status == GotACK)
			{
				if ( FpmAckMgr.ErrorCode == Error_NONE)
				{
					USERID = 0x00;
					CheckHomManyRegisteredFPuser.UserNum = 0x00;
					CheckHomManyRegisteredFPuser.StressUserNum = 0x00;
					CheckHomManyRegisteredFPuser.MasterNum = 0x00;
					for ( i=0;i<32;i++)
						{
							for (j=0;j<8;j++)
							{	
								if ( (FpmAckMgr.Buff[10+i]&0x01) !=0 )
								{
									if ( USERID < DEF_MAX_FPMASTER )
									{
										CheckHomManyRegisteredFPuser.MasterNum++;
										FpUserMemoryMgr[USERID].UserID = USERID;
										FpUserMemoryMgr[USERID].RegisterStatus = Registered;
										FpUserMemoryMgr[USERID].UserPriority = Master;
									}
									else if ( USERID < (DEF_MAX_FPMASTER+DEF_MAX_FPUSER))
									{
										CheckHomManyRegisteredFPuser.UserNum++;
										FpUserMemoryMgr[USERID].UserID = USERID;
										FpUserMemoryMgr[USERID].RegisterStatus = Registered;
										FpUserMemoryMgr[USERID].UserPriority = User;
									}
									else if ( USERID < (DEF_MAX_FPMASTER+DEF_MAX_FPUSER+DEF_MAX_STRESSFPUSER))
									{
										CheckHomManyRegisteredFPuser.StressUserNum++;
										FpUserMemoryMgr[USERID].UserID = USERID;
										FpUserMemoryMgr[USERID].RegisterStatus = Registered;
										FpUserMemoryMgr[USERID].UserPriority = User;
									}
								}
								FpmAckMgr.Buff[10+i]>>=1;
								USERID++;
							}
						}
					CheckHomManyRegisteredFPuser.Status = CheckHomManyRegisteredFPuserSuccess;
				}
				else{
					DEBUG_MARK;
					CheckHomManyRegisteredFPuser.Status = CheckHomManyRegisteredFPuserFail;
				}
			}
			if ( CheckHomManyRegisteredFPuser.TimeCnt-- < 1 )
			{
				CheckHomManyRegisteredFPuser.Status = CheckHomManyRegisteredFPuserFail;
			}
		}
	
}


/*******************************************************/
/*******************************************************/
/*******************************************************/
void CheckIfFpUserIsRegistered(uint16_t USERID)
{
	//uint8_t FPindexTable[32];
	uint8_t ByteOffset,BitOffset;
	
	if ( CheckIfFpUserIDisRegistered.Status == StartCheckIfFpUserIDisRegistered )
		{
			FPM_SendReadIndexTableCmd();
			CheckIfFpUserIDisRegistered.TimeCnt = Def_FPMcmdTimeOutDelay;
			CheckIfFpUserIDisRegistered.Status = WaitForCheckIfFpUserIDisRegisteredCmdAck;
			FpmAckMgr.Status = WaitACK;
			FpmAckMgr.TimeOutCnt = DEF_FpmAckTimeoutTime;
		}
	else if ( CheckIfFpUserIDisRegistered.Status == WaitForCheckIfFpUserIDisRegisteredCmdAck)
		{
			if (FpmAckMgr.Status == GotACK)
			{
				if ( FpmAckMgr.ErrorCode == Error_NONE)
				{
					ByteOffset = USERID/8;
					BitOffset = USERID%8;
					CheckIfFpUserIDisRegistered.Status = CheckIfFpUserIDisRegisteredSuccess;
					if ( ((FpmAckMgr.Buff[10+ByteOffset]>>=BitOffset)&0x01) != 0x00 )
					{
						CheckIfFpUserIDisRegistered.UserIDisRegistered = bTRUE;
					}
					else{
						CheckIfFpUserIDisRegistered.UserIDisRegistered = bFALSE;
					}
					
				}
				DEBUG_MARK;
			}
			if ( CheckIfFpUserIDisRegistered.TimeCnt-- < 1 )
			{
				CheckIfFpUserIDisRegistered.Status = CheckIfFpUserIDisRegisteredFail;
			}
		}
	
}



/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowDeleteAllUserFp(void)
{
	uint16_t i;
	
	//DisHZ16x14Str(0,0,TitleStr,NormalDisplay);

	
	if ( AllUserFpDeleteMgr.Status == StartAllUserFpDelete )
	{
		AllUserFpDeleteMgr.Status = WaitForUserConfirmDeleteAllFP;
		AllUserFpDeleteMgr.Selection = NO;
		if (SystemLanguage == Chinese)
		{
			DisHZ16x14Str(2,16,ConfirmDeleteStr,NormalDisplay);
			DisHZ16x14Str(4,16,AbortDeleteStr,NormalDisplay);
		}
		else
		{
			DisEN16x8Str(2,0,ConfirmDeleteStrEn,NormalDisplay);
			DisEN16x8Str(4,0,AbortDeleteStrEn,NormalDisplay);
		}	
		PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_ConfirmOrExitDelete);
	}
	else if ( AllUserFpDeleteMgr.Status == WaitForUserConfirmDeleteAllFP )
	{
		
	}
    else if ( AllUserFpDeleteMgr.Status == SendDeleteAllUserFpCmdToFPM )
	{
		
		if (AllUserFpDeleteMgr.FpUserType == FpUser )
		{
			FPM_DeleteCharCmd(DEF_MAX_FPMASTER,DEF_MAX_FPUSER);
		}
		else if (AllUserFpDeleteMgr.FpUserType == StressFpUser )
		{
			FPM_DeleteCharCmd(DEF_MAX_FPMASTER+DEF_MAX_FPUSER,DEF_MAX_STRESSFPUSER);
		}
		AllUserFpDeleteMgr.Status = WaitForDeleteAllUserFpCmdACKfromFPM;
		AllUserFpDeleteMgr.TimeCnt =Def_FPMcmdTimeOutDelay;
		GUI_Flag_RefreshLCD = bTRUE;
		FpmAckMgr.Status = WaitACK;
		FpmAckMgr.TimeOutCnt = DEF_FpmAckTimeoutTime;
	}
	
	else if ( AllUserFpDeleteMgr.Status == WaitForDeleteAllUserFpCmdACKfromFPM )
		  {
			  if (FpmAckMgr.Status == GotACK)
				{
					if ( FpmAckMgr.ErrorCode == Error_NONE)
					{
						AllUserFpDeleteMgr.Status = DeleteAllFpUserSuccess;
						GUI_Flag_RefreshLCD = bTRUE;
						FpUserDeleteMgr.TimeCnt = Def_MessageBoxTimeDelay;
						PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_DeleteSuccess);

						if (AllUserFpDeleteMgr.FpUserType == FpUser )
						{
							CheckMemoryMgr.FpUserNum=0;
							for (i=DEF_MAX_FPMASTER;i<(DEF_MAX_FPMASTER+DEF_MAX_FPUSER);i++)
							{
								FpUserMemoryMgr[i].UserID = 0xFFFF;
								FpUserMemoryMgr[i].RegisterStatus = UnRegistered;
								FpUserMemoryMgr[i].UserPriority = Undefined;
							}
							DeleteAllFPuserIdFromList();
						}
						else if (AllUserFpDeleteMgr.FpUserType == StressFpUser )
						{
							CheckMemoryMgr.StressFpUserNum=0;
							for (i=(DEF_MAX_FPMASTER+DEF_MAX_FPUSER);i<(DEF_MAX_FPMASTER+DEF_MAX_FPUSER+DEF_MAX_STRESSFPUSER);i++)
							{
								FpUserMemoryMgr[i].UserID = 0xFFFF;
								FpUserMemoryMgr[i].RegisterStatus = UnRegistered;
								FpUserMemoryMgr[i].UserPriority = Undefined;
							}
							DeleteAllStressFPuserIdFromList();
						}
						
					}
					else
					{
						AllUserFpDeleteMgr.Status = DeleteAllFpUserFail;
						GUI_Flag_RefreshLCD = bTRUE;
						AllUserFpDeleteMgr.TimeCnt = Def_MessageBoxTimeDelay;
						PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_DeleteFail);
					}
					DEBUG_MARK;
				}
			else if (--FpUserDeleteMgr.TimeCnt < 1 )
				{
					AllUserFpDeleteMgr.Status = DeleteAllFpUserFail;
					FpmAckMgr.ErrorCode = Error_TimeOut;
					GUI_Flag_RefreshLCD = bTRUE;
					AllUserFpDeleteMgr.TimeCnt = Def_MessageBoxTimeDelay;
					PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_DeleteFail);
				}
		  }
	
	else if ( AllUserFpDeleteMgr.Status == DeleteAllFpUserSuccess )
		{
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(4,36,OperationSuccessStr,NormalDisplay);
			}
			else{
				DisEN16x8Str(4,36,OperationSuccessStrEn,NormalDisplay);
			}
			if (--AllUserFpDeleteMgr.TimeCnt < 1 )
			{
				UnlockModeAutoCalculate();
				if (AllUserFpDeleteMgr.FpUserType == FpUser )
				{
					GoIntoFpDeleteMenu_Init();	
					GUI_CreatAndSaveLog(DeleteAllFpUser);
				}
				else// if (AllUserFpDeleteMgr.FpUserType == StressFpUser )
				{
					GoIntoFpDeleteMenu_Init();
					GUI_CreatAndSaveLog(DeleteAllStressFpUser);
				}
			}
		}
	else if ( AllUserFpDeleteMgr.Status == DeleteAllFpUserFail )
		{
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(4,36,OperationFailStr,NormalDisplay);
			}
			else{
				DisEN16x8Str(4,48,OperationFailStrEn,NormalDisplay);
			}
			if (--AllUserFpDeleteMgr.TimeCnt < 1 )
			{
				GoIntoFpDeleteMenu_Init();	
			}
		}
	else if ( AllUserFpDeleteMgr.Status == EXIT )
		{
			GoIntoFpDeleteMenu_Init();	
		}
	
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void DeleteAllFpFromFPM(void)
{
	//uint8_t i;
	
	if ( AllUserFpDeleteMgr.Status == StartAllUserFpDelete )
		{
			AllUserFpDeleteMgr.Status = SendDeleteAllUserFpCmdToFPM;
		}
    else if ( AllUserFpDeleteMgr.Status == SendDeleteAllUserFpCmdToFPM )
		{
			FPM_DeleteAllCharCmd();
			AllUserFpDeleteMgr.Status = WaitForDeleteAllUserFpCmdACKfromFPM;
			AllUserFpDeleteMgr.TimeCnt = Def_FPMcmdTimeOutDelay;
			FpmAckMgr.Status = WaitACK;
			FpmAckMgr.TimeOutCnt = DEF_FpmAckTimeoutTime;
		}
	else if ( AllUserFpDeleteMgr.Status == WaitForDeleteAllUserFpCmdACKfromFPM )
	  {
		  if (FpmAckMgr.Status == GotACK)
			{
				if ( FpmAckMgr.ErrorCode == Error_NONE)
				{
					AllUserFpDeleteMgr.Status = DeleteAllFpUserSuccess;
				}
				else
				{
					AllUserFpDeleteMgr.Status = DeleteAllFpUserFail;
				}
				DEBUG_MARK;
			}
		else if (--AllUserFpDeleteMgr.TimeCnt < 1 )
			{
				AllUserFpDeleteMgr.Status = DeleteAllFpUserFail;
				FpmAckMgr.ErrorCode = Error_TimeOut;
			}
	  }	
}



/*******************************************************/
/*******************************************************/
/*******************************************************/
uint8_t Get_Availabe_CardUserID(void)
{
	uint8_t i;
	for (i=0;i<DEF_MAX_CARDUSER;i++)
	{
		if ( CardMemoryMgr[i].Status == CIDisInvalid ){
			return (i+1);
		}
	}
	return 0;
}
/*******************************************************/
/*******************************************************/
/*******************************************************/
void ReadFPuserIdListFromEEPROM(void)
{
	EEPROM_ReadSequential(FPuserIDListStartAddr,&CheckHomManyRegisteredFPuser.UserIdList[0],33);
}
void WriteFPuserIdListToEEPROM(void)
{
	uint8_t i,checksum;
	checksum = 0x00;
	for ( i=0;i<32;i++)
	{
		checksum+=CheckHomManyRegisteredFPuser.UserIdList[i];
	}
	CheckHomManyRegisteredFPuser.UserIdListChecksum = checksum;
	
	EEPROM_WriteSequential(FPuserIDListStartAddr,&CheckHomManyRegisteredFPuser.UserIdList[0],33);
}
void ResetFPuserIdListInEEPROM(void)
{
	uint8_t i,checksum;
	checksum = 0x00;
	for ( i=0;i<32;i++)
	{
		CheckHomManyRegisteredFPuser.UserIdList[i]=0x00;
		checksum+=CheckHomManyRegisteredFPuser.UserIdList[i];
	}
	
	CheckHomManyRegisteredFPuser.UserIdListChecksum = checksum;
	WriteFPuserIdListToEEPROM();
}

/******************************************/
/******************************************/
/**** FPuserID from 1 to 100 **************/
/******************************************/
void AddFPuserIdToList(uint16_t FPuserID)
{
	uint8_t i,OffsetByte,OffsetBit,ByteValue;
	OffsetByte = (FPuserID-1)/8;
	OffsetBit =  (FPuserID-1)%8;

	ByteValue = 0x01;
	ByteValue <<= OffsetBit;
	CheckHomManyRegisteredFPuser.UserIdList[OffsetByte] |= ByteValue;
	WriteFPuserIdListToEEPROM();
}
void DeleteFPuserIdFromList(uint16_t FPuserID)
{
	uint8_t i,OffsetByte,OffsetBit;
 	uint8_t ByteValue;
	OffsetByte = (FPuserID-1)/8;
	OffsetBit =  (FPuserID-1)%8;

	ByteValue = 0x01;
	ByteValue <<= OffsetBit;
	ByteValue ^=0xFF;
	
	CheckHomManyRegisteredFPuser.UserIdList[OffsetByte] &= ByteValue;

	WriteFPuserIdListToEEPROM();
}
void DeleteAllFPuserIdFromList(void)
{
	uint8_t UserID,OffsetByte,OffsetBit,ByteValue;

	for (UserID=DEF_MAX_FPMASTER;UserID<(DEF_MAX_FPMASTER+DEF_MAX_FPUSER);UserID++)
	{
		OffsetByte = UserID/8;
		OffsetBit =  UserID%8;
		ByteValue = 0x01;
		ByteValue <<= OffsetBit;
		ByteValue = ~ByteValue;
		CheckHomManyRegisteredFPuser.UserIdList[OffsetByte] &= ByteValue;
	}

	WriteFPuserIdListToEEPROM();
}

void DeleteAllStressFPuserIdFromList(void)
{
	uint8_t UserID,OffsetByte,OffsetBit,ByteValue;

	for (UserID=(DEF_MAX_FPMASTER+DEF_MAX_FPUSER);UserID<(DEF_MAX_FPMASTER+DEF_MAX_FPUSER+DEF_MAX_STRESSFPUSER);UserID++)
	{
		OffsetByte = UserID/8;
		OffsetBit =  UserID%8;
		ByteValue = 0x01;
		ByteValue <<= OffsetBit;
		ByteValue = ~ByteValue;
		CheckHomManyRegisteredFPuser.UserIdList[OffsetByte] &= ByteValue;
	}

	WriteFPuserIdListToEEPROM();

}



/*******************************************************/
/*******************************************************/
/*******************************************************/
void ReadCardUserMemoryFromEEPROM(void)
{
	EEPROM_ReadSequential(CardUserMemoryStartAddr,&CardMemoryMgr[0].UserID,(6*DEF_MAX_CARDUSER));
}
void WriteCardUserMemoryToEEPROM(void)
{
	EEPROM_WriteSequential(CardUserMemoryStartAddr,&CardMemoryMgr[0].UserID,(6*DEF_MAX_CARDUSER));
}

/*******************************************************/
/*****		Input CID[5]; 					   *********/
/*****      OUTPUT S_FAIL- MemoryIsFull, SUCCESS-be saved ****************/
/*******************************************************/
status_t SaveCardUserToMemory(uint8_t *Point,uint8_t UserID)
{
	uint8_t j;
	//uint8_t CardUserID;

	for (j=0;j<4;j++)
	{
		CardMemoryMgr[UserID-1].CID[j] = *(Point+j);
	}
	CardMemoryMgr[UserID-1].UserID = UserID;
	CardMemoryMgr[UserID-1].Status = CIDisValid;
	
	WriteCardUserMemoryToEEPROM();
	return S_SUCCESS;

}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void DeleteCardUserfromMemory(uint8_t UserID)
{
	uint8_t j;

	CardMemoryMgr[UserID-1].Status = CIDisInvalid;
	CardMemoryMgr[UserID-1].UserID = 0xFF;
	for (j=0;j<4;j++)
	{
		CardMemoryMgr[UserID-1].CID[j] = 0xFF;
	}
	WriteCardUserMemoryToEEPROM();
}
/*******************************************************/
/*******************************************************/
/*******************************************************/
void DeleteAllCardUserfromMemory(void)
{
	uint8_t i,j;

	for (i=0;i<DEF_MAX_CARDUSER;i++)
	{

		CardMemoryMgr[i].Status = CIDisInvalid;
		CardMemoryMgr[i].UserID = 0xFF;
		for (j=0;j<4;j++)
		{
			CardMemoryMgr[i].CID[j] = 0xFF;
		}

	}	
	WriteCardUserMemoryToEEPROM();
}


/*******************************************************/
/*****Input CID[5]; OUTPUT 0-CardIsNotRegistered, 1~255 CardIDisRegister, value is the UserID****************/
/*******************************************************/
uint8_t CompareCardIDwithMemory(uint8_t *Point)
{
	uint8_t i;
	uint8_t CardUserID;

	for (i=0;i<DEF_MAX_CARDUSER;i++)
	{
		if ( CardMemoryMgr[i].Status == CIDisValid )
		{
			if (  (CardMemoryMgr[i].CID[0] == *Point)
				&&(CardMemoryMgr[i].CID[1] == *(Point+1))
				&&(CardMemoryMgr[i].CID[2] == *(Point+2))
				&&(CardMemoryMgr[i].CID[3] == *(Point+3))
				//&&(CardMemoryMgr[i].CID[4] == *(Point+4))
				 )
			{
				CardUserID = CardMemoryMgr[i].UserID;
				return CardUserID;
			}
		}
	}
	return 0;
}
/*******************************************************/
/*****Input CID[5]; OUTPUT 0-CardIsNotRegistered, 1~255 CardIDisRegister, value is the UserID****************/
/*******************************************************/
bool_t IfCardUserIDisRegistered(uint8_t CardUserID)
{
	uint8_t i;

	for (i=0;i<DEF_MAX_CARDUSER;i++)
	{
		if ( CardMemoryMgr[i].Status == CIDisValid )
			{
				if (  CardMemoryMgr[i].UserID == CardUserID)
				{
					return bTRUE;
				}
			}
	}
	return bFALSE;
}

uint8_t CheckHowManyRegisteredCardUser( void )
{
	uint8_t i,UserNum;
	UserNum =0;
	for (i=0;i<DEF_MAX_CARDUSER;i++)
	{
		if ( CardMemoryMgr[i].Status == CIDisValid )
		{
			UserNum++;
		}
	}
	return UserNum;
}

bool_t IfSystemIsNoCardUser(void)
{
	if ( CheckHowManyRegisteredCardUser() == 0 )
	{
		return bTRUE;
	}
	else
	{
		return bFALSE;
	}

}


/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowRegisterCardUser(void)
{
	status_t GetCardID;
	//uint8_t i;
	const uint8_t TitleStr[]={HZ_tian,HZ_jia,HZ_ka,HZ_pian,HZ_end};  	//添加卡片用户
	const uint8_t TitleStrEn[]={"Add card user"};  	//添加卡片用户
	
	if (SystemLanguage == Chinese){
		DisHZ16x14Str(0,36,TitleStr,NormalDisplay);
	}
	else{
		DisEN16x8Str(0,16,TitleStrEn,NormalDisplay);
	}

	if ( CardUserRegisterMgr.Status == StartCardUserRegister )
	{
		if ( CheckMemoryMgr.CardUserNum < DEF_MAX_CARDUSER )
		{
			CardUserRegisterMgr.Status = InputCardUserID;
			DataInputMgr.Status = InputIdle;
			//PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_PleaseInputID);
		}
		else
		{
			CardUserRegisterMgr.Status = Fail;
			CardUserRegisterMgr.ErrorType = MemoryIsFull;
			CardUserRegisterMgr.TimeCnt = Def_MessageBoxTimeDelay;
			PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_UsersAreFull);
		}	
	}
	else if ( CardUserRegisterMgr.Status == InputCardUserID )
	{	
		CardUserRegisterMgr.UserID = Get_Availabe_CardUserID();

//		if (SystemLanguage == Chinese){
//			DisHZ16x14Str(4,20,UserIDStr,NormalDisplay);
//		}
//		else{
//			DisEN16x8Str(4,20,UserIDStrEn,NormalDisplay);
//		}
//		
//		DisOneDigital16x8(4,84,CardUserRegisterMgr.UserID/100,NormalDisplay);
//		DisOneDigital16x8(4,92,CardUserRegisterMgr.UserID%100/10,NormalDisplay);
//		DisOneDigital16x8(4,100,CardUserRegisterMgr.UserID%10,NormalDisplay);

		if (SystemLanguage == Chinese)
		{
			DisHZ16x14Str(3,20,UserIDStr,NormalDisplay);
			DisOneDigital16x8(3,84,CardUserRegisterMgr.UserID/100,NormalDisplay);
			DisOneDigital16x8(3,92,CardUserRegisterMgr.UserID%100/10,NormalDisplay);
			DisOneDigital16x8(3,100,CardUserRegisterMgr.UserID%10,NormalDisplay);
		}
		else
		{
			DisEN16x8Str(2,20,UserIDStrEn,NormalDisplay);
			DisOneDigital16x8(2,84,CardUserRegisterMgr.UserID/100,NormalDisplay);
			DisOneDigital16x8(2,92,CardUserRegisterMgr.UserID%100/10,NormalDisplay);
			DisOneDigital16x8(2,100,CardUserRegisterMgr.UserID%10,NormalDisplay);
		}	
		
		if (SystemLanguage == Chinese)
		{
			DisHZ16x14Str(6,0,PressAsteriskKeyToReturnStr,NormalDisplay);
			DisHZ16x14Str(6,76,PressPoundKeyToConfirmStr,NormalDisplay);
		}
		else
		{
			DisEN16x8Str(4,0,PressPoundKeyToConfirmStrEn,NormalDisplay);
			DisEN16x8Str(6,0,PressAsteriskKeyToReturnStrEn,NormalDisplay);
		}
		
		VoiceReportUserIDWithUserConfirm(CardUserRegisterMgr.UserID);
		
		CardUserRegisterMgr.TimeCnt = Def_GuiTimeDelayCnt15s;
		
		CardUserRegisterMgr.Status = ReportCardUserID;
		
	}
	else if ( CardUserRegisterMgr.Status == ReportCardUserID )
	{
		if ( CardUserRegisterMgr.TimeCnt-- < 1 )
		{
			GoIntoCardMenu_Init();
		}
	}
	else if ( CardUserRegisterMgr.Status == ReadingCardID)
	{

		if (SystemLanguage == Chinese){
			DisHZ16x14Str(4,40,PleaseSwingCardStr,NormalDisplay);
		}
		else{
			DisEN16x8Str(4,16,PleaseSwingCardStrEn,NormalDisplay);
		}
#if 0
		if ( CardUserRegisterMgr.TimeCnt%32 == 0 )	//0.5s
		{
			GetCardID = MFC_Auto_Reader(CardUserRegisterMgr.CID);
			if ( GetCardID == S_SUCCESS )
			{
				CardUserRegisterMgr.Status = SavedCardID;
				GUI_RefreshSleepTime();
			}
		}
#endif
		if ( CardIdentifyMgr.CardDetectIntervalTimeCnt ==0  )	//FPM Cmd is sent out
		{
			CardIdentifyMgr.CardDetectIntervalTimeCnt = Def_CardDetectIntervalTime;
			
			GetCardID = MFC_Auto_Reader(CardUserRegisterMgr.CID);
			
			if ( GetCardID == S_SUCCESS )
			{
				CardUserRegisterMgr.Status = SavedCardID;
				GUI_RefreshSleepTime();
			}
		}
		if ( --CardUserRegisterMgr.TimeCnt < 1 )
		{
			CardUserRegisterMgr.Status = Fail;
			CardUserRegisterMgr.ErrorType = TimeOut;
			CardUserRegisterMgr.TimeCnt = Def_MessageBoxTimeDelay; 	
			GUI_Flag_RefreshLCD = bTRUE;
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_OperationFail);
			GUI_SetFPM_LED(DEF_FpmLedMode_Breath,DEF_FpmLedColor_Red,DEF_FpmLedColor_Red,255);
		}
	}
	else if ( CardUserRegisterMgr.Status ==  SavedCardID )
	{
		if (CompareCardIDwithMemory(CardUserRegisterMgr.CID) == 0x00 )		//card CID is not be used
		{
			if ( SaveCardUserToMemory(CardUserRegisterMgr.CID,CardUserRegisterMgr.UserID) == S_SUCCESS )
			{
				CardUserRegisterMgr.Status = Success;
				CardUserRegisterMgr.TimeCnt = Def_MessageBoxTimeDelay; 	
				GUI_Flag_RefreshLCD = bTRUE;
				PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_OperationSuccess);
				CheckMemoryMgr.CardUserNum+=1;

				ComPort_SetPost_Info(DEF_WifiInfo_AddUser,CARDUSER,CardUserRegisterMgr.UserID);
				#if defined (Function_YouzhiyunjiaWifi) || defined (Function_TuyaWifi)
				Wifi_PostEvent(DEF_WifiEvent_AddUser,CARDUSER,CardUserRegisterMgr.UserID);
				#endif

			}
			else
			{
				CardUserRegisterMgr.Status = Fail;
				CardUserRegisterMgr.ErrorType = MemoryIsFull;
				CardUserRegisterMgr.TimeCnt = Def_MessageBoxTimeDelay; 	
				GUI_Flag_RefreshLCD = bTRUE;
				PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_UsersAreFull);
			}
		}
		else
		{
			CardUserRegisterMgr.Status = Fail;
			CardUserRegisterMgr.ErrorType = CardCIDisBeUsed;
			CardUserRegisterMgr.TimeCnt = Def_MessageBoxTimeDelay;	
			GUI_Flag_RefreshLCD = bTRUE;
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_RepeatedCard);
		}

		if ( CardUserRegisterMgr.Status ==  Success )
		{
			GUI_SetFPM_LED(DEF_FpmLedMode_Breath,DEF_FpmLedColor_Green,DEF_FpmLedColor_Green,255);
		}
		else if (CardUserRegisterMgr.Status == Fail)
		{
			GUI_SetFPM_LED(DEF_FpmLedMode_Breath,DEF_FpmLedColor_Red,DEF_FpmLedColor_Red,255);
		}
	}
	else if ( CardUserRegisterMgr.Status ==  Success )
		{
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(4,36,OperationSuccessStr,NormalDisplay);
			}
			else{
				DisEN16x8Str(4,36,OperationSuccessStrEn,NormalDisplay);
			}
			if ( --CardUserRegisterMgr.TimeCnt < 1 )
			{
				GUI_SetFPM_LED(DEF_FpmLedMode_Breath,DEF_FpmLedColor_Blue,DEF_FpmLedColor_Blue,255);
				GUI_RefreshSleepTime();
				CardUserRegisterMgr.Status = StartCardUserRegister;
				GUI_Flag_RefreshLCD = bTRUE;
				GUI_CreatAndSaveLog(AddCardUser);
			}
		}
	else if ( CardUserRegisterMgr.Status == Fail )
		{
			if (CardUserRegisterMgr.ErrorType == UserIDisRegistered)
			{
				if (SystemLanguage == Chinese){
					DisHZ16x14Str(4,15,IDisRegisteredStr,NormalDisplay);
				}
				else{
					DisEN16x8Str(4,15,IDisRegisteredStrEn,NormalDisplay);
				}
			}
			else if ( CardUserRegisterMgr.ErrorType == SystemNoMaster )
			{
				Clear_Screen();
				DisImage(2,50,27,24,Icon_Warning,NormalDisplay);
				if (SystemLanguage == Chinese){
					DisHZ16x14Str(6,14,PleaseAddMasterStr,NormalDisplay);
				}
				else{
					DisEN16x8Str(6,10,PleaseAddMasterStrEn,NormalDisplay);
				}
			}
			else if ( CardUserRegisterMgr.ErrorType == MemoryIsFull )
			{
				if (SystemLanguage == Chinese){
					DisHZ16x14Str(4,32,UserIsFullStr,NormalDisplay);
				}
				else{
					DisEN16x8Str(4,4,UserIsFullStrEn,NormalDisplay);
				}
			}
			else if ( CardUserRegisterMgr.ErrorType == CardCIDisBeUsed )
			{
				if (SystemLanguage == Chinese){
					DisHZ16x14Str(4,36,CardCIDisBeUsedStr,NormalDisplay);
				}
				else{
					DisEN16x8Str(4,12,CardCIDisBeUsedStrEn,NormalDisplay);
				}
			}
			else
			{
				if (SystemLanguage == Chinese){
					DisHZ16x14Str(4,36,OperationFailStr,NormalDisplay);
				}
				else{
					DisEN16x8Str(4,48,OperationFailStrEn,NormalDisplay);
				}
			}
			if ( CardUserRegisterMgr.TimeCnt-- < 1 )
			{
				GUI_SetFPM_LED(DEF_FpmLedMode_Breath,DEF_FpmLedColor_Blue,DEF_FpmLedColor_Blue,255);
				GoIntoCardMenu_Init();
			}
		}
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowDeleteCardUser(void)
{
	//uint8_t i;
	const uint8_t TitleStr[]={HZ_shan,HZ_chufa,HZ_ka,HZ_pian,HZ_end};  	//删除卡片用户
	const uint8_t TitleStrEn[]={"Delete CD user"}; 	//删除卡片用户

	if (SystemLanguage == Chinese){
		DisHZ16x14Str(0,36,TitleStr,NormalDisplay);
	}
	else{
		DisEN16x8Str(0,8,TitleStrEn,NormalDisplay);
	}

	if ( CardUserDeleteMgr.Status == StartCardUserDelete )
	{
		CardUserDeleteMgr.Status = InputCardUserID;
		DataInputMgr.Status = InputIdle;
		PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_PleaseInputID);
	}
	if ( CardUserDeleteMgr.Status == InputCardUserID )
	{
		
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(3,28,InputUserIDStr,NormalDisplay);
		}
		else{
			DisEN16x8Str(3,0,InputUserIDStrEn,NormalDisplay);
		}	

		GUI_DataInputCreat(6,48,3,0x0000);

		if (DataInputMgr.Status == InputEnd)
		{
			CardUserDeleteMgr.UserID = DataInputMgr.Value;
			
			if ( CardUserDeleteMgr.UserID > DEF_MAX_CARDUSER )				
			{
				DataInputMgr.Status = InputIdle;		//Reinput
				PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_FailVoice);
			}
			else if ( IfCardUserIDisRegistered(CardUserDeleteMgr.UserID) == bTRUE )
			{
				DeleteCardUserfromMemory(CardUserDeleteMgr.UserID);
				CardUserDeleteMgr.Status = Success;	
				CardUserDeleteMgr.TimeCnt = Def_MessageBoxTimeDelay; 	
				GUI_Flag_RefreshLCD = bTRUE;
				PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_DeleteSuccess);
				if ( CheckMemoryMgr.CardUserNum > 0 )
				{
					CheckMemoryMgr.CardUserNum-=1;
				}
			
				ComPort_SetPost_Info(DEF_WifiInfo_DeleteUser,CARDUSER,CardUserDeleteMgr.UserID);

				#if defined (Function_YouzhiyunjiaWifi) || defined (Function_TuyaWifi)
				Wifi_PostEvent(DEF_WifiEvent_DeleteUser,CARDUSER,CardUserDeleteMgr.UserID);
				#endif
					
			}
			else
			{
				CardUserDeleteMgr.Status = Fail;
				CardUserDeleteMgr.ErrorType = UserIDisNotRegistered;
				CardUserDeleteMgr.TimeCnt = Def_MessageBoxTimeDelay; 	
				GUI_Flag_RefreshLCD = bTRUE;
				PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_UserIdIsNotExist);
			}
		}
		else if  (DataInputMgr.Status == InputExit)
		{
			CardUserDeleteMgr.Status = Fail;
			CardUserDeleteMgr.ErrorType = QUIT;
			CardUserDeleteMgr.TimeCnt = 0;
		}
	}
	
	else if  ( CardUserDeleteMgr.Status == Success)
		{
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(4,36,OperationSuccessStr,NormalDisplay);
			}
			else{
				DisEN16x8Str(4,36,OperationSuccessStrEn,NormalDisplay);
			}
			if (--CardUserDeleteMgr.TimeCnt < 1 )
				{
					GoIntoCardMenu_Init();
					UnlockModeAutoCalculate();
					GUI_CreatAndSaveLog(DeleteCardUser);
				}
		}
	else if ( CardUserDeleteMgr.Status == Fail)
		{
			if (CardUserDeleteMgr.ErrorType == UserIDisNotRegistered)
			{
				if (SystemLanguage == Chinese){
					DisHZ16x14Str(3,15,IDisNotRegisteredStr,NormalDisplay);
				}
				else{
					DisEN16x8Str(3,12,IDisNotRegisteredStrEn,NormalDisplay);
				}
			}
			else
			{
				if (SystemLanguage == Chinese){
					DisHZ16x14Str(6,36,OperationFailStr,NormalDisplay);
				}
				else{
					DisEN16x8Str(6,48,OperationFailStrEn,NormalDisplay);
				}

			}
			if ( CardUserDeleteMgr.TimeCnt-- < 1 )
			{
				GoIntoCardMenu_Init();
			}
		}
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowDeleteAllCardUser(void)
{
	
	if ( AllCardUserDeleteMgr.Status == StartAllCardUserDelete )
		{
			AllCardUserDeleteMgr.Status = WaitForUserConfirmDeleteAllCard;
			AllCardUserDeleteMgr.Selection = NO;
			if (SystemLanguage == Chinese)
			{
				DisHZ16x14Str(2,16,ConfirmDeleteStr,NormalDisplay);
				DisHZ16x14Str(4,16,AbortDeleteStr,NormalDisplay);
			}
			else
			{
				DisEN16x8Str(2,0,ConfirmDeleteStrEn,NormalDisplay);
				DisEN16x8Str(4,0,AbortDeleteStrEn,NormalDisplay);
			}	
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_ConfirmOrExitDelete);
		}
	else if ( AllCardUserDeleteMgr.Status == WaitForUserConfirmDeleteAllCard )
		{
			
		}
    else if ( AllCardUserDeleteMgr.Status == DeletingAllCardUser )
		{
			
			DeleteAllCardUserfromMemory();
			AllCardUserDeleteMgr.TimeCnt =Def_MessageBoxTimeDelay;
			AllCardUserDeleteMgr.Status = Success;
			GUI_Flag_RefreshLCD = bTRUE;
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_DeleteSuccess);
			CheckMemoryMgr.CardUserNum =0;
		}
	else if ( AllCardUserDeleteMgr.Status == Success )
		{
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(4,36,OperationSuccessStr,NormalDisplay);
			}
			else{
				DisEN16x8Str(4,36,OperationSuccessStrEn,NormalDisplay);
			}
			if (AllCardUserDeleteMgr.TimeCnt-- < 1 )
			{
				GoIntoCardMenu_Init();
				UnlockModeAutoCalculate();
				GUI_CreatAndSaveLog(DeleteAllCardUser);
			}
		}
	else if ( AllCardUserDeleteMgr.Status == CardUserEXIT )
		{
			GoIntoCardMenu_Init();
		}
}


/*******************************************************/
/*******************************************************/
/*******************************************************/
uint8_t AppPasscodeIdentify(uint8_t BUFF[])
{
	uint8_t i,j;
	uint8_t UserPasscodeLen;
	for (i=0;i<DEF_MAX_PASSCODEMASTER;i++)
	{
		UserPasscodeLen = 0;
		for (j=0;j<12;j++)
		{
	          if ( PasscodeMemoryMgr[i].PasscodeBuff[j] < 10)
	          {
	            UserPasscodeLen++;
	          }
	          else
	          {
	            break;
	          }
		}
		if ( pwd_decrypt_checkmode(PasscodeMemoryMgr[i].PasscodeBuff,UserPasscodeLen,BUFF,12) > 0 )
		{
                    return i+1;
		}
	}
	return 0;
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ReadPasscodeUserMemoryFromEEPROM(void)
{
	EEPROM_ReadSequential(PasscodeUserMemoryStartAddr,&PasscodeMemoryMgr[0].UserID,(14*(DEF_MAX_PASSCODEUSER+DEF_MAX_PASSCODEMASTER)));
}
void WritePasscodeUserMemoryToEEPROM(void)
{
	EEPROM_WriteSequential(PasscodeUserMemoryStartAddr,&PasscodeMemoryMgr[0].UserID,(14*(DEF_MAX_PASSCODEUSER+DEF_MAX_PASSCODEMASTER)));
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
bool_t GUI_CompareTwoPasscodes(uint8_t *BUFF1,uint8_t *BUFF2)
{
	if (  ( *BUFF1 == *BUFF2)
		&&(*(BUFF1+1) == *(BUFF2+1))
		&&(*(BUFF1+2) == *(BUFF2+2))
		&&(*(BUFF1+3) == *(BUFF2+3))
		&&(*(BUFF1+4) == *(BUFF2+4))
		&&(*(BUFF1+5) == *(BUFF2+5))
		&&(*(BUFF1+6) == *(BUFF2+6))
		&&(*(BUFF1+7) == *(BUFF2+7))
		&&(*(BUFF1+8) == *(BUFF2+8))
		&&(*(BUFF1+9) == *(BUFF2+9))
		&&(*(BUFF1+10) == *(BUFF2+10))
		&&(*(BUFF1+11) == *(BUFF2+11))
	   )
	{
		return bTRUE;
	}
	else
	{
		return bFALSE;
	}
}

/*******************************************************/
/*****Input BUFF[10]; OUTPUT 0-PasscodeIsNotRegistered, 1~255 Passcode is Registered, return value is the UserID****************/
/*******************************************************/
uint8_t PasscodeIdendify(uint8_t *BUFF1)
{
	uint8_t i,j,k;
	uint8_t PasscodeLen;
	for (i=0;i<(DEF_MAX_PASSCODEMASTER+DEF_MAX_PASSCODEUSER);i++)
	{
		if ( PasscodeMemoryMgr[i].Status != PasscodeIsValid ){
			continue;
		}

		PasscodeLen = 0;
		for (j=11;j>0;j--)
		{
			if ( PasscodeMemoryMgr[i].PasscodeBuff[j] != 0xFF )
			{
				PasscodeLen = j+1;
				break;
			}
		}
		
		for (j=0;j<(16-PasscodeLen+1);j++)
		{
			for (k=0;k<PasscodeLen;k++)
			{
			  if (
			  	  (PasscodeMemoryMgr[i].PasscodeBuff[k] != *(BUFF1+k+j))
				 )
			  	{
					break;
				}
			}
			if (k == PasscodeLen){
				return PasscodeMemoryMgr[i].UserID;
			}
		}
	}

	return 0;
}

bool_t IfPasscodeUserIDisRegistered(uint8_t UserID)
{
	uint8_t i;
	for (i=0;i<(DEF_MAX_PASSCODEUSER+DEF_MAX_PASSCODEMASTER);i++)
	{
		if (PasscodeMemoryMgr[i].UserID == UserID )
		{
			return bTRUE;
		}
	}	
	return bFALSE;
}

uint8_t CheckHowManyRegisteredPasscodeMaster( void )
{
	uint8_t i,MasterNum;
	MasterNum =0;
	for (i=0;i<(DEF_MAX_PASSCODEMASTER);i++)
	{
		if ( IfPasscodeUserIDisRegistered(i+1) == bTRUE )
		{
			MasterNum++;
		}
	}
	return MasterNum;
}

uint8_t CheckHowManyRegisteredPasscodeUser( void )
{
	uint8_t i,UserNum;
	UserNum =0;
	for (i=DEF_MAX_PASSCODEMASTER;i<(DEF_MAX_PASSCODEUSER+DEF_MAX_PASSCODEMASTER);i++)
	{
		if ( IfPasscodeUserIDisRegistered(i+1) == bTRUE )
		{
				UserNum++;
		}
	}
	return UserNum;
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void DeletePasscodeUserfromMemory(uint8_t UserID)
{
	uint8_t i,j;

	for (i=0;i<(DEF_MAX_PASSCODEUSER+DEF_MAX_PASSCODEMASTER);i++)
	{
		if (PasscodeMemoryMgr[i].UserID == UserID )
		{
			PasscodeMemoryMgr[i].Status = PasscodeIsInvalid;
			PasscodeMemoryMgr[i].UserID = 0xFF;
			for (j=0;j<12;j++)
			{
				PasscodeMemoryMgr[i].PasscodeBuff[j] = 0xFF;
			}
			break;
		}
	}	

	WritePasscodeUserMemoryToEEPROM();
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void DeleteAllPasscodeMasterfromMemory(void)
{
	uint8_t i,j;

	for (i=0;i<DEF_MAX_PASSCODEMASTER;i++)
	{

		PasscodeMemoryMgr[i].Status = PasscodeIsInvalid;
		PasscodeMemoryMgr[i].UserID = 0xFF;
		for (j=0;j<12;j++)
		{
			PasscodeMemoryMgr[i].PasscodeBuff[j] = 0xFF;
		}

	}	
	WritePasscodeUserMemoryToEEPROM();
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void DeleteAllPasscodeUserfromMemory(void)
{
	uint8_t i,j;

	for (i=DEF_MAX_PASSCODEMASTER;i<(DEF_MAX_PASSCODEMASTER+DEF_MAX_PASSCODEUSER);i++)
	{

		PasscodeMemoryMgr[i].Status = PasscodeIsInvalid;
		PasscodeMemoryMgr[i].UserID = 0xFF;
		for (j=0;j<12;j++)
		{
			PasscodeMemoryMgr[i].PasscodeBuff[j] = 0xFF;
		}

	}	
	WritePasscodeUserMemoryToEEPROM();
}

/*******************************************************/
/*****		Input BUFF[6]; 					   *********/
/*****      OUTPUT S_FAIL- MemoryIsFull, SUCCESS-be saved ****************/
/*******************************************************/
status_t SavePasscodeUserToMemory(uint8_t *Point,uint8_t UserID)
{
	uint8_t j;
/*
	for (i=0;i<DEF_MAX_PASSCODEUSER;i++)
	{
		if ( PasscodeMemoryMgr[i].Status == PasscodeIsValid)		//refer to flash default data
		{
			continue;
		}
		else
		{
			break;
		}
	}

	if ( i < DEF_MAX_PASSCODEUSER )
	{
		for (j=0;j<6;j++)
		{
			PasscodeMemoryMgr[i].PasscodeBuff[j] = *(Point+j);
		}
		PasscodeMemoryMgr[i].UserID = UserID;
		PasscodeMemoryMgr[i].Status = PasscodeIsValid;
		WritePasscodeUserMemoryToEEPROM();
		return S_SUCCESS;
	}
	else
	{
		return S_FAIL;
	}
*/
	for (j=0;j<12;j++)
	{
		PasscodeMemoryMgr[UserID-1].PasscodeBuff[j] = *(Point+j);
	}
	PasscodeMemoryMgr[UserID-1].UserID = UserID;
	PasscodeMemoryMgr[UserID-1].Status = PasscodeIsValid;
	WritePasscodeUserMemoryToEEPROM();
	return S_SUCCESS;

}

/*******************************************************/
/*******************************************************/
/*******************************************************/
uint8_t Get_Availabe_PasscodeMasterID(void)
{
	uint8_t i;
	for (i=0;i<DEF_MAX_PASSCODEMASTER;i++)
	{
		if ( PasscodeMemoryMgr[i].Status == PasscodeIsInvalid ){
			return (i+1);
		}
	}
	return 0;
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
uint8_t Get_Availabe_PasscodeUserID(void)
{
	uint8_t i;
	for (i=DEF_MAX_PASSCODEMASTER;i<(DEF_MAX_PASSCODEMASTER+DEF_MAX_PASSCODEUSER);i++)
	{
		if ( PasscodeMemoryMgr[i].Status == PasscodeIsInvalid ){
			return (i+1);
		}
	}
	return 0;
}


/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowRegisterPasscodeUser(void)
{
	uint8_t i;
	
	const uint8_t TitleStr1[]={HZ_tian,HZ_jia,HZ_guan,HZ_li,HZ_mi,HZ_ma,HZ_end};  	//添加管理密码
	const uint8_t TitleStr1En[]={"Add PW admin"};  	//添加管理密码
	const uint8_t TitleStr2[]={HZ_tian,HZ_jia,HZ_yong,HZ_hu,HZ_mi,HZ_ma,HZ_end};	//添加用户密码
	const uint8_t TitleStr2En[]={"Add PW user"};	//添加用户密码
	const uint8_t PasscodeIsBeUsedStr[]={HZ_chong,HZ_fu,HZ_mi,HZ_ma,HZ_end};
	const uint8_t PasscodeIsBeUsedStrEn[]={"Repeated PW"};


	if (PasscodeUserRegisterMgr.UserPriority == Master)
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(0,22,TitleStr1,NormalDisplay);
		}
		else{
			DisEN16x8Str(0,16,TitleStr1En,NormalDisplay);
		}
	}
	else
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(0,22,TitleStr2,NormalDisplay);
		}
		else{
			DisEN16x8Str(0,20,TitleStr2En,NormalDisplay);
		}
	}
	
	if (PasscodeUserRegisterMgr.Status == StartPasscodeUserRegister)
	{
		if ((CheckMemoryMgr.FpMasterNum == 0x00 )
			&&(PasscodeUserRegisterMgr.UserPriority == User)
			&&(CheckMemoryMgr.PasscodeMasterNum == 0x00)
			)
		{
			PasscodeUserRegisterMgr.Status = RegisterPasscodeUserFail;
			PasscodeUserRegisterMgr.ErrorType = SystemNoMaster;
			PasscodeUserRegisterMgr.TimeCnt = Def_MessageBoxTimeDelay;
			PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_PleaseAddMasterFirst);
		}
		else if ( 	((PasscodeUserRegisterMgr.UserPriority == Master)&&(!(CheckMemoryMgr.PasscodeMasterNum < DEF_MAX_PASSCODEMASTER)))
					||((PasscodeUserRegisterMgr.UserPriority == User)&&(!(CheckMemoryMgr.PasscodeUserNum < DEF_MAX_PASSCODEUSER)))
				)
		{
			PasscodeUserRegisterMgr.Status = RegisterPasscodeUserFail;
			PasscodeUserRegisterMgr.ErrorType = MemoryIsFull;
			PasscodeUserRegisterMgr.TimeCnt = Def_MessageBoxTimeDelay;
			PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_UsersAreFull);
		}
		else
		{
			PasscodeUserRegisterMgr.Status = InputPasscodeUserID;
			DataInputMgr.Status = InputIdle;
			for (i=0;i<12;i++)
			{
				PasscodeInputMgr.InputBuff[i] = 0xFF;		//Initial passcode buffer
			}
		}
	}
	else if  (PasscodeUserRegisterMgr.Status == InputPasscodeUserID)
	{
		if (PasscodeUserRegisterMgr.UserPriority == Master)
		{
			PasscodeUserRegisterMgr.UserID = Get_Availabe_PasscodeMasterID();
		}
		else
		{
			PasscodeUserRegisterMgr.UserID = Get_Availabe_PasscodeUserID();
		}
		
		if (SystemLanguage == Chinese)
		{
			DisHZ16x14Str(3,20,UserIDStr,NormalDisplay);
			DisOneDigital16x8(3,84,PasscodeUserRegisterMgr.UserID/100,NormalDisplay);
			DisOneDigital16x8(3,92,PasscodeUserRegisterMgr.UserID%100/10,NormalDisplay);
			DisOneDigital16x8(3,100,PasscodeUserRegisterMgr.UserID%10,NormalDisplay);
		}
		else
		{
			DisEN16x8Str(2,20,UserIDStrEn,NormalDisplay);
			DisOneDigital16x8(2,84,PasscodeUserRegisterMgr.UserID/100,NormalDisplay);
			DisOneDigital16x8(2,92,PasscodeUserRegisterMgr.UserID%100/10,NormalDisplay);
			DisOneDigital16x8(2,100,PasscodeUserRegisterMgr.UserID%10,NormalDisplay);
		}

		if (SystemLanguage == Chinese)
		{
			DisHZ16x14Str(6,0,PressAsteriskKeyToReturnStr,NormalDisplay);
			DisHZ16x14Str(6,76,PressPoundKeyToConfirmStr,NormalDisplay);
		}
		else
		{
			DisEN16x8Str(4,0,PressPoundKeyToConfirmStrEn,NormalDisplay);
			DisEN16x8Str(6,0,PressAsteriskKeyToReturnStrEn,NormalDisplay);
		}		
		
		PasscodeUserRegisterMgr.Status = ReportPasscodeUserID;

		VoiceReportUserIDWithUserConfirm(PasscodeUserRegisterMgr.UserID);
		
		PasscodeUserRegisterMgr.TimeCnt = Def_GuiTimeDelayCnt15s;
		
	}

	else if ( PasscodeUserRegisterMgr.Status == ReportPasscodeUserID )
	{
		if ( PasscodeUserRegisterMgr.TimeCnt-- <1 )
		{
			GoIntoPasscodeMenu_Init();
		}
	
	}
	else if  ( PasscodeUserRegisterMgr.Status == InputFirstPasscode)
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(3,24,InputPasscodeStr,NormalDisplay);
		}
		else{
			DisEN16x8Str(3,16,InputPasscodeStrEn,NormalDisplay);
		}

		GUI_PasscodeInputCreat(5,0);

		if (PasscodeInputMgr.Status == PasscodeInputEnd)
		{
			if ( PasscodeInputMgr.Point > 5 )
			{
				for (i=0;i<12;i++)
				{
					PasscodeBUFF1[i] = PasscodeInputMgr.InputBuff[i];
				}
				PasscodeUserRegisterMgr.Status = InputSecondPasscode;
				PasscodeUserRegisterMgr.TimeCnt = Def_GuiTimeDelayCnt10s;
				PasscodeInputMgr.Point = 0x00;
				PasscodeInputMgr.PasscodeLen = 12;
				PasscodeInputMgr.Status = PasscodeInputStart;
				for (i=0;i<12;i++)
				{
					PasscodeInputMgr.InputBuff[i] = 0xFF;		//Initial passcode buffer
				}
				PasscodeUserRegisterMgr.TimeCnt = Def_WaitUserInputPasscodeTimeDelay;	
				GUI_Flag_RefreshLCD = bTRUE;
				PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_PleaseInputPasswordAgain);
			}
			else
			{
				PasscodeUserRegisterMgr.Status = InputFirstPasscode;
				PasscodeInputMgr.Point = 0x00;
				PasscodeInputMgr.PasscodeLen = 12;
				PasscodeInputMgr.Status = PasscodeInputStart;
				for (i=0;i<12;i++)
				{
					PasscodeInputMgr.InputBuff[i] = 0xFF;		//Initial passcode buffer
				}
				PasscodeUserRegisterMgr.TimeCnt = Def_WaitUserInputPasscodeTimeDelay;
				PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_InputError);
				GUI_Flag_RefreshLCD = bTRUE;
			}
		}
		else if ( PasscodeInputMgr.Status == PasscodeInputExit )
		{
			PasscodeUserRegisterMgr.Status = RegisterPasscodeUserFail;
			PasscodeUserRegisterMgr.ErrorType = QUIT;
			PasscodeUserRegisterMgr.TimeCnt = 1;
		}

		if ( PasscodeUserRegisterMgr.TimeCnt > 0 )
		{
			PasscodeUserRegisterMgr.TimeCnt--;
		}
		else
		{
			PasscodeUserRegisterMgr.Status = RegisterPasscodeUserFail;
			PasscodeUserRegisterMgr.ErrorType = QUIT;
			PasscodeUserRegisterMgr.TimeCnt = 1;
		}
		
	}
	else if ( PasscodeUserRegisterMgr.Status == InputSecondPasscode )
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(3,24,ConfirmPasscode,NormalDisplay);
		}
		else{
			DisEN16x8Str(3,24,ConfirmPasscodeEn,NormalDisplay);
		}
		
		GUI_PasscodeInputCreat(5,0);

		if (PasscodeInputMgr.Status == PasscodeInputEnd)
		{
			if ( PasscodeInputMgr.Point > 5 )
				{
					PasscodeUserRegisterMgr.Status = CompareTwoPasscode;	
					GUI_Flag_RefreshLCD = bTRUE;
				}
			else
				{
					PasscodeUserRegisterMgr.Status = InputSecondPasscode;
					PasscodeInputMgr.Point = 0x00;
					PasscodeInputMgr.PasscodeLen = 12;
					PasscodeInputMgr.Status = PasscodeInputStart;
					for (i=0;i<12;i++)
					{
						PasscodeInputMgr.InputBuff[i] = 0xFF;		//Initial passcode buffer
					}
					PasscodeUserRegisterMgr.TimeCnt = Def_WaitUserInputPasscodeTimeDelay;	
					GUI_Flag_RefreshLCD = bTRUE;
				}
		}
		else if ( PasscodeInputMgr.Status == PasscodeInputExit )
		{
			PasscodeUserRegisterMgr.Status = RegisterPasscodeUserFail;
			PasscodeUserRegisterMgr.ErrorType = QUIT;
			PasscodeUserRegisterMgr.TimeCnt = 1;
		}
		
		if ( PasscodeUserRegisterMgr.TimeCnt > 0 )
		{
			PasscodeUserRegisterMgr.TimeCnt--;
		}
		else
		{
			PasscodeUserRegisterMgr.Status = RegisterPasscodeUserFail;
			PasscodeUserRegisterMgr.ErrorType = QUIT;
			PasscodeUserRegisterMgr.TimeCnt = 1;
		}

	}
	
	else if ( PasscodeUserRegisterMgr.Status == CompareTwoPasscode )
		{
			if (GUI_CompareTwoPasscodes(PasscodeBUFF1,PasscodeInputMgr.InputBuff) == bTRUE)
				{
					if (PasscodeIdendify(PasscodeInputMgr.InputBuff) == 0x00 )	//passcode is not be used
					{
						if (SavePasscodeUserToMemory(PasscodeInputMgr.InputBuff,PasscodeUserRegisterMgr.UserID) == S_SUCCESS)
						{
							PasscodeUserRegisterMgr.Status = RegisterPasscodeUserSuccess;
							PasscodeUserRegisterMgr.TimeCnt = Def_MessageBoxTimeDelay;	
							GUI_Flag_RefreshLCD = bTRUE;
							PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_OperationSuccess);
							if (PasscodeUserRegisterMgr.UserPriority == Master){
								CheckMemoryMgr.PasscodeMasterNum+=1;
							}
							else{
								CheckMemoryMgr.PasscodeUserNum+=1;
							}
							
							ComPort_SetPost_Info(DEF_WifiInfo_AddUser,PASSCODEUSER,PasscodeUserRegisterMgr.UserID);
							#if defined (Function_YouzhiyunjiaWifi) || defined (Function_TuyaWifi)
							Wifi_PostEvent(DEF_WifiEvent_AddUser,PASSCODEUSER,PasscodeUserRegisterMgr.UserID);
							#endif
							
						}
						else
						{
							PasscodeUserRegisterMgr.Status = RegisterPasscodeUserFail;
							PasscodeUserRegisterMgr.ErrorType = MemoryIsFull;
							PasscodeUserRegisterMgr.TimeCnt = Def_MessageBoxTimeDelay;	
							GUI_Flag_RefreshLCD = bTRUE;
							PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_UsersAreFull);
						}
					}
					else
					{
						PasscodeUserRegisterMgr.Status = RegisterPasscodeUserFail;
						PasscodeUserRegisterMgr.ErrorType = PasscodeIsBeUsed;
						PasscodeUserRegisterMgr.TimeCnt = Def_MessageBoxTimeDelay;	
						GUI_Flag_RefreshLCD = bTRUE;
						PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_RepeatedPassword);
					}
				}
			else{
					PasscodeUserRegisterMgr.Status = RegisterPasscodeUserFail;
					PasscodeUserRegisterMgr.ErrorType = TwoPasscodesDoNotMatch;
					PasscodeUserRegisterMgr.TimeCnt = Def_MessageBoxTimeDelay;	
					GUI_Flag_RefreshLCD = bTRUE;
					PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_InputError);
				}

			if ( PasscodeUserRegisterMgr.Status == RegisterPasscodeUserSuccess )
			{
				GUI_SetFPM_LED(DEF_FpmLedMode_Breath,DEF_FpmLedColor_Green,DEF_FpmLedColor_Green,255);
			}
			else if (PasscodeUserRegisterMgr.Status == RegisterPasscodeUserFail)
			{
				GUI_SetFPM_LED(DEF_FpmLedMode_Breath,DEF_FpmLedColor_Red,DEF_FpmLedColor_Red,255);
			}
		}

	else if ( PasscodeUserRegisterMgr.Status ==  RegisterPasscodeUserSuccess )
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(4,36,OperationSuccessStr,NormalDisplay);
		}
		else{
			DisEN16x8Str(4,36,OperationSuccessStrEn,NormalDisplay);
		}
		if ( PasscodeUserRegisterMgr.TimeCnt-- < 1 )
		{
			if ((CheckMemoryMgr.FpMasterNum == 0x00 )
			&&(PasscodeUserRegisterMgr.UserPriority == Master)
			&&(CheckMemoryMgr.PasscodeMasterNum == 0x01)
			)
			{
				ManagerIdentifyMgr.MasterType = PASSCODEUSER;
				ManagerIdentifyMgr.PasscodeUserID = 0X01;	
				GoIntoMainMenu_Init();
			}
			else
			{
				PasscodeUserRegisterMgr.Status = StartPasscodeUserRegister;
			}
			GUI_SetFPM_LED(DEF_FpmLedMode_Breath,DEF_FpmLedColor_Blue,DEF_FpmLedColor_Blue,255);
			GUI_Flag_RefreshLCD = bTRUE;
			GUI_CreatAndSaveLog(AddPasscodeUser);
		}
	}
	else if ( PasscodeUserRegisterMgr.Status == RegisterPasscodeUserFail )
	{
		if ( PasscodeUserRegisterMgr.ErrorType == SystemNoMaster )
		{
			Clear_Screen();
			DisImage(2,50,27,24,Icon_Warning,NormalDisplay);
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(6,14,PleaseAddMasterStr,NormalDisplay);
			}
			else{
				DisEN16x8Str(6,10,PleaseAddMasterStrEn,NormalDisplay);
			}
		}
		else if ( PasscodeUserRegisterMgr.ErrorType == PasscodeIsBeUsed )
		{
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(3,36,PasscodeIsBeUsedStr,NormalDisplay);
			}
			else{
				DisEN16x8Str(3,16,PasscodeIsBeUsedStrEn,NormalDisplay);
			}
		}
		else if ( PasscodeUserRegisterMgr.ErrorType == MemoryIsFull )
		{
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(4,32,UserIsFullStr,NormalDisplay);
			}
			else{
				DisEN16x8Str(4,4,UserIsFullStrEn,NormalDisplay);
			}
		}
		else if  ( PasscodeUserRegisterMgr.ErrorType == TwoPasscodesDoNotMatch )
		{
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(4,34,InputErrorStr,NormalDisplay);
			}
			else{
				DisEN16x8Str(4,20,InputErrorStrEn,NormalDisplay);
			}
		}
		else if ( PasscodeUserRegisterMgr.ErrorType == QUIT )
		{
		
		}
		else
		{
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(4,36,OperationFailStr,NormalDisplay);
			}
			else{
				DisEN16x8Str(4,48,OperationFailStrEn,NormalDisplay);
			}
		}
	
		if ( PasscodeUserRegisterMgr.TimeCnt-- < 1 )
		{
			if 	( 	(PasscodeUserRegisterMgr.ErrorType == QUIT )
					&&(CheckMemoryMgr.FpMasterNum == 0x00 )
					&&(CheckMemoryMgr.PasscodeMasterNum == 0x00)
				)
			{
				GoIntoMainScreen_WithIdentifyInit();
			}
			else if ((CheckMemoryMgr.FpMasterNum == 0x00 )
			&&(PasscodeUserRegisterMgr.UserPriority == Master)
			&&(CheckMemoryMgr.PasscodeMasterNum == 0x00)
			)
			{
				GoIntoMainScreen_WithIdentifyInit();
			}
			else if ( (PasscodeUserRegisterMgr.ErrorType == MemoryIsFull )
				||(PasscodeUserRegisterMgr.ErrorType == QUIT )
				||( PasscodeUserRegisterMgr.ErrorType == SystemNoMaster )
				)
			{
				GoIntoPasscodeMenu_Init();
			}
			else
			{
				PasscodeUserRegisterMgr.Status = StartPasscodeUserRegister;
				GUI_Flag_RefreshLCD = bTRUE;
			}
			GUI_SetFPM_LED(DEF_FpmLedMode_Breath,DEF_FpmLedColor_Blue,DEF_FpmLedColor_Blue,255);
		}
	}
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowDeletePasscodeUser(void)
{
	//uint8_t i;
	const uint8_t TitleStr2[]={HZ_shan,HZ_chufa,HZ_mi,HZ_ma,HZ_end};  	//删除用户密码
	const uint8_t TitleStr2En[]={"Delete Password"};  	//删除用户密码


	if (SystemLanguage == Chinese){
		DisHZ16x14Str(0,36,TitleStr2,NormalDisplay);
	}
	else{
		DisEN16x8Str(0,4,TitleStr2En,NormalDisplay);
	}

	
	if ( PasscodeUserDeleteMgr.Status == StartPasscodeUserDelete )
	{
		PasscodeUserDeleteMgr.Status = InputPasscodeUserID;
		DataInputMgr.Status = InputIdle;
		PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_PleaseInputID);
	}
	else if ( PasscodeUserDeleteMgr.Status == InputPasscodeUserID )
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(3,28,InputUserIDStr,NormalDisplay);
		}
		else{
			DisEN16x8Str(3,20,InputUserIDStrEn,NormalDisplay);
		}
		
		GUI_DataInputCreat(6,52,2,0x0000);

		if (DataInputMgr.Status == InputEnd)
		{
			PasscodeUserDeleteMgr.UserID = DataInputMgr.Value;
			PasscodeUserDeleteMgr.Status = ReportFpUserID;
			PasscodeUserDeleteMgr.TimeCnt = Def_GuiTimeDelayCnt4s;
			DataInputMgr.Status = InputIdle;	
			VoiceReportUserID(PasscodeUserDeleteMgr.UserID);
		}
		else if  (DataInputMgr.Status == InputExit)
		{
			PasscodeUserDeleteMgr.Status = DeletePasscodeUserFail;
			PasscodeUserDeleteMgr.ErrorType = QUIT;
			PasscodeUserDeleteMgr.TimeCnt = 0;
		}
	}
	else if ( PasscodeUserDeleteMgr.Status == ReportFpUserID )
	{
		if (--PasscodeUserDeleteMgr.TimeCnt < 1 )
		{
			PasscodeUserDeleteMgr.Status = CheckIfPasscodeUserIDisRegistered;
		}
	}
	else if ( PasscodeUserDeleteMgr.Status == CheckIfPasscodeUserIDisRegistered)
	{
		if ( IfPasscodeUserIDisRegistered(PasscodeUserDeleteMgr.UserID) == bTRUE )
		{
			if ( (ManagerIdentifyMgr.MasterType == PASSCODEUSER )
			&&(ManagerIdentifyMgr.PasscodeUserID == PasscodeUserDeleteMgr.UserID)
			)
			{
				PasscodeUserDeleteMgr.Status = DeletePasscodeUserFail;
				PasscodeUserDeleteMgr.ErrorType = ManagerHasLogin;
				PasscodeUserDeleteMgr.TimeCnt = Def_GuiTimeDelayCnt3s;
				GUI_Flag_RefreshLCD = bTRUE;
				//PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_DeleteFail);
				PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_AdminIsLoginCannotbeDelete);
			}
			else
			{
				DeletePasscodeUserfromMemory(PasscodeUserDeleteMgr.UserID);
				PasscodeUserDeleteMgr.Status = DeletePasscodeUserSuccess;	
				PasscodeUserDeleteMgr.TimeCnt = Def_MessageBoxTimeDelay; 	
				GUI_Flag_RefreshLCD = bTRUE;
				PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_DeleteSuccess);

				if ( PasscodeUserDeleteMgr.UserID <= DEF_MAX_PASSCODEMASTER )
				{
					if ( CheckMemoryMgr.PasscodeMasterNum > 0 )
					{
						CheckMemoryMgr.PasscodeMasterNum-=1;
					}
				}
				else
				{
					if ( CheckMemoryMgr.PasscodeUserNum > 0 )
					{
						CheckMemoryMgr.PasscodeUserNum-=1;
					}
				}
				
				ComPort_SetPost_Info(DEF_WifiInfo_DeleteUser,PASSCODEUSER,PasscodeUserDeleteMgr.UserID);
				
				#if defined (Function_YouzhiyunjiaWifi) || defined (Function_TuyaWifi)
				Wifi_PostEvent(DEF_WifiEvent_DeleteUser,PASSCODEUSER,PasscodeUserDeleteMgr.UserID);
				#endif
			}
				
		}
		else
			{
				PasscodeUserDeleteMgr.Status = DeletePasscodeUserFail;
				PasscodeUserDeleteMgr.ErrorType = UserIDisNotRegistered;
				PasscodeUserDeleteMgr.TimeCnt = Def_MessageBoxTimeDelay; 	
				GUI_Flag_RefreshLCD = bTRUE;
				PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_UserIdIsNotExist);
			}
	}
	else if  ( PasscodeUserDeleteMgr.Status == DeletePasscodeUserSuccess)
		{
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(4,36,OperationSuccessStr,NormalDisplay);
			}
			else{
				DisEN16x8Str(4,36,OperationSuccessStrEn,NormalDisplay);
			}

			if (--PasscodeUserDeleteMgr.TimeCnt < 1 )
				{
					PasscodeUserDeleteMgr.Status = StartPasscodeUserDelete;
					GUI_Flag_RefreshLCD = bTRUE;
					UnlockModeAutoCalculate();
					GUI_CreatAndSaveLog(DeletePasscodeUser);
				}
		}
	else if ( PasscodeUserDeleteMgr.Status == DeletePasscodeUserFail)
		{
			if ( PasscodeUserDeleteMgr.ErrorType == UserIDisNotRegistered)
			{
				if (SystemLanguage == Chinese){
					DisHZ16x14Str(3,15,IDisNotRegisteredStr,NormalDisplay);
				}
				else{
					DisEN16x8Str(3,12,IDisNotRegisteredStrEn,NormalDisplay);
				}
			}
			else if ( PasscodeUserDeleteMgr.ErrorType == ManagerHasLogin )
			{
				if (SystemLanguage == Chinese){
					DisHZ16x14Str(3,15,CanNotDeleteLastOneMasterStr1,NormalDisplay);
					DisHZ16x14Str(5,32,CanNotDeleteLastOneMasterStr2,NormalDisplay);
				}
				else
				{
					DisEN16x8Str(3,4,CanNotDeleteLastOneMasterStr1En,NormalDisplay);
					DisEN16x8Str(5,16,CanNotDeleteLastOneMasterStr2En,NormalDisplay);
				}
			}
			else
			{
				if (SystemLanguage == Chinese){
					DisHZ16x14Str(6,36,OperationFailStr,NormalDisplay);
				}
				else{
					DisEN16x8Str(6,48,OperationFailStrEn,NormalDisplay);
				}
			}
			
			if (PasscodeUserDeleteMgr.TimeCnt-- < 1 )
			{
				if ( PasscodeUserDeleteMgr.ErrorType == QUIT )
				{
					GoIntoPasscodeMenu_Init();
				}
				else
				{
					//PasscodeUserDeleteMgr.Status = StartPasscodeUserDelete;
					//GUI_Flag_RefreshLCD = bTRUE;
					GoIntoPasscodeMenu_Init();
				}
			}
		}
	
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowDeleteAllPasscodeUser(void)
{
	//uint8_t i;
	
	if ( AllPasscodeUserDeleteMgr.Status == StartAllPasscodeUserDelete )
	{
		AllPasscodeUserDeleteMgr.Status = WaitForDeleteAllPasscodeUserConfirm;
		AllPasscodeUserDeleteMgr.Selection = NO;
		if (SystemLanguage == Chinese)
		{
			DisHZ16x14Str(2,16,ConfirmDeleteStr,NormalDisplay);
			DisHZ16x14Str(4,16,AbortDeleteStr,NormalDisplay);
		}
		else
		{
			DisEN16x8Str(2,0,ConfirmDeleteStrEn,NormalDisplay);
			DisEN16x8Str(4,0,AbortDeleteStrEn,NormalDisplay);
		}	
		PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_ConfirmOrExitDelete);
	}
	else if ( AllPasscodeUserDeleteMgr.Status == WaitForDeleteAllPasscodeUserConfirm )
	{
		
	}
    else if ( AllPasscodeUserDeleteMgr.Status == DeletingAllPasscodeUser )
		{
			
			DeleteAllPasscodeUserfromMemory();
			AllPasscodeUserDeleteMgr.TimeCnt = Def_MessageBoxTimeDelay;
			AllPasscodeUserDeleteMgr.Status = DeleteAllPasscodeUserSuccess;
			GUI_Flag_RefreshLCD = bTRUE;
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_DeleteSuccess);
			CheckMemoryMgr.PasscodeUserNum = 0;
		}
	else if ( AllPasscodeUserDeleteMgr.Status == DeleteAllPasscodeUserSuccess )
		{
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(4,36,OperationSuccessStr,NormalDisplay);
			}
			else{
				DisEN16x8Str(4,36,OperationSuccessStrEn,NormalDisplay);
			}
			if (AllPasscodeUserDeleteMgr.TimeCnt-- < 1 )
			{
				GoIntoPasscodeMenu_Init();
				UnlockModeAutoCalculate();
				GUI_CreatAndSaveLog(DeleteAllPasscodeUser);
			}
		}
	else if  ( AllPasscodeUserDeleteMgr.Status == PasscodeUserEXIT )
		{
			GoIntoPasscodeMenu_Init();
		}
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowTimeSetting(void)
{
	//uint8_t i;
	uint8_t  const TitleStr[]={HZ_shi,HZ_jian,HZ_she,HZ_ding,HZ_end};  						//时间设定
	uint8_t  const TitleStrEn[]={"Time Setting"};  						//时间设定	
	
 	uint8_t  const YearMonthDayStrEn[]={"2019-01-01"};
	
	uint8_t  const TimeStrEn[]={"23:01:01"};

	if (SystemLanguage == Chinese){
		DisHZ16x14Str(0,28,TitleStr,NormalDisplay);
	}
	else{
		DisEN16x8Str(0,12,TitleStrEn,NormalDisplay);
	}
	
	DisEN16x8Str(3,24,YearMonthDayStrEn,NormalDisplay);
	DisBcdDigital16x8(3,40,TempSystemTime.year,NormalDisplay);
	DisBcdDigital16x8(3,64,TempSystemTime.month,NormalDisplay);
	DisBcdDigital16x8(3,88,TempSystemTime.date,NormalDisplay);
	
	DisEN16x8Str(6,24,TimeStrEn,NormalDisplay);
	DisBcdDigital16x8(6,24,TempSystemTime.hour,NormalDisplay);
	DisBcdDigital16x8(6,48,TempSystemTime.minute,NormalDisplay);
	DisBcdDigital16x8(6,72,TempSystemTime.second,NormalDisplay);
	

	if ( (TimeSettingMgr.Status == SetYearMajor)&&(GUI_ToggleFlag_05s==0x01) )
	{
		DisOneDigital16x8(3,40,TempSystemTime.year>>4,InverseDisplay);				
	}
	else if ( (TimeSettingMgr.Status == SetYearMinor)&&(GUI_ToggleFlag_05s==0x01) )
	{
		DisOneDigital16x8(3,48,TempSystemTime.year&0x0F,InverseDisplay);	
	}
	else if ( (TimeSettingMgr.Status == SetMonthMajor)&&(GUI_ToggleFlag_05s==0x01) )
	{
		DisOneDigital16x8(3,64,TempSystemTime.month>>4,InverseDisplay);		
	}
	else if ( (TimeSettingMgr.Status == SetMonthMinor)&&(GUI_ToggleFlag_05s==0x01) )
	{
		DisOneDigital16x8(3,72,TempSystemTime.month&0x0F,InverseDisplay);	
	}
	else if ( (TimeSettingMgr.Status == SetDateMajor)&&(GUI_ToggleFlag_05s==0x01) )
	{
		DisOneDigital16x8(3,88,TempSystemTime.date>>4,InverseDisplay);		
	}
	else if ( (TimeSettingMgr.Status == SetDateMinor)&&(GUI_ToggleFlag_05s==0x01) )
	{
		DisOneDigital16x8(3,96,TempSystemTime.date&0x0F,InverseDisplay);	
	}
	
	else if ( (TimeSettingMgr.Status == SetHourMajor)&&(GUI_ToggleFlag_05s==0x01) )
	{
		DisOneDigital16x8(6,24,TempSystemTime.hour>>4,InverseDisplay);		
	}
	else if ( (TimeSettingMgr.Status == SetHourMinor)&&(GUI_ToggleFlag_05s==0x01) )
	{
		DisOneDigital16x8(6,32,TempSystemTime.hour&0x0F,InverseDisplay);	
	}
	else if ( (TimeSettingMgr.Status == SetMinuteMajor)&&(GUI_ToggleFlag_05s==0x01) )
	{
		DisOneDigital16x8(6,48,TempSystemTime.minute>>4,InverseDisplay);		
	}
	else if ( (TimeSettingMgr.Status == SetMinuteMinor)&&(GUI_ToggleFlag_05s==0x01) )
	{
		DisOneDigital16x8(6,56,TempSystemTime.minute&0x0F,InverseDisplay);		
	}
	else if ( (TimeSettingMgr.Status == SetSecondMajor)&&(GUI_ToggleFlag_05s==0x01) )
	{
		DisOneDigital16x8(6,72,TempSystemTime.second>>4,InverseDisplay);		
	}
	else if ( (TimeSettingMgr.Status == SetSecondMinor)&&(GUI_ToggleFlag_05s==0x01) )
	{
		DisOneDigital16x8(6,80,TempSystemTime.second&0x0F,InverseDisplay);	
	}

	
}


/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowLowBattery(void)
{
	if ( SystemLanguage == Chinese ){
		DisImage(0,50,27,24,Icon_Warning,NormalDisplay);
		DisImage(5,4,120,24,Icon_ReplaceBattery,NormalDisplay);
	}
	else{
		DisImage(0,50,27,24,Icon_Warning,NormalDisplay);
		DisEN16x8Str(4,0,ReplaceBatteryStr1En,NormalDisplay); 
		DisEN16x8Str(6,0,ReplaceBatteryStr2En,NormalDisplay); 
	}
	
	if ( BatteryMgr.LowBatteryProtectionEnabled == bFALSE)
	{
		if (--BatteryMgr.TimeCnt < 1 )
		{
			GoIntoMainScreen_WithIdentifyInit();
			if ( IfSystemIsInFactoryDefaultStatus()==bTRUE )
			{
				PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_PleaseAddMasterFirst);
			}
			else
			{
				//PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_PleasePutFingerOrCardOrPasscode);
			}
		}
	}
}


/*******************************************************/
/*******************************************************/
/*******************************************************/
void VoiceReportCurrentLanguage(void)
{
	uint16_t VoiceStr[5];
	if ( SystemLanguage == Chinese )
	{
		VoiceStr[0] = VOICE_Current;
		VoiceStr[1] = VOICE_Setting;
		VoiceStr[2] = VOICE_Chinese;
	}
	else
	{
		VoiceStr[0] = VOICE_Current+1;
		VoiceStr[1] = VOICE_Setting+1;
		VoiceStr[2] = VOICE_English;
	}
	VoiceStr[3] = DEF_VoiceSegmentEndFlag;
	PLAY_VOICE_MULTISEGMENTS_IGNORELANGUAGE(VoiceMgr.volume,VoiceStr);
}

void GoIntoLanguageSetting_Init(void)
{
	VoiceMenuMgr.MenuPoint = 0;
	VoiceMenuMgr.TotalMenuNum = 4;
	CurrentScreen = SCREEN_LanguageSetting;
}

void ShowLanguageSetting(void)
{
	const uint8_t LanguageStr[]={ZF_1,ZF_xiaoshudian,HZ_zhong,HZ_wenzi,HZ_end};
	const uint8_t LanguageStrEn[]={"2.English"};

	if ( SystemLanguage == Chinese )
	{
		DisHZ16x14Str(0,4,LanguageStr,InverseDisplay);	
		DisEN16x8Str(2,4,LanguageStrEn,NormalDisplay);
	}
	else{
		DisHZ16x14Str(0,4,LanguageStr,NormalDisplay);	
		DisEN16x8Str(2,4,LanguageStrEn,InverseDisplay);
	}

	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < 2 )
		{
			PLAY_VOICE_MULTISEGMENTS_IGNORELANGUAGE(VoiceMgr.volume,LanguageSetVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
		else if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,LanguageSetVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
/*******************************************************/
/*******************************************************/
/*******************************************************/
void VoiceReportCurrentVolume(void)
{
	uint16_t VoiceStr[5];
	VoiceStr[0] = VOICE_Current;
	VoiceStr[1] = VOICE_Setting;
	if ( VoiceMgr.volume == 0 )
	{
		VoiceStr[2] = VOICE_Mute;
		VoiceStr[3] = VOICE_Mute20ms;
	}	
	else if ( VoiceMgr.volume == 1 )
	{
		VoiceStr[2] = VOICE_Small;
		VoiceStr[3] = VOICE_Volume;
	}
	else if ( VoiceMgr.volume == 2 )
	{
		VoiceStr[2] = VOICE_Middle;
		VoiceStr[3] = VOICE_Volume;
	}
	else
	{
		VoiceStr[2] = VOICE_Big;
		VoiceStr[3] = VOICE_Volume;
	}
	VoiceStr[4] = DEF_VoiceSegmentEndFlag;
	PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr);
}

void GoIntoVolumeSetting_Init(void)
{
	VoiceMenuMgr.MenuPoint = 0;
	VoiceMenuMgr.TotalMenuNum = 6;
	CurrentScreen = SCREEN_VolumeSetting;
}

void ShowVolumeSetting(void)
{
	uint8_t i;
	
	for (i=0;i<4;i++)
	{
		if (SystemLanguage == Chinese)
		{
			if ( VoiceMgr.volume == (3-i))
			{
				DisHZ16x14Str(2*i,4,VolumeAdjustStr[i],InverseDisplay);
			}
			else
			{
				DisHZ16x14Str(2*i,4,VolumeAdjustStr[i],NormalDisplay);
			}
		}
		else
		{
			if ( VoiceMgr.volume == (3-i))
			{
				DisEN16x8Str(2*i,4,VolumeAdjustStrEn[i],InverseDisplay);
			}
			else
			{
				DisEN16x8Str(2*i,4,VolumeAdjustStrEn[i],NormalDisplay);
			}
		}
	}

	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VolumeSetVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}

	if ( VoiceMgr.volume > 0 ){
		VoiceMgr.Enable = bTRUE;
	}
	else{
		VoiceMgr.Enable = bFALSE;
	}
	
}

/*******************************************************/
/*******************************************************/
/*******************************************************/

void GUI_GetUserNumList(void)
{

	if ( CheckMemoryMgr.Status == StartCheckMemory)
	{
		CheckHomManyRegisteredFPuser.Status = StartCheckHowManyRegisteredFPuser;
		CheckHomManyRegisteredFPuser.FailTimes = 0x00;
		CheckMemoryMgr.Status = WaitForReadFPuserNum;
	}
	else if ( CheckMemoryMgr.Status == WaitForReadFPuserNum )
	{
		CheckHowManyRegisteredFPuserFromFPM();
		if ( CheckHomManyRegisteredFPuser.Status == CheckHomManyRegisteredFPuserSuccess )
		{
		
			CheckMemoryMgr.FpUserNum = CheckHomManyRegisteredFPuser.UserNum;
			CheckMemoryMgr.StressFpUserNum = CheckHomManyRegisteredFPuser.StressUserNum;
			CheckMemoryMgr.FpMasterNum = CheckHomManyRegisteredFPuser.MasterNum;
			CheckMemoryMgr.CardUserNum = CheckHowManyRegisteredCardUser();
			CheckMemoryMgr.PasscodeMasterNum = CheckHowManyRegisteredPasscodeMaster();
			CheckMemoryMgr.PasscodeUserNum = CheckHowManyRegisteredPasscodeUser();
			CheckMemoryMgr.FaceMasterNum = CheckHowManyRegisteredFaceMaster();
			CheckMemoryMgr.FaceUserNum = CheckHowManyRegisteredFaceUser();
			CheckMemoryMgr.Status = CheckMemorySuccess;
		}
		else if ( CheckHomManyRegisteredFPuser.Status == CheckHomManyRegisteredFPuserFail )
		{
			if ( CheckHomManyRegisteredFPuser.FailTimes++ < 3 )
			{
				CheckHomManyRegisteredFPuser.Status = StartCheckHowManyRegisteredFPuser;//retry
			}
			else
			{
				ReadRegisteredFPuserFromLocalList();
				CheckMemoryMgr.FpUserNum = CheckHomManyRegisteredFPuser.UserNum;
				CheckMemoryMgr.StressFpUserNum = CheckHomManyRegisteredFPuser.StressUserNum;
				CheckMemoryMgr.FpMasterNum = CheckHomManyRegisteredFPuser.MasterNum;
				CheckMemoryMgr.CardUserNum = CheckHowManyRegisteredCardUser();
				CheckMemoryMgr.PasscodeMasterNum = CheckHowManyRegisteredPasscodeMaster();
				CheckMemoryMgr.PasscodeUserNum = CheckHowManyRegisteredPasscodeUser();
				CheckMemoryMgr.FaceMasterNum = CheckHowManyRegisteredFaceMaster();
				CheckMemoryMgr.FaceUserNum = CheckHowManyRegisteredFaceUser();
				CheckMemoryMgr.Status = CheckMemoryFail;
			}
		}
	} 
	
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void GoIntoMemoryUsageScreen()
{
	uint8_t TempValue;
	CurrentScreen = SCREEN_MemoryUsage;

	if ( FrmMgr.FrmFunctionConfig == FRM_Enabled )
		{
	VoiceMenuMgr.MenuPoint = 0;
	VoiceMenuMgr.TotalMenuNum = 6;	
		}
	else{
	VoiceMenuMgr.MenuPoint = 1;
	VoiceMenuMgr.TotalMenuNum = 6;	
		}
	
	MemoryUsageVoiceBuff[0][2]=TranslateNumberToVoice((CheckMemoryMgr.FaceMasterNum+CheckMemoryMgr.FaceUserNum)/100);
	MemoryUsageVoiceBuff[0][3]=TranslateNumberToVoice((CheckMemoryMgr.FaceMasterNum+CheckMemoryMgr.FaceUserNum)%100/10);
	MemoryUsageVoiceBuff[0][4]=TranslateNumberToVoice((CheckMemoryMgr.FaceMasterNum+CheckMemoryMgr.FaceUserNum)%10);
	TempValue = DEF_MAX_FACEMASTER+DEF_MAX_FACEUSER - (CheckMemoryMgr.FaceMasterNum+CheckMemoryMgr.FaceUserNum);
	MemoryUsageVoiceBuff[0][6]=TranslateNumberToVoice((TempValue)/100);
	MemoryUsageVoiceBuff[0][7]=TranslateNumberToVoice((TempValue)%100/10);
	MemoryUsageVoiceBuff[0][8]=TranslateNumberToVoice((TempValue)%10);

	MemoryUsageVoiceBuff[1][2]=TranslateNumberToVoice((CheckMemoryMgr.FpMasterNum+CheckMemoryMgr.FpUserNum+CheckMemoryMgr.StressFpUserNum)/100);
	MemoryUsageVoiceBuff[1][3]=TranslateNumberToVoice((CheckMemoryMgr.FpMasterNum+CheckMemoryMgr.FpUserNum+CheckMemoryMgr.StressFpUserNum)%100/10);
	MemoryUsageVoiceBuff[1][4]=TranslateNumberToVoice((CheckMemoryMgr.FpMasterNum+CheckMemoryMgr.FpUserNum+CheckMemoryMgr.StressFpUserNum)%10);
	TempValue = DEF_MAX_FPMASTER+DEF_MAX_FPUSER+DEF_MAX_STRESSFPUSER - (CheckMemoryMgr.FpMasterNum+CheckMemoryMgr.FpUserNum+CheckMemoryMgr.StressFpUserNum);
	MemoryUsageVoiceBuff[1][6]=TranslateNumberToVoice((TempValue)/100);
	MemoryUsageVoiceBuff[1][7]=TranslateNumberToVoice((TempValue)%100/10);
	MemoryUsageVoiceBuff[1][8]=TranslateNumberToVoice((TempValue)%10);


	MemoryUsageVoiceBuff[2][2]=TranslateNumberToVoice((CheckMemoryMgr.PasscodeMasterNum+CheckMemoryMgr.PasscodeUserNum)/10);
	MemoryUsageVoiceBuff[2][3]=TranslateNumberToVoice((CheckMemoryMgr.PasscodeMasterNum+CheckMemoryMgr.PasscodeUserNum)%10);
	TempValue = DEF_MAX_PASSCODEMASTER+DEF_MAX_PASSCODEUSER - (CheckMemoryMgr.PasscodeMasterNum+CheckMemoryMgr.PasscodeUserNum);
	MemoryUsageVoiceBuff[2][5]=TranslateNumberToVoice((TempValue)/10);
	MemoryUsageVoiceBuff[2][6]=TranslateNumberToVoice((TempValue)%10);


	
	MemoryUsageVoiceBuff[3][2]=TranslateNumberToVoice(CheckMemoryMgr.CardUserNum/100);
	MemoryUsageVoiceBuff[3][3]=TranslateNumberToVoice(CheckMemoryMgr.CardUserNum%100/10);
	MemoryUsageVoiceBuff[3][4]=TranslateNumberToVoice(CheckMemoryMgr.CardUserNum%10);
	TempValue = DEF_MAX_CARDUSER - CheckMemoryMgr.CardUserNum;
	MemoryUsageVoiceBuff[3][6]=TranslateNumberToVoice(TempValue/100);
	MemoryUsageVoiceBuff[3][7]=TranslateNumberToVoice(TempValue%100/10);
	MemoryUsageVoiceBuff[3][8]=TranslateNumberToVoice(TempValue%10);
	
	
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowMemoryUsage(void)
{

	const uint8_t Str0[]={HZ_ren,HZ_lianbu,ZF_maohao,HZ_end};				//指纹：/
	const uint8_t Str0En[]={"FACE:"}; 			//指纹：

	const uint8_t Str1[]={HZ_zhi,HZ_wen,ZF_maohao,HZ_end};				//指纹：/
	const uint8_t Str1En[]={"FP:"};				//指纹：
	
	const uint8_t Str3[]={HZ_mi,HZ_ma,ZF_maohao,HZ_end};	//密码：
	const uint8_t Str3En[]={"PW:"};					//密码：


	const uint8_t Str5[]={HZ_ka,HZ_pian,ZF_maohao,HZ_end};	//卡片：/
	const uint8_t Str5En[]={"CD:"};	//卡片用户：

	uint8_t TempValue;

	GUI_Flag_RefreshLCD = bTRUE;

	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,MemoryUsageVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}

	if ( FrmMgr.FrmFunctionConfig == FRM_Enabled )
	{
		if (SystemLanguage == Chinese)
		{
			DisHZ16x14Str(0,0,Str0,NormalDisplay);
		}
		else
		{
			DisEN16x8Str(0,0,Str0En,NormalDisplay);
		}
		DisOneDigital16x8(0,48,(CheckMemoryMgr.FaceMasterNum+CheckMemoryMgr.FaceUserNum)/100,NormalDisplay);
		DisOneDigital16x8(0,56,(CheckMemoryMgr.FaceMasterNum+CheckMemoryMgr.FaceUserNum)%100/10,NormalDisplay);
		DisOneDigital16x8(0,64,(CheckMemoryMgr.FaceMasterNum+CheckMemoryMgr.FaceUserNum)%10,NormalDisplay);
		DisEN16x8Str(0,72,"/",NormalDisplay);
		TempValue = DEF_MAX_FACEMASTER+DEF_MAX_FACEUSER - (CheckMemoryMgr.FaceMasterNum+CheckMemoryMgr.FaceUserNum);
		DisOneDigital16x8(0,80,TempValue/100,NormalDisplay);
		DisOneDigital16x8(0,88,TempValue%100/10,NormalDisplay);
		DisOneDigital16x8(0,96,TempValue%10,NormalDisplay);
	}
	
	if (SystemLanguage == Chinese)
	{
		DisHZ16x14Str(2,0,Str1,NormalDisplay);
	}
	else
	{
		DisEN16x8Str(2,0,Str1En,NormalDisplay);
	}
	DisOneDigital16x8(2,48,(CheckMemoryMgr.FpMasterNum+CheckMemoryMgr.FpUserNum+CheckMemoryMgr.StressFpUserNum)/100,NormalDisplay);
	DisOneDigital16x8(2,56,(CheckMemoryMgr.FpMasterNum+CheckMemoryMgr.FpUserNum+CheckMemoryMgr.StressFpUserNum)%100/10,NormalDisplay);
	DisOneDigital16x8(2,64,(CheckMemoryMgr.FpMasterNum+CheckMemoryMgr.FpUserNum+CheckMemoryMgr.StressFpUserNum)%10,NormalDisplay);
	DisEN16x8Str(2,72,"/",NormalDisplay);
	TempValue = DEF_MAX_FPMASTER+DEF_MAX_FPUSER+DEF_MAX_STRESSFPUSER - (CheckMemoryMgr.FpMasterNum+CheckMemoryMgr.FpUserNum+CheckMemoryMgr.StressFpUserNum);
	DisOneDigital16x8(2,80,TempValue/100,NormalDisplay);
	DisOneDigital16x8(2,88,TempValue%100/10,NormalDisplay);
	DisOneDigital16x8(2,96,TempValue%10,NormalDisplay);


	if (SystemLanguage == Chinese)
	{
		DisHZ16x14Str(4,0,Str3,NormalDisplay);
	}
	else
	{
		DisEN16x8Str(4,0,Str3En,NormalDisplay);
	}

	DisOneDigital16x8(4,48,(CheckMemoryMgr.PasscodeMasterNum+CheckMemoryMgr.PasscodeUserNum)/10,NormalDisplay);
	DisOneDigital16x8(4,56,(CheckMemoryMgr.PasscodeMasterNum+CheckMemoryMgr.PasscodeUserNum)%10,NormalDisplay);
	DisEN16x8Str(4,64,"/",NormalDisplay);
	TempValue = DEF_MAX_PASSCODEMASTER+DEF_MAX_PASSCODEUSER - (CheckMemoryMgr.PasscodeMasterNum+CheckMemoryMgr.PasscodeUserNum);
	DisOneDigital16x8(4,72,TempValue/10,NormalDisplay);
	DisOneDigital16x8(4,80,TempValue%10,NormalDisplay);
	

	GUI_Flag_RefreshLCD = bTRUE;		
	if (SystemLanguage == Chinese)
	{
		DisHZ16x14Str(6,0,Str5,NormalDisplay);
	}
	else
	{
		DisEN16x8Str(6,0,Str5En,NormalDisplay);
	}
	DisOneDigital16x8(6,48,CheckMemoryMgr.CardUserNum/100,NormalDisplay);
	DisOneDigital16x8(6,56,CheckMemoryMgr.CardUserNum%100/10,NormalDisplay);
	DisOneDigital16x8(6,64,CheckMemoryMgr.CardUserNum%10,NormalDisplay);
	DisEN16x8Str(6,72,"/",NormalDisplay);
	TempValue = DEF_MAX_CARDUSER - CheckMemoryMgr.CardUserNum;
	DisOneDigital16x8(6,80,TempValue/100,NormalDisplay);
	DisOneDigital16x8(6,88,TempValue%100/10,NormalDisplay);
	DisOneDigital16x8(6,96,TempValue%10,NormalDisplay);

}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowManagerIdentify(void)
{
	const uint8_t ManagerIdentifyStr[]={HZ_yan,HZ_zheng,HZ_guan,HZ_li,HZ_yuan,HZ_end};
	const uint8_t ManagerIdentifyStrEn1[]={"Authentication"};
	const uint8_t ManagerIdentifyStrEn2[]={"administrator"};
	const uint8_t TitleStr1[]={HZ_zhi,HZ_wen,HZ_ying,HZ_jianshu,HZ_bu,HZ_pi,HZ_pei,HZ_end};
	const uint8_t TitleStr2[]={HZ_qing,HZ_hui,HZ_fu,HZ_chu,HZ_chang,HZ_end};
	
	uint8_t i;

	if ( ManagerIdentifyMgr.Status == StartManagerIdentify )
	{	
		if ( IfSystemIsNoMaster() == bTRUE )
		{
			CurrentScreen = SCREEN_RegisterPasscodeUser;
			PasscodeUserRegisterMgr.Status = InputFirstPasscode;
			DataInputMgr.Status = InputIdle;
			PasscodeUserRegisterMgr.UserID = 0x01;
			PasscodeUserRegisterMgr.UserPriority = Master;
			if ( FrmMgr.FrmFunctionConfig == FRM_Enabled )
			{
				FaceRecognition_PowerDown();
			}
			
			PasscodeInputMgr.Point = 0x00;
			PasscodeInputMgr.PasscodeLen = 12;
			PasscodeInputMgr.Status = PasscodeInputStart;
			for (i=0;i<12;i++)
			{
				PasscodeInputMgr.InputBuff[i] = 0xFF;		//Initial passcode buffer
			}
			PasscodeUserRegisterMgr.TimeCnt = Def_WaitUserInputPasscodeTimeDelay;		
			GUI_Flag_RefreshLCD = bTRUE;
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_AddNewMasterPleaseInputPasscode);
			
		}
		else
		{
			PasscodeInputMgr.Point = 0x00;
			PasscodeInputMgr.PasscodeLen = 16;
			PasscodeInputMgr.Status = PasscodeInputStart;
			PasscodeUserIdentifyMgr.Status = PasscodeIdentifyPasscodeInput;
			PasscodeUserIdentifyMgr.TimeCnt = 240;	//
			for (i=0;i<PasscodeInputMgr.PasscodeLen;i++)
			{
				PasscodeInputMgr.InputBuff[i]=0xff;
			}
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(2,28,ManagerIdentifyStr,NormalDisplay);
			}
			else
			{
				DisEN16x8Str(0,8,ManagerIdentifyStrEn1,NormalDisplay);
				DisEN16x8Str(2,12,ManagerIdentifyStrEn2,NormalDisplay);
			}
			
			//DisHZ16x14Str(4,16,Str2,NormalDisplay);
			ManagerIdentifyMgr.Status = IdentifyingManager;
			FpIdentifyMgr.Status = FPMcmdStart;
			PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_IdentifyMaster);
		}
	}
	else if ( ManagerIdentifyMgr.Status == IdentifyingManager )
	{

		GUI_PasscodeInputCreat(6,16);
		
		if (PasscodeInputMgr.Status == PasscodeInputEnd)
		{	
			ManagerIdentifyMgr.Status = ManagerIdentifyFail;	
			ManagerIdentifyMgr.PasscodeUserID = PasscodeIdendify( PasscodeInputMgr.InputBuff);
			if ( (ManagerIdentifyMgr.PasscodeUserID != 0x00 )
				&&(ManagerIdentifyMgr.PasscodeUserID < (DEF_MAX_PASSCODEMASTER+1) )
			   )
			{
				ManagerIdentifyMgr.Status = ManagerIdentifySuccess;
				GUI_SetFPM_LED(DEF_FpmLedMode_Flash,DEF_FpmLedColor_Green,DEF_FpmLedColor_Green,1);
				ManagerIdentifyMgr.MasterType = PASSCODEUSER;
				ManagerIdentifyMgr.TimeCnt = Def_GuiTimeDelayCnt01s;	
				//ManagerIdentifyMgr.PasscodeUserID = ManagerIdentifyMgr.PasscodeUserID;
			}
			GUI_Flag_RefreshLCD = bTRUE;
			PasscodeUserIdentifyMgr.Status = PasscodeIdentifyIdle;

			if ( ManagerIdentifyMgr.Status == ManagerIdentifyFail )
			{
				PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_IdentifyFail);
				GUI_SetFPM_LED(DEF_FpmLedMode_Breath,DEF_FpmLedColor_Red,DEF_FpmLedColor_Red,255);
				ManagerIdentifyMgr.TimeCnt = Def_MessageBoxTimeDelay;	
				if ( SafetyMonitorMgr.ManagerPasscodeIdentifyFailedTimes < DEF_ManagerPasscodeIdentifyFailedTimesLimited )
				{
					SafetyMonitorMgr.ManagerPasscodeIdentifyFailedTimes++;
				}	
			}
			else
			{
				PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_VerifySuccess);
			}
			
		}
		else if (PasscodeInputMgr.Status == PasscodeInputExit)
		{
			ManagerIdentifyMgr.Status = ManagerIdentifyExit;
			PasscodeUserIdentifyMgr.Status = PasscodeIdentifyIdle;
		}

		FpUserIdentify();
		if ( (FpIdentifyMgr.Status == success)
			&&(FpIdentifyMgr.UserID < (DEF_MAX_FPMASTER) )
			)
		{
				ManagerIdentifyMgr.Status = ManagerIdentifySuccess;
				GUI_SetFPM_LED(DEF_FpmLedMode_Flash,DEF_FpmLedColor_Green,DEF_FpmLedColor_Green,1);
				ManagerIdentifyMgr.MasterType = FPUSER;
				ManagerIdentifyMgr.FpUserID = FpIdentifyMgr.UserID+1;
				ManagerIdentifyMgr.TimeCnt = Def_GuiTimeDelayCnt01s;
				FpIdentifyMgr.Status = FPMcmdStart;
				PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_VerifySuccess);
		}
		else if (( FpIdentifyMgr.Status == fail)
				||( (FpIdentifyMgr.Status == success)&&((FpIdentifyMgr.UserID+1) > DEF_MAX_FPMASTER ))
			)
		{
			if (FpIdentifyMgr.ErrorType == Error_SerialNumberMismatched ){
				ManagerIdentifyMgr.ErrorType = FPMserialNumberMismatched;
			}
			else{
				ManagerIdentifyMgr.ErrorType = MasterMismatched;
			}
			FpIdentifyMgr.Status = FPMcmdStart;
			ManagerIdentifyMgr.Status = ManagerIdentifyFail;
			ManagerIdentifyMgr.TimeCnt = Def_MessageBoxTimeDelay;
			GUI_SetFPM_LED(DEF_FpmLedMode_Breath,DEF_FpmLedColor_Red,DEF_FpmLedColor_Red,255);
			GUI_Flag_RefreshLCD = bTRUE;
			if ( SafetyMonitorMgr.ManagerFpIdentifyFailedTimes < DEF_ManagerFpIdentifyFailedTimesLimited )
			{
				SafetyMonitorMgr.ManagerFpIdentifyFailedTimes++;
			}	
			PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_IdentifyFail);
		}

		if ( FrmMgr.FrmFunctionConfig == FRM_Enabled )
		{
			if ( PasscodeInputMgr.Point > 0 )
			{
				FaceIdentifyMgr.IndentifyDelayTimeCnt = Def_GuiTimeDelayCnt2s;
			}
			
			if ( FaceIdentifyMgr.IndentifyDelayTimeCnt > 0 )
			{
				 FaceIdentifyMgr.IndentifyDelayTimeCnt--;	 
			}
			if ((IfSystemIsNoFaceUser() == bFALSE )			//system is has face user
				&&( FaceIdentifyMgr.IndentifyDelayTimeCnt == 0x0000 )			//
				)
			{
				FaceUserIdentify();
				if ( FaceIdentifyMgr.Status == FaceIdentifySuccess )
				{
					if ( GetUserIDbyFaceTemplateID(FrmMgr.UserID) <= DEF_MAX_FACEMASTER )
					{
						ManagerIdentifyMgr.Status = ManagerIdentifySuccess;
						ManagerIdentifyMgr.MasterType = FACEUSER;
						ManagerIdentifyMgr.FaceUserID = GetUserIDbyFaceTemplateID(FrmMgr.UserID);
						ManagerIdentifyMgr.TimeCnt = Def_GuiTimeDelayCnt01s;
						FaceIdentifyMgr.Status = FrmIdentifyStart;
						PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_VerifySuccess);
						GUI_SetFPM_LED(DEF_FpmLedMode_Flash,DEF_FpmLedColor_Green,DEF_FpmLedColor_Green,1);
					}
				}
				else if ( FaceIdentifyMgr.Status == FaceIdentifyFail )
				{
					FaceIdentifyMgr.Status = FrmIdentifyStart;
					DEBUG_MARK;
				}
			}
			else
			{
				if ( FrmMgr.PowerStatus != FRM_PowerOff )
				{
					FaceRecognition_HardwarePowerOff();
				}	
			}
		}
	}
	else if ( ManagerIdentifyMgr.Status == ManagerIdentifySuccess )
	{
		if (ManagerIdentifyMgr.TimeCnt > 0 )
		{
			ManagerIdentifyMgr.TimeCnt--;
		}
		else
		{
			if ( FrmMgr.FrmFunctionConfig == FRM_Enabled )
			{
				//FaceRecognition_PowerDown();
				FaceRecognition_HardwarePowerOff();
			}
			GoIntoMainMenu_Init();
			GUI_RefreshSleepTime();
			GUI_SetFPM_LED(DEF_FpmLedMode_Breath,DEF_FpmLedColor_Blue,DEF_FpmLedColor_Blue,255);
			SafetyMonitorMgr.ManagerPasscodeIdentifyFailedTimes = 0x00;
			SafetyMonitorMgr.ManagerFpIdentifyFailedTimes = 0x00;
		}
		
	}
	else if ( ManagerIdentifyMgr.Status == ManagerIdentifyFail )
	{
		if (SystemLanguage == Chinese)
		{
			if (ManagerIdentifyMgr.ErrorType == FPMserialNumberMismatched ){
				DisHZ16x14Str(1,14,TitleStr1,NormalDisplay);
				DisHZ16x14Str(4,20,TitleStr2,NormalDisplay);
			}
			else{
				DisImage(1,52,24,24,Icon_Incorrect,NormalDisplay);
				DisHZ16x14Str(5,34,IdentifyFailStr,NormalDisplay);
			}
		}
		else
		{
			DisImage(1,52,24,24,Icon_Incorrect,NormalDisplay);
			DisEN16x8Str(5,18,IdentifyFailStrEn,NormalDisplay);
		}
		if (--ManagerIdentifyMgr.TimeCnt < 1 )
		{
			if ( SafetyMonitorMgr.ManagerPasscodeIdentifyFailedTimes >= DEF_ManagerPasscodeIdentifyFailedTimesLimited )
			{
				SafetyMonitorMgr.SystemLocked = bTRUE;
				SafetyMonitorMgr.SystemLockedTimeDelay = DEF_SystemLockedTime;
				ComPort_SetPost_Alarm(DEF_WifiAlarm_PasswordUnlockAlarm,PASSCODEUSER,0x00);
			}
			else if ( SafetyMonitorMgr.ManagerFpIdentifyFailedTimes >= DEF_ManagerFpIdentifyFailedTimesLimited )
			{
				SafetyMonitorMgr.SystemLocked = bTRUE;
				SafetyMonitorMgr.SystemLockedTimeDelay = DEF_SystemLockedTime;
				ComPort_SetPost_Alarm(DEF_WifiAlarm_FpUnlockAlarm,FPUSER,0x00);
			}
			GUI_SetFPM_LED(DEF_FpmLedMode_Breath,DEF_FpmLedColor_Blue,DEF_FpmLedColor_Blue,255);
			GoIntoMainScreen_WithIdentifyInit();
		}
	}
	else if ( ManagerIdentifyMgr.Status == ManagerIdentifyExit )
	{
		GoIntoMainScreen_WithIdentifyInit();
	}

}
/*******************************************************/
/*******************************************************/
/*******************************************************/
void VoiceReportCurrentUnlockMode(void)
{
	uint16_t VoiceStr[5];
	VoiceStr[0] = VOICE_Current;
	VoiceStr[1] = VOICE_Setting;
	if( UserIdentifyResultMgr.UnlockingMode == SingalMode  )
	{
		VoiceStr[2] = VOICE_SingleUnlock;
	}
	else
	{
		VoiceStr[2] = VOICE_CombinationUnlock;
	}
	VoiceStr[3] = DEF_VoiceSegmentEndFlag;
	PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr);
}
void GoIntoUnlockingModeSetting_Init(void)
{
	VoiceMenuMgr.MenuPoint = 0;
	VoiceMenuMgr.TotalMenuNum = 4;
	CurrentScreen = SCREEN_UnlockingModeSetting;
}

void ShowUnlockingModeSetting(void)//开锁模式
{					

	if (SystemLanguage == Chinese)
	{
		if( UserIdentifyResultMgr.UnlockingMode == SingalMode )
			{
		DisHZ16x14Str(0,4,SingalModeStr,InverseDisplay);
		DisHZ16x14Str(2,4,DoubleModeStr,NormalDisplay);
			}
		else{
			DisHZ16x14Str(0,4,SingalModeStr,NormalDisplay);
			DisHZ16x14Str(2,4,DoubleModeStr,InverseDisplay);

			}
	}
	else
	{
		if( UserIdentifyResultMgr.UnlockingMode == SingalMode )
		{
			DisEN16x8Str(0,4,SingalModeStrEn,InverseDisplay);
			DisEN16x8Str(2,4,DoubleModeStrEn,NormalDisplay);
		}
		else{
			DisEN16x8Str(0,4,SingalModeStrEn,NormalDisplay);
			DisEN16x8Str(2,4,DoubleModeStrEn,InverseDisplay);
		}
	}
	
	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,UnlockModeSetVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}

}


/*******************************************************/
/*******************************************************/
/*******************************************************/

void VoiceReportCurrentPickAlarmEnableSetting(void)
{
	if(PickAlarmEnableMgr.Enable == bTRUE)
	{
		PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_AntiPryingOpened);
	}
	else
	{
		PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_AntiPryingClosed);
	}
}

void GoIntoPickAlarmEnableSetting_Init(void)
{
	VoiceMenuMgr.MenuPoint = 0;
	VoiceMenuMgr.TotalMenuNum = 4;
	CurrentScreen = SCREEN_PickAlarmEnableSetting;
}


void ShowPickAlarmEnableSetting(void)
{
	if (SystemLanguage == Chinese)
	{
		if(PickAlarmEnableMgr.Enable == bTRUE)
			{
		DisHZ16x14Str(0,4,AntiPryingEnableStr,InverseDisplay);
		DisHZ16x14Str(2,4,AntiPryingDisableStr,NormalDisplay);
			}
		else{
			DisHZ16x14Str(0,4,AntiPryingEnableStr,NormalDisplay);
			DisHZ16x14Str(2,4,AntiPryingDisableStr,InverseDisplay);
			}
	}
	else{
		if(PickAlarmEnableMgr.Enable == bTRUE)
		{
			DisEN16x8Str(0,4,AntiPryingEnableStrEn,InverseDisplay);
			DisEN16x8Str(2,4,AntiPryingDisableStrEn,NormalDisplay);
		}
		else
		{
			DisEN16x8Str(0,4,AntiPryingEnableStrEn,NormalDisplay);
			DisEN16x8Str(2,4,AntiPryingDisableStrEn,InverseDisplay);
		}

	}
	
	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,AntiPryingSettingVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}

}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void VoiceReportCurrentBodyInduction(void)
{
	uint16_t VoiceStr[4];
	VoiceStr[0] = VOICE_Current;
	VoiceStr[1] = VOICE_BodyInduction;
	if( BodyInductionMgr.SensingDistanceLevel == SensingDistanceL0 )
	{
		VoiceStr[2] = VOICE_Close;
	}
	else if( BodyInductionMgr.SensingDistanceLevel == SensingDistanceL1 )
	{
		VoiceStr[2] = VOICE_Small;
	}
	else if( BodyInductionMgr.SensingDistanceLevel == SensingDistanceL2 )
	{
		VoiceStr[2] = VOICE_Big;
	}

	VoiceStr[3] = DEF_VoiceSegmentEndFlag;
	PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr);
}
void GoIntoBodyInductionSetting_Init(void)
{
	VoiceMenuMgr.MenuPoint = 0;
	VoiceMenuMgr.TotalMenuNum = 5;
	CurrentScreen = SCREEN_BodyInductionSetting;
}

void ShowBodyInductionSetting(void)
{		
	uint8_t i;
	
	if (SystemLanguage == Chinese)
	{
		for (i=0;i<3;i++)
		{
			if ( BodyInductionMgr.SensingDistanceLevel == i )
			{
				DisHZ16x14Str(2*i,4,BodyInductionAdjustStr[i],InverseDisplay);
			}
			else
			{
				DisHZ16x14Str(2*i,4,BodyInductionAdjustStr[i],NormalDisplay);
			}
		}
	}
	else
	{
		for (i=0;i<3;i++)
		{
			if ( BodyInductionMgr.SensingDistanceLevel == i )
			{
				DisEN16x8Str(2*i,4,BodyInductionAdjustStrEn[i],InverseDisplay);
			}
			else
			{
				DisEN16x8Str(2*i,4,BodyInductionAdjustStrEn[i],NormalDisplay);
			}
		}
	}

	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,BodyInductionVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}

}


/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowRestoreFactoryDefault(void)
{
	uint8_t i;

	//RestoreFactoryDefaultMgr.Status = RestoreFactoryDefaultEXIT;

	if ( RestoreFactoryDefaultMgr.Status == StartRestoreFactoryDefault )
	{
		RestoreFactoryDefaultMgr.Status = WaitForRestoreFactoryDefaultUserConfirm;
		GUI_Flag_RefreshLCD = bTRUE;
		RestoreFactoryDefaultMgr.Selection = NO;
		PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_ConfirmOrExitRestorFactoryDefault);
	}
	
	else if ( RestoreFactoryDefaultMgr.Status == WaitForRestoreFactoryDefaultUserConfirm )
		{
			if (SystemLanguage == Chinese){
				
				DisHZ16x14Str(1,32,ConfirmRestoreFactoryDefaultStr1,NormalDisplay);	
				DisHZ16x14Str(4,32,ConfirmRestoreFactoryDefaultStr2,NormalDisplay);
			}
			else{
				DisEN16x8Str(0,4,ConfirmRestoreFactoryDefaultStr1En,NormalDisplay);	
				DisEN16x8Str(3,4,ConfirmRestoreFactoryDefaultStr2En,NormalDisplay);	
			}
		}
    else if ( RestoreFactoryDefaultMgr.Status == ConfirmedToRestoreFactoryDefault )
		{	
			GUI_Flag_RefreshLCD = bTRUE;
			
			SET_ALLKEYLED_OFF();

			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_RestorFactoryDefaultPleaseWait);

//			#ifdef Function_FPMserialNumberCheck
//			RestoreFactoryDefaultMgr.Status = SaveFPMserialNumber;
//			#else
//			RestoreFactoryDefaultMgr.Status = ResetFPMuser;
//			AllUserFpDeleteMgr.Status = StartAllUserFpDelete;
//			RestoreFactoryDefaultMgr.FailTimes = 0x00;
//			#endif

			RestoreFactoryDefaultMgr.Status = ResetFPMuser;
			AllUserFpDeleteMgr.Status = StartAllUserFpDelete;
			RestoreFactoryDefaultMgr.FailTimes = 0x00;

		}
//	else if ( RestoreFactoryDefaultMgr.Status == SaveFPMserialNumber )
//	{
//		if ( SaveFPMserialNumberToMemory() == S_SUCCESS )
//			{
//				RestoreFactoryDefaultMgr.Status = ResetFPMuser;
//				AllUserFpDeleteMgr.Status = StartAllUserFpDelete;
//				RestoreFactoryDefaultMgr.FailTimes = 0x00;
//			}
//		else{	
//			RestoreFactoryDefaultMgr.Status = RestoreFactoryFail;
//			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_OperationFail);
//		}
//	}	
	else if ( RestoreFactoryDefaultMgr.Status == ResetFPMuser )
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(2,24,RestoreFactoryDoingStr1,NormalDisplay);	
			DisHZ16x14Str(5,43,RestoreFactoryDoingStr2,NormalDisplay);	
		}
		else{
			DisEN16x8Str(3,16,RestoreFactoryDoingStr1En,NormalDisplay);
			DisEN16x8Str(5,20,RestoreFactoryDoingStr2En,NormalDisplay);
		}	
		DeleteAllFpFromFPM();
		if ( AllUserFpDeleteMgr.Status == DeleteAllFpUserSuccess )
		{
			if ( FrmMgr.FrmFunctionConfig == FRM_Enabled )
			{
				RestoreFactoryDefaultMgr.Status = ResetFaceUser;
				RestoreFactoryDefaultMgr.FailTimes = 0x00;
				DeleteAllFaceTemplateMgr.Status = StartAllFaceTemplateDelete;
			}
			else
			{
				RestoreFactoryDefaultMgr.Status = ResetCardUser;
				DeleteAllFaceUserfromMemory();
			}
			RestoreFactoryDefaultMgr.TimeCnt = Def_MessageBoxTimeDelay;
			CheckMemoryMgr.FpUserNum = 0x00;
			CheckMemoryMgr.StressFpUserNum = 0x00;
			CheckMemoryMgr.FpMasterNum = 0x00;	
			for (i=0;i<(DEF_MAX_FPMASTER+DEF_MAX_FPUSER+DEF_MAX_STRESSFPUSER);i++)
			{
				FpUserMemoryMgr[i].UserID = 0xFFFF;
				FpUserMemoryMgr[i].RegisterStatus = UnRegistered;
				FpUserMemoryMgr[i].UserPriority = Undefined;
			}	
			ResetFPuserIdListInEEPROM();
		}
		else if ( AllUserFpDeleteMgr.Status == DeleteAllFpUserFail )
		{
			if ( RestoreFactoryDefaultMgr.FailTimes < 4 )
			{
				RestoreFactoryDefaultMgr.FailTimes++;
				AllUserFpDeleteMgr.Status = StartAllUserFpDelete;
				SET_FPMPOWER_OFF;	
				Hardware_DelayMs(5);//Wait for Power line lost power
				SET_FPMPOWER_ON;	
				Hardware_DelayMs(10);//Wait for Power line lost power
				GUI_RefreshSleepTime();
			}
			else
			{
				//RestoreFactoryDefaultMgr.Status = RestoreFactoryFail;
				//PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_OperationFail);
				if ( FrmMgr.FrmFunctionConfig == FRM_Enabled )
				{
					RestoreFactoryDefaultMgr.Status = ResetFaceUser;
					RestoreFactoryDefaultMgr.FailTimes = 0x00;
					DeleteAllFaceTemplateMgr.Status = StartAllFaceTemplateDelete;
				}
				else
				{
					RestoreFactoryDefaultMgr.Status = ResetCardUser;
					DeleteAllFaceUserfromMemory();
				}
				RestoreFactoryDefaultMgr.TimeCnt = Def_MessageBoxTimeDelay;
				CheckMemoryMgr.FpUserNum = 0x00;
				CheckMemoryMgr.StressFpUserNum = 0x00;
				CheckMemoryMgr.FpMasterNum = 0x00;	
				for (i=0;i<(DEF_MAX_FPMASTER+DEF_MAX_FPUSER+DEF_MAX_STRESSFPUSER);i++)
				{
					FpUserMemoryMgr[i].UserID = 0xFFFF;
					FpUserMemoryMgr[i].RegisterStatus = UnRegistered;
					FpUserMemoryMgr[i].UserPriority = Undefined;
				}	
				ResetFPuserIdListInEEPROM();
			}
		}	
	}
	else if ( RestoreFactoryDefaultMgr.Status == ResetFaceUser)
	{
		DeleteAllFaceTemplate();
		if ( DeleteAllFaceTemplateMgr.Status == DeleteAllFaceTemplateSuccess )
		{
			RestoreFactoryDefaultMgr.Status = ResetCardUser;
			CheckMemoryMgr.FaceMasterNum = 0x00;
			CheckMemoryMgr.FaceUserNum = 0x00;
		}
		else if ( DeleteAllFaceTemplateMgr.Status == DeleteAllFaceTemplateFail )
		{
			if ( RestoreFactoryDefaultMgr.FailTimes < 2 )
			{
				RestoreFactoryDefaultMgr.FailTimes++;
				DeleteAllFaceTemplateMgr.Status = StartAllFaceTemplateDelete;
				GUI_RefreshSleepTime();
			}
			else
			{
				RestoreFactoryDefaultMgr.Status = ResetCardUser;
				DeleteAllFaceUserfromMemory();
				CheckMemoryMgr.FaceMasterNum = 0x00;
				CheckMemoryMgr.FaceUserNum = 0x00;				
			}
		}
	}
	
	else if ( RestoreFactoryDefaultMgr.Status == ResetCardUser )
		{			
			if (RestoreFactoryDefaultMgr.TimeCnt-- < 1 )
			{
				DeleteAllCardUserfromMemory();
				RestoreFactoryDefaultMgr.Status = ResetPasscodeUser;
				RestoreFactoryDefaultMgr.TimeCnt = Def_MessageBoxTimeDelay;
				CheckMemoryMgr.CardUserNum = 0x00;
			}	
		}
	else if ( RestoreFactoryDefaultMgr.Status == ResetPasscodeUser )
		{	
			if (RestoreFactoryDefaultMgr.TimeCnt-- < 1 )
			{
				DeleteAllPasscodeMasterfromMemory();
				DeleteAllPasscodeUserfromMemory();
				SystemConfigReset();

				/** Erase Event Log **/

				if ( DeleteAllLog() != S_SUCCESS )
				{
					if ( DeleteAllLog() != S_SUCCESS )
					{
						DeleteAllLog();
					}
				}
				
				LogMgr.LastPoint = 0x0000;
				LogMgr.DisplayPoint = 0x0000;
				LogMgr.NewLog.LogIDmajor = 0x00;


				ComPort_SetPost_ClearWifiData();
				
				#if defined (Function_YouzhiyunjiaWifi) || defined (Function_TuyaWifi)
				Wifi_PostClearData();
				#endif
				
				RestoreFactoryDefaultMgr.Status = RestoreFactorySuccess;
				RestoreFactoryDefaultMgr.TimeCnt = Def_MessageBoxTimeDelay;
				CheckMemoryMgr.PasscodeMasterNum = 0x00;
				CheckMemoryMgr.PasscodeUserNum = 0x00;
				GUI_Flag_RefreshLCD = bTRUE;
				PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_OperationSuccess);
			}
		}
	
	else if ( RestoreFactoryDefaultMgr.Status == RestoreFactorySuccess )
		{
			GUI_Flag_RefreshLCD = bTRUE;
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(4,36,OperationSuccessStr,NormalDisplay);
			}
			else{
				DisEN16x8Str(4,36,OperationSuccessStrEn,NormalDisplay);
			}
			if (RestoreFactoryDefaultMgr.TimeCnt-- < 1 )
			{
				GoIntoMainScreen_WithIdentifyInit();
				ComportMgr.RestoreFactoryDefaultTrig = bFALSE;
			}
			
		}
	else if ( RestoreFactoryDefaultMgr.Status == RestoreFactoryFail )
		{
			GUI_Flag_RefreshLCD = bTRUE;
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(3,36,OperationFailStr,NormalDisplay);
			}
			else{
				DisEN16x8Str(3,48,OperationFailStrEn,NormalDisplay);
			}
			if (RestoreFactoryDefaultMgr.TimeCnt-- < 1 )
			{
				GoIntoMainScreen_WithIdentifyInit();
				ComportMgr.RestoreFactoryDefaultTrig = bFALSE;
			}
		}
	else if ( RestoreFactoryDefaultMgr.Status == RestoreFactoryDefaultEXIT )
		{
			if ( ComportMgr.RestoreFactoryDefaultTrig == bTRUE )
			{
				GoIntoMainScreen_WithIdentifyInit();
				ComportMgr.RestoreFactoryDefaultTrig = bFALSE;
			}
			else{
				GoIntoInfoInquiryMenu_Init();	
			}
		}

}
//#ifdef Function_YouzhiyunjiaWifi
#if 0
/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowNetWorkConnecting(void)
{
	const uint8_t LinkingStr[]={HZ_lian,HZ_wang,HZ_zhong,ZF_douhao,HZ_qing,HZ_shao,HZ_houniao,HZ_end};		//锟斤拷锟斤拷锟叫ｏ拷锟斤拷锟皆猴拷
	const uint8_t LinkingStrEn[]={"Network Connecting"};						//
	const uint8_t LinkSuccessStr[]={HZ_lian,HZ_wang,HZ_cheng,HZ_gong,HZ_end};		//锟斤拷锟斤拷锟缴癸拷
	const uint8_t LinkSuccessStrEn[]={"Connected!"};	
	const uint8_t LinkFailStr[]={HZ_lian,HZ_wang,HZ_shibai,HZ_bai,HZ_end};		//锟斤拷锟斤拷失锟斤拷
	const uint8_t LinkFailStrEn[]={"Connect FAIL!"};	


	if ( WifiMgr.Link.Status == LinkStart )
	{
		Wifi_PostGetHotspotCmd();
		WifiMgr.Link.Status = LinkWait;
		SET_ALLKEYLED_OFF();
		PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_NetworkingConfiguration);
	}
	else if (( WifiMgr.Link.Status == LinkWait )||( WifiMgr.Link.Status == LinkRouter ))
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(3,16,LinkingStr,NormalDisplay);
		}
		else{
			DisEN16x8Str(3,0,LinkingStrEn,NormalDisplay);
		}
		WifiMgr.Link.Timer = Def_GuiTimeDelayCnt3s;
		GUI_RefreshSleepTime();
		KEYLED_ASTERISK_Flash();
	}
	else 
	{
		SET_ALLKEYLED_OFF();
		if ( WifiMgr.Link.Status == LinkSuccess )
		{
			GUI_Flag_RefreshLCD = bTRUE;
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(3,36,LinkSuccessStr,NormalDisplay);
			}
			else{
				DisEN16x8Str(3,8,LinkSuccessStrEn,NormalDisplay);
			}
			if ( WifiMgr.Link.Timer == (Def_GuiTimeDelayCnt3s) )
			{
				PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_OperationSuccess);
				Wifi_PostGetUTC(2); 
			}
		}
		else
		{
			GUI_Flag_RefreshLCD = bTRUE;
			
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(3,36,LinkFailStr,NormalDisplay);
			}
			else{
				DisEN16x8Str(3,8,LinkFailStrEn,NormalDisplay);
			}
			if ( WifiMgr.Link.Timer == (Def_GuiTimeDelayCnt3s) )
			{
				PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_OperationFail);
			}
		}
		
		if (WifiMgr.Link.Timer > 0){
			WifiMgr.Link.Timer--;
		}
		else
		{
			GoIntoMainMenu_Init();
		}
	}
	
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowWifiManufactureTest(void)
{
	//const uint8_t LinkingStr[]={HZ_lian,HZ_wang,HZ_zhong,ZF_douhao,HZ_qing,HZ_shao,HZ_houniao,HZ_end};		//锟斤拷锟斤拷锟叫ｏ拷锟斤拷锟皆猴拷
	const uint8_t wifiMFTStrEn[]={"Wifi MFT"};						//
	//const uint8_t LinkSuccessStr[]={HZ_lian,HZ_wang,HZ_cheng,HZ_gong,HZ_end};		//锟斤拷锟斤拷锟缴癸拷
	const uint8_t WifiMFTSuccessStrEn[]={"PASS!"};	
	//const uint8_t LinkFailStr[]={HZ_lian,HZ_wang,HZ_shibai,HZ_bai,HZ_end};		//锟斤拷锟斤拷失锟斤拷
	const uint8_t WifiMFTFailStrEn[]={"FAIL!"};	

	const uint8_t NoNetworkDeviceStr[]={HZ_wu,HZ_lian,HZ_wang,HZ_ying,HZ_jianshu,HZ_end};		//锟斤拷锟斤拷失锟斤拷
	const uint8_t NoNetworkDeviceStrEn[]={"No Wifi Module"};


	if ( WifiMgr.MFT.Status == MFTStart )
	{
		WifiMgr.MFT.Status = MFTWait;	
		Wifi_PostManufactureTest();
		SET_ALLKEYLED_OFF();
	}
	else if ( WifiMgr.MFT.Status == MFTWait )
	{
		DisEN16x8Str(3,32,wifiMFTStrEn,NormalDisplay);
		WifiMgr.MFT.TimeCnt = Def_GuiTimeDelayCnt3s;
		GUI_RefreshSleepTime();
		KEYLED_ASTERISK_Flash();
	}
	else 
	{
		SET_ALLKEYLED_OFF();
		if ( WifiMgr.MFT.Status == MFTSuccess )
		{
			GUI_Flag_RefreshLCD = bTRUE;
			DisEN16x8Str(3,42,WifiMFTSuccessStrEn,NormalDisplay);
			if ( WifiMgr.MFT.TimeCnt == (Def_GuiTimeDelayCnt3s) )
			{
				PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_OperationSuccess);
			}
		}
		else
		{
			GUI_Flag_RefreshLCD = bTRUE;

			if ( WifiMgr.MFT.errorcode == 0x02 )
			{
				if (SystemLanguage == Chinese){
					DisHZ16x14Str(3,36,NoNetworkDeviceStr,NormalDisplay);
				}
				else{

					DisEN16x8Str(3,8,NoNetworkDeviceStrEn,NormalDisplay);
				}
			}
			else
			{
				DisEN16x8Str(3,42,WifiMFTFailStrEn,NormalDisplay);
			}
			
			if ( WifiMgr.MFT.TimeCnt == (Def_GuiTimeDelayCnt3s) )
			{
				PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_OperationFail);
			}
		}
		
		if (WifiMgr.MFT.TimeCnt > 0){
			WifiMgr.MFT.TimeCnt--;
		}
		else
		{
			GoIntoMainScreen_WithIdentifyInit();
		}
	}
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void GUI_WifiDrivingRemoteUnlockSuccess(void)
{
	GUI_RefreshSleepTime();		
	UserIdentifyResultMgr.IdentifyType = RemoteUnlock;
	Enable_KEYLED_WATERLIGHT(); 
	CurrentScreen = SCREEN_IdentifySuccess;
	DisplayDoorStatusMgr.Status = DoorOpenInit;
	UserIdentifyResultMgr.TimeCnt = Def_IdentifySuccessScreenTimeLimited;
}

void ShowRemoteUnlockRequest(void)
{
	const uint8_t RemoteUnlockRequestStr1[]={HZ_yuanchu,HZ_chengxu,HZ_kai,HZ_suomen,HZ_qing,HZ_qiu,HZ_zhong,HZ_end};
	const uint8_t RemoteUnlockRequestStr2[]={HZ_qing,HZ_shao,HZ_houniao,HZ_end};
	const uint8_t RemoteUnlockRequestStr1En[]={"Request Remote Un-"};
	const uint8_t RemoteUnlockRequestStr2En[]={"lock Please wait"};
	const uint8_t NoNetworkDeviceStr[]={HZ_wu,HZ_lian,HZ_wang,HZ_ying,HZ_jianshu,HZ_end};		//联网失败
	const uint8_t NoNetworkDeviceStrEn[]={"No Wifi Module"};


	if ( WifiMgr.RemoteUnlockMgr.Status == RemoteUnlockRequestInit )
	{
		WifiMgr.RemoteUnlockMgr.TimeCnt = 60*64;
		WifiMgr.RemoteUnlockMgr.Status = RemoteUnlockRequestWait;
		Wifi_PostRemoteUnlockRequestCmd();
	}
	else if (WifiMgr.RemoteUnlockMgr.Status == RemoteUnlockRequestWait )
	{
		GUI_Flag_RefreshLCD = bTRUE;

		SystemPowerMgr.SleepDelayTimerCnt = Def_GuiTimeDelayCnt10s;
		
		if ( WifiMgr.RemoteUnlockMgr.TimeCnt > (55*64) )
		{
			DisImage(1,33,61,40,Icon_DoorBell,NormalDisplay);
		}	
		else //if ( WifiMgr.RemoteUnlockMgr.TimeCnt < (56*64) )
		{
			if (SystemLanguage == Chinese)
			{
				DisHZ16x14Str(2,16,RemoteUnlockRequestStr1,NormalDisplay);
				DisHZ16x14Str(4,36,RemoteUnlockRequestStr2,NormalDisplay);
			}
			else
			{
				DisEN16x8Str(2,0,RemoteUnlockRequestStr1En,NormalDisplay);
				DisEN16x8Str(4,0,RemoteUnlockRequestStr1En,NormalDisplay);
			}
		}
	}
	else if (WifiMgr.RemoteUnlockMgr.Status == RemoteUnlockSuccess)
	{
		GUI_RefreshSleepTime();
		
		UserIdentifyResultMgr.IdentifyType = RemoteUnlock;
		Enable_KEYLED_WATERLIGHT();	
		CurrentScreen = SCREEN_IdentifySuccess;
		DisplayDoorStatusMgr.Status = DoorOpenInit;
		UserIdentifyResultMgr.TimeCnt = Def_IdentifySuccessScreenTimeLimited;
	}
	else if (WifiMgr.RemoteUnlockMgr.Status == RemoteUnlockFail)
	{	
		if ( WifiMgr.RemoteUnlockMgr.errorcode == 0x02 )
		{
			WifiMgr.RemoteUnlockMgr.TimeCnt = 0x00;
		}
		else
		{
			GUI_Flag_RefreshLCD = bTRUE;
			
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(3,36,OperationFailStr,NormalDisplay);
			}
			else{
				DisEN16x8Str(3,8,OperationFailStrEn,NormalDisplay);
			}
			WifiMgr.RemoteUnlockMgr.TimeCnt = 0x00;
		}
	}
	else if ( WifiMgr.RemoteUnlockMgr.Status = RemoteUnlockExit )
	{
		WifiMgr.RemoteUnlockMgr.TimeCnt = 0x00;
		SystemPowerMgr.SleepDelayTimerCnt = Def_GuiTimeDelayCnt3s;
		ComPort_SetPost_ExitRemoteUnlcok();
	}

	if (WifiMgr.RemoteUnlockMgr.TimeCnt > 0)
	{
		if ( WifiMgr.RemoteUnlockMgr.TimeCnt == Def_GuiTimeDelayCnt3s )
		{
			//PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_OperationFail);
		}
		WifiMgr.RemoteUnlockMgr.TimeCnt--;
	}
	else
	{
		GoIntoMainScreen_WithIdentifyInit();
	}

}

#endif
/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowNetWorkConnecting(void)
{
	const uint8_t LinkingStr[]={HZ_lian,HZ_wang,HZ_zhong,ZF_douhao,HZ_qing,HZ_shao,HZ_houniao,HZ_end};		//配网中，请稍侯
	const uint8_t LinkingStrEn1[]={"Network"};
	const uint8_t LinkingStrEn2[]={"Connecting..."};
	const uint8_t LinkSuccessStr[]={HZ_lian,HZ_wang,HZ_cheng,HZ_gong,HZ_end};		//联网成功
	const uint8_t LinkSuccessStrEn[]={"Connected!"};	
	const uint8_t LinkFailStr[]={HZ_lian,HZ_wang,HZ_shibai,HZ_bai,HZ_end};		//联网失败
	const uint8_t LinkFailStrEn[]={"Connect FAIL!"};	

	const uint8_t NoNetworkDeviceStr[]={HZ_wu,HZ_lian,HZ_wang,HZ_ying,HZ_jianshu,HZ_end};		//联网失败
	const uint8_t NoNetworkDeviceStrEn[]={"No Wifi Module"};


	if ( WifiMgr.Link.Status == LinkStart )
	{
		ComPort_SetPost_WifiHotSpot();
		WifiMgr.Link.Status = LinkWait;
		WifiMgr.Link.TimeCnt = 185*64;	//185S
		SET_ALLKEYLED_OFF();
		PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_NetworkingConfiguration);
		
	}
	else if ( WifiMgr.Link.Status == LinkWait )
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(3,16,LinkingStr,NormalDisplay);
		}
		else
		{
			DisEN16x8Str(2,36,LinkingStrEn1,NormalDisplay);
			DisEN16x8Str(4,12,LinkingStrEn2,NormalDisplay);
		}
		GUI_RefreshSleepTime();
		KEYLED_ASTERISK_Flash();
	}
	else if ( WifiMgr.Link.Status == LinkSuccess )
	{
		SET_ALLKEYLED_OFF();
		//GUI_Flag_RefreshLCD = bTRUE;
		Clear_Screen();
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(3,36,LinkSuccessStr,NormalDisplay);
		}
		else{
			DisEN16x8Str(3,8,LinkSuccessStrEn,NormalDisplay);
		}
		PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_OperationSuccess);
		WifiMgr.Link.Status = LinkIdle;
		WifiMgr.Link.TimeCnt = Def_GuiTimeDelayCnt3s;
	}
	else if ( WifiMgr.Link.Status == LinkFail )
	{
		//GUI_Flag_RefreshLCD = bTRUE;
		Clear_Screen();
		if ( WifiMgr.Link.errorcode == 0x02 )
		{
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(3,36,NoNetworkDeviceStr,NormalDisplay);
			}
			else{
				DisEN16x8Str(3,8,NoNetworkDeviceStrEn,NormalDisplay);
			}
		}
		else
		{
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(3,36,LinkFailStr,NormalDisplay);
			}
			else{
				DisEN16x8Str(3,8,LinkFailStrEn,NormalDisplay);
			}
		}	
		if ( WifiMgr.Link.errorcode == 0x02 )	//wifi module not be found
		{
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume, VoiceStr_HardwareNotSupportOperationFail);
		}
		else
		{
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_OperationFail);
		}
		WifiMgr.Link.Status = LinkIdle;
		WifiMgr.Link.TimeCnt = Def_GuiTimeDelayCnt3s;

	}
	
	if (WifiMgr.Link.TimeCnt > 0){
		WifiMgr.Link.TimeCnt--;
	}
	else
	{
		if ( WifiMgr.Link.Status == LinkWait )
		{
			WifiMgr.Link.Status = LinkFail;
			WifiMgr.Link.errorcode = 0x01;
		}
		else
		{
			GoIntoMainMenu_Init();
		}
	}
	
	
}
/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowWifiManufactureTest(void)
{
	//const uint8_t LinkingStr[]={HZ_lian,HZ_wang,HZ_zhong,ZF_douhao,HZ_qing,HZ_shao,HZ_houniao,HZ_end};		//配网中，请稍侯
	const uint8_t wifiMFTStrEn[]={"Wifi MFT"};						//
	//const uint8_t LinkSuccessStr[]={HZ_lian,HZ_wang,HZ_cheng,HZ_gong,HZ_end};		//联网成功
	const uint8_t WifiMFTSuccessStrEn[]={"PASS!"};	
	//const uint8_t LinkFailStr[]={HZ_lian,HZ_wang,HZ_shibai,HZ_bai,HZ_end};		//联网失败
	const uint8_t WifiMFTFailStrEn[]={"FAIL!"};	

	const uint8_t NoNetworkDeviceStr[]={HZ_wu,HZ_lian,HZ_wang,HZ_ying,HZ_jianshu,HZ_end};		//联网失败
	const uint8_t NoNetworkDeviceStrEn[]={"No Wifi Module"};


	if ( WifiMgr.MFT.Status == MFTStart )
	{
		ComPort_SetPost_WifiMFT();
		WifiMgr.MFT.Status = MFTWait;
		WifiMgr.MFT.TimeCnt = 15*64;	//15s
		SET_ALLKEYLED_OFF();
	}
	else if ( WifiMgr.MFT.Status == MFTWait )
	{
		DisEN16x8Str(3,32,wifiMFTStrEn,NormalDisplay);
		GUI_RefreshSleepTime();
		KEYLED_ASTERISK_Flash();
	}

	else if ( WifiMgr.MFT.Status == MFTSuccess )
	{
		SET_ALLKEYLED_OFF();
		//GUI_Flag_RefreshLCD = bTRUE;
		Clear_Screen();
		DisEN16x8Str(3,42,WifiMFTSuccessStrEn,NormalDisplay);

		PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_OperationSuccess);

		WifiMgr.MFT.Status = MFTIdle;
		WifiMgr.MFT.TimeCnt = Def_GuiTimeDelayCnt3s;
	}
	else  if ( WifiMgr.MFT.Status == MFTFail )
	{
		SET_ALLKEYLED_OFF();
		//GUI_Flag_RefreshLCD = bTRUE;
		Clear_Screen();
		if ( WifiMgr.MFT.errorcode == 0x02 )
		{
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(3,36,NoNetworkDeviceStr,NormalDisplay);
			}
			else{

				DisEN16x8Str(3,8,NoNetworkDeviceStrEn,NormalDisplay);
			}
		}
		else
		{
			DisEN16x8Str(3,42,WifiMFTFailStrEn,NormalDisplay);
		}
		PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_OperationFail);
		WifiMgr.MFT.Status = MFTIdle;
		WifiMgr.MFT.TimeCnt = Def_GuiTimeDelayCnt3s;
	}
		
	if (WifiMgr.MFT.TimeCnt > 0){
		WifiMgr.MFT.TimeCnt--;
	}
	else
	{
		if ( WifiMgr.MFT.Status == MFTWait )
		{
			WifiMgr.MFT.Status = MFTFail;
			WifiMgr.MFT.errorcode = 0x01;
		}
		else
		{
			GoIntoMainScreen_WithIdentifyInit();
		}
	}

	
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowRemoteUnlockRequest(void)
{
	const uint8_t RemoteUnlockRequestStr1[]={HZ_zhengque,HZ_zaima,HZ_qing,HZ_qiu,HZ_yuanchu,HZ_chengxu,HZ_kai,HZ_suomen,HZ_end};
	const uint8_t RemoteUnlockRequestStr2[]={HZ_qing,HZ_shao,HZ_houniao,HZ_end};
	const uint8_t RemoteUnlockRequestStr1En[]={"Request Remote Un-"};
	const uint8_t RemoteUnlockRequestStr2En[]={"lock Please wait"};
	const uint8_t NoNetworkDeviceStr[]={HZ_wu,HZ_lian,HZ_wang,HZ_ying,HZ_jianshu,HZ_end};		//联网失败
	const uint8_t NoNetworkDeviceStrEn[]={"No Wifi Module"};


	if ( WifiMgr.RemoteUnlockMgr.Status == RemoteUnlockRequestInit )
	{
		WifiMgr.RemoteUnlockMgr.TimeCnt = 60*64;
		WifiMgr.RemoteUnlockMgr.Status = RemoteUnlcokDisplayDoorBell;
		WifiMgr.RemoteUnlockMgr.Result = IsUnknow;
		SystemPowerMgr.SleepDelayTimerCnt = Def_GuiTimeDelayCnt10s;
		if ( FrmMgr.PowerStatus != FRM_PowerOff )
		{
			FaceRecognition_HardwarePowerOff();
		}
		
		ComPort_SetPost_RemoteUnlcok();
	}
	else if (WifiMgr.RemoteUnlockMgr.Status == RemoteUnlcokDisplayDoorBell ) 
	{
		DisImage(1,33,61,40,Icon_DoorBell,NormalDisplay);
		
		if ( WifiMgr.RemoteUnlockMgr.TimeCnt < (55*64) )
		{
			if ( WifiMgr.RemoteUnlockMgr.Result != IsFail )
			{
				PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_RequestingRemoteUnlock);
			}
			WifiMgr.RemoteUnlockMgr.Status = RemoteUnlockRequestWait;
		}

	}
	else if (WifiMgr.RemoteUnlockMgr.Status == RemoteUnlockRequestWait )
	{
		GUI_Flag_RefreshLCD = bTRUE;

		SystemPowerMgr.SleepDelayTimerCnt = Def_GuiTimeDelayCnt10s;

		if ( WifiMgr.RemoteUnlockMgr.Result != IsFail )
		{
			if (SystemLanguage == Chinese)
			{
				DisHZ16x14Str(1,8,RemoteUnlockRequestStr1,NormalDisplay);
				DisHZ16x14Str(4,42,RemoteUnlockRequestStr2,NormalDisplay);
			}
			else
			{
				DisEN16x8Str(2,0,RemoteUnlockRequestStr1En,NormalDisplay);
				DisEN16x8Str(4,0,RemoteUnlockRequestStr1En,NormalDisplay);
			}
		}

		if ( WifiMgr.RemoteUnlockMgr.Result == IsSuccess )
		{
			GUI_RefreshSleepTime();
			UserIdentifyResultMgr.IdentifyType = RemoteUnlock;
			Enable_KEYLED_WATERLIGHT();	
			CurrentScreen = SCREEN_IdentifySuccess;
			DisplayDoorStatusMgr.Status = DoorOpenInit;
			UserIdentifyResultMgr.TimeCnt = Def_IdentifySuccessScreenTimeLimited;
			WifiMgr.RemoteUnlockMgr.Status = RemoteUnlockDisplayResult;
			
		}
		else if ( WifiMgr.RemoteUnlockMgr.Result == IsFail )
		{
			if ( WifiMgr.RemoteUnlockMgr.errorcode == 0x02 )//wifi not connected
			{
				WifiMgr.RemoteUnlockMgr.TimeCnt = 0x00;
			}
			else
			{
				GUI_Flag_RefreshLCD = bTRUE;
				if (SystemLanguage == Chinese){
					DisHZ16x14Str(3,36,OperationFailStr,NormalDisplay);
				}
				else{
					DisEN16x8Str(3,8,OperationFailStrEn,NormalDisplay);
				}
				WifiMgr.RemoteUnlockMgr.TimeCnt = Def_GuiTimeDelayCnt3s;
			}
			
			WifiMgr.RemoteUnlockMgr.Status = RemoteUnlockDisplayResult;
			
		}

		if ( WifiMgr.RemoteUnlockMgr.TimeCnt < 3 )
		{
			WifiMgr.RemoteUnlockMgr.Result = IsFail;
			WifiMgr.RemoteUnlockMgr.errorcode = 0x01;
		}
		
	}
	else if ( WifiMgr.RemoteUnlockMgr.Status == RemoteUnlockExit )
	{
		WifiMgr.RemoteUnlockMgr.TimeCnt = 0x00;
		SystemPowerMgr.SleepDelayTimerCnt = Def_GuiTimeDelayCnt3s;
		ComPort_SetPost_ExitRemoteUnlcok();
	}

	if (WifiMgr.RemoteUnlockMgr.TimeCnt > 0)
	{
		WifiMgr.RemoteUnlockMgr.TimeCnt--;
	}
	else
	{
		GoIntoMainScreen_WithIdentifyInit();
	}

}


/*******************************************************/
/*******************************************************/
/*******************************************************/
void VoiceReportCurrentSettingTime_Integer(uint8_t Value)
{
	uint16_t VoiceStr[6];
	VoiceStr[0]=VOICE_Current;
	VoiceStr[1]=VOICE_Setting;
	VoiceStr[2]=TranslateNumberToVoice(Value%100/10);
	VoiceStr[3]=TranslateNumberToVoice(Value%10);
	VoiceStr[4]=VOICE_Second;
	VoiceStr[5]=DEF_VoiceSegmentEndFlag;
	PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr);
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void VoiceReportCurrentSettingTime_Decimal(uint8_t Value)
{
	uint16_t VoiceStr[7];
	VoiceStr[0]=VOICE_Current;
	VoiceStr[1]=VOICE_Setting;
	VoiceStr[2]=TranslateNumberToVoice(Value%100/10);
	VoiceStr[3]=VOICE_Dot;
	VoiceStr[4]=TranslateNumberToVoice(Value%10);
	VoiceStr[5]=VOICE_Second;
	VoiceStr[6]=DEF_VoiceSegmentEndFlag;
	PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr);
}

void VoiceReportCurrentTravelSetting(uint8_t Value)
{
	uint16_t VoiceStr[4];
	VoiceStr[0]=VOICE_Current;
	VoiceStr[1]=VOICE_LockDistance;
	VoiceStr[2]=TranslateNumberToVoice(Value%10);
	VoiceStr[3]=DEF_VoiceSegmentEndFlag;
	PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr);
}

void VoiceReportCurrentUnlockDirrect(void)
{
	uint16_t VoiceStr[4];
	VoiceStr[0]=VOICE_Current;
	VoiceStr[1]=VOICE_Setting;
	if ( AutoMotorMgr.LockDirection == LEFTOPEN )
	{
		VoiceStr[2]=VOICE_LeftOpen;
	}
	else
	{
		VoiceStr[2]=VOICE_RightOpen;
	}
	VoiceStr[3]=DEF_VoiceSegmentEndFlag;
	PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr);
}

void VoiceReportCurrentTorque(void)
{
	uint16_t VoiceStr[5];
	VoiceStr[0]=VOICE_Current;
	VoiceStr[1]=VOICE_Setting;
	if ( AutoMotorMgr.TorqueLevel == LargeTorque )
	{
		VoiceStr[2]=VOICE_Big;
	}
	else if ( AutoMotorMgr.TorqueLevel == MiddleTorque )
	{
		VoiceStr[2]=VOICE_Middle;
	}
	else
	{
		VoiceStr[2]=VOICE_Small;
	}
	VoiceStr[3] = VOICE_Torque;
	VoiceStr[4] = DEF_VoiceSegmentEndFlag;
	PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr);
}


/*******************************************************/
/*******************************************************/
/*******************************************************/
void GotoMotorUnlockTimeSetting_Init(void)
{
	VoiceMenuMgr.MenuPoint = 0;
	VoiceMenuMgr.TotalMenuNum = 3;
	CurrentScreen = SCREEN_AutoMotorUnlockTime;	
}
void ShowAutoMotorUnlockTimeSetting(void)
{
	const uint8_t TitleStr[]={HZ_guanbi,HZ_men,HZ_shi,HZ_jian,HZ_end};							
	const uint8_t TitleStrEN[]={"Unlock time"};	
    //const uint8_t TimeUnitStr[]={HZ_miao,HZ_end};							
	const uint8_t TimeUnitStrEN[]={"S"};	

	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,UnlockTimeSettingVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}

    if (SystemLanguage == Chinese)
     {
	   DisHZ16x14Str(0,32,TitleStr,NormalDisplay);

	}
	else
	{
       DisEN16x8Str(0,24,TitleStrEN,NormalDisplay);
	}
	DisOneDigital16x8(4,48,AutoMotorMgr.UnlockTime/10,InverseDisplay);
	DisOneDigital16x8(4,56,AutoMotorMgr.UnlockTime%10,InverseDisplay);
	DisEN16x8Str(4,68,TimeUnitStrEN,NormalDisplay);	
}
/*******************************************************/
/*******************************************************/
/*******************************************************/
void GotoMotorAutoLockTimeSetting_Init(void)
{
	VoiceMenuMgr.MenuPoint = 0;
	VoiceMenuMgr.TotalMenuNum = 3;
	CurrentScreen = SCREEN_AutoMotorAutoLockTime;	
}

void ShowAutoMotorAutoLockTimeSetting(void)
{
	const uint8_t TitleStr[]={HZ_zhidong,HZ_dong,HZ_shang,HZ_suomen,HZ_shi,HZ_jian,HZ_end};							
	const uint8_t TitleStrEN[]={"AutoLockTime"};	
	//const uint8_t TimeUnitStr[]={HZ_miao,HZ_end};							
	const uint8_t TimeUnitStrEN[]={"S"};	

	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,AutoLockDelayTimeSettingVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}

	if (SystemLanguage == Chinese)
	{
		DisHZ16x14Str(0,16,TitleStr,NormalDisplay);
	}
	else
	{
		DisEN16x8Str(0,16,TitleStrEN,NormalDisplay);
	}

	DisOneDigital16x8(4,40,AutoMotorMgr.AutoLockTime/10,InverseDisplay);
	DisOneDigital16x8(4,48,AutoMotorMgr.AutoLockTime%10,InverseDisplay);
	DisEN16x8Str(4,64,TimeUnitStrEN,NormalDisplay);

}
/*******************************************************/
/*******************************************************/
/*******************************************************/
void GotoMotorLockDirrectionSetting_Init(void)
{
	VoiceMenuMgr.MenuPoint = 0;
	VoiceMenuMgr.TotalMenuNum = 4;
	CurrentScreen = SCREEN_AutoMotorLockDirection;	
}
void ShowAutoMotorLockDirectionSetting(void)
{					

	const  uint8_t LockDirectionStr[][5]=
		{			
			{ZF_1,ZF_xiaoshudian,HZ_zuo,HZ_kai,HZ_end},																
			{ZF_2,ZF_xiaoshudian,HZ_youbian,HZ_kai,HZ_end},
		};
	const  uint8_t LockDirectionStrEN[][8]=
		{			
			{"1.Left  "},																
			{"2.Right "}
		};

	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,UnlockDirrectionSettingVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}

    if (SystemLanguage == Chinese)
    {
    	if ( AutoMotorMgr.LockDirection == LEFTOPEN )
    	{
	     	DisHZ16x14Str(1,4,LockDirectionStr[0],InverseDisplay);		
			DisHZ16x14Str(4,4,LockDirectionStr[1],NormalDisplay);	
    	}
		else
		{
			DisHZ16x14Str(1,4,LockDirectionStr[0],NormalDisplay);		
			DisHZ16x14Str(4,4,LockDirectionStr[1],InverseDisplay);	
		}
	}
	else
	{
    	if ( AutoMotorMgr.LockDirection == LEFTOPEN )
    	{
	     	DisEN16x8Str(1,4,LockDirectionStrEN[0],InverseDisplay);		
			DisEN16x8Str(4,4,LockDirectionStrEN[1],NormalDisplay);	
    	}
		else
		{
			DisEN16x8Str(1,4,LockDirectionStrEN[0],NormalDisplay);		
			DisEN16x8Str(4,4,LockDirectionStrEN[1],InverseDisplay);	
		}	
	}

}
/*******************************************************/
/*******************************************************/
/*******************************************************/

void GotoMotorTorqueSetting_Init(void)
{
	VoiceMenuMgr.MenuPoint = 0;
	VoiceMenuMgr.TotalMenuNum = 5;
	CurrentScreen = SCREEN_AutoMotorTorque;	
}

void ShowAutoMotorTorqueSetting(void)
{						
	const uint8_t TorqueStr[][6]={								
									{ZF_1,ZF_xiaoshudian,HZ_xiao,HZ_niu,HZ_niuli,HZ_end},									
									{ZF_2,ZF_xiaoshudian,HZ_zhong,HZ_niu,HZ_niuli,HZ_end},
									{ZF_3,ZF_xiaoshudian,HZ_da,HZ_niu,HZ_niuli,HZ_end},
							 };

	const uint8_t TorqueStrEN[][9]={
		
								{"1.Little "},									
								{"2.Middle "},
								{"3.Big    "},
						 };
	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,MotorTorqueSettingVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}

	 if (SystemLanguage == Chinese)
     {
     	if (AutoMotorMgr.TorqueLevel == LowTorque)
     	{
			DisHZ16x14Str(1,4,TorqueStr[0],InverseDisplay);
			DisHZ16x14Str(3,4,TorqueStr[1],NormalDisplay);	
			DisHZ16x14Str(5,4,TorqueStr[2],NormalDisplay);
     	}
		else if  (AutoMotorMgr.TorqueLevel == MiddleTorque)
		{
			DisHZ16x14Str(1,4,TorqueStr[0],NormalDisplay);
			DisHZ16x14Str(3,4,TorqueStr[1],InverseDisplay);	
			DisHZ16x14Str(5,4,TorqueStr[2],NormalDisplay);
     	}
		else
		{
			DisHZ16x14Str(1,4,TorqueStr[0],NormalDisplay);
			DisHZ16x14Str(3,4,TorqueStr[1],NormalDisplay);	
			DisHZ16x14Str(5,4,TorqueStr[2],InverseDisplay);
     	}
	 }
	 else
	 {
		if (AutoMotorMgr.TorqueLevel == LowTorque)
     	{
			DisEN16x8Str(1,4,TorqueStrEN[0],InverseDisplay);
			DisEN16x8Str(3,4,TorqueStrEN[1],NormalDisplay);	
			DisEN16x8Str(5,4,TorqueStrEN[2],NormalDisplay);
     	}
		else if  (AutoMotorMgr.TorqueLevel == MiddleTorque)
		{
			DisEN16x8Str(1,4,TorqueStrEN[0],NormalDisplay);
			DisEN16x8Str(3,4,TorqueStrEN[1],InverseDisplay);	
			DisEN16x8Str(5,4,TorqueStrEN[2],NormalDisplay);
     	}
		else
		{
			DisEN16x8Str(1,4,TorqueStrEN[0],NormalDisplay);
			DisEN16x8Str(3,4,TorqueStrEN[1],NormalDisplay);	
			DisEN16x8Str(5,4,TorqueStrEN[2],InverseDisplay);
     	}
	 }
    
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void GotoLockingTravelSetting_Init(void)
{
	VoiceMenuMgr.MenuPoint = 0;
	VoiceMenuMgr.TotalMenuNum = 3;
	CurrentScreen = SCREEN_LockingTravel;
}

void ShowLockingTravelSetting(void)
{
	const uint8_t TitleStr[]={HZ_fanzheng,HZ_suomen,HZ_xing,HZ_chengxu,HZ_end};	
	const uint8_t TitleStrEN[]={"Locking Travel"};	

	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,LockingTravelSettingVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}

	 if (SystemLanguage == Chinese)
     { 
		DisHZ16x14Str(0,32,TitleStr,NormalDisplay);
	 }
	 else
	 {    
		DisEN16x8Str(0,20,TitleStrEN,NormalDisplay);
	 }
     DisOneDigital16x8(3,48,AutoMotorMgr.LockingTravel,InverseDisplay);
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void GotoAutoEjectSetting_Init(void)
{
	VoiceMenuMgr.MenuPoint = 0;
	VoiceMenuMgr.TotalMenuNum = 4;
	CurrentScreen = SCREEN_AutoEject;
}

void ShowAutoEjectSetting(void)
{			
	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,AutoEjectSettingVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}

	if (SystemLanguage == Chinese)
	{
		if (AutoMotorMgr.AutoEject == 0x01)
		{
			DisHZ16x14Str(1,4,AutoEjectEnableStr,InverseDisplay);
			DisHZ16x14Str(3,4,AutoEjectDisableStr,NormalDisplay);
		}
		else
		{
			DisHZ16x14Str(1,4,AutoEjectEnableStr,NormalDisplay);
			DisHZ16x14Str(3,4,AutoEjectDisableStr,InverseDisplay);
		}
	}
	else
	{
		if (AutoMotorMgr.AutoEject == 0x01)
		{
			DisEN16x8Str(1,4,AutoEjectEnableStrEn,InverseDisplay);
			DisEN16x8Str(3,4,AutoEjectDisableStrEn,NormalDisplay);
		}
		else
		{
			DisEN16x8Str(1,4,AutoEjectEnableStrEn,NormalDisplay);
			DisEN16x8Str(3,4,AutoEjectDisableStrEn,InverseDisplay);
		}
	}
			
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void GotoBoltLockTimeSetting_Init(void)
{
	VoiceMenuMgr.MenuPoint = 0;
	VoiceMenuMgr.TotalMenuNum = 3;
	CurrentScreen = SCREEN_AutoMotorBoltLockTime;
}
void ShowBoltLockTimeSetting(void)
{
	const uint8_t TitleStr[]={HZ_xielv,HZ_se,HZ_fanzheng,HZ_suomen,HZ_shi,HZ_jian,HZ_end};	//斜舌反锁时间						
	const uint8_t TitleStrEN[]={"Bolt Lock Time"};	//斜舌反锁时间												
	const uint8_t TimeUnitStrEN[]={"S"};	

	uint16_t temp_BoltLockTime;

	temp_BoltLockTime = AutoMotorMgr.BoltLockTime/2;

	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,BoltLockTimeSettingVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}
    if (SystemLanguage == Chinese)
    {
		DisHZ16x14Str(0,16,TitleStr,NormalDisplay);
	}
	else
	{
		DisEN16x8Str(0,0,TitleStrEN,NormalDisplay);
	}
	DisOneDigital16x8(4,48,temp_BoltLockTime/10,InverseDisplay);
	DisZF16x8(4,56,ZF_xiaoshudian,InverseDisplay);
	DisOneDigital16x8(4,64,temp_BoltLockTime%10,InverseDisplay);
	DisEN16x8Str(4,76,TimeUnitStrEN,NormalDisplay);
}
/*******************************************************/
/*******************************************************/
/*******************************************************/
void GotoMotorSelfTest_Init(void)
{
	VoiceMenuMgr.MenuPoint = 0;
	VoiceMenuMgr.TotalMenuNum = 1;
	CurrentScreen = SCREEN_AutoMotorSelfTest;
}

void ShowAutoMotorSelfTest(void)
{
	const uint8_t TitleStr[]={HZ_dian,HZ_jiqi,HZ_zhidong,HZ_xue,HZ_xiguan,HZ_end};	
	const uint8_t TitleStrEN[]={"Motor Test"};	
	const uint8_t Title1Str[]={HZ_qing,HZ_shao,HZ_houniao,HZ_end};	
	const uint8_t Title1StrEN[]={"Please Wait"};

	const uint8_t SelfTestSuccessStr[]={HZ_xue,HZ_xiguan,HZ_cheng,HZ_gong,HZ_end};	
	const uint8_t SelfTestSuccessStrEN[]={"Success"};	
	const uint8_t SelfTestFailStr[]={HZ_xue,HZ_xiguan,HZ_shibai,HZ_bai,HZ_end};	
	const uint8_t SelfTestFailStrEN[]={"Fail"};	
	const uint8_t MotorWithHallStr[]={HZ_dian,HZ_jiqi,HZ_you,HZ_huoer,HZ_erhou,HZ_end};	
	const uint8_t MotorWithHallStrEN[]={"With Hall"};	
	const uint8_t MotorWithoutHallStr[]={HZ_dian,HZ_jiqi,HZ_wu,HZ_huoer,HZ_erhou,HZ_end};	
	const uint8_t MotorWithoutHallStrEN[]={"Without Hall"};

	
	//if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,MotorSelfTestVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}
	if (SystemLanguage == Chinese)
	 {
		DisHZ16x14Str(0,32,TitleStr,NormalDisplay);
	 }
	 else
	 {
		DisEN16x8Str(0,20,TitleStrEN,NormalDisplay);
	 }

	if ( AutoMotorMgr.SelfTest.Status == SelfTestStart )
	{
		if (SystemLanguage == Chinese)
		 {
			DisHZ16x14Str(3,48,Title1Str,NormalDisplay);
		 }
		 else
		 {
			DisEN16x8Str(3,20,Title1StrEN,NormalDisplay);
		 }
		AutoMotorMgr.SelfTest.TimeLimitedCnt = 30*64;	
		AutoMotorMgr.SelfTest.Status = SelfTestWait;
		GUI_RefreshSleepTime();
	}
	else if ( AutoMotorMgr.SelfTest.Status == SelfTestWait )
	{

	}
	else if (AutoMotorMgr.SelfTest.Status == SelfTestSuccess )
	{
		 Clear_Screen();
		 if (SystemLanguage == Chinese)
	     {
			DisHZ16x14Str(3,40,SelfTestSuccessStr,NormalDisplay);
			if ( AutoMotorMgr.SelfTest.ErrorCode == 0x00 )
			{
				DisHZ16x14Str(5,32,MotorWithHallStr,NormalDisplay);
			}
			else if ( AutoMotorMgr.SelfTest.ErrorCode == 0x01 )
			{
				DisHZ16x14Str(5,32,MotorWithoutHallStr,NormalDisplay);
			}	
		 }
		 else
		 {
			DisEN16x8Str(3,32,SelfTestSuccessStrEN,NormalDisplay);
			if ( AutoMotorMgr.SelfTest.ErrorCode == 0x00 )
			{
				DisEN16x8Str(5,28,MotorWithHallStrEN,NormalDisplay);
			}
			else if ( AutoMotorMgr.SelfTest.ErrorCode == 0x01 )
			{
				DisEN16x8Str(5,16,MotorWithoutHallStrEN,NormalDisplay);
			}
		 }
		PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_OperationSuccess); 
		AutoMotorMgr.SelfTest.Status = SelfTestDisplayResult;
		AutoMotorMgr.SelfTest.TimeLimitedCnt = Def_GuiTimeDelayCnt3s;
		
	}
	else if (AutoMotorMgr.SelfTest.Status == SelfTestFail )
	{
		Clear_Screen();
		if (SystemLanguage == Chinese)
		{
		   DisHZ16x14Str(3,32,SelfTestFailStr,NormalDisplay);
		}
		else
		{
		   DisEN16x8Str(3,16,SelfTestFailStrEN,NormalDisplay);
		}
		PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_OperationFail);
		AutoMotorMgr.SelfTest.Status = SelfTestDisplayResult;
		AutoMotorMgr.SelfTest.TimeLimitedCnt = Def_GuiTimeDelayCnt3s;
	}
	else if (AutoMotorMgr.SelfTest.Status == SelfTestDisplayResult )
	{	
		if (AutoMotorMgr.SelfTest.TimeLimitedCnt > 0 )
	 	{
			AutoMotorMgr.SelfTest.TimeLimitedCnt--;
	 	}
		else
	 	{
	 		if ( AutoMotorMgr.SelfTest.Status == SelfTestWait )
	 		{
				AutoMotorMgr.SelfTest.Status = SelfTestFail;
	 		}
			else
			{
				GoIntoMainScreen_WithIdentifyInit();	
			}
	 	}
	}
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowFrmConnectionCheck(void)
{
	if (FrmConnectionCheckMgr.Status == FrmCheckInit )
	{
		FaceRecognition_HardwarePowerOff();
		FrmConnectionCheckMgr.TimeCnt = Def_GuiTimeDelayCnt025s;
		FrmConnectionCheckMgr.Status = FrmCheckPowerOff;
		FrmConnectionCheckMgr.FrmCheckFailTimes = 0x00;
	}
	else if (FrmConnectionCheckMgr.Status == FrmCheckPowerOff )
	{
		if (--FrmConnectionCheckMgr.TimeCnt < 1 )
		{
			FaceRecognition_HardwarePowerOn();
			FrmConnectionCheckMgr.Status = FrmCheckPowerOn;
			FrmConnectionCheckMgr.TimeCnt = Def_GuiTimeDelayCnt1s5;
		}
	}
	else if (FrmConnectionCheckMgr.Status == FrmCheckPowerOn )
	{
		FaceRecognitionMgr_Task();
		if ( FrmMgr.PostFlag_Ready == bTRUE)
		{
			DEBUG_MARK;
			DEBUG_MARK;
			FaceRecognition_HardwarePowerOff();
			FrmConnectionCheckMgr.Status = FrmCheckStop;
			FrmMgr.FrmFunctionConfig = FRM_Enabled;
		}
		if (--FrmConnectionCheckMgr.TimeCnt < 1 )
		{
			FrmConnectionCheckMgr.Status = FrmCheckReset;
			FaceRecognition_Reset();
			//FaceRecognition_GetVerionNumber();
			FrmConnectionCheckMgr.TimeCnt = Def_GuiTimeDelayCnt1s;
		}		
	}
	else if (FrmConnectionCheckMgr.Status == FrmCheckReset )
	{
		FaceRecognitionMgr_Task();
		UART3_TX_Task();
		if ( FrmMgr.PostFlag_ResetResult == bTRUE )
		{
			DEBUG_MARK;
			DEBUG_MARK;
			FaceRecognition_HardwarePowerOff();
			FrmConnectionCheckMgr.Status = FrmCheckStop;
			FrmMgr.FrmFunctionConfig = FRM_Enabled;	
		}
		if (--FrmConnectionCheckMgr.TimeCnt < 1 )
		{
			if ( ++FrmConnectionCheckMgr.FrmCheckFailTimes < 2 )
			{
				FaceRecognition_HardwarePowerOff();
				FrmConnectionCheckMgr.TimeCnt = Def_GuiTimeDelayCnt025s;
				FrmConnectionCheckMgr.Status = FrmCheckPowerOff;
				DEBUG_MARK;				
			}
			else
			{
				FaceRecognition_HardwarePowerOff();
				FrmConnectionCheckMgr.Status = FrmCheckStop;
				FrmMgr.FrmFunctionConfig = FRM_Disabled;
			}
		}
	}
}
void ShowInitialization(void)
{
	
	//DisHZ16x14Str(2,16,Str,NormalDisplay);
	uint8_t i;
	const uint8_t SystenRebootStr[]={HZ_xi,HZ_tong,HZ_qidong,HZ_dong,HZ_end};
	const uint8_t SystenRebootStr_En[]={"[Rebooting]"};
	const uint8_t FingprintStr[]={HZ_zhi,HZ_wen,HZ_end}; 
	const uint8_t FingprintStr_En[]={"Fingerprint"}; 
	const uint8_t CardReaderStr[]={HZ_du,HZ_ka,HZ_end}; 
	const uint8_t CardReaderStr_En[]={"Card Reader"}; 	
	const uint8_t EepromStr[]={HZ_nei,HZ_cun,HZ_end}; 
	const uint8_t EepromStr_En[]={"Memory"}; 
	const uint8_t TouchStr[]={HZ_chumo,HZ_motou,HZ_end}; 
	const uint8_t TouchStr_En[]={"Touch"}; 
	const uint8_t SystemCheckPassStr[]={HZ_xi,HZ_tong,HZ_zhengque,HZ_changchang,HZ_end}; 
	const uint8_t SystemCheckPassStr_En[]={"System OK"}; 
	const uint8_t SystemCheckFailStr[]={HZ_xi,HZ_tong,HZ_bu,HZ_zhengque,HZ_changchang,HZ_end}; 
	const uint8_t SystemCheckFailStr_En[]={"System NG"}; 	

	if ( InitializationMgr.Status == StartInitialization )
	{
		//SystemConfigLoad();
		if ( SystemLanguage == Chinese ){
			DisEN16x8Str(1,28,"[",NormalDisplay);	//display "["
			DisHZ16x14Str(1,36,SystenRebootStr,NormalDisplay);	//display "reboot"
			DisEN16x8Str(1,92,"]",NormalDisplay);	//display "]"
		}
		else{
			DisEN16x8Str(1,20,SystenRebootStr_En,NormalDisplay);	//display "reboot"
		}		
		CheckMemoryMgr.Status = StartCheckMemory;
		InitializationMgr.Status = MemoryCheck;
	}
	else if ( InitializationMgr.Status == MemoryCheck )
	{
	
		if ( SystemLanguage == Chinese ){
			DisHZ16x14Str(4,36,EepromStr,NormalDisplay);//display "EEPROM TEST"
		}
		else{
			DisEN16x8Str(4,40,EepromStr_En,NormalDisplay);
		}
		//SystemConfigLoad();
		ReadLockBrand();

		InitializationMgr.MemoryCheckPass = bTRUE;
		InitializationMgr.Status = MemoryCheckWait;
		InitializationMgr.TimeCnt = Def_GuiTimeDelayCnt1s5;
		PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_Beep);
	}
	else if ( InitializationMgr.Status == MemoryCheckWait )
	{
		if (InitializationMgr.MemoryCheckPass == bTRUE)
		{	
			DisImage(4,105,16,16,Icon_Pass,NormalDisplay);
		}
		else
		{
			DisImage(4,105,16,16,Icon_Fail,NormalDisplay);
		}
		if (--InitializationMgr.TimeCnt < 1 )
		{	
			InitializationMgr.Status = FingerPrintCheck;
			Clear_Screen_Page(4);
			Clear_Screen_Page(5);
			if ( SystemLanguage == Chinese ){
				DisHZ16x14Str(4,36,FingprintStr,NormalDisplay);	//display "FPM TEST"
			}
			else{
				DisEN16x8Str(4,4,FingprintStr_En,NormalDisplay);
			}
		}
	}

	else if ( InitializationMgr.Status == FingerPrintCheck )
	{
		GUI_GetUserNumList();		//read user number
		if ( CheckMemoryMgr.Status == CheckMemorySuccess )
		{
			GUI_SetFPM_LED(DEF_FpmLedMode_Breath,DEF_FpmLedColor_Blue,DEF_FpmLedColor_Blue,255);

			FpmAckMgr.Status = WaitACK;
			FpmAckMgr.TimeOutCnt = DEF_FpmAckTimeoutTime;
			FPM_TurnOffAntiFakeFp();
			for (i=0;i<25;i++)
			{
				Hardware_DelayMs(10);
				FPM_Mgr_Task();
				if ( FpmAckMgr.Status == GotACK )
				{
					break;
				}
			}

			InitializationMgr.FingerPrintCheckPass = bTRUE;
			InitializationMgr.Status = FingerPrintCheckWait;
			InitializationMgr.TimeCnt = Def_GuiTimeDelayCnt1s;
			PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_Beep);
			
		}
		else if  ( CheckMemoryMgr.Status == CheckMemoryFail )
		{	
			InitializationMgr.FingerPrintCheckPass = bFALSE;
			InitializationMgr.Status = FingerPrintCheckWait;
			InitializationMgr.TimeCnt = Def_GuiTimeDelayCnt1s;
			PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_FailVoice);
		}
	}
	else if  ( InitializationMgr.Status == FingerPrintCheckWait )
	{
		if (InitializationMgr.FingerPrintCheckPass == bTRUE)
		{	
			DisImage(4,105,16,16,Icon_Pass,NormalDisplay);
		}
		else
		{
			DisImage(4,105,16,16,Icon_Fail,NormalDisplay);
		}
		if (--InitializationMgr.TimeCnt < 1 )
		{
			InitializationMgr.Status = CardReaderCheck;
			Clear_Screen_Page(4);
			Clear_Screen_Page(5);	
			DisHZ16x14Str(4,36,CardReaderStr,NormalDisplay); 
			if ( SystemLanguage == Chinese ){
				DisHZ16x14Str(4,36,CardReaderStr,NormalDisplay); 
			}
			else{
				DisEN16x8Str(4,4,CardReaderStr_En,NormalDisplay);
			}
		}
	}
	else if ( InitializationMgr.Status == CardReaderCheck )
	{
		InitializationMgr.CardReaderCheckPass = bTRUE;
		InitializationMgr.Status = CardReaderCheckWait;
		InitializationMgr.TimeCnt = Def_GuiTimeDelayCnt1s;
		PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_Beep);
	}
	else if ( InitializationMgr.Status == CardReaderCheckWait )
	{
		if (InitializationMgr.CardReaderCheckPass == bTRUE)
		{	
			DisImage(4,105,16,16,Icon_Pass,NormalDisplay);
		}
		else
		{
			DisImage(4,105,16,16,Icon_Fail,NormalDisplay);
		}
		if (--InitializationMgr.TimeCnt < 1 )
		{
			InitializationMgr.Status = TouchCheck;
			Clear_Screen_Page(4);
			Clear_Screen_Page(5);	
			if ( SystemLanguage == Chinese ){
				DisHZ16x14Str(4,36,TouchStr,NormalDisplay);
			}
			else{
				DisEN16x8Str(4,36,TouchStr_En,NormalDisplay);
			}
		}
	}
	else if ( InitializationMgr.Status == TouchCheck )
		{
			InitializationMgr.TouchCheckPass = bTRUE;
			InitializationMgr.Status = TouchCheckWait;
			InitializationMgr.TimeCnt = Def_GuiTimeDelayCnt1s;
 
			PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_Beep);
		}
	else if ( InitializationMgr.Status == TouchCheckWait )
		{
			if (InitializationMgr.TouchCheckPass == bTRUE)
			{	
				DisImage(4,105,16,16,Icon_Pass,NormalDisplay);
			}
			else
			{
				DisImage(4,105,16,16,Icon_Fail,NormalDisplay);
			}
			if (--InitializationMgr.TimeCnt < 1 )
			{
				//Clear_Screen();
				Clear_Screen_Page(4);
				Clear_Screen_Page(5);
				InitializationMgr.Status = CheckFinished;
				InitializationMgr.TimeCnt = Def_GuiTimeDelayCnt1s;
				if( (InitializationMgr.FingerPrintCheckPass == bTRUE )
					&&(InitializationMgr.MemoryCheckPass == bTRUE )
					&&(InitializationMgr.CardReaderCheckPass == bTRUE )
					&&(InitializationMgr.TouchCheckPass == bTRUE )
				)
				{
					if ( SystemLanguage == Chinese ){
						DisHZ16x14Str(4,36,SystemCheckPassStr,NormalDisplay);
					}
					else{
						DisEN16x8Str(4,24,SystemCheckPassStr_En,NormalDisplay);
					}
				}
				else{
					if ( SystemLanguage == Chinese ){
						DisHZ16x14Str(4,28,SystemCheckFailStr,NormalDisplay);
					}
					else{
						DisEN16x8Str(4,24,SystemCheckFailStr_En,NormalDisplay);
					}
				}
			}
		}

	
	else if ( InitializationMgr.Status == CheckFinished )
	{
		if (--InitializationMgr.TimeCnt < 1 )
		{
			if (( BatteryMgr.BatteryLevel == LEVEL_0 )||( BatteryMgr.BatteryLevel == LEVEL_1 ))
			{
				PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_PleaseReplaceTheBattery);
				BatteryMgr.PostLowBattery = bFALSE;
				BatteryMgr.TimeCnt = Def_MessageBoxTimeDelay;
				CurrentScreen = SCREEN_LowBattery;	
				
				ComPort_SetPost_Info(DEF_WifiInfo_LowBattery,0x00,0x00);
				#if defined (Function_YouzhiyunjiaWifi) || defined (Function_TuyaWifi)
				Wifi_PostEvent(DEF_WifiEvent_LowBattery,0x00,0x00);
				#endif
			
			}
			else
			{
				GoIntoMainScreen_WithIdentifyInit();
              	PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_POWERON);
			}
		}
	}
		
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowSystemLocked(void)
{
	uint16_t Temp;
	if ( SafetyMonitorMgr.SystemLockedTimeDelay > 0x0000 )
	{
		if ( SafetyMonitorMgr.SystemLockedTimeDelay > (DEF_SystemLockedTime-640))
		{
			if ( SafetyMonitorMgr.SystemLockedTimeDelay%128 == 0  )
			{
				PLAY_VOICE_ONESEGMENT(3,VOICE_Alarm);
			}
		}
		SafetyMonitorMgr.SystemLockedTimeDelay--;
	}
	else
	{
		SafetyMonitorMgr.FpIdentifyFailedTimes = 0x00;
        SafetyMonitorMgr.CardIdentifyFailedTimes = 0x00;
        SafetyMonitorMgr.PasscodeIdentifyFailedTimes = 0x00;
		SafetyMonitorMgr.ManagerPasscodeIdentifyFailedTimes = 0x00;
		SafetyMonitorMgr.ManagerFpIdentifyFailedTimes = 0x00;
		SafetyMonitorMgr.SystemLocked = bFALSE;
		GoIntoMainScreen_WithIdentifyInit();
		STOP_VOICEPLAY();
	}

	DisImage(0,50,27,24,Icon_Warning,NormalDisplay);

	if (SystemLanguage == Chinese){
		DisHZ16x14Str(4,36,SystemLockedStr,NormalDisplay);
	}
	else{
		DisEN16x8Str(4,16,SystemLockedStrEn,NormalDisplay);
	}
	
	Temp = SafetyMonitorMgr.SystemLockedTimeDelay/64;
	
	DisOneDigital16x8(6,52,Temp/100,NormalDisplay);
	DisOneDigital16x8(6,60,(Temp%100)/10,NormalDisplay);
	DisOneDigital16x8(6,68,Temp%10,NormalDisplay);
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowPickLockAlarm(void)
{
	bool_t Flag_ExitPickLockAlarm;
	uint8_t i;

	GUI_RefreshSleepTime();		//reset system sleep time

	if ( PasscodeUserIdentifyMgr.Status == PasscodeIdentifyIdle )
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(0,36,AntiPryingAlarmStr,NormalDisplay);
			DisHZ16x14Str(3,16,RemovalAlarmStr1,NormalDisplay);
			DisHZ16x14Str(5,50,RemovalAlarmStr2,NormalDisplay); 
		}
		else{
			DisEN16x8Str(0,44,AntiPryingAlarmStrEn,NormalDisplay);
			DisEN16x8Str(3,12,RemovalAlarmStr1En,NormalDisplay);
			DisEN16x8Str(5,24,RemovalAlarmStr2En,NormalDisplay); 
		}

	}

	Flag_ExitPickLockAlarm = bFALSE;
	if (( CardIdentifyMgr.CardDetectIntervalTimeCnt ==0 ) )	//FPM Cmd is sent out
	{
		CardIdentifyMgr.CardDetectIntervalTimeCnt = Def_CardDetectIntervalTime;
		CardIdentifyMgr.DelayTimeAfterReadCard = 0;
		CardUserIdentify();
	}
		
	if ( CardIdentifyMgr.Status == Success)
	{
		Flag_ExitPickLockAlarm = bTRUE;
	}
	else if ( CardIdentifyMgr.Status == Fail)
	{
		if ( IfSystemIsInFactoryDefaultStatus()==bTRUE )
		{
			Flag_ExitPickLockAlarm = bTRUE;
		}
		else
		{
			CardIdentifyMgr.Status = ReadingCardID;
		}
	}

	FpUserIdentify();
	if ( FpIdentifyMgr.Status == success)
	{
		Flag_ExitPickLockAlarm = bTRUE;	
	}
	else if ( FpIdentifyMgr.Status == fail)
	{
		if ( IfSystemIsInFactoryDefaultStatus()==bTRUE )
		{
			Flag_ExitPickLockAlarm = bTRUE;		
		}
		else
		{
			FpIdentifyMgr.Status = FPMcmdStart;
		}
	}

	PasscodeUserIdentify();
	if (PasscodeUserIdentifyMgr.Status == PasscodeIdentifySuccess)
	{
		Flag_ExitPickLockAlarm = bTRUE;
	}
	else if ( PasscodeUserIdentifyMgr.Status == PasscodeIdentifyFail )
	{
		if ( IfSystemIsInFactoryDefaultStatus()==bTRUE )
		{
			Flag_ExitPickLockAlarm = bTRUE;
		}
		else
		{
			PasscodeUserIdentifyMgr.Status = PasscodeIdentifyIdle;
		}
	}

	if ( FrmMgr.FrmFunctionConfig == FRM_Enabled )
	{
		if ( PasscodeInputMgr.Point > 0 )
		{
			FaceIdentifyMgr.IndentifyDelayTimeCnt = Def_GuiTimeDelayCnt2s;
		}
		
		if ( FaceIdentifyMgr.IndentifyDelayTimeCnt > 0 )
		{
			 FaceIdentifyMgr.IndentifyDelayTimeCnt--;	 
		}
		if ( (IfSystemIsNoFaceUser() == bFALSE )			//system is has face user
			&&( FaceIdentifyMgr.IndentifyDelayTimeCnt == 0x0000 )
			)
		{
			FaceUserIdentify();
			if ( FaceIdentifyMgr.Status == FaceIdentifySuccess )
			{
				Flag_ExitPickLockAlarm = bTRUE;
			}
			else if ( FaceIdentifyMgr.Status == FaceIdentifySuccess_NoUser)
			{
				Flag_ExitPickLockAlarm = bTRUE;	
			}
			else if ( FaceIdentifyMgr.Status == FaceIdentifyFail )
			{
				FaceIdentifyMgr.Status = FrmIdentifyStart;
				DEBUG_MARK;
			}
		}
		else
		{
			if ( FrmMgr.PowerStatus != FRM_PowerOff )
			{
				FaceRecognition_HardwarePowerOff();
			}	
		}
	}

	
	
	if ( AntiPryingMgr.TimeCnt%128 == 0  )
	{
		PLAY_VOICE_ONESEGMENT(3,VOICE_Alarm);
	}

	if ( AntiPryingMgr.TimeCnt > 0 ){
		AntiPryingMgr.TimeCnt--;
	}
	else{
		Flag_ExitPickLockAlarm = bTRUE;
	}
	
	if ( Flag_ExitPickLockAlarm == bTRUE )
	{
		ComPort_SetPost_Alarm(DEF_WifiAlarm_RemoveAlarm,UNDEFINEDUSER,0x00);
		GUI_SetFPM_LED(DEF_FpmLedMode_Breath,DEF_FpmLedColor_Blue,DEF_FpmLedColor_Blue,255);
		AntiPryingMgr.AntiPryingTrigger = bFALSE;
		GoIntoMainScreen_WithIdentifyInit();
		GUI_RefreshSleepTime();		//reset system sleep time
 		STOP_VOICEPLAY();
	}
}


void VoiceReportOneEventLog(uint16_t EventLogID)
{
	static uint16_t VoiceStr_UnlockLog[]={VOICE_Log,VOICE_Type,VOICE_Unlock,DEF_VoiceSegmentEndFlag};
	static uint16_t VoiceStr_AddUser[]={VOICE_Log,VOICE_Type,VOICE_Add,VOICE_User,DEF_VoiceSegmentEndFlag};
	static uint16_t VoiceStr_DeleteUser[]={VOICE_Log,VOICE_Type,VOICE_Delete,VOICE_User,DEF_VoiceSegmentEndFlag};

	uint16_t VoiceStr_LogID[6];//={VOICE_Log,VOICE_ID,DEF_VoiceSegmentEndFlag};

	uint16_t VoiceStr_LogTime[10];

	uint16_t VoiceStr_FaceUserID[8];
	uint16_t VoiceStr_FpUserID[8];
	uint16_t VoiceStr_PasswordUserID[8];
	uint16_t VoiceStr_CardUserID[8];
	uint16_t VoiceStr_TempUserID[8];

	if ( VoiceReportLogMgr.Status == ReportLogInit )
	{
		VoiceReportLogMgr.TimeCnt = Def_GuiTimeDelayCnt05s;
		VoiceReportLogMgr.Status = ReportLogID;
	}
	else if ( VoiceReportLogMgr.Status == ReportLogID )
	{	
		VoiceStr_LogID[0] = VOICE_Log;
		VoiceStr_LogID[1] = VOICE_ID;
		VoiceStr_LogID[2] = TranslateNumberToVoice(EventLogID/100);
		VoiceStr_LogID[3] = TranslateNumberToVoice(EventLogID%100/10);
		VoiceStr_LogID[4] = TranslateNumberToVoice(EventLogID%10);
		VoiceStr_LogID[5] = DEF_VoiceSegmentEndFlag;
		PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_LogID);
		VoiceReportLogMgr.Status = ReportLogType;
		VoiceReportLogMgr.TimeCnt = Def_GuiTimeDelayCnt3s;
	}	

	else if ( VoiceReportLogMgr.Status == ReportLogType )
	{
		if ( VoiceReportLogMgr.TimeCnt--< 1 )
		{
			if ( LogMgr.DisplayLog.EventType == OpenDoor )
			{
				PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_UnlockLog);
				VoiceReportLogMgr.TimeCnt = Def_GuiTimeDelayCnt2s5;
			}
			else if (( LogMgr.DisplayLog.EventType == AddFaceUser )
					||( LogMgr.DisplayLog.EventType == AddFpUser )
					||( LogMgr.DisplayLog.EventType == AddPasscodeUser )
					||( LogMgr.DisplayLog.EventType == AddCardUser )
					)
			{
				PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_AddUser);
				VoiceReportLogMgr.TimeCnt = Def_GuiTimeDelayCnt3s;
			}

			else if (( LogMgr.DisplayLog.EventType == DeleteFaceUser )
					||( LogMgr.DisplayLog.EventType == DeleteFpUser )
					||( LogMgr.DisplayLog.EventType == DeleteAllStressFpUser )
					||( LogMgr.DisplayLog.EventType == DeletePasscodeUser )
					||( LogMgr.DisplayLog.EventType == DeleteCardUser )
					||( LogMgr.DisplayLog.EventType == DeleteAllFpUser )
					||( LogMgr.DisplayLog.EventType == DeleteAllCardUser )
					||( LogMgr.DisplayLog.EventType == DeleteAllPasscodeUser ) 
					)
			{
				PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_DeleteUser);
				VoiceReportLogMgr.TimeCnt = Def_GuiTimeDelayCnt3s;
			}
					
			VoiceReportLogMgr.Status = ReportLogYear;	
		}
	}

	else if ( VoiceReportLogMgr.Status == ReportLogYear )
	{
		if ( VoiceReportLogMgr.TimeCnt--< 1 )
		{
			VoiceStr_LogTime[0]=VOICE_Two;
			VoiceStr_LogTime[1]=VOICE_Zero;
			VoiceStr_LogTime[2]=TranslateNumberToVoice(LogMgr.DisplayLog.year/16);
			VoiceStr_LogTime[3]=TranslateNumberToVoice(LogMgr.DisplayLog.year%16);
			VoiceStr_LogTime[4]=VOICE_Year;
			VoiceStr_LogTime[5]=DEF_VoiceSegmentEndFlag;
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_LogTime);
			VoiceReportLogMgr.TimeCnt = Def_GuiTimeDelayCnt2s5;
			VoiceReportLogMgr.Status = ReportLogMonth;
		}
	}
	else if ( VoiceReportLogMgr.Status == ReportLogMonth )
	{	
		if ( VoiceReportLogMgr.TimeCnt--< 1 )
		{
			VoiceStr_LogTime[0]=TranslateNumberToVoice(LogMgr.DisplayLog.month/16);
			VoiceStr_LogTime[1]=TranslateNumberToVoice(LogMgr.DisplayLog.month%16);
			VoiceStr_LogTime[2]=VOICE_Month;
			VoiceStr_LogTime[3]=DEF_VoiceSegmentEndFlag;
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_LogTime);
			VoiceReportLogMgr.TimeCnt = Def_GuiTimeDelayCnt1s5;
			VoiceReportLogMgr.Status = ReportLogDate;
		}
	}
	else if ( VoiceReportLogMgr.Status == ReportLogDate )
	{	
		if ( VoiceReportLogMgr.TimeCnt--< 1 )
		{
			VoiceStr_LogTime[0]=TranslateNumberToVoice(LogMgr.DisplayLog.date/16);
			VoiceStr_LogTime[1]=TranslateNumberToVoice(LogMgr.DisplayLog.date%16);
			VoiceStr_LogTime[2]=VOICE_Date;
			VoiceStr_LogTime[3]=DEF_VoiceSegmentEndFlag;
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_LogTime);
			VoiceReportLogMgr.TimeCnt = Def_GuiTimeDelayCnt1s5;
			VoiceReportLogMgr.Status = ReportLogHour;
		}
	}	
	else if ( VoiceReportLogMgr.Status == ReportLogHour )
	{	
		if ( VoiceReportLogMgr.TimeCnt--< 1 )
		{
			VoiceStr_LogTime[0]=TranslateNumberToVoice(LogMgr.DisplayLog.hour/16);
			VoiceStr_LogTime[1]=TranslateNumberToVoice(LogMgr.DisplayLog.hour%16);
			VoiceStr_LogTime[2]=VOICE_Hour;
			VoiceStr_LogTime[3]=DEF_VoiceSegmentEndFlag;
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_LogTime);
			VoiceReportLogMgr.TimeCnt = Def_GuiTimeDelayCnt1s5;
			VoiceReportLogMgr.Status = ReportLogMinute;
		}
	}
	else if ( VoiceReportLogMgr.Status == ReportLogMinute )
	{	
		if ( VoiceReportLogMgr.TimeCnt--< 1 )
		{
			VoiceStr_LogTime[0]=TranslateNumberToVoice(LogMgr.DisplayLog.minute/16);
			VoiceStr_LogTime[1]=TranslateNumberToVoice(LogMgr.DisplayLog.minute%16);
			VoiceStr_LogTime[2]=VOICE_Minute;
			VoiceStr_LogTime[3]=DEF_VoiceSegmentEndFlag;
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_LogTime);
			VoiceReportLogMgr.TimeCnt = Def_GuiTimeDelayCnt1s5;
			VoiceReportLogMgr.Status = ReportLogSecond;
		}
	}
	else if ( VoiceReportLogMgr.Status == ReportLogSecond )
	{	
		if ( VoiceReportLogMgr.TimeCnt--< 1 )
		{
			VoiceStr_LogTime[0]=TranslateNumberToVoice(LogMgr.DisplayLog.second/16);
			VoiceStr_LogTime[1]=TranslateNumberToVoice(LogMgr.DisplayLog.second%16);
			VoiceStr_LogTime[2]=VOICE_Second;
			VoiceStr_LogTime[3]=DEF_VoiceSegmentEndFlag;
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_LogTime);
			VoiceReportLogMgr.TimeCnt = Def_GuiTimeDelayCnt1s5;
			VoiceReportLogMgr.Status = ReportLogUserID;
		}
	}

	else if ( VoiceReportLogMgr.Status == ReportLogUserID )
	{
		if ( VoiceReportLogMgr.TimeCnt--< 1 )
		{
			if ( LogMgr.DisplayLog.UserType == FACEUSER )
			{

				VoiceStr_FaceUserID[0] = VOICE_Face;
				VoiceStr_FaceUserID[1] = VOICE_ID;
				VoiceStr_FaceUserID[2] = TranslateNumberToVoice(LogMgr.DisplayLog.UserID/100);
				VoiceStr_FaceUserID[3] = TranslateNumberToVoice(LogMgr.DisplayLog.UserID%100/10);
				VoiceStr_FaceUserID[4] = TranslateNumberToVoice(LogMgr.DisplayLog.UserID%10);
				VoiceStr_FaceUserID[5] = DEF_VoiceSegmentEndFlag;

				PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_FaceUserID);
			}
			else if ( LogMgr.DisplayLog.UserType == CARDUSER )
			{
				if ( LogMgr.DisplayLog.EventType != DeleteAllCardUser )
				{
					VoiceStr_CardUserID[0] = VOICE_Card;
					VoiceStr_CardUserID[1] = VOICE_ID;
					VoiceStr_CardUserID[2] = TranslateNumberToVoice(LogMgr.DisplayLog.UserID/100);
					VoiceStr_CardUserID[3] = TranslateNumberToVoice(LogMgr.DisplayLog.UserID%100/10);
					VoiceStr_CardUserID[4] = TranslateNumberToVoice(LogMgr.DisplayLog.UserID%10);
					VoiceStr_CardUserID[5] = DEF_VoiceSegmentEndFlag;
				}
				else
				{
					VoiceStr_CardUserID[0] = VOICE_All;
					VoiceStr_CardUserID[1] = VOICE_Card;
					VoiceStr_CardUserID[2] = DEF_VoiceSegmentEndFlag;
				}
				PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_CardUserID);
			}
			else if ( LogMgr.DisplayLog.UserType ==  FPUSER )
			{

				if ( LogMgr.DisplayLog.EventType != DeleteAllFpUser )
				{
					VoiceStr_FpUserID[0] = VOICE_Fingerprint;
					VoiceStr_FpUserID[1] = VOICE_ID;
					VoiceStr_FpUserID[2] = TranslateNumberToVoice(LogMgr.DisplayLog.UserID/100);
					VoiceStr_FpUserID[3] = TranslateNumberToVoice(LogMgr.DisplayLog.UserID%100/10);
					VoiceStr_FpUserID[4] = TranslateNumberToVoice(LogMgr.DisplayLog.UserID%10);
					VoiceStr_FpUserID[5] = DEF_VoiceSegmentEndFlag;
				}
				else
				{
					VoiceStr_FpUserID[0] = VOICE_All;
					VoiceStr_FpUserID[1] = VOICE_Fingerprint;
					VoiceStr_FpUserID[2] = DEF_VoiceSegmentEndFlag;
				}
				PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_FpUserID);

			}
			else if (LogMgr.DisplayLog.UserType ==  PASSCODEUSER )
			{
				if ( LogMgr.DisplayLog.EventType != DeleteAllPasscodeUser )
				{
					VoiceStr_PasswordUserID[0] = VOICE_Password;
					VoiceStr_PasswordUserID[1] = VOICE_ID;
					VoiceStr_PasswordUserID[2] = TranslateNumberToVoice(LogMgr.DisplayLog.UserID/100);
					VoiceStr_PasswordUserID[3] = TranslateNumberToVoice(LogMgr.DisplayLog.UserID%100/10);
					VoiceStr_PasswordUserID[4] = TranslateNumberToVoice(LogMgr.DisplayLog.UserID%10);
					VoiceStr_PasswordUserID[5] = DEF_VoiceSegmentEndFlag;
				}
				else
				{
					VoiceStr_PasswordUserID[0] = VOICE_All;
					VoiceStr_PasswordUserID[1] = VOICE_Password;
					VoiceStr_PasswordUserID[2] = DEF_VoiceSegmentEndFlag;
				}
				PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_PasswordUserID);
			}
			else if (LogMgr.DisplayLog.UserType == ONLINEPASSCODEUSER )
			{
				VoiceStr_TempUserID[0] = VOICE_Temporary;
				VoiceStr_TempUserID[1] = VOICE_Password;
				VoiceStr_TempUserID[2] = VOICE_ID;
				VoiceStr_TempUserID[3] = TranslateNumberToVoice(LogMgr.DisplayLog.UserID/100);
				VoiceStr_TempUserID[4] = TranslateNumberToVoice(LogMgr.DisplayLog.UserID%100/10);
				VoiceStr_TempUserID[5] = TranslateNumberToVoice(LogMgr.DisplayLog.UserID%10);
				VoiceStr_TempUserID[6] = DEF_VoiceSegmentEndFlag;
				PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_TempUserID);
			}
			else
			{

			}
			VoiceReportLogMgr.TimeCnt = Def_GuiTimeDelayCnt3s;
			VoiceReportLogMgr.Status = ReportLogEnd;
		}
	}
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void DisplayOneEventLog(uint16_t EventLogID)
{
	const uint8_t UnlockLogStr[]={HZ_kai,HZ_suomen,HZ_ji,HZ_lu,ZF_hengan,HZ_end};
	const uint8_t UnlockLogStrEn[]={"Unlock-"}; 
	const uint8_t AddUserStr[]={HZ_tian,HZ_jia,HZ_yong,HZ_hu,ZF_hengan,HZ_end};
	const uint8_t AddUserStrEn[]={"Add User-"};
	const uint8_t DeleteUserStr[]={HZ_shan,HZ_chufa,HZ_yong,HZ_hu,ZF_hengan,HZ_end};
	const uint8_t DeleteUserStrEn[]={"Del. User-"};
	const uint8_t LotStr[]={HZ_piliang,HZ_liang,HZ_end};
	const uint8_t LotStrEn[]={"Lot"};
	
	if ( LogMgr.DisplayLog.EventType == OpenDoor ) 
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(0,24,UnlockLogStr,NormalDisplay);
		}
		else{
			DisEN16x8Str(0,32,UnlockLogStrEn,NormalDisplay);
		}
	}
	else if (( LogMgr.DisplayLog.EventType == AddFpUser )
			||( LogMgr.DisplayLog.EventType == AddPasscodeUser )
			||( LogMgr.DisplayLog.EventType == AddCardUser )
			||( LogMgr.DisplayLog.EventType == AddFaceUser )
			)
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(0,24,AddUserStr,NormalDisplay);
		}
		else{
			DisEN16x8Str(0,14,AddUserStrEn,NormalDisplay);
		}
	}

	else if (( LogMgr.DisplayLog.EventType == DeleteFaceUser )
			||( LogMgr.DisplayLog.EventType == DeleteFpUser )
			||( LogMgr.DisplayLog.EventType == DeleteAllStressFpUser )
			||( LogMgr.DisplayLog.EventType == DeletePasscodeUser )
			||( LogMgr.DisplayLog.EventType == DeleteCardUser )
			||( LogMgr.DisplayLog.EventType == DeleteAllFpUser )
			||( LogMgr.DisplayLog.EventType == DeleteAllCardUser )
			||( LogMgr.DisplayLog.EventType == DeleteAllPasscodeUser ) 
			)
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(0,24,DeleteUserStr,NormalDisplay);
		}
		else{
			DisEN16x8Str(0,8,DeleteUserStrEn,NormalDisplay);
		}
	}

	DisDigital16x8Str(0,88,EventLogID,NormalDisplay);

	DisBcdDigital16x8(2,28,LogMgr.DisplayLog.year,NormalDisplay);
	DisZF16x8(2,44,ZF_hengan,NormalDisplay);
	DisBcdDigital16x8(2,52,LogMgr.DisplayLog.month,NormalDisplay);
	DisZF16x8(2,68,ZF_hengan,NormalDisplay);
	DisBcdDigital16x8(2,76,LogMgr.DisplayLog.date,NormalDisplay);
	
	DisBcdDigital16x8(4,28,LogMgr.DisplayLog.hour,NormalDisplay);
	DisZF16x8(4,44,ZF_maohao,NormalDisplay);
	DisBcdDigital16x8(4,52,LogMgr.DisplayLog.minute,NormalDisplay);
	DisZF16x8(4,68,ZF_maohao,NormalDisplay);
	DisBcdDigital16x8(4,76,LogMgr.DisplayLog.second,NormalDisplay);

	if ( LogMgr.DisplayLog.UserType == FACEUSER )
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(6,20,FaceUserStr,NormalDisplay);
		}
		else{
			DisEN16x8Str(6,20,FaceUserStrEn,NormalDisplay);
		}
	}
	else if ( LogMgr.DisplayLog.UserType == CARDUSER )
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(6,20,CardUserStr,NormalDisplay);
		}
		else{
			DisEN16x8Str(6,20,CardUserStrEn,NormalDisplay);
		}
	}
	else if ( LogMgr.DisplayLog.UserType ==  FPUSER )
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(6,20,FpUserStr,NormalDisplay);
		}
		else{
			DisEN16x8Str(6,20,FpUserStrEn,NormalDisplay);
		}
	}
	else if (LogMgr.DisplayLog.UserType ==	PASSCODEUSER )
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(6,20,PasscodeUserStr,NormalDisplay);
		}
		else{
			DisEN16x8Str(6,20,PasscodeUserStrEn,NormalDisplay);
		}
	}
	else if (LogMgr.DisplayLog.UserType == ONLINEPASSCODEUSER )
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(6,20,TemporaryPasswordStr,NormalDisplay);
		}
		else{
			DisEN16x8Str(6,20,TemporaryPasswordStrEn,NormalDisplay);
		}
	}
	DisEN16x8Str(6,80,":",NormalDisplay);
	
	if (( LogMgr.DisplayLog.EventType == DeleteAllFpUser )
		||( LogMgr.DisplayLog.EventType == DeleteAllStressFpUser )
		||( LogMgr.DisplayLog.EventType == DeleteAllCardUser )
		||( LogMgr.DisplayLog.EventType == DeleteAllPasscodeUser )
		)
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(6,92,LotStr,NormalDisplay);
		}
		else{
			DisEN16x8Str(6,92,LotStrEn,NormalDisplay);
		}
	}
	else
	{
		DisDigital16x8Str(6,92,LogMgr.DisplayLog.UserID,NormalDisplay);
	}
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowEventLogByDate(void)
{
	const uint8_t InputDateStr[]={HZ_shu,HZ_ru,HZ_ri,HZ_qi,HZ_end};
	const uint8_t InputDateStrEn[]={"Input Date"};
	
	if (  (CheckEventLogByDateMgr.Status == SetYear)
		||(CheckEventLogByDateMgr.Status == SetMonth)
		||(CheckEventLogByDateMgr.Status == SetDate)
	   )
	{
		#ifdef Function_TouchKeyIsTwoLine
		SET_UDandTHREEandConfirmLED_ON();
		#else
		SET_UDandLRandConfirmLED_ON();
		#endif
		
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(0,32,InputDateStr,NormalDisplay);
		}
		else{
			DisEN16x8Str(0,24,InputDateStrEn,NormalDisplay);
		}
		
		DisZF16x8(3,20,ZF_2,NormalDisplay);
		DisZF16x8(3,28,ZF_0,NormalDisplay);
		DisBcdDigital16x8(3,36,CheckEventLogByDateMgr.year,NormalDisplay);
		DisZF16x8(3,52,ZF_hengan,NormalDisplay);
		DisBcdDigital16x8(3,60,CheckEventLogByDateMgr.month,NormalDisplay);
		DisZF16x8(3,76,ZF_hengan,NormalDisplay);
		DisBcdDigital16x8(3,84,CheckEventLogByDateMgr.date,NormalDisplay);
		if (CheckEventLogByDateMgr.Status == SetYear)
		{
			DisBcdDigital16x8(3,36,CheckEventLogByDateMgr.year,InverseDisplay);
		}
		else if (CheckEventLogByDateMgr.Status == SetMonth)
		{
			DisBcdDigital16x8(3,60,CheckEventLogByDateMgr.month,InverseDisplay);
		}
		else if (CheckEventLogByDateMgr.Status == SetDate)
		{
			DisBcdDigital16x8(3,84,CheckEventLogByDateMgr.date,InverseDisplay);	
		}
	}
	else if (CheckEventLogByDateMgr.Status == SearchEventLogWithDate)
	{
		//Clear_Screen();
		SET_ALLKEYLED_OFF();
		SearchEventLogByDate();
		CheckEventLogByDateMgr.Status = ShowEventLogWithDate;
		Clear_Screen();

		if ( CheckEventLogByDateMgr.MatchedEventLogNum > 0x0000 )
		{
			LogMgr.DisplayPoint = CheckEventLogByDateMgr.StartEventLogPoint;
			LogMgr.SavedDisplayPoint = LogMgr.DisplayPoint+1;	//for reload Display Log
			CheckEventLogByDateMgr.OffsetEventLogNum = 0x0000;
		}
	}
	else if (CheckEventLogByDateMgr.Status == ShowEventLogWithDate)
	{	
		if ( CheckEventLogByDateMgr.MatchedEventLogNum == 0x0000 )
		{
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(3,40,NoEventLogStr,NormalDisplay);
			}
			else{
				DisEN16x8Str(3,40,NoEventLogStrEn,NormalDisplay);
			}
		}
		else
		{
			SET_UDandAPled_ON();

			if ( LogMgr.SavedDisplayPoint != LogMgr.DisplayPoint )
			{
				LogMgr_ReadLog(LogMgr.DisplayPoint,&LogMgr.DisplayLog.FlagHighByte);
				LogMgr.SavedDisplayPoint = LogMgr.DisplayPoint;
				Clear_Screen();
			}
			DisplayOneEventLog(CheckEventLogByDateMgr.OffsetEventLogNum+1);
			VoiceReportOneEventLog(CheckEventLogBySequenceMgr.OffsetEventLogNum+1);
		}
		
	}

}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowClearEventLog(void)
{
	uint8_t i;
	//const uint8_t TitleStr[]={HZ_shan,HZ_chufa,HZ_ji,HZ_lu,ZF_xiaoshudian,ZF_xiaoshudian,ZF_xiaoshudian,HZ_end};
	//uint8_t PercentageStr[]={ZF_baifenhao,HZ_end};


	//DisHZ16x14Str(0,24,TitleStr,NormalDisplay);

	if (LogDeleteMgr.Status == LogDeleteStart)
	{
		LogDeleteMgr.DeleteSectorNum = 0;
		LogDeleteMgr.Percentage = 0;
		LogDeleteMgr.Status = WaitforLogDeleteCofirm;
		LogDeleteMgr.Selection = NO;
		SET_UDandConfirmLED_ON();
		PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_ConfirmOrExitDelete);
	}
	else if (LogDeleteMgr.Status == WaitforLogDeleteCofirm)
	{
		if (SystemLanguage == Chinese)
		{
			DisHZ16x14Str(2,16,ConfirmDeleteStr,NormalDisplay);
			DisHZ16x14Str(4,16,AbortDeleteStr,NormalDisplay);
		}
		else
		{
			DisEN16x8Str(2,0,ConfirmDeleteStrEn,NormalDisplay);
			DisEN16x8Str(4,0,AbortDeleteStrEn,NormalDisplay);
		}
	}
	else if (LogDeleteMgr.Status == LogDeleting)
	{
		GUI_Flag_RefreshLCD = bTRUE;
		
		if ( LogDeleteMgr.Percentage < 100 )
		{
			if ( LogDeleteMgr.Percentage == 0 )
			{		
				if ( DeleteAllLog() != S_SUCCESS )
				{
					if ( DeleteAllLog() != S_SUCCESS )
					{
						DeleteAllLog();
					}
				}
			}
			if (LogDeleteMgr.Percentage>99)
			{
				DisOneDigital16x8(3,40,1,NormalDisplay);
				DisOneDigital16x8(3,48,0,NormalDisplay);
				DisOneDigital16x8(3,56,0,NormalDisplay);
			}
			else
			{
				DisOneDigital16x8(3,48,LogDeleteMgr.Percentage/10,NormalDisplay);
				DisOneDigital16x8(3,56,LogDeleteMgr.Percentage%10,NormalDisplay);
			}
			DisEN16x8Str(3,64,"%",NormalDisplay);
			
			LogDeleteMgr.Percentage++;
			GUI_RefreshSleepTime();
		}
		else
		{
			LogDeleteMgr.Status = LogDeleteSuccess;	
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_OperationSuccess);
			LogDeleteMgr.TimeCnt = Def_GuiTimeDelayCnt2s;
		}
	}
	else if (LogDeleteMgr.Status == LogDeleteSuccess)
	{
		if (SystemLanguage == Chinese){
			DisHZ16x14Str(3,36,OperationSuccessStr,NormalDisplay);
		}
		else{
			DisEN16x8Str(3,36,OperationSuccessStrEn,NormalDisplay);
		}

		if ( LogDeleteMgr.TimeCnt-- < 1 )
		{
			LogDeleteMgr.Status = LogDeleteIdle;
		}
	}
	else
	{
		DEBUG_MARK;

		LogMgr.LastPoint = 0x0000;
		LogMgr.DisplayPoint = 0x0000;
		LogMgr.NewLog.LogIDmajor = 0x00;
		
		GoIntoLogMenu_Init();
		
	}

}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowSelfTest(void)
{
	
	const uint8_t Str1[]={"LEDs,KEY,Voice"}; 
	const uint8_t Str2[]={HZ_zhi,HZ_wen,HZ_end};  
	const uint8_t Str3[]={HZ_ren,HZ_lianbu,HZ_end};  
	const uint8_t Str4[]={HZ_ren,HZ_tiyan,HZ_gan,HZ_yingda,HZ_end};  
	const uint8_t Str5[]={HZ_nei,HZ_cun,HZ_end}; 
	const uint8_t Str6[]={HZ_tongyong,HZ_xunfei,HZ_end};
	const uint8_t Str7[]={HZ_ka,HZ_pian,HZ_end};
	const uint8_t Str8[]={"Voltage"}; 
	const uint8_t Str9[]={"LOW POWER TEST"}; 
	
	const uint8_t PASSstr[]={"PASS"}; 
	const uint8_t S_FAILstr[]={"S_FAIL"}; 
	uint16_t i;
    uint8_t TestByte;
	status_t GetCardID;
	uint8_t TestCardCID[5];
	keycode_t TestKeyCode;
	
	if ( SelfTestMgr.Status == SELFTEST_START )
	{
        SelfTestMgr.Status = VoiceAndTouchAndLEDs_TEST;
		SelfTestMgr.TouchKeySelfTestStatus.KEY_ZERO_OK = bFALSE;
		SelfTestMgr.TouchKeySelfTestStatus.KEY_ONE_OK = bFALSE;
		SelfTestMgr.TouchKeySelfTestStatus.KEY_TWO_OK = bFALSE;
		SelfTestMgr.TouchKeySelfTestStatus.KEY_THREE_OK = bFALSE;
		SelfTestMgr.TouchKeySelfTestStatus.KEY_FOUR_OK = bFALSE;
		SelfTestMgr.TouchKeySelfTestStatus.KEY_FIVE_OK = bFALSE;
		SelfTestMgr.TouchKeySelfTestStatus.KEY_SIX_OK = bFALSE;
		SelfTestMgr.TouchKeySelfTestStatus.KEY_SEVEN_OK = bFALSE;
		SelfTestMgr.TouchKeySelfTestStatus.KEY_EIGHT_OK = bFALSE;
		SelfTestMgr.TouchKeySelfTestStatus.KEY_NINE_OK = bFALSE;
		SelfTestMgr.TouchKeySelfTestStatus.KEY_ASTERISK_OK = bFALSE;
		SelfTestMgr.TouchKeySelfTestStatus.KEY_POUNDSIGN_OK = bFALSE;
		
		#if defined (ProjectIs_AutoBarLock_S90Z01) || defined (ProjectIs_AutoBarLock_S90Z11)

		SelfTestMgr.TouchKeySelfTestStatus.KEY_DOORCLOSE_OK = bTRUE;
		SelfTestMgr.TouchKeySelfTestStatus.KEY_DOORBELL_OK = bFALSE;

		#else

		SelfTestMgr.TouchKeySelfTestStatus.KEY_DOORCLOSE_OK = bTRUE;
		SelfTestMgr.TouchKeySelfTestStatus.KEY_DOORBELL_OK = bTRUE;

		#endif

		
		SET_ALLKEYLED_ON();
		SET_LEDRGB_R_ON;
		SET_LEDRGB_G_ON;
		SET_LEDRGB_B_ON;
		SET_LED_RB_R_ON;
		SET_LED_RB_B_ON;
		DisEN16x8Str(2,8,Str1,NormalDisplay);
		
	}
	else if ( SelfTestMgr.Status == VoiceAndTouchAndLEDs_TEST )
	{

		if ((SelfTestMgr.TouchKeySelfTestStatus.KEY_ZERO_OK == bTRUE )
			&&( SelfTestMgr.TouchKeySelfTestStatus.KEY_ONE_OK == bTRUE )
			&&( SelfTestMgr.TouchKeySelfTestStatus.KEY_TWO_OK == bTRUE )
			&&( SelfTestMgr.TouchKeySelfTestStatus.KEY_THREE_OK == bTRUE )
			&&( SelfTestMgr.TouchKeySelfTestStatus.KEY_FOUR_OK == bTRUE )
			&&( SelfTestMgr.TouchKeySelfTestStatus.KEY_FIVE_OK == bTRUE )
			&&( SelfTestMgr.TouchKeySelfTestStatus.KEY_SIX_OK == bTRUE )
			&&( SelfTestMgr.TouchKeySelfTestStatus.KEY_SEVEN_OK == bTRUE )
			&&( SelfTestMgr.TouchKeySelfTestStatus.KEY_EIGHT_OK == bTRUE )
			&&( SelfTestMgr.TouchKeySelfTestStatus.KEY_NINE_OK == bTRUE )
			&&( SelfTestMgr.TouchKeySelfTestStatus.KEY_ASTERISK_OK == bTRUE )
			&&( SelfTestMgr.TouchKeySelfTestStatus.KEY_POUNDSIGN_OK == bTRUE )
			&&( SelfTestMgr.TouchKeySelfTestStatus.KEY_DOORCLOSE_OK == bTRUE )
			&&( SelfTestMgr.TouchKeySelfTestStatus.KEY_DOORBELL_OK == bTRUE)
			)
			{
				SET_LEDRGB_R_OFF;
				SET_LEDRGB_G_OFF;
				SET_LEDRGB_B_OFF;
				SET_LED_RB_R_OFF;
				SET_LED_RB_B_OFF;
				SelfTestMgr.Status = Display_TEST;
				SelfTestMgr.TimeCnt = Def_GuiTimeDelayCnt5s;
				PLAY_VOICE_MULTISEGMENTS(3,VoiceStr_OperationSuccess);
				Display_FullScreen();
			}
	}
	
	else if ( SelfTestMgr.Status == Display_TEST )
	{
		SelfTestMgr.TimeCnt--;
		if ( SelfTestMgr.TimeCnt > Def_GuiTimeDelayCnt2s )
		{
			Display_FullScreen();
			
		}
		else if ( SelfTestMgr.TimeCnt > 0 )
		{
			Clear_Screen();
		}
		else
		{
			PLAY_VOICE_ONESEGMENT(3,VOICE_PleasePutFinger);
			SelfTestMgr.Status = FPM_TEST;
		}
	}
	else if ( SelfTestMgr.Status == FPM_TEST )
	{
		DisHZ16x14Str(3,50,Str2,NormalDisplay);
		FPMcmd_Excute();
		if (FpmAckMgr.Status == GotACK)
		{
			if ( FpmAckMgr.ErrorCode == Error_NONE)
			{
				if ( FrmMgr.FrmFunctionConfig == FRM_Enabled )
				{
					SelfTestMgr.Status = FRM_TEST;
					SelfTestMgr.TimeCnt = 0x0000;
					PLAY_VOICE_ONESEGMENT(3,VOICE_FaceVerifyInProgress);
					GUI_Flag_RefreshLCD = bTRUE;
				}
				else
				{
					SelfTestMgr.Status = RFID_TEST;
					PLAY_VOICE_ONESEGMENT(3,VOICE_PleaseSwingCard);
					GUI_Flag_RefreshLCD = bTRUE;
				}
			}
		}
		if ( SelfTestMgr.TimeCnt > 0)
		{
			SelfTestMgr.TimeCnt--;
		}
		else {
			FPM_SendGetImageCmd();
			FpmAckMgr.Status = WaitACK;
		FpmAckMgr.TimeOutCnt = DEF_FpmAckTimeoutTime;
			SelfTestMgr.TimeCnt = Def_GuiTimeDelayCnt05s;
		}
		
	}

	else if ( SelfTestMgr.Status == FRM_TEST )
	{
		DisHZ16x14Str(3,50,Str3,NormalDisplay);
		if ( SelfTestMgr.TimeCnt > 0)
		{
			SelfTestMgr.TimeCnt--;
			if (SelfTestMgr.TimeCnt == Def_GuiTimeDelayCnt2s)
			{
				FaceRecognition_Reset();
			}
		}
		else
		{
			if ( FrmMgr.PostFlag_ResetResult == bTRUE)
			{
				PLAY_VOICE_ONESEGMENT(3,VOICE_BodyInduction);
				GUI_Flag_RefreshLCD = bTRUE;
				SelfTestMgr.Status = BODYINDUCTION_TEST;
				SelfTestMgr.TimeCnt = Def_GuiTimeDelayCnt1s;
			}
			else
			{
				FaceRecognition_HardwarePowerOff();
				Hardware_DelayMs(100);
				FaceRecognition_HardwarePowerOn();
				SelfTestMgr.TimeCnt = Def_GuiTimeDelayCnt3s;
			}
		}
	}
	else if ( SelfTestMgr.Status == BODYINDUCTION_TEST )
	{
		DisHZ16x14Str(3,32,Str4,NormalDisplay);

		if ( BodyInductionMgr.BodyInductionDeviceType == PirModule )
		{
			PIR_Task();
			if ( SelfTestMgr.TimeCnt > 0)
			{
				SelfTestMgr.TimeCnt--;
			}
			else
			{
				if ( PIRmgr.FaceDetected == bTRUE )
				{
					SelfTestMgr.Status = RFID_TEST;
					PLAY_VOICE_ONESEGMENT(3,VOICE_PleaseSwingCard);
					GUI_Flag_RefreshLCD = bTRUE;
				}
			}
		}
		else
		{
			if (Radar_SelfTest() == S_SUCCESS)
			{
				SelfTestMgr.Status = RFID_TEST;
				PLAY_VOICE_ONESEGMENT(3,VOICE_PleaseSwingCard);
				GUI_Flag_RefreshLCD = bTRUE;
			}
		}
	}	
	else if ( SelfTestMgr.Status == RFID_TEST )
	{
		DisHZ16x14Str(3,50,Str7,NormalDisplay);
		GetCardID = MFC_Auto_Reader(TestCardCID);
		if ( GetCardID == S_SUCCESS )
		{
			PLAY_VOICE_ONESEGMENT(3,VOICE_AntiSkidAlarm);
			GUI_Flag_RefreshLCD = bTRUE;
			SelfTestMgr.Status = ANTIPRYING_TEST;
			SelfTestMgr.TimeCnt = Def_GuiTimeDelayCnt10s;
		}
	}
	else if ( SelfTestMgr.Status == ANTIPRYING_TEST )
	{
		DisHZ16x14Str(3,32,AntiPryingAlarmStr,NormalDisplay);
		if ( SelfTestMgr.TimeCnt > 0 )
		{
			SelfTestMgr.TimeCnt--;
		}
		else
		{
			GUI_Flag_RefreshLCD = bTRUE;
			SelfTestMgr.Status = EEPROM_TEST;
			SelfTestMgr.TimeCnt = Def_GuiTimeDelayCnt4s;
		}
		if ( PINMACRO_PICKLOCK_STATUS == 0 )
		{
			PLAY_VOICE_ONESEGMENT(3,VOICE_Alarm);
			GUI_Flag_RefreshLCD = bTRUE;
			SelfTestMgr.Status = EEPROM_TEST;
			SelfTestMgr.TimeCnt = Def_GuiTimeDelayCnt4s;
		}	
	}
	else if ( SelfTestMgr.Status == EEPROM_TEST )
	{
		DisHZ16x14Str(2,48,Str5,NormalDisplay);
		if ( SelfTestMgr.TimeCnt == Def_GuiTimeDelayCnt2s )
		{
			PLAY_VOICE_ONESEGMENT(3,VOICE_MemoryUsage);
		}
		if ( SelfTestMgr.TimeCnt-- < 1)
		{
			EEPROM_ReadSequential(SelfTestMemoryStartAddr,&TestByte,1);
			TestByte = 0x5B;
			EEPROM_WriteSequential(SelfTestMemoryStartAddr,&TestByte,1);
			TestByte = 0x00;
			EEPROM_ReadSequential(SelfTestMemoryStartAddr,&TestByte,1);
			if (TestByte == 0x5B)
			{
				SelfTestMgr.TestResult = Passed;
				PLAY_VOICE_ONESEGMENT(3,VOICE_Beep);
				SelfTestMgr.Status = COMMUNICATION_TEST;
				SelfTestMgr.ComOkFlag = bFALSE;
				SelfTestMgr.TimeCnt = Def_GuiTimeDelayCnt3s;
			}
			else
			{
				SelfTestMgr.TestResult = Failed;
				PLAY_VOICE_MULTISEGMENTS(3,VoiceStr_OperationFail);
				DisEN16x8Str(6,48,S_FAILstr,NormalDisplay);
				while(1)
				{
					CLRWDT();
				}
			}
		}
	}
	else if ( SelfTestMgr.Status == COMMUNICATION_TEST )
	{
		GUI_Flag_RefreshLCD = bTRUE;
		DisHZ16x14Str(3,40,Str6,NormalDisplay);

		if ( SelfTestMgr.TimeCnt-- < 1)
		{
			if ( SelfTestMgr.ComOkFlag == bTRUE )
			{
				//PLAY_VOICE_MULTISEGMENTS(3,VoiceStr_OperationSuccess);
				PLAY_VOICE_ONESEGMENT(3,VOICE_Beep);
				SelfTestMgr.Status = VOLTAGE_TEST;
			}
			else
			{
				SelfTestMgr.TestResult = Failed;
				PLAY_VOICE_MULTISEGMENTS(3,VoiceStr_OperationFail);
				DisEN16x8Str(6,48,S_FAILstr,NormalDisplay);
				while(1)
				{
					CLRWDT();
				}
			}
		}
	}
	else if ( SelfTestMgr.Status == VOLTAGE_TEST )
	{
		GUI_Flag_RefreshLCD = bTRUE;
		SET_KEYLED_ON(KEY_POUNDSIGN);
		DisEN16x8Str(3,40,Str8,NormalDisplay);
		DisOneDigital16x8(6,56,BatteryMgr.BatteryVoltage/10,NormalDisplay);
		DisOneDigital16x8(6,64,BatteryMgr.BatteryVoltage%10,NormalDisplay);
	}
	else if ( SelfTestMgr.Status == LOWPOWER_TEST )
	{
		GUI_Flag_RefreshLCD = bTRUE;
		
		SystemConfigReset();
		
		DisEN16x8Str(2,24,Str9,NormalDisplay);
		Hardware_DelayMs(300);
		CLRWDT();
		Hardware_DelayMs(300);
		System_PowerDown();
		
		System_Awake();

		GUI_RefreshSleepTime();
		
		SystemPowerMgr.AwakeTimerCnt=0x0000;
	}	
}

/*******************************************************/
/*******************************************************/
void ShowAgingTest(void)
{
	const uint8_t AgingTestStr[]={HZ_lao,HZ_hua,HZ_mo,HZ_si,HZ_end}; 
	const uint8_t AgingTestStrEn[]={"AGING MODE"}; 
	const uint8_t CyclesStr[]={"Cycles:"}; 
	const uint8_t VoltageStr[]={"Voltage:  . V"};

	if ( SystemLanguage == Chinese )
	{
		DisHZ16x14Str(0,36,AgingTestStr,NormalDisplay);
	}
	else
	{
		DisEN16x8Str(0,16,AgingTestStrEn,NormalDisplay);
	}


	if (ComportMgr.DoorStatus == Standby )
	{
		if ( AgingTestMgr.MotorRunTimes <0x0007A120 )		//500,000 times
		{
			if	( AgingTestMgr.TestFlag == bFALSE )
			{
				AgingTestMgr.MotorRunTimes++;
				AgingTestMgr.TestFlag = bTRUE;
				PLAY_VOICE_DOORBELL();
				ComPort_SetPost_OpenDoor(AGINGTESTUSER,0x00);
				ComPort_SetPost_RemoteUnlcok();
				Enable_KEYLED_WATERLIGHT(); 	
			}
		}
	}
	else
	{
		AgingTestMgr.TestFlag = bFALSE;
	}

	FpUserIdentify();
	if (AgingTestMgr.TimeCnt < (12*64))
	{
		AgingTestMgr.TimeCnt++;
		if ( AgingTestMgr.TimeCnt < (4*64) )
		{
			GUI_SetFPM_LED(DEF_FpmLedMode_Breath,DEF_FpmLedColor_Red,DEF_FpmLedColor_Red,1); 
		}
		else if ( AgingTestMgr.TimeCnt < (8*64) )
		{
			GUI_SetFPM_LED(DEF_FpmLedMode_Breath,DEF_FpmLedColor_Green,DEF_FpmLedColor_Green,1); 
		}
		else
		{
			GUI_SetFPM_LED(DEF_FpmLedMode_Breath,DEF_FpmLedColor_Blue,DEF_FpmLedColor_Blue,1); 
		}	
	}
	else
	{
		AgingTestMgr.TimeCnt = 0x0000;
	}
	


	DisEN16x8Str(3,0,CyclesStr,NormalDisplay);
	DisOneDigital16x8(3,64,AgingTestMgr.MotorRunTimes%1000000/100000,NormalDisplay);
	DisOneDigital16x8(3,72,AgingTestMgr.MotorRunTimes%100000/10000,NormalDisplay);
	DisOneDigital16x8(3,80,AgingTestMgr.MotorRunTimes%10000/1000,NormalDisplay);
	DisOneDigital16x8(3,88,AgingTestMgr.MotorRunTimes%1000/100,NormalDisplay);
	DisOneDigital16x8(3,96,AgingTestMgr.MotorRunTimes%100/10,NormalDisplay);
	DisOneDigital16x8(3,104,AgingTestMgr.MotorRunTimes%10,NormalDisplay);

	DisEN16x8Str(5,0,VoltageStr,NormalDisplay);	
	DisOneDigital16x8(5,64,BatteryMgr.BatteryVoltage%1000/100,NormalDisplay);
	DisOneDigital16x8(5,72,BatteryMgr.BatteryVoltage%100/10,NormalDisplay);
	DisOneDigital16x8(5,88,BatteryMgr.BatteryVoltage%10,NormalDisplay);


}

/*******************************************************/
/*******************************************************/
void GUI_CreatAndSaveLog(EventLogType_t LogType)
{
	if ( LogType == OpenDoor )
	{
		if ( UserIdentifyResultMgr.IdentifyType == CARD ){
			LogMgr.NewLog.UserType = CARDUSER;
			LogMgr.NewLog.UserID = UserIdentifyResultMgr.CardUserID;
			LogMgr.NewLog.EventType = OpenDoor;
			LogMgr.NewLog.EventParameter = 0x00;
		}
		else if ( UserIdentifyResultMgr.IdentifyType == FINGERPRINT ){
			LogMgr.NewLog.UserType = FPUSER;
			LogMgr.NewLog.UserID = UserIdentifyResultMgr.FPUserID;
			LogMgr.NewLog.EventType = OpenDoor;
			LogMgr.NewLog.EventParameter = 0x00;
		}
		else if (UserIdentifyResultMgr.IdentifyType == ONLINEPASSCODE ){
			LogMgr.NewLog.UserType = ONLINEPASSCODEUSER;
			LogMgr.NewLog.UserID = UserIdentifyResultMgr.PasscodeUserID;
			LogMgr.NewLog.EventType = OpenDoor;
			LogMgr.NewLog.EventParameter = 0x00;
		}
		else if (UserIdentifyResultMgr.IdentifyType == PASSCODE ){
			LogMgr.NewLog.UserType = PASSCODEUSER;
			LogMgr.NewLog.UserID = UserIdentifyResultMgr.PasscodeUserID;
			LogMgr.NewLog.EventType = OpenDoor;
			LogMgr.NewLog.EventParameter = 0x00;
		}
		else if ( UserIdentifyResultMgr.IdentifyType == FACE ){
			LogMgr.NewLog.UserType = FACEUSER;
			LogMgr.NewLog.UserID = UserIdentifyResultMgr.FaceUserID;
			LogMgr.NewLog.EventType = OpenDoor;
			LogMgr.NewLog.EventParameter = 0x00;
		}
		else if (UserIdentifyResultMgr.IdentifyType == FINGERPRINTandCARD ){
			LogMgr.NewLog.UserType = FPUSER;
			LogMgr.NewLog.UserID = UserIdentifyResultMgr.FPUserID;
			LogMgr.NewLog.EventType = OpenDoor;
			LogMgr.NewLog.EventParameter = 0x00;	
		}
		else if (UserIdentifyResultMgr.IdentifyType == FINGERPRINTandPASSCODE ){
			LogMgr.NewLog.UserType = FPUSER;
			LogMgr.NewLog.UserID = UserIdentifyResultMgr.FPUserID;
			LogMgr.NewLog.EventType = OpenDoor;
			LogMgr.NewLog.EventParameter = 0x00;

		}
		else if (UserIdentifyResultMgr.IdentifyType == CARDandPASSCODE ){
			LogMgr.NewLog.UserType = PASSCODEUSER;
			LogMgr.NewLog.UserID = UserIdentifyResultMgr.PasscodeUserID;
			LogMgr.NewLog.EventType = OpenDoor;
			LogMgr.NewLog.EventParameter = 0x00;	
		}
		else if (UserIdentifyResultMgr.IdentifyType == RemoteUnlock ){
			LogMgr.NewLog.UserType = REMOTEGUEST;
			LogMgr.NewLog.UserID = 0x00;//WifiMgr.RemoteUnlockUserID/256;
			LogMgr.NewLog.EventType = OpenDoor;
			LogMgr.NewLog.EventParameter = 0x00;	
		}
		else if (UserIdentifyResultMgr.IdentifyType == INITIALSTATUS )
		{
			return; 	//do nothing
		}
		else
		{
			return; 	//do nothing
		}
	}
	
	else if ( LogType == AddFaceUser )
	{
		LogMgr.NewLog.UserType = FACEUSER;
		LogMgr.NewLog.UserID = FaceUserRegisterMgr.UserID;
		LogMgr.NewLog.EventType = AddFaceUser;
		LogMgr.NewLog.EventParameter = 0x00;	
	}
	else if ( LogType == DeleteFaceUser )
	{
		LogMgr.NewLog.UserType = FACEUSER;
		LogMgr.NewLog.UserID = FaceUserDeleteMgr.UserID;
		LogMgr.NewLog.EventType = DeleteFaceUser;
		LogMgr.NewLog.EventParameter = 0x00;	
	}
	else if ( LogType == AddFpUser )
	{
		LogMgr.NewLog.UserType = FPUSER;
		LogMgr.NewLog.UserID = FpUserRegisterMgr.UserID;
		LogMgr.NewLog.EventType = AddFpUser;
		LogMgr.NewLog.EventParameter = 0x00;	
	}
	else if ( LogType == DeleteFpUser )
	{
		LogMgr.NewLog.UserType = FPUSER;
		LogMgr.NewLog.UserID = FpUserDeleteMgr.UserID;
		LogMgr.NewLog.EventType = DeleteFpUser;
		LogMgr.NewLog.EventParameter = 0x00;	
	}
	else if ( LogType == DeleteAllFpUser )
	{
		LogMgr.NewLog.UserType = FPUSER;
		LogMgr.NewLog.UserID = 0x00;
		LogMgr.NewLog.EventType = DeleteAllFpUser;
		LogMgr.NewLog.EventParameter = 0x00;	
	}
	else if ( LogType == DeleteAllStressFpUser )
	{
		LogMgr.NewLog.UserType = FPUSER;
		LogMgr.NewLog.UserID = 0x00;
		LogMgr.NewLog.EventType = DeleteAllStressFpUser;
		LogMgr.NewLog.EventParameter = 0x00;	
	}
	else if ( LogType == AddPasscodeUser )
	{
		LogMgr.NewLog.UserType = PASSCODEUSER;
		LogMgr.NewLog.UserID = PasscodeUserRegisterMgr.UserID;
		LogMgr.NewLog.EventType = AddPasscodeUser;
		LogMgr.NewLog.EventParameter = 0x00;	
	}
	else if ( LogType == DeletePasscodeUser )
	{
		LogMgr.NewLog.UserType = PASSCODEUSER;
		LogMgr.NewLog.UserID = PasscodeUserDeleteMgr.UserID;
		LogMgr.NewLog.EventType = DeletePasscodeUser;
		LogMgr.NewLog.EventParameter = 0x00;	
	}
	else if ( LogType == DeleteAllPasscodeUser )
	{
		LogMgr.NewLog.UserType = PASSCODEUSER;
		LogMgr.NewLog.UserID = 0x00;
		LogMgr.NewLog.EventType = DeleteAllPasscodeUser;
		LogMgr.NewLog.EventParameter = 0x00;	
	}
	else if ( LogType == AddCardUser )
	{
		LogMgr.NewLog.UserType = CARDUSER;
		LogMgr.NewLog.UserID = CardUserRegisterMgr.UserID;
		LogMgr.NewLog.EventType = AddCardUser;
		LogMgr.NewLog.EventParameter = 0x00;	
	}
	else if ( LogType == DeleteCardUser )
	{
		LogMgr.NewLog.UserType = CARDUSER;
		LogMgr.NewLog.UserID = CardUserDeleteMgr.UserID;
		LogMgr.NewLog.EventType = DeleteCardUser;
		LogMgr.NewLog.EventParameter = 0x00;	
	}
	else if ( LogType == DeleteAllCardUser )
	{
		LogMgr.NewLog.UserType = CARDUSER;
		LogMgr.NewLog.UserID = 0x00;
		LogMgr.NewLog.EventType = DeleteAllCardUser;
		LogMgr.NewLog.EventParameter = 0x00;	
	}
	else
	{
		return; 	
		//do nothing
	}
	

	LogMgr.NewLog.year = SystemTime.year;
	LogMgr.NewLog.month = SystemTime.month;
	LogMgr.NewLog.date = SystemTime.date;
	LogMgr.NewLog.hour = SystemTime.hour;
	LogMgr.NewLog.minute = SystemTime.minute;
	LogMgr.NewLog.second = SystemTime.second;

	LogMgr.NewLog.FlagHighByte = DEF_LogFlagHighByte;
	LogMgr.NewLog.FlagLowByte = DEF_LogFlagLowByte;
	//LogMgr.NewLog.LogIDmajor = 0x00;
	LogMgr_WriteLog(LogMgr.LastPoint);
		
	if ( LogMgr.LastPoint < (DEF_MaxRecordedLogID-1) )		//锟?1锟?7锟?1锟?7锟?1锟?7log锟?1锟?7锟?0锟?91280锟?1锟?7锟?1锟?7锟?1锟?7锟?1锟?7锟?1锟?7锟?0锟?5锟?1锟?7log
	{
		LogMgr.LastPoint++;
		//LogMgr.DisplayPoint = LogMgr.LastPoint-1;
	}
	else
	{
		LogMgr.NewLog.LogIDmajor++; 	//LogIDmajor+1锟?1锟?7锟?1锟?7锟?1锟?7锟?1锟?7锟?1锟?7锟?1锟?7锟?1锟?7锟?1锟?7锟?1锟?7锟?1锟?7锟?0锟?2锟?1锟?7log
		LogMgr.LastPoint=0x0000;
		//LogMgr.DisplayPoint = 999;
	}


	
}


void VoiceReportYearAndInputMonth(uint8_t value)
{
	uint16_t VoiceStr[7];
	VoiceStr[0] = TranslateNumberToVoice(value/16);
	VoiceStr[1] = TranslateNumberToVoice(value%16);
	VoiceStr[2] = VOICE_Year;
	VoiceStr[3] = VOICE_PleaseEnter;
	VoiceStr[4] = VOICE_Month;
	VoiceStr[5] = DEF_VoiceSegmentEndFlag;
	PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr);
}
void VoiceReportMonthAndInputDate(uint8_t value)
{
	uint16_t VoiceStr[7];
	VoiceStr[0] = TranslateNumberToVoice(value/16);
	VoiceStr[1] = TranslateNumberToVoice(value%16);
	VoiceStr[2] = VOICE_Month;
	VoiceStr[3] = VOICE_PleaseEnter;
	VoiceStr[4] = VOICE_Date;
	VoiceStr[5] = DEF_VoiceSegmentEndFlag;
	PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr);
}
void VoiceReportInputMonthErrorAndRetry(void)
{
	uint16_t VoiceStr[4];
	VoiceStr[0] = VOICE_InputError;
	VoiceStr[1] = VOICE_PleaseEnter;
	VoiceStr[2] = VOICE_Month;
	VoiceStr[3] = DEF_VoiceSegmentEndFlag;
	PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr);
}
void VoiceReportDateAndInputHour(uint8_t value)
{
	uint16_t VoiceStr[7];
	VoiceStr[0] = TranslateNumberToVoice(value/16);
	VoiceStr[1] = TranslateNumberToVoice(value%16);
	VoiceStr[2] = VOICE_Date;
	VoiceStr[3] = VOICE_PleaseEnter;
	VoiceStr[4] = VOICE_Hour;
	VoiceStr[5] = DEF_VoiceSegmentEndFlag;
	PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr);
}
void VoiceReportInputDateErrorAndRetry(void)
{
	uint16_t VoiceStr[4];
	VoiceStr[0] = VOICE_InputError;
	VoiceStr[1] = VOICE_PleaseEnter;
	VoiceStr[2] = VOICE_Date;
	VoiceStr[3] = DEF_VoiceSegmentEndFlag;
	PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr);
}
void VoiceReportHourAndInputMinute(uint8_t value)
{
	uint16_t VoiceStr[7];
	VoiceStr[0] = TranslateNumberToVoice(value/16);
	VoiceStr[1] = TranslateNumberToVoice(value%16);
	VoiceStr[2] = VOICE_Hour;
	VoiceStr[3] = VOICE_PleaseEnter;
	VoiceStr[4] = VOICE_Minute;
	VoiceStr[5] = DEF_VoiceSegmentEndFlag;
	PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr);
}
void VoiceReportInputHourErrorAndRetry(void)
{
	uint16_t VoiceStr[4];
	VoiceStr[0] = VOICE_InputError;
	VoiceStr[1] = VOICE_PleaseEnter;
	VoiceStr[2] = VOICE_Hour;
	VoiceStr[3] = DEF_VoiceSegmentEndFlag;
	PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr);
}
void VoiceReportMinuteAndInputSecond(uint8_t value)
{
	uint16_t VoiceStr[7];
	VoiceStr[0] = TranslateNumberToVoice(value/16);
	VoiceStr[1] = TranslateNumberToVoice(value%16);
	VoiceStr[2] = VOICE_Minute;
	VoiceStr[3] = VOICE_PleaseEnter;
	VoiceStr[4] = VOICE_Second;
	VoiceStr[5] = DEF_VoiceSegmentEndFlag;
	PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr);
}
void VoiceReportSecond(uint8_t value)
{
	uint16_t VoiceStr[8];
	VoiceStr[0] = TranslateNumberToVoice(value/16);
	VoiceStr[1] = TranslateNumberToVoice(value%16);
	VoiceStr[2] = VOICE_Second;
	VoiceStr[3] = VOICE_PressPoundKey;
	VoiceStr[4] = VOICE_Confirm;
	VoiceStr[5] = VOICE_PressAsteriskKey;
	VoiceStr[6] = VOICE_ReturnPreviousMenu;
	VoiceStr[7] = DEF_VoiceSegmentEndFlag;
	PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr);
}


/*******************************************************/
/*******************************************************/
void GUI_Button_Monitor(void)
{
	DEBUG_MARK;
	uint8_t i;


	#ifdef Function_TouchUsed_CSK14S
		#if defined (ProjectIs_AutoBarLock_S90Z01) || defined (ProjectIs_AutoBarLock_S90Z11)
			//|| defined (ProjectIs_AutoBarLock_S90Z20)
		//do nothing
		#else
		if (CardIdentifyMgr.DelayTimeAfterReadCard < 8 )
		{
			return;
		}
		#endif
	#endif
		

	gui_keycode = Key_Scan();

	DEBUG_MARK;

	if ( gui_keycode != KEY_NONE )
	{
		CardIdentifyMgr.CardDetectIntervalTimeCnt = Def_GuiTimeDelayCnt1s;
	
		if ( TouchPowerMgr.Status != HighSensitivity )
		{
			//SET_TOUCH_AWAKE_SENSITIVITY();
		}
		GUI_RefreshSleepTime();
		
		FaceIdentifyMgr.ReportFaceStatusTimeCnt = Def_GuiTimeDelayCnt2s;		//report face status delay time

		if ( AwakeDisplayMgr.DisplayType == DisplayHide )
		{
			PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_POWERON);
			AwakeDisplayMgr.DisplayType = DisplayCaseIgnore;
			SET_OLEDPOWER_ON;
			#ifdef Function_DisplayUsed_OLED128X64
			OLED_Init();
			#elif defined Function_DisplayUsed_HFG12864
			HFG12864_Init();
			#endif
			
		}
	}
	
	switch ( CurrentScreen)
	{
		case SCREEN_Main:
			
			if ( ComportMgr.RestoreFactoryDefaultTrig == bTRUE )
			{
				if ( AwakeDisplayMgr.DisplayType == DisplayHide )
				{
					AwakeDisplayMgr.DisplayType = DisplayCaseIgnore;
					SET_OLEDPOWER_ON;
					#ifdef Function_DisplayUsed_OLED128X64
					OLED_Init();
					#elif defined Function_DisplayUsed_HFG12864
					HFG12864_Init();
					#endif	
				}
				CurrentScreen = SCREEN_RestoreFactoryDefault;
				RestoreFactoryDefaultMgr.Status = StartRestoreFactoryDefault;
				PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_Beep);
				GUI_RefreshSleepTime();
			}
			
			if ( ((gui_keycode == KEY_DOORCLOSE )||(AwakeSystemKeyMgr.IsDoorColseKeyAwake == bTRUE))
					&&( AutoMotorMgr.AutoEject == 0x00 )		//自弹锁体关闭
				)
			{
					AwakeSystemKeyMgr.IsDoorColseKeyAwake = bFALSE;
					DEF_ButtonPress_Voice;
					CurrentScreen = SCREEN_IdentifySuccess;
					DisplayDoorStatusMgr.Status = DoorCloseInit;
					UserIdentifyResultMgr.IdentifyType = OutsideCloseDoor;
			}
			else if  ( PasscodeUserIdentifyMgr.Status == PasscodeIdentifyIdle )
			{
				if ( gui_keycode < 10 )		//from KEY0~KEY9
				{
					if ( PasscodeUserIdentifyMgr.Status == PasscodeIdentifyIdle )
					{	
						PasscodeInputMgr.Point = 0x01;
						PasscodeInputMgr.PasscodeLen = 16;
						PasscodeInputMgr.Status = PasscodeInputStart;
						PasscodeInputMgr.InputBuff[0] = gui_keycode;
						for (i=1;i<PasscodeInputMgr.PasscodeLen;i++)
						{
							PasscodeInputMgr.InputBuff[i]=0xff;
						}
						
						PasscodeUserIdentifyMgr.Status = PasscodeIdentifyStart;
						DEF_ButtonPress_Voice;
					}
				 }
				else if ( gui_keycode == KEY_ASTERISK )
				{
					if ( g_ASTERISK_PressedOnMainScreen == bFALSE ){
						DEF_ButtonPress_Voice;
						g_ASTERISK_PressedOnMainScreen = bTRUE;
					}
					else 
					{
						DEF_ButtonPress_Voice;
						SystemPowerMgr.SleepDelayTimerCnt = 0x0000;
						SystemPowerMgr.SleepSource = UserForced;
					}
				}
				else if ( gui_keycode == KEY_POUNDSIGN )
				{	
					#if    defined (ProjectIs_AutoBarLock_S49Z06) || defined (ProjectIs_AutoBarLock_S58Z02) \
						|| defined (ProjectIs_AutoBarLock_S67Z01) || defined (ProjectIs_AutoBarLock_S31Z01)\
						|| defined (ProjectIs_AutoBarLock_S67Z02) || defined (ProjectIs_AutoBarLock_S20Z04)\
						|| defined (ProjectIs_AutoBarLock_S11Z05) || defined ( ProjectIs_AutoBarLock_S58Z07)\
						|| defined (ProjectIs_AutoBarLock_S68Z04) || defined (ProjectIs_AutoBarLock_S68Z10)
						
					PLAY_VOICE_DOORBELL();	//DOORBELL
					CurrentScreen = SCREEN_RemoteUnlockRequest;
					WifiMgr.RemoteUnlockMgr.Status = RemoteUnlockRequestInit;
					
					#elif defined (ProjectIs_AutoBarLock_S90Z01) || defined (ProjectIs_AutoBarLock_S90Z11)\
						|| defined (ProjectIs_AutoBarLock_S90Z20)
					if ( g_ASTERISK_PressedOnMainScreen == bTRUE )
					{	
						 DEF_ButtonPress_Voice;
						 CurrentScreen = SCREEN_ManagerIdentify;
						 ManagerIdentifyMgr.Status = StartManagerIdentify;
						 g_ASTERISK_PressedOnMainScreen = bFALSE;
					}
					else
					{
						if  ( AutoMotorMgr.AutoEject == 0x00 )		//自弹锁体关闭
						{
							DEF_ButtonPress_Voice;
							CurrentScreen = SCREEN_IdentifySuccess;
							DisplayDoorStatusMgr.Status = DoorCloseInit;
							UserIdentifyResultMgr.IdentifyType = OutsideCloseDoor;
						}
					}
					
					#else
					if ( g_ASTERISK_PressedOnMainScreen == bTRUE )
					{	
						 DEF_ButtonPress_Voice;
						 CurrentScreen = SCREEN_ManagerIdentify;
						 ManagerIdentifyMgr.Status = StartManagerIdentify;
						 g_ASTERISK_PressedOnMainScreen = bFALSE;
					}
					else
					{
						#ifndef Function_IndependentDoorBellKey
						PLAY_VOICE_DOORBELL();	//DOORBELL			
						CurrentScreen = SCREEN_RemoteUnlockRequest;
						WifiMgr.RemoteUnlockMgr.Status = RemoteUnlockRequestInit;
						#endif
					}		
					#endif
				}
			}
			else if ( gui_keycode != KEY_NONE )		//from KEY0~KEY9
			{
				g_ASTERISK_PressedOnMainScreen = bFALSE;
				if ( PasscodeUserIdentifyMgr.Status == PasscodeIdentifyPasscodeInput )
				{
					DEF_ButtonPress_Voice;
					PasscodeUserIdentifyMgr.TimeCnt = 240;	//
					GUI_PasscodeInputButtonMonitor(gui_keycode);
				}
			}
			
			if ( gui_keycode == KEY_DOORBELL )  
			{
				PLAY_VOICE_DOORBELL();	//DOORBELL
				CurrentScreen = SCREEN_RemoteUnlockRequest;
				WifiMgr.RemoteUnlockMgr.Status = RemoteUnlockRequestInit;
			}
			
			break;
			
		case SCREEN_MainMenu:

			if ( gui_keycode == KEY_ONE )
			{
				DEF_ButtonPress_Voice;
				GoIntoUserManagementMenu_Init();
			}
			else if (gui_keycode == KEY_TWO)
			{
				DEF_ButtonPress_Voice;
				GoIntoSystemConfigMenu_Init();	
			}
			else if (gui_keycode == KEY_THREE)
			{
				DEF_ButtonPress_Voice;
				GoIntoInfoInquiryMenu_Init();
			}
			else if (gui_keycode == KEY_FOUR)
			{
				DEF_ButtonPress_Voice;
				CurrentScreen = SCREEN_NetWorkLink;
				WifiMgr.Link.Status = LinkStart;
			}

			else if ( gui_keycode == KEY_ASTERISK )
			{
				//DEF_ButtonPress_Voice;
				PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_ExitMenu);	
				GoIntoMainScreen_WithIdentifyInit();
			}
			else if ( gui_keycode == KEY_POUNDSIGN )
			{
				DEF_ButtonPress_Voice;
				GoIntoMainMenu_Init();
			}
			
			break;

		case SCREEN_UserManagementMenu:
			
			if ( gui_keycode == KEY_ONE )
				{
					DEF_ButtonPress_Voice;
					AddManagerMenu_Init();
					//GoIntoFaceMenu_Init();
				}
				else if ( gui_keycode == KEY_TWO )
				{
					DEF_ButtonPress_Voice;
					AddUserMenu_Init();
					//GoIntoFpMenu_Init();
				}
				else if ( gui_keycode == KEY_THREE)
				{
					DEF_ButtonPress_Voice;
					DeleteUserMenu_Init();
					//GoIntoPasscodeMenu_Init();
				}
				else if ( gui_keycode == KEY_ASTERISK )
				{
					DEF_ButtonPress_Voice;
					GoIntoMainMenu_Init();
				}
				else if ( gui_keycode == KEY_POUNDSIGN )
				{
					DEF_ButtonPress_Voice;
					GoIntoUserManagementMenu_Init();
				}
			
		
//			if ( FrmMgr.FrmFunctionConfig == FRM_Enabled )
//			{
//				if ( gui_keycode == KEY_ONE )
//				{
//					DEF_ButtonPress_Voice;
//					GoIntoFaceMenu_Init();
//				}
//				else if ( gui_keycode == KEY_TWO )
//				{
//					DEF_ButtonPress_Voice;
//					//GoIntoFpMenu_Init();
//				}
//				else if ( gui_keycode == KEY_THREE)
//				{
//					DEF_ButtonPress_Voice;
//					//GoIntoPasscodeMenu_Init();
//				}
//				else if (gui_keycode == KEY_FOUR)
//				{
//					DEF_ButtonPress_Voice;
//					 //GoIntoCardMenu_Init();
//				}
//				else if ( gui_keycode == KEY_ASTERISK )
//				{
//					DEF_ButtonPress_Voice;
//					GoIntoMainMenu_Init();
//				}
//				else if ( gui_keycode == KEY_POUNDSIGN )
//				{
//					DEF_ButtonPress_Voice;
//					GoIntoUserManagementMenu_Init();
//				}
//			}
//			else
//			{
//				if ( gui_keycode == KEY_ONE )
//				{
//					DEF_ButtonPress_Voice;
//					GoIntoFpMenu_Init();
//				}
//				else if ( gui_keycode == KEY_TWO)
//				{
//					DEF_ButtonPress_Voice;
//					GoIntoPasscodeMenu_Init();
//				}
//				else if (gui_keycode == KEY_THREE)
//				{
//					DEF_ButtonPress_Voice;
//					GoIntoCardMenu_Init();
//				}
//				else if ( gui_keycode == KEY_ASTERISK )
//				{
//					DEF_ButtonPress_Voice;
//					GoIntoMainMenu_Init();
//				}
//				else if ( gui_keycode == KEY_POUNDSIGN )
//				{
//					DEF_ButtonPress_Voice;
//					GoIntoUserManagementMenu_Init();
//				}
//			}
			break;
			
		case SCREEN_FaceMenu:
			
			if ( gui_keycode == KEY_ONE )
			{	
				//DEF_ButtonPress_Voice;
				FaceUserRegisterMgr.Status = StartFaceUserRegister;
				FaceUserRegisterMgr.FaceUserType = FaceMaster;
				CurrentScreen = SCREEN_RegisterMasterFace;
			}
			else if (gui_keycode == KEY_TWO)
			{	
				//DEF_ButtonPress_Voice;			
				FaceUserRegisterMgr.Status = StartFaceUserRegister;
				FaceUserRegisterMgr.FaceUserType = FaceUser;
				CurrentScreen = SCREEN_RegisterUserFace;
			}
			else if (gui_keycode == KEY_THREE)
			{	
				//DEF_ButtonPress_Voice;
				FaceUserDeleteMgr.Status = StartFaceUserDelete;
				CurrentScreen = SCREEN_DeleteUserFace;
			}			
			else if ( gui_keycode == KEY_ASTERISK )
			{
				DEF_ButtonPress_Voice;
				GoIntoUserManagementMenu_Init();
			}
			else if ( gui_keycode == KEY_POUNDSIGN )
			{
				DEF_ButtonPress_Voice;
				GoIntoFaceMenu_Init();
			}
			break;	
			
		case SCREEN_ManagerMenu:
			
			if ( gui_keycode == KEY_ONE )
			{	
				//DEF_ButtonPress_Voice;
//				FaceUserRegisterMgr.Status = StartFaceUserRegister;
//				FaceUserRegisterMgr.FaceUserType = FaceMaster;
//				CurrentScreen = SCREEN_RegisterMasterFace;
				GoInToSCREEN_RegisterManagerFPCInit();
			}
			else if (gui_keycode == KEY_TWO)
			{	
				//DEF_ButtonPress_Voice;			
//				FaceUserRegisterMgr.Status = StartFaceUserRegister;
//				FaceUserRegisterMgr.FaceUserType = FaceUser;
//				CurrentScreen = SCREEN_RegisterUserFace;
				GoInToSCREEN_RegisterManagerFaceInit();
			}			
			else if ( gui_keycode == KEY_ASTERISK )
			{
				DEF_ButtonPress_Voice;
				GoIntoUserManagementMenu_Init();
			}
			else if ( gui_keycode == KEY_POUNDSIGN )
			{
				DEF_ButtonPress_Voice;
				AddManagerMenu_Init();
			}
			break;
			
		case SCREEN_UserMenu:
			if ( gui_keycode == KEY_ONE )
			{	
				//DEF_ButtonPress_Voice;
//				FaceUserRegisterMgr.Status = StartFaceUserRegister;
//				FaceUserRegisterMgr.FaceUserType = FaceMaster;
//				CurrentScreen = SCREEN_RegisterMasterFace;
				GoInToSCREEN_RegisterUserFPCInit();
			}
			else if (gui_keycode == KEY_TWO)
			{	
				//DEF_ButtonPress_Voice;			
//				FaceUserRegisterMgr.Status = StartFaceUserRegister;
//				FaceUserRegisterMgr.FaceUserType = FaceUser;
//				CurrentScreen = SCREEN_RegisterUserFace;
				GoInToSCREEN_RegisterUserFace();
			}			
			else if ( gui_keycode == KEY_ASTERISK )
			{
				DEF_ButtonPress_Voice;
				GoIntoUserManagementMenu_Init();
			}
			else if ( gui_keycode == KEY_POUNDSIGN )
			{
				DEF_ButtonPress_Voice;
				AddUserMenu_Init();
			}
			break;
		
		case SCREEN_DeleteUserMenu:
			if ( gui_keycode == KEY_ONE )
			{	
				//DEF_ButtonPress_Voice;
//				FaceUserRegisterMgr.Status = StartFaceUserRegister;
//				FaceUserRegisterMgr.FaceUserType = FaceMaster;
//				CurrentScreen = SCREEN_RegisterMasterFace;
				GoInToSCREEN_DeleteUser();
			}
			else if (gui_keycode == KEY_TWO)
			{	
				//DEF_ButtonPress_Voice;			
//				FaceUserRegisterMgr.Status = StartFaceUserRegister;
//				FaceUserRegisterMgr.FaceUserType = FaceUser;
//				CurrentScreen = SCREEN_RegisterUserFace;
				GoInToSCREEN_AllDeleteUser();
			}			
			else if ( gui_keycode == KEY_ASTERISK )
			{
				DEF_ButtonPress_Voice;
				GoIntoUserManagementMenu_Init();
			}
			else if ( gui_keycode == KEY_POUNDSIGN )
			{
				DEF_ButtonPress_Voice;
				DeleteUserMenu_Init();
			}
			break;
			
		case SCREEN_FpMenu:
			
			if ( gui_keycode == KEY_ONE )
			{	
				//DEF_ButtonPress_Voice;
				FpUserRegisterMgr.Status = StartFpUserRegister;
				FpUserRegisterMgr.FpUserType = FpMaster;
				CurrentScreen = SCREEN_RegisterMasterFp;
			}
			else if (gui_keycode == KEY_TWO)
			{	
				//DEF_ButtonPress_Voice;			
				FpUserRegisterMgr.Status = StartFpUserRegister;
				FpUserRegisterMgr.FpUserType = FpUser;
				CurrentScreen = SCREEN_RegisterUserFp;
			}
			else if (gui_keycode == KEY_THREE)
			{	
				//DEF_ButtonPress_Voice;
				FpUserRegisterMgr.Status = StartFpUserRegister;
				FpUserRegisterMgr.FpUserType = StressFpUser;
				CurrentScreen = SCREEN_RegisterStressUserFp;
			}	
			else if (gui_keycode == KEY_FOUR)
			{	
				//DEF_ButtonPress_Voice;
				GoIntoFpDeleteMenu_Init();
			}		
			else if ( gui_keycode == KEY_ASTERISK )
			{
				DEF_ButtonPress_Voice;
				GoIntoUserManagementMenu_Init();
			}
			else if ( gui_keycode == KEY_POUNDSIGN )
			{
				DEF_ButtonPress_Voice;
				GoIntoFpMenu_Init();
			}
			break;
			
		case SCREEN_FpDeleteMenu:
			
			if (gui_keycode == KEY_ONE)
			{	
				//DEF_ButtonPress_Voice;
				FpUserDeleteMgr.Status = StartFpUserDelete;
				FpUserDeleteMgr.FpUserType = FpUser;
				CurrentScreen = SCREEN_DeleteUserFp;
			}	
			else if (gui_keycode == KEY_TWO)
			{	
				//DEF_ButtonPress_Voice;
				AllUserFpDeleteMgr.Status = StartAllUserFpDelete;
				AllUserFpDeleteMgr.FpUserType = FpUser;
				CurrentScreen = SCREEN_DeleteAllUserFp;
			}
			else if (gui_keycode == KEY_THREE)
			{	
				//DEF_ButtonPress_Voice;
				AllUserFpDeleteMgr.Status = StartAllUserFpDelete;
				AllUserFpDeleteMgr.FpUserType = StressFpUser;
				CurrentScreen = SCREEN_DeleteAllStressUserFp;
			}		
			else if ( gui_keycode == KEY_ASTERISK )
			{
				DEF_ButtonPress_Voice;
				GoIntoFpMenu_Init();
			}
			else if ( gui_keycode == KEY_POUNDSIGN )
			{
				DEF_ButtonPress_Voice;
				GoIntoFpDeleteMenu_Init();
			}
			break;
			
		case SCREEN_PasscodeMenu:
			
			if ( gui_keycode == KEY_ONE )
			{	
				//DEF_ButtonPress_Voice;
				CurrentScreen = SCREEN_RegisterPasscodeUser;
				PasscodeUserRegisterMgr.Status = StartPasscodeUserRegister;
				PasscodeUserRegisterMgr.UserPriority = Master;
			}
			else if ( gui_keycode == KEY_TWO )
			{	
				//DEF_ButtonPress_Voice;
				CurrentScreen = SCREEN_RegisterPasscodeUser;
				PasscodeUserRegisterMgr.Status = StartPasscodeUserRegister;
				PasscodeUserRegisterMgr.UserPriority = User;
			}
			else if ( gui_keycode == KEY_THREE )
			{	
				//DEF_ButtonPress_Voice;
				CurrentScreen = SCREEN_DeletePasscodeUser;
				PasscodeUserDeleteMgr.Status = StartPasscodeUserDelete;
				PasscodeUserDeleteMgr.UserPriority = Master;
			}
			else if ( gui_keycode == KEY_FOUR )
			{	
				//DEF_ButtonPress_Voice;
				CurrentScreen = SCREEN_DeleteAllPasscodeUser;
				AllPasscodeUserDeleteMgr.Status = StartAllPasscodeUserDelete;
			}
			else if ( gui_keycode == KEY_ASTERISK )
			{
				DEF_ButtonPress_Voice;
				GoIntoUserManagementMenu_Init();
			}
			else if ( gui_keycode == KEY_POUNDSIGN )
			{
				DEF_ButtonPress_Voice;
				GoIntoPasscodeMenu_Init();
			}

			break;

		case SCREEN_CardUserMenu:
			
			if ( gui_keycode == KEY_ONE )
			{	
				//DEF_ButtonPress_Voice;
				CurrentScreen = SCREEN_RegisterCardUser;
				CardUserRegisterMgr.Status = StartCardUserRegister;
			}
			else if ( gui_keycode == KEY_TWO )
			{	
				//DEF_ButtonPress_Voice;
				CurrentScreen = SCREEN_DeleteCardUser;
				CardUserDeleteMgr.Status = StartCardUserDelete;
			}
			else if ( gui_keycode == KEY_THREE )
			{	
				//DEF_ButtonPress_Voice;
				CurrentScreen = SCREEN_DeleteAllCardUser;
				AllCardUserDeleteMgr.Status = StartAllCardUserDelete;
			}
			else if ( gui_keycode == KEY_ASTERISK )
			{	
				DEF_ButtonPress_Voice;
				GoIntoUserManagementMenu_Init();	
			}
			else if ( gui_keycode == KEY_POUNDSIGN )
			{
				DEF_ButtonPress_Voice;
				GoIntoCardMenu_Init();
			}

			break;	


		case SCREEN_InfoInquiryMenu:
			
			if ( gui_keycode == KEY_ONE )
			{	
				DEF_ButtonPress_Voice;
				GoIntoLogMenu_Init();
			}
			else if ( gui_keycode == KEY_TWO )
			{	
				DEF_ButtonPress_Voice;
				GoIntoMemoryUsageScreen();
			}
			else if ( gui_keycode == KEY_THREE )
			{	
				DEF_ButtonPress_Voice;
				GoIntoShowSystemVersion();
			}
			else if ( gui_keycode == KEY_FOUR )
			{	
				DEF_ButtonPress_Voice;
				CurrentScreen = SCREEN_RestoreFactoryDefault;
				RestoreFactoryDefaultMgr.Status = StartRestoreFactoryDefault;		
			}			
			else if ( gui_keycode == KEY_ASTERISK )
			{	
				DEF_ButtonPress_Voice;
				GoIntoMainMenu_Init();	
			}
			else if ( gui_keycode == KEY_POUNDSIGN )
			{
				DEF_ButtonPress_Voice;
				GoIntoInfoInquiryMenu_Init();
			}

			break;	
			
		case SCREEN_EventLogMenu:

			if ( gui_keycode == KEY_ONE )
			{	
				DEF_ButtonPress_Voice;
				CurrentScreen = SCREEN_CheckEventLogBySequence;
				CheckEventLogBySequenceMgr.Status = SearchEventLogWithSequence;
			}
			/*
			else if ( gui_keycode == KEY_TWO )
			{
				CurrentScreen = SCREEN_CheckEventLogByDate;
				CheckEventLogByDateMgr.year = SystemTime.year;
				CheckEventLogByDateMgr.month = SystemTime.month;
				CheckEventLogByDateMgr.date = SystemTime.date;
				CheckEventLogByDateMgr.Status = SetYear;
			}
			else if ( gui_keycode == KEY_THREE )
			{
				CurrentScreen = SCREEN_DeleteEventLog;
				LogDeleteMgr.Status = LogDeleteStart;
			}
			*/
			else if ( gui_keycode == KEY_TWO )
			{	
				DEF_ButtonPress_Voice;
				CurrentScreen = SCREEN_DeleteEventLog;
				LogDeleteMgr.Status = LogDeleteStart;
			}
			else if ( gui_keycode == KEY_ASTERISK )
			{
				DEF_ButtonPress_Voice;
				GoIntoInfoInquiryMenu_Init();
			}
			else if ( gui_keycode == KEY_POUNDSIGN )
			{
				DEF_ButtonPress_Voice;
				GoIntoLogMenu_Init();
			}
			
			break;

			
		case SCREEN_SystemConfigMenu:
			
			if ( gui_keycode == KEY_ONE )
			{	
				DEF_ButtonPress_Voice;
				CurrentScreen = SCREEN_TimeSetting;
				
				TimeSettingMgr.Status = SetYearMajor;
				TempSystemTime.year	=SystemTime.year;
				TempSystemTime.month	=SystemTime.month ;
				TempSystemTime.date	=SystemTime.date ;
				TempSystemTime.day	=SystemTime.day;
				TempSystemTime.hour	=SystemTime.hour;
				TempSystemTime.minute	=SystemTime.minute;
				TempSystemTime.second	=SystemTime.second;

				PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_PleaseInputYear);
			}
			else if ( gui_keycode == KEY_TWO )
			{	
				DEF_ButtonPress_Voice;
				GoIntoVoiceSettingMenu_Init();
			}				
			else if ( gui_keycode == KEY_THREE )
			{	
				DEF_ButtonPress_Voice;
				GoIntoDoorLockSettingMenu_Init();
			}
			else if ( gui_keycode == KEY_FOUR )
			{	
				DEF_ButtonPress_Voice;
				GoIntoMotorSettingMenu_Init();		
			}
			else if ( gui_keycode == KEY_ASTERISK )
			{
				DEF_ButtonPress_Voice;
				GoIntoMainMenu_Init();	
			}
			else if ( gui_keycode == KEY_POUNDSIGN )
			{
				DEF_ButtonPress_Voice;
				GoIntoSystemConfigMenu_Init();
			}

			break;

		case SCREEN_VoiceSettingMenu:

			if ( gui_keycode == KEY_ONE )
			{	
				DEF_ButtonPress_Voice;
				GoIntoLanguageSetting_Init();
			}
			else if ( gui_keycode == KEY_TWO )
			{	
				DEF_ButtonPress_Voice;
				GoIntoVolumeSetting_Init();
			}
			else if ( gui_keycode == KEY_ASTERISK )
			{
				DEF_ButtonPress_Voice;
				GoIntoSystemConfigMenu_Init();	
			}
			else if ( gui_keycode == KEY_POUNDSIGN )
			{
				DEF_ButtonPress_Voice;
				GoIntoVoiceSettingMenu_Init();
			}
			break;

		case SCREEN_EngineeringModeMneu:
 
			if ( gui_keycode == KEY_ONE )
			{
				DEF_ButtonPress_Voice;
				GotoMotorLockDirrectionSetting_Init();		
			}
			else if ( gui_keycode == KEY_TWO )
			{
				DEF_ButtonPress_Voice;
				GotoMotorTorqueSetting_Init();	
			}
			else if ( gui_keycode == KEY_THREE )
			{
				DEF_ButtonPress_Voice;
				GotoBoltLockTimeSetting_Init();
			}
			else if ( gui_keycode == KEY_FOUR )
			{
				DEF_ButtonPress_Voice;
				GotoLockingTravelSetting_Init();	
			}
			else if ( gui_keycode == KEY_FIVE )
			{
				DEF_ButtonPress_Voice;
				GotoAutoEjectSetting_Init();
			}	
			else if ( gui_keycode == KEY_SIX )
			{
				DEF_ButtonPress_Voice;
				GotoMotorSelfTest_Init();
				AutoMotorMgr.SelfTest.Status = SelfTestStart;
				ComPort_SetPost_MotorSelftest();
			}
			else if ( gui_keycode == KEY_SEVEN )
			{
				DEF_ButtonPress_Voice;
				GoIntoBodyInductionSetting_Init();
			}
			else if ( gui_keycode == KEY_ASTERISK )
			{

				DEF_ButtonPress_Voice;
				GoIntoMainScreen_WithIdentifyInit();
			}
			else if ( gui_keycode == KEY_POUNDSIGN )
			{
				DEF_ButtonPress_Voice;
				GoIntoEngineeringModeMenu_Init();
			}

			break;
			
		case SCREEN_AutoMotorSettingMenu:

			 if ( gui_keycode == KEY_ONE )
			{
				DEF_ButtonPress_Voice;
				GotoMotorUnlockTimeSetting_Init();
			}
			else if ( gui_keycode == KEY_TWO )
			{
				DEF_ButtonPress_Voice;
				GotoMotorAutoLockTimeSetting_Init();
			}
			else if ( gui_keycode == KEY_THREE )
			{
				DEF_ButtonPress_Voice;
				GotoMotorSelfTest_Init();
				AutoMotorMgr.SelfTest.Status = SelfTestStart;
				ComPort_SetPost_MotorSelftest();
			}
			else if ( gui_keycode == KEY_FOUR )
			{
				DEF_ButtonPress_Voice;
				GotoMotorTorqueSetting_Init();	
			}
			else if ( gui_keycode == KEY_ASTERISK )
			{
				DEF_ButtonPress_Voice;
				GoIntoSystemConfigMenu_Init();		
			}
			else if ( gui_keycode == KEY_POUNDSIGN )
			{
				DEF_ButtonPress_Voice;
				GoIntoMotorSettingMenu_Init();	
			}

			break;		

		case SCREEN_RegisterMasterFace:
					
			if ( gui_keycode == KEY_ASTERISK )
			{
				if (( FaceUserRegisterMgr.Status != RegisterFaceUserSuccess )
					&&( FaceUserRegisterMgr.Status != RegisterFaceUserFail )
					)
				{
					DEF_ButtonPress_Voice;
					//FaceRecognition_FaceReset();
					FaceRecognition_HardwarePowerOff();
					GoIntoFaceMenu_Init();
				}
			}
			else if ( gui_keycode == KEY_POUNDSIGN )
			{
				if ( FaceUserRegisterMgr.Status == ReportFaceUserID )
				{
					FaceUserRegisterMgr.Status = AddUserToFRM;	
					GUI_FaceTemplateRegisterMgr.Status = StartFaceRegister;
					GUI_Flag_RefreshLCD = bTRUE;
				}
			}
				
			break;
		
	
		case SCREEN_RegisterUserFace:
			
			if ( gui_keycode == KEY_ASTERISK )
			{
				if (( FaceUserRegisterMgr.Status != RegisterFaceUserSuccess )
					&&( FaceUserRegisterMgr.Status != RegisterFaceUserFail )
					)
				{
					DEF_ButtonPress_Voice;
					//FaceRecognition_FaceReset();
					FaceRecognition_HardwarePowerOff();
					GoIntoFaceMenu_Init();
				}
			}
			else if ( gui_keycode == KEY_POUNDSIGN )
			{
				if ( FaceUserRegisterMgr.Status == ReportFaceUserID )
				{
					FaceUserRegisterMgr.Status = AddUserToFRM;	
					GUI_FaceTemplateRegisterMgr.Status = StartFaceRegister;
					GUI_Flag_RefreshLCD = bTRUE;
				}
			}
				
			break;
		
		
		case SCREEN_DeleteUserFace:
			
			if ( FaceUserDeleteMgr.Status == InputUserID )
			{
				GUI_UserIDinputButtonMonitor(gui_keycode);
			}
			
			break;
			
		case SCREEN_RegisterManagerFPC:
					
			if ( gui_keycode == KEY_ASTERISK )
			{
//				if (( FaceUserRegisterMgr.Status != RegisterFaceUserSuccess )
//					&&( FaceUserRegisterMgr.Status != RegisterFaceUserFail )
//					)
//				{
					DEF_ButtonPress_Voice;
					//FaceRecognition_FaceReset();
////					FaceRecognition_HardwarePowerOff();
					AddManagerMenu_Init();
//				}
			}
			else if ( gui_keycode == KEY_POUNDSIGN )
			{
////				if ( FaceUserRegisterMgr.Status == ReportFaceUserID )
////				{
////					FaceUserRegisterMgr.Status = AddUserToFRM;	
////					GUI_FaceTemplateRegisterMgr.Status = StartFaceRegister;
////					GUI_Flag_RefreshLCD = bTRUE;
////				}
			}
			
			//GUI_UserIDinputButtonMonitor(gui_keycode);
				
			break;
			
		case SCREEN_ResManagerFPCInput:
			
			GUI_UserIDinputButtonMonitor(gui_keycode);
			if(gui_keycode == KEY_POUNDSIGN ){
					//ShowResManagerIDFPC();
					//CurrentScreen = SCREEN_ResManagerIdFPC;
					GoInTo_ResManagerIdFPCInit();
					DataInputMgr.Status = InputIdle;
				}
			if(DataInputMgr.Status == InputExit){
				DataInputMgr.Status = InputIdle;
				if ( gui_keycode == KEY_ASTERISK ){
					AddManagerMenu_Init();
				}
			}
			break;
			
		case SCREEN_RegisterManagerFace:
			
			if ( gui_keycode == KEY_ASTERISK )
			{
//				if (( FaceUserRegisterMgr.Status != RegisterFaceUserSuccess )
//					&&( FaceUserRegisterMgr.Status != RegisterFaceUserFail )
//					)
//				{
					DEF_ButtonPress_Voice;
					//FaceRecognition_FaceReset();
////					FaceRecognition_HardwarePowerOff();
					AddManagerMenu_Init();
//				}
			}
			else if ( gui_keycode == KEY_POUNDSIGN )
			{
////				if ( FaceUserRegisterMgr.Status == ReportFaceUserID )
////				{
////					FaceUserRegisterMgr.Status = AddUserToFRM;	
////					GUI_FaceTemplateRegisterMgr.Status = StartFaceRegister;
////					GUI_Flag_RefreshLCD = bTRUE;
////				}
			}
				
			break;
			
		case SCREEN_ResManagerFaceInput:
			
			GUI_UserIDinputButtonMonitor(gui_keycode);
			if(gui_keycode == KEY_POUNDSIGN ){
					//ShowResManagerIDFPC();
					//CurrentScreen = SCREEN_ResManagerIdFPC;
					//GoInTo_ResManagerIdFPCInit();
					GoInTo_ResManagerIdFaceInit();
					DataInputMgr.Status = InputIdle;
					
					
				}
			if(DataInputMgr.Status == InputExit){
				DataInputMgr.Status = InputIdle;
				if ( gui_keycode == KEY_ASTERISK ){
					AddManagerMenu_Init();
				}
			}
			break;
		
		case SCREEN_RegisterNormalUserFPC:
			
			if ( gui_keycode == KEY_ASTERISK )
			{
//				if (( FaceUserRegisterMgr.Status != RegisterFaceUserSuccess )
//					&&( FaceUserRegisterMgr.Status != RegisterFaceUserFail )
//					)
//				{
					DEF_ButtonPress_Voice;
					//FaceRecognition_FaceReset();
////					FaceRecognition_HardwarePowerOff();
					AddUserMenu_Init();
//				}
			}
			else if ( gui_keycode == KEY_POUNDSIGN )
			{
////				if ( FaceUserRegisterMgr.Status == ReportFaceUserID )
////				{
////					FaceUserRegisterMgr.Status = AddUserToFRM;	
////					GUI_FaceTemplateRegisterMgr.Status = StartFaceRegister;
////					GUI_Flag_RefreshLCD = bTRUE;
////				}
			}
				
			break;
			
		case SCREEN_ResNormalUserFPCInput:
			GUI_UserIDinputButtonMonitor(gui_keycode);
			if(gui_keycode == KEY_POUNDSIGN ){
					//ShowResManagerIDFPC();
					//CurrentScreen = SCREEN_ResManagerIdFPC;
					//GoInTo_ResManagerIdFPCInit();
					GoInTo_ResNormalIdFPCInit();
					DataInputMgr.Status = InputIdle;
					
					
				}
			if(DataInputMgr.Status == InputExit){
				DataInputMgr.Status = InputIdle;
				if ( gui_keycode == KEY_ASTERISK ){
					AddUserMenu_Init();
				}
			}
			break;
			
		case SCREEN_RegisterNormalUserFace:
			
			if ( gui_keycode == KEY_ASTERISK )
			{
//				if (( FaceUserRegisterMgr.Status != RegisterFaceUserSuccess )
//					&&( FaceUserRegisterMgr.Status != RegisterFaceUserFail )
//					)
//				{
					DEF_ButtonPress_Voice;
					//FaceRecognition_FaceReset();
////					FaceRecognition_HardwarePowerOff();
					AddUserMenu_Init();
//				}
			}
			else if ( gui_keycode == KEY_POUNDSIGN )
			{
////				if ( FaceUserRegisterMgr.Status == ReportFaceUserID )
////				{
////					FaceUserRegisterMgr.Status = AddUserToFRM;	
////					GUI_FaceTemplateRegisterMgr.Status = StartFaceRegister;
////					GUI_Flag_RefreshLCD = bTRUE;
////				}
			}
				
			break;
			
		case SCREEN_DelUserId:
			if ( gui_keycode == KEY_ASTERISK )
			{
//				if (( FaceUserRegisterMgr.Status != RegisterFaceUserSuccess )
//					&&( FaceUserRegisterMgr.Status != RegisterFaceUserFail )
//					)
//				{
					DEF_ButtonPress_Voice;
					//FaceRecognition_FaceReset();
////					FaceRecognition_HardwarePowerOff();
					DeleteUserMenu_Init();
//				}
			}	
			break;
			
		case SCREEN_ResNormalUserFaceInput:
			
			GUI_UserIDinputButtonMonitor(gui_keycode);
			if(gui_keycode == KEY_POUNDSIGN ){
					//ShowResManagerIDFPC();
					//CurrentScreen = SCREEN_ResManagerIdFPC;
					//GoInTo_ResManagerIdFPCInit();
					GoInTo_ResNormalIdFaceInit();
					DataInputMgr.Status = InputIdle;
					
					
				}
			if(DataInputMgr.Status == InputExit){
				DataInputMgr.Status = InputIdle;
				if ( gui_keycode == KEY_ASTERISK ){
					AddUserMenu_Init();
				}
			}
			break;
			
		case SCREEN_NumberDelete:
			
			if ( gui_keycode == KEY_ASTERISK )
			{
//				if (( FaceUserRegisterMgr.Status != RegisterFaceUserSuccess )
//					&&( FaceUserRegisterMgr.Status != RegisterFaceUserFail )
//					)
//				{
					DEF_ButtonPress_Voice;
					//FaceRecognition_FaceReset();
////					FaceRecognition_HardwarePowerOff();
					DeleteUserMenu_Init();
//				}
			}
			else if ( gui_keycode == KEY_POUNDSIGN )
			{
////				if ( FaceUserRegisterMgr.Status == ReportFaceUserID )
////				{
////					FaceUserRegisterMgr.Status = AddUserToFRM;	
////					GUI_FaceTemplateRegisterMgr.Status = StartFaceRegister;
////					GUI_Flag_RefreshLCD = bTRUE;
////				}
			}
			
			break;
			
		case SCREEN_DelUserIdInput:
			
			GUI_UserIDinputButtonMonitor(gui_keycode);
			if(gui_keycode == KEY_POUNDSIGN ){
					//ShowResManagerIDFPC();
					//CurrentScreen = SCREEN_ResManagerIdFPC;
					//GoInTo_ResManagerIdFPCInit();
					GoInTo_DelUserIdInit();
					DataInputMgr.Status = InputIdle;
					
					
				}
			if(DataInputMgr.Status == InputExit){
				DataInputMgr.Status = InputIdle;
				if ( gui_keycode == KEY_ASTERISK ){
					DeleteUserMenu_Init();
				}else if(gui_keycode ==KEY_POUNDSIGN){
					
				}
			}
			break;
			
		case SCREEN_AllDelete:
			
			if ( gui_keycode == KEY_ASTERISK )
			{
//				if (( FaceUserRegisterMgr.Status != RegisterFaceUserSuccess )
//					&&( FaceUserRegisterMgr.Status != RegisterFaceUserFail )
//					)
//				{
					DEF_ButtonPress_Voice;
					//FaceRecognition_FaceReset();
////					FaceRecognition_HardwarePowerOff();
					DeleteUserMenu_Init();
//				}
			}
			else if ( gui_keycode == KEY_POUNDSIGN )
			{
////				if ( FaceUserRegisterMgr.Status == ReportFaceUserID )
////				{
////					FaceUserRegisterMgr.Status = AddUserToFRM;	
////					GUI_FaceTemplateRegisterMgr.Status = StartFaceRegister;
////					GUI_Flag_RefreshLCD = bTRUE;
////				}
				ShowOperateSuccess();
			}
			
			break;

		case SCREEN_RegisterMasterFp: 
				
			if ( gui_keycode == KEY_ASTERISK )
			{
				if (( FpUserRegisterMgr.Status != RegisterUserSuccess )
					&&( FpUserRegisterMgr.Status != RegisterUserFail )
					)
				{
					DEF_ButtonPress_Voice;
					GoIntoFpMenu_Init();
				}
			}
			else if ( gui_keycode == KEY_POUNDSIGN )
			{
				if ( FpUserRegisterMgr.Status == ReportFpUserID )
				{
					FpUserRegisterMgr.Status = AddUserToFPM;	
					FpRegisterMgr.Status = FPMcmdStart;		
					GUI_Flag_RefreshLCD = bTRUE;
				}
			}
				
			break;

		case SCREEN_RegisterUserFp:

			if ( gui_keycode == KEY_ASTERISK )
			{
				if (( FpUserRegisterMgr.Status != RegisterUserSuccess )
					&&( FpUserRegisterMgr.Status != RegisterUserFail )
					)
				{
					DEF_ButtonPress_Voice;
					GoIntoFpMenu_Init();
				}
			}
			else if ( gui_keycode == KEY_POUNDSIGN )
			{
				if ( FpUserRegisterMgr.Status == ReportFpUserID )
				{
					FpUserRegisterMgr.Status = AddUserToFPM;	
					FpRegisterMgr.Status = FPMcmdStart;		
					GUI_Flag_RefreshLCD = bTRUE;
				}
			}
				
			break;
		
			
		case SCREEN_DeleteUserFp:
			
			if ( FpUserDeleteMgr.Status == InputUserID )
			{
				GUI_UserIDinputButtonMonitor(gui_keycode);
			}

			break;

		case SCREEN_DeleteAllUserFp:
			
			if ( AllUserFpDeleteMgr.Status == WaitForUserConfirmDeleteAllFP )
			{
				if ( gui_keycode == KEY_POUNDSIGN )
				{
					AllUserFpDeleteMgr.Status = SendDeleteAllUserFpCmdToFPM;
				}
				else if ( gui_keycode == KEY_ASTERISK )
				{
					DEF_ButtonPress_Voice;
					AllUserFpDeleteMgr.Status = EXIT;
				}
			}

			break;

		case SCREEN_RegisterStressUserFp:
				
			
			if ( gui_keycode == KEY_ASTERISK )
			{
				if (( FpUserRegisterMgr.Status != RegisterUserSuccess )
					&&( FpUserRegisterMgr.Status != RegisterUserFail )
					)
				{
					DEF_ButtonPress_Voice;
					GoIntoFpMenu_Init();
				}
			}
			else if ( gui_keycode == KEY_POUNDSIGN )
			{
				if ( FpUserRegisterMgr.Status == ReportFpUserID )
				{
					FpUserRegisterMgr.Status = AddUserToFPM;	
					FpRegisterMgr.Status = FPMcmdStart;		
					GUI_Flag_RefreshLCD = bTRUE;
				}
			}
				
			break;
			
		case SCREEN_DeleteStressUserFp:
			
			if ( FpUserDeleteMgr.Status == InputUserID )
			{
				GUI_UserIDinputButtonMonitor(gui_keycode);
			}

			break;

		case SCREEN_DeleteAllStressUserFp:
			
			if ( AllUserFpDeleteMgr.Status == WaitForUserConfirmDeleteAllFP )
			{
				if ( gui_keycode == KEY_POUNDSIGN )
				{
					AllUserFpDeleteMgr.Status = SendDeleteAllUserFpCmdToFPM;
				}
				else if ( gui_keycode == KEY_ASTERISK )
				{
					DEF_ButtonPress_Voice;
					AllUserFpDeleteMgr.Status = EXIT;
				}
			}

			break;

		case SCREEN_RegisterCardUser:
			

				if ( gui_keycode == KEY_ASTERISK )
				{
					if (( CardUserRegisterMgr.Status != Success )
						&&( CardUserRegisterMgr.Status != Fail )
						)
					{
						DEF_ButtonPress_Voice;
						GoIntoCardMenu_Init();
					}
				}
				else if ( gui_keycode == KEY_POUNDSIGN )
				{
					if ( CardUserRegisterMgr.Status == ReportCardUserID )
					{
						PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_PleaseSwingCard);
						CardUserRegisterMgr.Status = ReadingCardID;
						CardUserRegisterMgr.TimeCnt = Def_UserSwingCardTimeDelay;	//set to 5s
						GUI_RefreshSleepTime();
						GUI_Flag_RefreshLCD = bTRUE;
					}
				}
			break;
			
		case SCREEN_DeleteCardUser:

				if ( CardUserDeleteMgr.Status == InputCardUserID )
				{
					GUI_UserIDinputButtonMonitor(gui_keycode);
				}
			break;

		case SCREEN_DeleteAllCardUser:
				
				if ( AllCardUserDeleteMgr.Status == WaitForUserConfirmDeleteAllCard )
				{
					if ( gui_keycode == KEY_POUNDSIGN )
					{
						AllCardUserDeleteMgr.Status = DeletingAllCardUser;
					}
					else if ( gui_keycode == KEY_ASTERISK )
					{
						DEF_ButtonPress_Voice;
						AllCardUserDeleteMgr.Status = CardUserEXIT;
					}	
				}

			break;	


		case SCREEN_RegisterPasscodeUser:

				if ( PasscodeUserRegisterMgr.Status == InputFirstPasscode )
				{
					if ( gui_keycode != KEY_NONE )
					{
						GUI_PasscodeInputButtonMonitor(gui_keycode);
						PasscodeUserRegisterMgr.TimeCnt = Def_GuiTimeDelayCnt10s;
					}
				}
				else if ( PasscodeUserRegisterMgr.Status == InputSecondPasscode )
				{
					if ( gui_keycode != KEY_NONE )
					{
						GUI_PasscodeInputButtonMonitor(gui_keycode);
						PasscodeUserRegisterMgr.TimeCnt = Def_GuiTimeDelayCnt10s;
					}
				}
				else if ( PasscodeUserRegisterMgr.Status == ReportPasscodeUserID )
				{
					if ( gui_keycode == KEY_POUNDSIGN )
					{
						PasscodeUserRegisterMgr.Status = InputFirstPasscode;	
						PasscodeUserRegisterMgr.TimeCnt = Def_GuiTimeDelayCnt10s;
						PasscodeInputMgr.Point = 0x00;
						PasscodeInputMgr.PasscodeLen = 12;
						PasscodeInputMgr.Status = PasscodeInputStart;
						PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_PleaseInputPassword);
						GUI_Flag_RefreshLCD = bTRUE;

					}
					else if ( gui_keycode == KEY_ASTERISK )
					{
						GoIntoPasscodeMenu_Init();
					}
				}
		
			break;
			
		case SCREEN_DeletePasscodeUser:

			if ( PasscodeUserDeleteMgr.Status == InputPasscodeUserID )
				{
					GUI_UserIDinputButtonMonitor(gui_keycode);
				}

			break;

		case SCREEN_DeleteAllPasscodeUser:
			
			if ( AllPasscodeUserDeleteMgr.Status == WaitForDeleteAllPasscodeUserConfirm )
			{
				if ( gui_keycode == KEY_POUNDSIGN )
				{
					AllPasscodeUserDeleteMgr.Status = DeletingAllPasscodeUser;
				}
				else if ( gui_keycode == KEY_ASTERISK )
				{
					DEF_ButtonPress_Voice;
					AllPasscodeUserDeleteMgr.Status = PasscodeUserEXIT;
				}
			}


			break;
			

		case SCREEN_TimeSetting:

				if ( TimeSettingMgr.Status == SetYearMajor)
				{
					if ( (gui_keycode < KEY_NINE )||(gui_keycode == KEY_NINE) )
					{
						DEF_ButtonPress_Voice;
						TempSystemTime.year = (TempSystemTime.year&0x0F)+(gui_keycode<<4);
						TimeSettingMgr.Status = SetYearMinor;
					}
				}
				else if ( TimeSettingMgr.Status == SetYearMinor)
				{
					if ( (gui_keycode < KEY_NINE )||(gui_keycode == KEY_NINE) )
					{
						DEF_ButtonPress_Voice;
						TempSystemTime.year =(TempSystemTime.year&0xF0)+gui_keycode;
						TimeSettingMgr.Status = SetMonthMajor;
						VoiceReportYearAndInputMonth(TempSystemTime.year);
					}
				}
				
				else if ( TimeSettingMgr.Status == SetMonthMajor)
				{
					if ( (gui_keycode < KEY_TWO) )
					{
						DEF_ButtonPress_Voice;
						TempSystemTime.month= (TempSystemTime.month&0x0F)+(gui_keycode<<4);
						TimeSettingMgr.Status = SetMonthMinor;
					}
				}
				else if ( TimeSettingMgr.Status == SetMonthMinor)
				{
					if ( (gui_keycode < KEY_NINE )||(gui_keycode == KEY_NINE) )
					{
						DEF_ButtonPress_Voice;
						TempSystemTime.month=(TempSystemTime.month&0xF0)+gui_keycode;
						if ( TempSystemTime.month > 0x12 )
						{
							//TempSystemTime.month = 0x12;
							VoiceReportInputMonthErrorAndRetry();
							TimeSettingMgr.Status = SetMonthMajor;
						}
						else
						{
							TimeSettingMgr.Status = SetDateMajor;
							VoiceReportMonthAndInputDate(TempSystemTime.month);
						}
					}
				}
				
				else if ( TimeSettingMgr.Status == SetDateMajor)
				{
					if ( (gui_keycode < KEY_FOUR) )
					{
						DEF_ButtonPress_Voice;
						TempSystemTime.date= (TempSystemTime.date&0x0F)+(gui_keycode<<4);
						TimeSettingMgr.Status = SetDateMinor;
					}
				}
				else if ( TimeSettingMgr.Status == SetDateMinor)
				{
					if ( (gui_keycode < KEY_NINE )||(gui_keycode == KEY_NINE) )
					{
						DEF_ButtonPress_Voice;
						TempSystemTime.date=(TempSystemTime.date&0xF0)+gui_keycode;

						if ( is_valid_date(TempSystemTime.year, TempSystemTime.month, TempSystemTime.date) == bTRUE )
						{
							TimeSettingMgr.Status = SetHourMajor;
							VoiceReportDateAndInputHour(TempSystemTime.date);
						}
						else
						{
							VoiceReportInputDateErrorAndRetry();
							TimeSettingMgr.Status = SetDateMajor;
						}
					}
				}

				else if ( TimeSettingMgr.Status == SetHourMajor)
				{
					if ( (gui_keycode < KEY_THREE )){
						DEF_ButtonPress_Voice;
						TempSystemTime.hour= (TempSystemTime.hour&0x0F)+(gui_keycode<<4);
						TimeSettingMgr.Status = SetHourMinor;
					}
				}
				else if ( TimeSettingMgr.Status == SetHourMinor)
				{
					if( (gui_keycode < KEY_NINE )||(gui_keycode == KEY_NINE) )
					{
						DEF_ButtonPress_Voice;
						TempSystemTime.hour=(TempSystemTime.hour&0xF0)+gui_keycode;
						if (TempSystemTime.hour > 0x23)
						{
							VoiceReportInputHourErrorAndRetry();
							TimeSettingMgr.Status = SetHourMajor;
						}
						else
						{
							TimeSettingMgr.Status = SetMinuteMajor;
							VoiceReportHourAndInputMinute(TempSystemTime.hour);
						}
					}
				}


				else if ( TimeSettingMgr.Status == SetMinuteMajor)
				{
					if ( gui_keycode < KEY_SIX)
					{
						DEF_ButtonPress_Voice;
						TempSystemTime.minute= (TempSystemTime.minute&0x0F)+(gui_keycode<<4);
						TimeSettingMgr.Status = SetMinuteMinor;
					}
				}
				else if ( TimeSettingMgr.Status == SetMinuteMinor)
				{
					if ( (gui_keycode < KEY_NINE )||(gui_keycode == KEY_NINE) ){
						DEF_ButtonPress_Voice;
						TempSystemTime.minute=(TempSystemTime.minute&0xF0)+gui_keycode;
						TimeSettingMgr.Status = SetSecondMajor;
						VoiceReportMinuteAndInputSecond(TempSystemTime.minute);
					}
				}

				else if ( TimeSettingMgr.Status == SetSecondMajor)
				{
					if ( gui_keycode < KEY_SIX){
						DEF_ButtonPress_Voice;
						TempSystemTime.second= (TempSystemTime.second&0x8F)+(gui_keycode<<4);
						TimeSettingMgr.Status = SetSecondMinor;
					}
				}
				else if ( TimeSettingMgr.Status == SetSecondMinor)
				{
					if ( (gui_keycode < KEY_NINE )||(gui_keycode == KEY_NINE) ){
						DEF_ButtonPress_Voice;
						TempSystemTime.second=(TempSystemTime.second&0xF0)+gui_keycode;
						//TimeSettingMgr.Status = SetYearMajor;
						VoiceReportSecond(TempSystemTime.second);
					}
				}

				if ( gui_keycode == KEY_POUNDSIGN )
				{

						//DEF_ButtonPress_Voice;
					PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_OperationSuccess);
					SystemTime.year    	=	TempSystemTime.year;
					SystemTime.month 	= 	TempSystemTime.month;
					SystemTime.date    	= 	TempSystemTime.date;
					SystemTime.day		= 	TempSystemTime.day;
					SystemTime.hour    	= 	TempSystemTime.hour;
					SystemTime.minute 	= 	TempSystemTime.minute;
					SystemTime.second 	= 	TempSystemTime.second;
					
					WriteSystemTime();
					
					GoIntoSystemConfigMenu_Init();		
				}
				else if ( gui_keycode == KEY_ASTERISK )
				{
					DEF_ButtonPress_Voice;
					GoIntoSystemConfigMenu_Init();	
				}

			break;

		  case SCREEN_RestoreFactoryDefault:

				if ( RestoreFactoryDefaultMgr.Status == WaitForRestoreFactoryDefaultUserConfirm )
				{
					if ( gui_keycode == KEY_ASTERISK )
					{
						DEF_ButtonPress_Voice;
						RestoreFactoryDefaultMgr.Status = RestoreFactoryDefaultEXIT;
					}
					else if ( gui_keycode == KEY_POUNDSIGN )
					{
						DEF_ButtonPress_Voice;
						RestoreFactoryDefaultMgr.Status = ConfirmedToRestoreFactoryDefault;
					}
				}

				break;

		  case SCREEN_LanguageSetting:
	
				if( gui_keycode == KEY_ONE )
				{
					SystemLanguage = Chinese;
					VoiceReportCurrentLanguage();
				}
				else if ( gui_keycode == KEY_TWO )
				{
					SystemLanguage = English;
					VoiceReportCurrentLanguage();
				}
				else if ( gui_keycode == KEY_ASTERISK )
				{
					SystemConfigSave();
					GoIntoVoiceSettingMenu_Init();	
				}
				else if ( gui_keycode == KEY_POUNDSIGN )
				{
					GoIntoLanguageSetting_Init();	
				}
				break;

		  case SCREEN_VolumeSetting:

				if ( gui_keycode == KEY_ONE )
				{
					VoiceMgr.volume = 3;
					//SET_VOLUME(VoiceMgr.volume*5);	
					VoiceReportCurrentVolume();
				}
				else if ( gui_keycode == KEY_TWO )
				{
					VoiceMgr.volume = 2;
					//SET_VOLUME(VoiceMgr.volume*5);
					VoiceReportCurrentVolume();
				}

				else if ( gui_keycode == KEY_THREE )
				{
					VoiceMgr.volume = 1;			
					//SET_VOLUME(VoiceMgr.volume*5);
					VoiceReportCurrentVolume();
				}
				else if ( gui_keycode == KEY_FOUR )
				{	
					VoiceMgr.volume = 0;		
					//SET_VOLUME(VoiceMgr.volume*5);	
					VoiceReportCurrentVolume();
				}
				else if ( gui_keycode == KEY_ASTERISK )
				{
					DEF_ButtonPress_Voice;
					SystemConfigSave();
					GoIntoVoiceSettingMenu_Init();	
				}
				else if ( gui_keycode == KEY_POUNDSIGN )
				{
					DEF_ButtonPress_Voice;
					GoIntoVolumeSetting_Init();	
				}
				break;	

          case SCREEN_AutoMotorLockDirection:
			
				if ( gui_keycode == KEY_ONE )
				{
					DEF_ButtonPress_Voice;
					AutoMotorMgr.LockDirection= LEFTOPEN;
					VoiceReportCurrentUnlockDirrect();
				}
				else if ( gui_keycode == KEY_TWO )
				{
					DEF_ButtonPress_Voice;
					AutoMotorMgr.LockDirection= RIGHTOPEN;
					VoiceReportCurrentUnlockDirrect();
				}
				else if ( gui_keycode == KEY_ASTERISK )
				{
					DEF_ButtonPress_Voice;
					EEPROM_WriteSequential(AutoMotorMgrLockDirectionAddr,&AutoMotorMgr.LockDirection,1);
					GoIntoEngineeringModeMenu_Init();	
					ComPort_SetPost_Parameter();
				}
				else if ( gui_keycode == KEY_POUNDSIGN )
				{
					GotoMotorLockDirrectionSetting_Init();		
				}
					
				break;

         case SCREEN_AutoMotorTorque:
				
				if ( gui_keycode == KEY_ONE )
				{
					DEF_ButtonPress_Voice;
					AutoMotorMgr.TorqueLevel = LowTorque;
					VoiceReportCurrentTorque();
				}
				else if ( gui_keycode == KEY_TWO )
				{
					DEF_ButtonPress_Voice;
					AutoMotorMgr.TorqueLevel = MiddleTorque;
					VoiceReportCurrentTorque();
				}
				else if ( gui_keycode == KEY_THREE )
				{
					DEF_ButtonPress_Voice;
					AutoMotorMgr.TorqueLevel = LargeTorque;
					VoiceReportCurrentTorque();
				}
				else if ((gui_keycode == KEY_ASTERISK))
				{
					DEF_ButtonPress_Voice;
					EEPROM_WriteSequential(AutoMotorMgrTorqueAddr,&AutoMotorMgr.TorqueLevel,1);
					if ( IfSystemIsInFactoryDefaultStatus() == bTRUE )
					{
						GoIntoEngineeringModeMenu_Init();
					}
					else
					{
						GoIntoMotorSettingMenu_Init();
					}
					ComPort_SetPost_Parameter();		
				}
				else if ( gui_keycode == KEY_POUNDSIGN )
				{
					GotoMotorTorqueSetting_Init();	
				}


				break;
				
			case SCREEN_AutoMotorUnlockTime:
			
				   if ( gui_keycode == KEY_TWO )
				   {
						DEF_ButtonPress_Voice;
						if ( AutoMotorMgr.UnlockTime < 10 )
						{
						   AutoMotorMgr.UnlockTime++;
						}
				   		VoiceReportCurrentSettingTime_Integer(AutoMotorMgr.UnlockTime);
				   }
				   else if ( gui_keycode == KEY_EIGHT )
				   {
					   DEF_ButtonPress_Voice;
					   if ( AutoMotorMgr.UnlockTime > 2 )
					   {
						   AutoMotorMgr.UnlockTime--;
					   }
					   VoiceReportCurrentSettingTime_Integer(AutoMotorMgr.UnlockTime);
				   }
				   else if ( gui_keycode == KEY_ASTERISK )
				   {
					   DEF_ButtonPress_Voice;
					   EEPROM_WriteSequential(AutoMotorMgrUnlockTimeAddr,&AutoMotorMgr.UnlockTime,1);
					   GoIntoMotorSettingMenu_Init();  
					   ComPort_SetPost_Parameter();
				   }
				   else if ( gui_keycode == KEY_POUNDSIGN )
				   {
						GotoMotorUnlockTimeSetting_Init();	
				   }
				   break;  
				   
			 case SCREEN_AutoMotorAutoLockTime:
	
				   if ( gui_keycode == KEY_TWO )
				   {
						DEF_ButtonPress_Voice;
					   if ( AutoMotorMgr.AutoLockTime < 5 )
					   {
						   AutoMotorMgr.AutoLockTime=5;
					   }
					   else if ( AutoMotorMgr.AutoLockTime < 20 )
					   {
						   AutoMotorMgr.AutoLockTime++;
					   }
					   VoiceReportCurrentSettingTime_Integer(AutoMotorMgr.AutoLockTime);
				   }
				   else if ( gui_keycode == KEY_EIGHT )
				   {
					   DEF_ButtonPress_Voice;
					   if ( AutoMotorMgr.AutoLockTime >5 )
					   {
						   AutoMotorMgr.AutoLockTime--;
						   VoiceReportCurrentSettingTime_Integer(AutoMotorMgr.AutoLockTime);
					   }
					   else
					   {
						   AutoMotorMgr.AutoLockTime=0;
						   PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,AutoLockOffVoiceStr);
					   }
					   
				   }
				   else if ( gui_keycode == KEY_ASTERISK )
				   {
					   DEF_ButtonPress_Voice;
					   EEPROM_WriteSequential(AutoMotorMgrAutoLockTimeAddr,&AutoMotorMgr.AutoLockTime,1);
					   GoIntoMotorSettingMenu_Init();  
					   ComPort_SetPost_Parameter();
				   }	
				   else if ( gui_keycode == KEY_POUNDSIGN )
				   {
						GotoMotorAutoLockTimeSetting_Init();	
				   }
				   break;  

				
		case SCREEN_AutoMotorBoltLockTime:

				if ( gui_keycode == KEY_TWO )
				{
					DEF_ButtonPress_Voice;
					if ( AutoMotorMgr.BoltLockTime< 23){
						AutoMotorMgr.BoltLockTime+=2;
					}
					VoiceReportCurrentSettingTime_Decimal(AutoMotorMgr.BoltLockTime/2);
				}
				else if ( gui_keycode == KEY_EIGHT )
				{
					DEF_ButtonPress_Voice;
					if ( AutoMotorMgr.BoltLockTime> 3 )
					{
						AutoMotorMgr.BoltLockTime-=2;
					}
					VoiceReportCurrentSettingTime_Decimal(AutoMotorMgr.BoltLockTime/2);
				}
				else if ((gui_keycode == KEY_ASTERISK) )
				{
					DEF_ButtonPress_Voice;
					EEPROM_WriteSequential(AutoMotorMgrBoltLockTimeAddr,&AutoMotorMgr.BoltLockTime,1);
					GoIntoEngineeringModeMenu_Init();	
					ComPort_SetPost_Parameter();
				}
				else if ( gui_keycode == KEY_POUNDSIGN )
			   {
					GotoBoltLockTimeSetting_Init();	
			   }
				break;	
				
		case SCREEN_LockingTravel:

				if ( gui_keycode == KEY_TWO )
				{
					DEF_ButtonPress_Voice;
					if ( AutoMotorMgr.LockingTravel< 8){
						AutoMotorMgr.LockingTravel++;
					}
					VoiceReportCurrentTravelSetting(AutoMotorMgr.LockingTravel);
				}
				else if ( gui_keycode == KEY_EIGHT )
				{
					DEF_ButtonPress_Voice;
					if ( AutoMotorMgr.LockingTravel> 1 )
					{
						AutoMotorMgr.LockingTravel--;
					}
					VoiceReportCurrentTravelSetting(AutoMotorMgr.LockingTravel);
				}
				else if ( (gui_keycode == KEY_POUNDSIGN)||(gui_keycode == KEY_ASTERISK) )
				{
					DEF_ButtonPress_Voice;
					EEPROM_WriteSequential(AutoMotorMgrLockingTravelAddr,&AutoMotorMgr.LockingTravel,1);
					GoIntoEngineeringModeMenu_Init();	
					ComPort_SetPost_Parameter();
				}
				else if ( gui_keycode == KEY_POUNDSIGN )
			   {
					GotoLockingTravelSetting_Init();	
			   }
				
				break;		
				
		case SCREEN_AutoEject:
		
				if ( gui_keycode == KEY_ONE )
				{
					DEF_ButtonPress_Voice;
					AutoMotorMgr.AutoEject = 0x01;
					PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,AutoEjectOnVoiceStr);
				}
				else if ( gui_keycode == KEY_TWO )
				{
					DEF_ButtonPress_Voice;
					AutoMotorMgr.AutoEject = 0x00;
					PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,AutoEjectOffVoiceStr);
				}
				else if ( gui_keycode == KEY_ASTERISK )
				{
					DEF_ButtonPress_Voice;
					EEPROM_WriteSequential(AutoMotorMgrAutoEjectAddr,&AutoMotorMgr.AutoEject,1);
					GoIntoEngineeringModeMenu_Init();	
					ComPort_SetPost_Parameter();
				}
				else if ( gui_keycode == KEY_POUNDSIGN )
			   {
					GotoAutoEjectSetting_Init();	
			   }

				break;
							
			case SCREEN_MemoryUsage:
				
				if ( gui_keycode == KEY_TWO )
				{
					DEF_ButtonPress_Voice;
					if ( DisplayMemoryUsageMgr.Status == DisplayCardUserMemoryUsage )
					{
						DisplayMemoryUsageMgr.Status = DisplayPasscodeUserMemoryUsage;
					}
					else if ( DisplayMemoryUsageMgr.Status == DisplayPasscodeUserMemoryUsage )
					{
						DisplayMemoryUsageMgr.Status = DisplayFpUserMemoryUsage;
					}
				}
				else if ( gui_keycode == KEY_EIGHT )
				{
					DEF_ButtonPress_Voice;
					if ( DisplayMemoryUsageMgr.Status == DisplayFpUserMemoryUsage )
					{
						DisplayMemoryUsageMgr.Status = DisplayPasscodeUserMemoryUsage;
					}
					else if ( DisplayMemoryUsageMgr.Status == DisplayPasscodeUserMemoryUsage )
					{
						DisplayMemoryUsageMgr.Status = DisplayCardUserMemoryUsage;
					}
				}
				if ( gui_keycode == KEY_ASTERISK )
				{
					DEF_ButtonPress_Voice;
					GoIntoInfoInquiryMenu_Init();
				}
				else if (gui_keycode == KEY_POUNDSIGN)
				{
					DEF_ButtonPress_Voice;
					GoIntoMemoryUsageScreen();
				}
				
				break;
				
			case SCREEN_ManagerIdentify:

					if ( ManagerIdentifyMgr.Status == IdentifyingManager )
						{
							GUI_PasscodeInputButtonMonitor(gui_keycode);
						}
				break;

			case SCREEN_DoorLockSettingMenu:

				if ( gui_keycode == KEY_ONE )
				{
					DEF_ButtonPress_Voice;
					GoIntoUnlockingModeSetting_Init();
				}
				else if ( gui_keycode == KEY_TWO )
				{
					DEF_ButtonPress_Voice;
					GoIntoPickAlarmEnableSetting_Init();
				}
				else if ( gui_keycode == KEY_THREE )
				{
					if ( FrmMgr.FrmFunctionConfig == FRM_Enabled )
					{
						GoIntoBodyInductionSetting_Init();
					}
				}
				else if ( gui_keycode == KEY_ASTERISK )
				{
					DEF_ButtonPress_Voice;
					GoIntoSystemConfigMenu_Init();
				}
				else if ( gui_keycode == KEY_POUNDSIGN )
				{
					DEF_ButtonPress_Voice;
					GoIntoDoorLockSettingMenu_Init();
				}

				break;
	
			case SCREEN_UnlockingModeSetting:
					
					if ( gui_keycode == KEY_ONE )
					{
						DEF_ButtonPress_Voice;
						UserIdentifyResultMgr.UnlockingMode = SingalMode;
						VoiceReportCurrentUnlockMode();
					}
					else if ( gui_keycode == KEY_TWO )
					{
						DEF_ButtonPress_Voice;
						if	( IfSystemWithoutSecondIdentity() == bTRUE )
						{
							UserIdentifyResultMgr.UnlockingMode = SingalMode;
							SystemConfigSave();
							PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_OperationFail);
							GoIntoUnlockingModeSetting_Init();
						}
						else
						{
							UserIdentifyResultMgr.UnlockingMode = DoubleMode;
							VoiceReportCurrentUnlockMode();
						}
					}
					else if (gui_keycode == KEY_ASTERISK)
					{
						DEF_ButtonPress_Voice;
						UnlockModeAutoCalculate();
						SystemConfigSave();
						GoIntoDoorLockSettingMenu_Init();
					}
					else if ( gui_keycode == KEY_POUNDSIGN )
					{
						DEF_ButtonPress_Voice;
						GoIntoUnlockingModeSetting_Init();
					}

				break;

						
			case SCREEN_PickAlarmEnableSetting:
			
					if ( gui_keycode == KEY_ONE )
					{
						DEF_ButtonPress_Voice;
						PickAlarmEnableMgr.Enable = bTRUE;
						AntiPryingMgr.AntiPryingTrigger = bFALSE;		//Refresh Anti-Prying Trigger
						VoiceReportCurrentPickAlarmEnableSetting();
					}
					else if ( gui_keycode == KEY_TWO )
					{
						DEF_ButtonPress_Voice;
						PickAlarmEnableMgr.Enable = bFALSE;
						AntiPryingMgr.AntiPryingTrigger = bFALSE;		//Refresh Anti-Prying Trigger
						VoiceReportCurrentPickAlarmEnableSetting();
					}
					else if (gui_keycode == KEY_ASTERISK)
					{
						DEF_ButtonPress_Voice;
						SystemConfigSave();
						Config_AntiPrying_Interrupt();
						GoIntoDoorLockSettingMenu_Init();	
					}
					else if ( gui_keycode == KEY_POUNDSIGN )
					{
						DEF_ButtonPress_Voice;
						GoIntoPickAlarmEnableSetting_Init();
					}
						
					break;
					
			case SCREEN_BodyInductionSetting:

				if ( gui_keycode == KEY_ONE )
				{
					DEF_ButtonPress_Voice;
					BodyInductionMgr.SensingDistanceLevel = SensingDistanceL0;
					VoiceReportCurrentBodyInduction();
				}
				else if ( gui_keycode == KEY_TWO )
				{
					DEF_ButtonPress_Voice;
					BodyInductionMgr.SensingDistanceLevel = SensingDistanceL1;
					VoiceReportCurrentBodyInduction();
				}

				else if ( gui_keycode == KEY_THREE )
				{
					DEF_ButtonPress_Voice;
					BodyInductionMgr.SensingDistanceLevel = SensingDistanceL2;
					VoiceReportCurrentBodyInduction();
				}
				else if ( gui_keycode == KEY_ASTERISK )
				{
					DEF_ButtonPress_Voice;
					SystemConfigSave();	
					if ( BodyInductionMgr.BodyInductionDeviceType == RadarModule )
					{
						Radar_Init();
					}
					
					if ( IfSystemIsInFactoryDefaultStatus() == bTRUE )
					{
						GoIntoEngineeringModeMenu_Init();
					}
					else
					{
						GoIntoDoorLockSettingMenu_Init();
					}
				}
				else if ( gui_keycode == KEY_POUNDSIGN )
				{
					DEF_ButtonPress_Voice;
					GoIntoBodyInductionSetting_Init();
				}
				break;

			case SCREEN_SystemVersion:

					if (gui_keycode == KEY_POUNDSIGN )
					{
						DEF_ButtonPress_Voice;
						GoIntoShowSystemVersion();
					}
					else if ( gui_keycode == KEY_ASTERISK )
					{
						DEF_ButtonPress_Voice;
						if (IfSystemIsInFactoryDefaultStatus()==bTRUE)
						{
							GoIntoMainScreen_WithIdentifyInit();
						}
						else
						{
							GoIntoInfoInquiryMenu_Init();
						}
					}
					break;

/*
			case SCREEN_SelfTest:

					if ( SelfTestMgr.Status == KEYandLED_TEST )
					{
						if (gui_keycode!=KEY_NONE){
							SelfTestMgr.ButtonStatus |= 1<<gui_keycode;
							SET_KEYLED_OFF(gui_keycode);
							DEF_ButtonPress_Voice;
						}
						
					}
					else if ( SelfTestMgr.Status == FPM_TEST )
					{
						if ( gui_keycode == KEY_POUNDSIGN )
						{
							SelfTestMgr.Status = MOTOR_TEST;
							SelfTestMgr.TestResult = Unknow;
						}
					}
					else if ( SelfTestMgr.Status == MOTOR_TEST )
					{
						if ( gui_keycode == KEY_POUNDSIGN )
						{
							SelfTestMgr.Status = EMAGNET_TEST;
							SelfTestMgr.TestResult = Unknow;
						}
					}
					else if ( SelfTestMgr.Status == EMAGNET_TEST )
					{
						if ( gui_keycode == KEY_POUNDSIGN )
						{
							SelfTestMgr.Status = EEPROM_TEST;
							SelfTestMgr.TestResult = Unknow;
						}
					}
					else if ( SelfTestMgr.Status == EEPROM_TEST )
					{
						if ( gui_keycode == KEY_POUNDSIGN )
						{
							SelfTestMgr.Status = FLASH_TEST;
							SelfTestMgr.TestResult = Unknow;
						}
					}
					
					else if ( SelfTestMgr.Status == FLASH_TEST )
					{
						if ( gui_keycode == KEY_POUNDSIGN )
						{
							SelfTestMgr.Status = RFID_TEST;
							SelfTestMgr.TestResult = Unknow;
						}
					}
					else if ( SelfTestMgr.Status == RFID_TEST )
					{
						if (( gui_keycode == KEY_POUNDSIGN )&&(SelfTestMgr.TestResult == Passed))
						{
							SelfTestMgr.Status = VOLTAGE_TEST;
							SelfTestMgr.TestResult = Unknow;
						}
					}
					else if ( SelfTestMgr.Status == VOLTAGE_TEST )
					{
						if ( gui_keycode == KEY_POUNDSIGN )
						{
							SelfTestMgr.Status = LOWPOWER_TEST;
							SelfTestMgr.TestResult = Unknow;
						}
					}					
				break;
*/
			case SCREEN_CheckEventLogBySequence:

				if (CheckEventLogBySequenceMgr.Status == ShowEventLogWithSequence)
				{
					if ((gui_keycode == KEY_TWO )||(gui_keycode == KEY_TWO_HOLD))
					{
						if ( gui_keycode == KEY_TWO ){
							DEF_ButtonPress_Voice;
						}
						if  ( CheckEventLogBySequenceMgr.OffsetEventLogNum > 0 )
						{
							CheckEventLogBySequenceMgr.OffsetEventLogNum--;

							if ( LogMgr.DisplayPoint < (DEF_MaxRecordedLogID-1)){
								LogMgr.DisplayPoint++;
							}
							else{
								LogMgr.DisplayPoint = 0;
							}
						}
					}
					else if (( gui_keycode == KEY_EIGHT )||(gui_keycode == KEY_EIGHT_HOLD))
					{
						if ( gui_keycode == KEY_EIGHT ){
							DEF_ButtonPress_Voice;
						}
						if ( (CheckEventLogBySequenceMgr.OffsetEventLogNum+1) < CheckEventLogBySequenceMgr.MatchedEventLogNum )
						{
							if ( LogMgr.DisplayPoint > 0 )
							{
								LogMgr.DisplayPoint--;
							}
							else if ( LogMgr.DisplayLog.LogIDmajor > 0 )
							{
								LogMgr.DisplayPoint = DEF_MaxRecordedLogID-1;
							}
							CheckEventLogBySequenceMgr.OffsetEventLogNum++;
						}
					}
					else if (gui_keycode == KEY_ASTERISK)
					{
						DEF_ButtonPress_Voice;	
						GoIntoLogMenu_Init();
					}
					else if ( gui_keycode == KEY_POUNDSIGN )
					{
						DEF_ButtonPress_Voice;	
						VoiceReportLogMgr.Status = ReportLogInit;
					}
				}

				break;

			case SCREEN_CheckEventLogByDate:

				if (CheckEventLogByDateMgr.Status == SetYear)
				{
					if ( gui_keycode == KEY_TWO )
					{
						DEF_ButtonPress_Voice;
						if ( CheckEventLogByDateMgr.year < 0x99 )
						{
							CheckEventLogByDateMgr.year = BcdCode_Plus(CheckEventLogByDateMgr.year);
						}
					}
					else if ( gui_keycode == KEY_EIGHT )
					{
						DEF_ButtonPress_Voice;
						if ( CheckEventLogByDateMgr.year > 0x00  )
						{
							CheckEventLogByDateMgr.year = BcdCode_Minus(CheckEventLogByDateMgr.year);
						}
					}
					#ifdef Function_TouchKeyIsTwoLine
					else if ( gui_keycode == KEY_THREE )
					{
						DEF_ButtonPress_Voice;
						if ( CheckEventLogByDateMgr.Status == SetYear ){
							CheckEventLogByDateMgr.Status = SetMonth;
						}
						else if ( CheckEventLogByDateMgr.Status == SetMonth ){
							CheckEventLogByDateMgr.Status = SetDate;
						}
						else if ( CheckEventLogByDateMgr.Status == SetDate ){
							CheckEventLogByDateMgr.Status = SetYear;
						}
					}
					#else
					else if ( gui_keycode == KEY_SIX )
					{
						DEF_ButtonPress_Voice;
						CheckEventLogByDateMgr.Status = SetMonth;
					}
					#endif
					else if (gui_keycode == KEY_POUNDSIGN )
					{
						DEF_ButtonPress_Voice;
						CheckEventLogByDateMgr.Status = SearchEventLogWithDate;
					}
					else if (gui_keycode == KEY_ASTERISK)
					{
						DEF_ButtonPress_Voice;	
						GoIntoLogMenu_Init();
					}
					
				}
				else if (CheckEventLogByDateMgr.Status == SetMonth)
				{
					if ( gui_keycode == KEY_TWO )
					{
						DEF_ButtonPress_Voice;
						if ( CheckEventLogByDateMgr.month < 0x12 )
						{
							CheckEventLogByDateMgr.month=BcdCode_Plus(CheckEventLogByDateMgr.month);
						}
					}
					else if ( gui_keycode == KEY_EIGHT )
					{
						DEF_ButtonPress_Voice;
						if ( CheckEventLogByDateMgr.month > 0x01  )
						{
							CheckEventLogByDateMgr.month=BcdCode_Minus(CheckEventLogByDateMgr.month);
						}
					}
					#ifdef Function_TouchKeyIsTwoLine
					else if ( gui_keycode == KEY_THREE )
					{
						DEF_ButtonPress_Voice;
						if ( CheckEventLogByDateMgr.Status == SetYear ){
							CheckEventLogByDateMgr.Status = SetMonth;
						}
						else if ( CheckEventLogByDateMgr.Status == SetMonth ){
							CheckEventLogByDateMgr.Status = SetDate;
						}
						else if ( CheckEventLogByDateMgr.Status == SetDate ){
							CheckEventLogByDateMgr.Status = SetYear;
						}
					}
					#else
					else if ( gui_keycode == KEY_FOUR )
					{
						DEF_ButtonPress_Voice;
						CheckEventLogByDateMgr.Status = SetYear;
					}
					else if ( gui_keycode == KEY_SIX )
					{
						DEF_ButtonPress_Voice;
						CheckEventLogByDateMgr.Status = SetDate;
					}
					#endif
					
					else if (gui_keycode == KEY_POUNDSIGN )
					{
						DEF_ButtonPress_Voice;
						CheckEventLogByDateMgr.Status = SearchEventLogWithDate;
					}
					else if (gui_keycode == KEY_ASTERISK)
					{
						DEF_ButtonPress_Voice;	
						GoIntoLogMenu_Init();
					}
				}
				else if (CheckEventLogByDateMgr.Status == SetDate)
				{
					if ( gui_keycode == KEY_TWO )
					{
						DEF_ButtonPress_Voice;
						if ( CheckEventLogByDateMgr.date < BcdMaxDays_in_Month(CheckEventLogByDateMgr.year,CheckEventLogByDateMgr.month) )
						{
							CheckEventLogByDateMgr.date = BcdCode_Plus(CheckEventLogByDateMgr.date);
						}
					}
					else if ( gui_keycode == KEY_EIGHT )
					{
						DEF_ButtonPress_Voice;
						if ( CheckEventLogByDateMgr.date > 0x01  )
						{
							CheckEventLogByDateMgr.date = BcdCode_Minus(CheckEventLogByDateMgr.date);
						}
					}
					#ifdef Function_TouchKeyIsTwoLine
					else if ( gui_keycode == KEY_THREE )
					{
						DEF_ButtonPress_Voice;
						if ( CheckEventLogByDateMgr.Status == SetYear ){
							CheckEventLogByDateMgr.Status = SetMonth;
						}
						else if ( CheckEventLogByDateMgr.Status == SetMonth ){
							CheckEventLogByDateMgr.Status = SetDate;
						}
						else if ( CheckEventLogByDateMgr.Status == SetDate ){
							CheckEventLogByDateMgr.Status = SetYear;
						}
					}
					#else
					else if ( gui_keycode == KEY_FOUR )
					{
						DEF_ButtonPress_Voice;
						CheckEventLogByDateMgr.Status = SetMonth;
					}
					#endif
					else if (gui_keycode == KEY_POUNDSIGN )
					{
						DEF_ButtonPress_Voice;
						CheckEventLogByDateMgr.Status = SearchEventLogWithDate;
					}
					else if (gui_keycode == KEY_ASTERISK)
					{
						DEF_ButtonPress_Voice;	
						GoIntoLogMenu_Init();
					}
				}
				else if (CheckEventLogByDateMgr.Status == ShowEventLogWithDate)
				{
					if (( gui_keycode == KEY_TWO )||( gui_keycode == KEY_TWO_HOLD ))
					{
						if (gui_keycode == KEY_TWO){
							DEF_ButtonPress_Voice;
						}
						if  ( CheckEventLogByDateMgr.OffsetEventLogNum > 0 )
						{
							CheckEventLogByDateMgr.OffsetEventLogNum--;
							if ( LogMgr.DisplayPoint < (DEF_MaxRecordedLogID-1)){
								LogMgr.DisplayPoint++;
							}
							else{
								LogMgr.DisplayPoint = 0;
							}
						}
					}
					else if (( gui_keycode == KEY_EIGHT )||(gui_keycode == KEY_EIGHT_HOLD))
					{
						if ( gui_keycode == KEY_EIGHT ){
							DEF_ButtonPress_Voice;
						}
						if ( (CheckEventLogByDateMgr.OffsetEventLogNum+1) < CheckEventLogByDateMgr.MatchedEventLogNum )
						{
							if ( LogMgr.DisplayPoint > 0 )
							{
								LogMgr.DisplayPoint--;
							}
							else if ( LogMgr.DisplayLog.LogIDmajor > 0 )
							{
								LogMgr.DisplayPoint = DEF_MaxRecordedLogID-1;
							}
							CheckEventLogByDateMgr.OffsetEventLogNum++;
						}
					}
					else if (( gui_keycode == KEY_POUNDSIGN )||(gui_keycode == KEY_ASTERISK))
					{
						DEF_ButtonPress_Voice;	
						GoIntoLogMenu_Init();
					}
				}
			break;

			case SCREEN_DeleteEventLog:
			
			if ( LogDeleteMgr.Status == WaitforLogDeleteCofirm )
				{
					if ( gui_keycode == KEY_POUNDSIGN )
					{
						DEF_ButtonPress_Voice;
						LogDeleteMgr.Status = LogDeleting;
					}
					else if ( gui_keycode == KEY_ASTERISK )
					{
						DEF_ButtonPress_Voice;
						//LogDeleteMgr.Status = LogDeleteIdle;
						GoIntoLogMenu_Init();
					}
				}

			break;

		case SCREEN_PickLockAlarm:
	
			if  ( PasscodeUserIdentifyMgr.Status == PasscodeIdentifyIdle )
			{
				if ( gui_keycode < 10 )		//from KEY0~KEY9
				{
					if ( PasscodeUserIdentifyMgr.Status == PasscodeIdentifyIdle )
					{
						
						PasscodeInputMgr.Point = 0x01;
						PasscodeInputMgr.PasscodeLen = 16;
						PasscodeInputMgr.Status = PasscodeInputStart;
						PasscodeInputMgr.InputBuff[0] = gui_keycode;
						for (i=1;i<PasscodeInputMgr.PasscodeLen;i++)
						{
							PasscodeInputMgr.InputBuff[i]=0xff;
						}
						
						PasscodeUserIdentifyMgr.Status = PasscodeIdentifyStart;
						//DEF_ButtonPress_Voice;
					}
				 }
				else if ( gui_keycode == KEY_ASTERISK )
				{
				}
			}
			else if ( gui_keycode != KEY_NONE )		//from KEY0~KEY9
			{
				if ( PasscodeUserIdentifyMgr.Status == PasscodeIdentifyPasscodeInput )
				{
					//DEF_ButtonPress_Voice;
					PasscodeUserIdentifyMgr.TimeCnt = 240;	//
					GUI_PasscodeInputButtonMonitor(gui_keycode);
				}
			}
			
			break;

		case SCREEN_NetWorkLink:

			if (gui_keycode == KEY_ASTERISK)
			{
				DEF_ButtonPress_Voice;
				ComPort_SetPost_ExitWifiHotSpot();
				GoIntoMainMenu_Init();
			}

			break;

		case SCREEN_RemoteUnlockRequest:
			
			if ( WifiMgr.RemoteUnlockMgr.Status == RemoteUnlockRequestWait )
			{
				if (gui_keycode == KEY_ASTERISK)
				{
					DEF_ButtonPress_Voice;
					WifiMgr.RemoteUnlockMgr.Status = RemoteUnlockExit;
					#if defined (Function_YouzhiyunjiaWifi) || defined (Function_TuyaWifi) 
						Wifi_PositiveExitRemoteUnlock();
					#endif
				}
				else if ( gui_keycode == KEY_DOORBELL )
				{
					if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
					{
						PLAY_VOICE_DOORBELL();	//DOORBELL	
						#ifndef Function_MainBoardWithoutVoicePlayer
						ComPort_SetPost_Info(DEF_WifiInfo_DoorBell,0x00,0x00);
						#endif
					}
				}
				else if ( gui_keycode == KEY_POUNDSIGN )
				{
					#ifndef Function_IndependentDoorBellKey
					if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
					{
						PLAY_VOICE_DOORBELL();	//DOORBELL
						#ifndef Function_MainBoardWithoutVoicePlayer
						ComPort_SetPost_Info(DEF_WifiInfo_DoorBell,0x00,0x00);
						#endif
					}
					#endif	
				}				
			}
			break;

		case SCREEN_SelfTest:

			if ( SelfTestMgr.Status == VoiceAndTouchAndLEDs_TEST )
			{
				if ( gui_keycode == KEY_ZERO )
				{
					DEF_ButtonPress_Voice;
					SelfTestMgr.TouchKeySelfTestStatus.KEY_ZERO_OK = bTRUE;
					SET_KEYLED_OFF(gui_keycode);
				}
				else if ( gui_keycode == KEY_ONE )
				{
					DEF_ButtonPress_Voice;
					SelfTestMgr.TouchKeySelfTestStatus.KEY_ONE_OK = bTRUE;
					SET_KEYLED_OFF(gui_keycode);
				}
				else if ( gui_keycode == KEY_TWO )
				{
					DEF_ButtonPress_Voice;
					SelfTestMgr.TouchKeySelfTestStatus.KEY_TWO_OK = bTRUE;
					SET_KEYLED_OFF(gui_keycode);
				}
				else if ( gui_keycode == KEY_THREE )
				{
					DEF_ButtonPress_Voice;
					SelfTestMgr.TouchKeySelfTestStatus.KEY_THREE_OK = bTRUE;
					SET_KEYLED_OFF(gui_keycode);
				}
				else if ( gui_keycode == KEY_FOUR )
				{
					DEF_ButtonPress_Voice;
					SelfTestMgr.TouchKeySelfTestStatus.KEY_FOUR_OK = bTRUE;
					SET_KEYLED_OFF(gui_keycode);
				}
				else if ( gui_keycode == KEY_FIVE )
				{
					DEF_ButtonPress_Voice;
					SelfTestMgr.TouchKeySelfTestStatus.KEY_FIVE_OK = bTRUE;
					SET_KEYLED_OFF(gui_keycode);
				}
				else if ( gui_keycode == KEY_SIX )
				{
					DEF_ButtonPress_Voice;
					SelfTestMgr.TouchKeySelfTestStatus.KEY_SIX_OK = bTRUE;
					SET_KEYLED_OFF(gui_keycode);
				}
				else if ( gui_keycode == KEY_SEVEN )
				{
					DEF_ButtonPress_Voice;
					SelfTestMgr.TouchKeySelfTestStatus.KEY_SEVEN_OK = bTRUE;
					SET_KEYLED_OFF(gui_keycode);
				}
				else if ( gui_keycode == KEY_EIGHT )
				{
					DEF_ButtonPress_Voice;
					SelfTestMgr.TouchKeySelfTestStatus.KEY_EIGHT_OK = bTRUE;
					SET_KEYLED_OFF(gui_keycode);
				}
				else if ( gui_keycode == KEY_NINE )
				{
					DEF_ButtonPress_Voice;
					SelfTestMgr.TouchKeySelfTestStatus.KEY_NINE_OK = bTRUE;
					SET_KEYLED_OFF(gui_keycode);
				}
				else if ( gui_keycode == KEY_ASTERISK )
				{
					DEF_ButtonPress_Voice;
					SelfTestMgr.TouchKeySelfTestStatus.KEY_ASTERISK_OK = bTRUE;
					SET_KEYLED_OFF(gui_keycode);
				}
				else if ( gui_keycode == KEY_POUNDSIGN )
				{
					DEF_ButtonPress_Voice;
					SelfTestMgr.TouchKeySelfTestStatus.KEY_POUNDSIGN_OK = bTRUE;
					SET_KEYLED_OFF(gui_keycode);
				}
				else if ( gui_keycode == KEY_DOORCLOSE )
				{
					DEF_ButtonPress_Voice;
					SelfTestMgr.TouchKeySelfTestStatus.KEY_DOORCLOSE_OK = bTRUE;
					SET_KEYLED_OFF(gui_keycode);
				}
				else if ( gui_keycode == KEY_DOORBELL )
				{
					DEF_ButtonPress_Voice;
					SelfTestMgr.TouchKeySelfTestStatus.KEY_DOORBELL_OK = bTRUE;
					SET_KEYLED_OFF(gui_keycode);
				}
			}
			/*
			else if ( SelfTestMgr.Status == Display_TEST )
			{
				if ( gui_keycode == KEY_POUNDSIGN )
				{
					DEF_ButtonPress_Voice;
					Clear_Screen();
				}
				else if ( gui_keycode == KEY_ASTERISK )
				{
					PLAY_VOICE_ONESEGMENT(3,VOICE_PleasePutFinger);
					SelfTestMgr.Status = FPM_TEST;
				}
			}
			*/
			else if ( SelfTestMgr.Status == VOLTAGE_TEST )
			{
				if ( gui_keycode == KEY_POUNDSIGN )
				{
					DEF_ButtonPress_Voice;
					SelfTestMgr.Status = LOWPOWER_TEST;
				}
			}

			break;
				
		default:

			break;
			
	}
	
	


}

/*******************************************************/
/*******************************************************/
void GUI_Update_Screen(void)
{	
	if (( CurrentScreen !=LastScreen )||(GUI_Flag_RefreshLCD == bTRUE))
	{
		Clear_Screen();
		LastScreen =  CurrentScreen;
		GUI_Flag_RefreshLCD = bFALSE;
	}

	switch ( CurrentScreen)
	{
		case SCREEN_Initialization:

			SET_ALLKEYLED_OFF();
			#ifdef Function_FaceRecoginitionSwitchedAutoDetection
			ShowFrmConnectionCheck();
			#endif
			ShowInitialization();
	
		break;
		
		case SCREEN_LowBattery:

			SET_ALLKEYLED_OFF();
			ShowLowBattery();
			
		break;

		case SCREEN_PickLockAlarm:

			SET_ALLKEYLED_ON();
			ShowPickLockAlarm();
			
		break;

		case SCREEN_Main:
				
			GUI_UpadteMain();

		break;

		case SCREEN_IdentifySuccess:
			
			if ( BatteryMgr.BatteryType == LiBattery )
			{
				KEYLED_WATERLIGHT_Task();
			}
			else
			{
				SET_ALLKEYLED_OFF();
			}
			ShowIdentifySuccessPage();
		
			break;
			
		case SCREEN_IdentifyFail:
				
			SET_ALLKEYLED_OFF();
			
			ShowIdentifyFailPage();
		
			break;

		case SCREEN_MainMenu:

			SET_1234andAPled_ON();
			
			ShowMainMenu();

			break;

		case SCREEN_UserManagementMenu:

			if ( FrmMgr.FrmFunctionConfig == FRM_Enabled )
				{
			SET_123andAPled_ON();
				}
			else{
			SET_123andAPled_ON();
				}
			
			ShowUserManagementMenu();

			break;
		
		case SCREEN_ManagerMenu:
			
			SET_12andAPled_ON();
		
			ShowManagerMenu();
			break;

		case SCREEN_UserMenu:
			
			SET_12andAPled_ON();

			ShowNormalMenu();
			break;

		case SCREEN_DeleteUserMenu:

			SET_12andAPled_ON();
			
			ShowDeleteUserMenu();
			break;
		
		case SCREEN_RegisterManagerFPC:
			
//			SET_ALLKEYLED_ON();
//			ShowRegisterManagerFPCPage();
			CurrentScreen = SCREEN_ResManagerFPCInput;
			break;
		
		case SCREEN_ResManagerFPCInput:
		
			SET_ALLKEYLED_ON();
			ShowRegisterManagerFPCPage();
			break;
		
		case SCREEN_ResManagerIdFPC:
			
			SET_ALLKEYLED_ON();
			ShowResManagerIDFPC();
			break;
		
		case SCREEN_ResManagerIdFace:
			
			SET_ALLKEYLED_ON();
			ShowResManagerIDFace();
			break;
		
		case SCREEN_ResNormalUserIdFPC:
			
			SET_ALLKEYLED_ON();
			ShowResNormalIDFPC();
			break;
		
		case SCREEN_ResNormalUserIdFace:
			
			SET_ALLKEYLED_ON();
			ShowResNormalIDFace();
			break;
		
		case SCREEN_DelUserId:
		
			SET_ALLKEYLED_ON();
			ShowDelUserId();
			break;
		
		case SCREEN_RegisterManagerFace:
			
//			SET_ALLKEYLED_ON();
//			ShowRegisterManagerFacePage();
			CurrentScreen = SCREEN_ResManagerFaceInput;
			break; 
		
		case SCREEN_ResManagerFaceInput:
			
			SET_ALLKEYLED_ON();
			ShowRegisterManagerFacePage();
			break;
		
		case SCREEN_RegisterNormalUserFPC:
			
//			SET_ALLKEYLED_ON();
//			ShowRegisterUserFPCPage();
			CurrentScreen = SCREEN_ResNormalUserFPCInput;
			break;
			
		case SCREEN_ResNormalUserFPCInput:
			
			SET_ALLKEYLED_ON();
			ShowRegisterUserFPCPage();
			break;
		
		case SCREEN_RegisterNormalUserFace:
			
//			SET_ALLKEYLED_ON();
//			ShowRegisterUserFacePage();
			CurrentScreen = SCREEN_ResNormalUserFaceInput;
			break;
		
		case SCREEN_ResNormalUserFaceInput:
			
			SET_ALLKEYLED_ON();
			ShowRegisterUserFacePage();
			break;
		
		case SCREEN_NumberDelete:
			
//			SET_ALLKEYLED_ON();
//			ShowDeleteUserPage();
			CurrentScreen = SCREEN_DelUserIdInput;
			break;
		
		case SCREEN_DelUserIdInput:
			
			SET_ALLKEYLED_ON();
			ShowDeleteUserPage();
			break;
		
		case SCREEN_AllDelete:
			
			SET_APled_ON();
			ShowAllDeleteUserPage();
			break;
		
		case SCREEN_OperateSuccess:
			
			SET_APled_ON();
			AllDeleteCnt--;
			ShowOperateSuccessPage();
			break;
		
		case SCREEN_AllDeleteSuccess:
			
			DeleteUserMenu_Init();
			break;
		
		case SCREEN_FaceMenu:
			
			SET_123andAPled_ON();
			ShowFaceMenu();
			break;
			
		case SCREEN_FpMenu:
			
			SET_1234andAPled_ON();
			ShowFpMenu();
			break;

		case SCREEN_FpDeleteMenu:
			
			SET_123andAPled_ON();
			ShowFpDeleteMenu();
			break;

		case SCREEN_CardUserMenu:

			SET_123andAPled_ON();
			ShowCardMenu();
			break;

		case SCREEN_PasscodeMenu:
			
			SET_1234andAPled_ON();
			ShowPasscodeMenu();
			break;
			
		case SCREEN_SystemConfigMenu:

			SET_1234andAPled_ON();
			ShowSystemConfigMenu();
			break;

		case SCREEN_VoiceSettingMenu:

			SET_12andAPled_ON();
			ShowVoiceSettingMenu();
			break;

		case SCREEN_InfoInquiryMenu:

			SET_1234andAPled_ON();
			ShowInfoInquiryMenu();
			break;

		case SCREEN_NetWorkLink:
			ShowNetWorkConnecting();
			break;

		case SCREEN_RegisterMasterFace:
			if ( FaceUserRegisterMgr.Status == ReportFaceUserID )
			{
				SET_APled_ON();
			}
			else
			{
				SET_AsteriskLED_ON();
			}
			ShowRegisterUserFace();
			break;

		//case SCREEN_DeleteMasterFace:
		//	SET_ALLKEYLED_ON();
		//	ShowDeleteUserFace();
		//	break;
			
		case SCREEN_RegisterUserFace:
			if ( FaceUserRegisterMgr.Status == ReportFaceUserID )
			{
				SET_APled_ON();
			}
			else
			{
				SET_AsteriskLED_ON();
			}
			ShowRegisterUserFace();

			break;

		case SCREEN_DeleteUserFace:
			SET_ALLKEYLED_ON();
			ShowDeleteUserFace();

			break;

	//	case SCREEN_DeleteAllUserFace:
	//		SET_UDandConfirmLED_ON();
	//		ShowDeleteAllUserFace();
	//		break;

		case SCREEN_RegisterUserFp:
			if ( FpUserRegisterMgr.Status == ReportFpUserID )
			{
				SET_APled_ON();
			}
			else
			{
				SET_AsteriskLED_ON();
			}
			ShowRegisterUserFp();

			break;
			
		case SCREEN_DeleteUserFp:
			SET_ALLKEYLED_ON();
			ShowDeleteUserFp();

			break;

		case SCREEN_DeleteAllUserFp:
			SET_APled_ON();
			ShowDeleteAllUserFp();

			break;

		case SCREEN_RegisterStressUserFp:
			if ( FpUserRegisterMgr.Status == ReportFpUserID )
			{
				SET_APled_ON();
			}
			else
			{
				SET_AsteriskLED_ON();
			}
			ShowRegisterUserFp();

			break;
			
		case SCREEN_DeleteStressUserFp:
			SET_ALLKEYLED_ON();
			ShowDeleteUserFp();

			break;

		case SCREEN_DeleteAllStressUserFp:
			SET_APled_ON();
			ShowDeleteAllUserFp();

			break;
		
		case SCREEN_RegisterMasterFp:
			if ( FpUserRegisterMgr.Status == ReportFpUserID )
			{
				SET_APled_ON();
			}
			else
			{
				SET_AsteriskLED_ON();
			}
			ShowRegisterUserFp();
			break;

		case SCREEN_RegisterCardUser:

			if ( CardUserRegisterMgr.Status == ReportCardUserID )
			{
				SET_APled_ON();
			}
			else
			{
				SET_AsteriskLED_ON();
			}
			ShowRegisterCardUser();

			break;
			
		case SCREEN_DeleteCardUser:
			SET_ALLKEYLED_ON();
			ShowDeleteCardUser();

			break;

		case SCREEN_DeleteAllCardUser:
			SET_APled_ON();
			ShowDeleteAllCardUser();

			break;

		case SCREEN_RegisterPasscodeUser:
			if ( PasscodeUserRegisterMgr.Status == ReportPasscodeUserID )
			{
				SET_APled_ON();
			}
			else if (( PasscodeUserRegisterMgr.Status == InputFirstPasscode )
				||(PasscodeUserRegisterMgr.Status == InputSecondPasscode))
			{
				SET_ALLKEYLED_ON();
			}
			else
			{
				SET_ALLKEYLED_OFF();
			}
			ShowRegisterPasscodeUser();

			break;
			
		case SCREEN_DeletePasscodeUser:
			SET_ALLKEYLED_ON();
			ShowDeletePasscodeUser();

			break;

		case SCREEN_DeleteAllPasscodeUser:
			SET_APled_ON();
			ShowDeleteAllPasscodeUser();

			break;


		case SCREEN_TimeSetting:
			SET_ALLKEYLED_ON();
			ShowTimeSetting();

			break;		
		case SCREEN_RestoreFactoryDefault:
			if ( RestoreFactoryDefaultMgr.Status == WaitForRestoreFactoryDefaultUserConfirm )
			{
				SET_APled_ON();
			}
			else
			{
				SET_ALLKEYLED_OFF();
			}
			
			ShowRestoreFactoryDefault();

			break;

		case SCREEN_LanguageSetting:
			SET_12andAPled_ON();
			ShowLanguageSetting();
		
			break;
			
		case SCREEN_VolumeSetting:
			SET_1234andAPled_ON();
			ShowVolumeSetting();

			break;

		case SCREEN_MemoryUsage:
			SET_APled_ON();
			ShowMemoryUsage();
		
			break;
			
		case SCREEN_ManagerIdentify:

			SET_ALLKEYLED_ON();
			ShowManagerIdentify();
			break;

		case SCREEN_DoorLockSettingMenu:
			if ( FrmMgr.FrmFunctionConfig == FRM_Enabled )
				{
			SET_123andAPled_ON();
				}
			else{
			SET_12andAPled_ON();
				}
			ShowDoorLockSettingMenu();
			break;

		case SCREEN_UnlockingModeSetting:
			SET_12andAPled_ON();
			ShowUnlockingModeSetting();
		
			break;			
		case SCREEN_PickAlarmEnableSetting:	
			SET_12andAPled_ON();
			ShowPickAlarmEnableSetting();
			break;
			
		case SCREEN_BodyInductionSetting:	
			SET_123andAPled_ON();
			ShowBodyInductionSetting();
			break;
			
		case SCREEN_SystemVersion:	
			SET_APled_ON();
			ShowSystemVersion();
			break;

		case SCREEN_SelfTest:	
			ShowSelfTest();		
			break;
	
		case SCREEN_AgingTest:

			KEYLED_WATERLIGHT_Task();
			ShowAgingTest();
			break;

		case SCREEN_SystemLocked:

			SET_ALLKEYLED_OFF();
			ShowSystemLocked();
			break;

		case SCREEN_EventLogMenu:
			
			SET_12andAPled_ON();
			ShowEventLogMenu();
			break;
			
		case SCREEN_CheckEventLogBySequence:

			SET_UDandAPled_ON();
			ShowEventLogBySequence();
			break;
			
		case SCREEN_CheckEventLogByDate:	
			SET_UDandAPled_ON();
			ShowEventLogByDate();
			break;

		case SCREEN_DeleteEventLog:
			SET_APled_ON();
			ShowClearEventLog();
			
			break;	
			
		case SCREEN_RemoteUnlockRequest:
			if ( WifiMgr.RemoteUnlockMgr.Status == RemoteUnlockRequestWait ){
				KEYLED_ASTERISK_Flash();
			}
			else{
				SET_ALLKEYLED_OFF();
			}
			ShowRemoteUnlockRequest();
			
			break;	
		case SCREEN_AutoMotorSettingMenu:
			SET_1234andAPled_ON();
			ShowAutoMotorSettingMenu();
			break;
			
		case SCREEN_EngineeringModeMneu:
			if ( FrmMgr.FrmFunctionConfig == FRM_Enabled )
			{
				SET_1234567andAPled_ON();
			}
			else{
				SET_123456andAPled_ON();
			}
			ShowEngineeringModeMenu();
			break;
	
		case  SCREEN_AutoMotorUnlockTime:
			SET_UDandAPled_ON();
			 ShowAutoMotorUnlockTimeSetting();
			break;
		case  SCREEN_AutoMotorAutoLockTime:
			SET_UDandAPled_ON();
			ShowAutoMotorAutoLockTimeSetting();
			break;
		case  SCREEN_AutoMotorLockDirection:
			SET_12andAPled_ON();
			ShowAutoMotorLockDirectionSetting();
			break;
		case  SCREEN_AutoMotorTorque:
			SET_123andAPled_ON();
			ShowAutoMotorTorqueSetting();
			break;
		case  SCREEN_AutoMotorBoltLockTime:
			SET_UDandAPled_ON();
			ShowBoltLockTimeSetting();
			break;
			
		case  SCREEN_LockingTravel:
			SET_UDandAPled_ON();
			ShowLockingTravelSetting();
			break;
			
		case  SCREEN_AutoEject:
			SET_12andAPled_ON();
			ShowAutoEjectSetting();
			break;
			
		case  SCREEN_AutoMotorSelfTest:
			KEYLED_ASTERISK_Flash();
			ShowAutoMotorSelfTest();
			break;	
		case  SCREEN_WifiMFT:
			KEYLED_ASTERISK_Flash();
			ShowWifiManufactureTest();
			break;
		case  SCREEN_ErrorMessage:
			ShowErrorMessage();
			break;
	
		default:
			break;
	}
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
/*******************************************************/
/*******************************************************/
/*******************************************************/
status_t SaveFPMserialNumberToMemory(void)
{
	if ( Get_FPM_SerialNumber(FPMserialNumberManager.SN) == S_SUCCESS )
	{ 
		EEPROM_WriteSequential(FPMserialNumberStartAddr,FPMserialNumberManager.SN,32);
		return S_SUCCESS;
	}
	else{
		return S_FAIL;
	}
	
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
status_t Get_FPM_SerialNumber(uint8_t buff[])
{
	uint8_t i,j,k;
		
	for (i=0;i<5;i++)
	{
		FpmAckMgr.Status = WaitACK;
		FpmAckMgr.TimeOutCnt = DEF_FpmAckTimeoutTime;
		FPM_SendGetChipSerialNumberCmd();
		for (j=0;j<20;j++)
		{
			CLRWDT();
			Hardware_DelayMs(10);
			FPM_Mgr_Task();
			if ( FpmAckMgr.Status == GotACK )
			{
				if ( FpmAckMgr.ErrorCode == Error_NONE)
				{
					for (k=0;k<32;k++)
					{
						buff[k]=FpmAckMgr.Buff[10+k];
					}
                    DEBUG_MARK;
                    return S_SUCCESS;
				}
				else		//GET ACK with Error,try again
				{
					break; 
				}
			}
		}
	}	
	return S_FAIL;
}


/*******************************************************/
/*******************************************************/
/*******************************************************/
/*******************************************************/
/*******************************************************/
/*******************************************************/
bool_t CheckFPMserialNumberIsMatch(uint8_t buff[])
{
	//uint8_t SavedFPMserialNumber[32];

	EEPROM_ReadSequential(FPMserialNumberStartAddr,FPMserialNumberManager.SN,32);
	if( CompareTwoArrayIsSame(buff,FPMserialNumberManager.SN,32) == bTRUE )	
		{
			FPMserialNumberManager.SNmatched = IsMatched;
			DEBUG_MARK;
			return bTRUE;
		}
		else{
			FPMserialNumberManager.SNmatched = IsNotMatched;
			DEBUG_MARK;
			return bFALSE;
		}
		
}


/******************************************************************************************
* ?1?7?1?7?1?7?1?7?1?7?1?7?1?7?1?7    ?1?7?1?7
* ?1?7?1?7?1?7?1?7?1?7?1?7?1?7?1?7    ?1?7?1?7 
* ?0?0?1?7?0?4?1?7?0?0?1?7?1?7?1?7?1?7?1?7?1?7
* ?1?7?0?2?1?7?1?7?1?7?0?0?1?7?1?7?1?7?1?7?1?7?1?7
*----------------------------------------?1?7?1?0?1?7?1?7?1?7?0?8------------------------------------------
* ?1?7?1?7?0?2?1?7汾    ?1?7?1?7 			 ?1?7?1?0?1?7?1?7?0?7?1?7                  ?1?7?1?0?1?7?1?7?1?7?1?7?1?3?1?7
* ?1?7?1?0?1?7?0?5?1?7?1?7    ?1?7?1?7
******************************************************************************************/
void System_PowerDown(void)
{
  uint8_t i;
	
	static uint16_t FPtimeout_times,FP_SleepFailedTimes;
	
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	CLRWDT();
	DEBUG_MARK;

	ComportMgr.RestoreFactoryDefaultTrig = bFALSE;

	#ifdef Function_VoiceIcPowerTurnOffInSleepMode
	SET_SPEEKERPOWER_OFF;
	Hardware_DelayMs(5);
	GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;//
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	VoiceDataTransferMgr.Status = Sleep;
	#endif
	
	SET_LEDPOWER_OFF;
	SET_ALLKEYLEDPIN_POWERDOWN();
	SET_DEBUG_LED_OFF;
	SET_LED_RGB_OFF();
	SET_LED_RB_OFF();

	PINMACRO_TurnOffVbatToADC;

	SET_FLASH_CS_H;

//	STOP_VOICEPLAY();

	#ifndef Function_MainBoardWithoutVoicePlayer
	STOP_VOICEPLAY();
	#endif

	Hardware_DelayMs(10);
	
	HAL_SPI_MspDeInit(&hspi1);
	GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_7|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;//GPIO_MODE_INPUT;//
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET); 	  //OLED RES
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); 	  //OLED DC
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET); 	  //OLED CS
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); 	  //OLED SCK
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET); 	  //OLED MISO 
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET); 	  //OLED MOSI
	SET_OLEDPOWER_OFF;

	if ( FrmMgr.FrmFunctionConfig == FRM_Enabled )
	{
		if (FrmMgr.PowerStatus != FRM_PowerOff)
		{
			FaceRecognition_PowerDown();
			for (i=0;i<100;i++)
			{
				Hardware_DelayMs(10);
				FaceRecognitionMgr_Task();
				CLRWDT();
				if ( FrmMgr.PostFlag_PownDown == bTRUE )
				{
					DEBUG_MARK;
					break;
				}
			}
		}
		FaceRecognition_HardwarePowerOff();
	}
  
//	HAL_SPI_MspDeInit(&hspi2);
//	GPIO_InitStruct.Pin = GPIO_PIN_0;
//	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;//GPIO_MODE_INPUT;//
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//
//	GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
//	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;//GPIO_MODE_INPUT;//
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
//
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET); 	  //SPI2 CLK
//	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET); 	  //SPI2 MISO
//	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET); 	  //SPI2 MOSI
	MFC_POWERDOWN();

	
//	MX_I2C1_DeInit();	

	while (ADC_Disable(&hadc1)!= HAL_OK);
	while (HAL_ADCEx_DisableVoltageRegulator(&hadc1)!= HAL_OK);
	while (HAL_ADC_DeInit(&hadc1)!= HAL_OK);

	GUI_SetFPM_LED(DEF_FpmLedMode_Off,DEF_FpmLedColor_All,DEF_FpmLedColor_All,255);

	

	//wait for user release touch button
	if ( SystemPowerMgr.SleepSource == UserForced )
	{
		for (i=0;i<10;i++)
		{
			CLRWDT();
			Hardware_DelayMs(100);
		}
	}
	
	//ADC1_CR1 &= CLRBIT0;	//ADC OFF

	TOUCH_POWERDOWN();

	CLRWDT();

	Hardware_AwakeLine_SleepStatus();
	HAL_UART_DeInit(&huart2);

	#if defined (Function_YouzhiyunjiaWifi) || defined (Function_TuyaWifi)
	HAL_UART_DeInit(&huart3);
	SET_WIFIPOWER_OFF;
	WifiMgr.Power.Status = PowerOff;
	#endif

	CLRWDT();

	FPtimeout_times=0;
	FP_SleepFailedTimes=0;
	while(1)
	{
		FpmAckMgr.Status = WaitACK;
		FpmAckMgr.TimeOutCnt = DEF_FpmAckTimeoutTime;
		FPM_SendSleepCmd();
		for (i=0;i<25;i++)
		{
			CLRWDT();
			Hardware_DelayMs(20);
			FPM_Mgr_Task();
			if ( FpmAckMgr.Status == GotACK )
			{
				if (  ( FpmAckMgr.ErrorCode == Error_NONE)
					&&(PINMACRO_FPM_TOUCH_STATUS == 0x00 )
					)
				{
					FPMpowerMgr.Status = FPMpowerDown;	
					DEBUG_MARK;
					break;
				}
				else
				{
					FP_SleepFailedTimes++;
					DEBUG_MARK;
					
					if (FP_SleepFailedTimes>350)
					{
						FPMpowerMgr.Status = FPMpowerDown;	
					}
					break;
				}
				
			}
			else
			{
				if (i>23)		//time out,FP failed
				{
					FPtimeout_times++;
					if (FPtimeout_times>2)
					{
						FPMpowerMgr.Status = FPMpowerDown;	
						break;
					}
				}
			}
		}
		CLRWDT();
		if ( FPMpowerMgr.Status == FPMpowerDown)
		{
			DEBUG_MARK;
			HAL_UART_DeInit(&huart1);
			SET_FPMPOWER_OFF;	
			Hardware_DelayMs(1);//Wait for Power line lost power
			break;
		}
	}


	while (1)
	{
		CLRWDT();
		
		MX_TIM3_DeInit();
		MX_TIM6_DeInit();
		
		HAL_TIM_Base_Stop_IT(&htim3);
		HAL_TIM_Base_Stop_IT(&htim6);

		HAL_SuspendTick();
		{__ASM ("nop");__ASM ("nop");}
		
		SystemPowerMgr.Status = SLEEP;
		SystemPowerMgr.AwakeSource = DoNotCare;

		if ( FrmMgr.FrmFunctionConfig == FRM_Enabled )
		{
			if ( BodyInductionMgr.BodyInductionDeviceType == PirModule )
			{
				if ( BodyInductionMgr.SensingDistanceLevel != SensingDistanceL0 )
				{
					RTC_EnableWakeUpTimer();	
					DEBUG_MARK;
				}
				else
				{
					RTC_DisableWakeUpTimer();
					DEBUG_MARK;
				}
			}
			else if ( BodyInductionMgr.BodyInductionDeviceType == RadarModule )
			{
				if ( BodyInductionMgr.SensingDistanceLevel != SensingDistanceL0 )
				{
					if ( BodyInductionMgr.BodyInductionDelayTimeCnt > 0 )
					{
						RTC_EnableWakeUpTimer();
						RadarAwake_TurnOff();
					}
					else
					{
						RTC_DisableWakeUpTimer();
						RadarAwake_TurnOn();
					}	
				}
				else
				{
					RTC_DisableWakeUpTimer();
					RadarAwake_TurnOff();
				}
			}
		}

		#ifdef Function_YouzhiyunjiaWifi
		EnableWifiRx_Interrupt();
		#endif

		if (((AntiPryingMgr.AntiPryingTrigger == bTRUE)&&( PickAlarmEnableMgr.Enable == bTRUE)))
		{
			SystemPowerMgr.AwakeSource = AntiPrying;
		}
		#ifdef Function_FM17622_LPCD
		else if ( PINMACRO_NFC_IRQ_STATUS == 0 )
		{
			SystemPowerMgr.AwakeSource = NFCIrq;
		}
		#endif
		else
		{
			HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);
		}
		
		CLRWDT();
		{__ASM ("nop");__ASM ("nop");}	

		if ( SystemPowerMgr.AwakeSource == RTCwakeupTimer )
		{
			if ( BodyInductionMgr.BodyInductionDeviceType == PirModule )
			{
				DEBUG_MARK;
				if ( BodyInductionMgr.BodyInductionDelayTimeCnt > 0 )
				{
					BodyInductionMgr.BodyInductionDelayTimeCnt--;
				}
				else
				{
					PIR_Task();
					if ( PIRmgr.FaceDetected == bTRUE )
					{
						SystemPowerMgr.AwakeSource = BodyInduction;
						RTC_DisableWakeUpTimer();
						MX_TIM3_Init();
						MX_TIM6_Init();
						HAL_TIM_Base_Start_IT(&htim3);
						HAL_TIM_Base_Start_IT(&htim6);
						HAL_ResumeTick();
						break;
					}
				}
			}
			else if ( BodyInductionMgr.BodyInductionDeviceType == RadarModule )
			{
				DEBUG_MARK;
				if ( BodyInductionMgr.BodyInductionDelayTimeCnt > 0 )
				{
					BodyInductionMgr.BodyInductionDelayTimeCnt--;
				}
			}
			else
			{
				DEBUG_MARK;
			}
		}
		
		else if (SystemPowerMgr.AwakeSource == BodyInduction)
		{	
			if ( BodyInductionMgr.BodyInductionDeviceType == RadarModule )
			{
				RadarAwake_TurnOff();
			}
			MX_TIM3_Init();
			MX_TIM6_Init();
			HAL_TIM_Base_Start_IT(&htim3);
			HAL_TIM_Base_Start_IT(&htim6);
			HAL_ResumeTick();
			break;
		}
		else
		{	
			RTC_DisableWakeUpTimer();
			MX_TIM3_Init();
			MX_TIM6_Init();
			HAL_TIM_Base_Start_IT(&htim3);
			HAL_TIM_Base_Start_IT(&htim6);
			HAL_ResumeTick();
			break;
		}
	}

	if (SystemPowerMgr.AwakeSource == BodyInduction)
	{	
		if ( BodyInductionMgr.BodyInductionDeviceType == PirModule )
		{
			if ( BodyInductionMgr.BodyInductionInvalidTimes < DEF_IrBodyInductionInvalidTimesLimited )
			{
				BodyInductionMgr.BodyInductionInvalidTimes++;
				DEBUG_MARK;
			}
			if ( BodyInductionMgr.BodyInductionInvalidTimes >= DEF_IrBodyInductionInvalidTimesLimited )
			{
				PIR_SensitivityDecrease();
				BodyInductionMgr.BodyInductionDelayTimeCnt = 0;
				DEBUG_MARK;
			}
		}
	}
	else
	{
		//BodyInductionMgr.BodyInductionInvalidTimes = 0;
		//BodyInductionMgr.BodyInductionBeLimited = bFALSE;
		BodyInductionMgr.BodyInductionDelayTimeCnt = 0;
		DEBUG_MARK;
	}
}
void System_Awake(void)
{
	uint8_t i;
	
	CLRWDT();

	Hardware_DelayMs(1);		//wait for system clock stable//

#ifdef Function_YouzhiyunjiaWifi
	DisableWifiRx_Interrupt();
#endif
	
	MX_GPIO_Init();
	MX_ADC1_Init();

	/* 得到精准电池电压 */
	PINMACRO_TurnOnVbatToADC;
	Hardware_DelayMs(5);		//wait for OLED Driver power on reset
	for (i=0;i<15;i++)
	{
        Hardware_Task_Analog_Battery();
        HardwareBatteryMgr_Task();
	}
	
	HAL_SPI_MspInit(&hspi1);
	//HAL_SPI_MspInit(&hspi2);
	MFC_WAKEUP();

	
	MX_I2C1_Init();	

	VOICE_Init();	
	
	#ifdef Function_VoiceIcPowerTurnOffInSleepMode
	SET_SPEEKERPOWER_ON;
	#endif

	
	if (( SystemPowerMgr.AwakeSource == DriverBoard )
		&&(BatteryMgr.BatteryType == ZnMnBattery)
		)
	{
		AwakeDisplayMgr.DisplayType = DisplayHide;
		//AwakeDisplayMgr.TimeCnt = Def_GuiTimeDelayCnt1s;
	}
	else if (SystemPowerMgr.AwakeSource == BodyInduction)
	{
		AwakeDisplayMgr.DisplayType = DisplayFace;
	}
	else if (SystemPowerMgr.AwakeSource == FingerTouch)
	{
		AwakeDisplayMgr.DisplayType = DisplayFinger;
	}
	else
	{
		AwakeDisplayMgr.DisplayType = DisplayCaseIgnore;
	}

	if ( FrmMgr.FrmFunctionConfig == FRM_Enabled )
	{
		if (( SystemPowerMgr.AwakeSource == KeyBoardTouch )
		||(SystemPowerMgr.AwakeSource == DriverBoard)
		)
		{
			FaceIdentifyMgr.IndentifyDelayTimeCnt = Def_GuiTimeDelayCnt2s;
		}	
		else
		{
			FaceIdentifyMgr.IndentifyDelayTimeCnt = 0x0000;
		}
		
		FaceRecognitionMgr_Init();

		if ( FaceIdentifyMgr.IndentifyDelayTimeCnt == 0x0000 )	//提前供电人脸模块，提高识别速度
		{
			FaceRecognition_HardwarePowerOn();
		}
	}


	
	SET_FPMPOWER_ON; 
	SET_LEDPOWER_ON;

	#ifdef Function_DisplayUsed_HFG12864
	if (AwakeDisplayMgr.DisplayType != DisplayHide)
	{
		SET_OLEDPOWER_ON;
	}
	#endif

	FPMpowerMgr.Status = FPMpowerOn;
//	Hardware_DelayMs(95);		//wait for OLED Driver power on reset
	
//	HAL_UART_MspInit(&huart1);	
	MX_USART1_UART_Init();
	
//	HAL_UART_MspInit(&huart2);	
	MX_USART2_UART_Init();

	#ifdef Function_YouzhiyunjiaWifi

	//WifiMgr.Power.Status = PowerOn;
	MX_USART3_UART_Init();
	
	
	#endif



	ComPort_Init();
	
	MFC_Init();
	Hardware_DelayMs(5);	
	
	TOUCH_AWAKE();

	#ifdef Function_DisplayUsed_OLED128X64
	OLED_Init();
	#elif defined Function_DisplayUsed_HFG12864
	HFG12864_Init();
	#endif

	LEDsMgr_Init();

	GUI_Init();		


	Hardware_DelayMs(5);

	
	Hardware_DelayMs(1);
	
//	/* 得到精准电池电压 */
//	for (i=0;i<10;i++)
//	{
//            Hardware_Task_Analog_Battery();
//            HardwareBatteryMgr_Task();
//	}
	if ((( BatteryMgr.BatteryLevel == LEVEL_1 ) ||( BatteryMgr.BatteryLevel == LEVEL_0 ))
		#if defined (ProjectIs_AutoBarLock_S68Z04) || defined (ProjectIs_AutoBarLock_S68Z10)
		&&(AwakeSystemKeyMgr.IsDoorColseKeyAwake != bTRUE)
		#endif
		)
	{
		PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_PleaseReplaceTheBattery);
		BatteryMgr.TimeCnt = Def_MessageBoxTimeDelay;
		CurrentScreen = SCREEN_LowBattery;	
		
		ComPort_SetPost_Info(DEF_WifiInfo_LowBattery,0x00,0x00);
		
		#if defined (Function_YouzhiyunjiaWifi) || defined (Function_TuyaWifi)
		Wifi_PostEvent(DEF_WifiEvent_LowBattery,0x00,0x00);
		#endif
	}
	else
	{
		GoIntoMainScreen_WithIdentifyInit();
		if ( IfSystemIsInFactoryDefaultStatus()==bTRUE )
		{
			PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_PleaseAddMasterFirst);
		}
		else
		{
			if ( (SystemPowerMgr.AwakeSource == KeyBoardTouch)
				&&(AwakeSystemKeyMgr.IsDoorBellKeyAwake !=bTRUE)
				)
			{
				PLAY_VOICE_ONESEGMENT(VoiceMgr.volume,VOICE_POWERON);	
			}
			
		}
		
	}
	
	SystemPowerMgr.SleepSource = SleepTimer;
	
	//GUI_SetFPM_LED(DEF_FpmLedMode_Breath,DEF_FpmLedColor_Blue,DEF_FpmLedColor_Blue,255);
	
	Hardware_AwakeLine_WorkingStatus();
	
}


void Wifi_PowerOnForOnlinePasswordVerifyMgr(void)
{
	if (   (CurrentScreen == SCREEN_Main )
		&& ( PasscodeUserIdentifyMgr.Status == PasscodeIdentifyPasscodeInput )		//passcode user identify
		 &&( PasscodeInputMgr.Point > 3 )
		 &&( PasscodeInputMgr.InputBuff[0] == 1)
		 &&( IfSystemIsInFactoryDefaultStatus()!=bTRUE )
		)
	{
		if ( WifiMgr.WifiOnlinePasswordVerifyMgr.WifiPowerIsOnForOnlinePassword != bTRUE )
		{
			ComPort_SetPost_PositivePowerOnWifi();
		}
	}
}

#if defined (Function_YouzhiyunjiaWifi) || defined (Function_TuyaWifi)
void LocalWifi_PowerOnForOnlinePasswordVerifyMgr(void)
{
	if (   (CurrentScreen == SCREEN_Main )
		&& ( PasscodeUserIdentifyMgr.Status == PasscodeIdentifyPasscodeInput )		//passcode user identify
		 //&&( PasscodeInputMgr.Status == PasscodeInputStart )
		 &&( PasscodeInputMgr.Point > 3 )
		 &&( PasscodeInputMgr.Point < 8 )		//online password lenth is	7
		 &&( PasscodeInputMgr.InputBuff[0] == 1)
		 &&( IfSystemIsInFactoryDefaultStatus()!=bTRUE )
		)
	{
		if ( WifiMgr.Power.PowerOnForOnlinePassword != bTRUE )
		{
			WifiMgr.Power.PowerOnForOnlinePassword = bTRUE;
		}
	}
	else
	{
		if (  ( PasscodeUserIdentifyMgr.Status == PasscodeIdentifyPasscodeInput )		//passcode user identify
			//&&( PasscodeInputMgr.Status == PasscodeInputEnd )
		  	&&( PasscodeInputMgr.Point == 7 ) 
		   	&&( PasscodeInputMgr.InputBuff[0] == 1)
		   )
		{
			//Do nothing
		}
	   else
		{
		
			#if defined (Function_YouzhiyunjiaWifi) || defined (Function_TuyaWifi)
			Wifi_PowerOffIfPowerOnForOnlinePasswordVerify();
			#endif
		}
	}
}
#endif

/*******************************************************/
/*******************************************************/
/*******************************************************/
void GUI_Init(void)
{

	//uint8_t i;

	Clear_Screen();

	GUI_ToggleFlag_05s=0x00;
	GUI_ToggleFlag_1s=0x01;

	GUI_Flag_RefreshLCD = bFALSE;

	FpIdentifyMgr.Status = FPMcmdStart;
	CardIdentifyMgr.Status = ReadingCardID;
	
	BatteryMgr.PostLowBattery = bTRUE;

	//VoiceMgr.Enable = bTRUE;		//default Enable Voice
	
	Key_Scan_Init();

	ReadSystemTime();

	ReadCardUserMemoryFromEEPROM();

	ReadPasscodeUserMemoryFromEEPROM();
	
	//if ( FrmMgr.FrmFunctionConfig == FRM_Enabled )
	{
		ReadFaceUserMemoryFromEEPROM();
	}
	
  	ReadAutoMotorMgrConfig();

//	ReadManagerPasscodeFromMemory();

//	SafetyMonitorMgr.IdentifyFailedTimes = 0x00;
//	SafetyMonitorMgr.SystemLocked = bFALSE;

	UserIdentifyResultMgr.CardIdentifyStatus = S_FAIL;
	UserIdentifyResultMgr.FPIdentifyStatus = S_FAIL;
	UserIdentifyResultMgr.PasscodeIdentifyStatus = S_FAIL;

	g_ASTERISK_PressedOnMainScreen = bFALSE;

	DriverBoardVersion.HWversion = 0x00;
	DriverBoardVersion.FWversion = 0x00;

	FpmLEDsMgr.OperationMode = DEF_FpmLedMode_Off;
	FpmLEDsMgr.StartColor = DEF_FpmLedColor_All;
	FpmLEDsMgr.StopColor = DEF_FpmLedColor_All;
	FpmLEDsMgr.Cycles = 0;
       
    DEBUG_MARK;
	
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowEventLogBySequence(void)
{
	static uint16_t VoiceReportLogBuff[][5]=
	{	
		{VOICE_Press,VOICE_Two,VOICE_Previous,DEF_VoiceSegmentEndFlag},
		{VOICE_Press,VOICE_Eight,VOICE_Next,DEF_VoiceSegmentEndFlag},
		{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag},
		{VOICE_PressPoundKey,VOICE_ToRepeatListening,DEF_VoiceSegmentEndFlag},
		{VOICE_Mute20ms,DEF_VoiceSegmentEndFlag}
	};

	if ( CheckEventLogBySequenceMgr.Status == SearchEventLogWithSequence)
	{	
		SearchEventLogBySequence();
		//Clear_Screen();
		
		if ( CheckEventLogBySequenceMgr.MatchedEventLogNum > 0x0000 )
		{
			LogMgr.DisplayPoint = CheckEventLogBySequenceMgr.StartEventLogPoint;
			LogMgr.SavedDisplayPoint = LogMgr.DisplayPoint+1;	//for reload Display Log
			CheckEventLogBySequenceMgr.OffsetEventLogNum = 0x0000;
			CheckEventLogBySequenceMgr.Status = ShowEventLogWithSequence;
			VoiceMenuMgr.MenuPoint = 0;
			VoiceMenuMgr.TotalMenuNum = 5;	
		}
		else
		{
			PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,NoLogVoiceStr);
			CheckEventLogBySequenceMgr.Status = ShowEventLogWithSequence;
		}
	}
	else if ( CheckEventLogBySequenceMgr.Status == VoiceReportOperation )
	{
		if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
		{
			if ( VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
			{
				PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceReportLogBuff[VoiceMenuMgr.MenuPoint]);
				VoiceMenuMgr.MenuPoint++;
			}
			else
			{
				CheckEventLogBySequenceMgr.Status = ShowEventLogWithSequence;
			}
		}
	}
	else if (CheckEventLogBySequenceMgr.Status == ShowEventLogWithSequence )
	{
		if ( CheckEventLogBySequenceMgr.MatchedEventLogNum == 0x0000 )
		{
			if (SystemLanguage == Chinese){
				DisHZ16x14Str(3,40,NoEventLogStr,NormalDisplay);
			}
			else{
				DisEN16x8Str(3,40,NoEventLogStrEn,NormalDisplay);
			}

		}
		else
		{
			if ( LogMgr.SavedDisplayPoint != LogMgr.DisplayPoint )
			{
				LogMgr_ReadLog(LogMgr.DisplayPoint,&LogMgr.DisplayLog.FlagHighByte);
				LogMgr.SavedDisplayPoint = LogMgr.DisplayPoint;
				VoiceReportLogMgr.Status = ReportLogInit;
				Clear_Screen();
			}
			
			DisplayOneEventLog(CheckEventLogBySequenceMgr.OffsetEventLogNum+1);

			if ( VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
			{
				if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
				{
					PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceReportLogBuff[VoiceMenuMgr.MenuPoint]);
					VoiceMenuMgr.MenuPoint++;
				}
			}
			else
			{
				VoiceReportOneEventLog(CheckEventLogBySequenceMgr.OffsetEventLogNum+1);
			}

		}
	}
	
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void GUI_Task(void)
{
	//uint8_t *Point;
        uint8_t i;
	if (++GUI_TimeCnt > 63)
	{
		GUI_ToggleFlag_1s^=0x01;
		GUI_TimeCnt =0;
	}
	
	if ( GUI_TimeCnt %32 == 0x00 ){	
		GUI_ToggleFlag_05s^= 0x01;
	}
	
	if ( CardIdentifyMgr.CardDetectIntervalTimeCnt >  0 )
	{
		CardIdentifyMgr.CardDetectIntervalTimeCnt --;
	}
	if ( CardIdentifyMgr.DelayTimeAfterReadCard < Def_CardDetectIntervalTime )
	{
		CardIdentifyMgr.DelayTimeAfterReadCard++;
	}
	
	if ( FrmMgr.FrmFunctionSwitchTimeDelay >  0 )
	{
		FrmMgr.FrmFunctionSwitchTimeDelay --;
	}
	if ( LockBrand.BrandChangeTimeDelay > 0 )
	{
		 LockBrand.BrandChangeTimeDelay--;
	}

	if (  (AntiPryingMgr.AntiPryingTrigger == bTRUE)
		&&(CurrentScreen != SCREEN_PickLockAlarm)
		&&( CurrentScreen != SCREEN_Initialization )
		&&( CurrentScreen != SCREEN_PickAlarmEnableSetting )
		&&( CurrentScreen != SCREEN_RestoreFactoryDefault )		//add on 2022.08.24
		&&( PickAlarmEnableMgr.Enable == bTRUE)
	   )
	{
		AntiPryingMgr.AntiPryingTrigger = bFALSE;
		AntiPryingMgr.FilterTimes = 0x00;
		for (i=0;i<50;i++)
		{
			if ( PINMACRO_PICKLOCK_STATUS == 0x00 )
			{
				AntiPryingMgr.FilterTimes++;
			}
			Hardware_DelayMs(1);
		}
		if ( AntiPryingMgr.FilterTimes > 45 )
		{
			if ( TouchPowerMgr.Status != HighSensitivity )
			{
				SET_TOUCH_AWAKE_SENSITIVITY();
			}/** in case of AntiPrying trigger when MotorSlider is moving down, turn on CSK14**/

			if ( CurrentScreen == SCREEN_NetWorkLink )
			{
				ComPort_SetPost_ExitWifiHotSpot();
			}
			else if ( CurrentScreen == SCREEN_RemoteUnlockRequest )
			{
				ComPort_SetPost_ExitRemoteUnlcok();
			}

			UserIdentifyResultMgr.CardIdentifyStatus = S_FAIL;
			UserIdentifyResultMgr.FPIdentifyStatus = S_FAIL;
			UserIdentifyResultMgr.PasscodeIdentifyStatus = S_FAIL;
			UserIdentifyResultMgr.FaceIdentifyStatus = S_FAIL;
	
			CurrentScreen = SCREEN_PickLockAlarm;
			AntiPryingMgr.TimeCnt = DEF_AntiPryingTime;
			PasscodeUserIdentifyMgr.Status = PasscodeIdentifyIdle;
			FpIdentifyMgr.Status = FPMcmdStart;
			CardIdentifyMgr.Status = ReadingCardID;		
			
			ComPort_SetPost_Alarm(DEF_WifiAlarm_PickLock,UNDEFINEDUSER,0x00);
			
			#if defined (Function_YouzhiyunjiaWifi) || defined (Function_TuyaWifi)
			Wifi_PostEvent(DEF_WifiEvent_AntiPryingAlarm,0x00,0x00);
			#endif
			
			GUI_SetFPM_LED(DEF_FpmLedMode_Off,DEF_FpmLedColor_All,DEF_FpmLedColor_All,255);

			SET_LED_RGB_OFF();
			SET_LED_RB_OFF();
		}
	}

	//if ( GUI_TimeCnt%4==0 )
	//{
		GUI_Button_Monitor();
	//}

	GUI_Update_Screen();
	
	#if defined (Function_YouzhiyunjiaWifi) || defined (Function_TuyaWifi)
	LocalWifi_PowerOnForOnlinePasswordVerifyMgr();
	#else
	Wifi_PowerOnForOnlinePasswordVerifyMgr();	
	#endif
	
}
