#include "valveFlowFunc.h"
#include "math.h"
#include "i2c.h"


#define localConstrain(x, min, max)	 (((x) < (min)) ? (min) :  (((x) > (max)) ? (max) : (x)))
/*
static float myLocalInterpolate(int num,float *x, float *y,float x0);*/
float dutyTable[]={0,27000,48600,65600,79000,90000,100000,109000,119000,130400,145000,163000,180000};
float flowTable[]={0,10,20,30,40,50,60,70,80,90,100,110,120};
static float blam=0.999f;
static float bcr=0.52f;
static float bcr_2=0.2304f;
static float sqrtT=17.3205;
static float C1=.0404184;
static float Patm=101325;
static float tem2=997.9166666;
static float areaMax = 1e-6;
static int16_t I2C1_flowsensor_BUF[2];
#define __SWAP16(x)   ((int16_t)((((uint16_t)(x)<<8)& 0xff00) | (((uint16_t)(x) >>8)&0x00ff)))
//Input percentage ----> output flowrate
//(0~1)   ----->>>>  (0~areaMax*C1*Tem)


//Input voltage -->> orifice area
static float voltage2Area(float u)
{
	 u= localConstrain(u,0,1.0f);
	return(u*areaMax);

}

// orifice area  -->>  needed Input voltage
static float area2Voltage(float area)
{
	area = localConstrain(area,0,areaMax);
	return(area/areaMax);
}


float voltage2flowrate(float Pu,float Pd,float u)
{
	float Pr,A,flow,tem;
	u=localConstrain(u,0,1);
	Pu+=Patm;
	Pd+=Patm;
    A=voltage2Area(u);
    tem=Pu/sqrtT;
    Pr=Pd/Pu;

    
    if (Pr < bcr)       //choked flow
	{
		flow = A * C1 * tem;
	} else if (Pr < blam)  //subsonic flow
	{
		flow = A * C1 * tem * sqrt(1 - (Pr - bcr) * (Pr - bcr) / bcr_2);
	} else if (Pr <= 1)           //laminar flow
	{
		tem = (Pu - Pd) / sqrtT;
		flow = A * C1 * tem * (1 - Pr) * tem2;
	} else
	{
		flow = 0;
	}

	return flow;
}


//output flowrate  ----->>>  needed input voltage
float flowrate2Voltage(float Pu,float Pd,float q)
{ 
	float Pr,A,tem;

	Pu+=Patm;
	Pd+=Patm;
    tem=Pu/sqrtT;
    Pr=Pd/Pu;
    if(Pr>=1)
    	return 0;
    else
    {
		if (Pr<bcr)       //choked flow  y=dir*A*C1*tem
			A=q/(C1*tem);
		else if (Pr<blam)  //subsonic flow   y=dir*A*C1*tem*sqrt(1-((Pr-bcr)/(1-bcr))^2);
			A=q/(C1*tem*sqrt(1-(Pr-bcr)*(Pr-bcr)/bcr_2));
		else{            //laminar flow    y=dir*A*Cd*C1*tem*((1-Pr)/(1-blam))*sqrt(1-((blam-bcr)/(1-bcr))^2);
			tem=(Pu-Pd)/sqrtT;
			A=q/(C1*tem*(1-Pr)*tem2);
			}
		return area2Voltage(A);
    }
}

float getFlowrate()
{
	HAL_I2C_Master_Receive(&hi2c1,0x92,(uint8_t *)I2C1_flowsensor_BUF,2,1);
	I2C1_flowsensor_BUF[1]=__SWAP16(I2C1_flowsensor_BUF[0]);
	return 100*(I2C1_flowsensor_BUF[1]/16384.0f-0.1f)/0.8f;
}

						
/*static float myLocalInterpolate(int num,float *x, float *y,float x0){
	if(x0<x[0])
		return map(x0,x[0],x[1],y[0],y[1]);
	else if(x0>=x[num-1])
		return map(x0,x[num-2],x[num-1],y[num-2],y[num-1]);
	else{
		for(int i=1;i<num;i++)
			if (x0<x[i])
				return map(x0,x[i-1],x[i],y[i-1],y[i]);
	}
	return 0;//never reach
}*/
