/*
 * SoftHand.cpp
 *
 *  Created on: Jun 28, 2018
 *      Author: Xiaojiao Chen
 *		 Email: chen2014@connect.hku.hk
 *	Laboratory: Bio-Control Lab
 * Orgnization: the University of Hong Kong
 */
#include <SoftHand.h>
#include "joyStick.h"


/*************************SOFT hand**************************
 *
 ***********************************************************/
SOFT_HAND::SOFT_HAND(int num)
{
	CHAMBER **ppChamber = (CHAMBER **)(&fingers);
	CHAMBER *pChamber=chambers;
	pressureDeadZone = 3000;
    for(int i=0;i<CHAMBERNUM;i++){
    	*ppChamber++ = pChamber++;
    	InitTraj(&(positionTrajectory[i]),1);
    	angleCommand[i]=0;
    	pressureCommandDot[i]=0;
    	pressureCommandMin[i]=-10000;
    	pressureCommandMax[i]=180000;
    	positionCommandMin[i]=0;
    	positionCommandMax[i]=120;
    	chambers[i].attach(i*2+BUILTIN_PWM_NUM+PWMBOARDSPI_CHANNELNUM,i*2+1+BUILTIN_PWM_NUM+PWMBOARDSPI_CHANNELNUM,31-i+BUILTIN_ANA_IN_NUM);
    	chambers[i].pressureDeadZone = pressureDeadZone;
    }
    sinkChamber.attach(52+BUILTIN_PWM_NUM+PWMBOARDSPI_CHANNELNUM,53+BUILTIN_PWM_NUM+PWMBOARDSPI_CHANNELNUM,5+BUILTIN_ANA_IN_NUM);
    float abPressureMax=180000;
    pressureCommandMax[3]=abPressureMax;
    pressureCommandMax[4]=abPressureMax;
    pressureCommandMax[8]=abPressureMax;
    pressureCommandMax[9]=abPressureMax;
    pressureCommandMax[13]=abPressureMax;
    pressureCommandMax[14]=abPressureMax;
    pressureCommandMax[18]=abPressureMax;
    pressureCommandMax[19]=abPressureMax;
    flowrate=0;
    frequencyDirty=0;
	startControl=0;
	controlTime=0;
	builtInControlFlag=0;
	valveOpen=0;
	wp=0;
	startDemo=0;
	psink_upperlimit=-20000;
	psink_lowerlimit=psink_upperlimit-10000;

    interestedChamber=1;
    memset(last50msBuffer,0,sizeof(last50msBuffer));
    pLastBuffer = last50msBuffer;
    frequency = 40;
    for(int i=0;i<5;i++)
    	ABADCommand[i]=0;
}



void SOFT_HAND::setup()
{
	for(int i=0;i<CHAMBERNUM;i++){
		if(chambers[i].pressureSensor.AnalogPort>=BUILTIN_ANA_IN_NUM && chambers[i].pressureSensor.AnalogPort<BUILTIN_ANA_IN_NUM+16)
			chambers[i].setSensorRange_GaugePa(0.5,4.5,0,200000);
		chambers[i].writeOpening(0);
	}
	pumpInflate.stop();
	pumpDeflate.stop();
}

