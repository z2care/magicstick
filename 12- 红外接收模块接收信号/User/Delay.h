#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f10x.h"

void Delay_us(uint32_t us);
void Delay_ms(uint32_t ms);
void Delay_s(uint32_t s);

//void delay_init(void);
//void delay_ms(uint16_t nms);
//void delay_us(uint32_t nus);

#endif
