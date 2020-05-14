/*
 * Interface_example_softArm.cpp
 *
 *  Created on: Jan 16, 2019
 *      Author: Xiaojiao Chen
 *		 Email: chen2014@connect.hku.hk
 *	Laboratory: Bio-Control Lab
 * Orgnization: the University of Hong Kong
 */

#include <PneuDrive.h> // always include this header in your own code.
#include "spiSlave.h"
#include <SoftArm.h>

SOFT_ARM softArm;

void setup() {
	/*setup 1000Hz control loop*/
	setPeriodControlLoop(10);

	/*setup 50Hz serial display loop*/
	setPeriodSendLoop(50);

	/*soft arm chambers' PWM port mapping*/
	softArm.setupChamberPWMPort();

	/*start canBus receive*/
	canConfig();

	/**********start the SPI slave in DMA*****/
	spiSlaveStart();

}

void loop() {

	/**********The sensor feedback are automatically received from nodes by CAN,
	 and stored in sensorData[segNum][bellowNum] in softArm.*/

	/**********The commands are automatically updated from SPI1 using DMA,
	 and stored in commandData[segNum][bellowNum] in softArm.*/

	/**Write the command of each chamber, either pressure or opening type*/
	softArm.writeCommandAll();

}

/*serial output using DMA*/
void serialDisplay() {
	//printf("Time: %d\r\n",millis());
	printf("Time:%10lu       PRESSURE(KPa)                 DISTANCE(mm)\r\n",
			millis());
	for (int i = 0; i < SEGMENTNUM; i++) {
		printf(
				"            %hu, %hu, %hu, %hu, %hu, %hu    |     %hu,  %hu, %hu, %hu, %hu, %hu\r\n",
				softArm.sensorData[i][0].pressure,
				softArm.sensorData[i][1].pressure,
				softArm.sensorData[i][2].pressure,
				softArm.sensorData[i][3].pressure,
				softArm.sensorData[i][4].pressure,
				softArm.sensorData[i][5].pressure,
				softArm.sensorData[i][0].distance,
				softArm.sensorData[i][1].distance,
				softArm.sensorData[i][2].distance,
				softArm.sensorData[i][3].distance,
				softArm.sensorData[i][4].distance,
				softArm.sensorData[i][5].distance);
	}
}

//called when serial input receive string with ending '\r\n'
void serialReceiveCallback(char *pSerialReceiveBuffer) {

	if (pSerialReceiveBuffer[0] == 'k') {
		//	/**Write command for each actuator, q&r of kalman filter for laser, */
		softArm.canBusCommand[0] = 0xFF;
		softArm.canBusCommand[1] = 0xEF;
		softArm.canBusCommand[2] = 0xFE;
		softArm.canBusCommand[3] = 0xAF;
		canSend();
	}
}

