/*
 * builtInDigital.c
 *
 *  Created on: Jan 4, 2019
 *      Author: Xiaojiao Chen
 *		 Email: chen2014@connect.hku.hk
 *	Laboratory: Bio-Control Lab
 * Orgnization: the University of Hong Kong
 */


#include "builtInDigital.h"
#include "gpio.h"
#include "main.h"

static GPIO_TypeDef* BuiltInDigitalOut_GPIO_Port[BUILTIN_DIT_OUT_NUM]={DO_0_GPIO_Port,DO_1_GPIO_Port,DO_2_GPIO_Port,DO_3_GPIO_Port,DO_4_GPIO_Port,DO_5_GPIO_Port,DO_6_GPIO_Port,DO_7_GPIO_Port,DO_8_GPIO_Port,DO_9_GPIO_Port,
		DO_10_GPIO_Port,DO_11_GPIO_Port,DO_12_GPIO_Port,DO_13_GPIO_Port,DO_14_GPIO_Port,DO_15_GPIO_Port};

static uint16_t BuiltInDigitalOut_Pin[BUILTIN_DIT_OUT_NUM]={DO_0_Pin,DO_1_Pin,DO_2_Pin,DO_3_Pin,DO_4_Pin,DO_5_Pin,DO_6_Pin,DO_7_Pin,DO_8_Pin,DO_9_Pin,
		DO_10_Pin,DO_11_Pin,DO_12_Pin,DO_13_Pin,DO_14_Pin,DO_15_Pin};

static GPIO_TypeDef* BuiltInDigitalIn_GPIO_Port[BUILTIN_DIT_IN_NUM]={DI_0_GPIO_Port,DI_1_GPIO_Port,DI_2_GPIO_Port,DI_3_GPIO_Port};

static uint16_t BuiltInDigitalIn_Pin[BUILTIN_DIT_IN_NUM]={DI_0_Pin,DI_1_Pin,DI_2_Pin,DI_3_Pin};



void DigitalBuiltIn_Write(uint16_t num, uint16_t state)
{
	GPIO_PinState sta = (GPIO_PinState)(state);
	if(0<=num && num<BUILTIN_DIT_OUT_NUM){
	HAL_GPIO_WritePin(BuiltInDigitalOut_GPIO_Port[num], BuiltInDigitalOut_Pin[num], sta);
	}
}

int DigitalBuiltIn_Read(uint16_t num)
{

	if(0<=num && num<BUILTIN_DIT_IN_NUM){
		GPIO_PinState sta = HAL_GPIO_ReadPin(BuiltInDigitalIn_GPIO_Port[num], BuiltInDigitalIn_Pin[num]);
		return sta;
	}
	else
		return 0;
}

void DigitalBuiltIn_Toggle(uint16_t num)
{
	if(0<=num && num<BUILTIN_DIT_OUT_NUM){
	HAL_GPIO_TogglePin(BuiltInDigitalOut_GPIO_Port[num], BuiltInDigitalOut_Pin[num]);
	}
}

