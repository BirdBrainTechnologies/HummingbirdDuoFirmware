/*
	tinyComm.c - Contains low level functions used by servos and motors libraries to communicate with Duo's ATtiny via SPI
	author:  Tom Lauwers
*/

// Initialize SPI
void init_tiny_comm()
{
	/* Set MOSI, SS, and SCK output */
	DDRB |= _BV(PB2)|_BV(PB1) | _BV(PB4);

	/* Enable SPI, Master, set clock rate fck/16, clock phase trailing */
	SPCR = _BV(SPE)| _BV(MSTR)| _BV(SPR0) | _BV(CPHA); // add | _BV(SPR1) for slow speed
	PORTB |= _BV(PB4); // Start active high to prevent accidental communication
}


void transmitByteToTiny(uint8_t cData)
{

	// Set SS pin low so Tiny can hear our SPI data
	PORTB &= ~_BV(PB4);
	_delay_us(20);
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
	_delay_us(40);
	PORTB |= _BV(PB4); // Set this high, when PB4 is high, the Tiny ignores all SPI signals
}