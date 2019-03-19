/*
 * PWMBoardSPI.c
 *
 *  Created on: Dec 28, 2018
 *      Author: Xiaojiao Chen
 *		 Email: chen2014@connect.hku.hk
 *	Laboratory: Bio-Control Lab
 * Orgnization: the University of Hong Kong
 */


#include "PWMBoardSPI.h"



#if (PWMBOARDSPI_NUM>0)
#include "builtInPWM.h"
#include "string.h"
#include "math.h"
#include "main.h"
#include "spi.h"

#define hspi_PWM		hspi4
#define htim_PWMBoardSPI htim14

#define PWM_BOARDSPI_CRITICAL_FRE_NUM 3
static uint32_t PWM_BOARDSPI_TIM_ClockFrequency=0;
static uint32_t PWM_BOARDSPI_ARR;
static uint32_t PWM_BOARDSPI_PSC;
static uint32_t PWM_BOARDSPI_CCR1;
static float 	PWM_BOARDSPI_preduty;
static uint32_t PWM_BOARDSPI_CriticalARR_power[PWM_BOARDSPI_CRITICAL_FRE_NUM]={16,2,2};
static uint32_t PWM_BOARDSPI_CriticalPSC_power[PWM_BOARDSPI_CRITICAL_FRE_NUM]={16,16,0};
static uint32_t PWM_BOARDSPI_CriticalARR[PWM_BOARDSPI_CRITICAL_FRE_NUM];
static uint32_t PWM_BOARDSPI_CriticalPSC[PWM_BOARDSPI_CRITICAL_FRE_NUM];
static float 	PWM_BOARDSPI_CRITICAL_FRE[PWM_BOARDSPI_CRITICAL_FRE_NUM];


PWM_BOARDSPI PWMBoardSPIDevices;
PWM_BOARDSPI *ptPWMBoardSPI=&PWMBoardSPIDevices;
#define	MY_APB1_CLK 108000000
#define	MY_APB2_CLK 216000000
static void PWM_CS_LOW()
{
	ptPWMBoardSPI->CS_Port->BSRR=(uint32_t)ptPWMBoardSPI->CS_Pin<<16U;
}

static void PWM_CS_HIGH()
{
	ptPWMBoardSPI->CS_Port->BSRR=(uint32_t)ptPWMBoardSPI->CS_Pin;
}
static uint32_t getTIM_SOURCECLK(TIM_HandleTypeDef *htim)
{
	if ((htim->Instance==TIM1) || (htim->Instance==TIM8) || (htim->Instance==TIM9) || (htim->Instance==TIM10) || (htim->Instance==TIM11))
		return MY_APB2_CLK;
	else
		return MY_APB1_CLK;
}
static void PWMBoardSPI_Callback()
{
	//the high pulse duration should be less than 5us to maintain the LDI functionality
	PWM_CS_HIGH();
	delay_ns(300);
	PWM_CS_LOW();
}
static void PWMBoardSPI_sendFrame()
{
	HAL_SPI_TransmitReceive(ptPWMBoardSPI->PWM_spi,(uint8_t *)(ptPWMBoardSPI->CMDBuffer),(uint8_t *)(ptPWMBoardSPI->StatusBuffer),PWMBoardSPI_FrameLength,1);
	PWMBoardSPI_Callback();
}

static void PWMBoardSPI_bufferCMD(uint8_t cmd)
{
	for(int i=1;i<PWMBOARDSPI_NUM;i++)
		ptPWMBoardSPI->CMDBuffer[i][LT8500_FrameLength-1]=cmd;
}

