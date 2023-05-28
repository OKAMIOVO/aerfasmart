#include "Project.h"
#include "IO.h"
#include "Hardware.h"
#include "StdTypes.h"
#include "Flash.h"
#include "spi.h"
#include "iwdg.h"

#define DEF_InternalFlash_LogMemoryStartAddr 0x0801D800		//Last 5 pages
#define DEF_InternalFlash_LogMemoryStartPage 0x3B			//59th page



static uint8_t TestByte[160];
static uint8_t StatusReg;
static uint8_t TESTBYTE;

static uint64_t FlashDwData;
status_t FalshTestStatus;

static uint32_t Debug_data32;


HAL_StatusTypeDef Flash_WriteDW(uint32_t Addr,uint64_t DWdata)
{
	HAL_StatusTypeDef tempStatus;
	
	uint8_t TryTimes;
	
	TryTimes=3;
	do
	{
		tempStatus = HAL_FLASH_Unlock();
		TryTimes--;
	}while((tempStatus!= HAL_OK)&&(TryTimes>0));
	if ( TryTimes == 0 )
	{
			return HAL_ERROR;
	}
	
	TryTimes=3;
	do
	{
		tempStatus = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, Addr, DWdata); 
		TryTimes--;
	}while((tempStatus!= HAL_OK)&&(TryTimes>0));
	
	if ( TryTimes == 0 )
	{
			return HAL_ERROR;
	}
	
	TryTimes=3;
	do
	{
		tempStatus = HAL_FLASH_Lock(); 
		TryTimes--;
	}while((tempStatus!= HAL_OK)&&(TryTimes>0));
	
	if ( TryTimes == 0 )
	{
			return HAL_ERROR;
	}
	
	return HAL_OK;
}

status_t Flash_EraseOnePage(uint32_t PageAddr)
{
		uint32_t PageError = 0;  //
		
		FLASH_EraseInitTypeDef My_Flash;  //

		HAL_FLASH_Unlock();               //
				
		My_Flash.TypeErase = FLASH_TYPEERASE_PAGES;  //
		My_Flash.Page = PageAddr;  //
		My_Flash.NbPages = 1;                        //
															
		HAL_FLASHEx_Erase(&My_Flash, &PageError);  //
	
		HAL_FLASH_Lock(); //
	
    if (PageError!=0xFFFFFFFF)
			return S_FAIL;
		else
			return S_SUCCESS;
}

uint32_t Flash_ReadWord(uint32_t Addr)
{
	uint32_t Tempdata;
	
	Tempdata = *(__IO uint32_t *)Addr;

	return Tempdata;

}


void LogMgr_WriteLog(uint16_t LogPoint)
{
	uint8_t i,j;
	static uint8_t Debug8bits;
	static uint64_t Data64Bits;
	uint32_t Addr;
	uint8_t *point;
	Addr = LogPoint*16+DEF_InternalFlash_LogMemoryStartAddr;
	//Flash_WriteSequential(Addr,&LogMgr.NewLog.FlagHighByte,16);
	
	point = &LogMgr.NewLog.FlagHighByte;
	
	if ( (Addr%2048) == 0x00000000)
	{
		Flash_EraseOnePage((Addr-0x08000000)/2048);
	}
	
	for (i=0;i<2;i++)
	{
		for (j=8;j>0;j--)
		{
			Data64Bits<<=8;
			Data64Bits+=*(point+j-1);
		}
		if ( Flash_WriteDW(Addr+(i*8),Data64Bits) == HAL_OK )
		{
			point+=8;
		}
	  else
		{
				break;
		}
	}
}

