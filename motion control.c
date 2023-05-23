/*
 * main.c
 *
 * Created: 3/29/2023 6:15:49 PM
 *  Author: ajayr
 */ 
#define F_CPU 14745600
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


void motion_pin_config (void)
{
	DDRA = DDRA | 0x0F;
	PORTA = PORTA & 0xF0;
	DDRL = DDRL | 0x18;
	PORTL = PORTL | 0x18;
}
void LED_bargraph_config (void)
{
	DDRJ =0xFF;
	PORTJ = 0x00;
}

void port_init()
{
	motion_pin_config();
	LED_bargraph_config();
}
void motion_set (unsigned char Direction)
{
	unsigned char PortARestore = 0;


	Direction &= 0x0F;
	PortARestore = PORTA;
	PortARestore &= 0xF0;
	PortARestore |= Direction;
	PORTA = PortARestore;
}


void forward (void)
{
	motion_set(0x06);
}

void back (void)
{
	motion_set(0x09);
}

void left (void)
{
	motion_set(0x05);
}

void right (void)
{
	motion_set(0x0A);
}

void soft_left (void)
{
	motion_set(0x04);
}

void soft_right (void)
{
	motion_set(0x02);
}

void soft_left_2 (void)
{
	motion_set(0x01);
}

void soft_right_2 (void)
{
	motion_set(0x08);
}

void stop (void) //hard stop
{
	motion_set(0x00);
}

void init_devices (void)
{
	cli();
	port_init();
	sei();
}



int main()
{
	init_devices();

	while(1)
	{

		forward();

		_delay_ms(1000);

		stop();
		_delay_ms(500);

		back();
		_delay_ms(1000);

		stop();
		_delay_ms(500);

		left();
		_delay_ms(1000);

		stop();
		_delay_ms(500);

		right();
		_delay_ms(1000);

		stop();
		_delay_ms(500);

		soft_left();
		_delay_ms(1000);

		stop();
		_delay_ms(500);

		soft_right();
		_delay_ms(1000);

		stop();
		_delay_ms(500);

		soft_left_2();
		_delay_ms(1000);

		stop();
		_delay_ms(500);

		soft_right_2();
		_delay_ms(1000);

		stop();
		_delay_ms(1000);
	}
}
