/*
 * messages.h
 *
 *  Created on: Mar 6, 2020
 *      Author: Xiaojiao Chen
 *       Email: yii402072495@gmail.com
 */

#ifndef MESSAGES_H_
#define MESSAGES_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f7xx_HAL.h"

/*If this value is 1, the pressure is encoded by 12 bit, with unit hPa. The quaternion is encoded by 14 bit for each component.
 *If this value is 0, the pressure is encoded by 9 bit, with unit KPa. The quaternion is encoded by 15 bit for each component.*/
#define COMPACT_VERSION_PRESSURE_HPA  0


struct QUATERNION{
 /*unCompressed Quaternion*/
 int16_t imuData[4];
};

struct QUATERNIONCOMPACT {
#if COMPACT_VERSION_PRESSURE_HPA==1
/* Compressed Quaternion by omitting the largest component, Other component limited to 14bit.
 * maxLocHigh and maxLocLow specify the location of the max component(omitted one) (default:q=w+xi+yj+zk)
 * maxSign is the sign bit of the max component, which is not necessarily needed. For simplicity we added it.*/
	uint16_t imuData0 :14, maxLocHigh :1, distanceBit2 :1;
	uint16_t imuData1 :14, maxLocLow :1, distanceBit1 :1;
	uint16_t imuData2 :14, maxSign :1, distanceBit0 :1;
#else
	uint16_t imuData0 :15, maxLocHigh :1;
	uint16_t imuData1 :15, maxLocLow :1;
	uint16_t imuData2 :15, maxSign :1;
#endif
};

struct SENSORDATA {
#if COMPACT_VERSION_PRESSURE_HPA==1
	uint16_t pressure:12, distance:4;
#else
	uint16_t pressure:9, distance:7;
#endif
	struct QUATERNIONCOMPACT quaternionCom;
};

struct COMMANDDATA {
	uint16_t values[3];
	uint16_t commandType;
};


/*packQ should be used in sender, i.e. actuator nodes*/
void packQuaternion(struct QUATERNION *qOri, struct QUATERNIONCOMPACT *qCom);

/*unpackQ should be used in decoder, i.e. PC host*/
void unpackQuaternion(struct QUATERNIONCOMPACT *qCom, struct QUATERNION *qOri);

#ifdef __cplusplus
}
#endif
#endif /* MESSAGES_H_ */