void LogMgr_ReadLog(uint16_t LogPoint,uint8_t *point)
{
	uint8_t i,j;
	uint32_t Addr;
	uint32_t Data32Bits;

	Addr = LogPoint<<=4;		//LogPoint*16	
	Addr+= DEF_InternalFlash_LogMemoryStartAddr;
	DEBUG_MARK;
	
	for (i=0;i<4;i++)
	{
		Data32Bits = Flash_ReadWord(Addr+(i*4));
		
		for (j=0;j<4;j++)
		{
			*point = Data32Bits;
			Data32Bits>>=8;
			point++;
		}
	}
}

status_t DeleteAllLog(void)
{
	uint8_t i;
	
	for (i=0;i<5;i++)
	{
		if ( Flash_EraseOnePage(DEF_InternalFlash_LogMemoryStartPage+i) == S_FAIL )
		{
			DEBUG_MARK;
			return S_FAIL;
		}
	}
	
	return S_SUCCESS;
}


void Flash_Test(void)
{
	uint16_t i;

	//FalshTestStatus = Flash_EraseOnePage(60);
	DEBUG_MARK;
	
	Debug_data32 = 0x00000000;
	DEBUG_MARK;
	Debug_data32 = Flash_ReadWord(DEF_InternalFlash_LogMemoryStartAddr+0x01FC);
	DEBUG_MARK;

	Debug_data32 = Flash_ReadWord(DEF_InternalFlash_LogMemoryStartAddr+0x01FC+4);
	DEBUG_MARK;

	Debug_data32 = Flash_ReadWord(DEF_InternalFlash_LogMemoryStartAddr+0x01FC+8);
	DEBUG_MARK;

	Debug_data32 = Flash_ReadWord(DEF_InternalFlash_LogMemoryStartAddr+0x01FC+12);
	DEBUG_MARK;

}



void LogMgr_Init(void)
{
	uint16_t LogID;
	uint8_t Temp_LogIDmajor;
	Log_t TempLog; 
	LogMgr.LastPoint = 0x0000;
	Temp_LogIDmajor = 0x00;
	
	for (LogID=0;LogID<DEF_MaxRecordedLogID;LogID++)
	{
		//CLRWDT();
		LogMgr_ReadLog(LogID,&TempLog.FlagHighByte);
		if ( 	(TempLog.FlagHighByte == DEF_LogFlagHighByte)
				&&(TempLog.FlagLowByte == DEF_LogFlagLowByte)
				&&(TempLog.LogIDmajor >= Temp_LogIDmajor)
			)
		{
			LogMgr.LastPoint++;
			Temp_LogIDmajor = TempLog.LogIDmajor;
			LogMgr.NewLog.LogIDmajor = TempLog.LogIDmajor;
		}
		else
		{
			break;
		}
	}

	if ( LogMgr.LastPoint == 0 )
	{
		LogMgr.NewLog.LogIDmajor = 0x00;
	}

	DEBUG_MARK;

}

void SearchEventLogBySequence(void)
{
	uint16_t LogID,i;
	Log_t TempLog; 

	if ( LogMgr.LastPoint >1 )
	{
		LogID = LogMgr.LastPoint-1;
		DEBUG_MARK;
	}
	else{
		LogID = 0x0000;
		DEBUG_MARK;
	}

	CheckEventLogBySequenceMgr.MatchedEventLogNum = 0;
	
	for (i=0;i<DEF_MaxSearchLogID;i++)
	{
		CLRWDT();
		
		LogMgr_ReadLog(LogID,&TempLog.FlagHighByte);
		if ( 	(TempLog.FlagHighByte == DEF_LogFlagHighByte)
				&&(TempLog.FlagLowByte == DEF_LogFlagLowByte)
			)
		{	
			if ( CheckEventLogBySequenceMgr.MatchedEventLogNum == 0 )
			{
				CheckEventLogBySequenceMgr.StartEventLogPoint = LogID;	//first matched Log, saved to start point memory
			}
			CheckEventLogBySequenceMgr.MatchedEventLogNum++;
		}
		else if ( (TempLog.FlagHighByte != DEF_LogFlagHighByte)
				||(TempLog.FlagLowByte != DEF_LogFlagLowByte)
				)
		{
			break;
		}

		if (LogID>0)
		{
			LogID--;
		}
		else
		{
			if ( TempLog.LogIDmajor>0 )
			{
				LogID=DEF_MaxRecordedLogID-1;
			}
			else
			{
				break;
			}
		}
	}
}

