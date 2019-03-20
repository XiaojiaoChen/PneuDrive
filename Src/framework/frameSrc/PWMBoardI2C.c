/*************************************************** 
  This is a modified driver for STM32f767ZI board
  based on Adafruit 16-channel PWM & Servo driver
  from http://www.adafruit.com/products/815
 ****************************************************/

#include <builtInHWConfig.h>
#if (PWMBOARDI2C_NUM>0)
#include <PWMBoardI2C.h>
#include <stdlib.h>
#include <string.h>
#include "i2c.h"
#include "math.h"

#define hi2c_PWM		hi2c2

 PWM_BOARDI2C PWMBoardI2CDevices;
 PWM_BOARDI2C *ptPWMBoardI2CDevices=&PWMBoardI2CDevices;


static uint8_t PCABoard_I2C_Addresses[PWMBOARDI2C_NUM]={0x00,0x01,0x02,0x03};

static uint8_t PCA9685_readReg(PCA9685_PWM_DEVICE *ptPWMBoardPCA9685, uint8_t address)
{
	 uint8_t initStruct[1];
	 uint8_t buffer[1];

	 initStruct[0] = address;

	 HAL_I2C_Master_Transmit(ptPWMBoardI2CDevices->pwm_hi2c,ptPWMBoardPCA9685->i2cAddress,initStruct,1,1);
	 HAL_I2C_Master_Receive(ptPWMBoardI2CDevices->pwm_hi2c,ptPWMBoardPCA9685->i2cAddress,buffer,1,1);

	 return(buffer[0]);
}
static void PCA9685_writeReg(PCA9685_PWM_DEVICE *ptPWMBoardPCA9685, uint8_t address,uint8_t data)
{
	 uint8_t initStruct[2];
	 initStruct[0] = address;
	 initStruct[1] = data;
	 HAL_I2C_Master_Transmit(ptPWMBoardI2CDevices->pwm_hi2c,ptPWMBoardPCA9685->i2cAddress,initStruct,2,1);
}


static void PCA9685_calculateDuty(PCA9685_PWM_DEVICE *ptPWMBoardPCA9685 , uint8_t num, float fduty)
{
	uint16_t on;
	uint16_t off;
	if (fduty <= 0) {
		on = 0;
		off = 4096;
	} else if (fduty >= 1) {
		on = 4096;
		off = 0;
	} else {
		on = 0;
		off = round(4096 * fduty);
		if (off > 1)
			off--;
	}
	ptPWMBoardPCA9685->PWMBuffer[num][0] = on & 0xff;
	ptPWMBoardPCA9685->PWMBuffer[num][1] = on >> 8;
	ptPWMBoardPCA9685->PWMBuffer[num][2] = off & 0xff;
	ptPWMBoardPCA9685->PWMBuffer[num][3] = off >> 8;
}

static void PCA9685_writeFrequency(PCA9685_PWM_DEVICE *ptPWMBoardPCA9685,float freq)
{
	  freq = CONSTRAIN(freq,EXTERNAL_PWMBOARDI2C_FREMIN,EXTERNAL_PWMBOARDI2C_FREMAX);
	  float prescaleval = 25000000.0/(4096*freq*0.915);
	  uint8_t prescale = floor(prescaleval + 0.5)-1;

	 uint8_t oldmode= PCA9685_readReg(ptPWMBoardPCA9685,PCA9685_REG_MODE1);
	 uint8_t sleepMode = ((oldmode & 0x7F) | 0x10); //go to sleep

	 PCA9685_writeReg(ptPWMBoardPCA9685,PCA9685_REG_MODE1,sleepMode);
	 PCA9685_writeReg(ptPWMBoardPCA9685,PCA9685_REG_PRESCALE,prescale);
	 PCA9685_writeReg(ptPWMBoardPCA9685,PCA9685_REG_MODE1,oldmode);
	 HAL_Delay(1);

	 oldmode &= 0xEF;
	 oldmode |= 0xA1;
	 PCA9685_writeReg(ptPWMBoardPCA9685,PCA9685_REG_MODE1,oldmode);



}


static void PCA9685_writeDuty(PCA9685_PWM_DEVICE *ptPWMBoardPCA9685 , uint8_t num, float fduty)
{
	uint8_t sendBuf[5];
	PCA9685_calculateDuty(ptPWMBoardPCA9685,num,fduty);
	sendBuf[0] = PCA9685_REG_LED0_ON_L + 4 * num;
	memcpy(&(sendBuf[1]),&(ptPWMBoardPCA9685->PWMBuffer[num][0]),4);
	HAL_I2C_Master_Transmit(ptPWMBoardI2CDevices->pwm_hi2c, ptPWMBoardPCA9685->i2cAddress, sendBuf, 5,1);
}


