/*
 * CHAMBER.h
 *
 *  Created on: Jun 15, 2018
 *      Author: Xiaojiao Chen
 *		 Email: chen2014@connect.hku.hk
 *	Laboratory: Bio-Control Lab
 * Orgnization: the University of Hong Kong
 */

#ifndef CHAMBER_H_
#define CHAMBER_H_

#ifdef __cplusplus
extern "C" {
#endif



#include <PRESSURESENSOR.h>
#include <pressureSensorSPI.h>
#include <SOLENOIDVALVE.h>
#include "Controller.h"

class CHAMBER{
public:
	CHAMBER(int PWMPort1=0,int PWMPort2=1,int AnalogPort=0);
	void attach(int PWMPort1,int PWMPort2,int AnalogPort);
	void attachPWM(int PWMPort1,int PWMPort2);
	void attachSensor(int AnalogPort);

	float readPressure();
	float filterPressure();
	void writePressure(float pressureCommand);
	void writePressure(float pNom,float pNomDot);

	float readPosition();
	void writePosition(float positionCommand);

	float readOpening();
	void writeOpening(float opening);
	void writeOpeningSequence();

	void setSensorRange_GaugePa(float vmin,float vmax,float pmin,float pmax);

	BASIC_PRESSURE_CONTROLLER *pressureController;
	SOLENOID_VALVE valves[2];
	PRESSURE_SENSOR pressureSensor;
	PRESSURE_SENSORSPI pressureSensorspi;

	float pressureDeadZone;
	float pressureMaxP;
	float pressureMinN;
	float opening;//([-1,1],    -1:maximum deflate;  0:shut off,  1: maximum inflate)

	float openingMinP;
	float openingMaxP;

	float openingMinN;
	float openingMaxN;

	float inflateVelocity;
	float pressure;
	float pressureFil;
	float pressuredot;
	float filterBeta;
	float pressureCommand;
	int inflatingFlag;

	float openingSequence[1000];
	int fulOpening;
	int endOpeningNum;
	int curOpeningNum;
	int inOpeningSequence;

	float pressureTable[13];
	float positionTable[13];
	float pressure2position(float);
	float position2pressure(float);
	void setValveOpeningLimit(float ominN,float omaxN,float ominP,float omaxP);
private:
	float length;
	float lengthCommand;




	static const float effectiveRadius;
	static const float effectiveArea;
	static const float pressureMin;
	static const float pressureMax;



};

#ifdef __cplusplus
}
#endif
#endif /* CHAMBER_H_ */
