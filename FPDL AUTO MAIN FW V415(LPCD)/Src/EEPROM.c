#include "Project.h"
#include "IO.h"
#include "Hardware.h"
#include "ISR.h"
#include "StdTypes.h"
#include "EEPROM.h"
#include "I2cBus.h"
#include "i2c.h"

uint8_t BUFF_A[600];
uint8_t BUFF_B[600];

void EEPROM_ReadSequential(uint16_t StartAddr,uint8_t *BUFF,uint16_t Num)
{
	uint8_t failtimes;
	for(failtimes=0;failtimes<5;failtimes++)
	{
		if ( HAL_I2C_Mem_Read(&hi2c1,I2CADDR_E2PROM,StartAddr,I2C_MEMADD_SIZE_16BIT,BUFF,Num,0xffff) == HAL_OK )
		{
			DEBUG_MARK;
			break;
		}
		else
		{
			MX_I2C1_DeInit();
			MX_I2C1_Init();			
		}
	}
}

void EEPROM_PageWrite(uint16_t StartAddr,uint8_t *BUFF,uint16_t Num)
{
	uint8_t failtimes;
	for(failtimes=0;failtimes<5;failtimes++)
	{
		if ( HAL_I2C_Mem_Write(&hi2c1,I2CADDR_E2PROM,StartAddr,I2C_MEMADD_SIZE_16BIT,BUFF,Num,0xffff) == HAL_OK )
		{
			DEBUG_MARK;
			break;
		}
		else
		{
			MX_I2C1_DeInit();
			MX_I2C1_Init();			
		}
	}
}

void EEPROM_WriteSequential(uint16_t StartAddr,uint8_t *BUFF,uint16_t Num)
{
	uint8_t OffsetByte;
	uint16_t SavedByteNum;
	uint16_t Addr;
	SavedByteNum = 0;
	Addr=StartAddr;
	
	for (;;)
		{
			if ( Num > SavedByteNum)
			{
					OffsetByte = Addr%32;
					if ( Num > (SavedByteNum+32-OffsetByte) )
					{
						EEPROM_PageWrite(Addr,BUFF,(32-OffsetByte));
						SavedByteNum+=(32-OffsetByte);
						BUFF+=(32-OffsetByte);
						Addr+=(32-OffsetByte);
						Hardware_DelayMs(8);
					}
					else
					{
						EEPROM_PageWrite(Addr,BUFF,Num-SavedByteNum);
						SavedByteNum = Num;
						Hardware_DelayMs(8);
						break;
					}
			}
		}		
}


void EEPROM_TESTING(void)
{
	uint16_t i;
	
	static uint8_t V_len;
	

	for (i=0;i<400;i++){
		BUFF_A[i] = i;
	}
	for (i=0;i<400;i++){
		BUFF_B[i] = 0x00;
	}
	
	DEBUG_MARK;
	
	//EEPROM_ReadSequential(0x0000,BUFF_B,100);

	DEBUG_MARK;

	EEPROM_WriteSequential(0x0400,BUFF_A,250);

	DEBUG_MARK;

	for (i=0;i<600;i++){
		BUFF_B[i] = 0x00;
	}	
	DEBUG_MARK;
	EEPROM_ReadSequential(0x0400,BUFF_B,250);
	DEBUG_MARK;
}


