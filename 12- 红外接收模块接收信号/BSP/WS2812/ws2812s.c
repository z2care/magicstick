#include "ws2812s.h"
#include "delay.h"

uint8_t pixelBuffer2[PIXEL_NUM2][24] ;
 

void ws281xs_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
  DMA_InitTypeDef DMA_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //PORTA时钟使能 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE); //SPI1时钟使能 	
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输


  /* PA7  SPI1_MOSI */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PA15复用推挽输出 SPI
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOA

	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//串行同步时钟的空闲状态为低电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//串行同步时钟的第2个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;		//定义波特率预分频的值:波特率预分频值为16
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI2, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI2, ENABLE); //使能SPI外设
  SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);
  
  DMA_DeInit(DMA1_Channel5);   //将DMA的通道1寄存器重设为缺省值
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &(SPI2 -> DR); //cpar;  //DMA外设ADC基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)pixelBuffer2; //cmar;  //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //数据传输方向，从内存读取发送到外设
	DMA_InitStructure.DMA_BufferSize = PIXEL_NUM2 * 24; //cndtr;  //DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMA通道 x拥有中优先级 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器 
  
  ws281xs_closeAll();  //关闭全部的灯	1`
  Delay_ms(100); //关闭全部的灯需要一定的时间  
}

void ws281xs_closeAll(void)
{
  uint16_t i;
  uint8_t j;
  
  for(i = 0; i < PIXEL_NUM2; ++i)
  {
    for(j = 0; j < 24; ++j)
    {
      pixelBuffer2[i][j] = WS_LOW;
    }
  }
  ws281xs_show(); 
}

uint32_t ws281xs_color(uint8_t red, uint8_t green, uint8_t blue)
{
  return green << 16 | red << 8 | blue;
}

void ws281xs_setPixelColor(uint16_t n ,uint32_t GRBcolor)
{
  uint8_t i;
  if(n < PIXEL_NUM2)
  {
    for(i = 0; i < 24; ++i)
    {
      pixelBuffer2[n][i] = (((GRBcolor << i) & 0X800000) ? WS_HIGH : WS_LOW);
    }
  }
}

void ws281xs_setPixelRGB(uint16_t n ,uint8_t red, uint8_t green, uint8_t blue)
{
  uint8_t i;
  
  if(n < PIXEL_NUM2)
  {
    for(i = 0; i < 24; ++i)
    {
      pixelBuffer2[n][i] = (((ws281xs_color(red,green,blue) << i) & 0X800000) ? WS_HIGH : WS_LOW);
    }
  }
}

void ws281xs_show(void)
{
    DMA_Cmd(DMA1_Channel5, DISABLE );  //关闭USART1 TX DMA1 所指示的通道 
    DMA_ClearFlag(DMA1_FLAG_TC5);    
 	  DMA_SetCurrDataCounter(DMA1_Channel5,24 * PIXEL_NUM2 );//DMA通道的DMA缓存的大小
 	  DMA_Cmd(DMA1_Channel5, ENABLE);  //使能USART1 TX DMA1 所指示的通道 
}



// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t ws281xs_wheel(uint8_t wheelPos) {
  wheelPos = 255 - wheelPos;
  if(wheelPos < 85) {
    return ws281xs_color(255 - wheelPos * 3, 0, wheelPos * 3);
  }
  if(wheelPos < 170) {
    wheelPos -= 85;
    return ws281xs_color(0, wheelPos * 3, 255 - wheelPos * 3);
  }
  wheelPos -= 170;
  return ws281xs_color(wheelPos * 3, 255 - wheelPos * 3, 0);
}

// Fill the dots one after the other with a color
void ws281xs_colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<PIXEL_NUM2; i++) {
    ws281xs_setPixelColor(i, c);
    ws281xs_show();
    delay_ms(wait);
  }
}

void ws281xs_rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<PIXEL_NUM2; i++) {
      ws281xs_setPixelColor(i, ws281xs_wheel((i+j) & 255));
    }
    ws281xs_show();
    delay_ms(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void ws281xs_rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< PIXEL_NUM2; i++) {
      ws281xs_setPixelColor(i,ws281xs_wheel(((i * 256 / PIXEL_NUM2) + j) & 255));
    }
    ws281xs_show();
    delay_ms(wait);
  }
}

//Theatre-style crawling lights.
void ws281xs_theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < PIXEL_NUM2; i=i+3) {
        ws281xs_setPixelColor(i+q, c);    //turn every third pixel on
      }
      ws281xs_show();

      delay_ms(wait);

      for (uint16_t i=0; i < PIXEL_NUM2; i=i+3) {
        ws281xs_setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void ws281xs_theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < PIXEL_NUM2; i=i+3) {
        ws281xs_setPixelColor(i+q, ws281xs_wheel( (i+j) % 255));    //turn every third pixel on
      }
      ws281xs_show();

      delay_ms(wait);

      for (uint16_t i=0; i < PIXEL_NUM2; i=i+3) {
        ws281xs_setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

void ws281xs_300line_stable()
{
	int16_t i = 0;
	for(i=0; i<PIXEL_NUM2; i++) {
		ws281xs_setPixelColor(i, 0x003000);
	}
	ws281xs_show();
}

void ws281xs_300line_flow()
{

	uint8_t j = 0;//<25
	uint8_t i = 0;//<12
	uint8_t direction = 0;
	for(j = 0; j < 25; j++){
		
		for(i = 0; i < 12; i++){
			direction = i%2;
			if(direction == 0){
				//color arr[i*25 + j];
				ws281xs_setPixelColor(i*25 + j, ws281xs_color(0x30,0x30,0x30));
			}else{
				//color arr[i*25+25-1-j];
				ws281xs_setPixelColor(i*25+25-1-j, ws281xs_color(0x30,0x30,0x30));
			}
		}
		ws281xs_show();
		Delay_ms(20);
		ws281xs_closeAll();
	}
}
