#include "ws2812s.h"
#include "delay.h"

uint8_t pixelBuffer2[PIXEL_NUM2][24] ;
 

void ws281xs_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
  DMA_InitTypeDef DMA_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //PORTAʱ��ʹ�� 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE); //SPI1ʱ��ʹ�� 	
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����


  /* PA7  SPI1_MOSI */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PA15����������� SPI
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOA

	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�͵�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//����ͬ��ʱ�ӵĵ�2�������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ16
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI2, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
	SPI_Cmd(SPI2, ENABLE); //ʹ��SPI����
  SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);
  
  DMA_DeInit(DMA1_Channel5);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &(SPI2 -> DR); //cpar;  //DMA����ADC����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)pixelBuffer2; //cmar;  //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
	DMA_InitStructure.DMA_BufferSize = PIXEL_NUM2 * 24; //cndtr;  //DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //��������������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMAͨ�� xӵ�������ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ��� 
  
  ws281xs_closeAll();  //�ر�ȫ���ĵ�	1`
  Delay_ms(100); //�ر�ȫ���ĵ���Ҫһ����ʱ��  
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
    DMA_Cmd(DMA1_Channel5, DISABLE );  //�ر�USART1 TX DMA1 ��ָʾ��ͨ�� 
    DMA_ClearFlag(DMA1_FLAG_TC5);    
 	  DMA_SetCurrDataCounter(DMA1_Channel5,24 * PIXEL_NUM2 );//DMAͨ����DMA����Ĵ�С
 	  DMA_Cmd(DMA1_Channel5, ENABLE);  //ʹ��USART1 TX DMA1 ��ָʾ��ͨ�� 
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
