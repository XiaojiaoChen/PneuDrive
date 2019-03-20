/*This is the an example controlling one chamber using low level APIs to direct manipulate hardware resources*/
/*******************************************************************************************/


/*Platform header file, always include this in your interface file*/
#include <PneumaticPlatform.h>




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

/*Create global variables to simplify programming for small projects*/
float chamberPressure=0;
float chamberPressureCommand=0;
float chamberValveOpening=0;
float chamberPressureDeadZone=2000;

float pSourcePressure=0;
float pSourcePressureCommand=0;

float pSinkPressure=0;
float pSinkPressureCommand=0;


#define PUMPSTART 1
#define PUMPSTOP 0

/*Called once in the beginning*/
void setup() {

}

/*Running at a default frequency of 1KHz, altered by changing CONTROL_FREQUENCY in the file UserConfig.h*/
void loop() {

	/*customized switch flag*/
	if (loop_gogogo) {

		/*Update and maintain the upper stream and lower stream pressure*/
		float pSourceVoltage=AnalogRead(pSourceAnalogInPort);
		pSourcePressure=MAP(pSourceVoltage,sensorVmin,sensorVmax,sensorPmin,sensorPmax);
		if(pSourcePressure<100000){
			DigitalWrite(pSourceDigitalOutPort,PUMPSTART); //start pump
		}
		else if(pSourcePressure>110000){
			DigitalWrite(pSourceDigitalOutPort,PUMPSTOP);	//stop pump
		}

		/*Update and control pressure of chamber 0*/
		float chamberVoltage=AnalogRead(sensorAnalogInPort);
		chamberPressure=MAP(chamberVoltage,sensorVmin,sensorVmax,sensorPmin,sensorPmax);
		float p_error=chamberPressure-chamberPressureCommand;
		if(p_error<chamberPressureDeadZone && p_error>-chamberPressureDeadZone)	{
			chamberValveOpening=0;
		}
		else{
			chamberValveOpening=MAP(p_error,-50000,50000,1,-1);
		}

		/*actuation of valve*/
		if(chamberValveOpening>=0){
			PWMWriteDuty(inValvePWMOutPort,chamberValveOpening);
			PWMWriteDuty(outValvePWMOutPort,0);
		}
		else{
			PWMWriteDuty(inValvePWMOutPort,0);
			PWMWriteDuty(outValvePWMOutPort,-chamberValveOpening);
		}
	}
}

/*Running at a default frequency of 20KHz, altered by changing DISPLAY_FREQUENCY in the file UserConfig.h*/
void serialDisplay() {
	printf("Time: %4.3f	PressureCommand: %6.0f Pa	Pressure: %6.0f Pa	valve opening: %1.4f\r\n",
			millis() / 1000.0,
			chamberPressureCommand,
			chamberPressure,
			chamberValveOpening);
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
		chamberPressureCommand=pCommand;
	} else if (commandChar == 'g') {
		loop_gogogo = 1;
	} else if (commandChar == 's') {
		loop_gogogo = 0;
	}
}
