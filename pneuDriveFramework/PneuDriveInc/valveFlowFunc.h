#ifndef __VALVEFLOWFUNC_H
#define	__VALVEFLOWFUNC_H

#ifdef __cplusplus
 extern "C" {
#endif
#include <stm32f7xx_hal.h>


float voltage2flowrate(float P1,float P2,float u);
float flowrate2Voltage(float P1,float P2,float q);
float getFlowrate();

#ifdef __cplusplus
 }
#endif

#endif
