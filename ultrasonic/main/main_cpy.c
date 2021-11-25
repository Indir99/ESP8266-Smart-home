#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "driver/gpio.h"
#include "debug.h"
#include "hw_timer.h"

#define TRIGGER 4
#define ECHO 5

const float speedOfSound = 0.0343;
float distance = 0;
//uint8_t real_distance = 0;

void app_main(void)
{

    initDEBUG("", '5', 115200, "DSA - Debug example");

    uint32_t numTicks = 0;

    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1 << TRIGGER);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

    gpio_config_t io_conf1;
    io_conf1.intr_type = GPIO_INTR_DISABLE;
    io_conf1.mode = GPIO_MODE_INPUT;
    io_conf1.pin_bit_mask = (1 << ECHO);
    io_conf1.pull_down_en = 0;
    io_conf1.pull_up_en = 0;
    gpio_config(&io_conf1);

start:
    while (1)
    {

        gpio_set_level(TRIGGER, 0);
        vTaskDelay(0.003 / portTICK_RATE_MS);

        //1. Output 10 usec TRIG

        gpio_set_level(TRIGGER, 1);
        vTaskDelay(0.01 / portTICK_RATE_MS);
        gpio_set_level(TRIGGER, 0);

        //2. Wait for ECHO pin rising edge

        int counter = 0;
        while (gpio_get_level(ECHO) == 0)
        {
            if (counter < 3)
            {
                counter++;
                continue;
            }

            goto start;
        }

        //3. Start measuring ECHO pulse width in usec

        numTicks = 0;
        while (gpio_get_level(ECHO) == 1)
        {
            numTicks++;
            vTaskDelay(0.0028 / portTICK_RATE_MS);
        };

        //4. Estimate distance in cm

        float num = (float)numTicks;
        distance = ((num * 0.0028 * speedOfSound) / 2) * 2000;
        //real_distance = (uint8_t)distance;

        printDEBUG(DSYS, "distance %d\n", (uint8_t)distance);

        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}