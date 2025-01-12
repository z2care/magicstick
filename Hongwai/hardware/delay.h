#ifndef __DELAY_H
#define __DELAY_H

#include "stm32g0xx_hal.h"


// 注：调用延时函数之前，必须先调用delay_Init()将SysTick初始化
//**************************************************************


// 将SysTick定时器的时钟设为：9M=72M/8（HCLK的8分频，HCLK默认为72M）
//**************************************************************
void delay_Init(void) ;				// 初始化SysTick定时器


//**************************************************************
// SysTick时钟：9M（计数9次=1us），计数器最大值：2^24-1=16777215
// SysTick定时器延时时长应 <= 1,864,135 us
//**************************************************************

// 参数delay_us_Num的值应 < 1,864,135(十进制)
//**************************************************************
void delay_us(__IO uint32_t delay) ;	// 延时delay_us_Num微秒

// 参数delay_ms_Num的值应 < 1,864(十进制)
//**************************************************************
//u8 delay_ms(u16 delay_ms_Num) ;	// 延时delay_ms_Num毫秒




#endif	/* __DELAY_H */
