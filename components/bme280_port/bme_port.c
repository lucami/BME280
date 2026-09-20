/*
 * bme.c
 *
 *  Created on: Mar 7, 2026
 *      Author: milu
 */
 
#include "bme_port.h"
#include "bme_port_private.h"
//#include "../../core/bme280_core/bme280_core.h"

static i2c_master_dev_handle_t dev_handle;
static i2c_master_bus_handle_t bus_handle;
static QueueHandle_t sensorDataQueue;
static EventGroupHandle_t bme_event_group;

/*
static T_TemperatureCoefficient temp_coeff;
static T_HumidityCoefficient hum_coeff;
static T_PressureCoefficient pres_coeff;

static int32_t Temperature_not_calibrated;
static int32_t Humidity_not_calibrated;
static int32_t Pressure_not_calibrated;

static int32_t temperature_calibration_factor;
*/

EventGroupHandle_t getEventGroup()
{
	return bme_event_group;
}

QueueHandle_t* bme280Port_getQueueReference()
{
	return &sensorDataQueue;
}

BME280_ErrorCode_t bme280Port_read_T_coefficients(T_TemperatureCoefficient *ptrTC)
{
	uint8_t temp;
	esp_err_t rval = ESP_OK;

	rval |= i2c_register_read(dev_handle, BME280_T1_0_COEFF, &temp, 1);
	ptrTC->T1 = temp;
	rval |= i2c_register_read(dev_handle, BME280_T1_1_COEFF, &temp, 1);
	ptrTC->T1 += temp<<8;

	rval |= i2c_register_read(dev_handle, BME280_T2_0_COEFF, &temp, 1);
	ptrTC->T2 = temp;
	rval |= i2c_register_read(dev_handle, BME280_T2_1_COEFF, &temp, 1);
	ptrTC->T2 += temp<<8;
	
	rval |= i2c_register_read(dev_handle, BME280_T3_0_COEFF, &temp, 1);
	ptrTC->T3 = temp;
	rval |= i2c_register_read(dev_handle, BME280_T3_1_COEFF, &temp, 1);
	ptrTC->T3 += temp<<8;
	
	return (rval == ESP_OK) ? BME280_OK : BME280_ERR_GENERIC;
}

BME280_ErrorCode_t bme280Port_read_H_coefficients(T_HumidityCoefficient *ptrHC)
{
	uint8_t temp;
	esp_err_t rval = ESP_OK;

	rval |= i2c_register_read(dev_handle, BME280_H1_1_COEFF, &temp, 1);
	ptrHC->H1 = temp;
	
	rval |= i2c_register_read(dev_handle, BME280_H2_1_COEFF, &temp, 1);
	ptrHC->H2 = temp;
	rval |= i2c_register_read(dev_handle, BME280_H2_2_COEFF, &temp, 1);
	ptrHC->H2 += temp<<8;
	
	rval |= i2c_register_read(dev_handle, BME280_H3_1_COEFF, &temp, 1);
	ptrHC->H3 = temp;
		
	rval |= i2c_register_read(dev_handle, BME280_H4_1_COEFF, &temp, 1);
	ptrHC->H4 = temp<<4;
	rval |= i2c_register_read(dev_handle, BME280_H4_2_COEFF, &temp, 1);
	ptrHC->H4 += temp&0x0F;

	rval |= i2c_register_read(dev_handle, BME280_H5_1_COEFF, &temp, 1);
	ptrHC->H5 = temp>>4;//temp&0xF0;
	rval |= i2c_register_read(dev_handle, BME280_H5_2_COEFF, &temp, 1);
	ptrHC->H5 += temp<<4;
	
	rval |= i2c_register_read(dev_handle, BME280_H6_1_COEFF, &temp, 1);
	ptrHC->H6 = temp;
	
	return (rval == ESP_OK) ? BME280_OK : BME280_ERR_GENERIC;
}

BME280_ErrorCode_t bme280Port_read_P_coefficients(T_PressureCoefficient *ptrPC)
{
	uint8_t temp;
	esp_err_t rval = ESP_OK;

	rval |= i2c_register_read(dev_handle, BME280_P1_1_COEFF, &temp, 1);
	ptrPC->P1 = temp;
	rval |= i2c_register_read(dev_handle, BME280_P1_2_COEFF, &temp, 1);
	ptrPC->P1 += temp<<8;
		
	rval |= i2c_register_read(dev_handle, BME280_P2_1_COEFF, &temp, 1);
	ptrPC->P2 = temp;
	rval |= i2c_register_read(dev_handle, BME280_P2_2_COEFF, &temp, 1);
	ptrPC->P2 += temp<<8;
		
	rval |= i2c_register_read(dev_handle, BME280_P3_1_COEFF, &temp, 1);
	ptrPC->P3 = temp;
	rval |= i2c_register_read(dev_handle, BME280_P3_2_COEFF, &temp, 1);
	ptrPC->P3 += temp<<8;
	
	rval |= i2c_register_read(dev_handle, BME280_P4_1_COEFF, &temp, 1);
	ptrPC->P4 = temp;
	rval |= i2c_register_read(dev_handle, BME280_P4_2_COEFF, &temp, 1);
	ptrPC->P4 += temp<<8;

	rval |= i2c_register_read(dev_handle, BME280_P5_1_COEFF, &temp, 1);
	ptrPC->P5 = temp;
	rval |= i2c_register_read(dev_handle, BME280_P5_2_COEFF, &temp, 1);
	ptrPC->P5 += temp<<8;
		
	rval |= i2c_register_read(dev_handle, BME280_P6_1_COEFF, &temp, 1);
	ptrPC->P6 = temp;
	rval |= i2c_register_read(dev_handle, BME280_P6_2_COEFF, &temp, 1);
	ptrPC->P6 += temp<<8;

	rval |= i2c_register_read(dev_handle, BME280_P7_1_COEFF, &temp, 1);
	ptrPC->P7 = temp;
	rval |= i2c_register_read(dev_handle, BME280_P7_2_COEFF, &temp, 1);
	ptrPC->P7 += temp<<8;

	rval |= i2c_register_read(dev_handle, BME280_P8_1_COEFF, &temp, 1);
	ptrPC->P8 = temp;
	rval |= i2c_register_read(dev_handle, BME280_P8_2_COEFF, &temp, 1);
	ptrPC->P8 += temp<<8;

	rval |= i2c_register_read(dev_handle, BME280_P9_1_COEFF, &temp, 1);
	ptrPC->P9 = temp;
	rval |= i2c_register_read(dev_handle, BME280_P9_2_COEFF, &temp, 1);
	ptrPC->P9 += temp<<8;

	return (rval == ESP_OK) ? BME280_OK : BME280_ERR_GENERIC;
}

