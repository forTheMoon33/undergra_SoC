/*
 *    kinetis_sysinit.c - Default init routines for P2
 *                     		Kinetis ARM systems
 *    Copyright 锟�012 Freescale semiConductor Inc. All Rights Reserved.
 */
 
#include "kinetis_sysinit.h"
#include "derivative.h"

#define Select_LED0 	0xFFFFEFFF		//1111 1110
#define Select_LED1	    0xFFFFDFFF		//1111 1101
#define Select_LED2	    0xFFFFBFFF		//1111 1011
#define Select_LED3 	0xFFFF7FFF		//1111 0111
#define Select_LED4	    0xFFFEFFFF		//1110 1111
#define Select_LED5	    0xFFFDFFFF		//1101 1111


/**
 **===========================================================================
 **  External declarations
 **===========================================================================
 */
#if __cplusplus
extern "C" {
#endif
extern uint32_t __vector_table[];
extern unsigned long _estack;
extern void __thumb_startup(void);
#if __cplusplus
}
#endif

unsigned int flag_100ms=0;
unsigned char flag_measure;
unsigned int number=6;				//初始设置计时数为6

int flag_1s0;

extern unsigned int Colorflag;

extern unsigned char show[3];
unsigned char num[10]={0xA0,0xBE,0x62,0x2A,0x3C,0x29,0x21,0xBA,0x20,0x28};
/**
 **===========================================================================
 **  Default interrupt handler
 **===========================================================================
 */
void Default_Handler()
{
	__asm("bkpt");
}
void delayms(unsigned int number)
{
  unsigned short i,j;
  for(i=0;i<number;i++)
  {
	  for(j=0;j<100;j++)
		  asm("nop");
  }
}
void pit_channel0_ISR(void)
{	
	//show number	    
	if(Colorflag==0)							//纵向通行
	{
		GPIOC_PDOR|=0x0FFF;    					//四个小灯全灭
		GPIOC_PDOR&=~0x0489;
		for(flag_1s0=0;flag_1s0<100;flag_1s0++)	//每一个数显示一秒，中断周期为10ms
		{			
			GPIOA_PDOR|=0x03F000; 
			GPIOD_PDOR=num[number];					
			GPIOA_PDOR&=Select_LED5;			//两个数码管都显示
			GPIOA_PDOR&=Select_LED0;
			if(flag_1s0==99)
			{
				number--;						//计数减一
			}
		}
		
		if(number==0)							//计数结束
		{
			number=0;							//为下一段黄灯计时准备
			Colorflag=1;						//黄灯模式
		}
			
	}
	else if(Colorflag==1)						//黄灯
	{
		GPIOC_PDOR|=0x0FFF;    //四个小灯全灭
		GPIOC_PDOR&=~0x06DB;
		for(flag_1s0=0;flag_1s0<100;flag_1s0++)
		{			
			GPIOA_PDOR|=0x03F000; 
			GPIOD_PDOR=num[number];					
			GPIOA_PDOR&=Select_LED5;
			GPIOA_PDOR&=Select_LED0;
			if(flag_1s0==99)
			{
				number--;
			}
		}
		
		if(number==-1)
		{
			number=9;
			Colorflag=2;
		}
	}
	else if(Colorflag==2)						//横向通行
	{
		GPIOC_PDOR|=0x0FFF;    //四个小灯全灭
		GPIOC_PDOR&=~0x0252;
		for(flag_1s0=0;flag_1s0<100;flag_1s0++)
		{			
			GPIOA_PDOR|=0x03F000; 
			GPIOD_PDOR=num[number];					
			GPIOA_PDOR&=Select_LED5;
			GPIOA_PDOR&=Select_LED0;
			if(flag_1s0==99)
			{
				number--;
			}
		}
		
		if(number==0)
		{
			number=0;
			Colorflag=3;
		}
	}
	else if(Colorflag==3)						//黄灯
	{
		GPIOC_PDOR|=0x0FFF;    //四个小灯全灭
		GPIOC_PDOR&=~0x06DB;
		for(flag_1s0=0;flag_1s0<100;flag_1s0++)
		{			
			GPIOA_PDOR|=0x03F000; 
			GPIOD_PDOR=num[number];					
			GPIOA_PDOR&=Select_LED5;
			GPIOA_PDOR&=Select_LED0;
			if(flag_1s0==99)
			{
				number--;
			}
		}
		
		if(number==-1)
		{
			number=6;
			Colorflag=0;
		}
	}
		

	
	/*Clear the flag of channel0,PIT*/
	PIT_TFLG0|=PIT_TFLG_TIF_MASK;
}
/**
 **===========================================================================
 **  Reset handler
 **===========================================================================
 */
void __init_hardware()
{
	SCB_VTOR = (uint32_t)__vector_table; /* Set the interrupt vector table position */
	/*
		Disable the Watchdog because it may reset the core before entering main().
		There are 2 unlock words which shall be provided in sequence before
		accessing the control register.
	*/
	WDOG_UNLOCK = KINETIS_WDOG_UNLOCK_SEQ_1;
	WDOG_UNLOCK = KINETIS_WDOG_UNLOCK_SEQ_2;
	WDOG_STCTRLH = KINETIS_WDOG_DISABLED_CTRL;
}

/* Weak definitions of handlers point to Default_Handler if not implemented */
void NMI_Handler() __attribute__ ((weak, alias("Default_Handler")));
void HardFault_Handler() __attribute__ ((weak, alias("Default_Handler")));
void MemManage_Handler() __attribute__ ((weak, alias("Default_Handler")));
void BusFault_Handler() __attribute__ ((weak, alias("Default_Handler")));
void UsageFault_Handler() __attribute__ ((weak, alias("Default_Handler")));
void SVC_Handler() __attribute__ ((weak, alias("Default_Handler")));
void DebugMonitor_Handler() __attribute__ ((weak, alias("Default_Handler")));
void PendSV_Handler() __attribute__ ((weak, alias("Default_Handler")));
void SysTick_Handler() __attribute__ ((weak, alias("Default_Handler")));


/* The Interrupt Vector Table */
void (* const InterruptVector[])() __attribute__ ((section(".vectortable"))) = {
    /* Processor exceptions */
    (void(*)(void)) &_estack,
    __thumb_startup,
    NMI_Handler, 
    HardFault_Handler, 
    MemManage_Handler, 
    BusFault_Handler,
    UsageFault_Handler, 
    0, 
    0, 
    0, 
    0, 
    SVC_Handler, 
    DebugMonitor_Handler, 
    0,
    PendSV_Handler, 
    SysTick_Handler,

    /* Interrupts */
    Default_Handler,  //0
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,  //10
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,   //20
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,   //30
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,   //40
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,   //50
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,   //60
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
 //   Default_Handler,
    pit_channel0_ISR,
    Default_Handler,
    Default_Handler,   //70
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,   //80
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,	
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,	
    Default_Handler,
    Default_Handler,
    Default_Handler,		
};
