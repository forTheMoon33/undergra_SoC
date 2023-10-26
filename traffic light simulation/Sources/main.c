/*
 * main implementation: use this 'C' sample to create your own application
 *
 */
#include "derivative.h" /* include peripheral declarations */

unsigned int ADC_Result;
unsigned char show[3];
unsigned int Colorflag=0;
//extern unsigned long int led[3]={BIT12,BIT13,BIT14};  //,BIT15,BIT16,BIT17
//unsigned char num[10]={0xA0,0xBE,0x62,0x2A,0x3C,0x29,0x21,0xBA,0x20,0x28};
void LED_4_Init()
{
	SIM_SCGC5|=0X800;   //使能PORTE时钟
	PORTC_PCR0=0x100;	//LED1红灯对应引脚设置为GPIO
	PORTC_PCR1=0x100;	//LED1绿灯对应引脚设置为GPIO
	PORTC_PCR2=0x100;	//LED1蓝灯对应引脚设置为GPIO
	PORTC_PCR3=0x100;	//LED2红灯对应引脚设置为GPIO
	PORTC_PCR4=0x100;	//LED2绿灯对应引脚设置为GPIO
	PORTC_PCR5=0x100;	//LED2蓝灯对应引脚设置为GPIO
	PORTC_PCR6=0x100;	//LED3红灯对应引脚设置为GPIO
	PORTC_PCR7=0x100;	//LED3绿灯对应引脚设置为GPIO
	PORTC_PCR8=0x100;	//LED3蓝灯对应引脚设置为GPIO
	PORTC_PCR9=0x100;	//LED4红灯对应引脚设置为GPIO
	PORTC_PCR10=0x100;	//LED4绿灯对应引脚设置为GPIO
	PORTC_PCR11=0x100;	//LED4蓝灯对应引脚设置为GPIO
	
	
	GPIOC_PDDR|=0x0FFF;   //对应端口设置为输出
	GPIOC_PDOR|=0x0FFF;    //四个小灯全灭 
}

void PIT_init(unsigned int number_ms)
{
	/*Trun on PIT clocks*/
	SIM_SCGC6|=SIM_SCGC6_PIT_MASK;
	
    /*Enable PIT Module*/
	PIT_MCR&=~(PIT_MCR_MDIS_MASK);
	
	/*Setup the channel0 of PIT*/
	PIT_LDVAL0=20000*number_ms;
	
	/*Enable the IRQ of channel0,PIT*/
	PIT_TCTRL0|=PIT_TCTRL_TIE_MASK;
	
	/*Running channel0,PIT*/
	PIT_TCTRL0|=PIT_TCTRL_TEN_MASK;	
}

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

void delay(void)
	{  
	   unsigned short i,j;
	   for(i=0;i<300;i++) 
	      for(j=0;j<20; j++)
	         asm("nop");
	}

int main(void)
{
	    LED_Disp_Init( );
	    LED_4_Init();
	    PIT_init(0x03E8);	     //1ms interval interrupt	
		NVICICPR2=1<<(68%32);
	    NVICISER2=1<<(68%32);


	    int counter=0;
	    
		for(;;)
		{
			counter++;
		}
		
}
