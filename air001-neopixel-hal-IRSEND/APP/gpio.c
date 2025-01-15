/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(IE_GPIO_Port, IE_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = IE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(IE_GPIO_Port, &GPIO_InitStruct);//初始化红外发射
	
	GPIO_InitTypeDef gpioinitstruct;
  gpioinitstruct.Pin = GPIO_PIN_4;
  gpioinitstruct.Pull = GPIO_PULLUP;//or GPIO_PULLDOWN
  gpioinitstruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  gpioinitstruct.Mode = GPIO_MODE_IT_FALLING;
  HAL_GPIO_Init(GPIOA, &gpioinitstruct);//初始化按键

  /* Enable and set Button EXTI Interrupt to the lowest priority */
  HAL_NVIC_SetPriority((IRQn_Type)(EXTI4_15_IRQn), 0, 0);//高优先级就是0,0
  HAL_NVIC_EnableIRQ((IRQn_Type)(EXTI4_15_IRQn));
}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
