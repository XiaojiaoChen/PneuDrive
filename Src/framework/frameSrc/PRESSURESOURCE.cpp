/*
 * PRESSURE_SOURCE.cpp
 *
 *  Created on: Jan 15, 2019
 *      Author: Xiaojiao Chen
 *		 Email: chen2014@connect.hku.hk
 *	Laboratory: Bio-Control Lab
 * Orgnization: the University of Hong Kong
 */

#include <PRESSURESOURCE.h>



PRESSURE_SOURCE::PRESSURE_SOURCE(int DigitalPort, int AnalogPort) {
	attach(DigitalPort,AnalogPort);
	sourceType = 0;
	direction=0;
	pressure=0;
	pressureLower=0;
	pressureUpper=0;
}


void PRESSURE_SOURCE::attach(int DigitalPort, int AnalogPort) {
	attachPump(DigitalPort);
	attachSensor(AnalogPort);
}

void PRESSURE_SOURCE::attachPump(int DigitalPort) {
	pump.attach(DigitalPort);
}

void PRESSURE_SOURCE::attachSensor(int AnalogPort) {
	pressureSensor.attach(AnalogPort);
}

void PRESSURE_SOURCE::maintainPressure(float p_low,float p_high) {

	pressureLower=p_low;
	pressureUpper=p_high;
	readPressure();

	if(pressure<pressureLower)
	{
		if(pressureLower>0)
			pump.start();
		else
			pump.stop();
	}
	else if(pressure>pressureUpper)
	{
		if(pressureUpper<0)
			pump.start();
		else
			pump.stop();
	}

}

void PRESSURE_SOURCE::stop()
{
	pump.stop();
}


float PRESSURE_SOURCE::readPressure() {
	pressure=pressureSensor.read();
	return pressure;
}
