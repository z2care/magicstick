#ifndef __UART_H
#define __UART_H
#include "stm32f10x.h"                  // Device header

#define USART_MAX_LEN 400

void Uart_Init(uint32_t __Baud);
void usart_send_data(uint8_t ucch);
void usart_send_String(uint8_t *ucstr);
void DMA_USART1_Send(uint8_t *data, uint16_t size);
void DMA1_Init(void);

#endif