void SearchEventLogByDate(void)
{
	uint16_t LogID,i;
	Log_t TempLog; 


	if ( LogMgr.LastPoint >1 )
	{
		LogID = LogMgr.LastPoint-1;
	}
	else{
		LogID = 0x0000;
	}

	CheckEventLogByDateMgr.MatchedEventLogNum = 0;
	
	for (i=0;i<DEF_MaxSearchLogID;i++)
	{
		CLRWDT();
		
		LogMgr_ReadLog(LogID,&TempLog.FlagHighByte);
		if ( 	(TempLog.FlagHighByte == DEF_LogFlagHighByte)
				&&(TempLog.FlagLowByte == DEF_LogFlagLowByte)
				&&(TempLog.year == CheckEventLogByDateMgr.year)
				&&(TempLog.month == CheckEventLogByDateMgr.month)
				&&(TempLog.date == CheckEventLogByDateMgr.date)
			)
		{	
			if ( CheckEventLogByDateMgr.MatchedEventLogNum == 0 )
			{
				CheckEventLogByDateMgr.StartEventLogPoint = LogID;	//first matched Log, saved to start point memory
			}
			CheckEventLogByDateMgr.MatchedEventLogNum++;
		}
		else if ( (TempLog.FlagHighByte != DEF_LogFlagHighByte)
				||(TempLog.FlagLowByte != DEF_LogFlagLowByte)
				)
		{
			break;
		
}

		if (LogID>0)
		{
			LogID--;
		}
		else
		{
			if ( TempLog.LogIDmajor>0 )
			{
				LogID=DEF_MaxRecordedLogID-1;
			}
			else
			{
				break;
			}
		}
	}
}

