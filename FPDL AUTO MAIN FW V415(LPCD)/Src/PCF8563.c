#include "Project.h"
#include "IO.h"
#include "Hardware.h"
#include "ISR.h"
#include "StdTypes.h"
#include "I2cBus.h"



/* pseudo func for I2C */
#define I2C1_START    (I2C1_CR2 |= 0x01)
#define I2C1_STOP     (I2C1_CR2 |= 0x02)
#define I2C1_ACK      (I2C1_CR2 |= SETBIT2)
#define I2C1_NACK     (I2C1_CR2 &= CLRBIT2)
#define I2C1_ISBUSY   (I2C1_SR3 & SETBIT1)
#define I2C1_ERROR    (I2C1_SR2 & SETBIT0)
#define I2C1_RST      {I2C1_CR2 |= SETBIT7;I2C1_CR2 &= CLRBIT7;}
#define I2C1_ISMASTER (I2C1_SR3 & SETBIT0)

#define PCF8653_CONFIG_REG0 	0x00
#define PCF8653_CONFIG_REG1		0x00		
#define PCF8653_CONFIG_REG2	 	0x00	//second
#define PCF8653_CONFIG_REG3		0x00
#define PCF8653_CONFIG_REG4		0x12
#define PCF8653_CONFIG_REG5		0x25
#define PCF8653_CONFIG_REG6		0x02
#define PCF8653_CONFIG_REG7 	0x09
#define PCF8653_CONFIG_REG8 	0x18
#define PCF8653_CONFIG_REG9 	0x80
#define PCF8653_CONFIG_REG10 	0x80
#define PCF8653_CONFIG_REG11 	0x80
#define PCF8653_CONFIG_REG12 	0x80
#define PCF8653_CONFIG_REG13 	0x00
#define PCF8653_CONFIG_REG14 	0x00
#define PCF8653_CONFIG_REG15 	0x00

static uint8_t TestVal;
static uint8_t TestBuff1[20];

//static uint8_t BUFFER[16];SPI1_

void PCF8563_Init(void)
{
	uint8_t temp;
	
	while((I2C1_SR3 & 0x02) != 0);                //WAIT FOR I2C IDLE

  I2C1_START; /* gen start condition */
	
  while(!(I2C1_SR1 & SETBIT0));     /* wait for start cond complete */
	
  /* read SR1 and then write DR to clear event */
  I2C1_DR = I2CADDR_RTC; /* set chip addr, dummy write */

  while(!(I2C1_SR1 & SETBIT1)) {
    
  }; /* wait for chip addr transmit */

  /* dummy read SR3 */
  temp = I2C1_SR3;

  I2C1_DR = 0x00; /* byte addr */
  while(!(I2C1_SR1 & SETBIT2)); /* wait for byte addr transmit */
  /* I2C1_STOP; gen stop condition */

 	/* write data */
	I2C1_DR = PCF8653_CONFIG_REG0;			while(!(I2C1_SR1 & SETBIT2)); /* wait for byte DATA transmit */
	I2C1_DR = PCF8653_CONFIG_REG1;			while(!(I2C1_SR1 & SETBIT2)); /* wait for byte DATA transmit */
	I2C1_DR = PCF8653_CONFIG_REG2;			while(!(I2C1_SR1 & SETBIT2)); /* wait for byte DATA transmit */
	I2C1_DR = PCF8653_CONFIG_REG3;			while(!(I2C1_SR1 & SETBIT2)); /* wait for byte DATA transmit */
	I2C1_DR = PCF8653_CONFIG_REG4;			while(!(I2C1_SR1 & SETBIT2)); /* wait for byte DATA transmit */
	I2C1_DR = PCF8653_CONFIG_REG5;			while(!(I2C1_SR1 & SETBIT2)); /* wait for byte DATA transmit */
	I2C1_DR = PCF8653_CONFIG_REG6;			while(!(I2C1_SR1 & SETBIT2)); /* wait for byte DATA transmit */
	I2C1_DR = PCF8653_CONFIG_REG7;			while(!(I2C1_SR1 & SETBIT2)); /* wait for byte DATA transmit */
	I2C1_DR = PCF8653_CONFIG_REG8;			while(!(I2C1_SR1 & SETBIT2)); /* wait for byte DATA transmit */
	I2C1_DR = PCF8653_CONFIG_REG9;			while(!(I2C1_SR1 & SETBIT2)); /* wait for byte DATA transmit */
	I2C1_DR = PCF8653_CONFIG_REG10;		while(!(I2C1_SR1 & SETBIT2)); /* wait for byte DATA transmit */
	I2C1_DR = PCF8653_CONFIG_REG11;		while(!(I2C1_SR1 & SETBIT2)); /* wait for byte DATA transmit */
	I2C1_DR = PCF8653_CONFIG_REG12;		while(!(I2C1_SR1 & SETBIT2)); /* wait for byte DATA transmit */
	I2C1_DR = PCF8653_CONFIG_REG13;		while(!(I2C1_SR1 & SETBIT2)); /* wait for byte DATA transmit */
	I2C1_DR = PCF8653_CONFIG_REG14;		while(!(I2C1_SR1 & SETBIT2)); /* wait for byte DATA transmit */
	I2C1_DR = PCF8653_CONFIG_REG15;		while(!(I2C1_SR1 & SETBIT2)); /* wait for byte DATA transmit */
	
	I2C1_STOP;
	
	Hardware_DelayMs(10);
}


void PCF8563_ReadTime(void)
{
	uint8_t i;
	uint8_t BUFFER[7];
	/*
	for (i=0;i<16;i++)
	{
		BUFFER[i]=0x00;
	}
	
	DEBUG_MARK;
	*/
	I2C_ReadSequential(I2CADDR_RTC,0x02,&BUFFER[0],7);

	SystemTime.second 	= 	BUFFER[0]&0x7F;		//Voltage Low bit is not care
	SystemTime.minute 	= 	BUFFER[1]&0x7F;	
	SystemTime.hour    	= 	BUFFER[2]&0x3F;	
	SystemTime.date    	= 	BUFFER[3]&0x3F;
	SystemTime.day		= 	BUFFER[4]&0x07;
	SystemTime.month 	= 	BUFFER[5]&0x1F;		//CENTURY BIT is not care 
	SystemTime.year    	=	BUFFER[6];

	DEBUG_MARK;
}


void PCF8563_WriteTime(void)
{
	uint8_t i;
	uint8_t BUFF[7];
	
	BUFF[0]= SystemTime.second;	
	BUFF[1]= SystemTime.minute;	
	BUFF[2]= SystemTime.hour;
	BUFF[3]= SystemTime.date;
	BUFF[4]= SystemTime.day;
	BUFF[5]= SystemTime.month;
	BUFF[6]= SystemTime.year;

	I2C_WriteSequential(I2CADDR_RTC,0x02,&BUFF[0],7);

	DEBUG_MARK;
	
	//for (i=0;i<7;i++)
	//{
	//	BUFF[i]=0x88;;
	//}

	//I2C1_ReadSequential(I2CADDR_RTC,0x02,&BUFF[0],7);

	DEBUG_MARK;
	
}



void PCF8563_Test(void)
{
	//PCF8563_RegRead(&TestBuff1[0],10);
	
	DEBUG_MARK;
}
