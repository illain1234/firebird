/*
 * main.c
 *
 * Created: 4/8/2022 2:11:23 PM
 *  Author: Mukesh
 */ 

#define F_CPU 14745600
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Function to configure buzzer
void buzzer_pin_config (void)
{
	DDRC = DDRC | 0x08; // Setting PORTC 3 as output - turn on buzzer 
	PORTC = PORTC & 0xF7; // Setting PORTC 3 as output - turn off buzzer
}

// Function to configure Interrupt switch
void interrupt_switch_config (void)
{
	DDRE = DDRE & 0x7F; // Setting PORTE 7 as input 
	PORTE = PORTE | 0x80; // PORTE7 internal pull-up enabled 
}

//Function to configure LDD bargraph display

void LED_bargraph_config (void)
{
	DDRJ = DDRJ | 0xFF; // PORT J is configured as output
	PORTJ = PORTJ & 0x00; // Output is set to 0
}

//Function to Initialize PORTS

void port_init (void)
{
	buzzer_pin_config();
	interrupt_switch_config();
	LED_bargraph_config();
}

void buzzer_on (void)
{
	unsigned char port_restore = 0;
	port_restore = PINC; // 0x00;
	port_restore = port_restore | 0x08;
	PORTC = port_restore; // 0x08;
}

void buzzer_off (void)
{
	unsigned char port_restore = 0;
	port_restore = PINC; // 0x00
	port_restore = port_restore & 0xF7;
	PORTC = port_restore; // 0x00
}


void init_devices (void)
{
	cli(); //Clears the global interrupts
	port_init();
	sei(); //Enables the global interrupts
}


int main(void)
{
	init_devices();
    while(1)
   if((PINE & 0x80) == 0x80) //switch is not pressed
   {
	   buzzer_off(); //Turn off buzzer
	   PORTJ = 0x00; //Turn off bar graph LEDs
   }
   else
   {
	   while((PINE & 0x80)!= 0x80)
	   {
		PORTJ = 0x01; // 000000001
        buzzer_on();
		_delay_ms(1000);
		buzzer_off();
		_delay_ms(1000);
		
		
		    }
   }
}