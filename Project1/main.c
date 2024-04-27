/*
* Project1.c
*
* Created: 04/03/2019
* Author: Reece Gavin 17197589
*/


#include <avr/io.h>
#include <avr/interrupt.h>

unsigned int delay; //Variable used to change the delay time
unsigned int timecount0; //Create a variable used for the overflow count
volatile unsigned int adc_reading;	/* Defined ADC as a global.*/
#define COUNT_FOR_10ms 5


int main(void)
{
	/* Set PortD to all outputs because LEDs are connected to this PORT */
	DDRD = 0xff;	/* 0xff = 0b11111111; all ones						*/
	DDRB=0x00;
	PORTB = 0b00110000; // Enable Programmable pull-ups on bits 5 and 4 on port B
	PCMSK0 = 0b00110000; // Pin change mask register set to bits 5 and 4
	PCICR = 0b00000001; //Pin change Interrupt control register set to 1 
	
	timecount0 = 0;		/* Initialise the overflow count. Note its scope	*/
	TCCR0B = (5<<CS00);	/* Set T0 Source = Clock (16MHz)/1024 and put Timer in Normal mode	*/
	
	TCCR0A = 0;			/* Not strictly necessary as these are the reset states but it's good	*/
	/* practice to show what you're doing									*/
	TCNT0 = 61;			/* Recall: 256-61 = 195 & 195*64us = 12.48ms, approx 12.5ms		*/
	TIMSK0 = (1<<TOIE0);		/* Enable Timer 0 interrupt										*/
	
	ADMUX = ((1<<REFS0) | (0<<MUX0));  /* AVCC selected for VREF, ADC channel 0 as ADC input  */
	ADCSRA = ((1<<ADEN)|(1<<ADSC)|(1<<ADATE)|(1<<ADIE)|(7<<ADPS0));
	ADCSRB = (0<<ADTS0); //Select AutoTrigger Source to Free Running Mode
	/* Enable ADC,Start Conversion,Auto Trigger enabled,
	Interrupt enabled, Prescaler = 128  */
	
	sei();						/* Global interrupt enable (I=1)								*/
	
	
	while(1); //Do nothing while loop
	
}


ISR (ADC_vect)//handles ADC interrupts
{


	adc_reading = ADC;   /* ADC is in Free Running Mode */
	
	
	if (adc_reading  >=0 && adc_reading <=512)
	{
		delay=20; //If adc reading is between 0 and 2.5V, the speed is 0.25 S
	}
	else
	{
		delay=80; //Otherwise the speed is 1S
	}
	
}


ISR(TIMER0_OVF_vect)
{
	TCNT0 = 61;		//	TCNT0 needs to be set to the start point each time
	++timecount0;			//count the number of times the interrupt has been reached
	
	
	if (timecount0 >= COUNT_FOR_10ms)	//	500 * 2ms = 1s
	{

		
		if((PINB & 0b00110000) == 0b00110000) //If no push buttons are pressed. If Pin B bit 5==1 & 4==1;
		
		{
			
			static int i;
			static int j;
			
			if(i==0)
			{
				
				PORTD=1;
				i=1;
			}
			else if (i<128 && j==0)
			{
				PORTD=i<<1;
				i=i<<1;
			}
			else if(i>1 && j==1)
			{
				PORTD=i>>1;
				i=i>>1;
			}
			
			if(i==128)
			{
				j=1;
			}
			else if(i==1)
			{
				j=0;
			}
			timecount0=0;
		}
		else if((PINB & 0b00010000) == 0b00000000) //If Push button S1 is pressed . If Pin B bit 5==1  & bit 4 == 0
		{
			
			int a = adc_reading;
			//goes through each increment of adc_reading and and changes portd to value it should 
			if(0>=a && a<128)
			PORTD = 0x00;
			else if(a>=128 && a<256)
			PORTD = 0x01;
			else if(a>=256 && a<384)
			PORTD = 0x03;
			else if(a>=384 && a<512)
			PORTD = 0x07;
			else if(a>=512 && a<640)
			PORTD = 0x0f;
			else if(a>=640 && a<768)
			PORTD = 0x1f;
			else if(a>=768 && a<896)
			PORTD = 0x3f;
			//A lower value than 1024 was used as 1024 was never fully reached with the potentiometer
			else if(a>=896 && a<1015)
			PORTD = 0x7f;
			else if(a>=1015)
			PORTD = 0xff;
			timecount0=0;
		}
		
		
		else if ((PINB & 0b00000000) == 0b00000000) //If push button pin S2 is pressed. If Pin B bit 5==0 
		{
			int halfd=0x00; //Two variables created in order to have half of PortD allocated to bits 7-4 and half to bits 3-0
			int halfd2=0x00;		
			static int i;
			static int j;
			
			//Have Cylon eyes show on bits 3-0 on PORTD
			if(i==0)
			{
				
				halfd=1;
				i=1;
			}
			else if (i<8 && j==0)
			{
				halfd=i<<1;
				i=i<<1;
			}
			else if(i>1 && j==1)
			{
				halfd=i>>1;
				i=i>>1;
			}
			
			if(i==8)
			{
				j=1;
			}
			else if(i==1)
			{
				j=0;
			}
			
			
			//Have upper half of the ADC values show on bits 7-4 on PORTD
			if(0>=adc_reading && adc_reading<256)
			halfd2 = 0x00;
			else if(adc_reading>=256 && adc_reading<512)
			halfd2 = 0x10;
			else if(adc_reading>=512 && adc_reading<768)
			halfd2 = 0x30;
			//had to use slightly lower value than 1024 as 1024 was never fully reached with the potentiometer
			else if(adc_reading>=768 && adc_reading<1015)
			halfd2 = 0x70;
			else if(adc_reading>=1015)
			halfd2 = 0xf0;
			//add the two halfs of portd together to display
			PORTD = halfd + halfd2;
			timecount0=0;
		}

	}
	
}


