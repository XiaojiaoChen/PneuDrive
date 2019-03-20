/*
 * PRESSURE_SOURCE.h
 *
 *  Created on: Jan 15, 2019
 *      Author: Xiaojiao Chen
 *		 Email: chen2014@connect.hku.hk
 *	Laboratory: Bio-Control Lab
 * Orgnization: the University of Hong Kong
 */

#ifndef FRAMEWORK_PNEUMATICELEMENTS_PRESSURESOURCE_H_
#define FRAMEWORK_PNEUMATICELEMENTS_PRESSURESOURCE_H_
#ifdef __cplusplus
extern "C" {
#endif

/*
 *
 */

#include "PUMP.h"
#include "PRESSURESENSOR.h"
class PRESSURE_SOURCE {
public:
	PRESSURE_SOURCE(int DigitalPort=0,int AnalogPort=0);
	void attach(int DigitalPort,int AnalogPort);
	void attachPump(int DigitalPort);
	void attachSensor(int AnalogPort);
	float readPressure();
	void maintainPressure(float p_low,float p_high);
	void stop(void);
	int sourceType;
	PUMP pump;
	PRESSURE_SENSOR pressureSensor;
	int direction;
	float pressure;
	float pressureLower;
	float pressureUpper;
};

#ifdef __cplusplus
}
#endif
#endif /* FRAMEWORK_PNEUMATICELEMENTS_PRESSURESOURCE_H_ */
