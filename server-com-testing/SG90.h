#ifndef __SG90
#define __SG90

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//#include <unistd.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//#include "driver/uart.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "driver/pwm.h"
void MoveSG90(uint8_t angle);
#endif