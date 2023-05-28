#ifndef __HostUART__
#define __HostUART__	1

#include "Project.h"

#define DEF_WifiAlarm_PickLock				0x00
#define DEF_WifiAlarm_FpUnlockAlarm			0x02
#define DEF_WifiAlarm_PasswordUnlockAlarm	0x03
#define DEF_WifiAlarm_CardUnlockAlarm		0x04
#define DEF_WifiAlarm_StressUnlock			0x08
#define DEF_WifiAlarm_RemoveAlarm			0xFF



#define DEF_WifiInfo_LowBattery			0x08
#define DEF_WifiInfo_AddUser			0x09
#define DEF_WifiInfo_DeleteUser			0x0A
#define DEF_WifiInfo_DoorBell			0x0C

#define DEF_WifiFlowChartTimeOutDelay	64		//1s

extern status_t UART2_Write_TxBUFFER(unsigned char *buff,unsigned char len);
extern void UART2_TX_Task(void);
extern void UART2_CLEAR_TXBUFF(void);
extern void ComPort_SetPost_Parameter(void);
extern void ComPort_SetPost_Status(void);
extern void ComPort_SetPost_OpenDoor(uint8_t Usertype,uint16_t userID);
extern void ComPort_SetPost_CloseDoor(void);
extern void ComPort_SetPost_Info(uint8_t infotype,uint8_t Usertype,uint16_t userID);
extern void ComPort_SetPost_Alarm(uint8_t alarmtype,uint8_t Usertype,uint16_t userID);
extern void ComPort_SetPost_WifiHotSpot(void);
extern void ComPort_SetPost_ExitWifiHotSpot(void);
extern void ComPort_SetPost_ClearWifiData(void);
extern void ComPort_SetPost_GetNetWorkTime(void);
extern void ComPort_SetPost_RemoteUnlcok(void);
extern void ComPort_SetPost_WifiMFT(void);
extern void ComPort_SetPost_MotorSelftest(void);


extern void ComPort_Init(void);
extern void ComPort_Mgr_Task(void);


#endif
