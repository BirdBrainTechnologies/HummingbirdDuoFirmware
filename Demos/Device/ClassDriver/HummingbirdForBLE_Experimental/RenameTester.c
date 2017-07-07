#include <HummingbirdForBLE.h>


int main(void)
{
	char response[25];
	int i = 0;
	SetupHardware();

	GlobalInterruptEnable();
	
	send_char('+');
	send_char('+');
	send_char('+');
	send_char('\n');
	
	_delay_ms(500);
	
	while(received_data_available())
	{
		response[0] = receive_char();
	}
	
	// send AT+BLEGETADDR
	send_char('A');
	send_char('T');
	send_char('+');
	send_char('B');
	send_char('L');
	send_char('E');
	send_char('G');
	send_char('E');
	send_char('T');
	send_char('A');
	send_char('D');
	send_char('D');
	send_char('R');
	send_char('\n');
	
	_delay_ms(500);
	while(received_data_available() && i < 15)
	{
		response[i] = receive_char();
		i++;
	}
	i=0;
	while(received_data_available())
	{
		response[i] = receive_char();
		i++;
	}

	// send AT+GAPDEVNAME=HBXXXXX
	send_char('A');
	send_char('T');
	send_char('+');
	send_char('G');
	send_char('A');
	send_char('P');
	send_char('D');
	send_char('E');
	send_char('V');
	send_char('N');
	send_char('A');
	send_char('M');
	send_char('E');
	send_char('=');
	send_char('t');
	send_char('l');
	send_char(response[10]);
	send_char(response[12]);
	send_char(response[13]);
	send_char(response[15]);
	send_char(response[16]);
	send_char('\n');
	i = 0;
	_delay_ms(500);
	while(received_data_available())
	{
		response[i] = receive_char();
		i++;
	}
	if(response[0] == 'O' && response[1] == 'K')
	{	
		set_led('0', 255);
	}
	if(response[0] == 'A' && response[1] == 'T')
	{	
		set_led('1', 255);
	}
	send_char('A');
	send_char('T');
	send_char('Z');
	send_char('\n');
	
	while(1);
}

/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware(void)
{
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);

	/* Hardware Initialization */
	LEDs_Init();
	init_analog();
	init_orb();
	init_vbr();
	init_tiny_comm();
	USB_Init();
	serial_init();
}
	