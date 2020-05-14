/*
 * PneuFunctions.h
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
#include "main.h"



/***************************Implement the following interface functions as needed********************/

void setup();			//called once at the beginning.
void loop();			//called at the frequency of CONTROL_FREQUENCY.
void serialDisplay();	//called at the frequency of DISPLAY_FREQUENCY.
void serialReceiveCallback(char *pSerialReceiveBuffer); //called when receive string with ending '\r\n'
void interruptCallback(int interrupt_Pin); //called when interrupt port has an uprising interrupt.


/*****************************Available low-levle APIs to use********************/
float AnalogRead(uint16_t num);
void AnalogWrite(uint16_t num,float voltage);

void DigitalRead(uint16_t num);
void DigitalWrite(uint16_t num, uint16_t state);
void DigitalToggle(uint16_t num);

void PWMWriteDuty(uint16_t num, float duty);
void PWMWriteFrequency(uint16_t num, float fre);

uint32_t millis();
uint32_t micros();
void delay_us(int32_t t);
void delay_ns(int32_t t);

/**************************Change the period of control loop and send loop**********/
void setPeriodControlLoop(int32_t clMS);
void setPeriodSendLoop(int32_t slMS);

/**************************Other interface functions to use**************************/
void serialPort1Callback(char *pSerialPort1Buffer);
void serialPort2Callback(char *pSerialPort4Buffer);


#include <builtInHWConfig.h>
#include "stm32f7xx_hal.h"
#include "arm_math.h"
#include "myUsartFunction.h"
#include "myPID.h"
#include "kalman.h"
#include "Controller.h"
#include "valveFlowFunc.h"

#ifdef __cplusplus
}
#endif

#endif /* USERINC_PNEUFUNCTIONS_H_ */
