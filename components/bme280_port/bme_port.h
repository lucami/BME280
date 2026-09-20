/*
 * bme.h
 *
 *  Created on: Mar 7, 2026
 *      Author: milu
 */

#ifndef COMPONENTS_BME280_PORT_BME_PORT_H_
#define COMPONENTS_BME280_PORT_BME_PORT_H_

#include "freertos/idf_additions.h"
#include <inttypes.h>
 #include "freertos/FreeRTOS.h"
 #include "freertos/queue.h"
//#include "../../core/bme280_core/bme280_core.h"

typedef enum {
    BME280_OK = 0,
    BME280_ERR_GENERIC
} BME280_ErrorCode_t;


typedef struct 
 {
    int32_t t;
    int32_t h;
    int32_t p;
	uint32_t counter;
 } BME280_Data_t;


typedef struct {
	uint32_t T1;
	int16_t  T2;
	int16_t T3;
} T_TemperatureCoefficient;

typedef struct {
	int16_t H2,H4,H5;
	uint8_t H1,H3,H6;
} T_HumidityCoefficient;


typedef struct{
	uint16_t P1;
	int16_t P2,P3,P4,P5,P6,P7,P8,P9;
}T_PressureCoefficient;



#define BME280_SENSOR_ADDR         0x76

QueueHandle_t* bme280Port_getQueueReference();


BME280_ErrorCode_t bme280_init();
BME280_ErrorCode_t bme280_get_deviceID(uint8_t*);
BME280_ErrorCode_t bme280_get_P(uint32_t*);
BME280_ErrorCode_t bme280_get_T(uint32_t*);
BME280_ErrorCode_t bme280_get_v(uint32_t*);

BME280_ErrorCode_t bme280Port_read_T_coefficients(T_TemperatureCoefficient*);
BME280_ErrorCode_t bme280Port_read_H_coefficients(T_HumidityCoefficient*);
BME280_ErrorCode_t bme280Port_read_P_coefficients(T_PressureCoefficient*);

BME280_ErrorCode_t bme280Port_read_T_value(T_TemperatureCoefficient*, int32_t*);
BME280_ErrorCode_t bme280Port_read_H_value(T_HumidityCoefficient*, int32_t*);
BME280_ErrorCode_t bme280Port_read_P_value(T_PressureCoefficient*, int32_t*);
EventGroupHandle_t getEventGroup();

BME280_ErrorCode_t bme280_port_init();

#define BME_IS_READY 			BIT0
#define BME_NEW_DATA_AVAILABLE	BIT1


#endif /* COMPONENTS_BME280_PORT_BME_PORT_H_ */
