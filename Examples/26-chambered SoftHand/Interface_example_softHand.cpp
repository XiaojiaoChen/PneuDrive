/*
 * Interface_example_softHand.cpp
 *
 *  Created on: Jan 20, 2019
 *      Author: Xiaojiao Chen
 *		 Email: chen2014@connect.hku.hk
 *	Laboratory: Bio-Control Lab
 * Orgnization: the University of Hong Kong
 */

#include <PneumaticPlatform.h> // always include this header in your own code.
#include "SoftHand.h"


SOFT_HAND softHand;

void setup()
{
	softHand.setup();
}

void loop()
{
	softHand.loop();
}

void serialDisplay()
{
	softHand.display();
}

void serialCommandCallback(char *pSerialReceiveBuffer)
{
	softHand.receiveCommand(pSerialReceiveBuffer);
}

void wirelessCommandCallback(char *pWirelessReceiveBuffer)
{
	softHand.receiveCommand(pWirelessReceiveBuffer);
}

void clickButtonCallback(int Button_Interrupt_Pin)
{

}

