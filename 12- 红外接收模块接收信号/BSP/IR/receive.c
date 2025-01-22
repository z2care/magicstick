#include "stm32f10x.h"                  // 包含STM32F10x系列的头文件，提供必要的宏定义、函数声明等
#include "receive.h"
#include "Delay.h"
#include "uart.h"
#include "stdio.h"

extern uint8_t ir_received;

typedef struct Infrared_Data{

    uint8_t AddressCode;            //地址码
    uint8_t AddressInverseCode;     //地址反码
    uint8_t CommandCode;            //命令码
    uint8_t CommandInverseCode;     //命令反码

}Infrared_Data_Struct;

Infrared_Data_Struct InfraredData;

void receive_Init(void){
    /* 开启时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 		// 开启GPIOA的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 		// 开启GPIOB的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  		// 开启AFIO的时钟，用于外部中断线配置

    /* GPIO初始化 */
    GPIO_InitTypeDef GPIO_InitStructure; 						// 定义GPIO初始化结构体
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 				// 设置GPIO模式为上拉输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 					// 选择GPIOA的PA3引脚
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 			// 设置GPIO速度为50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure); 						// 根据GPIO_InitStructure初始化GPIOA的PA3引脚

    /* 使能EXTI中断线路 */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource3); // 将PA3引脚配置为外部中断线3

    EXTI_InitTypeDef EXTI_InitStructure; 						// 定义外部中断初始化结构体
    EXTI_InitStructure.EXTI_Line = EXTI_Line3; 					// 选择外部中断线3
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; 		// 设置外部中断模式为中断
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 	// 设置中断触发方式为下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE; 					// 使能外部中断线3
    EXTI_Init(&EXTI_InitStructure); 							// 根据EXTI_InitStructure初始化外部中断

    NVIC_InitTypeDef NVIC_InitStructure; 						// 定义中断控制器初始化结构体
    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn; 			// 选择外部中断3中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;// 设置中断抢占优先级为1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01; 		// 设置中断响应优先级为1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			// 使能中断通道
    NVIC_Init(&NVIC_InitStructure); 							// 根据NVIC_InitStructure初始化中断控制器
	
}

//获取红外低电平时间
//引导码低电平9ms，引导码：由9ms的低电平+4.5ms的高电平组成。
void Infrared_low(uint32_t *low_time) {
    uint32_t time_val = 0;
    while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == 0) {
        if(time_val >= 500) {  			// 超过10ms，认为数据接收引导码9ms超时
            *low_time = time_val;		//low_time=500，跳出循环
            return;
        }
        Delay_us(20);  // 延时20微秒
        time_val++;
    }
    *low_time = time_val;
}

//获取红外高电平时间
//引导码高电平9ms，引导码：由9ms的低电平+4.5ms的高电平组成。
void Infrared_high(uint32_t *high_time)
{
    uint32_t time_val = 0;
    while( GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == 1 )
    {
        if( time_val >= 250 )        // 超过5ms，认为数据接收引导码4.5ms超时
        {
            *high_time = time_val;   //跳出循环，high_time=250
            return;
        }
        Delay_us(20);
        time_val++;
    }
    *high_time = time_val;
}
//重复码，它是由一个 9ms 的低电平和一个 2.5ms 的高电平组成。
//当一个红外信号连续发送时，可以通过发送重复码的方式快速发送。
//引导码和重复码判断
uint8_t Guide_Repeat_Judgment(void) {
    uint32_t out_time=0;
    Infrared_low(&out_time);
    if((out_time > 500) || (out_time < 400)) {
        return 1;  // 不是引导码
    }
    Infrared_high(&out_time);
    if((out_time > 250) || (out_time < 100)) {
        return 1;  // 不是引导码
    }
    if((out_time > 100) && (out_time < 150)) {
        return 2;  // 重复码
    }
    return 0;  // 引导码
}

