#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "receive.h"
//#include "uart.h"
#include "stdio.h"
#include "ws2812.h"
#include "ws2812s.h"

uint8_t ir_received = 0;

//idx0
uint8_t fu_arr[] = {80,10,22,23,39,40,43,44,52,53,56,58,59,67,68,71,74,77,81,82,86,87,89,92,93,94,98,99,102,105,106,110,111,116,117,122,123,125,127,128,129,130,133,136,138,139,140,141,142,146,147,149,150,153,155,157,158,159,163,166,167,170,171,172,181,182,183,184,187,188,189,197,200,201,203,214,217,218,230,231,235};
//idx1
uint8_t snake_arr[] = {78,7,8,20,21,24,25,26,39,41,42,45,46,54,55,67,70,71,75,76,85,86,91,95,97,98,102,105,112,113,114,116,117,118,120,121,123,127,128,130,131,134,135,136,139,142,143,147,148,150,154,155,156,157,158,164,165,166,169,172,173,175,178,181,182,183,184,185,200,202,203,205,206,207,214,215,235,236,245};
//idx2
uint8_t year_arr[] = {74,22,23,34,39,43,44,50,52,53,57,58,62,63,67,68,72,73,75,76,77,78,79,80,82,83,86,87,88,92,93,101,102,105,106,107,111,114,117,118,120,121,122,123,135,136,140,141,143,148,149,152,153,154,155,165,166,169,170,171,172,182,183,188,189,195,196,203,204,212,213,222,223,226,237};
//idx3
uint8_t big_arr[] = {69,7,18,19,25,26,40,41,45,46,47,51,52,53,55,56,74,75,76,77,85,86,87,96,97,98,99,102,103,104,105,106,107,108,109,115,116,119,120,121,122,123,124,125,126,127,131,132,133,137,138,142,143,145,146,151,152,169,170,184,185,200,201,215,216,217,233,234,246,247};
//idx4
uint8_t good_arr[] = {89,6,7,25,26,40,41,45,46,51,52,53,55,56,59,60,70,71,74,75,76,77,85,86,87,89,90,93,99,100,101,104,105,106,107,108,109,115,116,119,120,123,124,125,126,127,130,131,134,135,138,139,142,143,145,146,149,150,153,154,159,160,162,163,168,169,172,173,176,180,183,184,189,190,196,197,201,202,203,215,216,217,219,220,230,231,232,233,249,250};
//idx5
uint8_t I_arr[] = {44,21,22,44,45,51,52,53,75,76,77,78,79,82,85,86,87,105,106,107,119,120,121,135,136,137,153,154,155,162,165,166,167,187,188,189,190,191,195,196,197,220,221,229,230};
//idx6
uint8_t heart_arr[] = {151,25,26,27,28,36,37,38,39,40,41,55,56,57,58,59,60,61,62,67,68,69,70,71,72,73,74,87,88,89,90,91,92,93,94,95,98,99,100,101,102,103,104,105,106,107,108,109,115,116,117,118,119,120,121,122,123,124,125,126,127,130,131,132,133,134,135,136,137,138,139,140,141,142,143,146,147,148,149,150,151,152,153,154,155,156,157,158,159,162,163,164,165,166,167,168,169,170,171,172,173,174,175,178,179,180,181,182,183,184,185,186,187,188,189,190,191,194,195,196,197,198,199,200,201,202,203,204,205,206,212,213,214,215,216,217,218,219,220,221,222,223,226,227,228,229,230,231,232,233,234,235};
//idx7
uint8_t unisoc_arr[] = {81,38,39,42,43,53,54,55,57,58,59,60,68,69,72,73,76,77,83,84,89,90,93,94,98,99,102,103,110,111,113,114,117,118,119,120,121,122,123,124,127,128,129,130,133,134,135,136,137,138,139,140,143,144,146,147,150,151,158,159,163,164,169,170,173,174,180,181,184,185,188,189,197,198,201,202,203,204,214,215,218,219};

uint8_t *icons[8] = {fu_arr, snake_arr, year_arr, big_arr, good_arr, I_arr, heart_arr, unisoc_arr};

//GRB
uint32_t fgcolor_arr[] = {0x647F00, 0x647F00, 0x647F00, 0x647F00, 0x647F00, 0x007F64, 0x007F00, 0x004030 };
//RGB
uint32_t bgcolor_arr[] = {0x004000, 0x004000, 0x004000, 0x004000, 0x004000, 0x000000, 0x000000, 0x000000 };	

int main(void)
{
	ws281x_init();
	ws281xs_init();
	receive_Init();
	
	//Uart_Init(115200);
	//DMA1_Init();
	uint8_t ic_idx = 0;
	while (1)
	{
		if(ir_received == 1){
			//flow and show 8 icon//show unisoc 10s
			for(ic_idx = 0; ic_idx < 8; ic_idx++){
				ws281xs_300line_flow();
				ws281x_256square_publish(ic_idx, icons[ic_idx], fgcolor_arr, bgcolor_arr);
				Delay_ms(500);
			}
			Delay_ms(100000);
			ir_received = 0;
		}else{
			//show read
			ws281xs_300line_stable();
			//show fu
			ws281x_256square_publish(0, fu_arr, fgcolor_arr, bgcolor_arr);
			Delay_ms(200);
		}
	}
}
