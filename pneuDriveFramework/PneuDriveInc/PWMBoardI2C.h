
#ifndef __PWMBOARDI2C_H
#define __PWMBOARDI2C_H

#ifdef __cplusplus
 extern "C" {
#endif


#include <builtInHWConfig.h>
#if (PWMBOARDI2C_NUM>0)

#include <stm32f7xx_hal.h>

#define PCA9685_ADDR_PREFIX_UNSHIFTED  ((uint8_t)(0x40))
#define I2C_ShIFTED_ADDRESS(x)  ((uint8_t)( ((x) | PCA9685_ADDR_PREFIX_UNSHIFTED) <<1))

#define RESET_ADDRESS 0x00
#define RESET_COMMAND 0x06

#define CALLALL_ADDRESS 0xE0

#define PCA9685_REG_MODE1 0x0
#define PCA9685_REG_MODE2 0x1
#define PCA9685_REG_SUBADR1 0x02
#define PCA9685_REG_SUBADR2 0x03
#define PCA9685_REG_SUBADR3 0x04
#define PCA9685_REG_ALLCALL 0x05

#define PCA9685_REG_LED0_ON_L 0x6
#define PCA9685_REG_LED0_ON_H 0x7
#define PCA9685_REG_LED0_OFF_L 0x8
#define PCA9685_REG_LED0_OFF_H 0x9


#define PCA9685_REG_ALLLED_ON_L 0xFA
#define PCA9685_REG_ALLLED_ON_H 0xFB
#define PCA9685_REG_ALLLED_OFF_L 0xFC
#define PCA9685_REG_ALLLED_OFF_H 0xFD

#define PCA9685_REG_PRESCALE 0xFE



typedef struct PCA9685_PWM_DEVICE_STRUCT{

	uint8_t onboardAddress;
	uint8_t i2cAddress;

	uint8_t PWMBuffer[PCA9685_CHANNELNUM][PWMBOARDI2C_NUM];
	uint8_t PWMDMABuffer[PCA9685_CHANNELNUM*PWMBOARDI2C_NUM+1];
	uint8_t registers[70];

	float frequency;

}PCA9685_PWM_DEVICE;


typedef struct PWMBOARDI2C_STRUCT{

	PCA9685_PWM_DEVICE pwmBoardsI2C[PWMBOARDI2C_NUM];
	I2C_HandleTypeDef *pwm_hi2c;
}PWM_BOARDI2C;


extern PWM_BOARDI2C PWMBoardI2CDevices;

void PWMBoardI2C_writeFrequency(uint8_t num,float freq);
void PWMBoardI2C_writeDuty(uint8_t num, float fduty);
void PWMBoardI2C_init();
void PWMBoardI2C_reset(void);

void PWMBoardI2C_writeDutyAll(float fduty);

#endif
#ifdef __cplusplus
}
#endif

#endif
