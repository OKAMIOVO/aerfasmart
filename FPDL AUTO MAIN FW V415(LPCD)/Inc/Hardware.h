#ifndef __Hardware__
#define __Hardware__	1

#include "Project.h"



/* ***************************************************************************
*  PUBLIC FUNCTION PROTOTYPES
*/
void      intoff(void);           /* Interrupt Control */
void      inton(void);
void      do_timer(void);
void      hand_PEIE(void);        /* Peripheral Int Handler   */
void      hand_PORTB(void);       /* port B change int */
void      Hardware_Init(void);

//AD+ *********************************************************************
void     Hardware_A2D_Init( void);
uint16_t Hardware_A2D_Fetch( uint8_t channel);
void     Hardware_Task_Analog_Battery( void);

extern void Hardware_Task_Analog_Imotor( void);

extern void     Hardware_A2D_Start( uint8_t channel);
extern uint16_t Hardware_A2D_Read( void);

extern void  	Hardware_DelayMs(uint16_t time);
extern void 	Hardware_DelayX1us(uint16_t time);
extern void 	Hardware_DelayX5us(uint16_t time);


extern void Hardware_Init_IO( void);

extern void Hardware_CCP1_Init(void);
extern void Hardware_SETPWM_DUTY(uint8_t Duty);

extern uint8_t HardwareGet_TargetSpeed(void);

extern uint8_t Read_EE_Data(uint8_t Addr);
extern void Write_EE_Data(uint8_t Addr,uint8_t Data);
extern uint16_t HardwareGet_CurrentSpeed(void);
extern void PWM_Init(void);
extern void Hardware_SetBacklightPWM_(uint8_t pwmduty);
extern status_t UART1_Write_TxBUFFER(unsigned char *buff,unsigned char len);
extern void UART1_TX_Task(void);
extern void UART1_CLEAR_TXBUFF(void);

extern void HardwareBatteryMgr_Task(void);
extern void HardwareBatteryTypeAutoSwitch(void);


extern void I2C_Init(void);
extern void SPI1_Init(void);
extern void SPI2_Init(void);

extern void Hardware_AwakeLine_SleepStatus(void);
extern void Hardware_AwakeLine_WorkingStatus(void);
extern void Hardware_Awake_Driver(void);
#endif
