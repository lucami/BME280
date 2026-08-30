#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "mqtt_client.h"

#define TOPIC "casa/box3d/htp"

typedef enum {
    MQTT_INIT_STATUS = 0b0,
    MQTT_PRESENTING_STATUS = 0b1,
    MQTT_WORKING_STATUS = 0b10
} mqtt_fsm_status_t;

void mqtt_init();
