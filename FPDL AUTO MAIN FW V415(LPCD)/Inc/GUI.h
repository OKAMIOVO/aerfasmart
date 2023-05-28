#ifndef GUI_H
#define GUI_H

#include "StdTypes.h"

#define DEF_FpmLedColor_Red 0x04
#define DEF_FpmLedColor_Green 0x02
#define DEF_FpmLedColor_Blue 0x01
#define DEF_FpmLedColor_All 0x00


#define DEF_FpmLedMode_Breath	0x01
#define DEF_FpmLedMode_Flash	0x02
#define DEF_FpmLedMode_On		0x03
#define DEF_FpmLedMode_Off		0x04


#define Def_GuiTimeDelayCnt01s	7
#define Def_GuiTimeDelayCnt025s	16
#define Def_GuiTimeDelayCnt05s	32
#define Def_GuiTimeDelayCnt1s	64
#define Def_GuiTimeDelayCnt1s5	96
#define Def_GuiTimeDelayCnt2s	128
#define Def_GuiTimeDelayCnt2s5	160
#define Def_GuiTimeDelayCnt3s	192
#define Def_GuiTimeDelayCnt4s	256
#define Def_GuiTimeDelayCnt5s	320
#define Def_GuiTimeDelayCnt10s	640
#define Def_GuiTimeDelayCnt15s	960
#define Def_GuiTimeDelayCnt20s	1280
#define Def_GuiTimeDelayCnt25s	1600
#define Def_GuiTimeDelayCnt30s	1920
 

#define Def_BodyInductionDelayTime_AntiTurningBack	22					//0.7x22 = 15.4s
#define Def_BodyInductionDelayTime_FourHours	14400		//4 hour
#define Def_BodyInductionDelayTime_TwoHours	7200		//2 hour
#define Def_BodyInductionDelayTime_OneHour	3600		//1 hour
#define Def_BodyInductionDelayTime_HalfHour	1800		//0.5 hour

#define DEF_RadarBodyInductionInvalidTimesLimited	5	//5 times
#define DEF_IrBodyInductionInvalidTimesLimited	5	//5 times	


#define Def_CardDetectIntervalTime	32					//0.5s

#define Def_IdendtifyFailScreenTimeDelay	192		//3s
#define Def_IdendtifySuccessScreenTimeDelay	320		//5s

#define Def_RegisterUserScreenTimeDelay	128			//2s

#define Def_MessageBoxTimeDelay	128		//2s

#define Def_FPMcmdTimeOutDelay	128		//2s

#define Def_UserSwingCardTimeDelay	512	//8s

#define Def_WaitUserInputPasscodeTimeDelay		640		//10s	

#define Def_WaitUserRemoveFingerTimeDelay		320		//5s	

#define Def_WaitUserPutFingerTimeDelay		320		//5s

#define Def_IdentifySuccessScreenTimeLimited 2560		//40s


//#define Def_WaitUserInputUserIDTimeDelay	Def_GuiTimeDelayCnt10s

#define DEF_WifiEvent_FpUnlockAlarm			0x02
#define DEF_WifiEvent_PasswordUnlockAlarm	0x03
#define DEF_WifiEvent_CardUnlockAlarm		0x04
//#define DEF_WifiEvent_StressUnlockAlarm		0x05
#define DEF_WifiEvent_AntiPryingAlarm		0x06

#define DEF_WifiEvent_LowBattery			0x07
#define DEF_WifiEvent_AddUser				0x08
#define DEF_WifiEvent_DeleteUser			0x09
#define DEF_WifiEvent_DoorBell				0x0A
#define DEF_WifiEvent_PowerLevel			0x0B



#define DEF_FpIdentifyFailedTimesLimited	4
#define DEF_CardIdentifyFailedTimesLimited	4
#define DEF_PasscodeIdentifyFailedTimesLimited  4
#define DEF_ManagerPasscodeIdentifyFailedTimesLimited 4
#define DEF_ManagerFpIdentifyFailedTimesLimited   4


#define DEF_SystemLockedTime	11250	//180s, 16ms period
#define DEF_AntiPryingTime		11250	//180s

#define DEF_StrongUnlockingIntervalLimited		15			//in second

extern uint8_t BCDcodeToHEX(uint8_t Indata);
extern uint8_t HEXtoBCDcode(uint8_t Indata);
extern unsigned long mktime(unsigned int year0, unsigned int mon0,unsigned int day, unsigned int hour,unsigned int min, unsigned int sec);
extern bool_t IfSystemIsNoFaceUser(void);
extern bool_t IfSystemIsInFactoryDefaultStatus(void);
extern void GUI_Task(void);
extern void GUI_Init(void);
extern void SystemConfigLoad(void);
extern void ShowSystemVersion(void);
extern void GUI_GetUserNumList(void);
extern void System_PowerDown(void);
extern void System_Awake(void);
extern void GUI_RefreshSleepTime(void);
extern void BodyInductionConfigLoad(void);
extern void GUI_WifiDrivingRemoteUnlockSuccess(void);
extern void GUI_SetFPM_LED(uint8_t mode,uint8_t startcolor,uint8_t stopcolor,uint8_t cycles );;

extern bool_t IfSystemIsNoCardUser(void);


#endif
