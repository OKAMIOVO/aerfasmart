#ifndef _LCD_HFG12864_H
#define _LCD_HFG12864_H

#include "Project.h"

#ifdef Function_DisplayUsed_HFG12864

#define NormalDisplay		0x00
#define InverseDisplay		0x01
#define GREEN_COLOR			0x02
#define RED_COLOR			0x03

extern void HFG12864_Init(void);
extern void HFG12864_Display_Task(void);

extern void Clear_Screen(void);
extern void Clear_Screen_Page(uint8_t page);
extern void Clear_PartyScreen(uint8_t StartPage,uint8_t StartColumn,uint8_t Width,uint8_t Heigth);
extern void Display_FullScreen(void);
extern void Display_Init(void);

extern void DisHZ16x14Str(uint8_t  StartPage,uint8_t StartColumn,const uint8_t *BUFF,uint8_t Color);

extern void DisZF16x8(uint8_t StartPage,uint8_t StartColumn,uint8_t Value,uint8_t Color);

extern void DisOneDigital16x8(uint8_t StartPage,uint8_t StartColumn,uint8_t  Value,uint8_t Color);
extern void DisBcdDigital16x8(uint8_t StartPage,uint8_t StartColumn,uint8_t  Value,uint8_t Color);
extern void DisBcdDigital32x20(uint8_t StartPage,uint8_t StartColumn,uint8_t  Value,uint8_t Color);

extern void DisDigital16x8Str(uint8_t StartPage,uint8_t StartColumn,uint16_t  Value,uint8_t Color);
extern void DisEN16x8Str(uint8_t StartPage,uint8_t StartColumn,const uint8_t *BUFF,uint8_t Color);
extern void DisImage(uint8_t StartPage,uint8_t StartColumn,uint8_t Width,uint8_t Heigth,const uint8_t *BUFF,uint8_t Color);
extern void DisImage_RAM(uint8_t StartPage,uint8_t StartColumn,uint8_t Width,uint8_t Heigth,uint8_t *BUFF,uint8_t Color);
#endif

#endif