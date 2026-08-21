#include <stdio.h>
#include "mqtt_sendData.h"
#include "esp_event.h"
#include "bme_port.h"
#include <wifi_connect.h>

extern EventGroupHandle_t wifi_event_group;

static QueueHandle_t *sensorDataQueueReference;
static EventGroupHandle_t bme_event_group = 0;

static esp_mqtt_client_config_t mqtt_cfg;
static esp_mqtt_client_handle_t mqtt_client;

void mqtt_Task(void *pvParameters)
{
	BME280_Data_t data;
	
	char payload[64];
	static uint32_t i=0;

	sensorDataQueueReference = bme280Port_getQueueReference();

	while(bme_event_group == 0)
		bme_event_group = getEventGroup();
	
	xEventGroupWaitBits(wifi_event_group,
	                        WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
	                        pdFALSE, pdFALSE, portMAX_DELAY);
	xEventGroupWaitBits(bme_event_group, BME_IS_READY, pdFALSE,
						pdFALSE, portMAX_DELAY);
	printf("\nReady to send mqtt data");

    esp_mqtt_client_start(mqtt_client);
		
	while(1)
	{
		
		xQueuePeek(*sensorDataQueueReference, &data, 1000);

		int len = snprintf(payload, 64, "\n%"PRIi32")T: %"PRIi32"; H: %"PRIi32"; P: %"PRIi32"", i++, data.t,data.h/1024,data.p/256/100);
		printf("\nmqtt send task running");
        //esp_mqtt_client_publish(client, topic, data, len, qos, retain)
		esp_mqtt_client_publish(mqtt_client, "casa/htp/", payload, len, 0, 0);

		vTaskDelay(1000*60 / portTICK_PERIOD_MS);
	}
	vTaskDelete(NULL);
}

void mqtt_init()
{
    mqtt_cfg.broker.address.uri = "mqtt://192.168.1.17:1883";

    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    xTaskCreate(mqtt_Task, "MQTT send", 2048, NULL, 1, NULL);

}