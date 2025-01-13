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
#include "air001xx_hal.h"

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
#define IR_EXTI_IRQn EXTI0_1_IRQn
#define IE_Pin GPIO_PIN_6
#define IE_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

#ifdef HAL_UART_MODULE_ENABLED
// debug printf redirect config
#define DEBUG_USART_BAUDRATE 115200

#define DEBUG_USART USART2
#define DEBUG_USART_CLK_ENABLE()                           \
  do                                                       \
  {                                                        \
    __IO uint32_t tmpreg = 0x00U;                          \
    SET_BIT(RCC->APBENR1, RCC_APBENR1_USART2EN);           \
    /* RCCÍâÎ§Ê±ÖÓÆôÓÃºóµÄÑÓ³Ù */                \
    tmpreg = READ_BIT(RCC->APBENR1, RCC_APBENR1_USART2EN); \
    UNUSED(tmpreg);                                        \
  } while (0U)

#define __GPIOA_CLK_ENABLE()                            \
  do                                                    \
  {                                                     \
    __IO uint32_t tmpreg = 0x00U;                       \
    SET_BIT(RCC->IOPENR, RCC_IOPENR_GPIOAEN);           \
    /* RCCÍâÎ§Ê±ÖÓÆôÓÃºóµÄÑÓ³Ù */             \
    tmpreg = READ_BIT(RCC->IOPENR, RCC_IOPENR_GPIOAEN); \
    UNUSED(tmpreg);                                     \
  } while (0U)

#define DEBUG_USART_RX_GPIO_PORT GPIOA
#define DEBUG_USART_RX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define DEBUG_USART_RX_PIN GPIO_PIN_3
#define DEBUG_USART_RX_AF GPIO_AF4_USART2

#define DEBUG_USART_TX_GPIO_PORT GPIOA
#define DEBUG_USART_TX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define DEBUG_USART_TX_PIN GPIO_PIN_2
#define DEBUG_USART_TX_AF GPIO_AF4_USART2

#define DEBUG_USART_IRQHandler USART2_IRQHandler
#define DEBUG_USART_IRQ USART2_IRQn
#endif

/* USER CODE END Private defines */


#ifdef HAL_UART_MODULE_ENABLED
  extern UART_HandleTypeDef DebugUartHandle;
	void BSP_USART_Config(void);
	#define DEBUG_USART_Config BSP_USART_Config
#endif

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
