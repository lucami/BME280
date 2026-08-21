/*
 * bme_private.h
 *
 *  Created on: Mar 7, 2026
 *      Author: milu
 */

#include <inttypes.h>
#include "i2c_driver.h"

 
#ifndef COMPONENTS_BME280_BME_PRIVATE_H_
#define COMPONENTS_BME280_BME_PRIVATE_H_

//#define I2C_MASTER_SCL_IO           CONFIG_I2C_MASTER_SCL       /*!< GPIO number used for I2C master clock */
//#//define I2C_MASTER_SDA_IO           CONFIG_I2C_MASTER_SDA       /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM              I2C_NUM_0                   /*!< I2C port number for master dev */

//#define I2C_MASTER_FREQ_HZ          CONFIG_I2C_MASTER_FREQUENCY /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   0                           /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0                           /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS       1000

#define BME280_SENSOR_ADDR         0x76//0x68        /*!< Address of the MPU9250 sensor */
#define BME280_WHO_AM_I_REG_ADDR   0xD0//0x75        /*!< Register addresses of the "who am I" register */
#define BME280_PWR_MGMT_1_REG_ADDR 0xE0//0x6B        /*!< Register addresses of the power management register */
#define BME280_RESET_BIT           7
#define BME280_CONFIG			   0xF5
#define BME280_CTRL_MEAS		   0xF4
#define BME280_CTRL_HUM			   0xF2

#define BME280_T1_0_COEFF		   0x88
#define BME280_T1_1_COEFF		   0x89
#define BME280_T2_0_COEFF		   0x8A
#define BME280_T2_1_COEFF		   0x8B
#define BME280_T3_0_COEFF		   0x8C
#define BME280_T3_1_COEFF		   0x8D

#define BME280_H1_1_COEFF		   0xA1
#define BME280_H2_1_COEFF		   0xE1
#define BME280_H2_2_COEFF		   0xE2
#define BME280_H3_1_COEFF		   0xE3
#define BME280_H4_1_COEFF		   0xE4
#define BME280_H4_2_COEFF		   0xE5
#define BME280_H5_1_COEFF		   0xE5
#define BME280_H5_2_COEFF		   0xE6
#define BME280_H6_1_COEFF		   0xE7

#define BME280_P1_1_COEFF		   0x8E
#define BME280_P1_2_COEFF		   0x8F
#define BME280_P2_1_COEFF		   0x90
#define BME280_P2_2_COEFF		   0x91
#define BME280_P3_1_COEFF		   0x92
#define BME280_P3_2_COEFF		   0x93
#define BME280_P4_1_COEFF		   0x94
#define BME280_P4_2_COEFF		   0x95
#define BME280_P5_1_COEFF		   0x96
#define BME280_P5_2_COEFF		   0x97
#define BME280_P6_1_COEFF		   0x98
#define BME280_P6_2_COEFF		   0x99
#define BME280_P7_1_COEFF		   0x9A
#define BME280_P7_2_COEFF		   0x9B
#define BME280_P8_1_COEFF		   0x9C
#define BME280_P8_2_COEFF		   0x9D
#define BME280_P9_1_COEFF		   0x9E
#define BME280_P9_2_COEFF		   0x9F

#define BME_T0_VAL				   0xFA
#define BME_T1_VAL				   0xFB
#define BME_T2_VAL				   0xFC

#define BME_H0_VAL				   0xFD
#define BME_H1_VAL				   0xFE

#define BME_P0_VAL				   0xF7
#define BME_P1_VAL				   0xF8
#define BME_P2_VAL				   0xF9

#endif /* COMPONENTS_BME280_BME_PRIVATE_H_ */
