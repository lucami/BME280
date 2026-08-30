#define _PRESENTATION_TIME  (30000 / portTICK_PERIOD_MS)
#define _RUNNING_TIME       (60000 / portTICK_PERIOD_MS)
#define _PRESENTATION_TOPIC "casa/presenting"
#define _BROKER_ADDRESS      "mqtt://192.168.1.17:1883"

#define _MQTT_NVS_NAMESPACE "config" 
#define _MQTT_NVS_TOPIC_KEY "location"
#define _MQTT_SET_TOPIC "casa/set"