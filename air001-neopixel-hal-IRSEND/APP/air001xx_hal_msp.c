#include "main.h"

/**
  * @brief ��ʼ��ȫ��MSP
  */
void HAL_MspInit(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();
	
	//HAL_NVIC_SetPriority();
}
