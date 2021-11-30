#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/adc.h"
//#include "NTC.h"

#include "debug.h"
#include "ultrasonic.h"
#include "driver/gpio.h"
#include "KY-033-Hunt.h"
#include "water_sensor.h"

#define TRIGGER_PIN 4
#define ECHO_PIN 5
#define HUNT_PIN 16
#define WATER_RELAY 14

void app_main(void)
{

	initDEBUG("", '5', 115200, "DSA - Debug example");
	//	xTaskCreate(adc_task, "adc_task", 1024, NULL, 10, NULL);
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

	gpio_config_t io_conf2;
	io_conf2.intr_type = GPIO_INTR_DISABLE;
	io_conf2.mode = GPIO_MODE_INPUT;
	io_conf2.pin_bit_mask = (1 << HUNT_PIN);
	io_conf2.pull_down_en = 1;
	io_conf2.pull_up_en = 0;
	gpio_config(&io_conf2);

	gpio_config_t io_conf3;
	io_conf3.intr_type = GPIO_INTR_DISABLE;
	io_conf3.mode = GPIO_MODE_OUTPUT;
	io_conf3.pin_bit_mask = (1 << WATER_RELAY);
	io_conf3.pull_down_en = 0;
	io_conf3.pull_up_en = 0;
	gpio_config(&io_conf3);

	adc_config_t adc_config;
	adc_config.mode = ADC_READ_TOUT_MODE;
	adc_config.clk_div = 8; // ADC sample collection clock = 80MHz/clk_div = 10MHz
	adc_init(&adc_config);

	uint8_t distance;
	uint16_t water_detector;
	uint8_t hunter;

	while (1)
	{
		water_detector = Get_Water_Value();
		distance = get_ultrasonic(TRIGGER_PIN, ECHO_PIN);
		hunter = Get_KY033_Hunt(HUNT_PIN);

		printDEBUG(DSYS, "Distance %d\n", (uint8_t)distance);
		printDEBUG(DSYS, "hunter lvl: %d\n", hunter);
		printDEBUG(DSYS, "ADC: %d\n", water_detector);

		vTaskDelay(1000 / portTICK_RATE_MS);
	}
}

//void adc_task()
//{
//	adc_config_t adc_config;

// Depend on menuconfig->Component config->PHY->vdd33_const value
// When measuring system voltage(ADC_READ_VDD_MODE), vdd33_const must be set to 255.
//	adc_config.mode = ADC_READ_TOUT_MODE;
//	adc_config.clk_div = 8; // ADC sample collection clock = 80MHz/clk_div = 10MHz

// ESP_ERROR_CHECK(adc_init(&adc_config));
//	adc_init(&adc_config);
//	uint16_t temp;

//	while (1)
//	{
//		temp = Get_Temp_NTC();
//		printDEBUG(DSYS, "Temp: %d\n", temp);
//		vTaskDelay(250 / portTICK_RATE_MS);

//		if (ESP_OK == adc_read(&adc_data))
//		{

//			adc_data = (3300 * adc_data) / 1023;
//printf("ADC: %d\n", adc_data);
//printf("Hello world \n");
//vTaskDelay(1000 / portTICK_PERIOD_MS);
//		}
//	}
//vTaskDelay(1000 / portTICK_PERIOD_MS);
//}
