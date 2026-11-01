#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "mqtt_client.h"

#define TOPIC "casa/box3d/htp"

#define MQTT_CONNECTED_BIT  BIT0
#define MQTT_PUBLISHED_BIT  BIT1

extern EventGroupHandle_t mqtt_lp_event;


typedef enum {
    MQTT_INIT_STATUS = 0b0,
    MQTT_PRESENTING_STATUS = 0b1,
    MQTT_WORKING_STATUS = 0b10
} mqtt_fsm_status_t;

void mqtt_init(void );
void mqtt_init_lp(void);
void mqtt_send(int32_t, int32_t, int32_t);
void mqtt_stop();
