/* MQTT (over TCP) Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "esp_log.h"
#include "mqtt_client.h"

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
#define LED1_DIODE 14

esp_mqtt_client_config_t mqtt_cfg = {
    .uri = "mqtt://nakii.tech",
};
esp_mqtt_client_handle_t client;

char *Left;
char *Right;

char *ultrasonic = "ULS#";
char *water_sensor = "HUM#";
char *tracking_sensor = "TRC#";

char *ultrasonic_message;
char *water_sensor_message;
char *tracking_sensor_message;

int NumOfDigits(int number)
{
    int numDigi = 0;
    number = number / 10;
    while (number != 0)
    {
        numDigi++;
        number = number / 10;
    }
    numDigi++;
    return numDigi;
}

//Returned char* must be freed from memory
char *AddStrings(char *string1, char *string2)
{
    char *buff = (char *)malloc((strlen(string1) + strlen(string2) + 1) * sizeof(char));

    buff[strlen(string1) + strlen(string2)] = '\0';

    for (int i = 0; i < strlen(string1); i++)
    {
        buff[i] = string1[i];
    }
    for (int i = strlen(string1), j = 0; j < strlen(string2); i++, j++)
    {
        buff[i] = string2[j];
    }

    return buff;
}

//Returned char* must be freed from memory
char *AddStringNum(char *string1, int number)
{
    char *buff = (char *)malloc((strlen(string1) + NumOfDigits(number) + 1) * sizeof(char));

    buff[strlen(string1) + NumOfDigits(number)] = '\0';

    for (int i = 0; i < strlen(string1); i++)
    {
        buff[i] = string1[i];
    }

    char *numStr = (char *)malloc((NumOfDigits(number) + 1) * sizeof(char));
    numStr[NumOfDigits(number)] = '\0';

    sprintf(numStr, "%d", number);

    for (int i = strlen(string1), j = 0; j < NumOfDigits(number); i++, j++)
    {
        buff[i] = numStr[j];
    }

    free(numStr);
    return buff;
}

void SplitString(char c, char *toSplit)
{
    //Fill Left string till we hit char (include char in Left string!!!)

    if (strlen(toSplit) > 0)
    {

        Left = (char *)malloc(1 * sizeof(char));

        int currentIndex = 0;
        for (int i = 0; i < strlen(toSplit); i++)
        {

            //If we entered more than once then reallocate memory (increase memory by one char)
            if (i > 0)
            {
                Left = realloc(Left, i + 1);
            }

            //Append value of char c to allocated memory char
            if (toSplit[i] != c)
            {
                Left[i] = toSplit[i];
            }
            //If we hit char c then exit from alg loop for left string
            else
            {
                currentIndex = i;
                break;
            }
        }

        Left[currentIndex] = '\0';

        Right = (char *)malloc(1 * sizeof(char));

        //Grab all existing chars from right side
        for (int i = currentIndex + 1, j = 0; i < strlen(toSplit); i++, j++)
        {

            //If we entered more than once then reallocate memory (increase memory by one char)
            if (j > 0)
            {
                Right = realloc(Right, j + 1);
            }

            if (toSplit[i] != '\0')
            {
                Right[j] = toSplit[i];
                currentIndex = j;
            }
            else
            {
                break;
            }
        }
        Right = realloc(Right, currentIndex + 2);
        Right[currentIndex + 1] = '\0';
    }
}

void parseReceive(char *data, int data_len)
{
    //printf("Primio data i parsiram \n");
    //printf("DATA=%.*s\r\n", data_len, data);

    SplitString('#', data);
    printf("Received: Lef: %s  Right: %s \n", Left, Right);

    //Detect which sensor to affect
    if (!strcmp(Left, "LD0"))
    {
        char parsedEFUSE[1];

        parsedEFUSE[0] = Right[0];

        //printf("Parsed EFUSE: %s", parsedEFUSE);

        uint8_t vrijednost = (uint8_t)strtol(parsedEFUSE, NULL, 10);

        if (vrijednost == 1)
        {
            gpio_set_level(LED1_DIODE, 1);
        }
        else if (vrijednost == 0)
        {
            gpio_set_level(LED1_DIODE, 0);
        }
        else
        {
        }
    }
    else if (strcmp(Left, "LD1"))
    {
        char parsedEFUSE[1];

        parsedEFUSE[0] = Right[0];
        uint8_t vrijednost2 = (uint8_t)strtol(parsedEFUSE, NULL, 10);
        if (vrijednost2 == 1)
        {
            //kod ovdje
        }
        else if (vrijednost2 == 0)
        {
            //kod ovdje
        }
        else
        {
        }
    }
}

// mqtt config ------------------------------------------------------------------------------------------------------
static const char *TAG = "MQTT_EXAMPLE";

static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event)
{
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    switch (event->event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        msg_id = esp_mqtt_client_subscribe(client, "/esp/0", 0);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        msg_id = esp_mqtt_client_publish(client, "/esp/2", "Delic", 0, 0, 0);
        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);
        parseReceive(event->data, event->data_len);
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
    return ESP_OK;
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    mqtt_event_handler_cb(event_data);
}

//mqtt config end here -------------------------------------------------------------------------------------------------------

// deklaracija taska-------------------------------------------------
void send_task();

//main app ----------------------------------------------------------
void app_main(void)
{

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_ERROR_CHECK(example_connect());
    client = esp_mqtt_client_init(&mqtt_cfg);

    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);

    initDEBUG("", '5', 115200, "DSA - Debug example");

    // Konfiguracija trigger pina
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1 << TRIGGER_PIN);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

    //Konfiguracija echo pina
    gpio_config_t io_conf1;
    io_conf1.intr_type = GPIO_INTR_DISABLE;
    io_conf1.mode = GPIO_MODE_INPUT;
    io_conf1.pin_bit_mask = (1 << ECHO_PIN);
    io_conf1.pull_down_en = 0;
    io_conf1.pull_up_en = 0;
    gpio_config(&io_conf1);

    //konfiguracija hunt pina
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
    io_conf3.pin_bit_mask = (1 << LED1_DIODE);
    io_conf3.pull_down_en = 0;
    io_conf3.pull_up_en = 0;
    gpio_config(&io_conf3);

    adc_config_t adc_config;
    adc_config.mode = ADC_READ_TOUT_MODE;
    adc_config.clk_div = 8; // ADC sample collection clock = 80MHz/clk_div = 10MHz
    adc_init(&adc_config);

    xTaskCreate(send_task, "send_task", 1024, NULL, 10, NULL);
}

void send_task()
{
    uint8_t distance;
    uint16_t water_detector;
    uint8_t hunter;

    while (1)
    {
        water_detector = Get_Water_Value();
        distance = get_ultrasonic(TRIGGER_PIN, ECHO_PIN);
        hunter = Get_KY033_Hunt(HUNT_PIN);

        //printDEBUG(DSYS, "Distance %d\n", (uint8_t)distance);
        ultrasonic_message = AddStringNum(ultrasonic, (uint8_t)distance);
        //printDEBUG(DSYS, "hunter lvl: %d\n", hunter);
        tracking_sensor_message = AddStringNum(tracking_sensor, hunter);
        // printDEBUG(DSYS, "ADC: %d\n", water_detector);
        water_sensor_message = AddStringNum(water_sensor, water_detector);
        //vTaskDelay(2000 / portTICK_RATE_MS);
        //printDEBUG(DSYS, "Ultrasonic message: %s\n", ultrasonic_message);
        //printDEBUG(DSYS, "Water sensor message: %s\n", water_sensor_message);
        //printDEBUG(DSYS, "Tracking sensor message: %s\n", tracking_sensor_message);

        esp_mqtt_client_publish(client, "/esp/2", ultrasonic_message, 0, 1, 0);
        esp_mqtt_client_publish(client, "/esp/2", water_sensor_message, 0, 1, 0);
        esp_mqtt_client_publish(client, "/esp/2", tracking_sensor_message, 0, 1, 0);
        free(ultrasonic_message);
        free(water_sensor_message);
        free(tracking_sensor_message);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}