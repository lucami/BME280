/*
 * wifi_connect.h
 *
 *  Created on: Apr 29, 2026
 *      Author: milu
 */

#ifndef COMPONENTS_WIFI_WIFI_CONNECT_H_
#define COMPONENTS_WIFI_WIFI_CONNECT_H_

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

extern EventGroupHandle_t wifi_event_group;

void wifi_start(void);

#endif /* COMPONENTS_WIFI_WIFI_CONNECT_H_ */
