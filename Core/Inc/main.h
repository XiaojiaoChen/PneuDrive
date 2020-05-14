/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DO_12_Pin GPIO_PIN_3
#define DO_12_GPIO_Port GPIOE
#define DO_13_Pin GPIO_PIN_4
#define DO_13_GPIO_Port GPIOE
#define PWM_15_Pin GPIO_PIN_5
#define PWM_15_GPIO_Port GPIOE
#define PWM_16_Pin GPIO_PIN_6
#define PWM_16_GPIO_Port GPIOE
#define DO_0_Pin GPIO_PIN_3
#define DO_0_GPIO_Port GPIOF
#define DO_1_Pin GPIO_PIN_4
#define DO_1_GPIO_Port GPIOF
#define DO_2_Pin GPIO_PIN_5
#define DO_2_GPIO_Port GPIOF
#define PWM_17_Pin GPIO_PIN_6
#define PWM_17_GPIO_Port GPIOF
#define PWM_18_Pin GPIO_PIN_7
#define PWM_18_GPIO_Port GPIOF
#define DO_3_Pin GPIO_PIN_8
#define DO_3_GPIO_Port GPIOF
#define PWM_19_Pin GPIO_PIN_9
#define PWM_19_GPIO_Port GPIOF
#define DO_4_Pin GPIO_PIN_10
#define DO_4_GPIO_Port GPIOF
#define A3_Pin GPIO_PIN_0
#define A3_GPIO_Port GPIOC
#define A4_Pin GPIO_PIN_2
#define A4_GPIO_Port GPIOC
#define A5_Pin GPIO_PIN_3
#define A5_GPIO_Port GPIOC
#define A0_Pin GPIO_PIN_3
#define A0_GPIO_Port GPIOA
#define A1_Pin GPIO_PIN_4
#define A1_GPIO_Port GPIOA
#define A2_Pin GPIO_PIN_1
#define A2_GPIO_Port GPIOB
#define DO_5_Pin GPIO_PIN_11
#define DO_5_GPIO_Port GPIOF
#define DO_6_Pin GPIO_PIN_12
#define DO_6_GPIO_Port GPIOF
#define DO_7_Pin GPIO_PIN_13
#define DO_7_GPIO_Port GPIOF
#define DO_8_Pin GPIO_PIN_14
#define DO_8_GPIO_Port GPIOF
#define DO_9_Pin GPIO_PIN_15
#define DO_9_GPIO_Port GPIOF
#define E0_Pin GPIO_PIN_0
#define E0_GPIO_Port GPIOG
#define E0_EXTI_IRQn EXTI0_IRQn
#define E1_Pin GPIO_PIN_1
#define E1_GPIO_Port GPIOG
#define E1_EXTI_IRQn EXTI1_IRQn
#define DO_14_Pin GPIO_PIN_7
#define DO_14_GPIO_Port GPIOE
#define DO_15_Pin GPIO_PIN_8
#define DO_15_GPIO_Port GPIOE
#define PWM_0_Pin GPIO_PIN_9
#define PWM_0_GPIO_Port GPIOE
#define PWM_1_Pin GPIO_PIN_11
#define PWM_1_GPIO_Port GPIOE
#define PWM_3_Pin GPIO_PIN_10
#define PWM_3_GPIO_Port GPIOB
#define PWM_4_Pin GPIO_PIN_11
#define PWM_4_GPIO_Port GPIOB
#define PWM_9_Pin GPIO_PIN_12
#define PWM_9_GPIO_Port GPIOD
#define PWM_10_Pin GPIO_PIN_13
#define PWM_10_GPIO_Port GPIOD
#define PWM_11_Pin GPIO_PIN_14
#define PWM_11_GPIO_Port GPIOD
#define PWM_12_Pin GPIO_PIN_15
#define PWM_12_GPIO_Port GPIOD
#define E2_Pin GPIO_PIN_2
#define E2_GPIO_Port GPIOG
#define E2_EXTI_IRQn EXTI2_IRQn
#define E3_Pin GPIO_PIN_3
#define E3_GPIO_Port GPIOG
#define E3_EXTI_IRQn EXTI3_IRQn
#define E4_Pin GPIO_PIN_4
#define E4_GPIO_Port GPIOG
#define E4_EXTI_IRQn EXTI4_IRQn
#define E5_Pin GPIO_PIN_5
#define E5_GPIO_Port GPIOG
#define E5_EXTI_IRQn EXTI9_5_IRQn
#define PWM_13_Pin GPIO_PIN_6
#define PWM_13_GPIO_Port GPIOC
#define PWM_14_Pin GPIO_PIN_7
#define PWM_14_GPIO_Port GPIOC
#define PWM_7_Pin GPIO_PIN_8
#define PWM_7_GPIO_Port GPIOC
#define PWM_8_Pin GPIO_PIN_9
#define PWM_8_GPIO_Port GPIOC
#define PWM_2_Pin GPIO_PIN_15
#define PWM_2_GPIO_Port GPIOA
#define DI_0_Pin GPIO_PIN_2
#define DI_0_GPIO_Port GPIOD
#define DI_1_Pin GPIO_PIN_3
#define DI_1_GPIO_Port GPIOD
#define DI_2_Pin GPIO_PIN_4
#define DI_2_GPIO_Port GPIOD
#define DI_3_Pin GPIO_PIN_7
#define DI_3_GPIO_Port GPIOD
#define PWM_5_Pin GPIO_PIN_4
#define PWM_5_GPIO_Port GPIOB
#define PWM_6_Pin GPIO_PIN_5
#define PWM_6_GPIO_Port GPIOB
#define DO_10_Pin GPIO_PIN_0
#define DO_10_GPIO_Port GPIOE
#define DO_11_Pin GPIO_PIN_1
#define DO_11_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
