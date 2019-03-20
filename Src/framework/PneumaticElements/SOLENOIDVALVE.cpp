/*
 * SOLENOIDVALVE.cpp
 *
 *  Created on: Jun 15, 2018
 *      Author: Xiaojiao Chen
 *		 Email: chen2014@connect.hku.hk
 *	Laboratory: Bio-Control Lab
 * Orgnization: the University of Hong Kong
 */

#include <PneuFunctions.h>
#include <SOLENOIDVALVE.h>



const float SOLENOID_VALVE::frequencyMin = 0.5;
const float SOLENOID_VALVE::frequencyMax = 2000;

SOLENOID_VALVE::SOLENOID_VALVE(int num){

	duty=0;
	frequency = 10;
	dutyMin = 0;
	dutyMax = 1;
	attach(num);
}

void SOLENOID_VALVE::attach(int num)
{
	PWMPort = num;
}



float SOLENOID_VALVE::readDuty()
{
	return duty;
}

void SOLENOID_VALVE::writeDuty(float dut)
{
	dut = CONSTRAIN(dut,dutyMin,dutyMax);
	if(fabsf(dut-duty)>1e-6)
	{
		duty = (PWM_DIGITAL_OUTPUT_REVERSED == 0)?dut:(1-dut);
		PWMWriteDuty(PWMPort,duty);
	}

}


float SOLENOID_VALVE::readFrequency()
{
	return frequency;
}


void SOLENOID_VALVE::writeFrequency(float fre)
{
	fre = CONSTRAIN(fre,frequencyMin,frequencyMax);
	if(fabsf(fre-frequency)>1e-6)
	{
		frequency = fre;
		PWMWriteFrequency(PWMPort,frequency);
	}
}


