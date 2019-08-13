/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#define DO_6_Pin GPIO_PIN_3
#define DO_6_GPIO_Port GPIOE
#define DO_1_Pin GPIO_PIN_4
#define DO_1_GPIO_Port GPIOE
#define PWM_18_Pin GPIO_PIN_5
#define PWM_18_GPIO_Port GPIOE
#define PWM_19_Pin GPIO_PIN_6
#define PWM_19_GPIO_Port GPIOE
#define DO_2_Pin GPIO_PIN_13
#define DO_2_GPIO_Port GPIOC
#define DO_5_Pin GPIO_PIN_2
#define DO_5_GPIO_Port GPIOF
#define DO_0_Pin GPIO_PIN_3
#define DO_0_GPIO_Port GPIOF
#define DO_7_Pin GPIO_PIN_4
#define DO_7_GPIO_Port GPIOF
#define DO_8_Pin GPIO_PIN_5
#define DO_8_GPIO_Port GPIOF
#define PWM_20_Pin GPIO_PIN_6
#define PWM_20_GPIO_Port GPIOF
#define PWM_21_Pin GPIO_PIN_7
#define PWM_21_GPIO_Port GPIOF
#define PWM_24_Pin GPIO_PIN_8
#define PWM_24_GPIO_Port GPIOF
#define PWM25_Pin GPIO_PIN_9
#define PWM25_GPIO_Port GPIOF
#define DO_9_Pin GPIO_PIN_10
#define DO_9_GPIO_Port GPIOF
#define A10_Pin GPIO_PIN_0
#define A10_GPIO_Port GPIOC
#define A11_Pin GPIO_PIN_1
#define A11_GPIO_Port GPIOC
#define A12_Pin GPIO_PIN_2
#define A12_GPIO_Port GPIOC
#define A13_Pin GPIO_PIN_3
#define A13_GPIO_Port GPIOC
#define A0_Pin GPIO_PIN_0
#define A0_GPIO_Port GPIOA
#define A1_Pin GPIO_PIN_1
#define A1_GPIO_Port GPIOA
#define A2_Pin GPIO_PIN_2
#define A2_GPIO_Port GPIOA
#define A3_Pin GPIO_PIN_3
#define A3_GPIO_Port GPIOA
#define A4_Pin GPIO_PIN_4
#define A4_GPIO_Port GPIOA
#define A5_Pin GPIO_PIN_5
#define A5_GPIO_Port GPIOA
#define A6_Pin GPIO_PIN_6
#define A6_GPIO_Port GPIOA
#define A7_Pin GPIO_PIN_7
#define A7_GPIO_Port GPIOA
#define A14_Pin GPIO_PIN_4
#define A14_GPIO_Port GPIOC
#define A15_Pin GPIO_PIN_5
#define A15_GPIO_Port GPIOC
#define A8_Pin GPIO_PIN_0
#define A8_GPIO_Port GPIOB
#define A9_Pin GPIO_PIN_1
#define A9_GPIO_Port GPIOB
#define DI_0_Pin GPIO_PIN_11
#define DI_0_GPIO_Port GPIOF
#define DI_1_Pin GPIO_PIN_12
#define DI_1_GPIO_Port GPIOF
#define DI_2_Pin GPIO_PIN_13
#define DI_2_GPIO_Port GPIOF
#define DO_12_Pin GPIO_PIN_14
#define DO_12_GPIO_Port GPIOF
#define DI_3_Pin GPIO_PIN_15
#define DI_3_GPIO_Port GPIOF
#define E0_Pin GPIO_PIN_0
#define E0_GPIO_Port GPIOG
#define E0_EXTI_IRQn EXTI0_IRQn
#define E1_Pin GPIO_PIN_1
#define E1_GPIO_Port GPIOG
#define E1_EXTI_IRQn EXTI1_IRQn
#define DO_13_Pin GPIO_PIN_7
#define DO_13_GPIO_Port GPIOE
#define DO_14_Pin GPIO_PIN_8
#define DO_14_GPIO_Port GPIOE
#define PWM_0_Pin GPIO_PIN_9
#define PWM_0_GPIO_Port GPIOE
#define DO_15_Pin GPIO_PIN_10
#define DO_15_GPIO_Port GPIOE
#define PWM_1_Pin GPIO_PIN_11
#define PWM_1_GPIO_Port GPIOE
#define DO_16_Pin GPIO_PIN_12
#define DO_16_GPIO_Port GPIOE
#define DO_17_Pin GPIO_PIN_15
#define DO_17_GPIO_Port GPIOE
#define PWM_6_Pin GPIO_PIN_10
#define PWM_6_GPIO_Port GPIOB
#define PWM_7_Pin GPIO_PIN_11
#define PWM_7_GPIO_Port GPIOB
#define DO_18_Pin GPIO_PIN_12
#define DO_18_GPIO_Port GPIOB
#define DO_19_Pin GPIO_PIN_13
#define DO_19_GPIO_Port GPIOB
#define PWM_22_Pin GPIO_PIN_14
#define PWM_22_GPIO_Port GPIOB
#define PWM_23_Pin GPIO_PIN_15
#define PWM_23_GPIO_Port GPIOB
#define SPI4_CS_Pin GPIO_PIN_10
#define SPI4_CS_GPIO_Port GPIOD
#define SPI6_CS_Pin GPIO_PIN_11
#define SPI6_CS_GPIO_Port GPIOD
#define PWM_12_Pin GPIO_PIN_12
#define PWM_12_GPIO_Port GPIOD
#define PWM_13_Pin GPIO_PIN_13
#define PWM_13_GPIO_Port GPIOD
#define PWM_14_Pin GPIO_PIN_14
#define PWM_14_GPIO_Port GPIOD
#define PWM_15_Pin GPIO_PIN_15
#define PWM_15_GPIO_Port GPIOD
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
#define E6_Pin GPIO_PIN_6
#define E6_GPIO_Port GPIOG
#define E6_EXTI_IRQn EXTI9_5_IRQn
#define E7_Pin GPIO_PIN_7
#define E7_GPIO_Port GPIOG
#define E7_EXTI_IRQn EXTI9_5_IRQn
#define PWM_16_Pin GPIO_PIN_6
#define PWM_16_GPIO_Port GPIOC
#define PWM_17_Pin GPIO_PIN_7
#define PWM_17_GPIO_Port GPIOC
#define PWM_10_Pin GPIO_PIN_8
#define PWM_10_GPIO_Port GPIOC
#define PWM_11_Pin GPIO_PIN_9
#define PWM_11_GPIO_Port GPIOC
#define SPI1_CS_Pin GPIO_PIN_8
#define SPI1_CS_GPIO_Port GPIOA
#define PWM_2_Pin GPIO_PIN_10
#define PWM_2_GPIO_Port GPIOA
#define PWM_3_Pin GPIO_PIN_11
#define PWM_3_GPIO_Port GPIOA
#define PWM_4_Pin GPIO_PIN_15
#define PWM_4_GPIO_Port GPIOA
#define SPI3_CS_Pin GPIO_PIN_1
#define SPI3_CS_GPIO_Port GPIOD
#define PWM_5_Pin GPIO_PIN_3
#define PWM_5_GPIO_Port GPIOB
#define PWM_8_Pin GPIO_PIN_4
#define PWM_8_GPIO_Port GPIOB
#define PWM_9_Pin GPIO_PIN_5
#define PWM_9_GPIO_Port GPIOB
#define DO_4_Pin GPIO_PIN_8
#define DO_4_GPIO_Port GPIOB
#define DO_3_Pin GPIO_PIN_9
#define DO_3_GPIO_Port GPIOB
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
