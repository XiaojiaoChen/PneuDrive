/*
 * userInterface.c
 *
 *  Created on: Jun 5, 2018
 *      Author: 402072495
 */
#include <PneuFunctions.h>


#include "builtInPWM.h"
#include "builtInAnalog.h"
#include "builtInDigital.h"

#include "ADBoard.h"
#include "DABoard.h"
#include "PWMBoardSPI.h"
#include "PWMBoardI2C.h"
#include "tim.h"

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
	int numori=num;
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
	int numori=num;
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

__weak void setup() {
	printf("Running default setup()\r\n");
};

__weak void loop() {
	printf("Running default loop()\r\n");
}
;
__weak void serialDisplay() {
	printf("Running default serialDisplay()\r\n");
}
;
__weak void serialCommandCallback(char *pSerialReceiveBuffer) {
	printf("Running default serialCommandCallback()\r\n");
}
;
__weak void wirelessCommandCallback(char *pWirelessReceiveBuffer) {
	printf("Running default WirelessCommandCallbackFunction()\r\n");
}
;
__weak void clickButtonCallback(int Button_Interrupt_Pin)
{
};
__weak void serialPort4Callback(char *pSerialPort4Buffer)
{

};
/***************************************************************************************************/



/*-------------------Delay Function needs modification--------------------------------*/
#define DELAY_TIMER htim7

void resetTIC()
{
	DELAY_TIMER.Instance->CNT=0;
}
int32_t TIC()
{
	 return (int32_t)DELAY_TIMER.Instance->CNT;
}


void delay_us(int32_t t)  //1MHz 1us
{
	int32_t pre = DELAY_TIMER.Instance->CNT;
	while((int32_t)((int32_t)(DELAY_TIMER.Instance->CNT))<pre+t-1);
}
void delay_ns(int32_t t)  ////20MHz 50ns
{

   for(int i=0;i<t/50;i++)
	   {


	   	   __ASM volatile ("NOP");
	   }
}

uint32_t millis()
{
	return HAL_GetTick();
}
void init_builtInTime()
{
	HAL_TIM_Base_Start(&DELAY_TIMER);
}
