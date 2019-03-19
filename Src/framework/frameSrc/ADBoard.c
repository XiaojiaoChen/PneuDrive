
#include <ADBoard.h>

#if (ADBOARD_NUM>0)


#include "PneuFunctions.h"
#include "spi.h"


/*****Private define*************/


#define AD_RESET_GPIO_Port DO_19_GPIO_Port
#define AD_RESET_Pin	DO_19_Pin

#define AD_RESET_LOW()  (AD_RESET_GPIO_Port->BSRR=(uint32_t)AD_RESET_Pin<<16U)
#define AD_RESET_HIGH() (AD_RESET_GPIO_Port->BSRR=AD_RESET_Pin)

AD_BOARDS ADBoardDevices;
AD_BOARDS *ptADBoardDevices=&ADBoardDevices;


/******************************AS5048 Device**********************************/
static void AD_CS_LOW(AD7616_DEVICE *ptADDevice)
{
	ptADDevice->CS_Port->BSRR=(uint32_t)ptADDevice->CS_Pin<<16U;
}

static void AD_CS_HIGH(AD7616_DEVICE *ptADDevice)
{
	ptADDevice->CS_Port->BSRR=(uint32_t)ptADDevice->CS_Pin;
}
static void AD_CONV_LOW(AD7616_DEVICE *ptADDevice)
{
	ptADDevice->CONV_Port->BSRR=(uint32_t)ptADDevice->CONV_Pin<<16U;
}

static void AD_CONV_HIGH(AD7616_DEVICE *ptADDevice)
{
	ptADDevice->CONV_Port->BSRR=(uint32_t)ptADDevice->CONV_Pin;
}

static void AD7616_getVoltage(AD7616_DEVICE *ptADDev)
{
	ptADDev->chosen = 1;
	AD_CONV_HIGH(ptADDev);
	delay_ns(200);
	AD_CONV_LOW(ptADDev);
	ptADDev->ucDataFlag = 0;
}
static uint16_t ulDummyWord[16]={0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF};

int ADBoard_BUSYCallback(uint16_t GPIO_Pin)
{
	int ret=1;
	for(int i=0;i<ADBOARD_NUM;i++)
	{
		AD7616_DEVICE *ptADDev=&(ptADBoardDevices->ADBoards[i]);
		if (GPIO_Pin == ptADDev->BUSY_Pin)
			{
				ptADBoardDevices->curADBoardNum=i;
				AD_CS_LOW(ptADDev);
				HAL_SPI_TransmitReceive_DMA(ptADDev->AD_spi,(uint8_t *)ulDummyWord,(uint8_t *)ptADDev->uChannel, AD7616_CHANNELNUM);
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
					for(int j=0;j<AD7616_CHANNELNUM;j++)
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
					}
					ret=0;
					break;
				}

		}
	return ret;
}

void ADBoard_Reset()
{
	AD_RESET_HIGH();
	delay_us(5);
	AD_RESET_LOW();
	delay_us(5);
	AD_RESET_HIGH();
}

void ADBoard_updateVoltage()
{
	ptADBoardDevices->ucDataFlag=0;
	ptADBoardDevices->curADBoardNum = 0;
	AD7616_getVoltage(&(ptADBoardDevices->ADBoards[ptADBoardDevices->curADBoardNum]));
}

float AnalogRead_External(uint16_t num) {
	if (0<=num && num<EXTERNAL_ANA_IN_NUM)
	{
		uint16_t boardnum=num/AD7616_CHANNELNUM;
		uint16_t channelnum=num%AD7616_CHANNELNUM;
		return ptADBoardDevices->ADBoards[boardnum].fChannel[channelnum];
	}
	else
		return 0;
}

void ADBoard_Init()
{

	ptADBoardDevices->curADBoardNum=-1;
	ptADBoardDevices->ucDataFlag = 0;

	ptADBoardDevices->ADBoards[0].fMinVoltage = -5;
	ptADBoardDevices->ADBoards[0].fMaxVoltage = 5;
	ptADBoardDevices->ADBoards[0].fRange = 10;
	ptADBoardDevices->ADBoards[0].AD_spi = &hspi3;
	ptADBoardDevices->ADBoards[0].CONV_Port = DO_18_GPIO_Port;
	ptADBoardDevices->ADBoards[0].CONV_Pin = DO_18_Pin;
	ptADBoardDevices->ADBoards[0].CS_Port = SPI3_CS_GPIO_Port;
	ptADBoardDevices->ADBoards[0].CS_Pin = SPI3_CS_Pin;
	ptADBoardDevices->ADBoards[0].BUSY_Port = E7_GPIO_Port;
	ptADBoardDevices->ADBoards[0].BUSY_Pin = E7_Pin;

	ptADBoardDevices->ADBoards[1].fMinVoltage = -5;
	ptADBoardDevices->ADBoards[1].fMaxVoltage = 5;
	ptADBoardDevices->ADBoards[1].fRange = 10;
	ptADBoardDevices->ADBoards[1].AD_spi = &hspi1;
	ptADBoardDevices->ADBoards[1].CONV_Port = DO_17_GPIO_Port;
	ptADBoardDevices->ADBoards[1].CONV_Pin = DO_17_Pin;
	ptADBoardDevices->ADBoards[1].CS_Port = SPI1_CS_GPIO_Port;
	ptADBoardDevices->ADBoards[1].CS_Pin = SPI1_CS_Pin;
	ptADBoardDevices->ADBoards[1].BUSY_Port = E6_GPIO_Port;
	ptADBoardDevices->ADBoards[1].BUSY_Pin = E6_Pin;
	ptADBoardDevices->ADBoards[1].chosen = 0;
	ptADBoardDevices->ADBoards[1].ucDataFlag = 0;





	ADBoard_Reset();

	ADBoard_updateVoltage();
}


#endif
