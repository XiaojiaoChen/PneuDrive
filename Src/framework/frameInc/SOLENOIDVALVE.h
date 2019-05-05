/*
 * SOLENOIDVALVE.h
 *
 *  Created on: Jun 15, 2018
 *      Author: Xiaojiao Chen
 *		 Email: chen2014@connect.hku.hk
 *	Laboratory: Bio-Control Lab
 * Orgnization: the University of Hong Kong
 */

#ifndef SOLENOIDVALVE_H_
#define SOLENOIDVALVE_H_
#ifdef __cplusplus
extern "C" {
#endif


class SOLENOID_VALVE {
public:
	SOLENOID_VALVE(int num=0);
	void attach(int num);

	float readDuty();
	void writeDuty(float dut);

	float readFrequency();
	void writeFrequency(float fre);

private:
	int PWMPort;
	float duty;
	float _duty;
	float frequency;
	float dutyMin;
	float dutyMax;
};

#ifdef __cplusplus
}
#endif
#endif /* SOLENOIDVALVE_H_ */
