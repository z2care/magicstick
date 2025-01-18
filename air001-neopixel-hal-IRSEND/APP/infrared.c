#include "infrared.h"

// �������������ݵ�ȫ�ֱ���
//--------------------------------------------------------
uint32_t Each_bit_duration[64] = {0};	// ��¼ÿһλ���ݵ�ʱ��

uint8_t	Current_bit_CNT = 0;			// ��ʾ��ǰ�ǵڼ�λ

uint16_t Receive_user_code_16bit = 0 ;	// ��¼16λ�û���

uint8_t  Receive_data_code_8bit = 0 ;	// ��¼8λ������

uint8_t  Receive_data_code_opposite = 0;	// ��¼8λ������ķ���
//--------------------------------------------------------





// ����Ϊ���ⷢ�����غ���
//########################################################################################################

// �ز�����ʱ����ʱ����
//==============================================================

// ����SysTick��ʱ������ʱ����
// �ŵ㣺�������޹أ���ʱʱ�ӱȽ�׼ȷ
// ȱ�㣺������ȫ��׼��8.77us��17.53us
//---------------------------------------
#define		delay8_77us()	delay_us(8)
#define		delay17_53us()	delay_us(17)
//---------------------------------------


/*
// ��������ָ�����ʱ����
// �ŵ㣺�ܴﵽ�Ƚ�׼ȷ��8.77us��17.53us
// ȱ�㣺�������йأ������޸ĺ���Ż������ܵ�����ʱ��׼
// ���飺�ڳ���ܹ�������ȷ��������ʾ�����ȹ���΢������ֵ
//-----------------------------------------------------
volatile void delay8_77us(void)     // ��ʱ8.77us
{
	u8 i=68;		// ����ȫ׼ȷ
	while(i--);		// ����ʾ����������
}

volatile void delay17_53us(void)    // ��ʱ17.53us
{
	u8 i=136;		// ����ȫ׼ȷ
	while(i--);		// ����ʾ����������
}
//----------------------------------------------------
*/

//==============================================================




// ��ʼ�����ⷢ��ܽţ�Infrared_IE = PC1
//----------------------------------------------------------------------------------------------
//void Infrared_IE_Init_JX(void)
//{
//	GPIO_InitTypeDef GPIO_InitStruct;                       // ����һ��GPIO_InitTypeDef���͵ı���
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);   // ����GPIOCʱ��
//	
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;                  // GPIO_Pin_1
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;           // ͨ���������
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;          // 50MHz�ٶ�
//	GPIO_Init(GPIOC, &GPIO_InitStruct);
//	
//	GPIO_ResetBits(GPIOC, GPIO_Pin_1);                      //PC1����
//}
//----------------------------------------------------------------------------------------------


//*********************************** �ز����� *********************************************
//
//	26.3us��һ�����ڣ�= 8.77us��IEΪ�ߣ��������⣩+ 17.53us��IEΪ�ͣ����������⣩
//	ע��һ���ز������У���������8.77us  + ����������17.53us��������ھ����ز��������ڡ�
//	ע���ز����������У����������ռ�ձ�һ��Ϊ1/3��

//	26.3us��һ�����ڣ�= 26.3us��IEΪ�ͣ����������⣩
//	ע�����������ڣ������������⣬�������Ϊ�ز����������ڡ�

//******************************************************************************************


// NECЭ������"0"= �ز�����0.56ms + �ز�������0.56ms
//-------------------------------------------------
void NEC_IE_Send_zero(void)     
{
	uint8_t i;
	
	// �ز�����0.56ms �� 26.3us * 21
	//-------------------------------
	for(i=0;i<22;i++)
	{
        //26.3us���ز��������ڣ�
		//------------------------------------
		HAL_GPIO_WritePin(IE_GPIO_Port,IE_Pin,GPIO_PIN_SET);	// IȨ�ߣ���������
		delay8_77us();  // ��ʱ8.77us                        
											
		HAL_GPIO_WritePin(IE_GPIO_Port,IE_Pin,GPIO_PIN_RESET);// IE���ͣ�����������
	
		delay17_53us(); // ��ʱ17.53us
		//------------------------------------
	}

	
	// �ز�������0.56ms �� 26.3us * 21
	//-------------------------------
	for(i=0;i<21;i++)
	{
        //26.3us���ز����������ڣ�
		//------------------------------------
		HAL_GPIO_WritePin(IE_GPIO_Port,IE_Pin,GPIO_PIN_RESET);// IE���ͣ�����������
		delay8_77us();  // ��ʱ8.77us
                                            
		HAL_GPIO_WritePin(IE_GPIO_Port,IE_Pin,GPIO_PIN_RESET);// IE���ͣ�����������
		delay17_53us(); // ��ʱ17.53us
		//------------------------------------
	}
}
//-------------------------------------------------


