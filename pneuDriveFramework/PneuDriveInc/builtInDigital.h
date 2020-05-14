/*
 * builtInDigital.h
 *
 *  Created on: Jan 4, 2019
 *      Author: Xiaojiao Chen
 *		 Email: chen2014@connect.hku.hk
 *	Laboratory: Bio-Control Lab
 * Orgnization: the University of Hong Kong
 */

#ifndef FRAMEWORK_FRAMEINC_BUILTINDIGITAL_H_
#define FRAMEWORK_FRAMEINC_BUILTINDIGITAL_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include <builtInHWConfig.h>

 void DigitalBuiltIn_Write(uint16_t num, uint16_t state);
 void DigitalBuiltIn_Toggle(uint16_t num);



#ifdef __cplusplus
}
#endif
#endif /* FRAMEWORK_FRAMEINC_BUILTINDIGITAL_H_ */
