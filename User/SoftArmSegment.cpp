/*
 * SoftArm.c
 *
 *  Created on: May 12, 2018
 *      Author: 402072495
 */
#include <SoftArmSegment.h>
#include "stdio.h"

const static float Patm=101325;
const static float PaPerPSI=6895;


/*************************SOFT ARM**************************
 *
 ***********************************************************/
SOFT_ARM_SEGMENT::SOFT_ARM_SEGMENT(int num)
{
	basePlatform = new PNEUDRIVE(num);
	for(int i=0;i<BELLOWNUM;i++)
	{
		bellows[i]=basePlatform->chambers[i];
	}

}


float SOFT_ARM_SEGMENT::readPressure(int num)
{
	pressure[num]=basePlatform->chambers[num]->readPressure();
	pressureFil[num]=basePlatform->chambers[num]->pressureFil;
	return pressure[num];
}
void SOFT_ARM_SEGMENT::readPressureAll()
{
	for (int i = 0; i < BELLOWNUM; i++) {
		readPressure(i);
	}
}

void SOFT_ARM_SEGMENT::writeOpeningAll(float op)
{
	for (int i = 0; i < BELLOWNUM; i++) {
		basePlatform->chambers[i]->writeOpening(op);
	}
}

void SOFT_ARM_SEGMENT::controlPressureAll()
{
	for (int i = 0; i < BELLOWNUM; i++) {
		pressureD[i]=CONSTRAIN(pressureD[i],pressureCommandMin[i],pressureCommandMax[i]);
		basePlatform->chambers[i]->writePressure(pressureD[i]);
	}
}
void SOFT_ARM_SEGMENT::setPressureCommand(int num,float pre)
{
	pressureD[num]=pre;
	pressureD[num]=CONSTRAIN(pressureD[num],pressureCommandMin[num],pressureCommandMax[num]);
}
