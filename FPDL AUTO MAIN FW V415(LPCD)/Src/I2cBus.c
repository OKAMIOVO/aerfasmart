#include "Project.h"
#include "IO.h"
#include "Hardware.h"
#include "ISR.h"
#include "StdTypes.h"
#include "I2cBus.h"
#include "i2c.h"

//HAL_StatusTypeDef HAL_STATUS;
HAL_StatusTypeDef I2C_WriteRandom(uint8_t DeviceAddr,uint8_t RegisterAddr,uint8_t Value)
{
	return (HAL_I2C_Mem_Write(&hi2c1,DeviceAddr,RegisterAddr,I2C_MEMADD_SIZE_8BIT,&Value,1,50));
	DEBUG_MARK;
}

HAL_StatusTypeDef I2C_WriteSequential(uint8_t DeviceAddr,uint8_t StartAddr,uint8_t *BUFF,uint8_t Num)
{
	//HAL_StatusTypeDef HAL_STATUS;
	return (HAL_I2C_Mem_Write(&hi2c1,DeviceAddr,StartAddr,I2C_MEMADD_SIZE_8BIT,BUFF,Num,1000));	
}

HAL_StatusTypeDef I2C_ReadRandom(uint8_t DeviceAddr,uint8_t RegisterAddr,uint8_t *Point)
{
	//HAL_StatusTypeDef HAL_STATUS;
	return (HAL_I2C_Mem_Read(&hi2c1,DeviceAddr,RegisterAddr,I2C_MEMADD_SIZE_8BIT,Point,1,50));
}

HAL_StatusTypeDef I2C_ReadSequential(uint8_t DeviceAddr,uint8_t StartAddr,uint8_t *BUFF,uint8_t Num)
{
	//HAL_StatusTypeDef HAL_STATUS;
	return (HAL_I2C_Mem_Read(&hi2c1,DeviceAddr,StartAddr,I2C_MEMADD_SIZE_8BIT,BUFF,Num,1000));	
}
