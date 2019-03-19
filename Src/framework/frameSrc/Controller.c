




#include "Controller.h"
#include "valveFlowFunc.h"
#include "stdlib.h"
#include "string.h"

//including direction.  -1<u<1
//u>0  inflating
//u=0  hold
//u<0  deflating
static float controlPressure(struct BASIC_PRESSURE_CONTROLLER_STRUCT *bpc,float p,float pd,float pddot)
{

	float flow_cal;
	float perror=pd-p;
	float pdoterror=pddot;

	//simple PD control of the pressure
//	bpc->flow = updatePID(bpc->pPID,pd,p);

	//bpc->voltage = updatePID(bpc->pPID,pd,p);

//	bpc->voltage = map(bpc->pPID)
	/*//inflating
	if(bpc->flow>0)
	{
		bpc->voltage= bpc->flow;
			//	flowrate2Voltage(bpc->psource,p,bpc->flow );
	}
	else//deflating
	{

		//bpc->voltage= -flowrate2Voltage(p,bpc->psink,-bpc->flow );
	}*/


	return bpc->voltage;
}


BASIC_PRESSURE_CONTROLLER *NewPressureController(float psource,float psink,float dt,float kalmanQ,float kalmanR,
		float pidKp,float pidKi,float pidKd,float pidilim,float pidulim)
{
	//reference values
	/*kalmanQ=1e12;
	kalmanR=10000;
	pidKp=1e-7;
	pidKi=0;
	pidKd=1e-10;
	pidilim=6e-5;
	pidulim=5e-4;*/

	BASIC_PRESSURE_CONTROLLER *pController = (BASIC_PRESSURE_CONTROLLER *)malloc(sizeof(BASIC_PRESSURE_CONTROLLER));
	memset(pController,0,sizeof(BASIC_PRESSURE_CONTROLLER));
	pController->psource = psource;
	pController->psink = psink;
	pController->dt = dt;
	pController->pKalmanFilter = newSimpleKalmanFilter(kalmanQ,kalmanR,dt);
	pController->pPID = newPID(pidKp,pidKi,pidKd,dt,pidilim,pidulim);
	pController->controlPressure = controlPressure;
	return pController;
}




