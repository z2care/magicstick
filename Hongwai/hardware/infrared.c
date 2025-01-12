#include "infrared.h"

// 缓存红外接收数据的全局变量
//--------------------------------------------------------
u32 Each_bit_duration[64] = {0};	// 记录每一位数据的时长

u8	Current_bit_CNT = 0;			// 表示当前是第几位

u16 Receive_user_code_16bit = 0 ;	// 记录16位用户码

u8  Receive_data_code_8bit = 0 ;	// 记录8位数据码

u8  Receive_data_code_opposite = 0;	// 记录8位数据码的反码
//--------------------------------------------------------





// 以下为红外发射的相关函数
//########################################################################################################

// 载波调制时的延时函数
//==============================================================

// 调用SysTick定时器的延时函数
// 优点：跟程序无关，延时时钟比较准确
// 缺点：不能完全精准到8.77us、17.53us
//---------------------------------------
#define		delay8_77us()	delay_us(8)
#define		delay17_53us()	delay_us(17)
//---------------------------------------


/*
// 调用无用指令的延时函数
// 优点：能达到比较准确的8.77us、17.53us
// 缺点：跟程序有关，程序修改后的优化，可能导致延时不准
// 建议：在程序架构、函数确定后，再用示波器等工具微调计数值
//-----------------------------------------------------
volatile void delay8_77us(void)     // 延时8.77us
{
	u8 i=68;		// 不完全准确
	while(i--);		// 需用示波器检测调整
}

volatile void delay17_53us(void)    // 延时17.53us
{
	u8 i=136;		// 不完全准确
	while(i--);		// 需用示波器检测调整
}
//----------------------------------------------------
*/

//==============================================================




// 初始化红外发射管脚：Infrared_IE = PC1
//----------------------------------------------------------------------------------------------
//void Infrared_IE_Init_JX(void)
//{
//	GPIO_InitTypeDef GPIO_InitStruct;                       // 定义一个GPIO_InitTypeDef类型的变量
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);   // 允许GPIOC时钟
//	
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;                  // GPIO_Pin_1
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;           // 通用推挽输出
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;          // 50MHz速度
//	GPIO_Init(GPIOC, &GPIO_InitStruct);
//	
//	GPIO_ResetBits(GPIOC, GPIO_Pin_1);                      //PC1拉低
//}
//----------------------------------------------------------------------------------------------


//*********************************** 载波调制 *********************************************
//
//	26.3us（一个周期）= 8.77us（IE为高，发射红外光）+ 17.53us（IE为低，不发射红外光）
//	注：一个载波周期中，发射红外光8.77us  + 不发射红外光17.53us，这个周期就是载波发射周期。
//	注：载波发射周期中，发射红外光的占空比一般为1/3。

//	26.3us（一个周期）= 26.3us（IE为低，不发射红外光）
//	注：整个周期内，都不发射红外光，这个周期为载波不发射周期。

//******************************************************************************************


// NEC协议数据"0"= 载波发射0.56ms + 载波不发射0.56ms
//-------------------------------------------------
void NEC_IE_Send_zero(void)     
{
	u8 i;
	
	// 载波发射0.56ms ≈ 26.3us * 21
	//-------------------------------
	for(i=0;i<22;i++)
	{
        //26.3us（载波发射周期）
		//------------------------------------
		HAL_GPIO_WritePin(IE_GPIO_Port,IE_Pin,GPIO_PIN_SET);	// IE抬高，发射红外光
		delay8_77us();  // 延时8.77us                        
											
		HAL_GPIO_WritePin(IE_GPIO_Port,IE_Pin,GPIO_PIN_RESET);// IE拉低，不发射红外光
	
		delay17_53us(); // 延时17.53us
		//------------------------------------
	}

	
	// 载波不发射0.56ms ≈ 26.3us * 21
	//-------------------------------
	for(i=0;i<21;i++)
	{
        //26.3us（载波不发射周期）
		//------------------------------------
		HAL_GPIO_WritePin(IE_GPIO_Port,IE_Pin,GPIO_PIN_RESET);// IE拉低，不发射红外光
		delay8_77us();  // 延时8.77us
                                            
		HAL_GPIO_WritePin(IE_GPIO_Port,IE_Pin,GPIO_PIN_RESET);// IE拉低，不发射红外光
		delay17_53us(); // 延时17.53us
		//------------------------------------
	}
}
//-------------------------------------------------