// NECЭ������"1" = �ز�����0.56ms + �ز�������1.68ms
//-------------------------------------------------
void NEC_IE_Send_one(void)
{
	uint8_t i;
	
	// �ز�����0.56ms �� 26.3us * 21
	//-------------------------------
	for(i=0;i<22;i++)
	{
		//26.3us���ز��������ڣ�
		//------------------------------------
		HAL_GPIO_WritePin(IE_GPIO_Port,IE_Pin,GPIO_PIN_SET);    // IȨ�ߣ���������
		delay8_77us();	// ��ʱ8.77us
		
		HAL_GPIO_WritePin(IE_GPIO_Port,IE_Pin,GPIO_PIN_RESET);  // IE���ͣ�����������
		delay17_53us();	// ��ʱ17.53us
		//------------------------------------
	}
	
	// �ز�������1.68ms �� 26.3us * 64
	//--------------------------------
	for(i=0;i<64;i++)
	{
		//26.3us���ز����������ڣ�
		//------------------------------------
		HAL_GPIO_WritePin(IE_GPIO_Port,IE_Pin,GPIO_PIN_RESET);// IE���ͣ�����������
		delay8_77us();	// ��ʱ8.77us
		
		HAL_GPIO_WritePin(IE_GPIO_Port,IE_Pin,GPIO_PIN_RESET);// IE���ͣ�����������
		delay17_53us();	// ��ʱ17.53us
		//------------------------------------
	}
}
//-------------------------------------------------


// ��һ֡���ݵ���ΪNECЭ��涨�ĺ����ز������ȥ
// һ֡���ݸ�ʽ����λ��ǰ���ɵ͵��߷���8λ����
// ��֡���ݿ����ǣ���16λ�û�����ֽ�(8λ) / ��16λ�û�����ֽ�(8λ) / ��8λ������ / ��8λ������ķ���
//----------------------------------------------------------------------------------------------
void NEC_IE_One_Data(uint8_t IE_One_Data)
{
	uint8_t i;
	
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



// ����NEC�����ʽ����Ϣ
//----------------------------------------------------------------------------------------------------------------------
// NEC �����ʽ�� �������� + ��16λ�û�����ֽ�(8λ) + ��16λ�û�����ֽ�(8λ) + ��8λ������ + ��8λ������ķ��� + �޽����롮0�� 
//----------------------------------------------------------------------------------------------------------------------
void NEC_IE_code_message(uint16_t user_code_16bit, uint8_t data_code_8bit)
{
	uint16_t i;
	
	uint8_t T_user_code_high = user_code_16bit>>8;
	
	uint8_t T_user_code_low = user_code_16bit;

	// �������룺�ز�����9ms��������4.5ms
	//------------------------------------------------------------
	// �ز����� 9ms �� 26.3us * 342
	for(i=0;i<342;i++) 
	{
		HAL_GPIO_WritePin(IE_GPIO_Port,IE_Pin,GPIO_PIN_SET);	// IȨ�ߣ���������
//		HAL_GPIO_WritePin(IE_GPIO_Port,IE_Pin,GPIO_PIN_SET);
		delay8_77us();	// ��ʱ8.77us
		
		HAL_GPIO_WritePin(IE_GPIO_Port,IE_Pin,GPIO_PIN_RESET);;	// IE���ͣ�����������
		delay17_53us();	// ��ʱ17.53us
	}
	
	// �ز������� 4.5ms �� 26.3us * 171
	for(i=0;i<171;i++)
	{
		HAL_GPIO_WritePin(IE_GPIO_Port,IE_Pin,GPIO_PIN_RESET);// IE���ͣ�����������
		delay8_77us();	// ��ʱ8.77us
		
		HAL_GPIO_WritePin(IE_GPIO_Port,IE_Pin,GPIO_PIN_RESET);// IE���ͣ�����������
		delay17_53us();	// ��ʱ17.53us
	}
	//------------------------------------------------------------
	
	
	//------------------------------------------------------------
	// ��16λ�û�����ֽ�	��8λ����
	NEC_IE_One_Data(T_user_code_low);
	
	// ��16λ�û�����ֽڣ�8λ����
	NEC_IE_One_Data(T_user_code_high);
	
	// ��8λ������ ��8λ����
	NEC_IE_One_Data(data_code_8bit);
	
	// ��8λ������ķ��룺8λ����
	NEC_IE_One_Data(~data_code_8bit);
	
	//------------------------------------------------------------
	
	
	// �޽����롮0��
	//--------------
	NEC_IE_Send_zero();
	//--------------
}

//########################################################################################################



// ����Ϊ������յ���غ���
//########################################################################################################

// ��ʼ�����ⷢ��ܽţ�Infrared_IR = PC0
//--------------------------------------------------------------------------------
//void Infrared_IR_Init_JX(void) 
//{
//	GPIO_InitTypeDef GPIO_InitStruct; 
//	EXTI_InitTypeDef EXTI_InitStruct;
//	NVIC_InitTypeDef NVIC_InitStruct;
//	
//	/* ����PC0���� */
//	//---------------------------------------------------------------------------
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);		// ʹ��PC�˿�ʱ��											
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;	                // ��������ģʽ
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;						// PC0
//	GPIO_Init(GPIOC, &GPIO_InitStruct);	
//	//---------------------------------------------------------------------------
//	
//	/* ����PC0�����ж�ģʽ */
//	//----------------------------------------------------------------------------------
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);			// ʹ��ӳ��ʱ��
//    
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource0); // ���ж���0ӳ�䵽PC0����
//    
//	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;			// ѡ��Ϊ�ж�
//	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; 		// �½����ж�
//	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
//	EXTI_InitStruct.EXTI_Line = EXTI_Line0;						// ѡ���ⲿ�ж���0
//	EXTI_Init(&EXTI_InitStruct);								
//	
//	 /* ����NVIC */
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;		// ��ռ���ȼ�
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;				// ��Ӧ���ȼ�
//	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;	
//	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn; 				// �ж�ͨ�����ⲿ�ж���0
//	NVIC_Init(&NVIC_InitStruct);
//}


