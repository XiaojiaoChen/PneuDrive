/*For more examples, please go to the ./example folder*/

//step1
#include <PneuDrive.h>


PNEUDRIVE pneudrive(1);

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
void setup()
{

	pneudrive.chambers[0]->attachPWM(0,1);
	pneudrive.chambers[0]->writeOpening(0);

	pneudrive.chambers[0]->attachSensor(BUILTIN_ANA_IN_NUM+32-1);
	pneudrive.chambers[0]->setSensorRange_GaugePa(sensorVmin, sensorVmax, sensorPmin, sensorPmax);

	pneudrive.writeFrequency(frequencyy);


}
void loop(){
	if (loop_gogogo) {
		flowrate=getFlowrate();
			pneudrive.readPressure(0);
 			pneudrive.writePressureCommand(0);
 			if(flagReceive)
 				{
 					flagReceive=0;
					endTime=micros();
 				}
 			else
 			{
 				ReceiveTime=0;
 				endTime=0;
 			}
		}
}
void serialDisplay() {

	printf("%d	%ld %ld %ld %d %1.3f %d %3.2f %d\r\n",
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
void serialReceiveCallback(char *pSerialCommandBuffer) {
	char commandChar;
	int chambernum = 0;
	float pCommand = 0;

	//Read the received buffer with customized message. The first char means command type. The second number represents the interested chamber number. The third float number is the pressure command.
	sscanf(pSerialCommandBuffer, "%c %d %f", &commandChar, &chambernum, &pCommand);

	//customized message
	if (commandChar == 'p') {
		//Only change the value of command, leaving the control part to the loop().
		flagReceive=1;
		ReceiveTime=micros();
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



