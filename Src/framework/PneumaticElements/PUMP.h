/*
 * PUMP.h
 *
 *  Created on: Jun 15, 2018
 *      Author: Xiaojiao Chen
 *		 Email: chen2014@connect.hku.hk
 *	Laboratory: Bio-Control Lab
 * Orgnization: the University of Hong Kong
 */

#ifndef PUMP_H_
#define PUMP_H_
#ifdef __cplusplus
extern "C" {
#endif

class PUMP {
public:
	PUMP(int num=0);
	void attach(int num);
	void start();
	void stop();
	int DigitalPort;
	int status;
};

#ifdef __cplusplus
}
#endif
#endif /* PUMP_H_ */
