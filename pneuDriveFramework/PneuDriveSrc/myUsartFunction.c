/*
 * myUsartFunction.cpp
 *
 *  Created on: Jun 12, 2018
 *      Author: 402072495
 */


#include <PneuDriveLL.h>
#include "myUsartFunction.h"
#include "cmsis_os.h"
#include "usart.h"
#include "string.h"
#include <stdarg.h>
#include <stdio.h>

UART_DEVICE Usart3Device;
UART_DEVICE Usart1Device;
UART_DEVICE Usart2Device;

typedef struct
{
	unsigned char header[4];
	int16_t data[UART_TX_BUF_SIZE/2];
	int16_t *pData;
} __attribute__((packed)) PRINTFBINSTRUCT;
/*serial bin format:
 * Hex: 0x5a  0x5a 0xa5  0xa5 data 	0x0D 	0x0A
 * Dec: 90    90   165   165  data  13 		10
 * char: Z    Z     	    data  \r  	\n
 */

static PRINTFBINSTRUCT printfBinStruct;


/*This file enables to use printf() to output strings to a uart:
 * 1.None blocking Tx and Rx using DMA
 * 2.Tx has Buffers without loss of transmission data
 * 3.Rx accept variable size
 *
 *
 * Usage:
 * 1.configure as below.
 * 2.For transmitting, use printf() as usual.
 * 3.For receiving, received string is automatically updated in UsartDevice->RxBuf
 *
 * Stm32CubeMx configuration:
 * Usart configuration: Tx DMA  Rx	DMA Enable Interrupt
 * file location:
 * 1. put my_UsartInit() in initialization part
 * 2. put HAL_UART_RxIdleCallback() in ISR for receiving data
 * 3. put Usart_TerminalHandler() in polling loop for processing data
 *
 */




void usart1CommandHandler()
{
	serialPort1Callback((char *)Usart1Device.RxBuf);

}
void usart3CommandHandler()
{
	//since this termination is triggered by IDLE LINE, we keep looking for end of line '\r\n'
	int len=Usart3Device.countRxLineBuf+Usart3Device.countRxBuf;

	//If overflow Clear
	if(len>UART_RX_BUF_SIZE)
	{
		memset(Usart3Device.RxLineBuf,0,UART_RX_BUF_SIZE);
		Usart3Device.pRxLineBuf=Usart3Device.RxLineBuf;
		Usart3Device.countRxLineBuf=0;
	}
	else{
		//buffer to line
		memcpy(&(Usart3Device.RxLineBuf[Usart3Device.countRxLineBuf]),Usart3Device.RxBuf,Usart3Device.countRxBuf);
		Usart3Device.pRxLineBuf+=Usart3Device.countRxBuf;
		Usart3Device.countRxLineBuf=len;

		//if end of line
		if (Usart3Device.RxLineBuf[Usart3Device.countRxLineBuf-1]=='\n')
		{
			serialReceiveCallback((char *)Usart3Device.RxLineBuf);
			memset(Usart3Device.RxLineBuf,0,len);
			Usart3Device.pRxLineBuf=Usart3Device.RxLineBuf;
			Usart3Device.countRxLineBuf=0;
		}
	}

}

void usart2CommandHandler()
{
	//since this termination is triggered by IDLE LINE, we want to make sure it has the right number of bytes
		serialPort2Callback((char *)Usart2Device.RxBuf);
}
void UartCommandHandler(UART_DEVICE *UsartDevice)
{
	if(UsartDevice == &Usart3Device)
		usart3CommandHandler();
	else if(UsartDevice == &Usart2Device)
		usart2CommandHandler();
}


