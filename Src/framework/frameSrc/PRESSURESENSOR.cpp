/*
 * PRESSURESENSOR.cpp
 *
 *  Created on: Jun 15, 2018
 *      Author: Xiaojiao Chen
 *		 Email: chen2014@connect.hku.hk
 *	Laboratory: Bio-Control Lab
 * Orgnization: the University of Hong Kong
 */

#include <PneuDriveLL.h>
#include <PRESSURESENSOR.h>


//gauge pressure Pa
const float pressureSensorType1_vMin =  1.5;
const float pressureSensorType1_vMax =  4.5;
const float pressureSensorType1_pMin =   0;
const float pressureSensorType1_pMax =  300000;


PRESSURE_SENSOR::PRESSURE_SENSOR(int num):
voltage(0),
pressure(0)
{
	attach(num);
	setSensorRange_GaugePa(pressureSensorType1_vMin,pressureSensorType1_vMax,pressureSensorType1_pMin,pressureSensorType1_pMax);
}

float PRESSURE_SENSOR::read()
{
	voltage = AnalogRead(AnalogPort);
	pressure = MAP(voltage,vMin,vMax,pMin,pMax);
	return pressure;
}

void PRESSURE_SENSOR::attach(int num)
{
	AnalogPort = num;

}

void PRESSURE_SENSOR::setSensorRange_GaugePa(float vmin,float vmax,float pmin,float pmax)
{
	vMin=vmin;
	vMax=vmax;
	pMin=pmin;
	pMax=pmax;
}
