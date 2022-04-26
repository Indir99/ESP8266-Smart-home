#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "esp_log.h"

#include "driver/gpio.h"


extern TickType_t passed_Time;

//It is wise to take delay time(sampling time) of 100 ms to display in sync with timer
extern void TimerTick();

extern void StartTimer();
extern void ResumeTimer();

extern void StopTimer();

extern void RestartTimer();