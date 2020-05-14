/*
 * joyStick.h
 *
 *  Created on: Jun 14, 2018
 *      Author: Xiaojiao Chen
 *	Laboratory: Bio-Control Lab
 * Orgnization: the University of Hong Kong
 */

#ifndef JOYSTICK_H_
#define JOYSTICK_H_
#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f7xx_hal.h"

#define JOYSTICK_BUTTON_NUM 8
class JOYSTICK{
public:
	JOYSTICK();
	void receiveJoyStickCommand(uint8_t buttonSeq,int jxx,int jyy,int jzz,int jRxx,int jRyy,int jRzz);

 	int jx;
 	int jy;
 	int jz;

 	int jRx;
 	int jRy;
 	int jRz;

 	int jxOffset;
 	int jyOffset;
 	int jzOffset;

 	int jRxOffset;
 	int jRyOffset;
 	int jRzOffset;

 	union
 	{
 		struct{
 			uint8_t BtnSouth;
			uint8_t BtnEast;
			uint8_t BtnNorth;
			uint8_t BtnWest;
			uint8_t BtnThumbR;
			uint8_t BtnThumbL;
			uint8_t BtnRZ;
			uint8_t BtnZ;
 		};
 		uint8_t Btns[8];
 	};

 };

#ifdef __cplusplus
}
#endif
#endif /* JOYSTICK_H_ */