void SOFT_HAND::loop()
{
	static uint32_t curControlTime=0;
	curControlTime+=1;
	readPressure();
	/*flowrate=getFlowrate();*/
	if(valveOpen == 1)
	{
		for(int i=0;i<CHAMBERNUM;i++){
			chambers[i].valves[0].writeDuty(0);
			chambers[i].valves[1].writeDuty(0.98);
		}
/*		pumpInflate.start();
		pumpDeflate.start();*/
	}
	else if(valveOpen == 2)
	{
		for(int i=0;i<CHAMBERNUM;i++){
			chambers[i].valves[0].writeDuty(0);
			chambers[i].valves[1].writeDuty(0);
		}
		valveOpen++;
/*		pumpInflate.stop();
		pumpDeflate.stop();*/
	}
	if(frequencyDirty==1)
	{
		writeFrequency(frequency);
		frequencyDirty=0;
	}
	if(startControl)
	{
		/*sinkChamber.readPressure();
		if(sinkChamber.pressure>psink_upperlimit)
			pumpDeflate.start();
		else if(sinkChamber.pressure<psink_lowerlimit)
			pumpDeflate.stop();*/
		if(builtInControlFlag==2)
		{
			wp=1;
			for(int i=0;i<CHAMBERNUM;i++){
				TrajGen(&(positionTrajectory[i]),HAL_GetTick());
				pressureCommand[i]= positionTrajectory[i].x_out;
				if(i==2 || i==7 || i==12 || i==17)
					pressureCommand[i]= 100000;
			}
			float angleABAD=positionTrajectory[10].x_out-positionTrajectory[10].sinoid.offset;
			if(angleABAD>=0){
				pressureCommand[3] = angleABAD;
				pressureCommand[4]=0;
				pressureCommand[8] = angleABAD;
				pressureCommand[9]=0;
				pressureCommand[13] = angleABAD;
				pressureCommand[14]=0;
				pressureCommand[18] = angleABAD;
				pressureCommand[19]=0;
			}
			else{
				pressureCommand[3] = 0;
				pressureCommand[4]=-angleABAD;
				pressureCommand[8] = 0;
				pressureCommand[9]=-angleABAD;
				pressureCommand[13] = 0;
				pressureCommand[14]=-angleABAD;
				pressureCommand[18] = 0;
				pressureCommand[19]=-angleABAD;
			}
		}
		else if(builtInControlFlag==1)
		{
			TrajGen(&(positionTrajectory[interestedChamber]),HAL_GetTick());
			pressureCommand[interestedChamber]= positionTrajectory[interestedChamber].x_out;
			pressureCommandDot[interestedChamber]= positionTrajectory[interestedChamber].v_out;
			wp=1;
		}
		else if(builtInControlFlag==3)
		{
			chambers[interestedChamber].writeOpening(chambers[interestedChamber].opening);
			wp=-1   ;
		}
		else if(builtInControlFlag==4)
		{
			printf("%4.2f %f %.6f %3.3f\r\n", HAL_GetTick()/1000.0f,frequency,chambers[0].opening,flowrate);
		}

		if(wp==1){
			writePressure();
			/*pressureCommand[interestedChamber]=constrain(pressureCommand[interestedChamber],pressureCommandMin[interestedChamber],pressureCommandMax[interestedChamber]);
			chambers[interestedChamber].writePressure(pressureCommand[interestedChamber],pressureCommandDot[interestedChamber]);
*/		}
		else if(wp==0){
			writePosition();
		}


	}
	//storeLast500ms(controlCLK);
}
int16_t pCommandToSend[26];
int16_t pToSend[26];
void SOFT_HAND::display()
{
		for(int i=0;i<26;i++)
		{
			pCommandToSend[i]=pressureCommand[i]/1000;
			pToSend[i]=chambers[i].pressure/1000;
		}

      printf("%4.2f %4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d	%4d\r\n",
		  HAL_GetTick()/1000.0f,
		pCommandToSend[0],pToSend[0],
		pCommandToSend[1],pToSend[1],
		pCommandToSend[2],pToSend[2],
		pCommandToSend[3],pToSend[3],
		pCommandToSend[4],pToSend[4],
		pCommandToSend[5],pToSend[5],
		pCommandToSend[6],pToSend[6],
		pCommandToSend[7],pToSend[7],
		pCommandToSend[8],pToSend[8],
		pCommandToSend[9],pToSend[9],
		pCommandToSend[10],pToSend[10],
		pCommandToSend[11],pToSend[11],
		pCommandToSend[12],pToSend[12],
		pCommandToSend[13],pToSend[13],
		pCommandToSend[14],pToSend[14],
		pCommandToSend[15],pToSend[15],
		pCommandToSend[16],pToSend[16],
		pCommandToSend[17],pToSend[17],
		pCommandToSend[18],pToSend[18],
		pCommandToSend[19],pToSend[19],
		pCommandToSend[20],pToSend[20],
		pCommandToSend[21],pToSend[21],
		pCommandToSend[22],pToSend[22],
		pCommandToSend[23],pToSend[23],
		pCommandToSend[24],pToSend[24],
		pCommandToSend[25],pToSend[25]);
}

