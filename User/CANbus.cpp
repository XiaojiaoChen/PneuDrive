/*
 * myCanbus.c
 *
 *  Created on: 2020年1月7日
 *      Author: Xiaojiao Chen
 *       Email: yii402072495@gmail.com
 */

#include <CANbus.h>
#include "softArm.h"
struct CANBUS_HANDLE canbus;
extern SOFT_ARM softArm;

#include <inttypes.h>
static HAL_StatusTypeDef my_HAL_CAN_GetRxMessage(CAN_HandleTypeDef *hcan, uint32_t RxFifo, CAN_RxHeaderTypeDef *pHeader, uint8_t aData[]);
/***************************     Can bus        ***************************/
/***************************                     ***************************/
/**************************************************************************/
/**************************************************************************/
/**************************************************************************/
/**************************************************************************/
/**************************************************************************/
void canConfig(){
	canbus.CanHandle=hcan1;
	CAN_FilterTypeDef  sFilterConfig;
	/*##-2- Configure the CAN Filter ###########################################*/
	/*For single CAN instance(14 dedicated filter banks)*/
	for(int i=0;i<14;i++){
		sFilterConfig.FilterBank = i;
		sFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST;
		sFilterConfig.FilterScale = CAN_FILTERSCALE_16BIT;
		sFilterConfig.FilterIdHigh = (uint16_t)((uint16_t)(4*i))<<5;
		sFilterConfig.FilterIdLow = (uint16_t)((uint16_t)(4*i+1))<<5;
		sFilterConfig.FilterMaskIdHigh = (uint16_t)((uint16_t)(4*i+2))<<5;
		sFilterConfig.FilterMaskIdLow = (uint16_t)((uint16_t)(4*i+3))<<5;
		sFilterConfig.FilterFIFOAssignment = i%2;//CAN_RX_FIFO0=0; CAN_RX_FIFO1=1
		sFilterConfig.FilterActivation = ENABLE;
		sFilterConfig.SlaveStartFilterBank = 14;

//		sFilterConfig.FilterBank = 0;
//		sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
//		sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
//		sFilterConfig.FilterIdHigh = 0x0000;
//		sFilterConfig.FilterIdLow = 0x0000;
//		sFilterConfig.FilterMaskIdHigh = 0x0000;
//		sFilterConfig.FilterMaskIdLow = 0x0000;
//		sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
//		sFilterConfig.FilterActivation = ENABLE;
//		sFilterConfig.SlaveStartFilterBank = 14;

		if (HAL_CAN_ConfigFilter(&canbus.CanHandle, &sFilterConfig) != HAL_OK)
		{
			/* Filter configuration Error */
			Error_Handler();
		}
	}


	/*##-3- Start the CAN peripheral ###########################################*/
	if (HAL_CAN_Start(&canbus.CanHandle) != HAL_OK)
	{
		/* Start Error */
		Error_Handler();
	}

	/*##-4- Activate CAN RX notification #######################################*/
	if (HAL_CAN_ActivateNotification(&canbus.CanHandle, CAN_IT_RX_FIFO0_MSG_PENDING|CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK)
	{
		/* Notification Error */
		Error_Handler();
	}


}
/*No need to use this right now, left for sending commands to nodes in the future*/
void canSend()
{
	/*##-Configure Transmission Packet #####################################*/
	canbus.TxHeader.StdId = 0x400;
	canbus.TxHeader.ExtId = 0x01;
	canbus.TxHeader.RTR = CAN_RTR_DATA;
	canbus.TxHeader.IDE = CAN_ID_STD;
	canbus.TxHeader.TransmitGlobalTime = DISABLE;

	canbus.TxData=(uint8_t *)(softArm.canBusCommand);
	canbus.TxHeader.DLC = sizeof(softArm.canBusCommand);

	/* Start the Transmission process */
	if (HAL_CAN_AddTxMessage(&canbus.CanHandle, &canbus.TxHeader, canbus.TxData, &canbus.TxMailbox) != HAL_OK)
	{
	  /* Transmission request Error */
	  Error_Handler();
	}

	//printf("Send Can Tx %d\r\n",canbus.TxHeader.StdId);
}



/*Automatically called when can bus RxFiFo get message. Move the received message to corresponding buffer according to the RxHeader ID*/
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  /* Get RX message */

  if (my_HAL_CAN_GetRxMessage(&canbus.CanHandle, CAN_RX_FIFO0, &canbus.RxHeader, canbus.RxData) != HAL_OK)
  {
    /* Reception Error */
    Error_Handler();
  }
  //printf("Get Can Rx %d\r\n",canbus.RxHeader.StdId);
  //printf("ID[%d]: %x %x %x %x %x %x %x %x\r\n", canbus.RxHeader.StdId, canbus.RxData[0], canbus.RxData[1], canbus.RxData[2], canbus.RxData[3], canbus.RxData[4], canbus.RxData[5], canbus.RxData[6], canbus.RxData[7]);
  //printf("FIFO0: ID[%d]\r\n", canbus.RxHeader.StdId, canbus.RxData);
}

/*Automatically called when can bus RxFiFo get message. Move the received message to corresponding buffer according to the RxHeader ID*/
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  /* Get RX message */


  if (my_HAL_CAN_GetRxMessage(&canbus.CanHandle, CAN_RX_FIFO1, &canbus.RxHeader, canbus.RxData) != HAL_OK)
  {
    /* Reception Error */
    Error_Handler();
  }
  //printf("FIFO1: ID[%d]\r\n", canbus.RxHeader.StdId);

  //printf("Get Can Rx %d\r\n",canbus.RxHeader.StdId);
}

