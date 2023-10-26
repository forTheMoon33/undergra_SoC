/*
 * main implementation: use this 'C' sample to create your own application
 *
 */
#include "INKEY.h"
#include "derivative.h" /* include peripheral declarations */

#define PWM_CLK_FREQ 20970000
#define M1 (unsigned short)(PWM_CLK_FREQ/523)/2 
#define M4 (unsigned short)(PWM_CLK_FREQ/698)/2
#define L1 M1*2
#define H1 M1/2

unsigned int ADC_Result;
unsigned char show[5];
unsigned char AD_flag;
unsigned int ShowMode=2;

unsigned int Error=0;

extern unsigned int flag_2s;
extern unsigned int flag_100ms;

int T=0;				//Temperature
int upperlimit=100;
int lowerlimit=0;
int mode=0;
int weight=1;
int shownumber;
int Heat_flag=0;
int End_flag=0;

int time[2];
extern unsigned long int led[6];  //,BIT15,BIT16,BIT17
extern unsigned char num[10];
extern int counttime; 
extern unsigned int number;

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

void ADC1_Init (void)  {
	//enable ADC0  clock
	SIM_SCGC3|=SIM_SCGC3_ADC1_MASK;
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;
     // Set pin0 of PORTE as analog function 
	PORTE_PCR0=0x0000;
	//PORTE_PCR0=PORT_PCR_MUX(0X0);
	//long sample time single-end 12bit conversion
	//ADC1_CFG1=0X00000014;
	ADC1_CFG1=ADC_CFG1_ADLSMP_MASK+ADC_CFG1_MODE(1);	
	//ADxxxa channel select
	ADC1_CFG2=0X00000000;
	//default voltage reference Vrefh  and Vrefl,software trigger
	ADC1_SC2=0X00000000;
	//continuous conversions
	//ADC1_SC3=0X00000008; 
	ADC1_SC3=ADC_SC3_ADCO_MASK; 
	//interrupt disable and select ADC0_SE4a channel as input
	//ADC1_SC1A=0X0000004;
	ADC1_SC1A=ADC_SC1_ADCH(5);		
}

void ADC_Init (void)  {
	/*enable ADC0  clock*/
	SIM_SCGC3|=SIM_SCGC3_ADC1_MASK;
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;
     /* Set pin0 of PORTE as analog function */
	PORTE_PCR0=0x0000;
	//PORTE_PCR0=PORT_PCR_MUX(0X0);
	//long sample time single-end 12bit conversion
	ADC1_CFG1=0X00000014;
	//	ADC1_CFG1=ADC_CFG1_ADLSMP_MASK+ADC_CFG1_MODE(1);	
	//ADxxxa channel select
	ADC1_CFG2=0X00000000;
	//default voltage reference Vrefh  and Vrefl,software trigger
	ADC1_SC2=0X00000000;
	//continuous conversions
	ADC1_SC3=0X00000008; 
	//ADC1_SC3=ADC_SC3_ADCO_MASK; 
	//interrupt disable and select ADC0_SE4a channel as input
	ADC1_SC1A=0X0000004;
	//ADC1_SC1A=ADC_SC1_ADCH(4);		
}

