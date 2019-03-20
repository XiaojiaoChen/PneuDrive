/*This is the an example controlling one chamber using high level Class*/
/*******************************************************************************************/

/*Platform header file, always include this in your interface file*/
#include <PneumaticPlatform.h>

/*Create a global object, “1” indicating the quantity of the chambers*/
PNEUMATICS_PLATFORM pneudrive(1);

/*Create a flag to select the effective part of loop().*/
uint16_t loop_gogogo = 0;

uint16_t sensorAnalogInPort = 0;	//Analog port of the pressure sensor of chamber 0
uint16_t inValvePWMOutPort = 0;		//PWM port of the inlet valve of chamber 0
uint16_t outValvePWMOutPort = 1;	//PWM port of the outlet valve of chamber 0

uint16_t pSourceDigitalOutPort = 0;	//Digital port of the pump of pSource
uint16_t pSourceAnalogInPort = 7;	//Analog port of the pressure sensor of pSource

uint16_t pSinkDigitalOutPort = 1;	//Digital port of the pump of pSink
uint16_t pSinkAnalogInPort = 6;		//Analog port of the pressure sensor of pSink

float sensorVmin=0.5;		//pressure sensor's parameters
float sensorVmax=4.5;
float sensorPmin=0;
float sensorPmax=200000;

/*Called once in the beginning*/
void setup() {

	/*Configure the pump port and sensor paramter of the source*/
	pneudrive.pSource.attach(pSourceDigitalOutPort,pSourceAnalogInPort);
	pneudrive.pSource.pressureSensor.setSensorRange_GaugePa(sensorVmin, sensorVmax, sensorPmin, sensorPmax);

	/*Configure the pump port and sensor parameter of the sink*/
	pneudrive.pSink.attach(pSinkDigitalOutPort,pSinkAnalogInPort);
	pneudrive.pSink.pressureSensor.setSensorRange_GaugePa(sensorVmin, sensorVmax, sensorPmin, sensorPmax);

	/*Attach the valves and pressure sensors of chamber 0 to corresponding ports.
	 *Set the pressure sensor range. The parameter list are (vMin, vMax, pMin, pMax).
	 * vMin vMax have the unit of voltage. pMin pMax use the gauge pressure, has the unit of Pa. For example, atmosphere is 0, and vacuum is -101325.*/
	pneudrive.chambers[0]->attach(inValvePWMOutPort, outValvePWMOutPort,sensorAnalogInPort);
	pneudrive.chambers[0]->pressureSensor.setSensorRange_GaugePa(sensorVmin, sensorVmax, sensorPmin, sensorPmax);

}

/*Running at a default frequency of 1KHz, altered by changing CONTROL_FREQUENCY in the file UserConfig.h*/
void loop() {

	/*customized switch flag*/
	if (loop_gogogo) {

		/*maintain the upper stream and lower stream pressure*/
		pneudrive.pSource.maintainPressure(100000,110000);

		/*maintain a downsteam pressure if necessary*/
		//pneudrive.pSink.maintainPressure(-30000,-40000);

		/*Update sensor data of chamber 0*/
		pneudrive.readPressure(0);

		/*Actual control of chamber 0. The pressureCommand could be buffered elsewhere. Here we just control the pressure according to buffered pressure command*/
		pneudrive.writePressureCommand(0);
	}
}

/*Running at a default frequency of 20KHz, altered by changing DISPLAY_FREQUENCY in the file UserConfig.h*/
void serialDisplay() {
	printf("Time: %4.3f	PressureCommand: %6.0f Pa	Pressure: %6.0f Pa	valve opening: %1.4f\r\n",
			millis() / 1000.0,
			pneudrive.pressureCommands[0],
			pneudrive.readPressure(0),
			pneudrive.chambers[0]->readOpening());
}

/*Callback function when serial port 3 receives commands with ending of "\r\n", the parameter is the pointer to the received string buffer*/
void serialCommandCallback(char *pSerialCommandBuffer) {
	char commandChar;
	int chambernum = 0;
	float pCommand = 0;

	//Read the received buffer with customized message. The first char means command type. The second number represents the interested chamber number. The third float number is the pressure command.
	sscanf(pSerialCommandBuffer, "%c %d %f", &commandChar, &chambernum, &pCommand);

	//customized message
	if (commandChar == 'p') {
		//Only change the value of command, leaving the control part to the loop().
		pneudrive.bufferPressureCommand(chambernum, pCommand);
	} else if (commandChar == 'g') {
		loop_gogogo = 1;
	} else if (commandChar == 's') {
		loop_gogogo = 0;
	}
}