static HAL_StatusTypeDef my_HAL_CAN_GetRxMessage(CAN_HandleTypeDef *hcan, uint32_t RxFifo, CAN_RxHeaderTypeDef *pHeader, uint8_t aData[])
{
  HAL_CAN_StateTypeDef state = hcan->State;

  assert_param(IS_CAN_RX_FIFO(RxFifo));

  if ((state == HAL_CAN_STATE_READY) ||
      (state == HAL_CAN_STATE_LISTENING))
  {
    /* Check the Rx FIFO */
    if (RxFifo == CAN_RX_FIFO0) /* Rx element is assigned to Rx FIFO 0 */
    {
      /* Check that the Rx FIFO 0 is not empty */
      if ((hcan->Instance->RF0R & CAN_RF0R_FMP0) == 0U)
      {
        /* Update error code */
        hcan->ErrorCode |= HAL_CAN_ERROR_PARAM;

        return HAL_ERROR;
      }
    }
    else /* Rx element is assigned to Rx FIFO 1 */
    {
      /* Check that the Rx FIFO 1 is not empty */
      if ((hcan->Instance->RF1R & CAN_RF1R_FMP1) == 0U)
      {
        /* Update error code */
        hcan->ErrorCode |= HAL_CAN_ERROR_PARAM;

        return HAL_ERROR;
      }
    }

    /* Get the header */
    pHeader->IDE = CAN_RI0R_IDE & hcan->Instance->sFIFOMailBox[RxFifo].RIR;
    if (pHeader->IDE == CAN_ID_STD)
    {
      pHeader->StdId = (CAN_RI0R_STID & hcan->Instance->sFIFOMailBox[RxFifo].RIR) >> CAN_TI0R_STID_Pos;
    }
    else
    {
      pHeader->ExtId = ((CAN_RI0R_EXID | CAN_RI0R_STID) & hcan->Instance->sFIFOMailBox[RxFifo].RIR) >> CAN_RI0R_EXID_Pos;
    }
    pHeader->RTR = (CAN_RI0R_RTR & hcan->Instance->sFIFOMailBox[RxFifo].RIR);
    pHeader->DLC = (CAN_RDT0R_DLC & hcan->Instance->sFIFOMailBox[RxFifo].RDTR) >> CAN_RDT0R_DLC_Pos;
    pHeader->FilterMatchIndex = (CAN_RDT0R_FMI & hcan->Instance->sFIFOMailBox[RxFifo].RDTR) >> CAN_RDT0R_FMI_Pos;
    pHeader->Timestamp = (CAN_RDT0R_TIME & hcan->Instance->sFIFOMailBox[RxFifo].RDTR) >> CAN_RDT0R_TIME_Pos;



    /**********************  added  ******************************************/
	int i = pHeader->StdId/6;
    int j = pHeader->StdId%6;
    softArm.actuatorOnline[j][j]=10;
    aData=(uint8_t *)(&softArm.sensorData[i][j]);
    /****************************************************************/

    /* Get the data */
    aData[0] = (uint8_t)((CAN_RDL0R_DATA0 & hcan->Instance->sFIFOMailBox[RxFifo].RDLR) >> CAN_RDL0R_DATA0_Pos);
    aData[1] = (uint8_t)((CAN_RDL0R_DATA1 & hcan->Instance->sFIFOMailBox[RxFifo].RDLR) >> CAN_RDL0R_DATA1_Pos);
    aData[2] = (uint8_t)((CAN_RDL0R_DATA2 & hcan->Instance->sFIFOMailBox[RxFifo].RDLR) >> CAN_RDL0R_DATA2_Pos);
    aData[3] = (uint8_t)((CAN_RDL0R_DATA3 & hcan->Instance->sFIFOMailBox[RxFifo].RDLR) >> CAN_RDL0R_DATA3_Pos);
    aData[4] = (uint8_t)((CAN_RDH0R_DATA4 & hcan->Instance->sFIFOMailBox[RxFifo].RDHR) >> CAN_RDH0R_DATA4_Pos);
    aData[5] = (uint8_t)((CAN_RDH0R_DATA5 & hcan->Instance->sFIFOMailBox[RxFifo].RDHR) >> CAN_RDH0R_DATA5_Pos);
    aData[6] = (uint8_t)((CAN_RDH0R_DATA6 & hcan->Instance->sFIFOMailBox[RxFifo].RDHR) >> CAN_RDH0R_DATA6_Pos);
    aData[7] = (uint8_t)((CAN_RDH0R_DATA7 & hcan->Instance->sFIFOMailBox[RxFifo].RDHR) >> CAN_RDH0R_DATA7_Pos);

    /* Release the FIFO */
    if (RxFifo == CAN_RX_FIFO0) /* Rx element is assigned to Rx FIFO 0 */
    {
      /* Release RX FIFO 0 */
      SET_BIT(hcan->Instance->RF0R, CAN_RF0R_RFOM0);
    }
    else /* Rx element is assigned to Rx FIFO 1 */
    {
      /* Release RX FIFO 1 */
      SET_BIT(hcan->Instance->RF1R, CAN_RF1R_RFOM1);
    }

    /* Return function status */
    //printf("sensor[%d]: %hu\r\n", pHeader->StdId, softArm.sensorData[i][j].pressure);

    return HAL_OK;
  }
  else
  {
    /* Update error code */
    hcan->ErrorCode |= HAL_CAN_ERROR_NOT_INITIALIZED;

    return HAL_ERROR;
  }
}

