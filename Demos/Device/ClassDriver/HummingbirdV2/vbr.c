/*
	vbr.c - Contains functions used to drive vibration motors
	author:  Tom Lauwers
*/

// Initialize hardware 8-bit PWM on vibration motor ports, 31 KHz
void init_vbr() {
	// Configure counter such that we use fast
	// PWM with 8-bit resolution at a frequency of
	// 8000000/(1*255) = ~31 KHz

	TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM10);
	TCCR1B = _BV(CS10) | _BV(WGM12);

	// Configure the two PWM as outputs:
	DDRB |= _BV(PB5) | _BV(PB6);  // PWM pins

	// Turn off the motors. Necessary to prevent them from running on startup
	disable_vibration_motors();
}


void set_vibration_motor(uint8_t whichOne, uint8_t val) {
	if(whichOne == 48) {// 48 is ASCII 0
		OCR1A = val; // 8 bit PWM, so 0 is off, 255 is totally on
		vbr_values_temp[0] = val;
	}
	else {
		OCR1B = val;
		vbr_values_temp[1] = val;
	}
}

// Sets motors to 0, updates buffer
void disable_vibration_motors() {
	OCR1A = 0;
	OCR1B = 0;
	vbr_values_temp[0] = 0;
	vbr_values_temp[1] = 0;
}

