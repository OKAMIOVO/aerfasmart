#include "StdTypes.h"
#include "IO.h"
#include "Hardware.h"
#include "Project.h"
#include "OLEDdriver.h"
#include "spi.h"

//#include "Font.h"

#ifdef Function_DisplayUsed_OLED128X64

void SPI1_SendByte(uint8_t dat)
{
	HAL_SPI_Transmit(&hspi1,&dat,1,500);
}

void OLED_Write_Cmd(uint8_t cmd)
{
	SET_LCD_DS_L;
	__ASM ("nop");__ASM ("nop");__ASM ("nop");__ASM ("nop");
	__ASM ("nop");__ASM ("nop");__ASM ("nop");__ASM ("nop");	
	SET_LCD_CS_L;
	__ASM ("nop");__ASM ("nop");__ASM ("nop");__ASM ("nop");
	HAL_SPI_Transmit(&hspi1,&cmd,1,10);
	__ASM ("nop");__ASM ("nop");__ASM ("nop");__ASM ("nop");
	SET_LCD_CS_H;	
	__ASM ("nop");__ASM ("nop");__ASM ("nop");__ASM ("nop");
}

void OLED_Write_DisData(uint8_t DisData)
{
	SET_LCD_CS_L;
	SET_LCD_DS_H;
	//asm("nop");asm("nop");
	HAL_SPI_Transmit(&hspi1,&DisData,1,10);
}

void OLED_HardwareRest(void)
{
	SET_LCD_RST_H;
	Hardware_DelayMs(3);
	SET_LCD_RST_L;
	Hardware_DelayMs(3);
	SET_LCD_RST_H;
	Hardware_DelayMs(3);
}

