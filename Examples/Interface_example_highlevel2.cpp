///*This is the an example controlling one chamber using high level Class*/
///*******************************************************************************************/
//
///*Platform header file, always include this in your interface file*/
//#include <PneuDrive.h>
//
///*Configure the hardware components, control loop frequency*/
//#include <PneuDriveConfig.h>
//
///*Other headers*/
//#include "ADBoard.h"
//#include "PWMBoardSPI.h"
//#include "myPID.h"
//#include "main.h"
//#include "spi.h"
//#include "trajectoryGeneration.h"
//#include "i2c.h"
///*Create a global object,  indicating the quantity of the chambers*/
//PNEUDRIVE pneudrive(1);
//
///*Create a flag to select the effective part of loop().*/
//uint16_t loop_gogogo = 0;
//
//float sensorVmin=1.5;		//pressure sensor's parameters
//float sensorVmax=4.5;
//float sensorPmin=0;
//float sensorPmax=300000;
//float kk=0.013;
//TRAJ_GEN myTrajectory;
//float pup=30000;
//float pdown=-60000;
//float ampli=0;
//float fres=0;
//float ds=0;
//float ppresure=0;
//float pressureDes;
//float dPressureP;
//float dPressureI;
//float dd=0;
//float pre[3];
//int32_t CommandTime=0;
//float laserVol=0;
//float laserPos=0;
//float sensorVol=0;
//float sensorPos=0;
//int32_t curT=0;
//float sensorPosOffset=0;
//float laserPosOffset=0;
//float loopT=0;
//float localKp=0;
//float preCommand=0;
//float posCommand=0;
//int wp=0;
//float myKp=5;
//float myKi=0.02;
//int stepCommand=0;
//float astep=0;
//int aa=0;
//float preAmp=10000;
//float preFre=2;
//float preOffset=-2000;
//int prePeriod=500;
//uint16_t minitem[2];
//static uint32_t DummyByte[] = { 0xFFFFFFFF};
//int32_t localT=0;
//float localTime=0;
//int32_t localCount=0;
//int32_t cycleNumbers=0;
//float preCommandup=0;
//float preCommanddown=0;
///*Called once in the beginning*/
//void setup() {
//
////	/* 1000Hz control loop*/
//	setPeriodControlLoop(10);
////
////	/* 50Hz serial display loop*/
////	setPeriodSendLoop(50);
////
////	/*Configure the pump port and sensor parameter of the sink*/
////	pneudrive.pSource.attach(1,4);
////	pneudrive.pSource.pressureSensor.setSensorRange_GaugePa(sensorVmin, sensorVmax, sensorPmin, sensorPmax);
////	pneudrive.pSource.stop();
////
////	pneudrive.pSink.attach(0,6);
////	pneudrive.pSink.pressureSensor.setSensorRange_GaugePa(sensorVmin, sensorVmax, sensorPmin, sensorPmax);
////	pneudrive.pSink.stop();
////
////	/*Attach the valves and pressure sensors of chamber 0 to corresponding ports.
////	 *Set the pressure sensor range. The parameter list are (vMin, vMax, pMin, pMax).
////	 * vMin vMax have the unit of voltage. pMin pMax use the gauge pressure, has the unit of Pa. For example, atmosphere is 0, and vacuum is -101325.*/
////	pneudrive.chambers[0]->attach(0,1,5);
////	pneudrive.chambers[0]->pressureSensor.setSensorRange_GaugePa(sensorVmin, sensorVmax, sensorPmin, sensorPmax);
////
////	pneudrive.chambers[0]->setValveOpeningLimit(-1,-1,1,1);
////
////	pneudrive.chambers[0]->writeOpening(0);
////	pneudrive.writeFrequency(30);
////	InitTraj(&myTrajectory,1);
//}
//
//typedef struct SENSORDATA_TAG{
//int16_t pressure;
//float distance;
//float q0;
//float q1;
//float q2;
//float q3;
//}SENSORDATA;
//
//SENSORDATA sensorDatas[12];
///*Running at a default frequency of 1KHz, altered by changing CONTROL_FREQUENCY in the file UserConfig.h*/
//void loop() {
//	int32_t ret;
//
////		printf("%d %d %2.0f %1.3f %1.3f %1.3f %1.3f;%d %d %2.0f %1.3f %1.3f %1.3f %1.3f;%d %d %2.0f %1.3f %1.3f %1.3f %1.3f;%d %d %2.0f %1.3f %1.3f %1.3f %1.3f;%d %d %2.0f %1.3f %1.3f %1.3f %1.3f;%d %d %2.0f %1.3f %1.3f %1.3f %1.3f \r\n",
////	    		0,
////	    		  sensorDatas[0].pressure,
////				  sensorDatas[0].distance,
////				  sensorDatas[0].q0,
////				  sensorDatas[0].q1,
////				  sensorDatas[0].q2,
////				  sensorDatas[0].q3,
////				  1,
////				  	    		  sensorDatas[1].pressure,
////				  				  sensorDatas[1].distance,
////				  				  sensorDatas[1].q0,
////				  				  sensorDatas[1].q1,
////				  				  sensorDatas[1].q2,
////				  				  sensorDatas[1].q3,
////								  2,
////								  	    		  sensorDatas[2].pressure,
////								  				  sensorDatas[2].distance,
////								  				  sensorDatas[2].q0,
////								  				  sensorDatas[2].q1,
////								  				  sensorDatas[2].q2,
////								  				  sensorDatas[2].q3,
////												  3,
////												  	    		  sensorDatas[3].pressure,
////												  				  sensorDatas[3].distance,
////												  				  sensorDatas[3].q0,
////												  				  sensorDatas[3].q1,
////												  				  sensorDatas[3].q2,
////												  				  sensorDatas[3].q3,
////
////																  4,
////																  	    		  sensorDatas[4].pressure,
////																  				  sensorDatas[4].distance,
////																  				  sensorDatas[4].q0,
////																  				  sensorDatas[4].q1,
////																  				  sensorDatas[4].q2,
////																  				  sensorDatas[4].q3,
////																				  5,
////																				  	    		  sensorDatas[5].pressure,
////																				  				  sensorDatas[5].distance,
////																				  				  sensorDatas[5].q0,
////																				  				  sensorDatas[5].q1,
////																				  				  sensorDatas[5].q2,
////																				  				  sensorDatas[5].q3
////				  );
//		printf("%d %d %d %d %d %d\r\n",
//					(int)(sensorDatas[0].distance),
//					(int)(sensorDatas[1].distance),
//							(int)(sensorDatas[2].distance),
//									(int)(sensorDatas[3].distance),
//											(int)(sensorDatas[4].distance),
//													(int)(sensorDatas[5].distance)
//						  );
//
//
////	HAL_I2C_Mem_Read(&hi2c2, (uint8_t)(0x03<<1), MemAddress, MemAddSize, (uint8_t *)(&sensorData), 12, 10)
////	pneudrive.pSource.readPressure();
////	pneudrive.pSink.readPressure();
////
////	/*Update sensor data of chamber 0*/
////	ppresure=pneudrive.readPressure(0);
////
////	laserVol=AnalogRead(8);
////	laserPos=laserVol/5.0*0.07+0.065-laserPosOffset;
////
////
////	/*customized switch flag*/
////	if (loop_gogogo) {
////
////		/*maintain the upper stream and lower stream pressure*/
////		pneudrive.pSource.maintainPressure(pup,pup+10000);
////
////		/*maintain a downsteam pressure if necessary*/
////		pneudrive.pSink.maintainPressure(pdown,pdown+10000);
////
////		if(aa==1){
////
////			localTime=((int32_t)(millis()-localT))/1000.0f;
////			localCount++;
////			if(localCount==prePeriod/2+1)
////			{
////				preCommand=preCommandup;
////				//pneudrive.chambers[0]->writeOpening(1);
////			}
////			if(localCount>prePeriod)
////			{
////				preCommand=preCommanddown;
////				localCount=0;
////				cycleNumbers++;
////				//pneudrive.chambers[0]->writeOpening(-1);
////
////			}
////
////			pneudrive.writePressure(0,preCommand);
////
////		}
////		else
////		{
////
////			pneudrive.writePressure(0,preCommand);
////		}
////
////
////
////
////	}
//
//}
//
///*Running at a default frequency of 20KHz, altered by changing DISPLAY_FREQUENCY in the file UserConfig.h*/
//void serialDisplay() {
////	printf("%d %d %d %d %d %d %d\r\n",
////			millis(),
////			cycleNumbers,
////			(int)(pneudrive.pSource.pressure/1000),
////			(int)(pneudrive.pSink.pressure/1000),
////			(int)(pneudrive.pressureCommands[0]/1000),
////			(int)(pneudrive.readPressure(0)/1000),
////			(int)(laserPos*100000));
//
////    printf("%d %d %d %d %d %d\r\n",
////    		  sensorData.pressure/10,
////			  sensorData.distance,
////			  sensorData.q0,
////			  sensorData.q1,
////			  sensorData.q2,
////			  sensorData.q3
////			  );
//
//}
//
//
//
//typedef struct BUILTIN_SERIAL_COMMAND_Ttag {
//  int iCode;
//  char szString[50];
//}SERIAL_COMMAND;
//enum  {
//  CommandSetUNKNOWN = 0x00,
//  CommandSetVOLTAGE_v,
//  CommandSetPRESSURE_p,
//  CommandSetPRESSURE_pu,
//  CommandSetPRESSURE_pd,
//  CommandSetKP_kp,
//  CommandSetKI_ki,
//  CommandSetKD_kd,
//  CommandSetILim_ilim,
//  CommandSetULim_ulim,
//  CommandSetFlowKP_kf,
//  CommandSetSTIFFNESS_k,
//  CommandSetQUIT_s,
//  CommandSetSTART_g,
//  CommandSetZERO_z,
//  CommandSetFLOW_f,
//  CommandSetPOSITION_aa,
//  CommandSetPOSITION_ap,
//  CommandSetFEEDBACK_t,
//  CommandSetValveLimit_u,
//  CommandSetCONTROLFREQUENCY_loopMS,
//  CommandSetSENDFREQUENCY_sendMS,
//  CommandSetLAST     /** just for determining number of commands in this list */
//};
///*Callback function when serial port 3 receives commands with ending of "\r\n", the parameter is the pointer to the received string buffer*/
//void serialReceiveCallback(char *pSerialCommandBuffer) {
//	static SERIAL_COMMAND mySerialCommandList[] ={
//	  {CommandSetVOLTAGE_v,         		"v"},
//	  {CommandSetPRESSURE_p,        		"p"},
//	  {CommandSetPRESSURE_pu,"pu"},
//	  {CommandSetPRESSURE_pd,"pd"},
//	  {CommandSetKP_kp,       				"kp"},
//	  {CommandSetKI_ki, 			 		"ki"},
//	  {CommandSetKD_kd,				 		"kd"},
//	  {CommandSetILim_ilim,         		"ilim"},
//	  {CommandSetULim_ulim,         		"ulim"},
//	  {CommandSetFlowKP_kf,			 		"kf"},
//	  {CommandSetSTIFFNESS_k,        		"k"},
//	  {CommandSetQUIT_s,         			"s"},
//	  {CommandSetSTART_g,        			"g"},
//	  {CommandSetZERO_z,        	 		"z"},
//	  {CommandSetFLOW_f,         			"f"},
//	  {CommandSetPOSITION_aa,         		"aa"},
//	  {CommandSetPOSITION_ap,         		"ap"},
//	  {CommandSetFEEDBACK_t,         		"t"},
//	  {CommandSetValveLimit_u,				"u"},
//	  {CommandSetCONTROLFREQUENCY_loopMS,   "loopMS"},
//	  {CommandSetSENDFREQUENCY_sendMS,   "sendMS"}};
//	static char commandChar[50];
//	static int chambernum=0;
//	static float pCommand = 0;
//	static float ulims[4];
//	int recivedCommandStringCode=CommandSetUNKNOWN;
//	//Read the received buffer with customized message. The first char means command type. The second number represents the interested chamber number. The third float number is the pressure command.
//	sscanf((char *)pSerialCommandBuffer, "%s", commandChar);
//	for (int i = 0; i < (CommandSetLAST - 1); i++)
//		if (0== strcmp((const char *) (commandChar),(const char *) (&mySerialCommandList[i].szString[0])))
//			recivedCommandStringCode=mySerialCommandList[i].iCode;
//	switch(recivedCommandStringCode){
//	case CommandSetVOLTAGE_v:
//
//	  break;
//	case CommandSetPRESSURE_p:
//		//Only change the value of command, leaving the control part to the loop().
//		sscanf(pSerialCommandBuffer, "%s %d %f", commandChar, &chambernum, &preCommand);
//		aa=0;
//	  break;
//	case CommandSetPRESSURE_pu:
//			//Only change the value of command, leaving the control part to the loop().
//			sscanf(pSerialCommandBuffer, "%s %f", commandChar,&pup);
//		  break;
//	case CommandSetPRESSURE_pd:
//			//Only change the value of command, leaving the control part to the loop().
//		  sscanf(pSerialCommandBuffer, "%s %f", commandChar,&pdown);
//		  break;
//	case CommandSetKP_kp:
//		sscanf(pSerialCommandBuffer, "%s %f", commandChar,&myKp);
//	  break;
//
//	case CommandSetKI_ki:
//		sscanf(pSerialCommandBuffer, "%s %f", commandChar,&myKi);
//	  break;
//
//	case CommandSetKD_kd:
//		sscanf(pSerialCommandBuffer, "%s %f", commandChar,&dd);
//	  break;
//
//	case CommandSetFlowKP_kf:
//
//		break;
//
//	case CommandSetSTIFFNESS_k:
//
//	  break;
//
//	case   CommandSetQUIT_s:
//		loop_gogogo = 0;
//		pneudrive.chambers[0]->writeOpening(0);
//		pneudrive.pSource.stop();
//		pneudrive.pSink.stop();
//	  break;
//
//	case CommandSetSTART_g:
//		loop_gogogo = 1;
//		  break;
//
//	case CommandSetZERO_z:
//		laserPosOffset=laserVol/4096.0f*360;
//		  break;
//	case CommandSetFLOW_f:
//		  break;
//
//	case CommandSetPOSITION_aa:
//		float uiArgv[3];
//		int iArgc;
//		iArgc=sscanf((char *)pSerialCommandBuffer, "%s %f %f %f", commandChar, &(uiArgv[0]),  &(uiArgv[1]),  &(uiArgv[2]));
//		if(iArgc==4){
//			preAmp=uiArgv[0];
//			preFre=uiArgv[1];
//			preOffset=uiArgv[2];
//		}
//		preCommandup=preAmp+preOffset;
//		preCommanddown=preOffset-preAmp;
//		prePeriod=(round)(1000/preFre);
//
//		localT=millis();
//		aa=1;
//		break;
//	case CommandSetPOSITION_ap:
//		aa=0;
//		break;
//
//	case CommandSetILim_ilim:
//			 break;
//
//	case CommandSetULim_ulim:
//
//			 break;
//	case CommandSetValveLimit_u:
//		int ulimnum;
//		float ulims;
//		sscanf(pSerialCommandBuffer, "%s %d %f", commandChar, &ulimnum,	&ulims);
//		if (ulimnum == 0)
//			pneudrive.chambers[0]->openingMinN = ulims;
//		else if (ulimnum == 1)
//			pneudrive.chambers[0]->openingMaxN = ulims;
//		else if (ulimnum == 2)
//			pneudrive.chambers[0]->openingMinP = ulims;
//		else if (ulimnum == 3)
//			pneudrive.chambers[0]->openingMaxP = ulims;
//		break;
//	case CommandSetCONTROLFREQUENCY_loopMS:
//		int32_t loopMS;
//		sscanf(pSerialCommandBuffer, "%s %lu", commandChar, &loopMS);
//		setPeriodControlLoop(loopMS);
//	  break;
//	case CommandSetSENDFREQUENCY_sendMS:
//		int32_t sendMS;
//		sscanf(pSerialCommandBuffer, "%s %lu", commandChar, &sendMS);
//		setPeriodControlLoop(sendMS);
//	  break;
//
//	case CommandSetUNKNOWN:
//	  break;
//
//	default:
//	  break;
//	}
//}
