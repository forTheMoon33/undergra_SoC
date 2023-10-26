/*
 *    kinetis_sysinit.c - Default init routines for P2
 *                     		Kinetis ARM systems
 *    Copyright 闁跨噦鎷�12 Freescale semiConductor Inc. All Rights Reserved.
 */
#include "kinetis_sysinit.h"
#include "derivative.h"
#include "INKEY.h" 
#include "LED_CODE.h"

extern int ChosenNum;
extern int Startflag;
extern int Endflag;
extern int level;
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

/**
 **===========================================================================
 **  Default interrupt handler
 **===========================================================================
 */
void Default_Handler()
{
	__asm("bkpt");
}
void PORTE_Handler()									
{		
	if(Startflag==0)
	{
		if(PORTE_ISFR&0x04)
		{
			int m;
			for(m=0;m<250;m++)
			{
				GPIOA_PDOR |= 0x03F000;	
				GPIOD_PDOR &= ~0xFF;
				GPIOD_PDOR = LED_0;
				GPIOA_PDOR &= Select_LED0; 
				delay(50);
				GPIOA_PDOR |= 0x03F000;
				GPIOD_PDOR &= ~0xFF;
				GPIOD_PDOR = 0x21;
				GPIOA_PDOR &= Select_LED1;
				delay(50);
			}
			Startflag=1;
			Endflag=1;
			level=0;
		}
			
	}		
	else if(Startflag==1)
	{
		if(PORTE_ISFR&0x04)
		{
			ChosenNum=1;                              
		}
		else if(PORTE_ISFR&0x08)
		{
			ChosenNum=2;                               
		}
		else if(PORTE_ISFR&0x10)
		{
			ChosenNum=3;                               
		}
		else if(PORTE_ISFR&0x20)
		{
			ChosenNum=4;                               
		} 
	}
	PORTE_ISFR|=(0x04|0x08|0x10|0x20);       


}

void PORTB_Handler()									
{
	if(Startflag==1)
	{
		if(PORTB_ISFR&0x20000)
		{
			ChosenNum=5;                                
		}
		else if(PORTB_ISFR&0x10000)
		{
			ChosenNum=6;                                
		}
		else if(PORTB_ISFR&0x800)
		{
			ChosenNum=7;                               
		}
		else if(PORTB_ISFR&0x400)
		{
			ChosenNum=8;                                
		}
	}
  
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
    Default_Handler,   //0
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
    Default_Handler,  //20
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
    Default_Handler,    //40
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
    Default_Handler,  //60
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
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
