
#include "NTC.h"

int Vo;
float R1 = 10000; // value of R1 on board
float logR2, R2, T;
float c1 = 0.001129148, c2 = 0.000234125, c3 = 0.0000000876741; //steinhart-hart coeficients for thermistor

float calibration = 14;
uint16_t adc_data = 0;
float Get_Temp_NTC()
{
	float T = -999;

	if (ESP_OK == adc_read(&adc_data))
	{

		//adc_data = (3300 * adc_data) / 1023;
		Vo = adc_data;
		R2 = R1 * (1023.0 / (float)Vo - 1.0); //calculate resistance on thermistor
		logR2 = log(R2);
		T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2)); // temperature in Kelvin
		T = T - 273.15 - calibration;								//convert Kelvin to Celcius

		// T = (T * 9.0)/ 5.0 + 32.0; //convert Celcius to Farenheit
	}
	return T;
}