#include "stm32f10x.h"                  // Device header
#include "stdio.h"
#include "uart.h"
#include <string.h>
volatile uint16_t usart1_rx_len = 0;            //接收帧数据的长度
volatile uint16_t usart1_tx_len = 0;            //发送帧数据的长度
volatile uint8_t usart1_recv_end_flag = 0;      //帧数据接收完成标志

uint8_t DMA_USART1_RX_BUF[USART_MAX_LEN]={0};   //接收数据缓存
uint8_t DMA_USART1_TX_BUF[USART_MAX_LEN]={0};   //DMA发送缓存



// 串口引脚初始化
void Uart_Init(uint32_t __Baud) {
    // 开启时钟：GPIO复用时钟、GPIOA时钟和USART1时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 

    GPIO_InitTypeDef GPIO_InitStructure;

    // 初始化GPIO结构体，并配置TX引脚
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; // TX引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 速度50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 重新初始化GPIO结构体，并配置RX引脚
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; // RX引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 将USART1寄存器重置为默认值
    //USART_DeInit(USART1);//可写, 重置USART1寄存器为默认值，即取消之前的配置
	
    // 配置UART
    USART_InitTypeDef USART_InitStructure; // 定义配置串口的结构体变量 
    // 初始化USART结构体
    USART_StructInit(&USART_InitStructure);
    USART_InitStructure.USART_BaudRate = __Baud; // 设置波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; // 字节长度为8位
    USART_InitStructure.USART_StopBits = USART_StopBits_1; // 1个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No; // 没有校验位
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; // 收发模式
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无流控
    USART_Init(USART1, &USART_InitStructure); // 初始化USART1


		NVIC_InitTypeDef NVIC_InitStructure;

		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;         //串口1中断通道
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;   //抢占优先级2
		NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;         //子优先级1
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQ通道使能
		NVIC_Init(&NVIC_InitStructure);                           //根据指定的参数初始化VIC寄存器

		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;  //嵌套通道为DMA1_Channel4_IRQn
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //抢占优先级为 2
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;        //响应优先级为 2
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //通道中断使能
		NVIC_Init(&NVIC_InitStructure);

		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn ;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;   	//抢占优先级2
		NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 3;   	//子优先级3
		NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE; 	//IRQ通道使能
		NVIC_Init(&NVIC_InitStructure);

		USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);         			//开启串口空闲中断

    // 使能USART1
    USART_Cmd(USART1, ENABLE);
}
void DMA1_Init(void){
	
			/* 配置串口DMA接收*/
		DMA_InitTypeDef DMA_InitStructure;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);                                          // 开启DMA时钟

		// RX DMA1 通道5
		DMA_InitStructure.DMA_BufferSize = sizeof(DMA_USART1_RX_BUF);  // 定义了接收的最大长度
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;             // 串口接收，方向是外设->内存
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                   // 本次是外设到内存，所以关闭内存到内存
		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)DMA_USART1_RX_BUF;// 内存的基地址，要存储在哪里
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;// 内存数据宽度，按照字节存储
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;        // 内存递增，每次串口收到数据存在内存中，下次收到自动存储在内存的下一个位置
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                  // 正常模式
		DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_BASE + 0x04; // 外设的基地址，串口的数据寄存器
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // 外设的数据宽度，按照字节存储，与内存的数据宽度一致
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; // 接收只有一个数据寄存器 RDR，所以外设地址不递增
		DMA_InitStructure.DMA_Priority = DMA_Priority_High; // 优先级
		DMA_Init(DMA1_Channel5, &DMA_InitStructure);

		// TX DMA1 通道4
		DMA_InitStructure.DMA_BufferSize = 0;                              // 发送缓冲区的大小，初始化为0不发送
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                 // 发送是方向是外设到内存，外设作为目的地
		DMA_InitStructure.DMA_MemoryBaseAddr =(uint32_t)DMA_USART1_TX_BUF; // 发送内存地址，从哪里发送
		DMA_Init(DMA1_Channel4, &DMA_InitStructure);

		USART_DMACmd(USART1, USART_DMAReq_Tx | USART_DMAReq_Rx, ENABLE);   // 使能DMA串口发送和接受请求

		DMA_Cmd(DMA1_Channel5, ENABLE);     // 使能接收
		DMA_Cmd(DMA1_Channel4, DISABLE);    // 禁止发送

		DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE); // 使能接收DMA通道中断

		USART_Cmd(USART1, ENABLE);  //使能串口1
	
}

