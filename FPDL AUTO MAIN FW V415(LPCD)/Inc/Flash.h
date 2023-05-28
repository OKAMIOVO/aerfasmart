#ifndef Flash_H
#define Flash_H

#include "StdTypes.h"

#define DEF_LogFlagHighByte	0x55
#define DEF_LogFlagLowByte	0xAA

#define DEF_MaxRecordedLogID	640		//128*5,total 5 Pages
#define DEF_MaxSearchLogID	500			//


extern void Flash_PowerDown(void);

extern void Flash_PowerUp(void);
extern void Flash_SoftReset(void);
extern void  FlashWriteEnable(void);
extern void Flash_SectorErase(uint32_t Addr);

//extern uint8_t Flash_ReadStatusRegister1(void);
//extern void Flash_ReadManufactoryID(uint8_t *buff);
//extern void Flash_ReadUniqueID(uint8_t *buff);
//extern void Flash_WriteSequential(uint32_t Addr,uint8_t *BUFF,uint16_t Num);
//extern void Flash_ReadSequential(uint32_t Addr,uint8_t *BUFF,uint16_t Num);
extern void LogMgr_WriteLog(uint16_t LogPoint);
extern void LogMgr_ReadLog(uint16_t LogPoint,uint8_t *point);
extern void LogMgr_Init(void);
extern void SearchEventLogBySequence(void);
extern void SearchEventLogByDate(void);
extern status_t DeleteAllLog(void);

extern void FLASH_OB_Config(void);

extern void Flash_Test(void);

#endif
