/*
 * pressureSensorSPI.h
 *
 *  Created on: Mar 29, 2018
 *      Author: 402072495
 */
#ifndef USER_INC_PRESSURESENSORSPI_H_
#define USER_INC_PRESSURESENSORSPI_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f7xx_hal.h"

class PRESSURE_SENSORSPI{
public:
	PRESSURE_SENSORSPI();
	uint16_t rawData;
	int16_t rawSPIPressure;
	int16_t rawSPITemperature;
	float Pressure;
	float Temperature;
	int16_t uOutMin;
	int16_t uOutMax;
	float PMin;
	float PMax;
	SPI_HandleTypeDef *pressure_spi;
    GPIO_TypeDef *CS_Port;
    uint16_t	CS_Pin;
    float read();

private:
    void CS_LOW();
    void CS_HIGH();
};


#ifdef __cplusplus
}
#endif
#endif /* USER_INC_PRESSURESENSORSPI_H_ */

