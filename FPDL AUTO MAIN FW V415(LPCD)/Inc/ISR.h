#ifndef __ISR__
#define __ISR__	1

#include "StdTypes.h"

extern uint8_t tempvoicestatusvalue;

extern uint8_t UART2_RXBUFFER[10];
extern uint8_t UART1_RXBUFFER[10];
extern uint8_t UART3_RXBUFFER[10];


extern Uart3Mgr_t UART3_Mgr;

extern void TIM3_ISR(void);
extern void TIM6_ISR(void);
extern void SysTick_ISR(void);
extern void UART1_ISR(void);
extern void UART2_ISR(void);

#endif
