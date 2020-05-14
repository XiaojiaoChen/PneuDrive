/*
 * builtInPWM.c
 *
 *  Created on: Jan 3, 2019
 *      Author: Xiaojiao Chen
 *		 Email: chen2014@connect.hku.hk
 *	Laboratory: Bio-Control Lab
 * Orgnization: the University of Hong Kong
 */

#include "builtInPWM.h"
#include "main.h"
#include "tim.h"
#include "math.h"

//According to stm32F767zi's datasheet memory map
#define	MY_APB1_CLK 108000000
#define	MY_APB2_CLK 216000000





static TIM_HandleTypeDef *PWM_TIMs_Array[BUILTIN_PWM_NUM]={
		&htim1,&htim1,
		&htim2,&htim2,&htim2,
		&htim3,&htim3,&htim3,&htim3,
		&htim4,&htim4,&htim4,&htim4,
		&htim8,&htim8,&htim9,&htim9,
		&htim10,&htim11,&htim14};

static uint32_t PWM_CHANNELs_Array[BUILTIN_PWM_NUM]={
		TIM_CHANNEL_1,TIM_CHANNEL_2,
		TIM_CHANNEL_1,TIM_CHANNEL_3,TIM_CHANNEL_4,
		TIM_CHANNEL_1,TIM_CHANNEL_2,TIM_CHANNEL_3,TIM_CHANNEL_4,
		TIM_CHANNEL_1,TIM_CHANNEL_2,TIM_CHANNEL_3,TIM_CHANNEL_4,
		TIM_CHANNEL_1,TIM_CHANNEL_2,TIM_CHANNEL_1,TIM_CHANNEL_2,
		TIM_CHANNEL_1,TIM_CHANNEL_1,TIM_CHANNEL_1};

#define PWM_CRITICAL_FRE_NUM 10
static uint32_t PWM_TIMs_CLOCKFREQUENCYs[BUILTIN_PWM_NUM];

static uint32_t PWM_CriticalARR_power[PWM_CRITICAL_FRE_NUM]={16,16,14,14,10,10,6,6,2,2};
static uint32_t PWM_CriticalPSC_power[PWM_CRITICAL_FRE_NUM]={16,8,8,6,6,4,4,2,2,0};
static uint32_t PWM_CriticalARR[PWM_CRITICAL_FRE_NUM];
static uint32_t PWM_CriticalPSC[PWM_CRITICAL_FRE_NUM];
static float 	PWM_CRITICAL_FRE[BUILTIN_PWM_NUM][PWM_CRITICAL_FRE_NUM];

static uint32_t duty2CCR(uint32_t arr,float fduty){
	uint32_t ccr=(round)((uint32_t)(arr+1)*fduty);
	return CONSTRAIN(ccr,0,65535);
}

static float CCR2duty(uint32_t arr,uint32_t ccr){
	float fccr=ccr;
	return (fccr/(uint32_t)(arr+1));
}

static uint32_t getTIM_SOURCECLK(TIM_HandleTypeDef *htim)
{
	if ((htim->Instance==TIM1) || (htim->Instance==TIM8) || (htim->Instance==TIM9) || (htim->Instance==TIM10) || (htim->Instance==TIM11))
		return MY_APB2_CLK;
	else
		return MY_APB1_CLK;
}


