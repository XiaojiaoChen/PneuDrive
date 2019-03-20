/*
 * PWMBoardSPI.h
 *
 *  Created on: Dec 28, 2018
 *      Author: Xiaojiao Chen
 *		 Email: chen2014@connect.hku.hk
 *	Laboratory: Bio-Control Lab
 * Orgnization: the University of Hong Kong
 */

#ifndef FRAMEWORK_FRAMEINC_PWMBOARDSPI_H_
#define FRAMEWORK_FRAMEINC_PWMBOARDSPI_H_

#ifdef __cplusplus
 extern "C" {
#endif


#include <stm32f7xx_hal.h>
#include <HardwareConfig.h>
#include <stdio.h>

#if PWMBOARDSPI_NUM>0

#define LT8500_FrameLength 73 //One frame is 584 bit length, including 12bit * 48 channel PWM + 8bit CMD
#define PWMBoardSPI_FrameLength LT8500_FrameLength*PWMBOARDSPI_NUM
#define LT8500_CMD_SynUpdate			0x00	/*Update PWM’s Synchronously to PWM Period*/
#define LT8500_CMD_AsyUpdate			0x10	/*Update PWM’s Asynchronously to PWM Period*/
#define LT8500_CMD_Correction			0x20	/*Set PWM Correction Factor*/
#define LT8500_CMD_OutputEnable			0x30	/*Enable PWM Outputs*/
#define LT8500_CMD_OutputDisable		0x40	/*Disable (Drive Low) PWM Outputs*/
#define LT8500_CMD_SelfTest				0x50	/*Initiates Self Test*/
#define LT8500_CMD_PhaseShiftToggle		0x60	/*Toggle 16-Channel Bank 120° Phase-Shift (PHS)*/
#define LT8500_CMD_CorrentionToggle		0x70	/*Toggle Correction Disable Bit in Multiplier (CRD)*/





typedef struct PWMBOARDSPI_STRUCT{

	uint8_t CMDBuffer[PWMBOARDSPI_NUM][LT8500_FrameLength];
	uint8_t StatusBuffer[PWMBOARDSPI_NUM][LT8500_FrameLength];

	SPI_HandleTypeDef *PWM_spi;
	GPIO_TypeDef *CS_Port;
	uint16_t	CS_Pin;

	uint8_t syncMode;
	uint8_t dirtyDuty;
	uint8_t dirtyCOR;
	uint16_t Frequency_PortNum;
	float frequency;
	float clkFrequency;

}PWM_BOARDSPI;

extern PWM_BOARDSPI PWMBoardSPIDevices;
void PWMBoardSPI_writeFrequency(float freq);
void PWMBoardSPI_writeDuty(uint16_t num, float fduty);
void PWMBoardSPI_writeCorrection(uint16_t num,uint8_t value);
void PWMBoardSPI_init();

void PWMBoardSPI_updateDutyAll();
void PWMBoardSPI_updateCorrectionAll();

#endif

#ifdef __cplusplus
}
#endif
#endif /* FRAMEWORK_FRAMEINC_PWMBOARDSPI_H_ */
