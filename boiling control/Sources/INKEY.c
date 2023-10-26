/*
 * main implementation: use this 'C' sample to create your own application
 *
 */


#include "derivative.h" /* include peripheral declarations */
/*
 * INKEY.c
 *
 *  Created on: Dec 18, 2015
 *      Author: dell
 */

#include <derivative.h>



/*delay function*/


/*enable interrupt*/
void enable_irq(int irq)
{
	int div;
	/*Determinate which of the NVICISERs corresponds to the irq */
	div=irq/32;
	switch(div)
	{
	   case  0x0:
		   NVICICPR0=1<<(irq%32);
		   NVICISER0=1<<(irq%32);
		   break;
	   case  0x1:
		   NVICICPR1=1<<(irq%32);
		   NVICISER1=1<<(irq%32);
		   break;
	   case  0x2:
		   NVICICPR2=1<<(irq%32);
		   NVICISER2=1<<(irq%32);
		   break;		   
	}
}




/*enable initial*/
void EN_Init()
{
	enable_irq(88);	 //PORTB interrupt
	enable_irq(91);  //PORTE interrupt
}

/*KEY initial*/
void KEY_Init()
{
	/*enable PORTB、E  clock*/
	SIM_SCGC5|=(SIM_SCGC5_PORTE_MASK|SIM_SCGC5_PORTB_MASK);

	/*portE2、3、4、5，10、11、16、17  set to GPIO,raising edge interrupt,no pull enable,passive filter enable*/
	PORTE_PCR2=0x0A0103;	
	PORTE_PCR3=0x0A0103;
	PORTE_PCR4=0x0A0103;
	PORTE_PCR5=0x0A0103;
	
	PORTB_PCR10=0x0A0103;
	PORTB_PCR11=0x0A0103;
	PORTB_PCR16=0x0A0103;
	PORTB_PCR17=0x0A0103;
	
	//PORTE_PCR2=PORT_PCR_MUX(0X1)+PORT_PCR_IRQC(0X0A)+PORT_PCR_PE_MASK+PORT_PCR_PS_MASK;
	//PORTE_PCR3=PORT_PCR_MUX(0X1)+PORT_PCR_IRQC(0X0A)+PORT_PCR_PE_MASK+PORT_PCR_PS_MASK;	
	GPIOE_PDDR&= ~0x30C3C;  //八个按键端口设置为输入	
	//GPIOE_PDDR&=~0x3C;
}



