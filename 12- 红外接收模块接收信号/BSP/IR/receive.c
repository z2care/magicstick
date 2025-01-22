#include "stm32f10x.h"                  // ����STM32F10xϵ�е�ͷ�ļ����ṩ��Ҫ�ĺ궨�塢����������
#include "receive.h"
#include "Delay.h"
#include "uart.h"
#include "stdio.h"

extern uint8_t ir_received;

typedef struct Infrared_Data{

    uint8_t AddressCode;            //��ַ��
    uint8_t AddressInverseCode;     //��ַ����
    uint8_t CommandCode;            //������
    uint8_t CommandInverseCode;     //�����

}Infrared_Data_Struct;

Infrared_Data_Struct InfraredData;

void receive_Init(void){
    /* ����ʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 		// ����GPIOA��ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 		// ����GPIOB��ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  		// ����AFIO��ʱ�ӣ������ⲿ�ж�������

    /* GPIO��ʼ�� */
    GPIO_InitTypeDef GPIO_InitStructure; 						// ����GPIO��ʼ���ṹ��
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 				// ����GPIOģʽΪ��������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 					// ѡ��GPIOA��PA3����
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 			// ����GPIO�ٶ�Ϊ50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure); 						// ����GPIO_InitStructure��ʼ��GPIOA��PA3����

    /* ʹ��EXTI�ж���· */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource3); // ��PA3��������Ϊ�ⲿ�ж���3

    EXTI_InitTypeDef EXTI_InitStructure; 						// �����ⲿ�жϳ�ʼ���ṹ��
    EXTI_InitStructure.EXTI_Line = EXTI_Line3; 					// ѡ���ⲿ�ж���3
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; 		// �����ⲿ�ж�ģʽΪ�ж�
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 	// �����жϴ�����ʽΪ�½��ش���
    EXTI_InitStructure.EXTI_LineCmd = ENABLE; 					// ʹ���ⲿ�ж���3
    EXTI_Init(&EXTI_InitStructure); 							// ����EXTI_InitStructure��ʼ���ⲿ�ж�

    NVIC_InitTypeDef NVIC_InitStructure; 						// �����жϿ�������ʼ���ṹ��
    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn; 			// ѡ���ⲿ�ж�3�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;// �����ж���ռ���ȼ�Ϊ1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01; 		// �����ж���Ӧ���ȼ�Ϊ1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			// ʹ���ж�ͨ��
    NVIC_Init(&NVIC_InitStructure); 							// ����NVIC_InitStructure��ʼ���жϿ�����
	
}

//��ȡ����͵�ƽʱ��
//������͵�ƽ9ms�������룺��9ms�ĵ͵�ƽ+4.5ms�ĸߵ�ƽ��ɡ�
void Infrared_low(uint32_t *low_time) {
    uint32_t time_val = 0;
    while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == 0) {
        if(time_val >= 500) {  			// ����10ms����Ϊ���ݽ���������9ms��ʱ
            *low_time = time_val;		//low_time=500������ѭ��
            return;
        }
        Delay_us(20);  // ��ʱ20΢��
        time_val++;
    }
    *low_time = time_val;
}

//��ȡ����ߵ�ƽʱ��
//������ߵ�ƽ9ms�������룺��9ms�ĵ͵�ƽ+4.5ms�ĸߵ�ƽ��ɡ�
void Infrared_high(uint32_t *high_time)
{
    uint32_t time_val = 0;
    while( GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == 1 )
    {
        if( time_val >= 250 )        // ����5ms����Ϊ���ݽ���������4.5ms��ʱ
        {
            *high_time = time_val;   //����ѭ����high_time=250
            return;
        }
        Delay_us(20);
        time_val++;
    }
    *high_time = time_val;
}
//�ظ��룬������һ�� 9ms �ĵ͵�ƽ��һ�� 2.5ms �ĸߵ�ƽ��ɡ�
//��һ�������ź���������ʱ������ͨ�������ظ���ķ�ʽ���ٷ��͡�
//��������ظ����ж�
uint8_t Guide_Repeat_Judgment(void) {
    uint32_t out_time=0;
    Infrared_low(&out_time);
    if((out_time > 500) || (out_time < 400)) {
        return 1;  // ����������
    }
    Infrared_high(&out_time);
    if((out_time > 250) || (out_time < 100)) {
        return 1;  // ����������
    }
    if((out_time > 100) && (out_time < 150)) {
        return 2;  // �ظ���
    }
    return 0;  // ������
}

