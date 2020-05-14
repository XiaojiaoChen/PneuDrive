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
#include "PneuDriveLL.h"
#include "builtInPWM.h"
#include "string.h"
#include "math.h"
#include "main.h"
#include "spi.h"

#define hspi_PWM		hspi3


PWM_BOARDSPI PWMBoardSPIDevices;
PWM_BOARDSPI *ptPWMBoardSPI=&PWMBoardSPIDevices;

static void PWM_CS_LOW();
static void PWM_CS_HIGH();
static void PWMBoardSPI_Callback();
static void PWMBoardSPI_sendFrame();
static void PWMBoardSPI_bufferCMD(uint8_t cmd);
static void PWMBoardSPI_bufferDutyChannel(uint16_t num, float fduty);
static void PWMBoardSPI_bufferCorrection(uint16_t num,uint8_t value);
static void PWMBoardSPI_setOutputEnable(uint8_t flag);
static void PWMBoardSPI_setCorrectionEnable(uint8_t flag);
static void PWMBoardSPI_startPWMCK(float fre);


static void PWM_CS_LOW()
{
	ptPWMBoardSPI->CS_Port->BSRR=(uint32_t)ptPWMBoardSPI->CS_Pin<<16U;
}

static void PWM_CS_HIGH()
{
	ptPWMBoardSPI->CS_Port->BSRR=(uint32_t)ptPWMBoardSPI->CS_Pin;
}

static void PWMBoardSPI_Callback()
{
	//the high pulse duration should be less than 5us to maintain the LDI functionality
	PWM_CS_HIGH();
	delay_us(2);
	PWM_CS_LOW();
}

static void PWMBoardSPI_sendFrame()
{
	HAL_SPI_TransmitReceive(ptPWMBoardSPI->PWM_spi,(uint8_t *)(ptPWMBoardSPI->CMDBuffer),(uint8_t *)(ptPWMBoardSPI->StatusBuffer),PWMBoardSPI_FrameLength,2);
	PWMBoardSPI_Callback();
}

static void PWMBoardSPI_bufferCMD(uint8_t cmd)
{
	for(int i=0;i<PWMBOARDSPI_NUM;i++)
		ptPWMBoardSPI->CMDBuffer[i][LT8500_FrameLength-1]=cmd;
}

static void PWMBoardSPI_bufferDutyChannel(uint16_t num, float fduty)
{

	uint16_t on;

	on = round(4096 * fduty);
	on=CONSTRAIN(on,0,4095);
	num=CONSTRAIN(num,0,PWMBOARDSPI_CHANNELNUM-1);
	uint16_t boards=num/LT8500_CHANNELNUM;
	uint16_t boardNum=num%LT8500_CHANNELNUM;
	boardNum = LT8500_CHANNELNUM-1 - boardNum;

	uint16_t quo=(boardNum/2);
	uint16_t rem=(boardNum%2);
	uint16_t startingBytesNum=quo*3;
	uint8_t *pt=&(ptPWMBoardSPI->CMDBuffer[boards][startingBytesNum]);

	if(rem==0){
			*pt++=(uint8_t)((on>>4)&0xFF);
			uint8_t ori=*pt;
			ori&=0x0F;
			ori|= ((uint8_t)(on<<4) & 0xF0);
			*pt=ori;
		//	*pt=(uint8_t)(((*pt)&0x0F)|((uint8_t)(on&0x0F)<<4));
	}
	else
	{
		pt++;
		uint8_t ori=*pt;
		ori&=0xF0;
		ori|= ((uint8_t)(on>>8) & 0x0F);
		*pt++=ori;
		*pt=(uint8_t)(on);
//		*pt=(uint8_t)(((*pt)&0xF0)|((uint8_t)(on>>8)));
//		pt++;
//		*pt=(uint8_t)(on&0xFF);

	}
	ptPWMBoardSPI->dirtyDuty=1;
	PWMBoardSPI_bufferCMD(ptPWMBoardSPI->syncMode);
}

static void PWMBoardSPI_bufferCorrection(uint16_t num,uint8_t value)
{

	num=CONSTRAIN(num,0,PWMBOARDSPI_CHANNELNUM-1);
	uint16_t boards=num/LT8500_CHANNELNUM;
	uint16_t boardNum=num%LT8500_CHANNELNUM;
	uint16_t quo=(boardNum/2);
	uint16_t rem=(boardNum%2);
	boardNum = LT8500_CHANNELNUM-1 - boardNum;

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
	PWMBoardSPI_bufferCMD(LT8500_CMD_Correction);
}



