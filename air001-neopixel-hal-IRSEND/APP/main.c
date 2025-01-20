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
#include "gpio.h"
#include "dma.h"
#include "spi.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ws2812.h"
#include "Adafruit_NeoPixel.h"
#include "infrared.h"
#include "effects.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint16_t code_16bit[10] = {0x7F80};
/*  ¿ª  ÁÁ¶È¼Ó  ¸¨Öú¹âÔ´ ÁÁ¶È¼õ É«ÎÂ-  É«ÎÂ+  ·Ö¶Î*/
uint8_t code_8bit[10] = {0x01,0x12,0x1A,0x1E,0x03,0x02 ,0x06};

uint8_t keyPressed = 0;
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

#ifdef HAL_UART_MODULE_ENABLED
UART_HandleTypeDef DebugUartHandle;
#endif

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

HAL_StatusTypeDef status;

/* USER CODE END 0 */


void EXTI4_15_IRQHandler(void)
{
	//printf("key GPIOA4 pressed\r\n");
	keyPressed = 1;
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);//按键中断
}
/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	uint16_t	user_code_16bit = 0x2015;	//用户码
	
	uint8_t data_code_8bit = 0x01 ;		//数据码
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  MX_GPIO_Init();
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_DMA_Init();;
  MX_SPI1_Init();
	//BSP_USART_Config();

  /* USER CODE BEGIN 2 */
	adafruit_neopixel.NeoPixel_op = WS2812_send;//发送函数
	adafruit_neopixel.init(WS2812_LINK_COUNT, NEO_GRB + NEO_KHZ800);
	adafruit_neopixel.begin();
	adafruit_neopixel.setBrightness(BRIGHTNESS);
	adafruit_neopixel.show();
	//rainbowCycle(10);

  while(1)
  {
		if(keyPressed){
			keyPressed = 0;
			resetPixels();
			//colorWipe(adafruit_neopixel.Color_rgb(128, 128, 128), 25);//128 white
			colorWipe2(adafruit_neopixel.Color_rgb(128, 128, 128), 15);//128 white
      NEC_IE_code_message(user_code_16bit, data_code_8bit);
      delay_ms(1000);
			//keyPressed = 0;
		}
		rainbowCycle(10);
	}
	
  /* USER CODE END 2 */
}

/*配置HSI16MHZ，SPI所在APB1,8MHZ*/
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/* 配置HSI,HSE,LSE,LSI,PLL所有时钟 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;						  /* 使能HSI */
	RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;						  /* HSI预分频 */
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_24MHz; /* 设置HSI输出时钟为16MHz,库会设置校准值 */
	RCC_OscInitStruct.HSEState = RCC_HSE_OFF;						  /* 禁止HSE */
	RCC_OscInitStruct.HSEFreq = RCC_HSE_16_32MHz;					  /* 设置HSE频率范围,没用可以不设置 */
	RCC_OscInitStruct.LSIState = RCC_LSI_OFF;						  /* 禁止LSI */
	RCC_OscInitStruct.LSEState = RCC_LSE_OFF;						  /* 禁止LSE */
	RCC_OscInitStruct.LSEDriver = RCC_LSEDRIVE_MEDIUM;				  /*  LSE默认驱动能力  */
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;					  /* 不用锁相环倍频PLL */
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;			  /* PLL的时钟源，PLL时钟源只有24M时, 再使能PLL，此处的定义没啥用*/

	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) /* 配置时钟 */
	{
		Error_Handler();
	}

	/* 初始化AHB,APB总线时钟 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK; /* 配置AHB时钟源 */
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;	   /* 设置AHB预分频 */
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;	   /* 设置APB1预分频,APB1不分频，在SPI配置中2分频 */

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) /* 配置总线 */
	{
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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

#ifdef HAL_UART_MODULE_ENABLED
/**
 * @brief DEBUG_USART GPIO配置,模式配置,115200 8-N-1
 * @param None
 * @retval 无
 */
void BSP_USART_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  DEBUG_USART_CLK_ENABLE();

  DebugUartHandle.Instance = DEBUG_USART;

  DebugUartHandle.Init.BaudRate = DEBUG_USART_BAUDRATE;
  DebugUartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  DebugUartHandle.Init.StopBits = UART_STOPBITS_1;
  DebugUartHandle.Init.Parity = UART_PARITY_NONE;
  DebugUartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  DebugUartHandle.Init.Mode = UART_MODE_TX_RX;

  HAL_UART_Init(&DebugUartHandle);

  DEBUG_USART_RX_GPIO_CLK_ENABLE();
  DEBUG_USART_TX_GPIO_CLK_ENABLE();

  /**USART GPIO Configuration
    PA2     ------> USART1_TX
    PA3     ------> USART1_RX
    */
  GPIO_InitStruct.Pin = DEBUG_USART_TX_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = DEBUG_USART_TX_AF;
  HAL_GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = DEBUG_USART_RX_PIN;
  GPIO_InitStruct.Alternate = DEBUG_USART_RX_AF;

  HAL_GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStruct);

  /* ENABLE NVIC */
  HAL_NVIC_SetPriority(DEBUG_USART_IRQ, 0, 1);
  HAL_NVIC_EnableIRQ(DEBUG_USART_IRQ);
}

#if (defined(__CC_ARM)) || (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
/**
 * @brief 写一个字符到usart中。
 *@param ch
 * *f
 *@retval 该字符
 */
int fputc(int ch, FILE *f)
{
  /* Send a byte to USART */
  HAL_UART_Transmit(&DebugUartHandle, (uint8_t *)&ch, 1, 1000);

  return (ch);
}

/**
 *@brief 从usart中获取一个字符
 *@param *f
 *@retval 一个字符
 */
int fgetc(FILE *f)
{
  int ch;
  HAL_UART_Receive(&DebugUartHandle, (uint8_t *)&ch, 1, 1000);
  return (ch);
}

#elif defined(__ICCARM__)
/**
 * @brief 写一个字符到usart中。
 *@param ch
 * *f
 *@retval 该字符
 */
int putchar(int ch)
{
  /* Send a byte to USART */
  HAL_UART_Transmit(&DebugUartHandle, (uint8_t *)&ch, 1, 1000);

  return (ch);
}
#endif

#endif
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
