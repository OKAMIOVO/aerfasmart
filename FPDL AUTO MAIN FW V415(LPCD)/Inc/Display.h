#ifndef Display_H
#define Display_H

#define NormalDisplay		0x00
#define InverseDisplay		0x01


extern void Clear_Screen(void);
extern void Clear_Screen_Page(uint8_t page);
extern void Display_FullScreen(void);
extern void Display_Init(void);

extern void DisHZ16x14Str(uint8_t  StartPage,uint8_t StartColumn,const uint8_t *BUFF,uint8_t Color);
//extern void DisHZ16x14(uint8_t  StartPage,uint8_t StartColumn,uint8_t BUFF,uint8_t Color);

extern void DisZF16x8(uint8_t StartPage,uint8_t StartColumn,uint8_t Value,uint8_t Color);

extern void DisOneDigital16x8(uint8_t StartPage,uint8_t StartColumn,uint8_t  Value,uint8_t Color);
//extern void DisTwoDigital16x8(uint8_t StartPage,uint8_t StartColumn,uint8_t  Value,uint8_t Color);
extern void DisBcdDigital16x8(uint8_t StartPage,uint8_t StartColumn,uint8_t  Value,uint8_t Color);
extern void DisBcdDigital32x20(uint8_t StartPage,uint8_t StartColumn,uint8_t  Value,uint8_t Color);

extern void DisDigital16x8Str(uint8_t StartPage,uint8_t StartColumn,uint16_t  Value,uint8_t Color);
extern void DisEN16x8Str(uint8_t StartPage,uint8_t StartColumn,const uint8_t *BUFF,uint8_t Color);
//extern void DisEN16x8StrConst(uint8_t StartPage,uint8_t StartColumn,uint8_t const *BUFF,uint8_t Color);
extern void DisImage(uint8_t StartPage,uint8_t StartColumn,uint8_t Width,uint8_t Heigth,const uint8_t *BUFF,uint8_t Color);
extern void DisImage_RAM(uint8_t StartPage,uint8_t StartColumn,uint8_t Width,uint8_t Heigth,uint8_t *BUFF,uint8_t Color);

#endif
