/*
 * main.c
 *
 * Created: 4/5/2023 2:36:44 PM
 *  Author: ajayr
 */ 

#define F_CPU 14745600
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd.c"
//Function to initialize Buzzer
void buzzer_pin_config (void)
{
	DDRC = DDRC | 0x08; // SETTING PORTC PIN3 AS OUTPUT
	
	PORTC = PORTC & 0xF7; // SETTING PORTC TO LOGIC LOW, i.e., BUZZER OFF
}


void interrupt_switch_config (void)
{
	DDRE = DDRE & 0x7F; // SETTING PORTE PIN7 AS INPUT
	
	PORTE = PORTE | 0x80; // SETTING PORTC TO LOGIC LOW, i.e., SWITCH NOT PRESSED
}


void LED_bargraph_config (void)
{
	DDRJ = 0xFF; // SETTING PORTJ ALL PINS AS OUTPUT
	
	PORTJ = 0x00; // SETTING PORTJ TO LOGIC LOW, i.e., ALL LIGHTS OFF
}


//Function to configure ports to enable robot's motion
void motion_pin_config (void)
{
	DDRA = DDRA | 0x0F;
	PORTA = PORTA & 0xF0;
	DDRL = DDRL | 0x18;   //Setting PL3 and PL4 pins as output for PWM generation
	PORTL = PORTL | 0x18; //PL3 and PL4 pins are for velocity control using PWM.
}

// storing different patterns of led on in array
unsigned char p[8] = {0x01 , 0x03, 0x07 , 0x0F , 0x1F, 0x3F, 0x7F, 0xFF};

void buzzer_on (void)
{
	unsigned char port_restore = 0;
	port_restore = PINC;
	port_restore = PINC | 0x08;
	PORTC = port_restore;
}
void led_on (int i){
	PORTJ =p[i];
}


void buzzer_off (void)
{
	unsigned char port_restore = 0;
	port_restore = PINC;
	port_restore = PINC & 0x00;
	PORTC = port_restore;
}

//Function to initialize ports
void init_ports()
{
	buzzer_pin_config();
	interrupt_switch_config();
	LED_bargraph_config();
	motion_pin_config();
}

// Timer 5 initialized in PWM mode for velocity control
// Pre-scale:256
// PWM 8bit fast, TOP=0x00FF
// Timer Frequency:225.000Hz
void timer5_init()
{
	TCCR5B = 0x00;	//Stop
	TCNT5H = 0xFF;	//Counter higher 8-bit value to which OCR5xH value is compared with
	TCNT5L = 0x01;	//Counter lower 8-bit value to which OCR5xH value is compared with
	OCR5AH = 0x00;	//Output compare register high value for Left Motor
	OCR5AL = 0xFF;	//Output compare register low value for Left Motor
	OCR5BH = 0x00;	//Output compare register high value for Right Motor
	OCR5BL = 0xFF;	//Output compare register low value for Right Motor
	OCR5CH = 0x00;	//Output compare register high value for Motor C1
	OCR5CL = 0xFF;	//Output compare register low value for Motor C1
	TCCR5A = 0xA9;	/*{COM5A1=1, COM5A0=0; COM5B1=1, COM5B0=0; COM5C1=1 COM5C0=0}
 					  For Overriding normal port functionality to OCRnA outputs.
				  	  {WGM51=0, WGM50=1} Along With WGM52 in TCCR5B for Selecting FAST PWM 8-bit Mode*/
	TCCR5B = 0x0B;	//WGM12=1; CS12=0, CS11=1, CS10=1 (Prescaler=64)
}

// Function for robot velocity control
void velocity (unsigned char left_motor, unsigned char right_motor)
{
	OCR5AL = (unsigned char)left_motor;
	OCR5BL = (unsigned char)right_motor;
}

//Function used for setting motor's direction
void motion_set (unsigned char Direction)
{
	unsigned char PortARestore = 0;

	Direction &= 0x0F; 			// removing upper nibble as it is not needed
	PortARestore = PORTA; 			// reading the PORTA's original status
	PortARestore &= 0xF0; 			// setting lower direction nibble to 0
	PortARestore |= Direction; 	// adding lower nibble for direction command and restoring the PORTA status
	PORTA = PortARestore; 			// setting the command to the port
}

void forward (void) //both wheels forward
{
	motion_set(0x06);
}

void back (void) //both wheels backward
{
	motion_set(0x09);
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

void soft_left_2 (void) //Left wheel backward, right wheel stationary
{
	motion_set(0x01);
}

void soft_right_2 (void) //Left wheel stationary, Right wheel backward
{
	motion_set(0x08);
}

void stop (void)
{
	motion_set(0x00);
}

void init_devices (void) //use this function to initialize all devices
{
	cli(); //disable all interrupts
	init_ports();
	timer5_init();
	lcd_init();
	lcd_port_config();
	sei(); //re-enable interrupts
}


	
	// For 1st rows ----------------->
void lcd_name (char co[],int val){
		char *ptr = co ;
		lcd_init();
		while (1){
			lcd_port_config();
			//lcd_init();
			lcd_string(1,1,ptr);
			_delay_ms(100);
			break;
		}
	}
	
//Main Function
int main()
{
	init_devices();
	int count = 0;
	int led_bar_count = 0;
	char co0[20] = {"000"};
	char co1[20] = {"Counting"};
	lcd_name(co0,3);
	velocity(100,100);
	while(1)
	{    
		if(TCNT5 == 0xFF){
			count++;
			led_on(led_bar_count);
			itoa(count,co0,10);
			lcd_name(co0,sizeof(co0));
			buzzer_on();
			forward();
			_delay_ms(500);
			stop();
			buzzer_off();
		    _delay_ms(100);
			buzzer_on();
			back();
			_delay_ms(500);
			stop();
			buzzer_off();
			_delay_ms(100);
			buzzer_on();
			left();
			_delay_ms(500);
			stop();
			buzzer_off();
			_delay_ms(100);
			buzzer_on();
			right();
			_delay_ms(500);
			stop();
			buzzer_off();
			_delay_ms(100);
			led_bar_count++;
			if(led_bar_count==7){
				led_bar_count=0;
			}
		}
}

	}