/*
 * SoftHand.h
 *
 *  Created on: Jun 28, 2018
 *      Author: Xiaojiao Chen
 *		 Email: chen2014@connect.hku.hk
 *	Laboratory: Bio-Control Lab
 * Orgnization: the University of Hong Kong
 */

#ifndef SOFTHAND_H_
#define SOFTHAND_H_
#ifdef __cplusplus
extern "C" {
#endif


#include "PneumaticPlatform.h"
#define FINGER_LITTLE 0
#define FINGER_RING 1
#define FINGER_MIDDLE 2
#define FINGER_INDEX 3
#define FINGER_THUMB 4


#define CHAMBERNUM 26

typedef struct NORMAL_DIGITS_STRUCT{
	CHAMBER* DIP_Joint;
	CHAMBER* PIP_Joint;
	CHAMBER* MP_Joint;
	CHAMBER* AB_Joint;
	CHAMBER* AD_Joint;
}NORMAL_DIGITS;

typedef struct THUMB_STRUCT{
	CHAMBER* IP_Joint;
	CHAMBER* MP_Joint;
	CHAMBER* TP_Joint[3];
	CHAMBER* MT_Joint;
}THUMB;


typedef struct FINGERS_STRUCT{
	NORMAL_DIGITS small;
	NORMAL_DIGITS ring;
	NORMAL_DIGITS middle;
	NORMAL_DIGITS index;
	THUMB		  thumb;
}FINGERS;



class SOFT_HAND{
public:
	SOFT_HAND(int num=26);

	void setup();
	void loop();
	void display();
	void receiveCommand(char *pSerialReceiveBuffer);

	void attach(int num,int PWMPort1,int PWMPort2,int AnalogPort);
	void readPressure();
	void writePressure();
	void writeSequence();
	void writePosition();
	void increaseFrequency(float deltaFrequency);
	void writeFrequency(float fre);
	void storeLast500ms(uint32_t);

	float last50msBuffer[5000];
	float *pLastBuffer;

	int fingerBendingGrade[5];
	FINGERS fingers;
	CHAMBER chambers[CHAMBERNUM];

	uint16_t frequencyDirty;
	PUMP pumpInflate;
	PUMP pumpDeflate;
	CHAMBER sinkChamber;
	float psink_lowerlimit;
	float psink_upperlimit;
	float flowrate;

	float pressure[CHAMBERNUM];
	float pressureCommand[CHAMBERNUM];
	float pressureCommandMin[CHAMBERNUM];
	float pressureCommandMax[CHAMBERNUM];
	float pressureCommandDot[CHAMBERNUM];


	float position[CHAMBERNUM];
	float positionCommand[CHAMBERNUM];
	float positionCommandMin[CHAMBERNUM];
	float positionCommandMax[CHAMBERNUM];
	int angleCommand[CHAMBERNUM];

	float ABADCommand[5];

	float opening[CHAMBERNUM];
	float openingCommand[CHAMBERNUM];
	float OpeningLimArray[CHAMBERNUM][4];

	int interestedChamber;

	TRAJ_GEN positionTrajectory[CHAMBERNUM];
	PID_Type pressurePID[CHAMBERNUM];

	float frequency;
	float pressureDeadZone;


	uint8_t startControl;
	int32_t controlTime;
	uint8_t builtInControlFlag;
	int32_t valveOpen;
	int wp;
	int startDemo;


};


#ifdef __cplusplus
}
#endif
#endif /* SOFTHAND_H_ */
