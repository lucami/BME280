#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>
#include "freertos/idf_additions.h"
#include "freertos/task.h"
#include "../core/bme280_core/bme280_core.h"
#include "esp_wifi.h"
#include "wifi_connect.h"
#include "bme_port.h"
#include "mqtt_sendData.h"
#include "nvs_component.h"
#include "esp_sleep.h"

#define CONFIG_ESP_WIFI_AUTH_OPEN	1
#define WIFI_CONNECTED_BIT BIT0

#define TIMEOUT_MULTIPLIER_US_TO_S	1000000ULL //ULL sono 64 bit, UL sono 32bit

static QueueHandle_t *sensorDataQueueReference;

static EventGroupHandle_t s_wifi_event_group;
static int s_retry_num = 0;

static uint64_t timeout;


void ReadSensor_Task(void *pvParameters)
{
	uint8_t rval8;
	BME280_Data_t data;
	data.counter=0;

	static uint32_t i=0;
	bme280_core_init_normal();
	bme280_core_deviceID(&rval8);


	while(1)
	{
		bme280_core_getTHP(&data.t,&data.h,&data.p);
		xQueueOverwrite(*sensorDataQueueReference, &data);
		printf("\nMAIN: %"PRIi32") T: %"PRIi32"; H: %"PRIi32"; P: %"PRIi32"",data.counter++, data.t,data.h/1024,data.p/256/100);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}	
}

void low_power()
{
	char topic[64];
	size_t topic_len = 64;
	uint8_t rval8;
	int32_t t,h,p;
	BME280_ErrorCode_t bme280_rval;

	timeout = 10*TIMEOUT_MULTIPLIER_US_TO_S;

	nvs_init();
	
	bme280_rval = bme280_core_init_forced();

	if(bme280_rval != BME280_OK )
	{
		esp_deep_sleep(timeout); 
	}

	bme280_rval = bme280_core_getTHP(&t, &h, &p);

	if(bme280_rval != BME280_OK )
	{
		esp_deep_sleep(timeout); 
	}

	wifi_init();

	EventBits_t bits = xEventGroupWaitBits(wifi_event_group,
						WIFI_CONNECTED_BIT,
						pdFALSE, pdFALSE, pdMS_TO_TICKS(15000));

	if(bits != WIFI_CONNECTED_BIT)
	{
		esp_wifi_stop();
		esp_deep_sleep(timeout); 
	}
	
	mqtt_init_lp();

	bits = xEventGroupWaitBits(mqtt_lp_event, MQTT_CONNECTED_BIT,
										pdFALSE, pdFALSE, pdMS_TO_TICKS(5000));


	
	if (!(bits & MQTT_CONNECTED_BIT))
	{
		printf("Timeout - No MQTT connection\n");

		mqtt_stop();
		esp_wifi_stop();
		esp_deep_sleep(timeout); 
	}

	mqtt_send(t,h,p);
	bits = xEventGroupWaitBits(mqtt_lp_event, MQTT_PUBLISHED_BIT,
										pdFALSE, pdFALSE, pdMS_TO_TICKS(5000));
	if (!(bits & MQTT_PUBLISHED_BIT))
		printf("Timeout - No confirm from broker\n");

	mqtt_stop();
	esp_wifi_stop();

	esp_deep_sleep(timeout);

}

void app_main(void)
{

	low_power();
/*
	char topic[64];
	size_t topic_len = 64;
	uint8_t rval8;

	printf("Starting FreeRTOS tasks...\n");
	nvs_init();

	rval8 = nvs_readKey("CONFI", "TOPIC", topic, &topic_len);

	if(rval8 == NVS_ERROR_OPEN_NAMESPACE)
		nvs_writeKey("CONFI", "TOPIC", "config/location");


	wifi_start();
	mqtt_init();
	sensorDataQueueReference = bme280Port_getQueueReference();

	xTaskCreate(ReadSensor_Task, "Sensor Read", 2048, NULL, 1, NULL);
			
  	fflush(stdout);
  */  
}