//����������ȷ���ж�
uint8_t Infrared_Data_True(uint8_t *value) {
    if(value[0] != (uint8_t)(~value[1])) return 0;  // ��ַ�����ַ����У��
    if(value[2] != (uint8_t)(~value[3])) return 0;  // �������������У��
   printf("%x %x %x %x\r\n", value[0], value[1], value[2], value[3]);
	 //usart_send_String(&value[0]);	//�����ַ���
	 //usart_send_String(&value[1]);	//�����ַ���
	 //usart_send_String(&value[2]);	//�����ַ���
	 //usart_send_String(&value[3]);	//�����ַ���
    InfraredData.AddressCode = value[0];
    InfraredData.AddressInverseCode = value[1];
    InfraredData.CommandCode = value[2];
    InfraredData.CommandInverseCode = value[3];
    return 1;
}
//���պ�������
/*
	���ݷ���0�룺0.56ms�͵�ƽ+ 0.56ms�ĸߵ�ƽ
	���ݷ���1�룺0.56ms�͵�ƽ+ 1.68ms�ĸߵ�ƽ
	�յ�����λ0: 0.56ms�͵�ƽ+ 0.56ms�ĸߵ�ƽ
    �յ�����λ1: 0.56ms�͵�ƽ+ 1.68ms�ĸߵ�ƽ
*/
void Receiving_Infrared_Data(void) {
    
    uint16_t Group_num = 0, Data_num = 0;				 // ������������ڴ洢�����������������λ������  
    uint32_t time=0;									 // ������������ڴ洢ʱ�����ֵ			
    uint8_t Bit_data = 0;								 // ������������ڴ洢��������λ��ֵ��0��1��   
    uint8_t ir_value[5] = {0};							 // �������飬���ڴ洢���յ��ĺ������ݣ�4���ֽڵ����� + 1���ֽ���������Ŀ�ģ�
    uint8_t Guide_Repeat_Code = 0;						 // ������������ڴ洢��������ظ�����жϽ��
    Guide_Repeat_Code = Guide_Repeat_Judgment();		 // ���ú����жϽ��յ����Ƿ�����������ظ���		
  
    if(Guide_Repeat_Code == 1) {						 // ����жϽ�����������룬���ӡ������Ϣ����������
        //printf("err\r\n");
        return;
    }
		
		if(Guide_Repeat_Code != 1) {						 // ����жϽ�����������룬���ӡ������Ϣ����������
        //printf("err\r\n");
			//ir_received = 1;
        return;
    }

    for(Group_num = 0; Group_num < 4; Group_num++) {	 // ѭ��4�Σ�ÿ�δ���һ���ֽڵ�����	 
        for(Data_num = 0; Data_num < 8; Data_num++) {	 // ѭ��8�Σ�ÿ�δ���һ������λ	          
            Infrared_low(&time);						 // ���ú�����ȡ����͵�ƽʱ��
			
            if((time > 60) || (time < 20)) return;		 // ����͵�ƽʱ�䲻��0.4ms��1.2ms֮�䣬˵�����ݴ��󣬽�������           
            time = 0;									 // ����time������Ϊ��һ�β�����׼��          
            Infrared_high(&time);						 // ���ú�����ȡ����ߵ�ƽʱ��
			
            if((time >= 60) && (time < 100)) {			 // ����ߵ�ƽʱ����1.2ms��2ms֮�䣬˵�����յ���������λ1
                Bit_data = 1;
            }
            
            else if((time >= 10) && (time < 50)) {		 // ����ߵ�ƽʱ����0.2ms��1ms֮�䣬˵�����յ���������λ0
                Bit_data = 0;
            }
            
            ir_value[Group_num] <<= 1;					 // �����յ�������λ����1λ��Ϊ��һ������λ�ڳ�λ��	            
            ir_value[Group_num] |= Bit_data;			 // �����յ�������λд������	          
            time=0;										 // ����time������Ϊ��һ�β�����׼��					
        }
    }   
    Infrared_Data_True(ir_value);						 // ���ú����жϽ��յ��������Ƿ���ȷ����������ȷ����		
}

//��ȡ���ⷢ�͹���������
uint8_t Get_Infrared_Command(void)
{
    return InfraredData.CommandCode;
}
//������ⷢ�͹���������
void Clear_Infrared_Command(void)
{
    InfraredData.CommandCode = 0x00;
}

uint8_t temp2=0;
// �ⲿ�ж�3���жϷ�����
void EXTI3_IRQHandler(void)
{
    // ����Ƿ���EXTI3���ж�
    if(EXTI_GetITStatus(EXTI_Line3) != RESET)
    {
        // ���EXTI3���жϱ�־λ
        EXTI_ClearITPendingBit(EXTI_Line3);
        // �������д�жϴ������
        // ���磬�����жϽ��յ����ź�ʵ���л�һ��LED��״̬     
		// ����һ�κ�������
        //Receiving_Infrared_Data();
			ir_received = 1;
		
		//char temp1=Get_Infrared_Command();
		//temp2=Get_Infrared_Command();
		//printf("temp1=%x temp2=%x\r\n", temp1, temp2);
		// Clear_Infrared_Command();
        // �����жϴ������...
    }
}
