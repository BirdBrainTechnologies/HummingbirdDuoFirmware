/*
	motor.c - Contains functions and interrupts necessary to activate motors
	author:  Tom Lauwers
*/

void turn_off_motors()
{
	transmitByteToTiny(0xFE); // Hummingbird's ATtiny firmware takes 0xFE to indicate that motors should both be off
}

// Sets the motor and speed. The first byte controls both the motor port and direction, as follows:
// 4 - set port 1 forward
// 5 - set port 2 forward
// 6 - set port 1 backward
// 7 - set port 2 backward
// The speed byte just sets power to the motor, 0 is off, 255 is fully on
void set_motor(uint8_t motor, uint8_t direction, uint8_t speed)
{
	transmitByteToTiny(motor+4 + direction*2);
	_delay_us(50);
	transmitByteToTiny(speed);
	// Record the values
	motor_vals[motor*2] = direction;
	motor_vals[motor*2+1] = speed;
}
