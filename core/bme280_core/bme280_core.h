/*
 * bme280_core.h
 *
 *  Created on: Mar 7, 2026
 *      Author: milu
 */

 #include <inttypes.h>

 uint8_t bme280_core_init();
 uint8_t bme280_core_deviceID(uint8_t*);
 uint8_t bme280_core_getTHP(int32_t *t, int32_t *h, int32_t *p);

