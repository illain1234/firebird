/*
 * main.c
 *
 * Created: 4/12/2023 2:34:21 PM
 *  Author: ajayr
 */ 

#define F_CP 14745600

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile unsigned long int shaftCountLeft = 0;
volatile unsigned long int shaftCountRight = 0;

void motion_pin_config (void)
{
	DDRA = DDRA | 0x0F;
	PORTA = PORTA & 0xF0;
	DDRL = DDRL | 0x18;   //Setting PL3 and PL4 pins as output for PWM generation
	PORTL = PORTL | 0x18; //PL3 and PL4 pins are for velocity control using PWM.
}

void left_encoder_pin_config (void) //Configure Interrupt 4
{
	DDRE = DDRE & 0xEF; //Set the direction of the PORTE 4 pin as input
	PORTE = PORTE | 0x10; //Enable internal pull-up for PORTE 4 pin
}

void right_encoder_pin_config (void) //Configure Interrupt 5
{
	DDRE = DDRE & 0xDF; //Set the direction of the PORTE 5 pin as input
	PORTE = PORTE | 0x20; //Enable internal pull-up for PORTE 5 pin
}

void port_init()
{
	motion_pin_config();
	left_encoder_pin_config();
	right_encoder_pin_config();
	
}

void left_position_encoder_interrupt_init (void) //Interrupt 4 enable
{
	cli(); //Clears the global interrupt
	EICRB = EICRB | 0x02; // INT4 is set to trigger with falling edge
	EIMSK = EIMSK | 0x10; // Enable Interrupt INT4 for left position encoder
	sei(); // Enables the global interrupt
}

void right_position_encoder_interrupt_init (void) //Interrupt 5 enable
{
	cli(); //Clears the global interrupt
	EICRB = EICRB | 0x08; // INT5 is set to trigger with falling edge
	EIMSK = EIMSK | 0x20; // Enable Interrupt INT5 for left position encoder
	sei(); // Enables the global interrupt
}

ISR(INT5_vect)
{
	
	shaftCountRight++;
}

ISR(INT4_vect)
{
	shaftCountLeft++;
}

void motion_set (unsigned char Direction)
{
	unsigned char PortARestore = 0;

	Direction &= 0x0F; 			// removing upper nibbel as it is not needed
	PortARestore = PORTA; 			// reading the PORTA's original status
	PortARestore &= 0xF0; 			// setting lower direction nibbel to 0
	PortARestore |= Direction; 	// adding lower nibbel for direction command and restoring the PORTA status
	PORTA = PortARestore; 			// setting the command to the port
}

void forward (void) //both wheels forward
{
	motion_set(0x06);
}

void left (void) //Left wheel backward, Right wheel forward
{
	motion_set(0x05);
}

void right (void) //Left wheel forward, Right wheel backward
{
	motion_set(0x0A);
}

void soft_left (void) //Left wheel stationary, Right wheel forward
{
	motion_set(0x04);
}

void soft_right (void) //Left wheel forward, Right wheel is stationary
{
	motion_set(0x02);
}


void stop (void) //hard stop
{
	motion_set(0x00);
}

void linear_distance_mm(unsigned int DistanceInMM)
{
	float ReqdshaftCount = 0;
	unsigned long int ReqdshaftCountInt = 0;
	
	ReqdshaftCount = DistanceInMM / 5.338;
	ReqdshaftCountInt = (unsigned long int) ReqdshaftCount;
	
	shaftCountRight = 0;
	while(1)
	{
		if(shaftCountRight > ReqdshaftCountInt)
		{
			break;
		}
	}
	stop();
}

void forward_mm(unsigned int DistanceInMM)
{
	forward();
	linear_distance_mm(DistanceInMM);
	
}

void init_devices()
{
	cli();
	port_init();
	left_position_encoder_interrupt_init();
	right_position_encoder_interrupt_init();
	sei();
	
}

int main(void)
{
	init_devices();
	
	forward_mm(100);
	while(1);
}