static void PWMBoardSPI_setOutputEnable(uint8_t flag)
{
	PWMBoardSPI_bufferCMD((flag?LT8500_CMD_OutputEnable:LT8500_CMD_OutputDisable));
	PWMBoardSPI_sendFrame();
	HAL_Delay(500);//THis delay is important, otherwise the enabling funciton may not take effective. I don't know why...
}

static void PWMBoardSPI_setCorrectionEnable(uint8_t flag)
{
	if((ptPWMBoardSPI->StatusBuffer[0][LT8500_FrameLength-1]&0x01)!=(1-flag))
		{
		PWMBoardSPI_bufferCMD(LT8500_CMD_CorrentionToggle);
		PWMBoardSPI_sendFrame();
		}
}

static void PWMBoardSPI_startPWMCK(float fre)
{
	PWMBuiltIn_writeDuty(ptPWMBoardSPI->Frequency_PWMPortNum,0.5);
	PWMBoardSPI_writeFrequency(fre);
	PWMBuiltInStartChannel(ptPWMBoardSPI->Frequency_PWMPortNum);
}


void PWMBoardSPI_writeFrequency(float fre)
{
	fre = CONSTRAIN(fre,EXTERNAL_PWMBOARDSPI_FREMIN,EXTERNAL_PWMBOARDSPI_FREMAX);
	float newClkFrequency=fre*4096;
	if(fabs(newClkFrequency-ptPWMBoardSPI->clkFrequency)>1e-6)
	{
		ptPWMBoardSPI->frequency=fre;
		ptPWMBoardSPI->clkFrequency= newClkFrequency;
		ptPWMBoardSPI->clkFrequency = CONSTRAIN(ptPWMBoardSPI->clkFrequency, BUILTIN_PWM_FREMIN, BUILTIN_PWM_FREMAX);
		PWMBuiltIn_writeFrequency(ptPWMBoardSPI->Frequency_PWMPortNum,ptPWMBoardSPI->clkFrequency);
	}
}
void PWMBoardSPI_writeDuty(uint16_t num, float fduty)
{
	PWMBoardSPI_bufferDutyChannel(num,fduty);
}
void PWMBoardSPI_writeCorrectionAll(uint8_t value)
{
	for(int i=0;i<PWMBOARDSPI_CHANNELNUM;i++)
		{
			PWMBoardSPI_bufferCorrection(i,value);
		}
	PWMBoardSPI_sendFrame();
}

void PWMBoardSPI_flushDutyAll()
{
	uint32_t c1=micros();
	if(ptPWMBoardSPI->dirtyDuty==1)
	{
		ptPWMBoardSPI->dirtyDuty=0;
		PWMBoardSPI_sendFrame();
	}
	ptPWMBoardSPI->lastUpdateTime=micros()-c1;
}

void PWMBoardSPI_init()
{
	memset(ptPWMBoardSPI,0,sizeof(PWM_BOARDSPI));

	ptPWMBoardSPI->PWM_spi=&hspi_PWM;
	ptPWMBoardSPI->CS_Port=DO_15_GPIO_Port;
	ptPWMBoardSPI->CS_Pin=DO_15_Pin;

	ptPWMBoardSPI->Frequency_PWMPortNum=BUILTIN_PWM_NUM-1;
	ptPWMBoardSPI->syncMode=LT8500_CMD_AsyUpdate;
	ptPWMBoardSPI->dirtyCOR=0;
	ptPWMBoardSPI->dirtyDuty=0;
	ptPWMBoardSPI->lastUpdateTime=0;
	ptPWMBoardSPI->PWMTime=0;
	ptPWMBoardSPI->PWMTimeEnd=0;
	ptPWMBoardSPI->PWMTimeStart=0;

	PWMBuiltInStopChannel(ptPWMBoardSPI->Frequency_PWMPortNum);

	//initialization
	//1. Apply power and drive LDIBLANK low. SDO will go low when the on-chip power-on-reset (POR) de-asserts.
	PWM_CS_LOW();

	//2.Send a correction register frame (CMD = 0x20) on the serial interface. This sets the correction factor on each channel.
	PWMBoardSPI_writeCorrectionAll(63);

	//3. Send an update frame (CMD = 0x00 or CMD = 0x10) on the serial interface. This sets the pulse width of each channel.
	for(int i=0;i<PWMBOARDSPI_CHANNELNUM;i++)
	{
		PWMBoardSPI_bufferDutyChannel(i,0);
	}
	PWMBoardSPI_sendFrame();

	//4. start PWM clock (PWMCK)
	PWMBoardSPI_startPWMCK(40);

	PWMBoardSPI_setCorrectionEnable(0);

	//5. Send an output enable frame (CMD = 0x30) on the serial interface. This enables the modulated pulses on the PWM[48:1] outputs
	PWMBoardSPI_setOutputEnable(1);

}

#endif