// ����NEC�����ʽ����Ϣ
// ע��Э�����ʱ��������Ҫ���������޶��ж�����
// ע��TIM2�ļ�������Ϊ1us�����ֵΪ20000
//------------------------------------------------------------------------------------------
uint8_t NEC_IR_decode_message(void)
{
	uint8_t NEC_decode_cnt;
	
	// Э��������ı�׼����Ϊ13.5ms
	// �ж�������ĳ����Ƿ���(12ms,15ms)������
	//-----------------------------------------------------------
	if( Each_bit_duration[1]<12000 || Each_bit_duration[1]>15000 )
	{ return F_decode_error; }	// ���ڹ涨�������ڣ����ؽ������
	
	
	// ����16λ�û���
	//---------------------------------------------------------
	for(NEC_decode_cnt=2; NEC_decode_cnt<18; NEC_decode_cnt++)
	{
		// Э������"1"�ı�׼����Ϊ2.24ms
		// Э������λ�ĳ�����(2ms,2.5ms)������ => Э������"1"
		//-------------------------------------------------------------------------------------
		if( Each_bit_duration[NEC_decode_cnt]>2000 && Each_bit_duration[NEC_decode_cnt]<2500 )
		{ 
			Receive_user_code_16bit >>= 1;
			
			Receive_user_code_16bit |= 0x8000;
		}
		
		// Э������"0"�ı�׼����Ϊ1.12ms
		// Э������λ�ĳ�����(1ms,1.25ms)������ => Э������"0"
		//------------------------------------------------------------------------------------------
		else if( Each_bit_duration[NEC_decode_cnt]>1000 && Each_bit_duration[NEC_decode_cnt]<1250 )
		{
			Receive_user_code_16bit >>= 1;
			
			Receive_user_code_16bit &= ~0x8000;
		}
		
		// Э������λ�����޶���Χ�ڣ�����
		//-----------------------------
		else { return F_decode_error; }
	}
	
	// ����8λ������
	//---------------------------------------------------------
	for(NEC_decode_cnt=18; NEC_decode_cnt<26; NEC_decode_cnt++)
	{
		// Э������"1"�ı�׼����Ϊ2.24ms
		// Э������λ�ĳ�����(2ms,2.5ms)������ => Э������"1"
		//-------------------------------------------------------------------------------------
		if( Each_bit_duration[NEC_decode_cnt]>2000 && Each_bit_duration[NEC_decode_cnt]<2500 )
		{ 
			Receive_data_code_8bit >>= 1;
			
			Receive_data_code_8bit |= 0x80;
		}
		
		// Э������"0"�ı�׼����Ϊ1.12ms
		// Э������λ�ĳ�����(1ms,1.25ms)������ => Э������"0"
		//------------------------------------------------------------------------------------------
		else if( Each_bit_duration[NEC_decode_cnt]>1000 && Each_bit_duration[NEC_decode_cnt]<1250 )
		{
			Receive_data_code_8bit >>= 1;
			
			Receive_data_code_8bit &= ~0x80;
		}
		
		// Э������λ�����޶���Χ�ڣ�����
		//-----------------------------
		else { return F_decode_error; }
	}
	
	// ����8λ������ķ���
	//---------------------------------------------------------
	for(NEC_decode_cnt=26; NEC_decode_cnt<34; NEC_decode_cnt++)
	{
		// Э������"1"�ı�׼����Ϊ2.24ms
		// Э������λ�ĳ�����(2ms,2.5ms)������ => Э������"1"
		//-------------------------------------------------------------------------------------
		if( Each_bit_duration[NEC_decode_cnt]>2000 && Each_bit_duration[NEC_decode_cnt]<2500 )
		{ 
			Receive_data_code_opposite >>= 1;
			
			Receive_data_code_opposite |= 0x80;
		}
		
		// Э������"0"�ı�׼����Ϊ1.12ms
		// Э������λ�ĳ�����(1ms,1.25ms)������ => Э������"0"
		//------------------------------------------------------------------------------------------
		else if( Each_bit_duration[NEC_decode_cnt]>1000 && Each_bit_duration[NEC_decode_cnt]<1250 )
		{
			Receive_data_code_opposite >>= 1;
			
			Receive_data_code_opposite &= ~0x80;
		}
		
		// Э������λ�����޶���Χ�ڣ�����
		//-----------------------------
		else { return F_decode_error; }
	}
	
	return F_decode_success;	// ����ɹ�	
}
//########################################################################################################