void LED_4_Init(void)
{
	SIM_SCGC5|=0X800;   //使能PORTC时钟
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

void Beep_Init(void)
{
  //enable FTM2 clock
  SIM_SCGC3|=SIM_SCGC3_FTM2_MASK;
  SIM_SCGC5|=SIM_SCGC5_PORTB_MASK;
  
  //PTA13 is set to FTM2 pin
  PORTB_PCR18 = PORT_PCR_MUX(0x3);
  
}

//TPM1 module initialisation
void FTM2_Init(void)
{  
	FTM2_CNT = 0;
	FTM2_MOD = 0x2BC0;                                  //T
	FTM2_CNTIN=0x00;
	FTM2_SC = FTM_SC_CLKS(1);
	FTM2_C0SC = FTM_CnSC_ELSB_MASK+FTM_CnSC_MSB_MASK;   //
	//determine the Beep start or not
	FTM2_C0V = 0x00;     //initial	
}

//setup the frequency of the beep
void Beep_setup(unsigned int count)
{
	FTM2_MOD = count*2;                                      //T
	FTM2_C0V = count;                                    //initial	
}

void delay(void)
{  
	unsigned short i,j;
		for(i=0;i<3000;i++) 
			for(j=0;j<200; j++)
				asm("nop");
}

void delayshow(t)
{  
	unsigned short i,j;
		for(i=0;i<3000;i++) 
			for(j=0;j<t; j++)
				asm("nop");
}

void Display(void)
{
	int i=0;
	if(Heat_flag==0)
	{
		if(mode==1)
		{
			upperlimit+=weight;
		}
		else if(mode==2)
		{
			upperlimit-=weight;
		}
		else if(mode==3)
		{
			lowerlimit+=weight;
		}
		else if(mode==4)
		{
			lowerlimit-=weight;
		}
		
		if((mode==1)||(mode==2))
		{
			if(lowerlimit>=upperlimit)
			{
				upperlimit=lowerlimit+1;
				Error=1;
			}
			
		}
		else if((mode==3)||(mode==4))
		{
			if(lowerlimit>=upperlimit)
			{
				lowerlimit=upperlimit-1;
				Error=1;
			}
			if(lowerlimit<0)
			{
				lowerlimit=0;
				Error=1;
			}
			
		}
		
		if(mode>0)
		{
			if((mode==1)||(mode==2))
			{
				shownumber=upperlimit;
			}
			else if((mode==3)||(mode==4))
			{
				shownumber=lowerlimit;
			}
		}
		else if(mode==0)
		{
			ADC_Result=ADC1_RA;
			ADC_Result=(ADC_Result-120)/20;

			shownumber=ADC_Result;
			
		}
	}
	else if(Heat_flag==1)
	{
		shownumber=T;
	}
	
	for(i=0;i<3;i++)
	{
		show[i]=shownumber%10;
		shownumber/=10;
	}
	weight=0;
}

void React(void)
{
	if(Heat_flag==0)
	{
		T=(ADC1_RA-120)/20;
	}
	GPIOC_PDOR=0x0FFF;
	if(Error==1)
	{
		GPIOC_PDOR&=~(0x01|0x08|0x40|0x200|0x02|0x10|0x80|0x400);
		Beep_setup(M4);
		delay();
		Error=0;
	}
	else if(T>upperlimit)
	{
		GPIOC_PDOR&=~(0x01|0x08|0x40|0x200);
		Beep_setup(M1);
		delay();
	}
	else if(T<lowerlimit)
	{
		GPIOC_PDOR&=~(0x04|0x20|0x100|0x800);
		Beep_setup(L1);
		delay();
	}	
	else if((Heat_flag==1)&&(End_flag==0))
	{
		GPIOC_PDOR&=~(0x01|0x08|0x40|0x200|0x04|0x20|0x100|0x800);
		Beep_setup(0);
		delay();
	}
	else if((Heat_flag==1)&&(End_flag==1))
	{
		GPIOC_PDOR=0x0FFF;
		GPIOC_PDOR&=~(0x07|0x38|0x1C0|0xE00);
		delay();
		Beep_setup(0);
	}
	else 
	{
		GPIOC_PDOR&=~(0x02|0x10|0x80|0x400);
		Beep_setup(0);
		delay();
	}
		
}

void Detect(void)
{
	if(AD_flag)
	{
		while((ADC1_SC1A&ADC_SC1_COCO_MASK)!=ADC_SC1_COCO_MASK);
		AD_flag=0;
		Display();
		React();
		
		//clear flag	   	
		ADC1_SC1A&=~ADC_SC1_COCO_MASK;
	}
}

void Heat(void)
{
	int remeb;
	if(T<upperlimit)
	{
		T++;
		delayshow(100);
		Display();
		React();
	}
	else if(T>=upperlimit)
	{
		if(End_flag==0)
		{
			remeb=counttime;
			Beep_setup(H1);
		}
			
		number=0;
		End_flag=1;
		React();
		Display();
		if((counttime-remeb)>5)
		{
			Heat_flag=0;
			End_flag=0;
		}
			
	}
}

int main(void)
{
	Beep_Init();
	FTM2_Init();
	LED_4_Init();
	LED_Disp_Init();
	PIT_init(0x01);	     //1ms interval interrupt	
	NVICICPR2=1<<(68%32);
	NVICISER2=1<<(68%32);
	KEY_Init();
	EN_Init();
	
	AD_flag=0;
	
/*	
	if(ShowMode==1)
	{
		ADC1_Init();
		for(;;)
		{
			if(AD_flag)
			{
				while((ADC1_SC1A&ADC_SC1_COCO_MASK)!=ADC_SC1_COCO_MASK);
				AD_flag=0;
				ADC_Result=ADC1_RA;
				ADC_Result/=100;
				show[0]=ADC_Result%10;
				show[1]=ADC_Result/10;
				//clear flag	   	
				ADC1_SC1A&=~ADC_SC1_COCO_MASK;
				
			}
		}
	}
*/
	if(ShowMode==2)
	{
		ADC_Init();
		for(;;)
		{
			if(Heat_flag==0)
				Detect();
			else if(Heat_flag==1)
				Heat();
		}
	}
}