void SOFT_HAND::receiveCommand(char *pSerialReceiveBuffer)
{

		char str[200];
		uint32_t b;
		float c;


		if(pSerialReceiveBuffer[0] == 'g')
		{
			startControl=1;
			startDemo = 1;
			builtInControlFlag=0;
		}
		else if(pSerialReceiveBuffer[0] == 's')
		{
			pumpDeflate.stop();
			pumpInflate.stop();
			startControl=0;
			startDemo = 0;
			valveOpen=0;
			builtInControlFlag=0;
			for(int i=0;i<CHAMBERNUM;i++){
				positionCommand[i] = 0;
				pressureCommand[i] = 0;
			}

		}
		else if(pSerialReceiveBuffer[0]=='f')
		{
			float frenew=0;
			sscanf(pSerialReceiveBuffer, "%s %f", &str[0],  &frenew);
			if(fabs(frenew-frequency)>1e-5)
			{
				frequency=frenew;
				frequencyDirty=1;
			}
		}
		else if(pSerialReceiveBuffer[0] == 'P')
		{
			float pressureTem[CHAMBERNUM];
			float *p=pressureTem;
			sscanf(pSerialReceiveBuffer, "%s %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",str,
											p	, p+1, p+2, p+3, p+4, p+5 ,p+6, p+7, p+8, p+9,
											p+10,p+11,p+12,p+13,p+14,p+15,p+16,p+17,p+18,p+19,
											p+20,p+21,p+22,p+23,p+24,p+25
											);
			for(int i=0;i<CHAMBERNUM;i++)
			{
				pressureCommand[i] = pressureTem[i]*1000.0f;
				positionCommand[i] = chambers[i].pressure2position(pressureCommand[i]);
			}
			wp=1;
		}
		else if(pSerialReceiveBuffer[0] == 'p')
		{
			if (pSerialReceiveBuffer[1]==' '){
					float pressureDes=0;
					sscanf(&pSerialReceiveBuffer[2], "%f %d",&pressureDes,&interestedChamber);
					pressureCommand[interestedChamber]=pressureDes*1000;
					wp=1;
				}
			if (pSerialReceiveBuffer[1]=='s'){
				float pressureDes=0;
				sscanf(&pSerialReceiveBuffer[6], "%f",&pressureDes);
				psink_upperlimit=pressureDes*1000;
				psink_lowerlimit=psink_upperlimit-10000;
			}
		}
		else if(pSerialReceiveBuffer[0] == 'A')
		{
			if(pSerialReceiveBuffer[1] == ' ')
			{
				float tempPositionCommand[26];
			float *p=tempPositionCommand;
					sscanf(pSerialReceiveBuffer, "%s %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",str,
													p	, p+1, p+2, p+3, p+4, p+5 ,p+6, p+7, p+8, p+9,
													p+10,p+11,p+12,p+13,p+14,p+15,p+16,p+17,p+18,p+19,
													p+20,p+21,p+22,p+23,p+24,p+25
													);
					for(int i=0;i<CHAMBERNUM;i++)
					{

						positionCommand[i]=tempPositionCommand[i];
						pressureCommand[i] = (chambers[i].position2pressure(positionCommand[i]));
					}
			}
			else if(pSerialReceiveBuffer[3] == 'A')
			{
				RECEIVEBINSTRUCT *p=(RECEIVEBINSTRUCT *)pSerialReceiveBuffer;
				if (p->tail[0]=='\r')
					for(int i=0;i<26;i++){
						positionCommand[i]=p->data[i]/10.0f;
					}
			}
			wp=0;
		}
		else if(pSerialReceiveBuffer[0] == 'z')
		{
			if(pSerialReceiveBuffer[1] == '\r')
				for(int i=0;i<CHAMBERNUM;i++)
					chambers[i].pressureSensor.vMin = AnalogRead(chambers[i].pressureSensor.AnalogPort);
		}
		else if(pSerialReceiveBuffer[0] == 'o')
		{
			valveOpen++;
		}
		else if(pSerialReceiveBuffer[0] == 'O')
		{
			int num=2;
			int i=0;
			int j=0;
			int ticks=0;
			int cycles=0;
			int commandNum=0;
			float openingcommand=0;
			commandNum = sscanf(&pSerialReceiveBuffer[2], "%d %f %d %d",&interestedChamber,&openingcommand,&ticks,&cycles);
			builtInControlFlag = 3;
			chambers[interestedChamber].opening = openingcommand;
			if(commandNum==4){
				memset(chambers[num].openingSequence,0,sizeof(chambers[num].openingSequence));

				for(j=0;j<cycles;j++)
					for(i=j*ticks;i<(j+1)*ticks;i++)
							chambers[num].openingSequence[i]=openingcommand;
				chambers[num].curOpeningNum=0;
				chambers[num].endOpeningNum=i;
				chambers[num].inOpeningSequence =1;
			}
		}
		else if(pSerialReceiveBuffer[0] == 'u')
		{
			if(pSerialReceiveBuffer[1] == '1')
			{
				float deflateMinN;
				int num;
				sscanf(&pSerialReceiveBuffer[3], "%f %d",&deflateMinN,&num);
				if(num<0)
				 for(int i=0;i<CHAMBERNUM;i++){
					chambers[i].openingMinN = deflateMinN;
					}
				else{
					chambers[num].openingMinN = deflateMinN;
				}
			}
			else if(pSerialReceiveBuffer[1] == '2')
			{
				float deflateMaxN;
				int num;
				sscanf(&pSerialReceiveBuffer[3], "%f %d",&deflateMaxN,&num);
				if(num<0)
				 for(int i=0;i<CHAMBERNUM;i++){
					chambers[i].openingMaxN = deflateMaxN;
					}
				else{
					chambers[num].openingMaxN = deflateMaxN;
				}
			}
		}
		else if(pSerialReceiveBuffer[0] == 'U')
			{
			if(pSerialReceiveBuffer[1] == '1')
					{
						float inflateMinP;
						int num;
						sscanf(&pSerialReceiveBuffer[3], "%f %d",&inflateMinP,&num);
						if(num<0)
						 for(int i=0;i<CHAMBERNUM;i++){
							chambers[i].openingMinP = inflateMinP;
							}
						else{
							chambers[num].openingMinP = inflateMinP;
						}

					}
					else if(pSerialReceiveBuffer[1] == '2')
					{
						float inflateMaxP;
						int num;
						sscanf(&pSerialReceiveBuffer[3], "%f %d",&inflateMaxP,&num);
						if(num<0)
							 for(int i=0;i<CHAMBERNUM;i++){
							chambers[i].openingMaxP = inflateMaxP;
							}
						else{
							chambers[num].openingMaxP = inflateMaxP;
						}
					}
			}
		else if(pSerialReceiveBuffer[0] == 'k')
			{
				if(pSerialReceiveBuffer[1] == 'p'){
				float kp[CHAMBERNUM];
				float *p=kp;
				sscanf(pSerialReceiveBuffer, "%s %f",str,p);

				 for(int i=0;i<CHAMBERNUM;i++){
					setKp(chambers[i].pressureController->pPID, kp[0]);
				    }
				}

				else if(pSerialReceiveBuffer[1] == 'i'){
				float ki[CHAMBERNUM];
				float *p=ki;
				sscanf(pSerialReceiveBuffer, "%s %f",str,p);

				 for(int i=0;i<CHAMBERNUM;i++){
					setKi(chambers[i].pressureController->pPID, ki[0]);
				    }
				}

				else if(pSerialReceiveBuffer[1] == 'd'){
				float kd[CHAMBERNUM];
				float *p=kd;
				sscanf(pSerialReceiveBuffer, "%s %f",str,p);

				 for(int i=0;i<CHAMBERNUM;i++){
					setKd(chambers[i].pressureController->pPID, kd[0]);
				    }
				}

			}
		else if(pSerialReceiveBuffer[0] == 'd')
		{
			float deadzone;
				sscanf(pSerialReceiveBuffer, "%s %f",str,&deadzone);
			for(int i=0;i<CHAMBERNUM;i++){
				chambers[i].pressureDeadZone = deadzone*1000;
			}

		}
		else if(pSerialReceiveBuffer[0] == 'Q')
		{
			float kalmanq;
				sscanf(pSerialReceiveBuffer, "%s %f",str,&kalmanq);
			for(int i=0;i<CHAMBERNUM;i++){
				kalman_setQ(chambers[i].pressureController->pKalmanFilter,&kalmanq);
			}

		}
		else if(pSerialReceiveBuffer[0] == 'R')
		{
			float kalmanR;
				sscanf(pSerialReceiveBuffer, "%s %f",str,&kalmanR);
			for(int i=0;i<CHAMBERNUM;i++){
				kalman_setR(chambers[i].pressureController->pKalmanFilter,&kalmanR);
			}

		}
		else if(pSerialReceiveBuffer[0] == 'a')
		{
			if (pSerialReceiveBuffer[1]==' '){
				float angleDes=0;
				sscanf(&pSerialReceiveBuffer[2], "%f %d",&angleDes,&interestedChamber);
				positionCommand[interestedChamber]=angleDes;
			}
			else {
				if(pSerialReceiveBuffer[1] == 'a'){
					float posDes=0;
					int num=0;
					sscanf(&pSerialReceiveBuffer[3], "%f %d %d",&posDes,&interestedChamber,&num);
					chambers[interestedChamber].positionTable[num]=posDes;
				}
				else{
					if(pSerialReceiveBuffer[1] == 'p'){
					float preDes=0;
					int num=0;
					sscanf(&pSerialReceiveBuffer[3], "%f %d %d",&preDes,&interestedChamber,&num);
					chambers[interestedChamber].pressureTable[num]=preDes;
					}
				}
			}
			wp=0;
		}
		else if(pSerialReceiveBuffer[0] == 'c')
		{
			sscanf(&pSerialReceiveBuffer[2], "%d",&(interestedChamber));
		}
		else if(pSerialReceiveBuffer[0] == 'm')
		{
			if(pSerialReceiveBuffer[1] == '1')
				builtInControlFlag=1;
			else if(pSerialReceiveBuffer[1]=='A')
				builtInControlFlag=2;
			float angleDes;
			float freDes;
			float offDes;
			sscanf(&pSerialReceiveBuffer[3], "%f %f %d",&angleDes,&freDes,&interestedChamber);
			for(int i=0;i<26;i++){
				if((((i==3||i==4)||(i==8||i==9))||((i==13||i==14)||(i==18||i==19)))||i==24)
					continue;
				UpdateTrajSin(&(positionTrajectory[i]),
				positionTrajectory[i].x_out,
				(angleDes),
				(freDes),
				(angleDes),
				HAL_GetTick());
			}
			wp=0;
		}
		else if(pSerialReceiveBuffer[0] == 'v')
		{
			float dd;
			sscanf(&pSerialReceiveBuffer[2], "%f",&dd);
			for(int i=0;i<CHAMBERNUM;i++)
			{
				chambers[i].inflateVelocity=dd;
			}
		}
		else if(pSerialReceiveBuffer[0] == 'W')
		{
			builtInControlFlag=4;
			wp=0;
		}
		else if(pSerialReceiveBuffer[0] == 'J')
			{
				builtInControlFlag=0;
				uint32_t dd=0;
				int midValue=8;
				sscanf(&pSerialReceiveBuffer[2], "%lu",&dd);
				int8_t x=(int8_t)(((dd>>8)>>18)&0x3F)-1;
				int8_t y=(int8_t)(((dd>>8)>>12)&0x3F)-midValue;
				int8_t rx=(int8_t)(((dd>>8)>>6)&0x3F)-1;
				int8_t ry=(int8_t)(((dd>>8))&0x3F)-midValue;
				uint8_t btn_tr=(uint8_t)((dd&0x01));
				uint8_t btn_tl=(uint8_t)((dd>>1)&0x01);
				uint8_t btn_south=(uint8_t)((dd>>2)&0x01);
				uint8_t btn_east=(uint8_t)((dd>>3)&0x01);
				uint8_t btn_north=(uint8_t)((dd>>4)&0x01);
				uint8_t btn_west=(uint8_t)((dd>>5)&0x01);
				uint8_t z=(uint8_t)((dd>>6)&0x01);
				uint8_t rz=(uint8_t)((dd>>7)&0x01);
				uint8_t fingerFlag[5]={0,0,0,0,0};
				int8_t jointFlag[6]={1,1,1,0,0,0};
				int baddata=0;
				if(abs(x)>1 || abs(rx)>1 || abs(y)>8 || abs(ry)>8)
					baddata=1;
				if(!baddata){
					if(z){
						fingerFlag[3]=1;
						fingerFlag[2]=1;
					}
					else if(btn_tl){
						fingerFlag[2]=1;
					}
					else{
						fingerFlag[3]=1;
					}

					if(rz){
						fingerFlag[1]=1;
						fingerFlag[0]=1;
					}
					else if(btn_tr){
						fingerFlag[1]=1;
					}
					else{
						fingerFlag[0]=1;
					}

					if((btn_south+btn_north+btn_east)!=0)
					{
						jointFlag[0]=btn_south;
						jointFlag[1]=btn_east;
						jointFlag[2]=btn_north;
					}

					for(int i=0;i<5;i++){
						if(fingerFlag[i]){
							for(int j=0;j<3;j++){
								if(jointFlag[j]){
									if(i<2)
										positionCommand[i*5+j]+=ry;
									else
										positionCommand[i*5+j]+=y;
								}
							}
							if(i<2)
								ABADCommand[i]+=rx*3;
							else
								ABADCommand[i]+=x*3;
							if (ABADCommand[i]>0){
								positionCommand[i*5+3]=0;
								positionCommand[i*5+4]=ABADCommand[i];
							}
							else
							{
								positionCommand[i*5+3]=-ABADCommand[i];
								positionCommand[i*5+4]=0;
							}
						}
					}
				}
				wp=0;
			}

}



