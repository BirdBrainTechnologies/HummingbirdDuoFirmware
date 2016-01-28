/*  
	servo.h - Contains function prototypes for servo activation
	author:  Tom Lauwers
*/

uint8_t servo_values_temp[4] = {0,0,0,0};

// Information on functions available in servo.c
void enable_servos(void);
void set_servo(uint8_t servo, uint8_t value);
void disable_servos(void);
