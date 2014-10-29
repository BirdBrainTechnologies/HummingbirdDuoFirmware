/*  analog.h - Header file for Finch Analog inputs
	author:  Tom Lauwers
*/

#define SENSOR1   0b01100110
#define SENSOR2   0b01100101
#define SENSOR3   0b01100100
#define SENSOR4   0b01100001
#define EXT_PWR   0b01100000

// Function description can be found in analog.c
void init_analog(void);
unsigned int read_sensor(char sensor);
