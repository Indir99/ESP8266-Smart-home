
#include "MyTimer.h";


TickType_t passed_Time = 0;
TaskHandle_t xTask_Timer_Handler;
//It is wise to take delay time(sampling time) of 100 ms to display in sync with timer
void TimerTick(void* pvParameters)
{
	passed_Time = 0;

	for (;;)
	{
		/* Simply increment value for time of int variable */
		vTaskDelay(10 / portTICK_PERIOD_MS);
		passed_Time += 10;
		
	}
}
void StartTimer()
{
	xTaskCreate(TimerTick, "TimerTick", 1024, NULL, 10, &xTask_Timer_Handler);
}

void ResumeTimer()
{
	vTaskResume(xTask_Timer_Handler);	
}

void StopTimer()
{
	vTaskSuspend(xTask_Timer_Handler);	
}

void RestartTimer()
{
	passed_Time = 0;	
}