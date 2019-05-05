/*
 * SOLENOIDVALVE.cpp
 *
 *  Created on: Jun 15, 2018
 *      Author: Xiaojiao Chen
 *		 Email: chen2014@connect.hku.hk
 *	Laboratory: Bio-Control Lab
 * Orgnization: the University of Hong Kong
 */

#include <PneuDriveLL.h>
#include <SOLENOIDVALVE.h>


SOLENOID_VALVE::SOLENOID_VALVE(int num){

	_duty=0;
	duty=(PWM_DIGITAL_OUTPUT_REVERSED == 0)?_duty:(1-_duty);
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
		 duty = dut;
		_duty= (PWM_DIGITAL_OUTPUT_REVERSED == 0)?duty:(1-duty);
		PWMWriteDuty(PWMPort,_duty);
	}

}


float SOLENOID_VALVE::readFrequency()
{
	return frequency;
}


void SOLENOID_VALVE::writeFrequency(float fre)
{
	if(fabsf(fre-frequency)>1e-6)
	{
		frequency = fre;
		PWMWriteFrequency(PWMPort,frequency);
	}
}