static void PWMBoardSPI_bufferDuty(uint16_t num, float fduty)
{

	uint16_t on;
	on = round(4096 * fduty);
	on=CONSTRAIN(on,0,4095);
	num=CONSTRAIN(num,0,PWMBOARDSPI_CHANNELNUM-1);
	uint16_t boards=num/LT8500_CHANNELNUM;
	uint16_t boardNum=num%LT8500_CHANNELNUM;
	uint16_t quo=(boardNum/2);
	uint16_t rem=(boardNum%2);
	uint16_t startingBytesNum=quo*3;
	uint8_t *pt=&(ptPWMBoardSPI->CMDBuffer[boards][startingBytesNum]);

	if(rem==0){
			*pt++=(uint8_t)(on>>4);
			uint8_t ori=*pt;
			ori&=0x0F;
			ori|= ((uint8_t)(on<<4) & 0xF0);
			*pt=ori;
	}
	else
	{
		pt++;
		uint8_t ori=*pt;
		ori&=0xF0;
		ori|= ((uint8_t)(on>>8) & 0x0F);
		*pt++=ori;
		*pt=(uint8_t)(on);
	}
	ptPWMBoardSPI->dirtyDuty=1;
}
static void PWMBoardSPI_bufferCorrection(uint16_t num,uint8_t value)
{

	num=CONSTRAIN(num,0,PWMBOARDSPI_CHANNELNUM-1);
	uint16_t boards=num/LT8500_CHANNELNUM;
	uint16_t boardNum=num%LT8500_CHANNELNUM;
	uint16_t quo=(boardNum/2);
	uint16_t rem=(boardNum%2);
	uint16_t startingBytesNum=quo*3;
	uint8_t *pt=&(ptPWMBoardSPI->CMDBuffer[boards][startingBytesNum]);
	if(rem==0){
		*pt=(uint8_t)(value<<2);
	}
	else
	{
		pt++;
		*pt++=(uint8_t)(value>>2);
		*pt=(uint8_t)(value<<6);
	}
	ptPWMBoardSPI->dirtyCOR=1;
}



void PWMBoardSPI_setOutputEnable(uint8_t flag)
{
	PWMBoardSPI_bufferCMD((flag?LT8500_CMD_OutputEnable:LT8500_CMD_OutputDisable));
	PWMBoardSPI_sendFrame();
}

void PWMBoardSPI_setSyncMode(uint8_t mode)
{
	PWMBoardSPI_bufferCMD(mode);
	PWMBoardSPI_sendFrame();
}
void PWMBoardSPI_setCorrectionEnable(uint8_t flag)
{
	if((ptPWMBoardSPI->StatusBuffer[0][LT8500_FrameLength-1]&0x01)!=flag)
		{
		PWMBoardSPI_bufferCMD(LT8500_CMD_CorrentionToggle);
		PWMBoardSPI_sendFrame();
		}
}

void PWMBoardSPI_startPWMCK(float fre)
{
	PWM_BOARDSPI_TIM_ClockFrequency=getTIM_SOURCECLK(&htim_PWMBoardSPI);
	PWM_BOARDSPI_ARR=500-1;
	PWM_BOARDSPI_PSC=PWM_BOARDSPI_TIM_ClockFrequency/1000-1;
	PWM_BOARDSPI_preduty=0.5;
	PWM_BOARDSPI_CCR1=500;
	for(int j=0;j<PWM_BOARDSPI_CRITICAL_FRE_NUM;j++)
	{
		PWM_BOARDSPI_CriticalARR[j]=pow(2,PWM_BOARDSPI_CriticalARR_power[j]);
		PWM_BOARDSPI_CriticalPSC[j]=pow(2,PWM_BOARDSPI_CriticalPSC_power[j]);
		PWM_BOARDSPI_CRITICAL_FRE[j]=(PWM_BOARDSPI_TIM_ClockFrequency/(double)(PWM_BOARDSPI_CriticalARR[j])/PWM_BOARDSPI_CriticalPSC[j]);
	}
	PWMBoardSPI_writeFrequency(fre);
	HAL_TIM_PWM_Start(&htim_PWMBoardSPI,TIM_CHANNEL_1);
}