void PWMBuiltIn_writeDuty(uint16_t num, float fduty){
	//uduty~[0,ARR+1],uduty=0 means constant low; uduty=ARR+1 means constant high;
	__HAL_TIM_SET_COMPARE(PWM_TIMs_Array[num], PWM_CHANNELs_Array[num], duty2CCR((uint32_t)PWM_TIMs_Array[num]->Instance->ARR,fduty));

}
void PWMBuiltIn_writeFrequency(uint16_t num, float fre) {
	fre = CONSTRAIN(fre, BUILTIN_PWM_FREMIN, BUILTIN_PWM_FREMAX);
	uint32_t ARRtem=0;

	//recalculate the ARR and PSC with the propriate resolution and accuracy
	for (int i = 1; i < PWM_CRITICAL_FRE_NUM; i++) {
		if (fre <= PWM_CRITICAL_FRE[num][i]) {
			if (PWM_CriticalARR[i] == PWM_CriticalARR[i - 1]) {
				ARRtem = PWM_CriticalARR[i];
				__HAL_TIM_SET_PRESCALER(PWM_TIMs_Array[num],(uint32_t)((PWM_TIMs_CLOCKFREQUENCYs[num]/fre)/(ARRtem+1))-1);
			} else {
				__HAL_TIM_SET_PRESCALER(PWM_TIMs_Array[num],PWM_CriticalPSC[i]);
				ARRtem =(uint32_t)((PWM_TIMs_CLOCKFREQUENCYs[num]/fre)/(uint32_t)(PWM_TIMs_Array[num]->Instance->PSC+1))-1;
			}
			break;
		}
	}

	//check if the ARR has changed
	if(ARRtem!=(uint32_t)(PWM_TIMs_Array[num]->Instance->ARR)){

		//alter all the channels' CCR under the same TIM because they share a common ARR.
		for(int j=0;j<BUILTIN_PWM_NUM;j++){
			if(PWM_TIMs_Array[j]==PWM_TIMs_Array[num]){
				__HAL_TIM_SET_COMPARE(PWM_TIMs_Array[j], PWM_CHANNELs_Array[j],duty2CCR(ARRtem,PWMBuiltIn_readDuty(j)));
				__HAL_TIM_SET_COUNTER(PWM_TIMs_Array[j],0);
			}
		}
		//alter ARR after CCR
		__HAL_TIM_SET_AUTORELOAD(PWM_TIMs_Array[num],ARRtem);
	}
}

float PWMBuiltIn_readDuty(uint16_t num){
	return CCR2duty((uint32_t)PWM_TIMs_Array[num]->Instance->ARR,(uint32_t)(__HAL_TIM_GET_COMPARE(PWM_TIMs_Array[num], PWM_CHANNELs_Array[num])));
}

float PWMBuiltIn_readFrequency(uint16_t num){
	float arr=(uint32_t)(PWM_TIMs_Array[num]->Instance->ARR) + 1;
	float psc=(uint32_t)(PWM_TIMs_Array[num]->Instance->PSC) + 1;
	return (PWM_TIMs_CLOCKFREQUENCYs[num]/arr)/psc;
}

void PWMBuiltInStartChannel(int i)
{
	HAL_TIM_PWM_Start(PWM_TIMs_Array[i],PWM_CHANNELs_Array[i]);
}

void PWMBuiltInStopChannel(int i)
{
	HAL_TIM_PWM_Stop(PWM_TIMs_Array[i],PWM_CHANNELs_Array[i]);
}

void PWMBuiltInStart()
{
	for(int i=0;i<BUILTIN_PWM_NUM;i++)
		PWMBuiltInStartChannel(i);
}
void PWMBuiltInStop()
{
	for(int i=0;i<BUILTIN_PWM_NUM;i++)
		PWMBuiltInStopChannel(i);
}

void PWMBuiltIn_init()
{
	for(int i=0;i<BUILTIN_PWM_NUM;i++)
	{
		PWM_TIMs_CLOCKFREQUENCYs[i]=getTIM_SOURCECLK(PWM_TIMs_Array[i]);
		for(int j=0;j<PWM_CRITICAL_FRE_NUM;j++)
		{
			PWM_CriticalARR[j]=(uint32_t)(0x01<<PWM_CriticalARR_power[j])-1;
			PWM_CriticalPSC[j]=(uint32_t)(0x01<<PWM_CriticalPSC_power[j])-1;
			float arr=(uint32_t)(PWM_CriticalARR[j]+1);
			float psc=(uint32_t)(PWM_CriticalPSC[j]+1);
			PWM_CRITICAL_FRE[i][j]=((PWM_TIMs_CLOCKFREQUENCYs[i]/arr)/psc);  //be careful of overflow
		}
		PWMBuiltIn_writeDuty(i,0);
		PWMBuiltIn_writeFrequency(i,40);

	}
	PWMBuiltInStart();
}

