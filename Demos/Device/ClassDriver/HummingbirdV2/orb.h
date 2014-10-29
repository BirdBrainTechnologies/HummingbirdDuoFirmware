/*
	orb.h - Contains variables and function prototypes necessary to run the LEDs on the Duo
	author:  Tom Lauwers
*/


// Defining pins used by the LEDs
#define LED1  PD0
#define LED2  PD1
#define LED3  PE2
#define LED4  PF7
#define REDLED1   PE6
#define GREENLED1 PD4
#define BLUELED1  PD6
#define REDLED2   PC6
#define GREENLED2 PD7
#define BLUELED2  PB7
#define ACTIVITY  PC7

// led_values_temp stores the red, green, blue values specified by calling set_orb and the single color LED values as well

// led_values reads led_values_temp at the beginning of each interrupt cycle and
// so holds these values constant even if set_orb is called in the middle of an interrupt cycle
uint8_t led_values[11] = {0,0,0, 0, 0, 0, 0, 0, 0, 0, 0};
uint8_t led_values_temp[11] = {0,0,0, 0, 0, 0, 0, 0, 0, 0, 0};
uint8_t next_led_val = 0;
uint8_t  activity_state = 0;  // Stores the activity state - fading for idle, solid for connected
uint8_t  activity_val = 0; // Activity intensity value
uint8_t  activity_dir = 0; // Direction of intensity value of activity LED - necessary for fading

// For function descriptions see orb.c
void init_orb(void);
void set_orb(uint8_t whichOne, unsigned int red_led, unsigned int green_led, unsigned int blue_led);
void set_orb0(unsigned int red_led, unsigned int green_led, unsigned int blue_led);
void set_orb1(unsigned int red_led, unsigned int green_led, unsigned int blue_led);
void set_led(uint8_t whichOne, unsigned int intensity);
void set_led0(unsigned int user);
void set_led1(unsigned int user);
void set_led2(unsigned int user);
void set_led3(unsigned int user);
void turn_off_leds(void);


