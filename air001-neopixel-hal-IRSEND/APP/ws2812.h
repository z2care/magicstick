#ifndef __WS2812_H
#define __WS2812_H

#include "spi.h"
#include "dma.h"

/***********************************************
 * 使用的SPI端口
************************************************/
#define WS2812_SPI  hspi1

/***********************************************
 * 数据线速率及位码元定义(根据SPI时钟速率不同需重新计算)
************************************************/
#define WS2812_0_CODE       0xC0 // 1100 0000b
#define WS2812_1_CODE       0xFC // 1111 1100b
#define WS2812_RET_CODE     0x00 // 0000 0000b
#define WS2812_RET_COUNT    51// 复位至少50us (50+1)*(WS2812_SPI_CLOCK/1000000)/8
#define WS2812_RST_COUNT_LOOP5 10//10个数据循环5次

extern void WS2812_send(uint8_t *pixels,uint16_t numBytes);

#endif

