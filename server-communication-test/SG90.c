
#include "SG90.h"



void MoveSG90(uint8_t angle)
{
	//For 0 degree angle, its 500 ms width of pulse for S90 manfacturer:DIY MORE
	uint16_t targetPeriod = 500;
	
	//Calculate period in ms for selected angle
	for (uint8_t i = 0; i < angle; i++)
	{
		targetPeriod =  500 + (11*i);
	}
	pwm_stop(0);
	pwm_set_duty(0, targetPeriod);
	pwm_start();
}
