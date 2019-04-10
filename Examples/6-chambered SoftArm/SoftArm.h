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


#define BELLOWNUM 6


#include "PneuDrive.h"

#include "joyStick.h"

enum CONTROL_MODE{
	joyStickControl,
	mannualControl,
	pressureControl,
	positionControl,
};

typedef struct IMUFRAME_STRUCT{
	uint8_t headerEuler[2];
	int16_t Rx;
	int16_t Ry;
	int16_t Rz;
	int16_t temperature;
	uint8_t chekcum1;
	uint8_t headerQuotanion[2];
	int16_t q0;
	int16_t q1;
	int16_t q2;
	int16_t q3;
	uint8_t chekcum2;
}__attribute__((packed)) IMUFRAME;

typedef struct IMUDATA_STRUCT{
	float angleX;
	float angleY;
	float angleZ;
	float q0;
	float q1;
	float q2;
	float q3;
}__attribute__((packed)) IMUDATA;



class SOFT_ARM{
public:
	SOFT_ARM(int num=BELLOWNUM);

	void attach(int num,int PWMPort1,int PWMPort2,int AnalogPort);

	float readPressure(int num);
	void writePressure(int num,float pre);

	void readPressureAll();
	void controlPressureAll();

	void writeOpeningAll(float op);

	float readLength();
	void readIMU(char *);

	void increaseFrequency(float deltaFrequency);
	void increaseVelocity(float deltaVelocity);

	float length;
	float angle;
	float bending;

	JOYSTICK joyStick;

	float velocity;
	float frequency;
	float pressureBase;
	float openingBase;

	PNEUDRIVE *basePlatform;
	CHAMBER *bellows[BELLOWNUM];
	float psource_lowerlimit;
	float psource_upperlimit;
	float psink_lowerlimit;
	float psink_upperlimit;

	int holdflag[BELLOWNUM];
	float pressure[BELLOWNUM];
	float pressureCommand[BELLOWNUM];
	float pressureCommandMin[BELLOWNUM];
	float pressureCommandMax[BELLOWNUM];
	float OpeningLimArray[BELLOWNUM][4];

	float bellowProjection[BELLOWNUM];
	float bellowConfigurationAngle[BELLOWNUM];
	float bellowConfigurationRadius;
	float bellowConfigurationPositionX[BELLOWNUM];
	float bellowConfigurationPositionY[BELLOWNUM];

	uint8_t startControl;
	int32_t controlTime;
	int32_t valveOpen;
	int wp;
	enum CONTROL_MODE commandSource;
	int startDemo;
	int frequencyDirty;

	int interestedBellow;
	int lengthAnalogPort;

	float alpha;
	float beta;

 	float angleCommand;
 	float amplitudeCommand;
 	int Rmin;
 	int Rmax;
 	float rawAngle;
 	float rawAmplitude;
 	float rawAmplitudeMax;
 	int buttonCheckTime[JOYSTICK_BUTTON_NUM];


 	float k0;
 	float length0;
 	float crossA;
 	float radR;
 	float radr;

 	IMUDATA imuData;

 	void switchValveStatus();
	void joyStickController(int pControlMode);
	void setup();
	void loop();
	void display();
	void receiveCommand(char *pSerialReceiveBuffer);


	void lengthTrack(float lengthD);
	void alphaTrack(float alphaD);
	void betaTrack(float betaD);
	void stiffnessAlphaTrack();


};

#ifdef __cplusplus
}
#endif
#endif /* USERINC_SOFTARM_H_ */
