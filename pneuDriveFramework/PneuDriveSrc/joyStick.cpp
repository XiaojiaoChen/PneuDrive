/*
 * joyStick.cpp
 *
 *  Created on: Jun 14, 2018
 *      Author: Xiaojiao Chen
 *	Laboratory: Bio-Control Lab
 * Orgnization: the University of Hong Kong
 */
#include "joyStick.h"

JOYSTICK::JOYSTICK()
{
	jx=0;
	jy=0;
	jz=0;
	jRx=0;
	jRy=0;
	jRz=0;
	BtnNorth=0;
	BtnEast=0;
	BtnSouth=0;
	BtnWest=0;
	BtnThumbR=0;
	BtnThumbL=0;
	jxOffset = 0;
	jyOffset = 0;
	jzOffset = 0;
	jRxOffset = 0;
	jRyOffset = 0;
	jRzOffset = 0;
}




void JOYSTICK::receiveJoyStickCommand(uint8_t buttonSeq,int jxx,int jyy,int jzz,int jRxx,int jRyy,int jRzz)
{

	BtnThumbR = (uint8_t)(buttonSeq & 0x01);
	BtnThumbL=(uint8_t)((buttonSeq>>1) & 0x01);
	BtnSouth = (uint8_t)((buttonSeq>>2) & 0x01);
	BtnEast = (uint8_t)((buttonSeq>>3) & 0x01);
	BtnNorth = (uint8_t)((buttonSeq>>4) & 0x01);
	BtnWest = (uint8_t)((buttonSeq>>5) & 0x01);

	jx=jxx-jxOffset;
	jy=jyy-jyOffset;
	jz=jzz-jzOffset;

	jRx=jRxx-jRxOffset;
	jRy=jRyy-jRyOffset;
	jRz=jRzz-jRzOffset;
}

