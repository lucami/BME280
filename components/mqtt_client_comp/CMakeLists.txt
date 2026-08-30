idf_component_register(
    SRCS "mqtt_sendData.c"
    INCLUDE_DIRS "."
    REQUIRES mqtt wifi esp_wifi esp_event esp_netif nvs_flash log freertos "bme280_port" "bme280_core" "nvs_component"
)