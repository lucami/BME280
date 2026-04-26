idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    PRIV_REQUIRES spi_flash
    REQUIRES i2c
    REQUIRES "bme280_core"
)

                       