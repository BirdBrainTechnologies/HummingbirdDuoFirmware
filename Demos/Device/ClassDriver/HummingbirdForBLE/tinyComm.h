/*
	tinyComm.h - Contains function headers to communicate with Duo's ATtiny via SPI
	author:  Tom Lauwers
*/

void init_tiny_comm(void);
void transmitByteToTiny(uint8_t cData);