/*
	analog.c - Contains the functions for manipulating the Finch's ADC
	author:  Tom Lauwers
*/

// Initialize ADC as single sample

void init_analog(void)
{
	// ADMUX register
	// Bit 7,6 - Set voltage reference to AVcc (0b01)
	// Bit 5 - Set ADLAR bit for left adjust to do simple 8-bit reads
	// Bit 4 - X
	// Bit 3:0 - Sets the current channel, set here to ADC0
	ADMUX = 0x60;

	// ADC Status Register A
	// We're only using the comparator right now, so just set ADC clock
	// Bit 7 - ADEN is cleared (Analog is only enabled when doing a read)
	// Bit 6 - We'll start a conversion later
	// Bit 5 - Enable Auto Trigger
	// Bit 3 - No ADC Interrupt
	// Bits 2-0 - Set to create a clock divisor of 16, to make ADC clock = 8,000,000/16 = 500,000 Hz
	ADCSRA = 0b10000100;

	// ADC Status Register B
	// Bit 7 - ADC High speed mode enabled; may be unnecessary
	// Bit 5 - Mux mode, cleared for single ended input
	// Bit 3:0 - Set interrupt mode, currently cleared
	ADCSRB = 0x00;


	// Digital input disable - disabling digital ins on ADC0, 1, 4, 5, 6
	DIDR0 = 0x73;
}


// Read a sensor value
unsigned int read_sensor(char sensor) {
	ADMUX = sensor;

	// Start conversion
	ADCSRA |= 0b01000000;
	// Gets cleared when the conversion ends
	while((ADCSRA &  0b01000000));

	ADCSRA |= _BV(ADIF);
	return ADCH;
}
