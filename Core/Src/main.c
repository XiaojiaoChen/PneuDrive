/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "adc.h"
#include "can.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "PneuDriveLL.h"
#include "myUsartFunction.h"
#include "ADBoard.h"
#include "DABoard.h"
#include "PWMBoardI2C.h"
#include "PWMBoardSPI.h"
#include "builtInAnalog.h"
#include "builtInPWM.h"
#include "builtInDigital.h"
#include "spiSlave.h"
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM7_Init();
  MX_TIM8_Init();
  MX_TIM9_Init();
  MX_TIM10_Init();
  MX_TIM11_Init();
  MX_TIM14_Init();
  MX_SPI1_Init();
  MX_SPI3_Init();
  MX_SPI4_Init();
  MX_SPI6_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  MX_CAN1_Init();
  /* USER CODE BEGIN 2 */
	/*******************************Usart1 and Usart3**************************/
	my_UsartInit();

	/*******************************Analog init***************************/
	//AnaBuiltInStart();

#if (ADBOARD_NUM>0)
	ADBoard_Init();
#endif

	/******************************  PWM  init***************************/
	PWMBuiltIn_init();

#if (PWMBOARDSPI_NUM>0)
	PWMBoardSPI_init();
#endif

#if (PWMBOARDI2C_NUM>0)
	PWMBoardI2C_init();
#endif

	/******************************* setup() function**************************/
	setup();
  /* USER CODE END 2 */

  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init(); 
  /* Start scheduler */
  osKernelStart();
 
  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode 
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART2
                              |RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_I2C1;
  PeriphClkInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInitStruct.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/*-----------------------Call back functions----------------------*/
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{

#if (ADBOARD_NUM>0)
	ADBoard_SPICallback(hspi);
#endif
	slaveSPITxRxCpltCallback(hspi);

}
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
	slaveSPIErrorCallback(hspi);

}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	int ret = 1;
	static int INTChannel=0;
	if(GPIO_Pin==E0_Pin){
		INTChannel=0;
	}
	else if(GPIO_Pin==E1_Pin){
		INTChannel=1;
	}else if(GPIO_Pin==E2_Pin){
		INTChannel=2;
	}else if(GPIO_Pin==E3_Pin){
		INTChannel=3;
	}else if(GPIO_Pin==E4_Pin){
		INTChannel=4;
	}
	else if(GPIO_Pin==E5_Pin){
		INTChannel=5;
	}
#if (ADBOARD_NUM>0)
	ret = ADBoard_BUSYCallback(INTChannel);
#endif
	if (ret)
		interruptCallback(INTChannel);
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c){
	uint32_t SDA_PIN = GPIO_PIN_0;
	uint32_t SCL_PIN = GPIO_PIN_1;
	GPIO_TypeDef *SDA_PORT=GPIOF;
	GPIO_TypeDef *SCL_PORT=GPIOF;
	GPIO_InitTypeDef GPIO_InitStructure;
	hi2c->Instance->CR1 &= ~I2C_CR1_PE;
	GPIO_InitStructure.Mode         = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStructure.Pull         = GPIO_PULLUP;
	GPIO_InitStructure.Speed        = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pin          = SCL_PIN;
	HAL_GPIO_Init(SCL_PORT, &GPIO_InitStructure);
	HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_SET);
	GPIO_InitStructure.Pin          = SDA_PIN;
	HAL_GPIO_Init(SDA_PORT, &GPIO_InitStructure);
	HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_SET);


	GPIO_InitStructure.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStructure.Alternate = GPIO_AF4_I2C2;
	GPIO_InitStructure.Pin          = SCL_PIN;
	HAL_GPIO_Init(SCL_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.Pin          = SDA_PIN;
	HAL_GPIO_Init(SDA_PORT, &GPIO_InitStructure);
//	hi2c->Instance->CR1 |= I2C_CR1_SWRST;
//	asm("nop");
//	hi2c->Instance->CR1 &= ~I2C_CR1_SWRST;
//	asm("nop");
	hi2c->Instance->CR1 |= I2C_CR1_PE;
	HAL_I2C_Init(hi2c);
}
/* USER CODE END 4 */

 /**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM5 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM5) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
