/*
	serial.c - Contains function headers to use the TTL serial port (Rx1, Tx1)
	author:  Tom Lauwers
*/

// Read serial.c for function descriptions
void serial_init(void);
void send_char(char c);
char receive_char(void);
bool received_data_available(void);
void store_received_serial_data(void);


// Define a buffer size of 256 for received data
#define USART_RX_BUFFER_SIZE 256
/* \brief Receive buffer mask. */
#define USART_RX_BUFFER_MASK ( USART_RX_BUFFER_SIZE - 1 )

/* \brief USART receive ring buffer. */
typedef struct USART_Buffer
{
	/* \brief Receive buffer. */
	volatile uint8_t RX[USART_RX_BUFFER_SIZE];
	/* \brief Receive buffer head. */
	volatile uint8_t RX_Head;
	/* \brief Receive buffer tail. */
	volatile uint8_t RX_Tail;
} USART_Buffer_t;

USART_Buffer_t rx_buffer;