void SOFT_HAND::attach(int num,int PMWPort1,int PWMPort2,int AnalogPort)
{
	chambers[num].attach(PMWPort1,PWMPort2,AnalogPort);
}


void SOFT_HAND::increaseFrequency(float dFre){
	frequency = frequency+dFre;
	frequency = CONSTRAIN(frequency,0.05,10000);
	for (int i = 0; i < CHAMBERNUM; i++) {
		chambers[i].valves[0].writeFrequency(frequency);
		chambers[i].valves[1].writeFrequency(frequency);
	}
}

void SOFT_HAND::writeFrequency(float fre){
	frequency = CONSTRAIN(fre,0.05,10000);
	for (int i = 0; i < CHAMBERNUM; i++) {
		chambers[i].valves[0].writeFrequency(frequency);
		chambers[i].valves[1].writeFrequency(frequency);
	}
}


void SOFT_HAND::readPressure()
{
	for (int i = 0; i < CHAMBERNUM; i++) {
		 pressure[i]=chambers[i].readPressure();
		 if(pressure[i]<-10000)
			 pressure[i]=0;
	}
}

void SOFT_HAND::writePressure()
{
	for (int i = 0; i < CHAMBERNUM; i++) {
		pressureCommand[i]=CONSTRAIN(pressureCommand[i],pressureCommandMin[i],pressureCommandMax[i]);
		chambers[i].writePressure(pressureCommand[i]);
	}
}
void SOFT_HAND::writeSequence()
{
	for (int i = 0; i < CHAMBERNUM; i++) {
		if(chambers[i].inOpeningSequence)
			chambers[i].writeOpeningSequence();
		else
			chambers[i].writeOpening(chambers[i].opening);
	}
}

void SOFT_HAND::writePosition()
{

	for (int i = 0; i < CHAMBERNUM; i++) {
		positionCommand[i]=CONSTRAIN(positionCommand[i],positionCommandMin[i],positionCommandMax[i]);
		pressureCommand[i] = chambers[i].position2pressure(positionCommand[i]);
		}
  	writePressure();
}



void SOFT_HAND::storeLast500ms(uint32_t curTime)
{

	//curT:1 ~ unlimited
	static uint32_t localCounter = 0;
	static int32_t gCnt=0;
	gCnt+=1;
	printfBinPush((int16_t)(curTime/1000));
	printfBinPush((int16_t)(curTime%1000));
    printfBinPush((int16_t)(positionCommand[0]));
	printfBinPush((int16_t)(positionCommand[1]));
	printfBinPush((int16_t)(positionCommand[2]));
	printfBinPush((int16_t)(chambers[interestedChamber].opening*1e4));
	localCounter++;
	if(localCounter==10)
	{
		localCounter=0;
		printfBinFlush();
	}


}

