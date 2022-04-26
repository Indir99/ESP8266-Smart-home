#include "ultrasonic.h"

uint8_t get_ultrasonic(uint8_t TRIGGER_PIN, uint8_t ECHO_PIN)
{
    const float speedOfSound = 0.0343;
    float distance = 0;
    uint32_t numTicks = 0;

start:
    while (1)
    {

        gpio_set_level(TRIGGER_PIN, 0);
        vTaskDelay(0.003 / portTICK_RATE_MS);

        //1. Output 10 usec TRIG

        gpio_set_level(TRIGGER_PIN, 1);
        vTaskDelay(0.01 / portTICK_RATE_MS);
        gpio_set_level(TRIGGER_PIN, 0);

        //2. Wait for ECHO pin rising edge

        int counter = 0;
        while (gpio_get_level(ECHO_PIN) == 0)
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
        while (gpio_get_level(ECHO_PIN) == 1)
        {
            numTicks++;
            vTaskDelay(0.0028 / portTICK_RATE_MS);
        };

        //4. Estimate distance in cm

        float num = (float)numTicks;
        distance = ((num * 0.0028 * speedOfSound) / 2) * 2000;
        //real_distance = (uint8_t)distance;
        return (uint8_t)distance;
    }
}
