/*
 * main implementation: use this 'C' sample to create your own application
 *
 */
#include "derivative.h" /* include peripheral declarations */
#include "INKEY.h"
#include "LED_CODE.h"
#include <stdlib.h>  

unsigned char NumCode[10]={0xA0,0xBE,0x62,0x2A,0x3C,0x29,0x21,0xBA,0x20,0x28};
int ChosenNum=0;
int Startflag=0;
int level=0;
int Endflag=1;

void LED_Disp_Init(void) 
{
	SIM_SCGC5 |=(0x1000 | 0x0200);
	/*portA  set to GPIO*/	    
	PORTA_PCR12=0x100;
	PORTA_PCR13=0x100;
	PORTA_PCR14=0x100;
	PORTA_PCR15=0x100;
	PORTA_PCR16=0x100;
	PORTA_PCR17=0x100;

	/*portD  set to GPIO*/
	PORTD_PCR0 = 0x0100;
	PORTD_PCR1 = 0x0100;
	PORTD_PCR2 = 0x0100;
	PORTD_PCR3 = 0x0100;
	PORTD_PCR4 = 0x0100;
	PORTD_PCR5 = 0x0100;
	PORTD_PCR6 = 0x0100;		
	PORTD_PCR7 = 0x0100;
	
	/*LED bit pin set to out*/
	GPIOD_PDDR |=0x00FF;
	GPIOA_PDDR |=0x03F000;
	/*LED control pin set to high ,LEDs are closed*/
	GPIOA_PDOR |=0x03F000;

	/*LED bit pin set to high ,LED bit are all off, change GPIOD_PDOR register can change the number it shows*/
	GPIOD_PDOR = 0xFF;
}


void delay(int t)
{  
	unsigned short a,b;
   for(a=0;a<100; a++)
	   for(b=0;b<t;b++)
		   asm("nop");
}


int main(void)
{
	LED_Disp_Init();
	KEY_Init();
	EN_Init();
	int Select_LED;

	int CurrentNum;
	int ShowTime;
	int EndNum[2]={0,0};
	int m;
	int counter=0;
	    
	
	for(;;)
	{
		if(Startflag==1)
		{
			if(Endflag==1)
			{
				srand(counter++);
				Select_LED=Select_LED0;
				CurrentNum=(rand()%7)+1;
				ShowTime=10000-400*level;
				for(m=0;m<6;m++)
				{
					GPIOA_PDOR |= 0x03F000;	
					GPIOD_PDOR = NumCode[CurrentNum];
					GPIOA_PDOR &= Select_LED; 
					delay(ShowTime-150*m);
					Select_LED=Select_LED<<1;
					Select_LED++;
					if(ChosenNum==CurrentNum)
					{
						Endflag=1;				
						GPIOD_PDOR = NumCode[CurrentNum];
						delay(5000);
						ChosenNum=0;
						break;
					}	
				}
				Select_LED--;
				Select_LED=Select_LED>>1;							
				GPIOA_PDOR &= Select_LED; 
				delay(8000);
					
				level++;
				if((m==6)||(level==20))
				{
					Endflag=0;
					if(level==20)
						level=21;
				}
					
			}
			else if(Endflag==0)
			{
				level--;
				EndNum[0]=level%10;
				EndNum[1]=level/10;
				
				for(m=0;m<500;m++)
				{
					GPIOA_PDOR |= 0x03F000;	
					GPIOD_PDOR &= ~0xFF;
					GPIOD_PDOR = NumCode[EndNum[0]];
					GPIOA_PDOR &= Select_LED0; 
					delay(50);
					GPIOA_PDOR |= 0x03F000;	
					GPIOD_PDOR &= ~0xFF;
					GPIOD_PDOR = NumCode[EndNum[1]];
					GPIOA_PDOR &= Select_LED1;
					delay(50);
				}
				
				Startflag=0;
				GPIOA_PDOR |= 0x03F000;	
				GPIOD_PDOR &= ~0xFF;
			
			}		
		}
	}
}
