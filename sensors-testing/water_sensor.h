#ifndef __WATERSENSOR_H
#define __WATERSENSOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"

extern uint16_t Get_Water_Value();
#endif