/*	BASIC INTERRUPT VECTOR TABLE FOR STM8 devices
 *	Copyright (c) 2007 STMicroelectronics
 */

typedef void @far (*interrupt_handler_t)(void);
//extern @far @interrupt void TLI_Interrupt(void);
extern @far @interrupt void Timer1_Interrupt(void);
extern @far @interrupt void Timer4_Interrupt(void);
extern @far @interrupt void UART3_RX_Interrupt(void);
extern @far @interrupt void PORTB_Interrupt(void);
extern @far @interrupt void PORTD_Interrupt(void);
extern @far @interrupt void EXTI2_Interrupt(void);
extern @far @interrupt void EXIT7_Interrupt(void);
extern @far @interrupt void I2C_Interrupt(void);

struct interrupt_vector {
	unsigned char interrupt_instruction;
	interrupt_handler_t interrupt_handler;
};

@far @interrupt void NonHandledInterrupt (void)
{
	/* in order to detect unexpected events during development, 
	   it is recommended to set a breakpoint on the following instruction
	*/
	return;
}

extern void _stext();     /* startup routine */

struct interrupt_vector const _vectab[] = {
	{0x82, (interrupt_handler_t)_stext}, /* reset */
	{0x82, NonHandledInterrupt}, 		/* trap,TRAP 软件中断 - - 0x00 8004  */
	{0x82, NonHandledInterrupt}, 		/* irq0,TLI 外部最高级中断 - - 0x00 8008  */
	{0x82, NonHandledInterrupt}, 		/* irq1,AWU 从停机模式自动唤醒 - 是 0x00 800C  */
	{0x82, NonHandledInterrupt}, 		/* irq2,CLK 时钟控制器 - - 0x00 8010  */
	{0x82, NonHandledInterrupt}, 		/* irq3,EXTI0 端口A外部中断 是 是(1)0x00 8014  */
	{0x82, NonHandledInterrupt}, 		/* irq4,EXTI1 端口B外部中断 是 是 0x00 8018  */
	{0x82, NonHandledInterrupt}, 		/* irq5,EXTI2 端口C外部中断 是 是 0x00 801C */
	{0x82, PORTB_Interrupt}, 				/* irq6,EXTI3 端口D外部中断 是 是 0x00 8020  */
	{0x82, PORTD_Interrupt}, 				/* irq7,EXTI4 端口E外部中断 是 是 0x00 8024  */
	{0x82, NonHandledInterrupt}, 		/* irq8, External interrupt 0 */
	{0x82, NonHandledInterrupt}, 		/* irq9, External interrupt 1 */
	{0x82, EXTI2_Interrupt}, 		/* irq10,External interrupt 2 */
	{0x82, NonHandledInterrupt}, 		/* irq11,External interrupt 3 */
	{0x82, NonHandledInterrupt}, 		/* irq12,External interrupt 4 */
	{0x82, NonHandledInterrupt}, 		/* irq13,External interrupt 5 */
	{0x82, NonHandledInterrupt}, 		/* irq14,External interrupt 6 */
	{0x82, EXIT7_Interrupt}, 		/* irq15,External interrupt 7 */
	{0x82, NonHandledInterrupt}, 		/* irq16,TIM3 定时器2 捕获/比较 - - 0x00 8048 */
	{0x82, NonHandledInterrupt}, 		/* irq17,保留 - - 0x00 804C */
	{0x82, NonHandledInterrupt}, 		/* irq18,保留 - - 0x00 8050 */
	{0x82, NonHandledInterrupt}, 		/* irq19,I2C中断 是是 0x00 8054 */
	{0x82, NonHandledInterrupt}, 		/* irq20,UART2 发送完成 - - 0x00 8058 */
	{0x82, NonHandledInterrupt}, 		/* irq21,UART2 接收寄存器数据满 - - 0x00 805C */
	{0x82, UART3_RX_Interrupt}, 		/* irq22,ADC1 ADC1转换结束/模拟看门狗中断 - - 0x00 8060 */
	{0x82, Timer1_Interrupt}, 		  /* irq23,TIM4 定时器4 更新/上溢出 - - 0x00 8064 */
	{0x82, NonHandledInterrupt}, 		/* irq24,FLASH EOP/WR_PG_DIS - - 0x00 8068 */
	{0x82, Timer4_Interrupt}, 			/* irq25,保留*/
	{0x82, NonHandledInterrupt}, 		/* irq26,保留*/
	{0x82, NonHandledInterrupt}, 		/* irq27,保留*/
	{0x82, NonHandledInterrupt}, 		/* irq28,保留*/
	{0x82, I2C_Interrupt}, 					/* irq29,保留*/
};


