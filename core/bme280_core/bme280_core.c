/*
 * bme280_core.c
 *
 *  Created on: Mar 7, 2026
 *      Author: milu
 */

#include <stdint.h>
#include <stdio.h>
#include "bme280_core.h"
#include "bme_port.h"

 static T_TemperatureCoefficient temp_coeff;
 static T_HumidityCoefficient hum_coeff;
 static T_PressureCoefficient pres_coeff;

 static int32_t Temperature_not_calibrated;
 static int32_t Humidity_not_calibrated;
 static int32_t Pressure_not_calibrated;

 static int32_t temperature_calibration_factor;


 static BME280_ErrorCode_t read_all_coefficients()
 {
 	BME280_ErrorCode_t rval = BME280_OK;
 	rval |= bme280Port_read_T_coefficients(&temp_coeff);
 	rval |= bme280Port_read_H_coefficients(&hum_coeff);
	rval |= bme280Port_read_P_coefficients(&pres_coeff);
 	return rval;
 }

 static BME280_ErrorCode_t read_all_values()
 {
 	BME280_ErrorCode_t rval = BME280_OK;
 	rval |= bme280Port_read_T_value(&temp_coeff,&Temperature_not_calibrated);
 	rval |= bme280Port_read_H_value(&hum_coeff, &Humidity_not_calibrated);
 	rval |= bme280Port_read_P_value(&pres_coeff, &Pressure_not_calibrated);
 	return rval;
 }

 static int32_t compensate_T()
 {
 	int32_t var1, var2, T;
 	var1 = ((((Temperature_not_calibrated>>3) - ((int32_t)temp_coeff.T1<<1))) * ((int32_t)temp_coeff.T2)) >> 11;
 	var2 = (((((Temperature_not_calibrated>>4) - ((int32_t)temp_coeff.T1)) * ((Temperature_not_calibrated>>4) - ((int32_t)temp_coeff.T1))) >> 12) *	((int32_t)temp_coeff.T3)) >> 14;
        	temperature_calibration_factor = var1 + var2;
 	T =(temperature_calibration_factor*5+128)>>8;
    return T;
 }

 static uint32_t compensate_H() 
 {

     int32_t v_x1_u32r;
     v_x1_u32r = (temperature_calibration_factor-((int32_t)76800));
     v_x1_u32r = (((((Humidity_not_calibrated << 14)-(((int32_t)hum_coeff.H4) << 20)-(((int32_t)hum_coeff.H5) * v_x1_u32r)) +((int32_t)16384)) >> 15) * (((((((v_x1_u32r * ((int32_t)hum_coeff.H6)) >> 10) * (((v_x1_u32r * ((int32_t)hum_coeff.H3)) >> 11) + ((int32_t)32768))) >> 10) + ((int32_t)2097152)) * ((int32_t)hum_coeff.H2) + 8192) >> 14));
     v_x1_u32r = (v_x1_u32r-(((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * ((int32_t)hum_coeff.H1)) >> 4));
     v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
     v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);
     return (uint32_t)(v_x1_u32r>>12);
 }

 static uint32_t compensate_P()
 {
 	int64_t var1, var2, p;
 	var1 = ((int64_t)temperature_calibration_factor)-128000;
 	var2 = var1 * var1 * (int64_t)pres_coeff.P6;
 	var2 = var2 + ((var1*(int64_t)pres_coeff.P5)<<17);
 	var2 = var2 + (((int64_t)pres_coeff.P4)<<35);
 	var1 = ((var1 * var1 * (int64_t)pres_coeff.P3)>>8) + ((var1 * (int64_t)pres_coeff.P2)<<12);
     var1 = (((((int64_t)1)<<47)+var1))*((int64_t)pres_coeff.P1)>>33;
 	if (var1 == 0)
 	{
 		return 0; // avoid exception caused by division by zero
 	}
 	p = 1048576-Pressure_not_calibrated;
 	p = (((p<<31)-var2)*3125)/var1;
 	var1 = (((int64_t)pres_coeff.P9) * (p>>13) * (p>>13)) >> 25;
 	var2 = (((int64_t)pres_coeff.P8) * p) >> 19;
 	p = ((p + var1 + var2) >> 8) + (((int64_t)pres_coeff.P7)<<4);
 	return (uint32_t)p;
 }



 
 BME280_ErrorCode_t bme280_core_init()
 {
	BME280_ErrorCode_t rval = BME280_OK;
	printf("\nCore init");
 	rval |= bme280_port_init();
 	rval |= read_all_coefficients();


 	return rval;
 }

 BME280_ErrorCode_t bme280_core_deviceID(uint8_t *ptr)
 {
	return bme280_get_deviceID(ptr);
 }

 BME280_ErrorCode_t bme280_core_getTHP(int32_t *t, int32_t *h, int32_t *p)
 {
	BME280_ErrorCode_t rval = read_all_values();
	*t = compensate_T();
	*h = compensate_H();
	*p = compensate_P();

 	return rval;
 }