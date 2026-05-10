/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "../core/bme280_core/bme280_core.h"
#include "esp_wifi.h"
#include "wifi_connect.h"
#include "wifi_sendData.h"



#define CONFIG_ESP_WIFI_AUTH_OPEN	1
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1


static EventGroupHandle_t s_wifi_event_group;
static int s_retry_num = 0;



void ReadSensor_Task(void *pvParameters)
{
	uint8_t rval8;
	int32_t h,t,p;

	bme280_core_init();
	bme280_core_deviceID(&rval8);


	while(1)
	{
		bme280_core_getTHP(&t,&h,&p);
		printf("\nT: %"PRIi32"; H: %"PRIi32"; P: %"PRIi32"", t,h/1024,p/256/100);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}	
}

void app_main(void)
{
	printf("Starting FreeRTOS tasks...\n");
	wifi_start();
	xTaskCreate(ReadSensor_Task, "Sensor Read", 2048, NULL, 1, NULL);
  
	
					
  	fflush(stdout);
    
}
