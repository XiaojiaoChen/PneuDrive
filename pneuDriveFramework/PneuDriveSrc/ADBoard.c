
#include <ADBoard.h>

#if (ADBOARD_NUM>0)


#include "PneuDriveLL.h"
#include "spi.h"


/*****Private define*************/



AD_BOARDS ADBoardDevices;
AD_BOARDS *ptADBoardDevices=&ADBoardDevices;


/******************************AD7616 Device**********************************/
static void AD_CS_LOW(AD7616_DEVICE *ptADDevice)
{
	DigitalWrite(ptADDevice->CS_DO_x,LOW);
}

static void AD_CS_HIGH(AD7616_DEVICE *ptADDevice)
{
	DigitalWrite(ptADDevice->CS_DO_x,HIGH);
}
static void AD_CONV_LOW(AD7616_DEVICE *ptADDevice)
{
	DigitalWrite(ptADDevice->CONV_DO_x,LOW);
}

static void AD_CONV_HIGH(AD7616_DEVICE *ptADDevice)
{
	DigitalWrite(ptADDevice->CONV_DO_x,HIGH);
}

static void AD_RESET_LOW(AD7616_DEVICE *ptADDevice)
{
	DigitalWrite(ptADDevice->RESET_DO_x,LOW);
}

static void AD_RESET_HIGH(AD7616_DEVICE *ptADDevice)
{
	DigitalWrite(ptADDevice->RESET_DO_x,HIGH);
}


static void AD7616_getVoltage(AD7616_DEVICE *ptADDev)
{
	ptADDev->chosen = 1;
	 if (ADBOARD_TYPE==AD7616){
	AD_CONV_HIGH(ptADDev);
	delay_ns(200);
	AD_CONV_LOW(ptADDev);}
	else if (ADBOARD_TYPE==AD7606){
	AD_CONV_LOW(ptADDev);
	delay_ns(200);
	AD_CONV_HIGH(ptADDev);}
	ptADDev->ucDataFlag = 0;
}
static uint16_t ulDummyWord[16]={0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF};

int ADBoard_BUSYCallback(uint16_t Int_x)
{
	int ret=1;
	for(int i=0;i<ADBOARD_NUM;i++)
	{
		AD7616_DEVICE *ptADDev=&(ptADBoardDevices->ADBoards[i]);
		if (Int_x == ptADDev->BUSY_INT_x)
			{
				ptADBoardDevices->curADBoardNum=i;
				AD_CS_LOW(ptADDev);
				HAL_SPI_TransmitReceive_DMA(ptADDev->AD_spi,(uint8_t *)ulDummyWord,(uint8_t *)ptADDev->uChannel, AD76x6_CHANNELNUM);
				ret=0;
				break;
			}
	}
	return ret;
}

int ADBoard_SPICallback(SPI_HandleTypeDef *hspi)
{
	int ret=1;
		for(int i=0;i<ADBOARD_NUM;i++)
		{
			AD7616_DEVICE *ptADDev=&(ptADBoardDevices->ADBoards[i]);
			if (hspi == ptADDev->AD_spi)
				{
					ptADBoardDevices->curADBoardNum=i;
					AD_CS_HIGH(ptADDev);
					ptADDev->ucDataFlag = 1;
					ptADDev->chosen = 0;
					for(int j=0;j<AD76x6_CHANNELNUM;j++)
						ptADDev->fChannel[j]=((int16_t)ptADDev->uChannel[j])/65536.0f*ptADDev->fRange;
					if(ptADBoardDevices->curADBoardNum + 1 <ADBOARD_NUM)
					{
						ptADBoardDevices->curADBoardNum +=1;

						AD7616_getVoltage(&(ptADBoardDevices->ADBoards[ptADBoardDevices->curADBoardNum]));
					}
					else
					{
						ptADBoardDevices->curADBoardNum = -1;
						ptADBoardDevices->ucDataFlag = 1;
						ptADBoardDevices->ADTimeEnd=micros();
						ptADBoardDevices->ADTime=ptADBoardDevices->ADTimeEnd-ptADBoardDevices->ADTimeStart;
					}
					ret=0;
					break;
				}

		}
	return ret;
}

void ADBoard_Reset()
{
 if (ADBOARD_TYPE==AD7616){
	AD7616_DEVICE *ptADDev=&(ptADBoardDevices->ADBoards[0]);
	AD_RESET_HIGH(ptADDev);
	delay_us(5);
	AD_RESET_LOW(ptADDev);
	delay_us(5);
	AD_RESET_HIGH(ptADDev);}
 else if (ADBOARD_TYPE==AD7606){
	AD7616_DEVICE *ptADDev=&(ptADBoardDevices->ADBoards[0]);
	AD_RESET_LOW(ptADDev);
	delay_us(5);
	AD_RESET_HIGH(ptADDev);
	delay_us(5);
	AD_RESET_LOW(ptADDev);}

}

void ADBoard_updateVoltage()
{
	ptADBoardDevices->ucDataFlag=0;
	ptADBoardDevices->curADBoardNum = 0;
	ptADBoardDevices->ADTimeStart=micros();
	AD7616_getVoltage(&(ptADBoardDevices->ADBoards[ptADBoardDevices->curADBoardNum]));
}

float AnalogRead_External(uint16_t num) {
	if (0<=num && num<EXTERNAL_ANA_IN_NUM)
	{
		uint16_t boardnum=num/AD76x6_CHANNELNUM;
		uint16_t channelnum=num%AD76x6_CHANNELNUM;
		return ptADBoardDevices->ADBoards[boardnum].fChannel[channelnum];
	}
	else
		return 0;
}

void ADBoard_Init()
{
	uint8_t CONV_DO_x=11;
	uint8_t RESET_DO_x=12;
	uint8_t CS_DO_x=13;
	uint8_t BUSY_INT_x=3;
	ptADBoardDevices->curADBoardNum=-1;
	ptADBoardDevices->ucDataFlag = 0;
	ptADBoardDevices->ADTime=0;
	ptADBoardDevices->ADTimeStart=0;
	ptADBoardDevices->ADTimeEnd=0;

	ptADBoardDevices->ADBoards[0].fMinVoltage = -10;
	ptADBoardDevices->ADBoards[0].fMaxVoltage = 10;
	ptADBoardDevices->ADBoards[0].fRange = ptADBoardDevices->ADBoards[0].fMaxVoltage-ptADBoardDevices->ADBoards[0].fMinVoltage;
	ptADBoardDevices->ADBoards[0].AD_spi = &hspi4;
	ptADBoardDevices->ADBoards[0].CONV_DO_x=CONV_DO_x;
	ptADBoardDevices->ADBoards[0].CS_DO_x = CS_DO_x;
	ptADBoardDevices->ADBoards[0].RESET_DO_x = RESET_DO_x;
	ptADBoardDevices->ADBoards[0].BUSY_INT_x = BUSY_INT_x;

	ADBoard_Reset();

	ADBoard_updateVoltage();
}


#endif
