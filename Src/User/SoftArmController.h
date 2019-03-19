/*
 * SoftArmController.h
 *
 *  Created on: Jun 14, 2018
 *      Author: Xiaojiao Chen
 *	Laboratory: Bio-Control Lab
 * Orgnization: the University of Hong Kong
 */

#ifndef __SOFTARMCONTROLLER_H
#define __SOFTARMCONTROLLER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "SoftArm.h"

class softArmLenAlphaBetaController{

	SOFT_ARM *ptSoftarm;
};

void softArmAutoControl();

#ifdef __cplusplus
}
#endif
#endif /* __SOFTARMCONTROLLER_H */
