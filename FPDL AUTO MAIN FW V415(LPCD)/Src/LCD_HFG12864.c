#include "StdTypes.h"
#include "IO.h"
#include "Hardware.h"
#include "Project.h"
#include "LCD_HFG12864.h"
#include "spi.h"

#ifdef Function_DisplayUsed_HFG12864

#define SET_LCD_SDA_H				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET);
#define SET_LCD_SDA_L				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET);

#define SET_LCD_SCL_H				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
#define SET_LCD_SCL_L				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);

static uint8_t SwitchPageNumber;


void SPI1_SendByte(uint8_t dat)
{
	HAL_SPI_Transmit(&hspi1,&dat,1,500);
}

void WriteComm(uint8_t cmd)
{

	SET_LCD_DS_L;
	__ASM ("nop");__ASM ("nop");//__ASM ("nop");__ASM ("nop");__ASM ("nop");	
	SET_LCD_CS_L;
	__ASM ("nop");__ASM ("nop");__ASM ("nop");__ASM ("nop");__ASM ("nop");
	HAL_SPI_Transmit(&hspi1,&cmd,1,10);
	__ASM ("nop");__ASM ("nop");__ASM ("nop");__ASM ("nop");__ASM ("nop");
	SET_LCD_CS_H;	
	//__ASM ("nop");__ASM ("nop");__ASM ("nop");__ASM ("nop");__ASM ("nop");


}

