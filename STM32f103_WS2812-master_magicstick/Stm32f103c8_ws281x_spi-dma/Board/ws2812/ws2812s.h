#ifndef __WS2812S_H
#define __WS2812S_H

#include "stm32f10x.h"

#define PIXEL_NUM2 300

//硬件spi模拟ws2811时序（用spi的8位数据模拟ws281x的一位数据）
//要将系统时钟设置为56M，分频数设置为8，则SPI的通信频率为7M，传输一位数据的时间约为143纳秒（ns）
//3*143 = 429ns   5*143 = 715ns  符合WS281X芯片的通信时序。
//  _____   
// |     |___|   11111000  high level
//  ___         
// |   |_____|   11100000  low level

#define WS_HIGH 0XF8
#define WS_LOW  0XE0

void ws281xs_init(void);
void ws281xs_closeAll(void);
void ws281xs_rainbowCycle(uint8_t wait);
uint32_t ws281xs_color(uint8_t red, uint8_t green, uint8_t blue);
void ws281xs_setPixelColor(uint16_t n ,uint32_t GRBcolor);
void ws281xs_show(void);

void ws281xs_theaterChase(uint32_t c, uint8_t wait);
void ws281xs_colorWipe(uint32_t c, uint8_t wait);
void ws281xs_rainbow(uint8_t wait);
void ws281xs_theaterChaseRainbow(uint8_t wait);

void ws281xs_300redcage();
void ws281xs_256square(uint8_t *pic_index_arr);

#endif /* __WS2812S_H */

