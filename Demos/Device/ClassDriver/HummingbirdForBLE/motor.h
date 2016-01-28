/*
	motor.h - Contains function headers to run motors
	author:  Tom Lauwers
*/

// Used for storing speed and direction of each motor
uint8_t motor_vals[4] = {0,0,0,0};

void turn_off_motors(void);
void set_motor(uint8_t motor, uint8_t direction, uint8_t speed);