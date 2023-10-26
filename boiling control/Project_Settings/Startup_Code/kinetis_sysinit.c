/*
 *    kinetis_sysinit.c - Default init routines for P2
 *                     		Kinetis ARM systems
 *    Copyright �2012 Freescale semiConductor Inc. All Rights Reserved.
 */
 
#include "kinetis_sysinit.h"
#include "derivative.h"
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

unsigned int counttime=0;
unsigned int flag_1s=0;
unsigned int flag_2s=0;
unsigned int flag_100ms=0;
unsigned int number=0;
unsigned int numbertime=0;
extern int Heat_flag;
extern unsigned char show[5];
extern unsigned char AD_flag;
extern unsigned int End_flag;
extern unsigned int ShowMode;
extern int time[2];


extern int T;				//Temperature
extern int upperlimit;
extern int lowerlimit;
extern int mode;
extern int weight;


unsigned long int led[6]={BIT12,BIT13,BIT14,BIT15,BIT16,BIT17};
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
	if((Heat_flag==1)&&(End_flag==0))
	{
		show[3]=counttime%10;
		show[4]=counttime/10;
		GPIOA_PDOR|=0xFFFFFF; 
		GPIOD_PDOR=num[show[number]];					
		GPIOA_PDOR&=~(led[number]);
		number++;
		if(number==5)
			number=0;
	}
	else //if((Heat_flag==0)||(End_flag==1))
	{
		GPIOA_PDOR|=0xFFFFFF; 
		GPIOD_PDOR=num[show[number]];					
		GPIOA_PDOR&=~(led[number]);
		number++;	
		if(number==3)
			number=0;
	}
	
	flag_1s++;
	flag_2s++;	
	flag_100ms++;
	
	if(Heat_flag==0)
	{
		flag_1s=0;
	}
	if(mode==0)
	{
		if(flag_2s==2000)
		{
			flag_2s=0;
			AD_flag=1;
		}
		flag_100ms=0;
		
	}
	else if(mode>0)
	{
		if(flag_100ms==100)
		{
			flag_100ms=0;
			AD_flag=1;
		}
		flag_2s=0;
		
	}
	if(flag_1s==1500)
	{
		flag_1s=0;
		counttime++;
	}
		
	
	
	/*Clear the flag of channel0,PIT*/
	PIT_TFLG0|=PIT_TFLG_TIF_MASK;
}

void PORTE_Handler()
{	
	if(PORTE_ISFR&0x04)
	{
		mode=1;
	}
	else if(PORTE_ISFR&0x08)
	{
		mode=2;
	}
	else if(PORTE_ISFR&0x10)
	{
		mode=3;
	}
	else if(PORTE_ISFR&0x20)
	{		
		mode=4;
	}            
	
	PORTE_ISFR|=(0x04|0x08|0x10|0x20);     

}

void PORTB_Handler()								
{
	if(PORTB_ISFR&0x20000)
	{
		weight=1;
	}
	else if(PORTB_ISFR&0x10000)
	{
		weight=5;
	}
	else if(PORTB_ISFR&0x800)
	{
		weight=10;
	}
	else if(PORTB_ISFR&0x400)
	{
		if((mode==0)&&(T<upperlimit)&&(Heat_flag==0))
		{
			Heat_flag=1;
			counttime=0;
		}
			
		else if (Heat_flag==1)
		{
			Heat_flag=0;
			number=0;
			End_flag=0;
		}
			
		else if(mode>0)
			mode=0;
	}
	delay();
	PORTB_ISFR|=(0x400|0x800|0x10000|0x20000);      
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
    
    //Default_Handler,
    PORTB_Handler,
    Default_Handler,  
    Default_Handler,   //90
    PORTE_Handler,	  //91  PORTE Interrupt
   // Default_Handler,  	
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
