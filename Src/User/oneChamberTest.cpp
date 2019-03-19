/*
 * oneChamberTest.cpp
 *
 *  Created on: Mar 17, 2019
 *      Author: 402072495
 */
//step1
#include <PneumaticPlatform.h>
//step2
extern TIM_HandleTypeDef    htim5;
PNEUMATIC_PLATFORM pneudrive(1);
//parameter setting
uint16_t loop_gogogo=0;
float flowrate;
float sensorVmin=0.5;		//pressure sensor's parameters
float sensorVmax=4.5;
float sensorPmin=-100000;//////////////////////////
float sensorPmax=300000;/////////////////////
int32_t c1,c2,c3,c4;
int flagReceive=0;
int frequencyy=40;
uint32_t ReceiveTime=0;
uint32_t endTime=0;
//step3
void setup(void)
{

	pneudrive.chambers[0]->attachPWM(0,1);
	pneudrive.chambers[0]->writeOpening(0);

	pneudrive.chambers[0]->attachSensor(BUILTIN_ANA_IN_NUM+32-1);
	pneudrive.chambers[0]->setSensorRange_GaugePa(sensorVmin, sensorVmax, sensorPmin, sensorPmax);

	pneudrive.writeFrequency(frequencyy);


}
void loop(void){
	if (loop_gogogo) {
		flowrate=getFlowrate();
			pneudrive.readPressure(0);
 			pneudrive.writePressureCommand(0);
 			if(flagReceive)
 				{
 					c1=TIC();
 					flagReceive=0;
 					endTime=millis();
					uint32_t endTime2=(uint32_t)(htim5.Instance->CNT);
					endTime=(endTime%10)*1000+endTime2;
 				}
 			else
 			{
 				ReceiveTime=0;
 				endTime=0;
 			}
		}
}
void serialDisplay() {

	printf("%d	%d %d %d %d %1.3f %d %3.2f %d\r\n",
			millis(),
			(int)(pneudrive.pressureCommands[0]),
			ReceiveTime,
			endTime,
			c1,
			pneudrive.chambers[0]->readOpening(),
			(int)(pneudrive.readPressure(0)),
			flowrate,
			frequencyy);
}
void serialCommandCallback(char *pSerialCommandBuffer) {
	char commandChar;
	int chambernum = 0;
	float pCommand = 0;

	//Read the received buffer with customized message. The first char means command type. The second number represents the interested chamber number. The third float number is the pressure command.
	sscanf(pSerialCommandBuffer, "%c %d %f", &commandChar, &chambernum, &pCommand);

	//customized message
	if (commandChar == 'p') {
		//Only change the value of command, leaving the control part to the loop().
		flagReceive=1;
		ReceiveTime=millis();
		uint32_t ReceiveTime2=(uint32_t)(htim5.Instance->CNT);
		ReceiveTime=(ReceiveTime%10)*1000+ReceiveTime2;
		resetTIC();

		pneudrive.bufferPressureCommand(chambernum, pCommand);

	} else if (commandChar == 'g') {
		loop_gogogo = 1;
	} else if (commandChar == 's') {
		loop_gogogo = 0;
		pneudrive.chambers[0]->writeOpening(0);
		pneudrive.pSource.stop();
		pneudrive.pSink.stop();
	} else if (commandChar == 'f'){
		frequencyy=chambernum;
		pneudrive.writeFrequency(chambernum);
	} else if (commandChar == 'u'){
		if (chambernum == 0)
			pneudrive.chambers[0]->openingMinN=pCommand;
		else if(chambernum==1)
			pneudrive.chambers[0]->openingMaxN=pCommand;
		else if(chambernum==2)
			pneudrive.chambers[0]->openingMinP=pCommand;
		else if(chambernum==3)
			pneudrive.chambers[0]->openingMaxP=pCommand;

	}
	else if (commandChar == 'O') {
		pneudrive.chambers[chambernum]->opening=pCommand;
	}
}



