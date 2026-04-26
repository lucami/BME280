/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "../core/bme280_core/bme280_core.h"

//#include "driver/i2c_master.h"
//#include "driver/i2c/i2c_driver.h"
//
//#define CONFIG_I2C_MASTER_SCL 4
//#define CONFIG_I2C_MASTER_SDA 5
//#define CONFIG_I2C_MASTER_FREQUENCY 400000
//
//static const char *TAG = "example";
//
// #define I2C_MASTER_SCL_IO           CONFIG_I2C_MASTER_SCL       /*!< GPIO
// number used for I2C master clock */ #define I2C_MASTER_SDA_IO
// CONFIG_I2C_MASTER_SDA       /*!< GPIO number used for I2C master data  */
// #define I2C_MASTER_NUM              I2C_NUM_0                   /*!< I2C port
// number for master dev */
//
// #define I2C_MASTER_FREQ_HZ          CONFIG_I2C_MASTER_FREQUENCY /*!< I2C
// master clock frequency */ #define I2C_MASTER_TX_BUF_DISABLE   0 /*!< I2C
// master doesn't need buffer */ #define I2C_MASTER_RX_BUF_DISABLE   0 /*!< I2C
// master doesn't need buffer */ #define I2C_MASTER_TIMEOUT_MS       1000
//
// #define BME280_SENSOR_ADDR         0x76//0x68        /*!< Address of the
// MPU9250 sensor */ #define BME280_WHO_AM_I_REG_ADDR   0xD0//0x75        /*!<
// Register addresses of the "who am I" register */ #define
// BME280_PWR_MGMT_1_REG_ADDR 0xE0//0x6B        /*!< Register addresses of the
// power management register */ #define BME280_RESET_BIT           7 #define
// BME280_CONFIG			   0xF5 #define BME280_CTRL_MEAS		   0xF4
// #define BME280_CTRL_HUM			   0xF2
//
// #define BME280_T1_0_COEFF		   0x88
// #define BME280_T1_1_COEFF		   0x89
// #define BME280_T2_0_COEFF		   0x8A
// #define BME280_T2_1_COEFF		   0x8B
// #define BME280_T3_0_COEFF		   0x8C
// #define BME280_T3_1_COEFF		   0x8D
//
// #define BME280_H1_1_COEFF		   0xA1
// #define BME280_H2_1_COEFF		   0xE1
// #define BME280_H2_2_COEFF		   0xE2
// #define BME280_H3_1_COEFF		   0xE3
// #define BME280_H4_1_COEFF		   0xE4
// #define BME280_H4_2_COEFF		   0xE5
// #define BME280_H5_1_COEFF		   0xE5
// #define BME280_H5_2_COEFF		   0xE6
// #define BME280_H6_1_COEFF		   0xE7
//
// #define BME280_P1_1_COEFF		   0x8E
// #define BME280_P1_2_COEFF		   0x8F
// #define BME280_P2_1_COEFF		   0x90
// #define BME280_P2_2_COEFF		   0x91
// #define BME280_P3_1_COEFF		   0x92
// #define BME280_P3_2_COEFF		   0x93
// #define BME280_P4_1_COEFF		   0x94
// #define BME280_P4_2_COEFF		   0x95
// #define BME280_P5_1_COEFF		   0x96
// #define BME280_P5_2_COEFF		   0x97
// #define BME280_P6_1_COEFF		   0x98
// #define BME280_P6_2_COEFF		   0x99
// #define BME280_P7_1_COEFF		   0x9A
// #define BME280_P7_2_COEFF		   0x9B
// #define BME280_P8_1_COEFF		   0x9C
// #define BME280_P8_2_COEFF		   0x9D
// #define BME280_P9_1_COEFF		   0x9E
// #define BME280_P9_2_COEFF		   0x9F
//
// #define BME_T0_VAL				   0xFA
// #define BME_T1_VAL				   0xFB
// #define BME_T2_VAL				   0xFC
//
// #define BME_H0_VAL				   0xFD
// #define BME_H1_VAL				   0xFE
//
// #define BME_P0_VAL				   0xF7
// #define BME_P1_VAL				   0xF8
// #define BME_P2_VAL				   0xF9
//
//typedef struct {
//	uint32_t T1;
//	int32_t  T2;
//	int32_t T3;
//} T_TemperatureCoefficient;
//
//typedef struct {
//	int16_t H2,H4,H5;
//	uint8_t H1,H3,H6;
//} T_HumidityCoefficient;
//
//
//typedef struct{
//	uint16_t P1;
//	int16_t P2,P3,P4,P5,P6,P7,P8,P9;
//}T_PressureCoefficient;
//
//
//int32_t notCalibratedTemp=0;
//
//static esp_err_t bme820_register_write_byte(i2c_master_dev_handle_t dev_handle, uint8_t reg_addr, uint8_t data)
//{
//    uint8_t write_buf[2] = {reg_addr, data};
//    return i2c_master_transmit(dev_handle, write_buf, sizeof(write_buf), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
//}
//
//
//static esp_err_t bme820_register_read(i2c_master_dev_handle_t dev_handle, uint8_t reg_addr, uint8_t *data, size_t len)
//{
//    return i2c_master_transmit_receive(dev_handle, &reg_addr, 1, data, len, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
//}
//
//static void i2c_master_init(i2c_master_bus_handle_t *bus_handle, i2c_master_dev_handle_t *dev_handle)
//{
//	i2c_master_bus_config_t bus_config = {
//		.i2c_port = I2C_MASTER_NUM,
//		.sda_io_num = CONFIG_I2C_MASTER_SDA,
//		.scl_io_num = I2C_MASTER_SCL_IO,
//		.clk_source = I2C_CLK_SRC_DEFAULT,
//		.glitch_ignore_cnt = 7,
//		.flags.enable_internal_pullup = true,
//	};
//	ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, bus_handle));
//
//    i2c_device_config_t dev_config = {
//        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
//        .device_address = BME280_SENSOR_ADDR,
//        .scl_speed_hz = I2C_MASTER_FREQ_HZ,
//    };
//    ESP_ERROR_CHECK(i2c_master_bus_add_device(*bus_handle, &dev_config, dev_handle));
//}
//
//
//static void bme820_read_T_Coefficient(i2c_master_dev_handle_t dev_handle, T_TemperatureCoefficient *ptrTC)
//{
//	uint8_t temp;
//	
//	bme820_register_read(dev_handle, BME280_T1_0_COEFF, &temp, 1);
//	ptrTC->T1 = temp;
//	bme820_register_read(dev_handle, BME280_T1_1_COEFF, &temp, 1);
//	ptrTC->T1 += temp<<8;
//
//	bme820_register_read(dev_handle, BME280_T2_0_COEFF, &temp, 1);
//	ptrTC->T2 = temp;
//	bme820_register_read(dev_handle, BME280_T2_1_COEFF, &temp, 1);
//	ptrTC->T2 += temp<<8;
//	
//	bme820_register_read(dev_handle, BME280_T3_0_COEFF, &temp, 1);
//	ptrTC->T3 = temp;
//	bme820_register_read(dev_handle, BME280_T3_1_COEFF, &temp, 1);
//	ptrTC->T3 += temp<<8;
//}
//
//static void bme820_read_H_Coefficient(i2c_master_dev_handle_t dev_handle, T_HumidityCoefficient *ptrHC)
//{
//	uint8_t temp;
//	
//	bme820_register_read(dev_handle, BME280_H1_1_COEFF, &temp, 1);
//	ptrHC->H1 = temp;
//	
//	bme820_register_read(dev_handle, BME280_H2_1_COEFF, &temp, 1);
//	ptrHC->H2 = temp;
//	bme820_register_read(dev_handle, BME280_H2_2_COEFF, &temp, 1);
//	ptrHC->H2 += temp<<8;
//	
//	bme820_register_read(dev_handle, BME280_H3_1_COEFF, &temp, 1);
//	ptrHC->H3 = temp;
//		
//	bme820_register_read(dev_handle, BME280_H4_1_COEFF, &temp, 1);
//	ptrHC->H4 = temp<<4;
//	bme820_register_read(dev_handle, BME280_H4_2_COEFF, &temp, 1);
//	ptrHC->H4 += temp&0x0F;
//
//	bme820_register_read(dev_handle, BME280_H5_1_COEFF, &temp, 1);
//	ptrHC->H5 = temp&0xF0;
//	bme820_register_read(dev_handle, BME280_H5_2_COEFF, &temp, 1);
//	ptrHC->H5 += temp<<4;
//	
//	bme820_register_read(dev_handle, BME280_H6_1_COEFF, &temp, 1);
//	ptrHC->H6 = temp;
//		
//}
//
//static void bme820_read_P_Coefficient(i2c_master_dev_handle_t dev_handle, T_PressureCoefficient *ptrPC)
//{
//	uint8_t temp;
//	
//	bme820_register_read(dev_handle, BME280_P1_1_COEFF, &temp, 1);
//	ptrPC->P1 = temp;
//	bme820_register_read(dev_handle, BME280_P1_2_COEFF, &temp, 1);
//	ptrPC->P1 += temp<<8;
//		
//	bme820_register_read(dev_handle, BME280_P2_1_COEFF, &temp, 1);
//	ptrPC->P2 = temp;
//	bme820_register_read(dev_handle, BME280_P2_2_COEFF, &temp, 1);
//	ptrPC->P2 += temp<<8;
//		
//	bme820_register_read(dev_handle, BME280_P3_1_COEFF, &temp, 1);
//	ptrPC->P3 = temp;
//	bme820_register_read(dev_handle, BME280_P3_2_COEFF, &temp, 1);
//	ptrPC->P3 += temp<<8;
//	
//	bme820_register_read(dev_handle, BME280_P4_1_COEFF, &temp, 1);
//	ptrPC->P4 = temp;
//	bme820_register_read(dev_handle, BME280_P4_2_COEFF, &temp, 1);
//	ptrPC->P4 += temp<<8;
//
//	bme820_register_read(dev_handle, BME280_P5_1_COEFF, &temp, 1);
//	ptrPC->P5 = temp;
//	bme820_register_read(dev_handle, BME280_P5_2_COEFF, &temp, 1);
//	ptrPC->P5 += temp<<8;
//		
//	bme820_register_read(dev_handle, BME280_P6_1_COEFF, &temp, 1);
//	ptrPC->P6 = temp;
//	bme820_register_read(dev_handle, BME280_P6_2_COEFF, &temp, 1);
//	ptrPC->P6 += temp<<8;
//
//	bme820_register_read(dev_handle, BME280_P7_1_COEFF, &temp, 1);
//	ptrPC->P7 = temp;
//	bme820_register_read(dev_handle, BME280_P7_2_COEFF, &temp, 1);
//	ptrPC->P7 += temp<<8;
//
//	bme820_register_read(dev_handle, BME280_P8_1_COEFF, &temp, 1);
//	ptrPC->P8 = temp;
//	bme820_register_read(dev_handle, BME280_P8_2_COEFF, &temp, 1);
//	ptrPC->P8 += temp<<8;
//
//	bme820_register_read(dev_handle, BME280_P9_1_COEFF, &temp, 1);
//	ptrPC->P9 = temp;
//	bme820_register_read(dev_handle, BME280_P9_2_COEFF, &temp, 1);
//	ptrPC->P9 += temp<<8;
//}
//
//static int32_t bme820_read_T_Values(i2c_master_dev_handle_t dev_handle, T_TemperatureCoefficient *ptrTC)
//{
//	uint8_t t1,t2,t3;
//	
//	bme820_register_read(dev_handle, BME_T0_VAL, &t1, 1);
//	bme820_register_read(dev_handle, BME_T1_VAL, &t2, 1);
//	bme820_register_read(dev_handle, BME_T2_VAL, &t3, 1);
//	
//	notCalibratedTemp = ((int32_t)(t3))>>4 | ((int32_t)(t2))<<4 | ((int32_t)(t1)) <<12; 
//	
//	return notCalibratedTemp;
//}
//
//static int32_t bme820_read_H_Values(i2c_master_dev_handle_t dev_handle)
//{
//	uint8_t h1,h2;
//	int32_t h;
//	
//	bme820_register_read(dev_handle, BME_H0_VAL, &h1, 1);
//	bme820_register_read(dev_handle, BME_H1_VAL, &h2, 1);
//	
//	h = h2|h1<<8;
//	
//	return h;
//}
//
//static int32_t bme820_read_P_Values(i2c_master_dev_handle_t dev_handle)
//{
//	uint8_t p1,p2,p3;
//	
//	bme820_register_read(dev_handle, BME_P0_VAL, &p1, 1);
//	bme820_register_read(dev_handle, BME_P1_VAL, &p2, 1);
//	bme820_register_read(dev_handle, BME_P2_VAL, &p3, 1);
//	p3=p3&0xF0;
//	return p1<<12 | p2<<4 | (p3)>>4;
//}