// NEC协议数据"1" = 载波发射0.56ms + 载波不发射1.68ms
//-------------------------------------------------
void NEC_IE_Send_one(void)
{
	u8 i;
	
	// 载波发射0.56ms ≈ 26.3us * 21
	//-------------------------------
	for(i=0;i<22;i++)
	{
		//26.3us（载波发射周期）
		//------------------------------------
		HAL_GPIO_WritePin(IE_GPIO_Port,IE_Pin,GPIO_PIN_SET);    // IE抬高，发射红外光
		delay8_77us();	// 延时8.77us
		
		HAL_GPIO_WritePin(IE_GPIO_Port,IE_Pin,GPIO_PIN_RESET);  // IE拉低，不发射红外光
		delay17_53us();	// 延时17.53us
		//------------------------------------
	}
	
	// 载波不发射1.68ms ≈ 26.3us * 64
	//--------------------------------
	for(i=0;i<64;i++)
	{
		//26.3us（载波不发射周期）
		//------------------------------------
		HAL_GPIO_WritePin(IE_GPIO_Port,IE_Pin,GPIO_PIN_RESET);// IE拉低，不发射红外光
		delay8_77us();	// 延时8.77us
		
		HAL_GPIO_WritePin(IE_GPIO_Port,IE_Pin,GPIO_PIN_RESET);// IE拉低，不发射红外光
		delay17_53us();	// 延时17.53us
		//------------------------------------
	}
}
//-------------------------------------------------


// 将一帧数据调制为NEC协议规定的红外载波发射出去
// 一帧数据格式：低位在前，由低到高发送8位数据
// 这帧数据可以是：②16位用户码低字节(8位) / ③16位用户码高字节(8位) / ④8位数据码 / ⑤8位数据码的反码
//----------------------------------------------------------------------------------------------
void NEC_IE_One_Data(u8 IE_One_Data)
{
	u8 i;
	
	for(i=0;i<8;i++)
	{
		if( IE_One_Data & 0x01 )
			NEC_IE_Send_one();
		else
			NEC_IE_Send_zero();
			
		IE_One_Data>>=1;
	}
}
//---------------------------------



// 发送NEC编码格式的信息
//----------------------------------------------------------------------------------------------------------------------
// NEC 编码格式： ①引导码 + ②16位用户码低字节(8位) + ③16位用户码高字节(8位) + ④8位数据码 + ⑤8位数据码的反码 + ⑥结束码‘0’ 
//----------------------------------------------------------------------------------------------------------------------
void NEC_IE_code_message(u16 user_code_16bit, u8 data_code_8bit)
{
	u16 i;
	
	u8 T_user_code_high = user_code_16bit>>8;
	
	u8 T_user_code_low = user_code_16bit;

	// ①引导码：载波发射9ms，不发射4.5ms
	//------------------------------------------------------------
	// 载波发射 9ms ≈ 26.3us * 342
	for(i=0;i<342;i++) 
	{
		HAL_GPIO_WritePin(IE_GPIO_Port,IE_Pin,GPIO_PIN_SET);	// IE抬高，发射红外光
//		HAL_GPIO_WritePin(IE_GPIO_Port,IE_Pin,GPIO_PIN_SET);
		delay8_77us();	// 延时8.77us
		
		HAL_GPIO_WritePin(IE_GPIO_Port,IE_Pin,GPIO_PIN_RESET);;	// IE拉低，不发射红外光
		delay17_53us();	// 延时17.53us
	}
	
	// 载波不发射 4.5ms ≈ 26.3us * 171
	for(i=0;i<171;i++)
	{
		HAL_GPIO_WritePin(IE_GPIO_Port,IE_Pin,GPIO_PIN_RESET);// IE拉低，不发射红外光
		delay8_77us();	// 延时8.77us
		
		HAL_GPIO_WritePin(IE_GPIO_Port,IE_Pin,GPIO_PIN_RESET);// IE拉低，不发射红外光
		delay17_53us();	// 延时17.53us
	}
	//------------------------------------------------------------
	
	
	//------------------------------------------------------------
	// ②16位用户码低字节	：8位数据
	NEC_IE_One_Data(T_user_code_low);
	
	// ③16位用户码高字节：8位数据
	NEC_IE_One_Data(T_user_code_high);
	
	// ④8位数据码 ：8位数据
	NEC_IE_One_Data(data_code_8bit);
	
	// ⑤8位数据码的反码：8位数据
	NEC_IE_One_Data(~data_code_8bit);
	
	//------------------------------------------------------------
	
	
	// ⑥结束码‘0’
	//--------------
	NEC_IE_Send_zero();
	//--------------
}

//########################################################################################################



// 以下为红外接收的相关函数
//########################################################################################################

// 初始化红外发射管脚：Infrared_IR = PC0
//--------------------------------------------------------------------------------
//void Infrared_IR_Init_JX(void) 
//{
//	GPIO_InitTypeDef GPIO_InitStruct; 
//	EXTI_InitTypeDef EXTI_InitStruct;
//	NVIC_InitTypeDef NVIC_InitStruct;
//	
//	/* 配置PC0引脚 */
//	//---------------------------------------------------------------------------
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);		// 使能PC端口时钟											
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;	                // 上拉输入模式
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;						// PC0
//	GPIO_Init(GPIOC, &GPIO_InitStruct);	
//	//---------------------------------------------------------------------------
//	
//	/* 配置PC0引脚中断模式 */
//	//----------------------------------------------------------------------------------
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);			// 使能映射时钟
//    
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource0); // 将中断线0映射到PC0线上
//    
//	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;			// 选择为中断
//	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; 		// 下降沿中断
//	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
//	EXTI_InitStruct.EXTI_Line = EXTI_Line0;						// 选择外部中断线0
//	EXTI_Init(&EXTI_InitStruct);								
//	
//	 /* 配置NVIC */
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;		// 抢占优先级
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;				// 响应优先级
//	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;	
//	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn; 				// 中断通道：外部中断线0
//	NVIC_Init(&NVIC_InitStruct);
//}


