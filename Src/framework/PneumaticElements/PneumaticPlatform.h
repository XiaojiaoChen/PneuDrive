/*
 * wholePlatform.h
 *
 *  Created on: Aug 29, 2018
 *      Author: Xiaojiao Chen
 *		 Email: chen2014@connect.hku.hk
 *	Laboratory: Bio-Control Lab
 * Orgnization: the University of Hong Kong
 */

#ifndef __PNEUMATICS_PLATFORM_H_
#define __PNEUMATICS_PLATFORM_H_
#ifdef __cplusplus
extern "C" {
#endif

/*
 *
 */

#include <PneuFunctions.h>

#include <PUMP.h>
#include <SOLENOIDVALVE.h>
#include <CHAMBER.h>
#include <PressureSource.h>
#include "myPID.h"
#include "trajectoryGeneration.h"

#define DEFAULT_CHAMBER_NUM 4

class PNEUMATIC_PLATFORM {
public:
	PNEUMATIC_PLATFORM(int chambernum=DEFAULT_CHAMBER_NUM);
	void setupPlatform();
	void increaseFrequency(float deltaFrequency);
	void writeFrequency(float fre);

	float readPressure(int num);
	void writePressure(int num,float presCommand);

	void bufferPressureCommand(int num,float preCommand);
	void writePressureCommand(int num);

	float *pressureCommands;

	uint16_t NumChambers;
	CHAMBER **chambers;

	PRESSURE_SOURCE pSource;
	PRESSURE_SOURCE pSink;

	float frequency;
	float pressureDeadZone;

};

#ifdef __cplusplus
}
#endif
#endif /* __PNEUMATICS_PLATFORM_H_ */
