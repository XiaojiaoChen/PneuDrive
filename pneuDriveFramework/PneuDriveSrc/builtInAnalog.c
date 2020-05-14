/*
 * builtInAnalog.h
 *
 *  Created on: Jan 3, 2019
 *      Author: Xiaojiao Chen
 *		 Email: chen2014@connect.hku.hk
 *	Laboratory: Bio-Control Lab
 * Orgnization: the University of Hong Kong
 */

#include "builtInAnalog.h"
#include "adc.h"

uint16_t ADDataBuiltIn[BUILTIN_ANA_IN_NUM];
float fADDataBuiltIn[BUILTIN_ANA_IN_NUM];

void AnaBuiltInStart()
{
	HAL_ADC_Start_DMA(&hadc1,(uint32_t *)(&ADDataBuiltIn[0]),BUILTIN_ANA_IN_NUM);
}

void AnaBuiltInStop()
{
	HAL_ADC_Stop_DMA(&hadc1);
}


/*void AnaBuiltIn_CallBack()
{
	for(int i=0;i<BuiltInAnaNum;i++)
		fADDataBuiltIn[i]=ADDataBuiltIn[i]/4096.0f*3.3f;

}*/

float AnalogRead_BuiltIn(int num)
{
	if(num>=0 && num<BUILTIN_ANA_IN_NUM)
	{
		fADDataBuiltIn[num]=ADDataBuiltIn[num]/4096.0f*3.3f;
		return fADDataBuiltIn[num];
	}
	else
		return 0;
}
