/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "PneuDriveLL.h"

#include "builtInAnalog.h"
#include "PWMBoardSPI.h"
#include "ADBoard.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId sendTaskHandle;
uint32_t sendTaskBuffer[ 4096 ];
osStaticThreadDef_t sendTaskControlBlock;
osThreadId controlTaskHandle;
uint32_t controlTaskBuffer[ 4096 ];
osStaticThreadDef_t controlTaskControlBlock;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void sendTaskFunc(void const * argument);
void controlTaskFunc(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];
  
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}                   
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of sendTask */
  osThreadStaticDef(sendTask, sendTaskFunc, osPriorityBelowNormal, 0, 4096, sendTaskBuffer, &sendTaskControlBlock);
  sendTaskHandle = osThreadCreate(osThread(sendTask), NULL);

  /* definition and creation of controlTask */
  osThreadStaticDef(controlTask, controlTaskFunc, osPriorityAboveNormal, 0, 4096, controlTaskBuffer, &controlTaskControlBlock);
  controlTaskHandle = osThreadCreate(osThread(controlTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_sendTaskFunc */
/**
  * @brief  Function implementing the sendTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_sendTaskFunc */
void sendTaskFunc(void const * argument)
{
  /* USER CODE BEGIN sendTaskFunc */
	TickType_t xLastWakeTime=xTaskGetTickCount();
	TickType_t sendTaskPeriod=pdMS_TO_TICKS(1);
	static int32_t sendTick=0;
	extern int32_t globalPeriodSendLoop;
  /* Infinite loop */
  for(;;)
  {
	  if(++sendTick>=globalPeriodSendLoop)
	  {
		  sendTick=0;
		  serialDisplay();
	  }
	  vTaskDelayUntil(&xLastWakeTime,sendTaskPeriod);
  }
  /* USER CODE END sendTaskFunc */
}

/* USER CODE BEGIN Header_controlTaskFunc */
/**
* @brief Function implementing the controlTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_controlTaskFunc */
void controlTaskFunc(void const * argument)
{
  /* USER CODE BEGIN controlTaskFunc */
	TickType_t xLastWakeTime=xTaskGetTickCount();
	TickType_t controlTaskPeriod=pdMS_TO_TICKS(1);
	static int32_t loopTick=0;
	extern int32_t globalPeriodControlLoop;
  /* Infinite loop */
	 for(;;)
	  {

		//	AnaBuiltInStart();
	#if (ADBOARD_NUM>0)
			ADBoard_updateVoltage();
	#endif
			//terminal command process
			Usart_TerminalHandler();

			//User loop function
	 if(++loopTick>=globalPeriodControlLoop)
	 {
		loopTick=0;
		loop();
	 }

	#if (PWMBOARDSPI_NUM>0)
			PWMBoardSPI_flushDutyAll();
	#endif
			vTaskDelayUntil(&xLastWakeTime,controlTaskPeriod);

	  }
  /* USER CODE END controlTaskFunc */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
