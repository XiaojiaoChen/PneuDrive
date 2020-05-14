/*
 * spiSlave.h
 *
 *  Created on: Mar 9, 2020
 *      Author: Xiaojiao Chen
 *       Email: yii402072495@gmail.com
 */

#ifndef SPISLAVE_H_
#define SPISLAVE_H_
#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f7xx.h"


void spiSlaveStart();

void slaveSPITxRxCpltCallback(SPI_HandleTypeDef *hspi);

void slaveSPIErrorCallback(SPI_HandleTypeDef *hspi);


#ifdef __cplusplus
}
#endif
#endif /* SPISLAVE_H_ */
