/*
 * i2c_driver.h
 *
 *  Created on: Mar 1, 2026
 *      Author: milu
 */

#ifndef MAIN_DRIVER_I2C_I2C_DRIVER_H_
#define MAIN_DRIVER_I2C_I2C_DRIVER_H_

#include "driver/i2c_master.h"


void i2c_master_init(i2c_master_bus_handle_t *bus_handle, i2c_master_dev_handle_t *dev_handle, uint8_t device_address);
esp_err_t i2c_register_write_byte(i2c_master_dev_handle_t dev_handle, uint8_t reg_addr, uint8_t data);
esp_err_t i2c_register_read(i2c_master_dev_handle_t dev_handle, uint8_t reg_addr, uint8_t *data, size_t len);

#endif /* MAIN_DRIVER_I2C_I2C_DRIVER_H_ */
