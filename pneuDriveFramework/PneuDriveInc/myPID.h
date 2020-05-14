/*
 * myPID.h
 *
 *  Created on: Oct 4, 2017
 *      Author: 40207
 */

#ifndef USER_INC_MYPID_H_
#define USER_INC_MYPID_H_
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	 float Kp;
	 float Ki;
	 float Kd;

	 float Kpr;
	 float Kdr;

	 float Dt;


	 float Ypre;
	 float N;
	 float Td;
	 float Dcoeff[2];


	 float Imax;
	 float Imin;
	 float Umax;
	 float Umin;
	 float P;
	 float I;
	 float D;

	 float Direction;

	 float Tau;
	 float U;
}PID_Type;


float updatePID(PID_Type *pid,float r, float y);
void initPID(PID_Type *pid,float kp,float ki,float kd,float dt,float ilim,float ulim);
void setKp(PID_Type *, float);
void setKi(PID_Type *, float);
void setKd(PID_Type *, float);
void setKpr(PID_Type *, float);
void setKdr(PID_Type *, float);
void setDt(PID_Type *, float);
void setIlim(PID_Type *, float);
void setUlim(PID_Type *, float);
PID_Type *newPID(float kp,float ki,float kd,float dt,float ilim,float ulim);

#ifdef __cplusplus
}
#endif
#endif /* USER_INC_MYPID_H_ */
