#ifndef __KY033HUNTC_H
#define __KY033HUNTC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//#include <unistd.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//#include "driver/uart.h"
//#include "driver/gpio.h"
#include "driver/adc.h"

uint8_t Get_KY033_Hunt(uint8_t GPIO);

#endif