/*
 * pressureSensorSPI.c
 *
 *  Created on: Mar 29, 2018
 *      Author: 402072495
 */
#include <pressureSensorSPI.h>
#include "string.h"
#include <stdlib.h>
#include "main.h"
#include "spi.h"


static uint32_t DummyByte[] = { 0xFFFFFFFF};



PRESSURE_SENSORSPI::PRESSURE_SENSORSPI()
{
	rawData= 0;
	rawSPIPressure = 0;
	rawSPITemperature = 0;
	Pressure = 0;
	Temperature = 0;
	PMax = 60 * 6895; //60*6895 Pa
	PMin = 0;
	uOutMin = 0x0666;
	uOutMax = 0x3999;
	pressure_spi = &hspi6;
	CS_Port =DO_13_GPIO_Port;
	CS_Pin = DO_13_Pin;
}
float PRESSURE_SENSORSPI::read() {
	uint16_t tem[2];
	/*read SPI pressure and temperature, 4 bytes*/
	CS_LOW();
	HAL_SPI_TransmitReceive(pressure_spi, (uint8_t *) DummyByte, (uint8_t *) tem, 1, 1);
	CS_HIGH();
	rawSPIPressure = (int16_t) (tem[0] & 0x3FFF);
	Pressure = (float) (rawSPIPressure - uOutMin) / (uOutMax - uOutMin) * (PMax - PMin) + PMin; //unit Pa
	return Pressure;
}


/************************************HoneyWell 060PGSA3 gauge pressure sensor****************************************/
void PRESSURE_SENSORSPI::CS_LOW() {
	CS_Port->BSRR = (uint32_t) CS_Pin << 16U;
}

void PRESSURE_SENSORSPI::CS_HIGH() {
	CS_Port->BSRR = (uint32_t) CS_Pin;
}
