/*
 * spiSlave.c
 *
 *  Created on: Mar 9, 2020
 *      Author: Xiaojiao Chen
 *       Email: yii402072495@gmail.com
 */

#include "spiSlave.h"
#include "softArm.h"
#include "spi.h"
#include "string.h"
#include <inttypes.h>

extern SOFT_ARM softArm;

SPI_HandleTypeDef *hspiSlave=&hspi1;


void spiSlaveStart(){
	HAL_SPI_TransmitReceive_DMA(&hspi1, (uint8_t *)(&softArm.sensorData[0][0]),  (uint8_t *)(&softArm.commandDataBuffer[0][0]), sizeof(softArm.commandDataBuffer));
}



void slaveSPITxRxCpltCallback(SPI_HandleTypeDef *hspi){
	if(hspi==hspiSlave){
		/*Re-arm the SPI slave, ready to receive and transmit*/
		HAL_SPI_TransmitReceive_DMA(hspiSlave, (uint8_t *)(&softArm.sensorData[0][0]),  (uint8_t *)(&softArm.commandDataBuffer[0][0]), sizeof(softArm.commandDataBuffer));
		memcpy(&softArm.commandData[0][0],&softArm.commandDataBuffer[0][0],sizeof(softArm.commandDataBuffer));
	}
}

void slaveSPIErrorCallback(SPI_HandleTypeDef *hspi){
	if(hspi==hspiSlave){
		printf("Error SPI 1 communication with RPI4\r\n");
		/*If an error occurs, we also need to re-arm the SPI slave, ready to receive and transmit*/
		HAL_SPI_TransmitReceive_DMA(&hspi1, (uint8_t *)(&softArm.sensorData[0][0]),  (uint8_t *)(&softArm.commandDataBuffer[0][0]), sizeof(softArm.commandDataBuffer));

	}
}

