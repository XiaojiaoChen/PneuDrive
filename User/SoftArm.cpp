/*
 * SoftArm.c
 *
 *  Created on: May 12, 2018
 *      Author: 402072495
 */
#include "SoftArm.h"
#include "stdio.h"


/*************************SOFT ARM**************************
 *
 ***********************************************************/
SOFT_ARM::SOFT_ARM()
{

}


void SOFT_ARM::setupChamberPWMPort()
{
	/*Every actuator is refered to with two numbers, segNum(0-8) and bellowNum(0-5)*/

	for(int j=0;j<SEGMENTNUM;j++){
		SOFT_ARM_SEGMENT* armSegCur=&armSegments[j];
		for(int i=0;i<BELLOWNUM;i++)
		{
			CHAMBER *bellowCur=armSegCur->bellows[i];
			int pwmPortOffset=BUILTIN_PWM_NUM+j*16; //each pwm board has 16*3 ports, while we only need 6*2*3 on each
			/*analog port is treated as the overall No. in our arm.*/
			bellowCur->attach(pwmPortOffset+i*2, pwmPortOffset+i*2+1, j*BELLOWNUM+i);
			bellowCur->writeOpening(0);
		}
	}



}



void SOFT_ARM::writeCommandAll()
{
	for(int j=0;j<SEGMENTNUM;j++){
		SOFT_ARM_SEGMENT* armSegCur=&armSegments[j];
		for(int i=0;i<BELLOWNUM;i++)
		{
			CHAMBER *bellowCur=armSegCur->bellows[i];
			armSegCur->bellows[i]->pressure=sensorData[j][i].pressure*1000-101500;

			if(commandData[j][i].commandType==pressureCommandType){
				float pressureCommandTemp=commandData[j][i].values[0];
				bellowCur->writePressure(pressureCommandTemp);
			}
			else if(commandData[j][i].commandType==openingCommandType){
				float openingCommandTemp=((int16_t)commandData[j][i].values[0])*3.0517578125e-5;//values[0]/32767
				bellowCur->writeOpening(openingCommandTemp);
			}
		}
	}
}




