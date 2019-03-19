/*
 * userInterface.h
 *
 *  Created on: Jun 5, 2018
 *      Author: Xiaojiao Chen
 *	Laboratory: Bio-Control Lab
 * Orgnization: the University of Hong Kong
 */
#ifndef USERINC_PNEUFUNCTIONS_H_
#define USERINC_PNEUFUNCTIONS_H_
#ifdef __cplusplus
 extern "C" {
#endif

#include "UserConfig.h"
#include "main.h"
#include "stm32f7xx_hal.h"
#include "arm_math.h"
#include "myUsartFunction.h"
#include "myPID.h"
#include "kalman.h"
#include "Controller.h"
#include "valveFlowFunc.h"


 /*****************************Available function ********************/
 float AnalogRead(uint16_t num);
 void AnalogWrite(uint16_t num,float voltage);

 void DigitalRead(uint16_t num);
 void DigitalWrite(uint16_t num, uint16_t state);
 void DigitalToggle(uint16_t num);

 void PWMWriteDuty(uint16_t num, float duty);
 void PWMWriteFrequency(uint16_t num, float fre);

 uint32_t millis();
 void resetTIC();
 int32_t TIC();
 void delay_us(int32_t t);
 void delay_ns(int32_t t);
void init_builtInTime(void);
 /*******************************************************************/


/*****************************interface function to be implemented********************/
void setup();
void loop();
void serialDisplay();
void serialCommandCallback(char *pSerialReceiveBuffer);
void wirelessCommandCallback(char *pWirelessReceiveBuffer);
void clickButtonCallback(int Button_Interrupt_Pin);
void serialPort4Callback(char *pSerialPort4Buffer);
/**********************************************************************/






#ifdef __cplusplus
}
#endif

#endif /* USERINC_PNEUFUNCTIONS_H_ */