void PWMBoardSPI_writeFrequency(float fre)
{
	fre = CONSTRAIN(fre,EXTERNAL_PWMBOARDSPI_FREMIN,EXTERNAL_PWMBOARDSPI_FREMAX);
	float newClkFrequency=fre*4096;
	if(fabs(newClkFrequency-ptPWMBoardSPI->clkFrequency)>1e-6)
	{
		ptPWMBoardSPI->frequency=fre;
		ptPWMBoardSPI->clkFrequency=fre*4096;
		ptPWMBoardSPI->clkFrequency = CONSTRAIN(ptPWMBoardSPI->clkFrequency, BUILTIN_PWM_FREMIN, BUILTIN_PWM_FREMAX);
		for (int i = 1; i < PWM_BOARDSPI_CRITICAL_FRE_NUM; i++) {
				if (ptPWMBoardSPI->clkFrequency <= PWM_BOARDSPI_CRITICAL_FRE[i]) {
					if (PWM_BOARDSPI_CriticalARR[i] == PWM_BOARDSPI_CriticalARR[i - 1]) {
						PWM_BOARDSPI_ARR = PWM_BOARDSPI_CriticalARR[i];
						PWM_BOARDSPI_PSC = (PWM_BOARDSPI_TIM_ClockFrequency/ptPWMBoardSPI->clkFrequency)/PWM_BOARDSPI_ARR;
					} else {
						PWM_BOARDSPI_PSC = PWM_BOARDSPI_CriticalPSC[i];
						PWM_BOARDSPI_ARR = (PWM_BOARDSPI_TIM_ClockFrequency/ptPWMBoardSPI->clkFrequency)/PWM_BOARDSPI_PSC;
					}
					break;
				}
			}
		PWM_BOARDSPI_CCR1 = PWM_BOARDSPI_ARR / 2;
		PWM_BOARDSPI_CCR1 = CONSTRAIN(PWM_BOARDSPI_CCR1, 0, 65535);
		htim_PWMBoardSPI.Instance->PSC = PWM_BOARDSPI_PSC - 1;
		htim_PWMBoardSPI.Instance->ARR = PWM_BOARDSPI_ARR - 1;
		htim_PWMBoardSPI.Instance->CNT = 0;
		htim_PWMBoardSPI.Instance->CCR1 = PWM_BOARDSPI_CCR1;
	}
}
void PWMBoardSPI_writeDuty(uint16_t num, float fduty)
{
	PWMBoardSPI_bufferDuty(num,fduty);
}
void PWMBoardSPI_writeCorrection(uint16_t num,uint8_t value)
{
	PWMBoardSPI_bufferCorrection(num,value);
}

void PWMBoardSPI_updateDutyAll()
{
	if(ptPWMBoardSPI->dirtyDuty!=0)
	{
		PWMBoardSPI_bufferCMD(ptPWMBoardSPI->syncMode);
		PWMBoardSPI_sendFrame();
		ptPWMBoardSPI->dirtyDuty=0;
	}
}
void PWMBoardSPI_updateCorrectionAll()
{
	if(ptPWMBoardSPI->dirtyCOR!=0)
		{
			PWMBoardSPI_bufferCMD(LT8500_CMD_Correction);
			PWMBoardSPI_sendFrame();
			ptPWMBoardSPI->dirtyCOR=0;
		}
}

void PWMBoardSPI_init()
{
	memset(ptPWMBoardSPI,0,sizeof(PWM_BOARDSPI));

	ptPWMBoardSPI->PWM_spi=&hspi_PWM;
	ptPWMBoardSPI->CS_Port=PWM_CS_GPIO_Port;//PWM_CS_GPIO_Port;
	ptPWMBoardSPI->CS_Pin=PWM_CS_Pin;//PWM_CS_Pin;

	ptPWMBoardSPI->Frequency_PortNum=BUILTIN_PWM_NUM;//the one after the last one of the builtInPWMs
	ptPWMBoardSPI->syncMode=LT8500_CMD_AsyUpdate;
	ptPWMBoardSPI->dirtyCOR=0;
	ptPWMBoardSPI->dirtyDuty=0;

	//initialization
	//1. Apply power and drive LDIBLANK low. SDO will go low when the on-chip power-on-reset (POR) de-asserts.
	PWM_CS_LOW();

	//2.Send a correction register frame (CMD = 0x20) on the serial interface. This sets the correction factor on each channel.
	for(int i=0;i<PWMBOARDSPI_CHANNELNUM;i++)
	{
		PWMBoardSPI_bufferCorrection(i,63);
	}
	PWMBoardSPI_updateCorrectionAll();

	//3. Send an update frame (CMD = 0x00 or CMD = 0x10) on the serial interface. This sets the pulse width of each channel.
	PWMBoardSPI_setSyncMode(LT8500_CMD_AsyUpdate);

	//
	for(int i=0;i<PWMBOARDSPI_CHANNELNUM;i++)
	{
		PWMBoardSPI_bufferDuty(i,0.5);
	}
	PWMBoardSPI_updateDutyAll();

	//4. start PWM clock (PWMCK)
	PWMBoardSPI_startPWMCK(40);

	//5. stop correction
	PWMBoardSPI_setCorrectionEnable(0);

	//5. Send an output enable frame (CMD = 0x30) on the serial interface. This enables the modulated pulses on the PWM[48:1] outputs
	PWMBoardSPI_setOutputEnable(1);

}


#endif