BME280_ErrorCode_t bme280Port_read_T_value(T_TemperatureCoefficient *ptrTC, int32_t *ptr)
{
	uint8_t t1,t2,t3;
	esp_err_t rval = ESP_OK;

	rval |= i2c_register_read(dev_handle, BME_T0_VAL, &t1, 1);
	rval |= i2c_register_read(dev_handle, BME_T1_VAL, &t2, 1);
	rval |= i2c_register_read(dev_handle, BME_T2_VAL, &t3, 1);
	
	*ptr = ((int32_t)(t3))>>4 | ((int32_t)(t2))<<4 | ((int32_t)(t1)) <<12; 
	return (rval == ESP_OK) ? BME280_OK : BME280_ERR_GENERIC;
}

BME280_ErrorCode_t bme280Port_read_H_value(T_HumidityCoefficient *ptrHC, int32_t *ptr)
{
	uint8_t h1,h2;
	int32_t h;
	esp_err_t rval = ESP_OK;

	rval |= i2c_register_read(dev_handle, BME_H0_VAL, &h1, 1);
	rval |= i2c_register_read(dev_handle, BME_H1_VAL, &h2, 1);
	
	h = h2|h1<<8;
	*ptr = h;
	return (rval == ESP_OK) ? BME280_OK : BME280_ERR_GENERIC;
}

BME280_ErrorCode_t bme280Port_read_P_value(T_PressureCoefficient *ptrPC, int32_t *ptr)
{
	uint8_t p1,p2,p3;
	esp_err_t rval = ESP_OK;

	rval |= i2c_register_read(dev_handle, BME_P0_VAL, &p1, 1);
	rval |= i2c_register_read(dev_handle, BME_P1_VAL, &p2, 1);
	rval |= i2c_register_read(dev_handle, BME_P2_VAL, &p3, 1);
	
	p3=p3&0xF0;
	*ptr=(p1<<12 | p2<<4 | (p3)>>4);
	
	return (rval == ESP_OK) ? BME280_OK : BME280_ERR_GENERIC;
}


BME280_ErrorCode_t bme280_port_init()
{
	uint8_t deviceID;
	esp_err_t rval = ESP_OK;

	i2c_master_init(&bus_handle, &dev_handle,BME280_SENSOR_ADDR);
	
	bme280_get_deviceID(&deviceID);
	printf("Device ID: %X", deviceID);
	
	rval |= i2c_register_write_byte(dev_handle, BME280_CONFIG, 0x50); // Tstandby 1s, no iir filter, no spi3wire	
	rval |= i2c_register_write_byte(dev_handle, BME280_CTRL_HUM, 0x01);	// humidity oversample Hos:no
	rval |= i2c_register_write_byte(dev_handle, BME280_CTRL_MEAS, 0x27); // temperature oversample [2:0] pressure oversample [2:0] mode [1:0]
	
	bme_event_group = xEventGroupCreate();

	sensorDataQueue = xQueueCreate(1, sizeof(BME280_Data_t));
	xEventGroupSetBits(bme_event_group, BME_IS_READY);


	return (rval == ESP_OK) ? BME280_OK : BME280_ERR_GENERIC;
}

BME280_ErrorCode_t bme280_get_deviceID(uint8_t *rval)
{
	int error = i2c_register_read(dev_handle, BME280_WHO_AM_I_REG_ADDR, rval, 1);
	return (error == ESP_OK) ? BME280_OK : BME280_ERR_GENERIC; 
}

BME280_ErrorCode_t bme280_get_P(uint32_t *rval)
{
	*rval = 0;
	return BME280_ERR_GENERIC;
}

BME280_ErrorCode_t bme280_get_T(uint32_t *rval)
{
	*rval = 0;
	return BME280_ERR_GENERIC;
}

BME280_ErrorCode_t bme280_get_v(uint32_t *rval)
{
	*rval = 0;
	return BME280_ERR_GENERIC;
}


