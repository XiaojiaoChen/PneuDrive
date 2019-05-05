///*For more examples, please go to the ./example folder*/
////step0: configure your hardware component in UserConfig.h
//
//#include "UserConfig.h"
//#include <PneuDrive.h>
//
//#include "PWMBoardSPI.h"
//
//PNEUDRIVE pneudrive(1);
//
////parameter setting
//uint16_t loop_gogogo=0;
//float flowrate;
//float sensorVmin=0.5;		//pressure sensor's parameters
//float sensorVmax=4.5;
//float sensorPmin=-100000;//////////////////////////
//float sensorPmax=300000;/////////////////////
//int32_t c1,c2,c3,c4;
//int flagReceive=0;
//int frequencyy=6000;
//uint32_t loopMS=1;
//uint32_t loopTick=0;
//uint32_t sendTick=0;
//uint32_t sendMS=1;
//uint32_t ReceiveTime=0;
//uint32_t endTime=0;
//float manualOpening=0;
//float aaa=0.5;
////step3
//TRAJ_GEN myTrajectory;
//
//enum{
//	wp,
//	wo,
//	wos
//};
//int commandSource=wp;
//
//void setup()
//{
//	pneudrive.pSource.stop();
//	pneudrive.pSink.stop();
//
//	pneudrive.chambers[0]->attachPWM(0,1);
//	pneudrive.chambers[0]->openingMinN = -1;
//	pneudrive.chambers[0]->openingMaxN = -0.98;
//	pneudrive.chambers[0]->openingMinP = 0.99;
//	pneudrive.chambers[0]->openingMaxP = 1;
//	pneudrive.chambers[0]->pressureDeadZone=1000;
//	pneudrive.chambers[0]->writeOpening(0);
//	pneudrive.chambers[0]->attachSensor(5);
//	pneudrive.chambers[0]->setSensorRange_GaugePa(sensorVmin, sensorVmax, sensorPmin, sensorPmax);
//
//	InitTraj(&myTrajectory,1);
//	pneudrive.writeFrequency(frequencyy);
//
//
//}
//void loop(){
//
//	loopTick = loopTick + 1;
//	if (loop_gogogo) {
//		TrajGen(&(myTrajectory), HAL_GetTick());
//		pneudrive.bufferPressure(0,myTrajectory.x_out);
//		if(loopTick>=loopMS)
//		{
//			loopTick=0;
//			if(commandSource==wp)
//				pneudrive.chambers[0]->writePressure(myTrajectory.x_out);
//			else if(commandSource==wo)
//				pneudrive.chambers[0]->writeOpening(manualOpening);
//			else if(commandSource==wos)
//				pneudrive.chambers[0]->writeOpeningSequence();
//			if(flagReceive)
//				endTime=micros();
//			flowrate=getFlowrate();
//		}
//	}
//}
//void serialDisplay() {
//	sendTick = sendTick + 1;
//	if(sendTick>=sendMS)
//	{
//		sendTick=0;
////		printf("%lu	%lu %lu %1.3f %d %d %d %3.2f %d %lu\r\n",
////				micros(),
////				ReceiveTime,
////				endTime,
////				pneudrive.chambers[0]->readOpening(),
////				(int)(pneudrive.pressureCommands[0]),
////				(int)(pneudrive.readPressure(0)),
////				(int)(pneudrive.chambers[0]->pressureFil),
////				flowrate,
////				frequencyy,
////				loopMS);
//		printf("%lu %d %d %d %d %d %d\r\n",
//		millis(),
//		(int)(myTrajectory.x_out),
//		(int)(pneudrive.readPressure(0)),
//		loopMS,
////		(int)(myTrajectory.v_out),
////		(int)(pneudrive.chambers[0]->pressureController->pKalmanFilter->X.pData[1]),
////		(int)(pneudrive.chambers[0]->pressuredot),
//		(int)(pneudrive.chambers[0]->readOpening()*10000),
//		(int)(flowrate*10000)
//		);
//	}
//	if(flagReceive)
//	{
//		ReceiveTime=0;
//		endTime=0;
//		flagReceive=0;
//	}
///*	printf("%1.3f %1.3f %1.3f %1.3f %1.3f %1.3f %1.3f %1.3f %1.3f %1.3f %1.3f %1.3f %1.3f %1.3f %1.3f %1.3f\r\n",
//			vol[0],
//			vol[1],
//			vol[2],
//			vol[3],
//			vol[4],
//			vol[5],
//			vol[6],
//			vol[7],
//			vol[8],
//			vol[9],
//			vol[10],
//			vol[11],
//			vol[12],
//			vol[13],
//			vol[14],
//			vol[15]);*/
//
//}
//
//
//typedef struct BUILTIN_SERIAL_COMMAND_Ttag {
//  int iCode;
//  char szString[50];
//  char szHelp[50];
//}SERIAL_COMMAND;
//enum {
//  CommandSetUNKNOWN = 0x00,
//  CommandSetVOLTAGE_v,
//  CommandSetPRESSURE_p,
//  CommandSetKP_kp,
//  CommandSetKI_ki,
//  CommandSetKD_kd,
//  CommandSetILim_ilim,
//  CommandSetULim_ulim,
//  CommandSetKalmanQ_kalmanQ,
//  CommandSetKalmanR_kalmanR,
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
//  CommandSetDeadZone_deadzone,
//  CommandSetCONTROLFREQUENCY_loopMS,
//  CommandSetSendFREQUENCY_sendMS,
//  CommandSetValveFrequency_vf,
//  CommandSetValveOpening_vo,
//  CommandSetControllerSMCk,
//  CommandSetOpeningSequency,
//  CommandSetFilterBeta,
//  CommandSetLamda,
//  CommandSetDeadZone,
//  CommandTAIL     /** just for determining number of commands in this list */
//};
//
//void serialReceiveCallback(char *pSerialCommandBuffer) {
//	static SERIAL_COMMAND mySerialCommandList[] ={
//	  {CommandSetVOLTAGE_v,         		"v",    		"Change Voltage 0"},
//	  {CommandSetPRESSURE_p,        		"p",    		"write var val"},
//	  {CommandSetKP_kp,       				"kp",			"tune PID Kp"},
//	  {CommandSetKI_ki, 			 		"ki",   		"tune PID Ki"},
//	  {CommandSetKD_kd,				 		"kd",   		"tune PID Kd"},
//	  {CommandSetILim_ilim,         		"ilim",    		"tune PID ilim"},
//	  {CommandSetULim_ulim,         		"ulim",    		"tune PID ulim"},
//	  {CommandSetKalmanQ_kalmanQ,					"kalmanQ",    	"tune kalman filter Q"},
//	  {CommandSetKalmanR_kalmanR,					"kalmanR",    	"tune kalman filter R"},
//	  {CommandSetFlowKP_kf,			 		"kf",   		"tune flow PID Kp"},
//	  {CommandSetSTIFFNESS_k,        		"k",    		"tune stiffness"},
//	  {CommandSetQUIT_s,         			"s",    		"Stop and close"},
//	  {CommandSetSTART_g,        			"g",    		"Start loop control"},
//	  {CommandSetZERO_z,        	 		"z",    		"zero sensor offset"},
//	  {CommandSetFLOW_f,         			"f",    		"change frequency"},
//	  {CommandSetPOSITION_aa,         		"aa",    		"Position command"},
//	  {CommandSetPOSITION_ap,         		"ap",    		"Position command"},
//	  {CommandSetFEEDBACK_t,         		"t",    		"Enable position feedback"},
//	  {CommandSetDeadZone_deadzone,         		"deadzone",    		"Set the deadzone of pressure control"},
//	  {CommandSetValveLimit_u,				"u",			"Set the ulim of the onoff valve"},
//	  {CommandSetCONTROLFREQUENCY_loopMS,      "loopMS",    "change the control frequency"},
//	  {CommandSetSendFREQUENCY_sendMS,      "sendMS",    "change the control frequency"},
//	  {CommandSetValveFrequency_vf,	"vf",	"change the frequency of valve"},
//	  {CommandSetControllerSMCk,	"SMCk",	" SetControllerSMCk"},
//	  {CommandSetOpeningSequency,	"os",	" SetOpeningSequency"},
//	  {CommandSetFilterBeta,	"fb",	" SetFilterBetaForPressureDotApproximation"},
//	  {CommandSetLamda, "lambda", "123"},
//	  {CommandSetValveOpening_vo,	"vo",	"change the opening of valve"},
//	  {CommandSetDeadZone, "pdead", "deadzone"}};
//
//	char commandChar[100];
//	int chambernum;
//	float pCommand;
//	int commandCode=CommandTAIL;
//	sscanf((char *)pSerialCommandBuffer, "%s", commandChar);
//	for (int i = 0; i < (CommandTAIL - 1); i++) {
//		if (0== strcmp((const char *) (commandChar),(const char *) (mySerialCommandList[i].szString))) {
//			commandCode=mySerialCommandList[i].iCode;
//			break;
//		}
//	}
//	switch(commandCode){
//	case CommandSetVOLTAGE_v:
//
//	  break;
//	case CommandSetPRESSURE_p:
//
//		//Only change the value of command, leaving the control part to the loop().
//		sscanf(pSerialCommandBuffer, "%s %d %f", commandChar, &chambernum, &pCommand);
//		flagReceive=1;
//		ReceiveTime=micros();
//		pneudrive.bufferPressure(chambernum, pCommand);
//	  break;
//
//	case CommandSetKP_kp:
//
//	  break;
//
//	case CommandSetKI_ki:
//
//	  break;
//
//	case CommandSetKD_kd:
//
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
//		  break;
//	case CommandSetFLOW_f:
//		  break;
//
//	case CommandSetPOSITION_aa:
//		break;
//	case CommandSetPOSITION_ap:
//
//			float uiArgv[3];
//			int iArgc;
//			iArgc=sscanf((char *)pSerialCommandBuffer, "%s %f %f %f", commandChar, &(uiArgv[0]),  &(uiArgv[1]),  &(uiArgv[2]));
//			UpdateTrajSin(&myTrajectory,pneudrive.chambers[0]->pressure,uiArgv[0],uiArgv[1],uiArgv[2],HAL_GetTick());
//			flagReceive=1;
//			ReceiveTime=micros();
//			commandSource=wp;
//		break;
//
//	case CommandSetILim_ilim:
//			 break;
//
//	case CommandSetULim_ulim:
//
//			 break;
//	case CommandSetValveLimit_u:
//		sscanf(pSerialCommandBuffer, "%s %d %f", commandChar, &chambernum,	&pCommand);
//		if (chambernum == 0)
//			pneudrive.chambers[0]->openingMinN = pCommand;
//		else if (chambernum == 1)
//			pneudrive.chambers[0]->openingMaxN = pCommand;
//		else if (chambernum == 2)
//			pneudrive.chambers[0]->openingMinP = pCommand;
//		else if (chambernum == 3)
//			pneudrive.chambers[0]->openingMaxP = pCommand;
//		break;
//	case CommandSetCONTROLFREQUENCY_loopMS:
//		sscanf(pSerialCommandBuffer, "%s %lu", commandChar, &loopMS);
//	  break;
//	case CommandSetSendFREQUENCY_sendMS:
//			sscanf(pSerialCommandBuffer, "%s %lu", commandChar, &sendMS);
//		  break;
//	case CommandSetValveFrequency_vf:
//		sscanf(pSerialCommandBuffer, "%s %d", commandChar, &frequencyy);
//		pneudrive.writeFrequency(frequencyy);
//		break;
//	case CommandSetValveOpening_vo:
//		sscanf(pSerialCommandBuffer, "%s %f", commandChar, &manualOpening);
//		commandSource=wo;
//		break;
//	case CommandSetControllerSMCk:
//		sscanf(pSerialCommandBuffer, "%s %f", commandChar, &(pneudrive.chambers[0]->pressureController->SUPER_TWISTING_SMC.k));
//		break;
//	case CommandSetOpeningSequency:
//		sscanf(pSerialCommandBuffer, "%s %f %d", commandChar, &(pneudrive.chambers[0]->openingSequence[0]),&(pneudrive.chambers[0]->endOpeningNum));
//		for(int i=0;i<pneudrive.chambers[0]->endOpeningNum;i++)
//			pneudrive.chambers[0]->openingSequence[i]=pneudrive.chambers[0]->openingSequence[0];
//		pneudrive.chambers[0]->inOpeningSequence=1;
//		commandSource=wos;
//		break;
//	case CommandSetKalmanQ_kalmanQ:
//		float qtemp;
//		sscanf(pSerialCommandBuffer, "%s %f", commandChar, &qtemp);
//		kalman_setQ(pneudrive.chambers[0]->pressureController->pKalmanFilter,&qtemp);
//		break;
//	case CommandSetKalmanR_kalmanR:
//		float rtemp;
//		sscanf(pSerialCommandBuffer, "%s %f", commandChar, &rtemp);
//		kalman_setR(pneudrive.chambers[0]->pressureController->pKalmanFilter,&rtemp);
//		break;
//	case CommandSetFilterBeta:
//		sscanf(pSerialCommandBuffer, "%s %f", commandChar, &(pneudrive.chambers[0]->filterBeta));
//		break;
//	case CommandSetLamda:
//			sscanf(pSerialCommandBuffer, "%s %f", commandChar, &(pneudrive.chambers[0]->pressureController->SUPER_TWISTING_SMC.lamdadot));
//			break;
//	case CommandSetDeadZone:
//			sscanf(pSerialCommandBuffer, "%s %f", commandChar, &(pneudrive.chambers[0]->pressureDeadZone));
//				break;
//	case CommandSetUNKNOWN:
//	  break;
//
//	default:
//	  break;
//	}
//}
//