//红外数据正确性判断
uint8_t Infrared_Data_True(uint8_t *value) {
    if(value[0] != (uint8_t)(~value[1])) return 0;  // 地址码与地址反码校验
    if(value[2] != (uint8_t)(~value[3])) return 0;  // 命令码与命令反码校验
   printf("%x %x %x %x\r\n", value[0], value[1], value[2], value[3]);
	 //usart_send_String(&value[0]);	//发送字符串
	 //usart_send_String(&value[1]);	//发送字符串
	 //usart_send_String(&value[2]);	//发送字符串
	 //usart_send_String(&value[3]);	//发送字符串
    InfraredData.AddressCode = value[0];
    InfraredData.AddressInverseCode = value[1];
    InfraredData.CommandCode = value[2];
    InfraredData.CommandInverseCode = value[3];
    return 1;
}
//接收红外数据
/*
	数据发送0码：0.56ms低电平+ 0.56ms的高电平
	数据发送1码：0.56ms低电平+ 1.68ms的高电平
	收到数据位0: 0.56ms低电平+ 0.56ms的高电平
    收到数据位1: 0.56ms低电平+ 1.68ms的高电平
*/
void Receiving_Infrared_Data(void) {
    
    uint16_t Group_num = 0, Data_num = 0;				 // 定义变量，用于存储数据组的索引和数据位的索引  
    uint32_t time=0;									 // 定义变量，用于存储时间测量值			
    uint8_t Bit_data = 0;								 // 定义变量，用于存储单个数据位的值（0或1）   
    uint8_t ir_value[5] = {0};							 // 定义数组，用于存储接收到的红外数据（4个字节的数据 + 1个字节用于其他目的）
    uint8_t Guide_Repeat_Code = 0;						 // 定义变量，用于存储引导码和重复码的判断结果
    Guide_Repeat_Code = Guide_Repeat_Judgment();		 // 调用函数判断接收到的是否是引导码或重复码		
  
    if(Guide_Repeat_Code == 1) {						 // 如果判断结果不是引导码，则打印错误信息并结束函数
        //printf("err\r\n");
        return;
    }
		
		if(Guide_Repeat_Code != 1) {						 // 如果判断结果不是引导码，则打印错误信息并结束函数
        //printf("err\r\n");
			//ir_received = 1;
        return;
    }

    for(Group_num = 0; Group_num < 4; Group_num++) {	 // 循环4次，每次处理一个字节的数据	 
        for(Data_num = 0; Data_num < 8; Data_num++) {	 // 循环8次，每次处理一个数据位	          
            Infrared_low(&time);						 // 调用函数获取红外低电平时间
			
            if((time > 60) || (time < 20)) return;		 // 如果低电平时间不在0.4ms到1.2ms之间，说明数据错误，结束函数           
            time = 0;									 // 重置time变量，为下一次测量做准备          
            Infrared_high(&time);						 // 调用函数获取红外高电平时间
			
            if((time >= 60) && (time < 100)) {			 // 如果高电平时间在1.2ms到2ms之间，说明接收到的是数据位1
                Bit_data = 1;
            }
            
            else if((time >= 10) && (time < 50)) {		 // 如果高电平时间在0.2ms到1ms之间，说明接收到的是数据位0
                Bit_data = 0;
            }
            
            ir_value[Group_num] <<= 1;					 // 将接收到的数据位左移1位，为下一个数据位腾出位置	            
            ir_value[Group_num] |= Bit_data;			 // 将接收到的数据位写入数组	          
            time=0;										 // 重置time变量，为下一次测量做准备					
        }
    }   
    Infrared_Data_True(ir_value);						 // 调用函数判断接收到的数据是否正确，并保存正确数据		
}

//获取红外发送过来的命令
uint8_t Get_Infrared_Command(void)
{
    return InfraredData.CommandCode;
}
//清除红外发送过来的数据
void Clear_Infrared_Command(void)
{
    InfraredData.CommandCode = 0x00;
}

uint8_t temp2=0;
// 外部中断3的中断服务函数
void EXTI3_IRQHandler(void)
{
    // 检查是否是EXTI3的中断
    if(EXTI_GetITStatus(EXTI_Line3) != RESET)
    {
        // 清除EXTI3的中断标志位
        EXTI_ClearITPendingBit(EXTI_Line3);
        // 在这里编写中断处理代码
        // 例如，可以判断接收到的信号实现切换一个LED的状态     
		// 接收一次红外数据
        //Receiving_Infrared_Data();
			ir_received = 1;
		
		//char temp1=Get_Infrared_Command();
		//temp2=Get_Infrared_Command();
		//printf("temp1=%x temp2=%x\r\n", temp1, temp2);
		// Clear_Infrared_Command();
        // 其他中断处理代码...
    }
}
