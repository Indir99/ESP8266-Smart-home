#ifndef __ULTRASONIC_H
#define __ULTRASONIC_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "hw_timer.h"

uint8_t get_ultrasonic(uint8_t TRIGGER_PIN, uint8_t ECHO_PIN);

#endif