// 串口发送函数 发送单个字符
void usart_send_data(uint8_t ucch) {
    USART_SendData(USART1, (uint8_t)ucch);
    // 等待发送数据缓冲区为空,USART_FLAG_TXE传输数据寄存器空标志
    while (RESET == USART_GetFlagStatus(USART1, USART_FLAG_TXE)) {}

    // 等待传输完成,USART_FLAG_TC传输完成标志
    while (RESET == USART_GetFlagStatus(USART1, USART_FLAG_TC)) {}
}

// 发送字符串
void usart_send_String(uint8_t *ucstr)
{
      while(ucstr && *ucstr)  // 地址为空或者值为空跳出
      {
            usart_send_data(*ucstr++);// 发送当前字符,+1移动指针到下一个字符
      }
}
/*会在不使用微库的时候生效，我们这样设置就能正常使用printf函数了，而且兼容性会更高。*/
#if !defined(__MICROLIB)
//不使用微库的话就需要添加下面的函数
#if (__ARMCLIB_VERSION <= 6000000)
//如果编译器是AC5  就定义下面这个结构体
struct __FILE
{
        int handle;
};
#endif

FILE __stdout;

//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
      x = x;
}
#endif

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)//将C库printf函数重定向到USART
{  // 将字符变量ch发送到串口USART1
    USART_SendData(USART1, (uint8_t)ch);

    // 等待，直到USART1的数据寄存器为空，即数据已经发送出去
    while(RESET == USART_GetFlagStatus(USART1, USART_FLAG_TXE)) {
        // 循环等待，直到TXE（发送缓冲区为空）标志位被硬件设置为SET
    }

    // 返回发送的字符，这符合fputc函数的期望行为
    return ch;
}

void DMA_USART1_Send(uint8_t *data, uint16_t size)
{
      memcpy(DMA_USART1_TX_BUF, data, size); // 复制数据到DMA发送缓存区
      DMA_Cmd(DMA1_Channel4, DISABLE); // 确保DMA可以被设置
      DMA_SetCurrDataCounter(DMA1_Channel4, size); // 设置数据传输长度
      DMA_Cmd(DMA1_Channel4, ENABLE); // 打开DMA通道，开始发送	
}

void USART1_IRQHandler(void)  //串口1中断服务程序
{
    if(USART_GetITStatus(USART1,USART_IT_IDLE)!=RESET) //空闲中断触发
    {
        usart1_recv_end_flag = 1;     // 接受完成标志位置1

        DMA_Cmd(DMA1_Channel5, DISABLE);  /* 暂时关闭dma，数据尚未处理 */
        usart1_rx_len = USART_MAX_LEN - DMA_GetCurrDataCounter(DMA1_Channel5);/* 获取接收到的数据长度 单位为字节*/
        DMA_ClearFlag(DMA1_FLAG_TC5);     /* 清DMA标志位 */

        DMA_USART1_Send(DMA_USART1_RX_BUF, usart1_rx_len); // 将接收的数据回显

        DMA_SetCurrDataCounter(DMA1_Channel5,USART_MAX_LEN); /* 重新赋值计数值，必须大于等于最大可能接收到的数据帧数目 */
        DMA_Cmd(DMA1_Channel5, ENABLE);   /*打开DMA*/

        USART_ReceiveData(USART1);   //清除空闲中断标志位（接收函数有清标志位的作用）
    }

    if(USART_GetFlagStatus(USART1,USART_IT_TC)==RESET) //传输完成中断
    {
        USART_ClearITPendingBit(USART1, USART_IT_TC); // 清除传输完成中断标志位
        USART_ITConfig(USART1,USART_IT_TC,DISABLE);
        usart1_rx_len = 0;
    }
}

// DMA1_Channel5_IRQHandler函数
void DMA1_Channel5_IRQHandler(void)
{
    if(DMA_GetFlagStatus(DMA1_FLAG_TC5) != RESET) // 等待DMA1_Channel5传输完成
    {
        DMA_ClearFlag(DMA1_FLAG_TC5); // 清除DMA1_Channel5传输完成标志
        DMA_Cmd(DMA1_Channel5, DISABLE); // 关闭使能
        USART_ITConfig(USART1, USART_IT_TC, ENABLE); // 打开串口发送完成中断
    }
}
