#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "receive.h"
#include "uart.h"
#include "stdio.h"

extern uint8_t DMA_USART1_RX_BUF[USART_MAX_LEN];
int main(void)
{
	
	/*OLED初始化*/
	//OLED_Init();
	receive_Init();
	Uart_Init(115200);
	DMA1_Init();
	uint8_t str[] = "Hello,World!\r\n";
	usart_send_String(str);	//发送字符串
	
	//uint8_t data[] = "Hello,World!\r\n";	
	//DMA_USART1_Send(data, sizeof(data)); // 发送字符串，DMA转运
	printf("Hello,World!\r\n");
	//OLED_ShowString(0, 0,"Demo:", OLED_8X16);
	//OLED_Update();

	while (1)
	{

	}
}
