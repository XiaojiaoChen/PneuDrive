/*
 * SoftArm.h
 *
 *  Created on: May 12, 2018
 *      Author: 402072495
 */

#ifndef USERINC_SOFTARM_H_
#define USERINC_SOFTARM_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "PneuDrive.h"
#include "SoftArmSegment.h"
#include <CANbus.h>
#include "messages.h"

enum COMMAND_MODE{
	openingCommandType,
	pressureCommandType

};


#define SEGMENTNUM 9

class SOFT_ARM{
public:
	SOFT_ARM();

	SOFT_ARM_SEGMENT armSegments[SEGMENTNUM];

	struct SENSORDATA sensorData[SEGMENTNUM][BELLOWNUM];
	struct COMMANDDATA commandData[SEGMENTNUM][BELLOWNUM];
	struct COMMANDDATA commandDataBuffer[SEGMENTNUM][BELLOWNUM];
	uint8_t actuatorOnline[SEGMENTNUM][BELLOWNUM];
	uint8_t canBusCommand[8];
	void setupChamberPWMPort();
	void writeCommandAll();
};



#ifdef __cplusplus
}
#endif
#endif /* USERINC_SOFTARM_H_ */