void app_main(void)
{
	
//	uint8_t data[2];
//    printf("Hello world!\n");
//
//    /* Print chip information */
//    esp_chip_info_t chip_info;
//    uint32_t flash_size;
//    esp_chip_info(&chip_info);
//    printf("This is %s chip with %d CPU core(s), %s%s%s%s, ",
//           CONFIG_IDF_TARGET,
//           chip_info.cores,
//           (chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "WiFi/" : "",
//           (chip_info.features & CHIP_FEATURE_BT) ? "BT" : "",
//           (chip_info.features & CHIP_FEATURE_BLE) ? "BLE" : "",
//           (chip_info.features & CHIP_FEATURE_IEEE802154) ? ", 802.15.4 (Zigbee/Thread)" : "");
//
//    unsigned major_rev = chip_info.revision / 100;
//    unsigned minor_rev = chip_info.revision % 100;
//    printf("silicon revision v%d.%d, ", major_rev, minor_rev);
//    if(esp_flash_get_size(NULL, &flash_size) != ESP_OK) {
//        printf("Get flash size failed");
//        return;
//    }
//
//    printf("%" PRIu32 "MB %s flash\n", flash_size / (uint32_t)(1024 * 1024),
//           (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");
//
//    printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());
//
//	
//	i2c_master_bus_handle_t bus_handle;
//	i2c_master_dev_handle_t dev_handle;
//	printf("Running master init\n");
//BME280_SENSOR_ADDR
//	i2c_master_init(&bus_handle, &dev_handle);
//	bme820_register_read(dev_handle, BME280_WHO_AM_I_REG_ADDR, data, 1);
//	printf("Device ID: 0x%X 0x%X\n", data[0], data[1]);
//	
//	
//	bme820_register_write_byte(dev_handle, BME280_CONFIG, 0x50); 	// Tstandby 1s, no iir filter, no spi3wire
//	bme820_register_write_byte(dev_handle, BME280_CTRL_HUM, 0x01);	// humidity oversample Hos:no
//	bme820_register_write_byte(dev_handle, BME280_CTRL_MEAS, 0x27); // temperature oversample [2:0] pressure oversample [2:0] mode [1:0]
//																					// 0x27 -> 0010 0111 -> 001 001 11 -> Tos:no; Pos:no; Normal mode
//		
//	T_TemperatureCoefficient temp_coeff;
//	T_HumidityCoefficient hum_coeff;
//	T_PressureCoefficient press_coeff;
//	
//	bme820_read_T_Coefficient(dev_handle, &temp_coeff);	
//	bme820_read_H_Coefficient(dev_handle, &hum_coeff);
//	bme820_read_P_Coefficient(dev_handle, &press_coeff);
//	
//	//printf("Coefficient:\n  %"PRIu32"\n  %"PRIi32"\n  %"PRIi32"\n", temp_coeff.T1, temp_coeff.T2, temp_coeff.T3);
//	
//	while(1)
//	{
//		printf("- Temperature: %"PRIu32"			\n", compensate_T(bme820_read_T_Values(dev_handle, &temp_coeff), &temp_coeff));
//		printf("- Humidity: %"PRIu32"			\n", compensate_H(bme820_read_H_Values(dev_handle), &hum_coeff)/1024);
//		printf("- Pressure: %"PRIu32"			\n", compensate_P(bme820_read_P_Values(dev_handle), &press_coeff)/256);
//		vTaskDelay(1000 / portTICK_PERIOD_MS);
//	}
//	
//
//	esp_restart();
	
	
  uint8_t rval8;
  int32_t h,t,p;
  
  bme280_core_init();
  bme280_core_deviceID(&rval8);

  
  while(1)
  {
	bme280_core_getTHP(&t,&h,&p);
	printf("\nT: %"PRIi32"; H: %"PRIi32"; P: %"PRIi32"", t,h/1024,p/256/100);
	vTaskDelay(1000 / portTICK_PERIOD_MS);
}
  
  fflush(stdout);
    
}
