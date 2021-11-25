#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "debug.h"
#include "ultrasonic.h"
#include "driver/gpio.h"

#define TRIGGER_PIN 4
#define ECHO_PIN 5

void app_main(void)
{

	initDEBUG("", '5', 115200, "DSA - Debug example");
	gpio_config_t io_conf;
	io_conf.intr_type = GPIO_INTR_DISABLE;
	io_conf.mode = GPIO_MODE_OUTPUT;
	io_conf.pin_bit_mask = (1 << TRIGGER_PIN);
	io_conf.pull_down_en = 0;
	io_conf.pull_up_en = 0;
	gpio_config(&io_conf);

	gpio_config_t io_conf1;
	io_conf1.intr_type = GPIO_INTR_DISABLE;
	io_conf1.mode = GPIO_MODE_INPUT;
	io_conf1.pin_bit_mask = (1 << ECHO_PIN);
	io_conf1.pull_down_en = 0;
	io_conf1.pull_up_en = 0;
	gpio_config(&io_conf1);
	uint8_t distance;

	while (1)
	{
		distance = get_ultrasonic(TRIGGER_PIN, ECHO_PIN);
		printDEBUG(DSYS, "distance %d\n", (uint8_t)distance);

		vTaskDelay(1000 / portTICK_RATE_MS);
	}
}
