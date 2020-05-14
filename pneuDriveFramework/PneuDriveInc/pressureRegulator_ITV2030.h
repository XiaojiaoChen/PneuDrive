#ifndef __PRESSURERUGULATORITV2030_H
#define __PRESSURERUGULATORITV2030_H

#include "stm32f7xx_hal.h"


void setPressure(float);
float getPressure(float);
void setZeroRegulator(float);
#endif
