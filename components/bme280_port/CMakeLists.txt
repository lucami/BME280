idf_component_register(
    SRCS "bme_port.c"
    INCLUDE_DIRS "."
    REQUIRES i2c driver
)