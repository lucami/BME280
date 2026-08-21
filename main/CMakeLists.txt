idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    PRIV_REQUIRES spi_flash
    REQUIRES i2c esp_wifi
    REQUIRES "bme280_core"  wifi
    REQUIRES "mqtt_client_comp"
)

                       