/*put this function in the main.c for initilization*/
void my_UsartInit()
{

	memset(&printfBinStruct,0,sizeof(PRINTFBINSTRUCT));
	printfBinStruct.header[0]=0x5a;
	printfBinStruct.header[1]=0x5a;
	printfBinStruct.header[2]=0xa5;
	printfBinStruct.header[3]=0xa5;

	printfBinStruct.pData=printfBinStruct.data;

	 memset(&Usart3Device,0,sizeof(Usart3Device));
	 Usart3Device.huart = &huart3;
	 Usart3Device.pRxBuf = Usart3Device.RxBuf;
	 Usart3Device.pRxLineBuf=Usart3Device.RxLineBuf;
	 Usart3Device.receveBinPtr = (RECEIVEBINSTRUCT *)Usart3Device.RxBuf;
	 /*get ready for receive*/
	 HAL_UART_Receive_DMA(Usart3Device.huart, Usart3Device.RxBuf, UART_RX_BUF_SIZE-1);

	 /*Enable USART_Rx IDLE Detection to stop USART1_Rx_DMA*/
	 __HAL_UART_ENABLE_IT(Usart3Device.huart, UART_IT_IDLE);

	// memset(&Usart2Device,0,sizeof(Usart2Device));
	 Usart2Device.huart = &huart2;
	 Usart2Device.pRxBuf = Usart2Device.RxBuf;
	 Usart2Device.pRxLineBuf=Usart2Device.RxLineBuf;
	 Usart2Device.receveBinPtr = (RECEIVEBINSTRUCT *)Usart2Device.RxBuf;
	 /*get ready for receive*/
	 HAL_UART_Receive_DMA(Usart2Device.huart, Usart2Device.RxBuf, UART_RX_BUF_SIZE-1);

	 /*Enable USART_Rx IDLE Detection to stop USART1_Rx_DMA*/
	 __HAL_UART_ENABLE_IT(Usart2Device.huart, UART_IT_IDLE);

	 memset(&Usart1Device,0,sizeof(Usart1Device));
	 Usart1Device.huart = &huart1;
	 Usart1Device.pRxBuf = Usart1Device.RxBuf;
	 __HAL_UART_ENABLE_IT(&huart1,UART_IT_RXNE);

}


/*put this function in stm32f7xx_it.c as below
void USART3_IRQHandler(void)
{
  HAL_UART_RxIdleCallback(UsartDevice->huart);
  HAL_UART_IRQHandler(&huart3);
}
*/

void HAL_UART_RxIdleCallback(UART_HandleTypeDef *huart)
{
	 uint32_t tmp_flag = __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE);
	 uint32_t tmp_it_source = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE);

	 /* UART RX Idle interrupt*/
	 if((tmp_flag != RESET) && (tmp_it_source != RESET)){

		 /*Clear Idle Flag*/
		__HAL_UART_CLEAR_IDLEFLAG(huart);

		/*receive flag*/
		if(huart == &huart3)
			Usart3Device.Received = 1;
		else if(huart == &huart2)
			Usart2Device.Received = 1;

		/*We stop the DMA in the polling task, not here, since some data are still on the fly now*/

	}

}

/*Redirect printf() by implementing (weak) _write function.
 *Every printf() call would store the output string in TxBuf[], ready for Usart DMA output instead of directly output*/
int _write(int file, char *pSrc, int len)
{
	uint8_t *pDes=Usart3Device.TxBuf[Usart3Device.producerTxBufNum];

	//store the string to next buffer
	memcpy(pDes,pSrc,len);
	*(pDes+len)='\0';
	Usart3Device.countTxBuf[Usart3Device.producerTxBufNum] = len;

	//add one bufferedTxNum, recording how many buffered strings that haven't been sent
	Usart3Device.bufferedTxNum++;

	//Try to send just buffered string if this is the only one
	if(Usart3Device.bufferedTxNum == 1){
		HAL_UART_Transmit_DMA(Usart3Device.huart,pDes,Usart3Device.countTxBuf[Usart3Device.producerTxBufNum]);
		Usart3Device.TxStart = micros();
	}
	else{
	//TO DO, There is a bug here, when the builtInPWMFrequency is changed, the Usart3Devices would somehow suddenly lost the configurations
		Usart3Device.bufferedTxNum=Usart3Device.bufferedTxNum;
	}
	//move producerTxBufNum forward
	Usart3Device.producerTxBufNum++;
	Usart3Device.producerTxBufNum%=UART_TX_BUF_NUM;

	//Buffered term full, wait for consumer to reduce producerTxBufNum
//	while(Usart3Device.bufferedTxNum > (UART_TX_BUF_NUM-2)){
//		//Danger! May block the main program continuously !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//		//This waiting mechanism is to take care of the high frequency output within a short period during the Ethercat Initialization
//		//If the producer is always quicker than consumer, for example a high frequency output ,this function would block the program permanently
//	};
	return len;
}

//input: array pointer, and data number
//function: add header and tail, send into buffer
void printfBinFlush()
{
int len=(printfBinStruct.pData-printfBinStruct.data)*sizeof(int16_t);
char *pChar=(char *)(printfBinStruct.pData);
*(pChar)++='\r';
*(pChar)++='\n';
len= len+6;
printfBinStruct.pData = printfBinStruct.data;
_write(0,(char *)(&printfBinStruct),len);
}

void printfBinPush(int16_t dat)
{
*(printfBinStruct.pData)++=dat;
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	//printf("RxCplCall_Test\r\n");
}

