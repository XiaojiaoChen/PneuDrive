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


#define BELLOWNUM 6
#define SEGMENTNUM 2

#include "PneuDrive.h"

#include "joyStick.h"

enum CONTROL_MODE{
	joyStickControl,
	mannualControl,
	autofeedbackControl,
	pressureControl,
	positionControl,
	visionControl,
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



class SOFT_ARM_SEGMENT{
public:
	SOFT_ARM_SEGMENT(int num=BELLOWNUM);

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
	float alpha;
	float beta;
	float phyc;
	float phys;
	float phyp;
	float pressure[BELLOWNUM];
	float pressureFil[BELLOWNUM];

	float alphaModeled;
	float betaModeled;
	float lengthModeled;
	float encoderLength;

	float pLimitOptimal;
	float lengthD;
	float alphaD;
	float betaD;
	float phycD;
	float physD;
	float phypD;
	float pressureD[BELLOWNUM];
	float pressureDFeed[BELLOWNUM];
	float pressureDBack[BELLOWNUM];

	float lengthCommand;
	float alphaCommand;
	float betaCommand;

	float angle;
	float bending;

	float tAutofeedbackBase;
	float tAutofeedbackCur;

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

	float pressureCommandMin[BELLOWNUM];
	float pressureCommandMax[BELLOWNUM];
	float OpeningLimArray[BELLOWNUM][4];

	float bellowProjection[BELLOWNUM];
	float bellowConfigurationAngle[BELLOWNUM];
	float bellowConfigurationRadius;
	float bellowConfigurationPositionX[BELLOWNUM];
	float bellowConfigurationPositionY[BELLOWNUM];

	float bellowConfigurationAngleCos[BELLOWNUM];
	float bellowConfigurationAngleSin[BELLOWNUM];

	uint8_t startControl;
	int32_t controlTime;
	int32_t valveOpen;
	int wp;
	enum CONTROL_MODE commandSource;
	int startDemo;
	int frequencyDirty;

	int interestedBellow;
	int lengthAnalogPort;


	float dalpha;
	float dalphaRatio;
	float dbeta;
	float dlength;
	float dphycD;
	float dphysD;
	float Tx;
	float Ty;

	float Texta;
	float Textb;
	float Fl;

	PID_Type *alphaPID;
	PID_Type *betaPID;
	PID_Type *lengthPID;

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
 	float C1;


 	float phycFeedbackBeta;
 	float phycFeedbackAlpha;
 	float phycFeedback;
 	float phycAll;
 	float physFeedbackBeta;
 	float physFeedbackAlpha;
 	float physFeedback;
 	float phypFeedback;
 	float physAll;
 	float lengthAll;
 	float phypAll;

 	IMUDATA imuData;

 	void switchValveStatus();
	void joyStickController(int pControlMode);
	void joyStickOpeningController();
	void joyStickABLController();
	void setup();
	void loop();
	void display();
	void receiveCommand(char *pSerialReceiveBuffer);
	void receiveABLCommand(char *);

	void lengthTrack(float lengthD);
	void alphaTrack(float alphaD);
	void betaTrack(float betaD);
	void stiffnessAlphaTrack();

	void ABLD2PD();
	void PD2ABLD();
	float getPhycD();
	float getPhysD();
	void lowPassABLCommand();

	void ABL2P();
	void P2ABLModeled();
	float getPhyc();
	float getPhys();
	void quatern2AB();
	void geometricController(int feedbackFlag);


};

#ifdef __cplusplus
}
#endif
#endif /* USERINC_SOFTARMSEGMENT_H_ */
