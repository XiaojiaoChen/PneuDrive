/*
 * PNEUDRIVE.h
 *
 *  Created on: Aug 29, 2018
 *      Author: Xiaojiao Chen
 *		 Email: chen2014@connect.hku.hk
 *	Laboratory: Bio-Control Lab
 * Orgnization: the University of Hong Kong
 */

#ifndef __PNEUDRIVE_H_
#define __PNEUDRIVE_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *
 */


#include <PneuDriveLL.h>
#include <PUMP.h>
#include <SOLENOIDVALVE.h>
#include <CHAMBER.h>
#include <PressureSource.h>
#include "myPID.h"
#include "trajectoryGeneration.h"
#include "joyStick.h"

#define DEFAULT_CHAMBER_NUM 6

class PNEUDRIVE {
public:
	PNEUDRIVE(int chambernum=DEFAULT_CHAMBER_NUM);
	void setupPlatform();
	void increaseFrequency(float deltaFrequency);
	void writeFrequency(float fre);

	float readPressure(int num);
	void writePressure(int num,float presCommand);

	void bufferPressure(int num,float preCommand);
	void writeBufferedPressure(int num);

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
#endif /* __PNEUDRIVE_H_ */