void HFG12864_Init(void)
{
	uint8_t i,j;
	
	SET_LCD_RST_H;  
	Hardware_DelayMs(1);
	
	SET_LCD_RST_L;
	Hardware_DelayMs(2);

	SET_LCD_RST_H;
	Hardware_DelayMs(3);
 
	WriteComm(0xe2); //reset
	WriteComm(0xa3); //LCD BIAS=1/9行写入数据地�?�?1，但读入不加

#if defined (ProjectIs_AutoBarLock_S70Z02)|| defined (ProjectIs_AutoBarLock_S51Z05)\
	|| defined (ProjectIs_AutoBarLock_S64Z07) || defined (ProjectIs_AutoBarLock_S51Z06)\
	|| defined (ProjectIs_AutoBarLock_S61Z11) || defined (ProjectIs_AutoBarLock_S58Z07)\
	|| defined (ProjectIs_AutoBarLock_S64Z19) || defined (ProjectIs_AutoBarLock_S83Z01)\
	|| defined (ProjectIs_AutoBarLock_S83Z02) || defined (ProjectIs_AutoBarLock_S70Z09)
	WriteComm(0xa1); //0xA0 or 0xA1, 列地�?移动方向:ADC=0(SEG1->SEG132)
	WriteComm(0xc8); //0xC0 or 0xC8, 行扫描方�?:SHL=1(COM64->COM1)
	
#elif defined (ProjectIs_AutoBarLock_S64Z08)		||	defined (ProjectIs_AutoBarLock_S64Z09)\
			|| defined (ProjectIs_AutoBarLock_S49Z15)	|| 	defined (ProjectIs_AutoBarLock_S95Z01)
	WriteComm(0xa0); //0xA0 or 0xA1, 列地�?移动方向:ADC=0(SEG1->SEG132)
	WriteComm(0xc8); //0xC0 or 0xC8, 行扫描方�?:SHL=1(COM64->COM1)

#elif defined (ProjectIs_AutoBarLock_S20Z04) || defined (ProjectIs_AutoBarLock_S68Z04)\
		||defined (ProjectIs_AutoBarLock_S78Z01)|| defined (ProjectIs_AutoBarLock_S68Z10)\
		|| defined (ProjectIs_AutoBarLock_S86Z02)
	WriteComm(0xa1); //0xA0 or 0xA1, 列地�?移动方向:ADC=0(SEG1->SEG132)
	WriteComm(0xc0); //0xC0 or 0xC8, 行扫描方�?:SHL=1(COM64->COM1)
	
#elif  defined (ProjectIs_AutoBarLock_S90Z20)

	WriteComm(0xa0); //0xA0 or 0xA1, 列地�?移动方向:ADC=0(SEG1->SEG132)
	WriteComm(0xc0); //0xC0 or 0xC8, 行扫描方�?:SHL=1(COM64->COM1)
#endif
	
	WriteComm(0x2c); //VC ON
	Hardware_DelayMs(1);
	WriteComm(0x2e); //VR ON
	Hardware_DelayMs(1);
	
	WriteComm(0x2f); //电源控制模式：VC,VR,VF=111
	Hardware_DelayMs(1);
	
	WriteComm(0x81); //对比度寄存器模式
	//Hardware_DelayMs(1);
	WriteComm(0x20); //对比度调�?
	Hardware_DelayMs(1);
	
	WriteComm(0x23); //设置电阻分压
	Hardware_DelayMs(1);

	Clear_Screen();

	for (i=0;i<8;i++)
	{
		#if defined (ProjectIs_AutoBarLock_S70Z02) || defined (ProjectIs_AutoBarLock_S51Z05)\
			|| defined (ProjectIs_AutoBarLock_S64Z07) || defined (ProjectIs_AutoBarLock_S51Z06)\
			|| defined (ProjectIs_AutoBarLock_S61Z11) || defined (ProjectIs_AutoBarLock_S58Z07)\
			|| defined (ProjectIs_AutoBarLock_S20Z04) || defined (ProjectIs_AutoBarLock_S68Z04)\
			|| defined (ProjectIs_AutoBarLock_S64Z19) || defined (ProjectIs_AutoBarLock_S83Z01)\
		    || defined (ProjectIs_AutoBarLock_S78Z01) || defined (ProjectIs_AutoBarLock_S83Z02)\
			|| defined (ProjectIs_AutoBarLock_S68Z10)\
			|| defined (ProjectIs_AutoBarLock_S70Z09) || defined (ProjectIs_AutoBarLock_S86Z02)
		WriteComm(0x10);// set start and end column addr
		WriteComm(0x04);// set start column addr	
		#elif defined (ProjectIs_AutoBarLock_S64Z08) || defined (ProjectIs_AutoBarLock_S64Z09)\
			|| defined (ProjectIs_AutoBarLock_S49Z15)|| defined (ProjectIs_AutoBarLock_S95Z01)\
			|| defined (ProjectIs_AutoBarLock_S90Z20)
		WriteComm(0x10);// set start and end column addr
		WriteComm(0x00);// set start column addr
		#endif
		
		WriteComm(0xB0+i);// set start page addr	
		
		SET_LCD_DS_H;
		__ASM ("nop");__ASM ("nop");//__ASM ("nop");__ASM ("nop");__ASM ("nop");
		SET_LCD_CS_L;
		__ASM ("nop");__ASM ("nop");__ASM ("nop");__ASM ("nop");__ASM ("nop");
		
		HAL_SPI_Transmit(&hspi1,&G_DisRAM[128*i],128,1000);
		SET_LCD_CS_H;
	}
	
	//WriteComm(0x10);// set start and end column addr
	//WriteComm(0x00);// set start column addr

	WriteComm(0xaf); //�?显示
	Hardware_DelayMs(1);

	SwitchPageNumber = 0x00;
	
}

		 
void HFG12864_Display_Task(void)
{
	uint8_t i,j;
	
	
	//WriteComm(0x40);// set start Line addr

	if ( SwitchPageNumber++ > 7 )
	{
		SwitchPageNumber = 0x00;
	}

#if defined (ProjectIs_AutoBarLock_S70Z02)|| defined (ProjectIs_AutoBarLock_S51Z05)\
	|| defined (ProjectIs_AutoBarLock_S64Z07) || defined (ProjectIs_AutoBarLock_S51Z06)\
	|| defined (ProjectIs_AutoBarLock_S61Z11) || defined (ProjectIs_AutoBarLock_S58Z07)\
	|| defined (ProjectIs_AutoBarLock_S20Z04) || defined (ProjectIs_AutoBarLock_S68Z04)\
	|| defined (ProjectIs_AutoBarLock_S64Z19) || defined (ProjectIs_AutoBarLock_S83Z01)\
	||defined (ProjectIs_AutoBarLock_S78Z01)  || defined (ProjectIs_AutoBarLock_S83Z02)\
	|| defined (ProjectIs_AutoBarLock_S68Z10)\
	|| defined (ProjectIs_AutoBarLock_S70Z09) || defined (ProjectIs_AutoBarLock_S86Z02)
	WriteComm(0x10);// set start and end column addr
	WriteComm(0x04);// set start column addr	
#elif defined (ProjectIs_AutoBarLock_S64Z08) || defined (ProjectIs_AutoBarLock_S64Z09)\
	|| defined (ProjectIs_AutoBarLock_S49Z15)|| defined (ProjectIs_AutoBarLock_S95Z01)\
	|| defined (ProjectIs_AutoBarLock_S90Z20)
	WriteComm(0x10);// set start and end column addr
	WriteComm(0x00);// set start column addr
#endif

	
	WriteComm(0xB0+SwitchPageNumber);// set start page addr	
	
	SET_LCD_DS_H;
	__ASM ("nop");__ASM ("nop");//__ASM ("nop");__ASM ("nop");__ASM ("nop");
	SET_LCD_CS_L;
	__ASM ("nop");__ASM ("nop");__ASM ("nop");__ASM ("nop");__ASM ("nop");
	
	HAL_SPI_Transmit(&hspi1,&G_DisRAM[128*SwitchPageNumber],128,1000);
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
		if (((*BUFF<230)&&( Column >113 ))||((*BUFF>229)&&( Column >120 )))//it is not enough to display one HZ	
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

void DisDigital16x8Str(uint8_t StartPage,uint8_t StartColumn,uint16_t  Value,uint8_t Color)
{
	uint8_t i,j,Page,Column;
	uint16_t StartByte;
	const uint8_t *Point;
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