// 解码NEC编码格式的信息
// 注：协议码的时长根据需要，可自行限定判断区间
// 注：TIM2的计数周期为1us，溢出值为20000
//------------------------------------------------------------------------------------------
u8 NEC_IR_decode_message(void)
{
	u8 NEC_decode_cnt;
	
	// 协议引导码的标准长度为13.5ms
	// 判断引导码的长度是否在(12ms,15ms)区间内
	//-----------------------------------------------------------
	if( Each_bit_duration[1]<12000 || Each_bit_duration[1]>15000 )
	{ return F_decode_error; }	// 不在规定的区间内，返回解码出错
	
	
	// 解码16位用户码
	//---------------------------------------------------------
	for(NEC_decode_cnt=2; NEC_decode_cnt<18; NEC_decode_cnt++)
	{
		// 协议数据"1"的标准长度为2.24ms
		// 协议数据位的长度在(2ms,2.5ms)区间内 => 协议数据"1"
		//-------------------------------------------------------------------------------------
		if( Each_bit_duration[NEC_decode_cnt]>2000 && Each_bit_duration[NEC_decode_cnt]<2500 )
		{ 
			Receive_user_code_16bit >>= 1;
			
			Receive_user_code_16bit |= 0x8000;
		}
		
		// 协议数据"0"的标准长度为1.12ms
		// 协议数据位的长度在(1ms,1.25ms)区间内 => 协议数据"0"
		//------------------------------------------------------------------------------------------
		else if( Each_bit_duration[NEC_decode_cnt]>1000 && Each_bit_duration[NEC_decode_cnt]<1250 )
		{
			Receive_user_code_16bit >>= 1;
			
			Receive_user_code_16bit &= ~0x8000;
		}
		
		// 协议数据位不在限定范围内，出错
		//-----------------------------
		else { return F_decode_error; }
	}
	
	// 解码8位数据码
	//---------------------------------------------------------
	for(NEC_decode_cnt=18; NEC_decode_cnt<26; NEC_decode_cnt++)
	{
		// 协议数据"1"的标准长度为2.24ms
		// 协议数据位的长度在(2ms,2.5ms)区间内 => 协议数据"1"
		//-------------------------------------------------------------------------------------
		if( Each_bit_duration[NEC_decode_cnt]>2000 && Each_bit_duration[NEC_decode_cnt]<2500 )
		{ 
			Receive_data_code_8bit >>= 1;
			
			Receive_data_code_8bit |= 0x80;
		}
		
		// 协议数据"0"的标准长度为1.12ms
		// 协议数据位的长度在(1ms,1.25ms)区间内 => 协议数据"0"
		//------------------------------------------------------------------------------------------
		else if( Each_bit_duration[NEC_decode_cnt]>1000 && Each_bit_duration[NEC_decode_cnt]<1250 )
		{
			Receive_data_code_8bit >>= 1;
			
			Receive_data_code_8bit &= ~0x80;
		}
		
		// 协议数据位不在限定范围内，出错
		//-----------------------------
		else { return F_decode_error; }
	}
	
	// 解码8位数据码的反码
	//---------------------------------------------------------
	for(NEC_decode_cnt=26; NEC_decode_cnt<34; NEC_decode_cnt++)
	{
		// 协议数据"1"的标准长度为2.24ms
		// 协议数据位的长度在(2ms,2.5ms)区间内 => 协议数据"1"
		//-------------------------------------------------------------------------------------
		if( Each_bit_duration[NEC_decode_cnt]>2000 && Each_bit_duration[NEC_decode_cnt]<2500 )
		{ 
			Receive_data_code_opposite >>= 1;
			
			Receive_data_code_opposite |= 0x80;
		}
		
		// 协议数据"0"的标准长度为1.12ms
		// 协议数据位的长度在(1ms,1.25ms)区间内 => 协议数据"0"
		//------------------------------------------------------------------------------------------
		else if( Each_bit_duration[NEC_decode_cnt]>1000 && Each_bit_duration[NEC_decode_cnt]<1250 )
		{
			Receive_data_code_opposite >>= 1;
			
			Receive_data_code_opposite &= ~0x80;
		}
		
		// 协议数据位不在限定范围内，出错
		//-----------------------------
		else { return F_decode_error; }
	}
	
	return F_decode_success;	// 解码成功	
}
//########################################################################################################
