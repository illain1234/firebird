/*
 * main.c
 *
 * Created: 3/30/2023 10:45:03 PM
 *  Author: ajayr
 */ 

#define F_CPU 14745600

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>
#include <lcd.c>

unsigned char ADC_Conversion(unsigned char);
unsigned char ADC_Value;
unsigned char sharp, distance, adc_reading;
unsigned int value;




unsigned char ADC_Conversion(unsigned char Ch)
{
	unsigned char a;
	if(Ch>7)
	{
		ADCSRB = 0x08;
	}
	Ch = Ch & 0x07;
	ADMUX= 0x20| Ch;
	ADCSRA = ADCSRA | 0x40;
	while((ADCSRA&0x10)==0);
	a=ADCH;
	ADCSRA = ADCSRA|0x10;
	ADCSRB = 0x00;
	return a;
}




void print_sensor(char row, char coloumn,unsigned char channel)
{
	ADC_Value = ADC_Conversion(channel);
	
}


unsigned int Sharp_GP2D12_estimation(unsigned char adc_reading)
{
	float distance;
	unsigned int distanceInt;
	distance = (int)(10.00*(2799.6*(1.00/(pow(adc_reading,1.1546)))));
	distanceInt = (int)distance;
	if(distanceInt>800)
	{
		distanceInt=800;
	}
	return distanceInt;
}




int main()
{
	DDRA    =0x0F;
	PORTA   =0x0F;
	DDRL    =0x18;
	PORTL   =0x18;
	DDRF	= 0x00;
	PORTF	= 0x00;
	DDRK	= 0x00;
	PORTK	= 0x00;
	lcd_port_config();
	lcd_init();
	ADCSRA = 0x00;
	ADCSRB = 0x00;
	ADMUX  = 0x20;
	
	ACSR   = 0x80;
	ADCSRA = 0x86;
	unsigned int value;
	lcd_set_4bit();
	
	
	while(1)
	{
		
		

		sharp = ADC_Conversion(11);						//Stores the Analog value of front sharp connected to ADC channel 11 into variable "sharp"
		value = Sharp_GP2D12_estimation(sharp);			//Stores Distance calsulated in a variable "value".
		lcd_numeric_value(2, 5, value, 3);  			//Prints Value Of Distanc in MM measured by Sharp Sensor.
		PORTA=0x06;
		if(value>=100 && value<200)
		{
			PORTA=0x00;
			lcd_string(1, 1,"Obstacle is Near:");
			lcd_string(2, 1,":rotating");
			_delay_ms(1000);
			PORTA=0x08;
			_delay_ms(1000);
			
		}
		if(value>=200 && value<500)
		{
			
			lcd_string(1, 1,"Obstacle is Far: ");
			lcd_string(2, 1,"    ");
			
		}
		
		if(value>=500 && value<800)
		{
			
			lcd_string(1, 1,"Obstacle is Very ");
			lcd_string(2, 1,"Far:");
			
		}
		
		
	}
}
