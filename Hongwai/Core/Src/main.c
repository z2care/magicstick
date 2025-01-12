/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "infrared.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
u16 code_16bit[10] = {0x7F80};
/*  开  亮度加  辅助光源 亮度减 色温-  色温+  分段*/
u8 code_8bit[10] = {0x01,0x12,0x1A,0x1E,0x03,0x02 ,0x06};

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

	u16	user_code_16bit = 0x1234;	// 初始化16位用户码
	
	u8 data_code_8bit = 0x66 ;		// 初始化8位数据码
	u8 i = 0;
  /* USER CODE END 1 */

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
  MX_I2C1_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim3);
    __HAL_UART_ENABLE_IT(&huart2,UART_IT_IDLE);
    HAL_UART_Receive_DMA(&huart2,Usart2_Rx,sizeof(Usart2_Rx));
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		// 判断是否接收到有效的NEC红外信息
		//------------------------------------------------------------------------
//		HAL_Delay(200);
//		NEC_IE_code_message(code_16bit[0],code_8bit[2]);
		
//						HAL_Delay(200);
//						NEC_IE_code_message(user_code_16bit,data_code_8bit);	// 红外发送：用户码 + 数据
//		HAL_Delay(500);
//		NEC_IE_code_message(code_16bit[0],code_8bit[2]);
		if(Current_bit_CNT>=34)
		{
			// 解码NEC格式红外信息
			//------------------------------
			if( ! NEC_IR_decode_message() )
			{
				// 判断数据码和数据码反码是否相反
				//-----------------------------
				if( Receive_data_code_8bit == (u8)(~Receive_data_code_opposite) )
				{
					// 避免重复保存
					//-------------
					if( user_code_16bit!=Receive_user_code_16bit || data_code_8bit!=Receive_data_code_8bit )
					{
						user_code_16bit = Receive_user_code_16bit;	// 保存用户码
					
						data_code_8bit  = Receive_data_code_8bit ;	// 保存数据码
						
						//将接收到的：用户码高字节 / 用户码低字节 / 数据码，通过串口发送
						#if USART_SEND
						//------------------------------------------------------------
						USART_SendData( USART1, user_code_16bit>>8 );
						while ( USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET );
						USART_SendData( USART1, user_code_16bit );
						while ( USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET );
						USART_SendData( USART1, data_code_8bit );
						while ( USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET );
						//------------------------------------------------------------
						#endif
						
//						PB_out(5) = 0;	// LED0亮一下，表示将红外码接收
//						HAL_Delay(200);
//						NEC_IE_code_message(user_code_16bit,data_code_8bit);	// 红外发送：用户码 + 数据码
//						NEC_IE_code_message(code_16bit[0],code_8bit[2]);
//						PB_out(5) = 1;
					}
					
					
					if(It_flag == 1 && user_code_16bit == 0x7F80 && uart_flag == 0)
					{
							i++;
						It_flag = 0;
						if(i%2==0)
						{
							i = 0;
							HAL_Delay(500);
//							NEC_IE_code_message(code_16bit[0],code_8bit[2]);
							NEC_IE_code_message(user_code_16bit,data_code_8bit);	// 红外发送：用户码 + 数据码
						}
					}
					
				}
			}
			
		}
		//------------------------------------------------------------------------
		
		
		// 按键发码
		#if SEND_KEY
		//------------------------------------------------------------------------------------------
		KEY_Scan_delay_JX();	// 扫描按键是否按下
		
		if( S_KEY_down == 0x04 )
		{
			if( F_key_down == 0 )
			{
				F_key_down = 1;	// 长按只有效一次
				
				     (user_code_16bit,data_code_8bit);	// 红外发送：用户码 + 数据码
				
				//将接收到的：用户码高字节 / 用户码低字节 / 数据码，通过串口发送
				//------------------------------------------------------------
				USART_SendData( USART1, user_code_16bit>>8 );
				while ( USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET );
				USART_SendData( USART1, user_code_16bit );
				while ( USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET );
				USART_SendData( USART1, data_code_8bit );
				while ( USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET );
				//------------------------------------------------------------
			}
		}
		
		else F_key_down = 0;
		#endif
		if(uart_flag == 1)
		{
			uart_flag = 0;
			switch(Usart2_Rx[1])
			{
				case 0x02:
							HAL_Delay(500);
							NEC_IE_code_message(code_16bit[0],code_8bit[0]);
					break;
				case 0x03:
							HAL_Delay(500);
							NEC_IE_code_message(code_16bit[0],code_8bit[0]);
					break;
				case 0x04:
							HAL_Delay(500);
							NEC_IE_code_message(code_16bit[0],code_8bit[1]);
					break;
				case 0x05:
							HAL_Delay(500);
							NEC_IE_code_message(code_16bit[0],code_8bit[3]);
					break;
				case 0x06:
							HAL_Delay(500);
							NEC_IE_code_message(code_16bit[0],code_8bit[2]);
					break;
				case 0x07:
							HAL_Delay(500);
							NEC_IE_code_message(code_16bit[0],code_8bit[6]);
					break;
				default:
					break;
			}
			
		}
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

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;//64M
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;//div 1
  RCC_OscInitStruct.PLL.PLLN = 8;//mul 8 = 64*8= 512
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;//div 2=256M
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;//div 2=256M
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
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
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

