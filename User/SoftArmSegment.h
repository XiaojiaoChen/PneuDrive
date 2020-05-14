/*
 * SoftArmSegment.h
 *
 *  Created on: May 12, 2018
 *      Author: 402072495
 */

#ifndef USERINC_SOFTARMSEGMENT_H_
#define USERINC_SOFTARMSEGMENT_H_

#ifdef __cplusplus
extern "C" {
#endif



#include "PneuDrive.h"

#define BELLOWNUM 6

class SOFT_ARM_SEGMENT{
public:
	SOFT_ARM_SEGMENT(int num=BELLOWNUM);

	void attach(int num,int PWMPort1,int PWMPort2,int AnalogPort);

	float readPressure(int num);
	void setPressureCommand(int num,float pre);

	void readPressureAll();
	void controlPressureAll();
	void writeOpeningAll(float op);

	float pressure[BELLOWNUM];
	float pressureFil[BELLOWNUM];

	float pressureD[BELLOWNUM];


	float frequency;
	float pressureBase;
	float openingBase;

	PNEUDRIVE *basePlatform;
	CHAMBER *bellows[BELLOWNUM];

	float pressureCommandMin[BELLOWNUM];
	float pressureCommandMax[BELLOWNUM];

	int frequencyDirty;

};

#ifdef __cplusplus
}
#endif
#endif /* USERINC_SOFTARMSEGMENT_H_ */
