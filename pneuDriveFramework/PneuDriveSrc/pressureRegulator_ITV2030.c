#include <PneuDriveLL.h>
#include "pressureRegulator_ITV2030.h"


static uint16_t regulator_DAPort=0;
static float  Pmin = 0;
static float  Pmax = 4000;
static float  PCommand;
static float  VCommand;
static float VoutZero = 0.94;

void setPressure(float pc)       // gauge pressure
{ 
	if(pc<Pmin)
		PCommand = Pmin;
	else if(pc>Pmax)
		PCommand = Pmax;
	else
		PCommand = pc;
	VCommand = (pc-Pmin)/(Pmax-Pmin)*(5-0)+0;
	AnalogWrite(regulator_DAPort,VCommand);
}


float getPressure(float vtem)
{
	//output: 1~5v for 0~500000, regardless of selected output range.
	return (vtem-VoutZero)*12500.0f;
}

void setZeroRegulator(float v)
{
	VoutZero = v;
}
