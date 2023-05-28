#ifndef I2cBus_h
#define I2cBus_h

/* pseudo func for I2C */
#define I2C_START    (I2C1_CR2 |= 0x01)
#define I2C_STOP     (I2C1_CR2 |= 0x02)
#define I2C_ACK      (I2C1_CR2 |= SETBIT2)
#define I2C_NACK     (I2C1_CR2 &= CLRBIT2)
#define I2C_ISBUSY   (I2C1_SR3 & SETBIT1)
#define I2C_ERROR    (I2C1_SR2 & SETBIT0)
#define I2C_RST      {I2C1_CR2 |= SETBIT7;I2C1_CR2 &= CLRBIT7;}
#define I2C_ISMASTER (I2C1_SR3 & SETBIT0)

#define I2CADDR_CSK14S		0x2A
#define I2CADDR_TSM12		0xD0
#define I2CADDR_TSM12S		0xD0
#define I2CADDR_MPR121 	0xB4
//#define I2CADDR_MPR121 	0xB8
#define I2CADDR_RTC			0xA2
#define I2CADDR_E2PROM 	0xA8	//0xA0
//#define I2CADDR_MFC			0x5E
#define I2CADDR_MFC			0x50//0x58
#define I2CADDR_FM5114_TOUCH 0x64


extern HAL_StatusTypeDef I2C_WriteRandom(uint8_t DeviceAddr,uint8_t RegisterAddr,uint8_t Value);
extern HAL_StatusTypeDef I2C_WriteSequential(uint8_t DeviceAddr,uint8_t StartAddr,uint8_t *BUFF,uint8_t Num);
extern HAL_StatusTypeDef I2C_ReadRandom(uint8_t DeviceAddr,uint8_t RegisterAddr,uint8_t *Point);
extern HAL_StatusTypeDef I2C_ReadSequential(uint8_t DeviceAddr,uint8_t StartAddr,uint8_t *BUFF,uint8_t Num);

#endif
