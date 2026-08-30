#include <stdio.h>
#include <stdlib.h>
#include "mqtt_sendData.h"
#include "esp_event.h"
#include "bme_port.h"
#include <wifi_connect.h>
#include "nvs_component.h"
#include "mqtt_private.h"

extern EventGroupHandle_t wifi_event_group;

static QueueHandle_t *sensorDataQueueReference;
static EventGroupHandle_t bme_event_group = 0;

static esp_mqtt_client_config_t mqtt_cfg;
static esp_mqtt_client_handle_t mqtt_client;

static char mqtt_topic[64];

static size_t key_length = 64;
static mqtt_fsm_status_t mqtt_status = MQTT_INIT_STATUS;

uint32_t update_status_get_timeout()
{
	static uint8_t iteration_counter = 0;
	uint32_t timeout = _RUNNING_TIME; //60s

	if(mqtt_status == MQTT_PRESENTING_STATUS)
	{
		timeout = _PRESENTATION_TIME;
		iteration_counter++;
		printf("\n debug setting timeout to presentation");
	}

	if (iteration_counter > 2)
	{
		if(strcmp(mqtt_topic, "unregistered") == 0)
		{ 
			printf("\n debug Restarting devcice to get a new topic from NVS\n");
			esp_restart();
		}
		else
		{
			esp_mqtt_client_unsubscribe(mqtt_client, _MQTT_SET_TOPIC);
		}

		mqtt_status = MQTT_WORKING_STATUS;
	}

	return timeout;
}

void get_mqtt_payload(char* payload, uint8_t *length, char* mqtt_current_topic, uint8_t *topic_length)
{
	BME280_Data_t data;
	if(mqtt_status == MQTT_PRESENTING_STATUS)
	{
		*length = snprintf(payload, 64, "%s", mqtt_topic);
		*topic_length = sprintf(mqtt_current_topic, _PRESENTATION_TOPIC);
	}
	else
	{
		xQueuePeek(*sensorDataQueueReference, &data, 1000);
		*length = snprintf(payload, 64, "{\"t\":%"PRIi32",\"h\":%"PRIi32",\"p\":%"PRIi32"}", data.t, data.h/1024, data.p/256/100);		
		*topic_length = snprintf(mqtt_current_topic, 64, "%s", mqtt_topic);
	}
	
}

void mqtt_Task(void *pvParameters)
{
	BME280_Data_t data;
	
	char payload[64], mqtt_current_topic[64];
	static uint32_t i=0;
	uint8_t topic_length;
	uint8_t length;
	uint32_t timeout = 30000 / portTICK_PERIOD_MS; //30s

	while(bme_event_group == 0)
		bme_event_group = getEventGroup();
	
	xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT | WIFI_FAIL_BIT, pdFALSE, pdFALSE, portMAX_DELAY);
	xEventGroupWaitBits(bme_event_group, BME_IS_READY, pdFALSE,pdFALSE, portMAX_DELAY);

	printf("\ndebug Ready to send mqtt data");

    esp_mqtt_client_start(mqtt_client);
		
	while(1)
	{
		get_mqtt_payload(payload, &length, mqtt_current_topic, &topic_length);
		printf("\ndebug mqtt send task running\n");

		esp_mqtt_client_publish(mqtt_client, mqtt_current_topic	, payload, length, 0, 0);

		timeout = update_status_get_timeout();
		printf("\ndebug timeout: %"PRIi32"\n", timeout/portTICK_PERIOD_MS);

		vTaskDelay(timeout);
	}
	vTaskDelete(NULL);
}


void mqtt_event_handler_function(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;
    switch (event_id)
    {
        case MQTT_EVENT_CONNECTED:
            esp_mqtt_client_subscribe(mqtt_client, _MQTT_SET_TOPIC, 0);
            break;

        case MQTT_EVENT_DATA:
			char *payload = malloc(event->data_len+1);
			memcpy(payload, event->data, event->data_len);
			payload[event->data_len] = 0;
			printf("\n debug MQTT_EVENT_DATA: TOPIC=%s\r\n", payload);

			nvs_writeKey(_MQTT_NVS_NAMESPACE, _MQTT_NVS_TOPIC_KEY, payload);
			memcpy(mqtt_topic, payload, event->data_len+1);

			esp_mqtt_client_unsubscribe(mqtt_client, _MQTT_SET_TOPIC);
			
			free(payload);

            break;
        default:
            break;
    }

}

void mqtt_init()
{
	
	if(nvs_readKey(_MQTT_NVS_NAMESPACE, _MQTT_NVS_TOPIC_KEY, mqtt_topic, &key_length) != NVS_OK)
	{
		sprintf(mqtt_topic, "unregistered");
	}

	printf("\n debug MQTT topic: %s\n", mqtt_topic);

    mqtt_cfg.broker.address.uri = _BROKER_ADDRESS;
    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
	mqtt_status = MQTT_PRESENTING_STATUS;

	esp_err_t rval = esp_mqtt_client_register_event(mqtt_client, ESP_EVENT_ANY_ID, mqtt_event_handler_function, NULL);

	sensorDataQueueReference = bme280Port_getQueueReference();
    xTaskCreate(mqtt_Task, "MQTT send", 2048, NULL, 1, NULL);

}

