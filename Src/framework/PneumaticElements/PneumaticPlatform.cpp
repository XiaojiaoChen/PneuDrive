/*
 * PneumaticPlatform.cpp
 *
 *  Created on: Aug 29, 2018
 *      Author: Xiaojiao Chen
 *		 Email: chen2014@connect.hku.hk
 *	Laboratory: Bio-Control Lab
 * Orgnization: the University of Hong Kong
 */

#include <PneumaticPlatform.h>

#include "stdlib.h"

PNEUMATIC_PLATFORM::PNEUMATIC_PLATFORM(int chambernum)
{
	NumChambers=chambernum;
	chambers =(CHAMBER **)calloc(chambernum,sizeof(CHAMBER *));
	pressureCommands = (float *)calloc(chambernum,sizeof(float));
	for(int i=0;i<NumChambers;i++)
	{
		chambers[i]=new CHAMBER(2*i,2*i+1,i);
	}
	pressureDeadZone = 2000;
    frequency = 10;
    setupPlatform();
}


void PNEUMATIC_PLATFORM::increaseFrequency(float dFre){
	frequency = frequency+dFre;
	for (int i = 0; i < NumChambers; i++) {
		chambers[i]->valves[0].writeFrequency(frequency);
		chambers[i]->valves[1].writeFrequency(frequency);
	}
}

void PNEUMATIC_PLATFORM::writeFrequency(float fre){
	frequency = fre;
	for (int i = 0; i < NumChambers; i++) {
		chambers[i]->valves[0].writeFrequency(frequency);
		chambers[i]->valves[1].writeFrequency(frequency);
	}
}


float PNEUMATIC_PLATFORM::readPressure(int num)
{
	return (chambers[num]->readPressure());
}

void PNEUMATIC_PLATFORM::writePressure(int num,float preCommand)
{
	bufferPressureCommand(num,preCommand);
	writePressureCommand(num);
}

void PNEUMATIC_PLATFORM::bufferPressureCommand(int num,float preCommand)
{
	pressureCommands[num]=preCommand;
}
void PNEUMATIC_PLATFORM::writePressureCommand(int num)
{
	chambers[num]->writePressure(pressureCommands[num]);
}


void PNEUMATIC_PLATFORM::setupPlatform() {
	for(int i=0;i<NumChambers;i++){
		chambers[i]->writeOpening(0);
	}
	pSource.stop();
	pSink.stop();
}
