///*
// * myinterface.cpp
// *
// *  Created on: 8 Mar 2019
// *      Author: diana
// */
////step1
//#include <PneumaticPlatform.h>
////step2
//PNEUMATIC_PLATFORM pneudrive(1);
////parameter setting
//uint16_t loop_gogogo=0;
//float sensorVmin=0.5;		//pressure sensor's parameters
//float sensorVmax=4.5;
//float sensorPmin=-100000;//////////////////////////
////float sensorPmax=300000;/////////////////////
//
////step3
//void setup(void)
//{
//	pneudrive.chambers[0]->attachPWM(0,1);
//	pneudrive.chambers[0]->attachSensor(0);
//	pneudrive.chambers[0]->setSensorRange_GaugePa(sensorVmin, sensorVmax, sensorPmin, sensorPmax);
//	pneudrive.pSource.attach(0,1);
//	pneudrive.pSource.pressureSensor.setSensorRange_GaugePa(sensorVmin, sensorVmax, sensorPmin, sensorPmax);
//	pneudrive.pSink.attach(1,2);
//	pneudrive.pSink.pressureSensor.setSensorRange_GaugePa(sensorVmin, sensorVmax, sensorPmin, sensorPmax);
//
//	pneudrive.writeFrequency(40);
//	pneudrive.chambers[0]->writeOpening(0);
//	pneudrive.pSource.stop();
//	pneudrive.pSink.stop();
//
//
//
//}
//void loop(void){
//	if (loop_gogogo) {
//			pneudrive.pSource.maintainPressure(110000,130000);
//			pneudrive.pSink.maintainPressure(-40000,-30000);//sink and source pressure setting£¿£¿£¿£¿£¿need or not
//			pneudrive.readPressure(0);//??????????????
//
//			float curt=millis()/1000.0;
//			float pdSin=sin(2*M_PI*0.5*curt)*40000+40000;
//			pneudrive.bufferPressureCommand(0, pdSin);
// 			pneudrive.writePressureCommand(0);
//		}
//}
//void serialDisplay() {
//	printf("Time: %4.3f	psource:%6.0f psink: %6.0f Pd: %6.0f Pa	P: %6.0f Pa	valveopening: %1.4f\r\n",
//			millis() / 1000.0,
//			pneudrive.pSource.pressure,
//			pneudrive.pSink.pressure,
//			pneudrive.pressureCommands[0],
//			pneudrive.readPressure(0),
//			pneudrive.chambers[0]->readOpening());
//}
//void serialCommandCallback(char *pSerialCommandBuffer) {
//	char commandChar;
//	int chambernum = 0;
//	float pCommand = 0;
//
//	//Read the received buffer with customized message. The first char means command type. The second number represents the interested chamber number. The third float number is the pressure command.
//	sscanf(pSerialCommandBuffer, "%c %d %f", &commandChar, &chambernum, &pCommand);
//
//	//customized message
//	if (commandChar == 'p') {
//		//Only change the value of command, leaving the control part to the loop().
//		pneudrive.bufferPressureCommand(chambernum, pCommand);
//	} else if (commandChar == 'g') {
//		loop_gogogo = 1;
//	} else if (commandChar == 's') {
//		loop_gogogo = 0;
//		pneudrive.chambers[0]->writeOpening(0);
//		pneudrive.pSource.stop();
//		pneudrive.pSink.stop();
//	} else if (commandChar == 'f'){
//		pneudrive.writeFrequency(chambernum);
//	} else if (commandChar == 'u'){
//		if (chambernum == 0)
//			pneudrive.chambers[0]->openingMinN=pCommand;
//		else if(chambernum==1)
//			pneudrive.chambers[0]->openingMaxN=pCommand;
//		else if(chambernum==2)
//			pneudrive.chambers[0]->openingMinP=pCommand;
//		else if(chambernum==3)
//			pneudrive.chambers[0]->openingMaxP=pCommand;
//
//	}
//}
//
