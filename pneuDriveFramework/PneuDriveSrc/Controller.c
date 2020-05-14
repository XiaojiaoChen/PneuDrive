




#include "Controller.h"
#include "valveFlowFunc.h"
#include "stdlib.h"
#include "string.h"
static float mySAT(float u)
{
	return u/(fabs(u)+0.01);
}
static float superTwisting2rdSMCUpdate(SUPER_TWISTING_SMC *pSMC, float err , float errdot)
{
	pSMC->err=err;
	pSMC->errdot = errdot;
	pSMC->s = (pSMC->lamdadot * errdot + pSMC->lamda*err)/1000;

	float sgnS=mySAT(pSMC->s);
	float absSK=fabs(pSMC->s)*pSMC->k;

	float sqrSK=0;

	arm_sqrt_f32(absSK,&sqrSK);

	//pSMC->ystate = pSMC->ystate + pSMC->dt*1.1f*pSMC->k*sgnS;

	pSMC->y= sqrSK*sgnS;// + pSMC->ystate;

	return pSMC->y;
}


//including direction.  -1<u<1
//u>0  inflating
//u=0  hold
//u<0  deflating
static float controlPressure(struct BASIC_PRESSURE_CONTROLLER_STRUCT *bpc,float e,float edot)
{

	superTwisting2rdSMCUpdate(&(bpc->SUPER_TWISTING_SMC),e,edot);

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


	return bpc->SUPER_TWISTING_SMC.y;
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
	pController->SUPER_TWISTING_SMC.dt=dt;
	pController->SUPER_TWISTING_SMC.k=2.5e-6;
	pController->SUPER_TWISTING_SMC.lamdadot=1;
	pController->SUPER_TWISTING_SMC.lamda=20;
	pController->controlPressure = controlPressure;
	return pController;
}






