#ifndef __AD7616BOARD_H
#define	__AD7616BOARD_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f7xx_hal.h"
#include <HardwareConfig.h>

#if (ADBOARD_NUM>0)
typedef struct AD7616_DEVICE_STRUCT {
	int16_t uChannel[AD7616_CHANNELNUM];
	float fChannel[AD7616_CHANNELNUM];
	float fMinVoltage;
	float fMaxVoltage;
	float fRange;
	float fMid;
	SPI_HandleTypeDef *AD_spi;
	GPIO_TypeDef *CONV_Port;
	uint16_t CONV_Pin;
	GPIO_TypeDef *CS_Port;
	uint16_t CS_Pin;
	GPIO_TypeDef *BUSY_Port;
	uint16_t BUSY_Pin;
	uint16_t chosen;
	uint8_t ucDataFlag;
} AD7616_DEVICE;



typedef struct AD_DEVICES_STRUCT{
	AD7616_DEVICE ADBoards[ADBOARD_NUM];


	uint16_t curADBoardNum;
	uint16_t ucDataFlag;

}AD_BOARDS;



extern AD_BOARDS ADBoardDevices;





void ADBoard_updateVoltage();
void ADBoard_Reset();
int ADBoard_BUSYCallback(uint16_t GPIO_Pin);
int ADBoard_SPICallback(SPI_HandleTypeDef *hspi);
float AnalogRead_External(uint16_t num);
void ADBoard_Init();
#endif

#ifdef __cplusplus
}
#endif
#endif /* __AD7616BOARD_H */