void OLED_Dirrect_Config(void)
{
#if defined (ProjectIs_AutoBarLock_S58Z02)||defined (ProjectIs_AutoBarLock_S51Z03)\
	||defined (ProjectIs_AutoBarLock_S49Z05)||defined (ProjectIs_AutoBarLock_S67Z01)\
	||defined (ProjectIs_AutoBarLock_S64Z04)||defined (ProjectIs_AutoBarLock_S06Z04)\
	||defined (ProjectIs_AutoBarLock_S64Z10)|| defined (ProjectIs_AutoBarLock_S78Z01)\
	|| defined (ProjectIs_AutoBarLock_S72Z03)
	OLED_Write_Cmd(0xA1);//--Set SEG/Column Mapping 	0xa1左右反置 0xa0正常
	OLED_Write_Cmd(0xC8);//Set COM/Row Scan Direction	0xc0上下反置 0xc8正常
#else
	OLED_Write_Cmd(0xA0);//--Set SEG/Column Mapping 	0xa1左右反置 0xa0正常
	OLED_Write_Cmd(0xC0);//Set COM/Row Scan Direction	0xc0上下反置 0xc8正常
#endif

}
void OLED_DriverConfig(void)
{
	OLED_Write_Cmd(0xAE);//关闭OLED
	
//	OLED_Write_Cmd(0x00);//设置列低位地址
//	OLED_Write_Cmd(0x10);//设置列高位地址
	
	OLED_Write_Cmd(0x40);//设置起始行地址及映射RAM显示起始行 (0x00~0x3F)
	
	OLED_Write_Cmd(0x81);//对比度设置
	OLED_Write_Cmd(0xCF); //0xCF Set SEG Output Current Brightness
	
	OLED_Dirrect_Config();

	OLED_Write_Cmd(0xA4);//禁用显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)(0xa4/0xa5) 
	
	OLED_Write_Cmd(0xA6);//设置显示方式;bit0:1,反相显示;0,正常显示
	
	OLED_Write_Cmd(0xA8);//设置驱动路数(1 to 64)
	OLED_Write_Cmd(0x3f);//--1/64 duty
	
	OLED_Write_Cmd(0xD3);//-设置显示偏移(0x00~0x3F)
	OLED_Write_Cmd(0x00);//-not offset
	
	OLED_Write_Cmd(0xd5);//--set display clock divide ratio/oscillator frequency
	OLED_Write_Cmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_Write_Cmd(0xD9);//--set pre-charge period
	OLED_Write_Cmd(0xF1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_Write_Cmd(0xDA);//--set com pins hardware configuration
	OLED_Write_Cmd(0x12);
	OLED_Write_Cmd(0xDB);//--set vcomh
	OLED_Write_Cmd(0x40);//Set VCOM Deselect Level
	
	OLED_Write_Cmd(0x20);//设置页地址模式(0x00/0x01/0x02)
	OLED_Write_Cmd(0x00);//
	
	OLED_Write_Cmd(0x21);// set start and end column addr
	OLED_Write_Cmd(0x00);// set start column addr
	OLED_Write_Cmd(0x7F);// set end column addr

	OLED_Write_Cmd(0x22);// set start and end page addr
	OLED_Write_Cmd(0x00);// set start page addr
	OLED_Write_Cmd(0x07);// set end page addr	
	
	OLED_Write_Cmd(0x8D);//--set Charge Pump enable/disable
	OLED_Write_Cmd(0x14);//--set(0x10) disable
	
	OLED_Write_Cmd(0xAF);//打开显示
	
//	Hardware_DelayMs(10);
	
	SET_LCD_DS_H;
	SET_LCD_CS_H;

}

void OLED_Init(void)
{
	OLED_HardwareRest();
	
	OLED_DriverConfig();

	
	Clear_Screen();
	OLED_Display_Task();

	DEBUG_MARK;
}

void OLED_Display_Task(void)
{
	uint8_t i,j;

	OLED_Write_Cmd(0x21);// set start and end column addr
	OLED_Write_Cmd(0x00);// set start column addr
	OLED_Write_Cmd(0x7F);// set end column addr

	OLED_Write_Cmd(0x22);// set start and end page addr
	OLED_Write_Cmd(0x00);// set start page addr
	OLED_Write_Cmd(0x07);// set end page addr	


	SET_LCD_DS_H;
	__ASM ("nop");
	__ASM ("nop");
	__ASM ("nop");
	__ASM ("nop");
	__ASM ("nop");
	__ASM ("nop");__ASM ("nop");__ASM ("nop");	
	__ASM ("nop");__ASM ("nop");__ASM ("nop");__ASM ("nop");
	SET_LCD_CS_L;
	__ASM ("nop");__ASM ("nop");
	
	HAL_SPI_Transmit(&hspi1,G_DisRAM,1024,1000);
	
	//OLED_Write_Cmd(0xE3);// addtional NOP Command
	
	SET_LCD_CS_H;
}


void Clear_Screen(void)
{
	uint16_t i;

	for (i=0;i<1024;i++)
	{
		G_DisRAM[i] = 0x00;
	}
	
}
void Clear_Screen_Page(uint8_t page)
{
	uint16_t i;

	for (i=0;i<128;i++)
	{
		G_DisRAM[page*128+i] = 0x00;
	}
}
void Clear_PartyScreen(uint8_t StartPage,uint8_t StartColumn,uint8_t Width,uint8_t Heigth)
{
	uint8_t i,j,Page,Column;
	uint16_t StartByte;
	
	Page=  StartPage;
	Column = StartColumn;

	StartByte = (128*Page) + Column;	
	for (j=0;j<(Heigth/8);j++)
	{
		for (i=0;i<Width;i++)
		{
			G_DisRAM[StartByte+i]=0x00;
		}
		StartByte+=128;
	}	
}


void Display_FullScreen(void)
{
	uint16_t i;
	for (i=0;i<1024;i++)
	{
          G_DisRAM[i] = 0xFF;
	}
}

void Display_Init(void)
{

}

/*
void DisHZ16x14(uint8_t   StartPage,uint8_t StartColumn,uint8_t BUFF,uint8_t Color)
{
	uint8_t j,Page,Column;
	uint16_t StartByte;
	const uint8_t *Point;
	
	Page = StartPage;
	Column = StartColumn;
	
	Point = &HZ16x14[BUFF][0];
	StartByte = (128*Page) + Column;
	for (j=0;j<14;j++)
	{
		if ( Color == NormalDisplay ) { //normal display
			G_DisRAM[StartByte+j]=*Point;
		}else{		//inverse display
			G_DisRAM[StartByte+j]=(*Point)^0xFF;
		}
		Point++;
	}
	for (j=0;j<14;j++)
	{
		if ( Color == NormalDisplay ) { //normal display
			G_DisRAM[StartByte+j+128]=*Point;
		}else{		//inverse display
			G_DisRAM[StartByte+j+128]=(*Point)^0xFF;
		}
		Point++;
	}
}
*/

void DisHZ16x14Str(uint8_t StartPage,uint8_t StartColumn,const uint8_t *BUFF,uint8_t Color)
{
	uint8_t i,j,Page,Column;
	uint16_t StartByte;
	const uint8_t *Point;
	
	Page=  StartPage;
	Column = StartColumn;
	
	for (i=0;i<36;i++)
	{
		if ( *BUFF==HZ_end )
		{
			break;
		}
		if (((*BUFF<200)&&( Column >113 ))||((*BUFF>199)&&( Column >120 )))//it is not enough to display one HZ	
		{
			Column=0;
			Page+=2;
		}
		
		StartByte = (128*Page) + Column;

		if (*BUFF<230)		//the char is HZ16*14
		{
			Point = &HZ16x14[*BUFF][0];
			for (j=0;j<14;j++)
			{
				if ( Color == NormalDisplay ) { //normal display
					G_DisRAM[StartByte+j]=*Point;
				}else{		//inverse display
					G_DisRAM[StartByte+j]=(*Point)^0xFF;
				}
				Point++;
			}
			for (j=0;j<14;j++)
			{
				if ( Color == NormalDisplay ) { //normal display
					G_DisRAM[StartByte+j+128]=*Point;
				}else{		//inverse display
					G_DisRAM[StartByte+j+128]=(*Point)^0xFF;
				}
				Point++;
			}
			BUFF++;
			Column+=14;
		}
		else if (*BUFF < HZ_end )		//the char is ZF
		{
			Point = &ZF16x8[(*BUFF)-230][0];
			for (j=0;j<8;j++)
			{
				if ( Color == NormalDisplay ) { //normal display
					G_DisRAM[StartByte+j]=*Point;
				}else{		//inverse display
					G_DisRAM[StartByte+j]=(*Point)^0xFF;
				}
				Point++;
			}
			for (j=0;j<8;j++)
			{
				if ( Color == NormalDisplay ) { //normal display
					G_DisRAM[StartByte+j+128]=*Point;
				}else{		//inverse display
					G_DisRAM[StartByte+j+128]=(*Point)^0xFF;
				}
				Point++;
			}
			BUFF++;
			Column+=8;
		}
	}
}
void DisEN16x8Str(uint8_t StartPage,uint8_t StartColumn,const uint8_t *BUFF,uint8_t Color)
{
	uint8_t i,j,Page,Column;
	uint16_t StartByte;
	const uint8_t *Point;
	
	Page=  StartPage;
	Column = StartColumn;
	
	for (i=0;i<16;i++)
	{
		if ( *BUFF=='\0' )
		{
			break;
		}
		if ( Column >120 )		//it is not enough to display one English char
		{
			break;
		}
		
		StartByte = (128*Page) + Column;

		if  ( (*BUFF>0x1F)&&(*BUFF<0x7B) )		//FROM space to 'z'
		{
			Point = &EN16x8[(*BUFF)-0x20][0];
		}
		else
		{
			Point = &EN16x8[0][0];	//SPACE
		}
		
		for (j=0;j<8;j++)
		{
			if ( Color == NormalDisplay ) { //normal display
				G_DisRAM[StartByte+j]=*Point;
			}else{		//inverse display
				G_DisRAM[StartByte+j]=(*Point)^0xFF;
			}
			Point++;
		}
		for (j=0;j<8;j++)
		{
			if ( Color == NormalDisplay ) { //normal display
				G_DisRAM[StartByte+j+128]=*Point;
			}else{		//inverse display
				G_DisRAM[StartByte+j+128]=(*Point)^0xFF;
			}
			Point++;
		}
		BUFF++;
		Column+=8;
	}
}

/*
void DisEN16x8StrConst(uint8_t StartPage,uint8_t StartColumn,uint8_t const *BUFF,uint8_t Color)
{
	uint8_t i,j,Page,Column;
	uint16_t StartByte;
	const uint8_t *Point;
	
	Page=  StartPage;
	Column = StartColumn;
	
	for (i=0;i<16;i++)
	{
		if ( *BUFF=='\0' )
		{
			break;
		}
		if ( Column >119 )		//it is not enough to display one English char
		{
			break;
		}
		
		StartByte = (128*Page) + Column;

		if  ( (*BUFF>0x1F)&&(*BUFF<0x7B) )		//FROM space to 'z'
		{
			Point = &EN16x8[(*BUFF)-0x20][0];
		}
		else
		{
			Point = &EN16x8[0][0];	//SPACE
		}
		
		for (j=0;j<8;j++)
		{
			if ( Color == NormalDisplay ) { //normal display
				G_DisRAM[StartByte+j]=*Point;
			}else{		//inverse display
				G_DisRAM[StartByte+j]=(*Point)^0xFF;
			}
			Point++;
		}
		for (j=0;j<8;j++)
		{
			if ( Color == NormalDisplay ) { //normal display
				G_DisRAM[StartByte+j+128]=*Point;
			}else{		//inverse display
				G_DisRAM[StartByte+j+128]=(*Point)^0xFF;
			}
			Point++;
		}
		BUFF++;
		Column+=8;
	}
}
*/
void DisZF16x8(uint8_t StartPage,uint8_t StartColumn,uint8_t  Value,uint8_t Color)
{
	uint8_t j,Page,Column;
	uint16_t StartByte;
	const uint8_t *Point;
	//uint8_t DigitalPosition;

	//DigitalPosition =0x00;
	
	Page=  StartPage;
	Column = StartColumn;

	if ( Column >120 )		//it is not enough to display one English char
	{
		return;
	}
	
	StartByte = (128*Page) + Column;

	if ( Value >229 )
	{
		Point = &ZF16x8[Value-230][0];
		for (j=0;j<8;j++)
		{
			if ( Color == NormalDisplay ) { //normal display
				G_DisRAM[StartByte+j]=*Point;
			}else{		//inverse display
				G_DisRAM[StartByte+j]=(*Point)^0xFF;
			}
			Point++;
		}
		for (j=0;j<8;j++)
		{
			if ( Color == NormalDisplay ) { //normal display
				G_DisRAM[StartByte+j+128]=*Point;
			}else{		//inverse display
				G_DisRAM[StartByte+j+128]=(*Point)^0xFF;
			}
			Point++;
		}
	}

}

void DisOneDigital16x8(uint8_t StartPage,uint8_t StartColumn,uint8_t  Value,uint8_t Color)
{
	uint8_t j,Page,Column;
	uint16_t StartByte;
	const uint8_t *Point;
	//uint8_t DigitalPosition;

	//DigitalPosition =0x00;
	
	Page=  StartPage;
	Column = StartColumn;

	
	StartByte = (128*Page) + Column;

	Point = &ZF16x8[Value%10][0];
	for (j=0;j<8;j++)
	{
		if ( Color == NormalDisplay ) { //normal display
			G_DisRAM[StartByte+j]=*Point;
		}else{		//inverse display
			G_DisRAM[StartByte+j]=(*Point)^0xFF;
		}
		Point++;
	}
	for (j=0;j<8;j++)
	{
		if ( Color == NormalDisplay ) { //normal display
			G_DisRAM[StartByte+j+128]=*Point;
		}else{		//inverse display
			G_DisRAM[StartByte+j+128]=(*Point)^0xFF;
		}
		Point++;
	}

}

/*
void DisTwoDigital16x8(uint8_t StartPage,uint8_t StartColumn,uint8_t  Value,uint8_t Color)
{
	uint8_t i,j,Page,Column;
	uint16_t StartByte;
	uint8_t *Point;
	uint8_t DigitalPosition;

	DigitalPosition =0x00;
	
	Page=  StartPage;
	Column = StartColumn;

	Value =Value%100;
		

	for (i=0;i<2;i++)
	{
		if ( Column >120 )		//it is not enough to display one HZ
		{
			Column=0;
			Page++;
		}
		
		StartByte = (128*Page) + Column;

		Point = &ZF16x8[Value/10][0];
		for (j=0;j<8;j++)
		{
			if ( Color == NormalDisplay ) { //normal display
				G_DisRAM[StartByte+j]=*Point;
			}else{		//inverse display
				G_DisRAM[StartByte+j]=(*Point)^0xFF;
			}
			Point++;
		}
		for (j=0;j<8;j++)
		{
			if ( Color == NormalDisplay ) { //normal display
				G_DisRAM[StartByte+j+128]=*Point;
			}else{		//inverse display
				G_DisRAM[StartByte+j+128]=(*Point)^0xFF;
			}
			Point++;
		}
		Column+=8;
		Value =(Value%10);
		Value*=10;

	}
}
*/

void DisBcdDigital16x8(uint8_t StartPage,uint8_t StartColumn,uint8_t  Value,uint8_t Color)
{
	uint8_t i,j,Page,Column;
	uint16_t StartByte;
	const uint8_t *Point;

	
	Page=  StartPage;
	Column = StartColumn;


	for (i=0;i<2;i++)
	{
		
		StartByte = (128*Page) + Column;
		Point = &ZF16x8[Value>>4][0];
		for (j=0;j<8;j++)
		{
			if ( Color == NormalDisplay ) { //normal display
				G_DisRAM[StartByte+j]=*Point;
			}else{		//inverse display
				G_DisRAM[StartByte+j]=(*Point)^0xFF;
			}
			Point++;
		}
		for (j=0;j<8;j++)
		{
			if ( Color == NormalDisplay ) { //normal display
				G_DisRAM[StartByte+j+128]=*Point;
			}else{		//inverse display
				G_DisRAM[StartByte+j+128]=(*Point)^0xFF;
			}
			Point++;
		}
		Column+=8;
		Value <<=4;
	}
}

#if 0
void DisDigital16x8Str(uint8_t StartPage,uint8_t StartColumn,uint16_t  Value,uint8_t Color)
{
	uint8_t i,j,Page,Column;
	uint16_t StartByte;
	const uint8_t *Point;
	uint8_t DigitalPosition;
	uint16_t DataValue;

	DigitalPosition =0x00;
	
	Page=  StartPage;
	Column = StartColumn;
	DataValue = Value;

	for (DigitalPosition=4;DigitalPosition>1;DigitalPosition--)
	{
		if ( DataValue < 1000 )
		{
			DataValue=DataValue*10;
		}
		else
		{
			break;
		}
	}
			

	for (i=0;i<DigitalPosition;i++)
	{
		if ( Column >120 )		//it is not enough to display one HZ
		{
			Column=0;
			Page++;
		}
		
		StartByte = (128*Page) + Column;

		Point = &ZF16x8[DataValue/1000][0];
		for (j=0;j<8;j++)
		{
			if ( Color == NormalDisplay ) { //normal display
				G_DisRAM[StartByte+j]=*Point;
			}else{		//inverse display
				G_DisRAM[StartByte+j]=(*Point)^0xFF;
			}
			Point++;
		}
		for (j=0;j<8;j++)
		{
			if ( Color == NormalDisplay ) { //normal display
				G_DisRAM[StartByte+j+128]=*Point;
			}else{		//inverse display
				G_DisRAM[StartByte+j+128]=(*Point)^0xFF;
			}
			Point++;
		}
		Column+=8;
		DataValue=(DataValue%1000)*10;

	}
}
#endif

void DisDigital16x8Str(uint8_t StartPage,uint8_t StartColumn,uint16_t  Value,uint8_t Color)
{
	uint8_t i,j,Page,Column;
	uint16_t StartByte;
	const uint8_t *Point;
	uint8_t DigitalPosition;
	uint16_t DataValue;
	
	Page=  StartPage;
	Column = StartColumn;
	DataValue = Value;
			

	for (i=0;i<3;i++)
	{
		if ( Column >120 )		//it is not enough to display one HZ
		{
			Column=0;
			Page++;
		}
		
		StartByte = (128*Page) + Column;

		Point = &ZF16x8[DataValue/100][0];
		for (j=0;j<8;j++)
		{
			if ( Color == NormalDisplay ) { //normal display
				G_DisRAM[StartByte+j]=*Point;
			}else{		//inverse display
				G_DisRAM[StartByte+j]=(*Point)^0xFF;
			}
			Point++;
		}
		for (j=0;j<8;j++)
		{
			if ( Color == NormalDisplay ) { //normal display
				G_DisRAM[StartByte+j+128]=*Point;
			}else{		//inverse display
				G_DisRAM[StartByte+j+128]=(*Point)^0xFF;
			}
			Point++;
		}
		Column+=8;
		DataValue=(DataValue%100)*10;

	}
}




void DisImage(uint8_t StartPage,uint8_t StartColumn,uint8_t Width,uint8_t Heigth,const uint8_t *BUFF,uint8_t Color)
{
	uint8_t i,j,Page,Column;
	uint16_t StartByte;
	
	Page=  StartPage;
	Column = StartColumn;

	StartByte = (128*Page) + Column;	
	for (j=0;j<(Heigth/8);j++)
	{
		for (i=0;i<Width;i++)
		{
			if ( Color == NormalDisplay ) { //normal display
				G_DisRAM[StartByte+i]=*(BUFF+i);
			}else{		//inverse display
				G_DisRAM[StartByte+i]=(*(BUFF+i))^0xFF;
			}
		}
		StartByte+=128;
		BUFF+=Width;
	}	
}

void DisImage_RAM(uint8_t StartPage,uint8_t StartColumn,uint8_t Width,uint8_t Heigth,uint8_t *BUFF,uint8_t Color)
{
	uint8_t i,j,Page,Column;
	uint16_t StartByte;
	
	Page=  StartPage;
	Column = StartColumn;

	StartByte = (128*Page) + Column;	
	for (j=0;j<(Heigth/8);j++)
	{
		for (i=0;i<Width;i++)
		{
			if ( Color == NormalDisplay ) { //normal display
				G_DisRAM[StartByte+i]=*(BUFF+i);
			}else{		//inverse display
				G_DisRAM[StartByte+i]=(*(BUFF+i))^0xFF;
			}
		}
		StartByte+=128;
		BUFF+=Width;
	}	
}

void DisBcdDigital32x20(uint8_t StartPage,uint8_t StartColumn,uint8_t  Value,uint8_t Color)
{
	uint8_t i,j,k,Page,Column;
	uint16_t StartByte;
	const uint8_t *Point;
	
	Page=  StartPage;
	Column = StartColumn;

	for (k=0;k<2;k++)
	{
		
		StartByte = (128*Page) + Column;	
		Point = &ZF32x20[Value>>4][0];
		for (j=0;j<4;j++)
		{
			for (i=0;i<20;i++)
			{
				if ( Color == NormalDisplay ) { //normal display
					G_DisRAM[StartByte+i]=*(Point);
				}else{		//inverse display
					G_DisRAM[StartByte+i]=(*Point)^0xFF;
				}
				Point++;
			}
			StartByte+=128;
		}
		Column+=20;
		Value <<=4;
	}
}


#endif

