/*
 * PRESSURESENSOR.h
 *
 *  Created on: Jun 15, 2018
 *      Author: Xiaojiao Chen
 *		 Email: chen2014@connect.hku.hk
 *	Laboratory: Bio-Control Lab
 * Orgnization: the University of Hong Kong
 */

#ifndef PRESSURESENSOR_H_
#define PRESSURESENSOR_H_
#ifdef __cplusplus
extern "C" {
#endif

#define PascalPerPSI 6894.76


class PRESSURE_SENSOR {
public:
	PRESSURE_SENSOR(int num = 0);
	void attach(int num);
	float read();
	void setSensorRange_GaugePa(float vmin,float vmax,float pmin,float pmax);
	int AnalogPort;
	float voltage;
	float pressure;


	float vMin;
	float vMax;
	float pMin;
	float pMax;
};


#ifdef __cplusplus
}
#endif
#endif /* PRESSURESENSOR_H_ */
