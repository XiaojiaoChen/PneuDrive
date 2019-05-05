/*This is a simple example of using PneuDrive to control one chamber. For more examples, please go to the ./example folder*/


//step0: configure your hardware component in UserConfig.h and include "PneuDrive.h"
#include "UserConfig.h"
#include <PneuDrive.h>

//parameter setting
uint16_t loop_gogogo=0;
uint16_t commandSource=0;

//sin pressure command parameters
float pSinAmp=40000;
float pSinFre=0.2;
float pSinOff=40000;

//pressure source and sink set-up pressure
float pSourceLimit=150000;
float pSinkLimit=-30000;

//Create a global pointer
PNEUDRIVE *ptPneudrive;

void setup()
{
	//sensor specifications
	float sensorVmin=0.5;
	float sensorVmax=4.5;
	float sensorPmin=-100000;
	float sensorPmax=300000;
	float valvePWMFrequency=80;

	//create an object of Class PNEUDRIVE
	ptPneudrive = new PNEUDRIVE(1);

	//attach to PWM port 0 for inlet valve and PWM port 1 for outlet valve.
	ptPneudrive->chambers[0]->attachPWM(0,1);
	//attach to analogIn port 0 for pressure sensor
	ptPneudrive->chambers[0]->attachSensor(0);

	//write opening of the chamber to be 0. opening could be [-1,1], where [-1,0] represents the delfating duty and [0,1] represents inflating duty.
	ptPneudrive->chambers[0]->writeOpening(0);

	//set sensor range
	ptPneudrive->chambers[0]->setSensorRange_GaugePa(sensorVmin, sensorVmax, sensorPmin, sensorPmax);

	//write proper frequency of all the valves atttached.
	ptPneudrive->writeFrequency(valvePWMFrequency);

	//PSource is optional if you have other pressurized source.
	//attach inlet pump to digital out port 0, analogIn port 1.
	ptPneudrive->pSource.attachPump(0);
	ptPneudrive->pSource.attachSensor(1);
	ptPneudrive->pSource.pressureSensor.setSensorRange_GaugePa(sensorVmin, sensorVmax, sensorPmin, sensorPmax);
	ptPneudrive->pSource.stop();

	//PSink is optional if atmosphere is OK
	//attach vacuum pump to digital out port 1, analogIn port 2.
	ptPneudrive->pSink.attachPump(1);
	ptPneudrive->pSink.attachSensor(2);
	ptPneudrive->pSink.pressureSensor.setSensorRange_GaugePa(sensorVmin, sensorVmax, sensorPmin, sensorPmax);
	ptPneudrive->pSink.stop();

}
void loop(){
	float aa=getFlowrate();
	if (loop_gogogo) {
		//maintain the upstream to be within [110KPa, 130KPa], and downstream pressure to be within[-40KPa, -30KPa].
		ptPneudrive->pSource.maintainPressure(pSourceLimit,pSourceLimit+20000);
		ptPneudrive->pSink.maintainPressure(pSinkLimit-10000,pSinkLimit);

		if(commandSource==0){
			//generate a sinoid pressure command
			float pSin=pSinAmp*sin(2*M_PI*pSinFre*(millis()/1000.0))+pSinOff;
			ptPneudrive->bufferPressure(0, pSin);
		}

		//control the pressure of chamber 0 with built-in controller, the pressure command has  already been buffered either from serial port or from above generated sin curve.
		ptPneudrive->writeBufferedPressure(0);
	}
}
void serialDisplay() {

	printf("T: %4.3f source: %3d sink: %3d Pd: %3d P: %3d valve: %1.4f\r\n",
				millis() / 1000.0,
				(int)(ptPneudrive->pSource.pressure/1000),
				(int)(ptPneudrive->pSink.pressure/1000),
				(int)(ptPneudrive->pressureCommands[0]/1000),
				(int)(ptPneudrive->readPressure(0)/1000),
				ptPneudrive->chambers[0]->readOpening());
}

void serialReceiveCallback(char *pSerialCommandBuffer) {
	char commandChar[20]={0};
	int chambernum = 0;
	float pCommand = 0;

	//Read the received buffer with customized message. The first char means command type. The second number represents the interested chamber number. The third float number is the pressure command.
	sscanf(pSerialCommandBuffer, "%s %d %f", &(commandChar[0]), &chambernum, &pCommand);

	/*Commands explanation
	 * g			: start control loop
	 * s			: stop control loop
	 *
	 * a 0 40000 	: change the amplitude of the sinoid pressure command of chamber 0 to 40000Pa
	 * f 0 0.5		:change the frequency of the sinoid pressure command of chamber 0 to 0.5Hz
	 * o 0 40000 	: change the offset of the sinoid pressure command of chamber 0 to 40000Pa
	 *
	 * F 0 80 		: change the valve PWM frequency of chmaber 0 to be 80Hz.
	 * p -2 130000	: change the pressure source to be 130000~150000Pa
	 * p -1 -30000	: change the pressure sink to be -40000~-30000Pa
	 * p 0 100000	: change the pressure command to be constant 100000Pa. This would overwrite the built-in sinoid pressure command
	 */

	//customized message
	if (commandChar[0] == 'p') {

		if(chambernum==-2)
		{
			pSourceLimit = pCommand;
		}
		else if(chambernum==-1)
		{
			pSinkLimit = pCommand;
		}
		else if(chambernum>=0)
		{//change the pressure command of the chamber
			//Only buffer the command without acting immediately, leaving the actual control to the loop() function.
			ptPneudrive->bufferPressure(chambernum, pCommand);
			commandSource=1;
		}
	}
	else if (commandChar[0] == 'g') {
		//start the loop()
		loop_gogogo = 1;
	} else if (commandChar[0] == 's') {
		//stop the loop()
		loop_gogogo = 0;
		ptPneudrive->chambers[0]->writeOpening(0);
		ptPneudrive->pSource.stop();
		ptPneudrive->pSink.stop();
	} else if (commandChar[0] == 'F'){
		//change the frequency of chambernum
		ptPneudrive->writeFrequency(pCommand);
	} else if (commandChar[0] == 'u'){
		//change the valve opening limit of the inlet valve and the outlet valve instead of common[0,1].
		if (chambernum == 0)
			ptPneudrive->chambers[0]->openingMinN=pCommand;
		else if(chambernum==1)
			ptPneudrive->chambers[0]->openingMaxN=pCommand;
		else if(chambernum==2)
			ptPneudrive->chambers[0]->openingMinP=pCommand;
		else if(chambernum==3)
			ptPneudrive->chambers[0]->openingMaxP=pCommand;
	}
	else if (commandChar[0] == 'O') {
		//Mannually change the valve opening
		ptPneudrive->chambers[chambernum]->opening=pCommand;
	}
	else if (commandChar[0] == 'a') {
			//change the amplitude of the sinoid pressure command
			commandSource=0;
			pSinAmp=pCommand;
		}
	else if (commandChar[0] == 'f'){
		//change the frequency of the sinoid pressure command
			pSinFre=pCommand;
	}
	else if (commandChar[0] == 'o'){
			//change the frequency of the sinoid pressure command
				pSinOff=pCommand;
		}
}