void FLASH_OB_Config(void)
{
	static FLASH_OBProgramInitTypeDef obData;
	static struct
	{	
		uint32_t USERBYTE;
		uint32_t RDPLEVEL;
		uint32_t WRPASTARTOFFSET;
		uint32_t WRPAENDOFFSET;
		uint32_t WRPBSTARTOFFSET;
		uint32_t WRPBENDOFFSET;
	}OB_Config_Value;

	HAL_FLASH_Unlock();
	HAL_FLASH_OB_Unlock();		
	obData.OptionType = OPTIONBYTE_USER;
	obData.USERType = OB_USER_ALL; 
	HAL_FLASHEx_OBGetConfig(&obData);
	OB_Config_Value.USERBYTE = obData.USERConfig;
	
	obData.OptionType = OPTIONBYTE_RDP;
	HAL_FLASHEx_OBGetConfig(&obData);
	OB_Config_Value.RDPLEVEL = obData.RDPLevel;

	obData.OptionType = OPTIONBYTE_WRP;
	obData.WRPArea = OB_WRPAREA_ZONE_A; 
	HAL_FLASHEx_OBGetConfig(&obData);
	OB_Config_Value.WRPASTARTOFFSET = obData.WRPStartOffset;
	OB_Config_Value.WRPAENDOFFSET = obData.WRPEndOffset;

	obData.OptionType = OPTIONBYTE_WRP;
	obData.WRPArea = OB_WRPAREA_ZONE_B; 
	HAL_FLASHEx_OBGetConfig(&obData);
	OB_Config_Value.WRPBSTARTOFFSET = obData.WRPStartOffset;
	OB_Config_Value.WRPBENDOFFSET = obData.WRPEndOffset;
	
	DEBUG_MARK;	
	
#ifdef DEBUG_MODE 

		if (  (OB_Config_Value.USERBYTE != 0x074D6000)
		||(OB_Config_Value.RDPLEVEL != OB_RDP_LEVEL_0)
		||(OB_Config_Value.WRPASTARTOFFSET != 0x0000003F)
		||(OB_Config_Value.WRPAENDOFFSET != 0x00000000)
		||(OB_Config_Value.WRPBSTARTOFFSET != 0x0000003F)
		||(OB_Config_Value.WRPBENDOFFSET != 0x00000000)
		)
		{			
			DEBUG_MARK;			
			obData.OptionType = OPTIONBYTE_USER;
			obData.USERType = OB_USER_ALL; 
			obData.USERConfig = 0x074D6000;
			while ( HAL_FLASHEx_OBProgram(&obData) != HAL_OK );

			Hardware_DelayMs(10);
			obData.OptionType = OPTIONBYTE_RDP;
			obData.RDPLevel = OB_RDP_LEVEL_0;
			while ( HAL_FLASHEx_OBProgram(&obData) != HAL_OK );

			Hardware_DelayMs(10);
			
			obData.OptionType = OPTIONBYTE_WRP;
			obData.WRPArea = OB_WRPAREA_ZONE_A; 
			obData.WRPStartOffset = 0x0000003F;
			obData.WRPEndOffset = 0x00000000;
			while ( HAL_FLASHEx_OBProgram(&obData) != HAL_OK );
			
			Hardware_DelayMs(10);
			
			obData.OptionType = OPTIONBYTE_WRP;
			obData.WRPArea = OB_WRPAREA_ZONE_B; 
			obData.WRPStartOffset = 0x0000003F;
			obData.WRPEndOffset = 0x00000000;
			while ( HAL_FLASHEx_OBProgram(&obData) != HAL_OK );			
			Hardware_DelayMs(10);
			
			HAL_FLASH_OB_Launch();

			Hardware_DelayMs(10);
		}

#else
		
	if (  (OB_Config_Value.USERBYTE != 0x074D6000)
		||(OB_Config_Value.RDPLEVEL != OB_RDP_LEVEL_1)
		||(OB_Config_Value.WRPASTARTOFFSET != 0x00000000)
		||(OB_Config_Value.WRPAENDOFFSET != 0x0000003A)
		||(OB_Config_Value.WRPBSTARTOFFSET != 0x00000000)
		||(OB_Config_Value.WRPBENDOFFSET != 0x0000003A)
		)
		{			
			DEBUG_MARK;			
			obData.OptionType = OPTIONBYTE_USER;
			obData.USERType = OB_USER_ALL; 
			obData.USERConfig = 0x074D6000;
			while ( HAL_FLASHEx_OBProgram(&obData) != HAL_OK );

			Hardware_DelayMs(10);
			obData.OptionType = OPTIONBYTE_RDP;
			obData.RDPLevel = OB_RDP_LEVEL_1;
			while ( HAL_FLASHEx_OBProgram(&obData) != HAL_OK );

			Hardware_DelayMs(10);
			
			obData.OptionType = OPTIONBYTE_WRP;
			obData.WRPArea = OB_WRPAREA_ZONE_A; 
			obData.WRPStartOffset = 0x00000000;
			obData.WRPEndOffset = 0x0000003A;
			while ( HAL_FLASHEx_OBProgram(&obData) != HAL_OK );
			
			Hardware_DelayMs(10);
			
			obData.OptionType = OPTIONBYTE_WRP;
			obData.WRPArea = OB_WRPAREA_ZONE_B; 
			obData.WRPStartOffset = 0x00000000;
			obData.WRPEndOffset = 0x0000003A;
			while ( HAL_FLASHEx_OBProgram(&obData) != HAL_OK );			
			Hardware_DelayMs(10);
			
			HAL_FLASH_OB_Launch();

			Hardware_DelayMs(10);
		}
#endif

		
		HAL_FLASH_Lock();
		HAL_FLASH_OB_Lock();	

	DEBUG_MARK;
}