void PWMBoardI2C_writeFrequency(uint8_t num,float fre)
{
	PCA9685_writeFrequency(&(ptPWMBoardI2CDevices->pwmBoardsI2C[num/PCA9685_CHANNELNUM]),fre);
}
void PWMBoardI2C_writeDuty(uint8_t num,float fduty)
{
	PCA9685_writeDuty(&(ptPWMBoardI2CDevices->pwmBoardsI2C[num/PCA9685_CHANNELNUM]),num%PCA9685_CHANNELNUM,fduty);
}


void PWMBoardI2C_writeDutyAll(float fduty)
{
	PCA9685_calculateDuty(&(ptPWMBoardI2CDevices->pwmBoardsI2C[0]),0,fduty);
	for(int i=1;i<16;i++)
		memcpy(&(ptPWMBoardI2CDevices->pwmBoardsI2C[0].PWMBuffer[i][0]),&(ptPWMBoardI2CDevices->pwmBoardsI2C[0].PWMBuffer[0][0]),PWMBOARDI2C_NUM);
	for(int j=1;j<3;j++)
		memcpy(&(ptPWMBoardI2CDevices->pwmBoardsI2C[j].PWMBuffer[0][0]),&(ptPWMBoardI2CDevices->pwmBoardsI2C[0].PWMBuffer[0][0]),PCA9685_CHANNELNUM*PWMBOARDI2C_NUM);
	uint8_t sendBuf[5];
	sendBuf[0] = PCA9685_REG_ALLLED_ON_L;
	memcpy(&(sendBuf[1]),&(ptPWMBoardI2CDevices->pwmBoardsI2C[0].PWMBuffer[0][0]),4);
	HAL_I2C_Master_Transmit(ptPWMBoardI2CDevices->pwm_hi2c, CALLALL_ADDRESS, sendBuf, 5,1);
}




void PWMBoardI2C_reset()
{
	static uint8_t buf[1] = {RESET_COMMAND};
	HAL_I2C_Master_Transmit(ptPWMBoardI2CDevices->pwm_hi2c,RESET_ADDRESS,buf,1,1);
}
void PWMBoardI2C_init()
{
	for(int i=0;i<PWMBOARDI2C_NUM;i++)
	{
		PCABoard_I2C_Addresses[i] = i;
		ptPWMBoardI2CDevices->pwmBoardsI2C[i].onboardAddress = PCABoard_I2C_Addresses[i];
		ptPWMBoardI2CDevices->pwmBoardsI2C[i].i2cAddress = I2C_ShIFTED_ADDRESS(PCABoard_I2C_Addresses[i]);

		ptPWMBoardI2CDevices->pwmBoardsI2C[i].frequency = 50;
	}

	ptPWMBoardI2CDevices->pwm_hi2c = &hi2c_PWM;

	PWMBoardI2C_reset();

	for(int i=0;i<PWMBOARDI2C_NUM;i++)
	{
		PCA9685_writeFrequency(&(ptPWMBoardI2CDevices->pwmBoardsI2C[i]),ptPWMBoardI2CDevices->pwmBoardsI2C[i].frequency);

	}
	PWMBoardI2C_writeDutyAll(1);
}



/*static void PCA9685_readRegAll(PCA9685_PWM_DEVICE *ptPWMBoardPCA9685)
{
	for(int i=0;i<PCA9685_CHANNELNUM;i++)
		ptPWMBoardPCA9685->registers[i] = PCA9685_readReg(ptPWMBoardPCA9685, i);

	printf("mode1: %x   mode2: %x  \r\n",ptPWMBoardPCA9685->registers[0],ptPWMBoardPCA9685->registers[1]);
	for(int i=0;i<PWMBOARDI2C_NUM;i++)
		{
			int j=6;
			int k=j+PCA9685_CHANNELNUM*i;
			printf("LED%d: %x %x %x %x        %x %x %x %x      %x %x %x %x        %x %x %x %x\r\n",i,
					ptPWMBoardPCA9685->registers[k],ptPWMBoardPCA9685->registers[k+1],ptPWMBoardPCA9685->registers[k+2],ptPWMBoardPCA9685->registers[k+3],
					ptPWMBoardPCA9685->registers[k+4],ptPWMBoardPCA9685->registers[k+5],ptPWMBoardPCA9685->registers[k+6],ptPWMBoardPCA9685->registers[k+7],
					ptPWMBoardPCA9685->registers[k+8],ptPWMBoardPCA9685->registers[k+9],ptPWMBoardPCA9685->registers[k+10],ptPWMBoardPCA9685->registers[k+11],
					ptPWMBoardPCA9685->registers[k+12],ptPWMBoardPCA9685->registers[k+13],ptPWMBoardPCA9685->registers[k+14],ptPWMBoardPCA9685->registers[k+15]
				);
		}
}
void PWMBoardI2C_readRegAll()
{
	for(int i=0;i<PWMBOARDI2C_NUM;i++)
		PCA9685_readRegAll(&(ptPWMBoardI2CDevices->pwmBoardsI2C[i]));
}
*/
#endif
