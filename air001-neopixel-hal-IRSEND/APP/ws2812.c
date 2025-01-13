#include "ws2812.h"

// WS2812��λ����
//static uint8_t WS2812_RET_Buf[WS2812_RET_COUNT]=
//    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//     0x00};

// WS2812�������ݻ�����(ÿλռ��24���ֽ�)
//static uint8_t WS2812_Data_Buf[WS2812_LINK_COUNT * 3 * 8];
//static uint8_t WS2812_Data_Buf[WS2812_LINK_COUNT * 3 * 8/64];
static uint8_t WS2812_Data_Buf[8];

void WS2812_send(uint8_t *pixels, uint16_t numBytes)
{
    uint16_t pos;
    uint8_t pos_bit;
    uint8_t temp;
    
    // ����WS2812��λ
    //HAL_SPI_Transmit_DMA(&WS2812_SPI, WS2812_RET_Buf, sizeof(WS2812_RET_Buf));

    //ѭ������5��
    //for(int i = 0; i < 5; i++)
      //HAL_SPI_Transmit_DMA(&WS2812_SPI, WS2812_RST_Buf_LOOP5, sizeof(WS2812_RST_Buf_LOOP5));
    
    // ����WS2812����
//    for(pos=0;pos<numBytes;pos++)
//    {
//        temp = *pixels;
//        for(pos_bit=0;pos_bit<8;pos_bit++)
//        {
//           if(temp & 0x80) WS2812_Data_Buf[pos * 8 + pos_bit] = WS2812_1_CODE;
//           else WS2812_Data_Buf[pos * 8 + pos_bit] = WS2812_0_CODE;
//           temp <<= 1;
//        }
//        pixels++;
//    }
//    HAL_SPI_Transmit_DMA(&WS2812_SPI, WS2812_Data_Buf, sizeof(WS2812_Data_Buf));

    //loop = 4, 64/4=16(pixel) buffer=16*3*8(byte)
    //loop = 8, 64/8=8(pixel)  buffer=8*3*8(byte)
    //loop = 16 64/16=4(pixel) buffer=4*3*8(byte)
		//loop = 32 64/32=2(pixel) buffer=2*3*8(byte)
		//loop = 64 64/64=1(pixel) buffer=1*3*8(byte)
    for(int i = 0; i < 64; i++)
    {
        for(pos=0;pos<1*3;pos++)
        {
            temp = *pixels;
            for(pos_bit=0;pos_bit<8;pos_bit++)
            {
               if(temp & 0x80) WS2812_Data_Buf[pos_bit] = WS2812_1_CODE;
               else WS2812_Data_Buf[pos_bit] = WS2812_0_CODE;
               temp <<= 1;
            }
            pixels++;
						while(WS2812_SPI.State!=HAL_SPI_STATE_READY){};
						HAL_SPI_Transmit_DMA(&WS2812_SPI, WS2812_Data_Buf, sizeof(WS2812_Data_Buf));
        }
        //8byte������Ҫ����ʱ�򣬿��ܼ�Ϸ����ǲ��е���
			}
}
