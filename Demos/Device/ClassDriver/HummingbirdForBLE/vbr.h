/*
	vbr.h - Contains function headers used to drive vibration motors
	author:  Tom Lauwers
*/


uint8_t vbr_values_temp[2] = {0,0};

void init_vbr(void);
void set_vibration_motor(uint8_t whichOne, uint8_t val);
void disable_vibration_motors(void);