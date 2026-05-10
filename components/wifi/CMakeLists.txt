idf_component_register(
    SRCS "wifi_connect.c" "wifi_sendData.c"
    INCLUDE_DIRS "."
    REQUIRES esp_wifi esp_event esp_netif nvs_flash log freertos "bme280_port" "bme280_core"
)