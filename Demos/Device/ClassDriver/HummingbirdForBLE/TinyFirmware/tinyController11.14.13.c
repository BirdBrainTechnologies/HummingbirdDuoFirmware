	#include <inttypes.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <util/delay.h>
	#include <avr/io.h>
	#include <avr/interrupt.h>
	#include <util/twi.h>
	#include <avr/wdt.h>
	#include <avr/power.h>
	#include <string.h>

	#define SERVO0 PA0
	#define SERVO1 PA1
	#define SERVO2 PA2
	#define SERVO3 PA3

	#define MOSI_PIN PA6
	#define SCK_PIN PA4
	#define SS_PIN PA5

	#define MIN_SERVO   76 // 0.608 ms is the minimum uptime
	#define MAX_SERVO   0xFFFF // Overflow should occur at roughly 40 Hz

	void init_servos(void);
	void set_servo(uint8_t , uint8_t);
	void disable_servos(void);
	void init_motors(void);
	void set_motor(uint8_t , uint8_t );
	void disable_motors(void);
	void init_spi(void);
	void printByte(uint8_t value);

	unsigned int servo_values[4] = {0,0,0,0};
	unsigned int servo_values_temp[4] = {0,0,0,0};
	uint8_t servo_enabled[4] = {0,0,0,0};
	volatile unsigned int next_servo_val = 0;

	volatile uint8_t toggle;

	int main(void)
	{
		uint8_t incoming_byte1=0;
		uint8_t incoming_byte2=0;
		uint8_t state = 0;

		_delay_ms(1);
		init_servos();
		init_motors();
		init_spi();

		// init global interrupts
		sei();

		while(1) {
			// Only do stuff if Pin A5 is low
			if(!(PINA & _BV(PA5))) {
				if(USISR & _BV(USIOIF)) {
					// Clear the flag
					USISR |= _BV(USIOIF);

					// Clear the counter just in case
					USISR &= 0xF0;

					if(state == 0) {
						incoming_byte1 = USIBR;
						if(incoming_byte1 == 0xFF) {
							disable_servos();
						}
						else if(incoming_byte1 == 0xFE) {
							disable_motors();
						}
						else {
							state = 1;
						}
					//	printByte(incoming_byte1);
					}
					else {
						incoming_byte2 = USIBR;
						if(incoming_byte1 < 4) {
							set_servo(incoming_byte1, incoming_byte2);
						}
						else {
							set_motor(incoming_byte1, incoming_byte2);
						}
						state = 0;
					//	printByte(incoming_byte2);
					}
				}
			}
		}


		return 1;

	}

	void init_spi()
	{
		// ensures the direction of the SPI port is all inputs
		DDRA &= (0xFF - _BV(MOSI_PIN) - _BV(SS_PIN) - _BV(SCK_PIN));

		// Enable the counter interrupt, and 3-wire mode, use external clocking and sample on leading edge
		USICR = _BV(USIWM0) | _BV(USICS1) | _BV(USICS0); // _BV(USIOIE) |

		// Clear counter
		USISR &= 0xF0;

	}

	void init_servos()
	{
		DDRA |= _BV(SERVO0) | _BV(SERVO1) | _BV(SERVO2) | _BV(SERVO3);

		// Set pins to normal mode, set waveform to CTC mode (OCR1A becomes top)
		TCCR1A = 0x00;
		// Set clock divider to 64, counting frequency is one count/8us
		TCCR1B = _BV(WGM12) | _BV(CS11) | _BV(CS10);

		// Set overflows to occur every 40 Hz
		OCR1A = 3125;
		// Enable ocr1a and ocr1b interrupts
		TIMSK1 = _BV(OCIE1B) | _BV(OCIE1A);

	}

	void init_motors()
	{
		// set motor pins to output
		DDRA |= _BV(PA7);
		DDRB |= _BV(PB0) | _BV(PB1) | _BV(PB2);
		// Set to non-inverting fast PWM mode
		TCCR0A = 0xA3; //_BV(COM0A1) | _BV(COM0B1) | _BV(WGM01) | (WGM00);
		// Use the built-in 8 MHz oscillator for clocking
		TCCR0B = 0x01;//_BV(CS00);
	}

	/* Which combines the motor and direction of the motor, values:
	    4 - motor 1 forward
	    5 - motor 2 forward
	    6 - motor 1 backward
	    7 - motor 2 backward
	   value is the power going to the motor */
	void set_motor(uint8_t which, uint8_t value)
	{
		// Set motor 1 or 2
		if(which == 4 || which == 6)
		{
			if(which == 4)
				PORTB |= _BV(PB1);
			else
				PORTB &= ~_BV(PB1);
			OCR0B = value;
		}
		else
		{
			if(which == 5)
				PORTB |= _BV(PB0);
			else
				PORTB &= ~_BV(PB0);
			OCR0A = value;
		}
	}

	void disable_motors()
	{
		OCR0A = 0;
		OCR0B = 0;
	}

	// Disables the timer1 interrupt and turns ports to 0, disabling the servos
	void disable_servos()
	{
		// Disables interrupts
		servo_enabled[0] = 0;
		servo_enabled[1] = 0;
		servo_enabled[2] = 0;
		servo_enabled[3] = 0;
		PORTA &= (0xFF - _BV(SERVO0) - _BV(SERVO1) - _BV(SERVO2) - _BV(SERVO3));
	}

	// Turns servo pin to output and sets its value
	void set_servo(uint8_t which, uint8_t value)
	{
		// servo values over 250 turn power off to the servo
		if(value > 250)
			servo_enabled[which] = 0;
		else {
			servo_enabled[which] = 1;
			servo_values_temp[which] = value + MIN_SERVO;
		}
	}

	// For debugging only
	void printByte(uint8_t value)
	{
		PORTA |= _BV(SERVO3);
		_delay_us(5);
		PORTA &= (0xFF - _BV(SERVO3));
		_delay_us(2);
		for(int i = 7; i >= 0; i--)
		{
			PORTA |= _BV(SERVO3);

			if((value>>i) & 0x01)
				_delay_us(25);
			else
				_delay_us(2);
			PORTA &= (0xFF - _BV(SERVO3));
			_delay_us(20);
		}

		PORTA &= (0xFF - _BV(SERVO3));
		_delay_us(5);
		PORTA |= _BV(SERVO3);
		_delay_us(2);
		PORTA &= (0xFF - _BV(SERVO3));

	}

	/* Timer 1 compare A interrupt, acting as timer overflow. Set all servo pins high, then set the time to trigger output compare B (which turns them off) */
	ISR(TIM1_COMPA_vect)
	{
		int i;

		for(i = 0; i < 4; i++) {
			servo_values[i] = servo_values_temp[i];
		}

		// Set the first servo high
		if(servo_enabled[0] == 1)
			PORTA |= _BV(SERVO0);

		next_servo_val = 0;

		// now set the next interrupt to occur in current time plus the time of the next servo pulse
		OCR1B = servo_values[next_servo_val] + TCNT1;

		/*
		OCR1BH = servo_values[0]>>8;
		OCR1BL = servo_values[0];*/
	}

	/* Timer 1 output compare B interrupt.  Does the following:
		Checks to see if the current servo is 4, in which case just need
		to set low servo 3's signal and return.
		Else, check to see if the current servo is enabled, and set it's signal high
		Then, set the previous servo's signal low
		Now, set the next output compare to occur for some time from now
		specified by the current servo's value
	*/


	ISR(TIM1_COMPB_vect)
	{
		// turn off the current servo pin
		PORTA &= (0xFF-_BV(next_servo_val));

		// turn on the next one
		next_servo_val++;
		// if we've reach our limit, break out instead
		if(next_servo_val > 3)
			return;

		// if enabled, turn on the next one
		if(servo_enabled[next_servo_val] == 1)
			PORTA |= _BV(next_servo_val);

		// now set the next interrupt to occur in current time plus the time of the next servo pulse
		OCR1B = servo_values[next_servo_val] + TCNT1;
		/*
		OCR1BL = next_servo_val + TCNT1L;
		OCR1BH = servo_values[next_servo_val]>>8 + TCNT1H;*/
	}
	/*
	ISR(USI_OVF_vect)//, ISR_NOBLOCK)
	{

		if(toggle <  2)
		{
			set_servo(0, 255);
			toggle++;
		}
		else {
			set_servo(0,0);
			toggle = 0;
		}
		// Clear the interrupt - somehow this isn't being done in the ISR
		USISR |= _BV(USIOIF);

		// Clear the counter just in case
		USISR &= 0xF0;

		if(state == 0) {
			incoming_byte1 = USIBR;
			if(incoming_byte1 == 0xFF) {
				disable_servos();
			}
			else if(incoming_byte1 == 0xFE) {
				disable_vbr();
			}
			else {
				state = 1;
			}
			printByte(incoming_byte1);
		}
		else {
			incoming_byte2 = USIBR;
			if(incoming_byte1 < 4) {
				set_servo(incoming_byte1, incoming_byte2);
			}
			else {
				set_vbr(incoming_byte1, incoming_byte2);
			}
			state = 0;
			printByte(incoming_byte2);
		}
	}*/
