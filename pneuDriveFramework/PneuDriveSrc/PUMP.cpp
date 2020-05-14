/*
 * PUMP.cpp
 *
 *  Created on: Jun 15, 2018
 *      Author: Xiaojiao Chen
 *		 Email: chen2014@connect.hku.hk
 *	Laboratory: Bio-Control Lab
 * Orgnization: the University of Hong Kong
 */

#include <PneuDriveLL.h>
#include <PUMP.h>


PUMP::PUMP(int num) {
	status=0;
	attach(num);
}

void PUMP::attach(int num)
{
	DigitalPort = num;
}

void PUMP::start()
{

	DigitalWrite(DigitalPort,1-PWM_DIGITAL_OUTPUT_REVERSED);
	status=1;

}

void PUMP::stop()
{
	DigitalWrite(DigitalPort,PWM_DIGITAL_OUTPUT_REVERSED);
	status=0;

}

