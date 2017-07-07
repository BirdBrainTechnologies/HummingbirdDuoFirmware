/*
	serial.c - Contains functions to use the TTL serial port (Rx1, Tx1)
	author:  Tom Lauwers
*/

uint8_t rx_buff[256]; // A buffer for storing incoming serial data

// Set serial to 115200, 8n1
void serial_init()
{
		UCSR1A = _BV(U2X1); // Double speed mode
		UCSR1B = _BV(RXCIE1) | _BV(RXEN1) | _BV(TXEN1); // Enable Rx/Tx, and Rx interrupts
		UCSR1C = _BV(UCSZ11) | _BV(UCSZ10); // Set to 8-bit mode
		rx_buffer.RX_Head = 0;
		rx_buffer.RX_Tail = 0;

		// Set baud rate to 115200
		UBRR1L = 16;
}

// Sends one byte over serial
void send_char(char c)
{
	while(!(UCSR1A & _BV(UDRE1)));
	UDR1 = c;
}

/*! \brief Test if there is data in the receive software buffer.
 *
 *  This function can be used to test if there is data in the receive software
 *  buffer.
 *
 *
 *  \retval true      There is data in the receive buffer.
 *  \retval false     The receive buffer is empty.
 */
bool received_data_available()
{
	/* Make copies to make sure that volatile access is specified. */
	uint8_t tempHead = rx_buffer.RX_Head;
	uint8_t tempTail = rx_buffer.RX_Tail;

	/* There are data left in the buffer unless Head and Tail are equal. */
	return (tempHead != tempTail);
//	return (_BV(RXC1) & UCSR1A);


}

void send_sensors()
{
	send_char(0x05); // Send 5 bytes
	send_char(read_sensor(SENSOR1));
	send_char(read_sensor(SENSOR2));
	send_char(read_sensor(SENSOR3));
	send_char(read_sensor(SENSOR4));
	send_char(read_sensor(EXT_PWR));
}


/*! \brief Get received data (5-8 bit character).
 *
 *  The function received_data_available should be used before this
 *  function is used to ensure that data is available.
 *
 *  Returns data from RX software buffer.
 *
 *
 *  \return         Received data.
 */
char receive_char()
{

	char ans;
	ans = rx_buffer.RX[rx_buffer.RX_Tail];

	/* Advance buffer tail. */
	rx_buffer.RX_Tail = (rx_buffer.RX_Tail + 1);
	//send_char(ans); // Echo the received character to verify that we've received it to the sender
	return ans;
}



/*! \brief RX Complete Interrupt Service Routine.
 *
 *  Receive data Interrupt Service Routine.
 *  Stores received data in RX software buffer.
 *
 */
void store_received_serial_data()
{
	/* Advance buffer head. */
	uint8_t tempRX_Head = (rx_buffer.RX_Head + 1);

	/* Check for overflow. */
	uint8_t tempRX_Tail = rx_buffer.RX_Tail;
	// Get data
	uint8_t data = UDR1;
	if (tempRX_Head != tempRX_Tail) {
		rx_buffer.RX[rx_buffer.RX_Head] = data;
		rx_buffer.RX_Head = tempRX_Head;
	}
}


/*! \brief Receive complete interrupt service routine.
 *
 *  Receive complete interrupt service routine.
 *  Calls the common receive complete handler with pointer to the correct USART
 *  as argument.
 */

ISR(USART1_RX_vect)
{
	store_received_serial_data();
}