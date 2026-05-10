#include <stdio.h>
#include <wifi_sendData.h>
#include "esp_event.h"
#include <wifi_connect.h>
#include "bme_port.h"

#include "bme280_core.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"

static EventGroupHandle_t bme_event_group = 0;
extern EventGroupHandle_t wifi_event_group;


void wifi_sendData(void *pvParameters)
{
	int32_t h,t,p;
	char payload[64];
	
	while(bme_event_group == 0)
		bme_event_group = getEventGroup();
	
	xEventGroupWaitBits(wifi_event_group,
	                        WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
	                        pdFALSE, pdFALSE, portMAX_DELAY);
	xEventGroupWaitBits(bme_event_group, BME_IS_READY, pdFALSE,
						pdFALSE, portMAX_DELAY);
	printf("\nReady to send data");

	int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	struct sockaddr_in dest = {
	        .sin_family      = AF_INET,
	        .sin_port        = htons(6660),
	        .sin_addr.s_addr = inet_addr("192.168.1.10")
	    };
	while(1)
	{
		
		bme280_core_getTHP(&t,&h,&p);
		int len = snprintf(payload, 64, "\nT: %"PRIi32"; H: %"PRIi32"; P: %"PRIi32"", t,h/1024,p/256/100);
		printf("\nwifi send task running");
		int err = sendto(sock, payload, len, 0,(struct sockaddr *)&dest, sizeof(dest));

		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
	vTaskDelete(NULL);
}

