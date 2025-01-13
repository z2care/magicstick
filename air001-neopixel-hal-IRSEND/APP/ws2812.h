#ifndef __WS2812_H
#define __WS2812_H

#include "spi.h"
#include "dma.h"

/***********************************************
 * ʹ�õ�SPI�˿�
************************************************/
#define WS2812_SPI  hspi1

/***********************************************
 * ���������ʼ�λ��Ԫ����(����SPIʱ�����ʲ�ͬ�����¼���)
************************************************/
#define WS2812_0_CODE       0xC0 // 1100 0000b
#define WS2812_1_CODE       0xFC // 1111 1100b
#define WS2812_RET_CODE     0x00 // 0000 0000b
#define WS2812_RET_COUNT    51// ��λ����50us (50+1)*(WS2812_SPI_CLOCK/1000000)/8
#define WS2812_RST_COUNT_LOOP5 10//10������ѭ��5��

extern void WS2812_send(uint8_t *pixels,uint16_t numBytes);

#endif

