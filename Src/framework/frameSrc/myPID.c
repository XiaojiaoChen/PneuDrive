/*
 * myPID.c
 *
 *  Created on: Oct 4, 2017
 *      Author: yii
 */

#include "myPID.h"
#include "string.h"
#include "stdlib.h"

#define  Constrain(x, min, max)	 (((x) < (min)) ? (min) :  (((x) > (max)) ? (max) : (x)))


/* PID update function. Should be called every dt seconds.
 *  input r:reference
 *	input y:feedback
 *	return output
 */
float updatePID(PID_Type *pid,float r, float y)
{
	float err = r - y;
	float kprerr = pid->Kpr * r-y;
	float kdrerr = pid->Kdr * r-y;
	float v1;


	if(pid->Direction==-1)
	{
		err=-err;
		kprerr=-kprerr;
		kdrerr=-kdrerr;
	}

	//P_part=Kp*(Kr*ref-yin)
	pid->P = pid->Kp * kprerr;

	//I_part
	pid->I = pid->I +  pid->Ki * err;
	pid->I = Constrain(pid->I,pid->Imin,pid->Imax);

	//D_part
	//first order low-pass
	//backward Euler
	pid->D = pid->D*pid->Dcoeff[0]+pid->Dcoeff[1]*(y-pid->Ypre);
	pid->Ypre = y;


	//PID total output is the sum of P I D part.
	pid->U = pid->P + pid->I + pid->D;

	//saturation
	pid->U = Constrain(pid->U,pid->Umin,pid->Umax);

	return pid->U;

}

void initPID(PID_Type *pid,float kp,float ki,float kd,float dt,float ilim,float ulim)
{
	 pid->Kp=kp;
	 pid->Ki=ki;
	 pid->Kd=kd;

	 pid->Kpr=1;
	 pid->Kdr=0.0f;

	 pid->Dt=dt;

	 pid->Direction=1;


	 pid->Ypre = 0;
	 pid->N = 15;
	 pid->Td = pid->Kd/pid->Kp;
	 pid->Dcoeff[0]=pid->Td/(pid->Td+pid->N*pid->Dt);
	 pid->Dcoeff[1]=-pid->Kp*pid->N*pid->Dcoeff[0];


	 pid->P=0;
	 pid->I=0;
	 pid->D=0;

	 pid->U=0;

	 pid->Imax = ilim;
	 pid->Imin = -ilim;
	 pid->Umax=ulim;//5e-4;
	 pid->Umin=-ulim;//-5e-4;
}


void setKp(PID_Type *pid, float kp)
{
	if(kp>=0)
	{
		 pid->Kp = kp;
		 pid->Td = pid->Kd/pid->Kp;
		 pid->Dcoeff[0]=pid->Td/(pid->Td+pid->N*pid->Dt);
		 pid->Dcoeff[1]=-pid->Kp*pid->N*pid->Dcoeff[0];
	}
}

void setKd(PID_Type *pid, float kd)
{
	if(kd>=0){
		pid->Kd = kd;
		pid->Td = pid->Kd/pid->Kp;
		 pid->Dcoeff[0]=pid->Td/(pid->Td+pid->N*pid->Dt);
		 pid->Dcoeff[1]=-pid->Kp*pid->N*pid->Dcoeff[0];
	}
}

void setKi(PID_Type *pid, float ki)
{
	if(ki>=0)
		pid->Ki = ki;
}



void setKdr(PID_Type *pid, float kdr)
{
	if(kdr>=0)
		pid->Kdr = kdr;
}

void setKpr(PID_Type *pid, float kpr)
{
	if(kpr>=0)
		pid->Kpr = kpr;
}


void setDt(PID_Type *pid, float dt)
{
	pid->Dt = dt;
}

void setDir(PID_Type *pid, float dir)
{
	if(dir==1 || dir==-1)
		pid->Direction = dir;
}
void setIlim(PID_Type *pid, float ilim)
{
	pid->Imax = ilim;
	pid->Imin = -ilim;
}
void setUlim(PID_Type *pid, float ulim)
{
	pid->Umax = ulim;
	pid->Umin = -ulim;
}


PID_Type *newPID(float kp,float ki,float kd,float dt,float ilim,float ulim)
{

	PID_Type *pp=(PID_Type *)malloc(sizeof(PID_Type));
	if(pp==NULL)
		return NULL;
	memset(pp,0,sizeof(PID_Type));
	initPID(pp,kp,ki,kd,dt,ilim,ulim);
	return pp;
}

