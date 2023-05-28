#ifndef __YouzhiyunjiaWIFI__
#define __YouzhiyunjiaWIFI__	1

#include "Project.h"

#define DEF_YouzhiyunWifiAlarm_AntiPryingAlarm		0x00
#define DEF_YouzhiyunWifiAlarm_FpUnlockAlarm		0x02
#define DEF_YouzhiyunWifiAlarm_PasswordUnlockAlarm	0x03
#define DEF_YouzhiyunWifiAlarm_CardUnlockAlarm		0x04
#define DEF_YouzhiyunWifiAlarm_StressUnlock			0x08

#define DEF_YouzhiyunWifiInfo_LowBattery			0x08
#define DEF_YouzhiyunWifiInfo_AddUser				0x09
#define DEF_YouzhiyunWifiInfo_DeleteUser			0x0A
#define DEF_YouzhiyunWifiInfo_DoorBell				0x0C

#define DEF_YouzhiyunUserType_FP				0x01
#define DEF_YouzhiyunUserType_PASSCODE			0x02
#define DEF_YouzhiyunUserType_CARD				0x03
#define DEF_YouzhiyunUserType_MOBILE			0x05
#define DEF_YouzhiyunUserType_TEMPUSER			0x09


#ifdef Function_YouzhiyunjiaWifi 

#define DEF_WifiFlowChartTimeOutDelay	64		//1s
#define DEF_WifiPowerDelayTime			96		//1.5s
#define DEF_WifiFastPowerDelayTime		64		//0.5s

extern void Wifi_PostGetHotspotCmd(void);
extern void Wifi_PostlockStatusCmd(UserType_t usertype, uint16_t userID );
extern void Wifi_PostUnlockStatusCmd(UserType_t usertype, uint16_t userID );
extern void Wifi_PostRemoteUnlockRequestCmd(void );
extern void Wifi_PostRemoteUnlockSuccessAck(void );
extern void Wifi_PostEvent(uint8_t EventType, UserType_t usertype, uint16_t userID);
extern void Wifi_PostGetUTC(uint8_t Times);
extern void Wifi_PostCheckWifiStatus(uint8_t Times);
extern void Wifi_PositiveClosing(void);
extern void Wifi_PowerOffIfPowerOnForOnlinePasswordVerify(void);
extern void Wifi_PostOnlinePasswordVerify(uint8_t BUFF[]);
extern void Wifi_PositiveExitRemoteUnlock(void);
extern void Wifi_PostPowerDown(void);
extern void Wifi_PostAwake(void);
extern void Wifi_PostClearData(void);
extern void Wifi_PostManufactureTest(void);
extern void Wifi_DoorBellAwake(void);
extern void Wifi_Init(void);
extern void Wifi_Mgr_Task(void);
#endif


#endif
