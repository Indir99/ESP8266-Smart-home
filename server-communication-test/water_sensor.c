#include "water_sensor.h"

uint16_t adc_data2 = 0;

uint16_t Get_Water_Value()
{
    if (ESP_OK == adc_read(&adc_data2))
    {
        adc_data2 = (3300 * adc_data2) / 1023;
    }
    return adc_data2;
}