/*this function would overwrite HAL's weak HAL_UART_TxCpltCallback*/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	 /*update information*/
	 Usart3Device.TxEnd = micros();
	 Usart3Device.lastTxTime = Usart3Device.TxEnd - Usart3Device.TxStart;
	 Usart3Device.lastTxCount = Usart3Device.countTxBuf[Usart3Device.consumerTxBufNum];

	/*One consumption done. move consumer forward*/
	Usart3Device.consumerTxBufNum++;
	Usart3Device.consumerTxBufNum%=UART_TX_BUF_NUM;

	/*reduce one bufferedTxNum*/
	 Usart3Device.bufferedTxNum--;

	/*If it is still positive, go on consume next*/
	if(Usart3Device.bufferedTxNum>0){
		Usart3Device.TxStart = micros();
		uint8_t *px = &Usart3Device.TxBuf[Usart3Device.consumerTxBufNum][0];
		HAL_UART_Transmit_DMA(Usart3Device.huart,px,Usart3Device.countTxBuf[Usart3Device.consumerTxBufNum]);
	}
}


void myUsart1IRQ()
{

 	if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE)!=RESET)   //receive interrupt
	{
		*(Usart1Device.pRxBuf)=(uint8_t)(huart1.Instance->RDR & (uint8_t)0x00FF);  //read and clear flag
		if(*(Usart1Device.pRxBuf)==0x0a) // if current char is 0x0a, take care. If not, go on receiving.
		{
			if((Usart1Device.pRxBuf)!=(Usart1Device.RxBuf)) // if '0x0a' is not in the beginning
			{
				if(*(Usart1Device.pRxBuf-1)==0x0d)// if previous char is 0x0d, legal end.
			    {
					Usart1Device.Received = 1;
				}
				else//if previous char is not 0x0d, illegal end. Reset Buffer.
				{
					memset(Usart1Device.RxBuf,0,UART_RX_BUF_SIZE);
					Usart1Device.pRxBuf=Usart1Device.RxBuf;
				}

			}//if '0x0a' is received in the beginning. do nothing.
		}
		else Usart1Device.pRxBuf++;
	}

}


void UsartDMAIdleHandler(UART_DEVICE *UsartDevice)
{
	/********************************Usart DMA reception****************************/
		/*Only process with idle receiving detection*/
		if (UsartDevice->Received == 1) {

			/* Stop UART DMA Rx request if ongoing */
			if ((UsartDevice->huart->RxState == HAL_UART_STATE_BUSY_RX) && (HAL_IS_BIT_SET(UsartDevice->huart->Instance->CR3, USART_CR3_DMAR))) {
				CLEAR_BIT(UsartDevice->huart->Instance->CR3, USART_CR3_DMAR);

				/* Abort the UART DMA Rx channel */
				if (UsartDevice->huart->hdmarx != NULL) {
					/* Determine how many items of data have been received */
					UsartDevice->countRxBuf = UsartDevice->huart->RxXferSize - __HAL_DMA_GET_COUNTER(UsartDevice->huart->hdmarx);
					UsartDevice->huart->RxXferCount = 0;

					/*Abort DMA*/
					HAL_DMA_Abort(UsartDevice->huart->hdmarx);
				}

				/* Disable RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts */
				CLEAR_BIT(UsartDevice->huart->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));
				CLEAR_BIT(UsartDevice->huart->Instance->CR3, USART_CR3_EIE);

				/* At end of Rx process, restore huart->RxState to Ready */
				UsartDevice->huart->RxState = HAL_UART_STATE_READY;
			}

			/*Process commands*/
			UartCommandHandler(UsartDevice);

			/*clear Recived flag*/
			UsartDevice->Received = 0;

			/*clear buffer*/
			memset(UsartDevice->szCmd, 0, sizeof(UsartDevice->szCmd));
			memset(UsartDevice->RxBuf, 0, UART_RX_BUF_SIZE);
			UsartDevice->countRxBuf = 0;
		}
		//Always try to start a new reception
		HAL_UART_Receive_DMA(UsartDevice->huart, UsartDevice->RxBuf, UART_RX_BUF_SIZE - 1);
}

/*Put this function in a loop for polling*/
void Usart_TerminalHandler()
{
	UsartDMAIdleHandler(&Usart3Device);
	UsartDMAIdleHandler(&Usart2Device);

	/********************************Usart 1 Int reception****************************/
	if(Usart1Device.Received == 1)
	{
		usart1CommandHandler();
		memset(Usart1Device.RxBuf,0,UART_RX_BUF_SIZE);
		Usart1Device.pRxBuf=Usart1Device.RxBuf;
		Usart1Device.Received = 0;

	}

}

