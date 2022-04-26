
#include "KY-033-Hunt.h"
#include "driver/gpio.h"

uint8_t Get_KY033_Hunt(uint8_t GPIO)
{
	uint8_t val = (uint8_t)gpio_get_level(GPIO);
	switch (val)
	{
	case 1:
		val = 0;
		break;
	case 0:
		val = 1;
		break;
	}

	return val;
}