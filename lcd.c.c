#define F_CPU 14745600
#include <avr/io.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <lcd.h>
void buzzer_pin_config (void)
{
	DDRC = DDRC | 0x08;
	PORTC = PORTC & 0xF7;
}
void interrupt_switch_config (void)
{
	DDRE = DDRE &0x7F; // setting PORTE 7 as input =0x7F or 0x00
	PORTE = PORTE | 0x80; // PORTE 7 internal pull up enabled 0x80 0r 0xFF
}
void LED_bargraph_config (void)
{
	DDRJ =0xFF;
	PORTJ = 0x00;
}
void port_init (void)
{
	buzzer_pin_config();
	interrupt_switch_config();
	LED_bargraph_config();
}
void buzzer_on (void)
{
	unsigned char port_restore = 0;
	port_restore = PINC;
	port_restore = port_restore | 0x08;
	PORTC = port_restore;
}
void buzzer_off (void)
{
	unsigned char port_restore = 0;
	port_restore = PINC;
	port_restore = port_restore & 0xF7;
	PORTC = port_restore;
}
void init_devices (void)
{
	cli();
	port_init();
	sei();
}
int main(void)
{
	char ROLLNUMBER[50]="ECE19010209";
	char *p=ROLLNUMBER;
	char SURNAME[50]="KUMAR";
	char *q=SURNAME;
	char NAME[50]="SACHIN";
	char *t=NAME;
	while(1)
	{
		lcd_port_config();
		lcd_init();
		
		if((PINE & 0x80)==0x80)
		{
			buzzer_off();
			PORTJ=0x00;
		}
		else
		{
			buzzer_on();
			lcd_string(1,1,t);
			_delay_ms(1000);
			
			buzzer_on();
			lcd_string(1,8,q);
			_delay_ms(1000);
			
			buzzer_on();
			lcd_string(2,1,p);
			_delay_ms(2000);
		}
	}
}
