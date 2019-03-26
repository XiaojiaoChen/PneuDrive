/*
 * PNEUDRIVE.cpp
 *
 *  Created on: Aug 29, 2018
 *      Author: Xiaojiao Chen
 *		 Email: chen2014@connect.hku.hk
 *	Laboratory: Bio-Control Lab
 * Orgnization: the University of Hong Kong
 */

#include <PneuDrive.h>
#include "stdlib.h"

PNEUDRIVE::PNEUDRIVE(int chambernum)
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
}


void PNEUDRIVE::increaseFrequency(float dFre){
	frequency = frequency+dFre;
	for (int i = 0; i < NumChambers; i++) {
		chambers[i]->valves[0].writeFrequency(frequency);
		chambers[i]->valves[1].writeFrequency(frequency);
	}
}

void PNEUDRIVE::writeFrequency(float fre){
	frequency = fre;
	for (int i = 0; i < NumChambers; i++) {
		chambers[i]->valves[0].writeFrequency(frequency);
		chambers[i]->valves[1].writeFrequency(frequency);
	}
}


float PNEUDRIVE::readPressure(int num)
{
	return (chambers[num]->readPressure());
}

void PNEUDRIVE::writePressure(int num,float preCommand)
{
	bufferPressure(num,preCommand);
	writeBufferedPressure(num);
}

void PNEUDRIVE::bufferPressure(int num,float preCommand)
{
	pressureCommands[num]=preCommand;
}
void PNEUDRIVE::writeBufferedPressure(int num)
{
	chambers[num]->writePressure(pressureCommands[num]);
}


void PNEUDRIVE::setupPlatform() {
	for(int i=0;i<NumChambers;i++){
		chambers[i]->writeOpening(0);
	}
	pSource.stop();
	pSink.stop();
}
