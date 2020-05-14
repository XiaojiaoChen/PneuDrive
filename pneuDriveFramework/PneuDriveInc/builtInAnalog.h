/*
 * builtInAnalog.h
 *
 *  Created on: Jan 3, 2019
 *      Author: Xiaojiao Chen
 *		 Email: chen2014@connect.hku.hk
 *	Laboratory: Bio-Control Lab
 * Orgnization: the University of Hong Kong
 */

#ifndef FRAMEWORK_FRAMEINC_BUILTINANALOG_H_
#define FRAMEWORK_FRAMEINC_BUILTINANALOG_H_

#ifdef __cplusplus
 extern "C" {
#endif


#include <builtInHWConfig.h>


 void AnaBuiltInStart();
 void AnaBuiltInStop();
 void AnaBuiltIn_CallBack();
 float AnalogRead_BuiltIn(int num);






#ifdef __cplusplus
}
#endif
#endif /* FRAMEWORK_FRAMEINC_BUILTINANALOG_H_ */
