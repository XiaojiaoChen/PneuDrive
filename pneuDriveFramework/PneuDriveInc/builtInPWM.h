/*
 * builtInPWM.h
 *
 *  Created on: Jan 3, 2019
 *      Author: Xiaojiao Chen
 *		 Email: chen2014@connect.hku.hk
 *	Laboratory: Bio-Control Lab
 * Orgnization: the University of Hong Kong
 */

#ifndef FRAMEWORK_FRAMEINC_BUILTINPWM_H_
#define FRAMEWORK_FRAMEINC_BUILTINPWM_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include <builtInHWConfig.h>

void PWMBuiltIn_writeDuty(uint16_t num, float fduty);
void PWMBuiltIn_writeFrequency(uint16_t num, float fre);

float PWMBuiltIn_readDuty(uint16_t num);
float PWMBuiltIn_readFrequency(uint16_t num);

void PWMBuiltInStartChannel(int i);
void PWMBuiltInStopChannel(int i);

void PWMBuiltInStart();
void PWMBuiltInStop();

void PWMBuiltIn_init();


#ifdef __cplusplus
}
#endif
#endif /* FRAMEWORK_FRAMEINC_BUILTINPWM_H_ */
