
/*
 * i2c_driver.h
 *
 *  Created on: Mar 1, 2026
 *      Author: milu
 */

#ifndef MAIN_DRIVER_I2C_I2C_DRIVER_H_
#define MAIN_DRIVER_I2C_I2C_DRIVER_H_

#include "driver/i2c_master.h"

#define I2C_MASTER_NUM         I2C_NUM_0
#define I2C_MASTER_SDA_IO      5
#define I2C_MASTER_SCL_IO      4
#define I2C_MASTER_FREQ_HZ     100000
#define I2C_MASTER_TIMEOUT_MS  1000


void i2c_master_init(i2c_master_bus_handle_t *bus_handle, i2c_master_dev_handle_t *dev_handle, uint8_t device_address);
esp_err_t i2c_register_write_byte(i2c_master_dev_handle_t dev_handle, uint8_t reg_addr, uint8_t data);
esp_err_t i2c_register_read(i2c_master_dev_handle_t dev_handle, uint8_t reg_addr, uint8_t *data, size_t len);

#endif /* MAIN_DRIVER_I2C_I2C_DRIVER_H_ */
