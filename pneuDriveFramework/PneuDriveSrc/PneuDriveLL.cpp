/*
 * userInterface.c
 *
 *  Created on: Jun 5, 2018
 *      Author: 402072495
 */
#include <PneuDriveLL.h>
#include "builtInPWM.h"
#include "builtInAnalog.h"
#include "builtInDigital.h"

#include "ADBoard.h"
#include "DABoard.h"
#include "PWMBoardSPI.h"
#include "PWMBoardI2C.h"
#include "tim.h"
extern TIM_HandleTypeDef htim5;
int32_t globalPeriodControlLoop=1;
int32_t globalPeriodSendLoop=1;

float AnalogRead(uint16_t num)
{
	float value=0;
	if(0<=num && num<BUILTIN_ANA_IN_NUM)
		value=AnalogRead_BuiltIn(num);
	else
	{
#if (ADBOARD_NUM>0)
		num-=BUILTIN_ANA_IN_NUM;
		value=AnalogRead_External(num);
#endif
	}
	return value;

}

void AnalogWrite(uint16_t num,float voltage)
{
#if (DABOARD_NUM>0)
	DABoard_writeVoltage(num,voltage);
#endif
}
void DigitalWrite(uint16_t num, uint16_t state)
{
	DigitalBuiltIn_Write(num,state);
}

void DigitalToggle(uint16_t num)
{
	DigitalBuiltIn_Toggle(num);
}

void PWMWriteDuty(uint16_t num, float fduty)
{
	uint16_t numori=num;
	if(0<=num && num<BUILTIN_PWM_NUM){
		PWMBuiltIn_writeDuty(num,fduty);
	}
	else
	{
#if(PWMBOARDSPI_NUM>0)
		num= numori - BUILTIN_PWM_NUM;
		if(0<=num && num<PWMBOARDSPI_CHANNELNUM){
			PWMBoardSPI_writeDuty(num,fduty);
		}
#endif

#if(PWMBOARDI2C_NUM>0)
		num=numori - BUILTIN_PWM_NUM;
		if(0<=num && num<PWMBOARDI2C_CHANNELNUM){
		 PWMBoardI2C_writeDuty(num,fduty);
		}
#endif
	}
}

void PWMWriteFrequency(uint16_t num, float fre)
{
	uint16_t numori=num;
	if(num<BUILTIN_PWM_NUM){
			PWMBuiltIn_writeFrequency(num,fre);
	}
	else
	{
#if (PWMBOARDSPI_NUM>0)
		num= numori - BUILTIN_PWM_NUM;
		if(0<=num && num<PWMBOARDSPI_CHANNELNUM){
			PWMBoardSPI_writeFrequency(fre);
		}
#endif

#if (PWMBOARDI2C_NUM>0)
		num= numori - BUILTIN_PWM_NUM;
		if(0<=num && num<PWMBOARDI2C_CHANNELNUM){
			PWMBoardI2C_writeFrequency(num,fre);
		}
#endif

	}
}




/***************************************************************************************************/
/* NOTE : The following functions should not be modified, but to be implemented in the user file. */

/**
 * @brief  called once at the beginning.
 * @note   This function is called only once at the beginning.
 * You could put the initialization code here.
 * @param  None
 * @retval None
 */
__weak  void setup() {
	printf("Running default setup()\r\n");
};

/**
 * @brief  called at a default frequency of 1000Hz.
 * @note   This function is called after setup(), and would be continuously running  at a frequency of CONTROL_FREQUENCY.
 * CONTROL_FREQUENCY is a user-defined MACRO, with a default value of 1000, and the maximum value is also 1000.
 * You could put the control algorithms here.
 * @param  None
 * @retval None
 */
__weak  void loop() {
	printf("Running default loop()\r\n");
}
;

/**
 * @brief  called at a default frequency of 100Hz.
 * @note   This function is called after setup(), and would be continuously running  at a frequency of DISPLAY_FREQUENCY.
 * DISPLAY_FREQUENCY is a user-defined MACRO, with a default value of 100, and the maximum value is 1000.
 * You could transfer informations to PC from there by using function printf(). In the maximum transfer frequency of 1000Hz,
 * the maximum sending bytes per cycle is 100 Bytes under the baudrate of 921600.
 * This capacity could be doubled if the baudrate is changed to 2000000.
 * @param  None
 * @retval None
 */
__weak  void serialDisplay() {
	printf("Running default serialDisplay()\r\n");
}
;

/**
  * @brief  serialReceive Callback function
  * @note   This function is called  when serialPort3 receives strings ended with '\r\n' from PC serial communication.
  * Users could use scanf() to read the ASCII string, or use structure to decode the BIN information.
  * @param  pSerialReceiveBuffer : Pointer to the received string ended with '\r\n'.
  * @retval None
  */
__weak  void serialReceiveCallback(char *pSerialReceiveBuffer) {
	printf("Running default serialCommandCallback()\r\n");
}
;

/**
  * @brief  hardware interrupt Callback function
  * @note   This function is called  when uprising change occurs on the interrupt port
  * @param  interrupt_Pin : the number of port where interrupt took place
  * @retval None
  */
__weak  void interruptCallback(int Button_Interrupt_Pin)
{
};

__weak void serialPort1Callback(char *pWirelessReceiveBuffer) {
	printf("Running default WirelessCommandCallbackFunction()\r\n");
}
;

__weak  void serialPort2Callback(char *pSerialPort4Buffer)
{

};
/***************************************************************************************************/



/*-------------------Delay Function needs modification--------------------------------*/



uint32_t millis()
{
	return HAL_GetTick();
}

uint32_t micros()
{
	uint32_t t=(millis()*1000) + htim5.Instance->CNT;
	return t;
}

void delay_us(int32_t t)  //1MHz 1us
{
	uint32_t ts=micros();
	while((int32_t)(micros()-ts)<t);
}
void delay_ns(int32_t t)  ////20MHz 50ns
{
	__HAL_TIM_ENABLE(&htim7);
   while((int32_t)(htim7.Instance->CNT)<t);
   __HAL_TIM_DISABLE(&htim7);
   htim7.Instance->CNT=0;

}

void setPeriodControlLoop(int32_t clMS)
{
	globalPeriodControlLoop=clMS;
}

void setPeriodSendLoop(int32_t slMS)
{
	globalPeriodSendLoop=slMS;
}
