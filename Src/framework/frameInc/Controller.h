#ifndef __CONTROLLER_H
#define __CONTROLLER_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "kalman.h"
#include "myPID.h"

 typedef struct SUPER_TWISTING_SMC_TAG{
	 float err;
	 float errdot;
	 float s;
	 float y;
	 float k;
	 float ystate;
	 float dt;
	 float lamda;
	 float lamdadot;
 }SUPER_TWISTING_SMC; // Behave like a PI controller


typedef struct BASIC_PRESSURE_CONTROLLER_STRUCT{
	float p;
	float pd;
	float psource;
	float psink;
	float flow;
	float voltage;
	float dt;
	KALMAN_FILTER *pKalmanFilter;
	PID_Type *pPID;
	SUPER_TWISTING_SMC SUPER_TWISTING_SMC;
	float (*controlPressure)(struct BASIC_PRESSURE_CONTROLLER_STRUCT *bpc,float e,float edot);
}BASIC_PRESSURE_CONTROLLER;


BASIC_PRESSURE_CONTROLLER *NewPressureController(float psource,float psink,float dt,float kalmanQ,float kalmanR,
		float pidKp,float pidKi,float pidKd,float pidilim,float pidulim);

#ifdef __cplusplus
 }
#endif

#endif
