/*This is the an example controlling one chamber using high level Class*/
/*******************************************************************************************/

/*Platform header file, always include this in your interface file*/
#include <PneuDrive.h>

/*Configure the hardware components, control loop frequency*/
#include <PneuDriveConfig.h>

/*Other headers*/
#include "ADBoard.h"
#include "PWMBoardSPI.h"
#include "myPID.h"

/*Create a global object,  indicating the quantity of the chambers*/
PNEUDRIVE pneudrive(1);

/*Create a flag to select the effective part of loop().*/
uint16_t loop_gogogo = 0;

float sensorVmin=1.5;		//pressure sensor's parameters
float sensorVmax=4.5;
float sensorPmin=0;
float sensorPmax=200000;
int32_t timings[10];
float kk=0.013;

float pup=70000;
float pdown=-40000;
float ampli=0;
float fres=0;

int32_t curT=0;
float loopT=0;

int wp=0;

/*Called once in the beginning*/
void setup() {

	/*Configure the pump port and sensor paramter of the source*/
	pneudrive.pSource.attach(0,5);
	pneudrive.pSource.pressureSensor.setSensorRange_GaugePa(sensorVmin, sensorVmax, sensorPmin, sensorPmax);

	/*Configure the pump port and sensor parameter of the sink*/
	pneudrive.pSink.attach(1,6);
	pneudrive.pSink.pressureSensor.setSensorRange_GaugePa(sensorVmin, sensorVmax, sensorPmin, sensorPmax);

	/*Attach the valves and pressure sensors of chamber 0 to corresponding ports.
	 *Set the pressure sensor range. The parameter list are (vMin, vMax, pMin, pMax).
	 * vMin vMax have the unit of voltage. pMin pMax use the gauge pressure, has the unit of Pa. For example, atmosphere is 0, and vacuum is -101325.*/
	pneudrive.chambers[0]->attach(0, 1,4);
	pneudrive.chambers[0]->pressureSensor.setSensorRange_GaugePa(sensorVmin, sensorVmax, sensorPmin, sensorPmax);

	pneudrive.chambers[0]->setValveOpeningLimit(-0.3,-0.1,0.1,0.12);

}

/*Running at a default frequency of 1KHz, altered by changing CONTROL_FREQUENCY in the file UserConfig.h*/
void loop() {

//	/*customized switch flag*/
//	if (loop_gogogo) {
//
//		/*maintain the upper stream and lower stream pressure*/
//		pneudrive.pSource.maintainPressure(pup,pup+10000);
//
//		/*maintain a downsteam pressure if necessary*/
//		pneudrive.pSink.maintainPressure(pdown,pdown-10000);
//
//		/*Update sensor data of chamber 0*/
//		pneudrive.readPressure(0);
//
//		if(wp==0)
//		{
//			loopT=(millis()-curT)/1000.0f;
//			pneudrive.bufferPressure(0, ampli*sin(2*M_PI*fres*loopT));
//		}
//
//		/*Actual control of chamber 0. The pressureCommand could be buffered elsewhere. Here we just control the pressure according to buffered pressure command*/
//		pneudrive.writeBufferedPressure(0);
//	}

	timings[0]=micros();
	pneudrive.readPressure(0);
	pneudrive.chambers[0]->filterPressure();
	timings[1]=micros();
	pneudrive.chambers[0]->writePressure(120025);
	timings[2]=micros();

	timings[5]=timings[1]-timings[0];

	timings[3]=timings[2]-timings[1];
}

/*Running at a default frequency of 20KHz, altered by changing DISPLAY_FREQUENCY in the file UserConfig.h*/
void serialDisplay() {
//	printf("%4.3f	%d	%d\r\n",
//			millis() / 1000.0f,
//			(int)(pneudrive.pressureCommands[0]/1000),
//			(int)(pneudrive.readPressure(0)/1000));
	int32_t ttt=micros();
	printf("%d %d %d\r\n",
			timings[4],
			timings[3],
			timings[5]);
	timings[4]=micros()-ttt;
}

/*Callback function when serial port 3 receives commands with ending of "\r\n", the parameter is the pointer to the received string buffer*/
void serialReceiveCallback(char *pSerialCommandBuffer) {
	static char commandChar;
	static float pCommand = 0;
	static float ulims[4];
	//Read the received buffer with customized message. The first char means command type. The second number represents the interested chamber number. The third float number is the pressure command.
	sscanf(pSerialCommandBuffer, "%c %f", &commandChar, &pCommand);

	//customized message
	if (commandChar == 'p') {
		//Only change the value of command, leaving the control part to the loop().
		wp=0;
		pneudrive.bufferPressure(0, (pCommand*1000));
	} else if (commandChar == 'g') {
		loop_gogogo = 1;
		curT=millis();
	} else if (commandChar == 's') {
		loop_gogogo = 0;
		pneudrive.chambers[0]->writeOpening(0);
	}else if (commandChar == 'a') {
		wp=1;
		ampli=pCommand;
	}
	else if (commandChar == 'f') {
		wp=1;
		fres=pCommand;
	}
	else if(commandChar == 'u')
	{
		sscanf(&(pSerialCommandBuffer[2]), "%f %f %f %f" , &(ulims[0]),&(ulims[1]),&(ulims[2]),&(ulims[3]));
		pneudrive.chambers[0]->setValveOpeningLimit(ulims[0],ulims[1],ulims[2],ulims[3]);
	}
}
