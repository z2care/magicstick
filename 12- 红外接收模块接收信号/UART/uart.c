#include "stm32f10x.h"                  // Device header
#include "stdio.h"
#include "uart.h"
#include <string.h>
volatile uint16_t usart1_rx_len = 0;            //����֡���ݵĳ���
volatile uint16_t usart1_tx_len = 0;            //����֡���ݵĳ���
volatile uint8_t usart1_recv_end_flag = 0;      //֡���ݽ�����ɱ�־

uint8_t DMA_USART1_RX_BUF[USART_MAX_LEN]={0};   //�������ݻ���
uint8_t DMA_USART1_TX_BUF[USART_MAX_LEN]={0};   //DMA���ͻ���



// �������ų�ʼ��
void Uart_Init(uint32_t __Baud) {
    // ����ʱ�ӣ�GPIO����ʱ�ӡ�GPIOAʱ�Ӻ�USART1ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 

    GPIO_InitTypeDef GPIO_InitStructure;

    // ��ʼ��GPIO�ṹ�壬������TX����
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; // TX����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // �����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // �ٶ�50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // ���³�ʼ��GPIO�ṹ�壬������RX����
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; // RX����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // ��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// ��USART1�Ĵ�������ΪĬ��ֵ
    //USART_DeInit(USART1);//��д, ����USART1�Ĵ���ΪĬ��ֵ����ȡ��֮ǰ������
	
    // ����UART
    USART_InitTypeDef USART_InitStructure; // �������ô��ڵĽṹ����� 
    // ��ʼ��USART�ṹ��
    USART_StructInit(&USART_InitStructure);
    USART_InitStructure.USART_BaudRate = __Baud; // ���ò�����
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; // �ֽڳ���Ϊ8λ
    USART_InitStructure.USART_StopBits = USART_StopBits_1; // 1��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No; // û��У��λ
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; // �շ�ģʽ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // ������
    USART_Init(USART1, &USART_InitStructure); // ��ʼ��USART1


		NVIC_InitTypeDef NVIC_InitStructure;

		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;         //����1�ж�ͨ��
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;   //��ռ���ȼ�2
		NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;         //�����ȼ�1
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQͨ��ʹ��
		NVIC_Init(&NVIC_InitStructure);                           //����ָ���Ĳ�����ʼ��VIC�Ĵ���

		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;  //Ƕ��ͨ��ΪDMA1_Channel4_IRQn
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //��ռ���ȼ�Ϊ 2
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;        //��Ӧ���ȼ�Ϊ 2
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //ͨ���ж�ʹ��
		NVIC_Init(&NVIC_InitStructure);

		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn ;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;   	//��ռ���ȼ�2
		NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 3;   	//�����ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE; 	//IRQͨ��ʹ��
		NVIC_Init(&NVIC_InitStructure);

		USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);         			//�������ڿ����ж�

    // ʹ��USART1
    USART_Cmd(USART1, ENABLE);
}
void DMA1_Init(void){
	
			/* ���ô���DMA����*/
		DMA_InitTypeDef DMA_InitStructure;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);                                          // ����DMAʱ��

		// RX DMA1 ͨ��5
		DMA_InitStructure.DMA_BufferSize = sizeof(DMA_USART1_RX_BUF);  // �����˽��յ���󳤶�
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;             // ���ڽ��գ�����������->�ڴ�
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                   // ���������赽�ڴ棬���Թر��ڴ浽�ڴ�
		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)DMA_USART1_RX_BUF;// �ڴ�Ļ���ַ��Ҫ�洢������
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;// �ڴ����ݿ�ȣ������ֽڴ洢
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;        // �ڴ������ÿ�δ����յ����ݴ����ڴ��У��´��յ��Զ��洢���ڴ����һ��λ��
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                  // ����ģʽ
		DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_BASE + 0x04; // ����Ļ���ַ�����ڵ����ݼĴ���
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // ��������ݿ�ȣ������ֽڴ洢�����ڴ�����ݿ��һ��
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; // ����ֻ��һ�����ݼĴ��� RDR�����������ַ������
		DMA_InitStructure.DMA_Priority = DMA_Priority_High; // ���ȼ�
		DMA_Init(DMA1_Channel5, &DMA_InitStructure);

		// TX DMA1 ͨ��4
		DMA_InitStructure.DMA_BufferSize = 0;                              // ���ͻ������Ĵ�С����ʼ��Ϊ0������
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                 // �����Ƿ��������赽�ڴ棬������ΪĿ�ĵ�
		DMA_InitStructure.DMA_MemoryBaseAddr =(uint32_t)DMA_USART1_TX_BUF; // �����ڴ��ַ�������﷢��
		DMA_Init(DMA1_Channel4, &DMA_InitStructure);

		USART_DMACmd(USART1, USART_DMAReq_Tx | USART_DMAReq_Rx, ENABLE);   // ʹ��DMA���ڷ��ͺͽ�������

		DMA_Cmd(DMA1_Channel5, ENABLE);     // ʹ�ܽ���
		DMA_Cmd(DMA1_Channel4, DISABLE);    // ��ֹ����

		DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE); // ʹ�ܽ���DMAͨ���ж�

		USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1
	
}

// ���ڷ��ͺ��� ���͵����ַ�
void usart_send_data(uint8_t ucch) {
    USART_SendData(USART1, (uint8_t)ucch);
    // �ȴ��������ݻ�����Ϊ��,USART_FLAG_TXE�������ݼĴ����ձ�־
    while (RESET == USART_GetFlagStatus(USART1, USART_FLAG_TXE)) {}

    // �ȴ��������,USART_FLAG_TC������ɱ�־
    while (RESET == USART_GetFlagStatus(USART1, USART_FLAG_TC)) {}
}

// �����ַ���
void usart_send_String(uint8_t *ucstr)
{
      while(ucstr && *ucstr)  // ��ַΪ�ջ���ֵΪ������
      {
            usart_send_data(*ucstr++);// ���͵�ǰ�ַ�,+1�ƶ�ָ�뵽��һ���ַ�
      }
}
/*���ڲ�ʹ��΢���ʱ����Ч�������������þ�������ʹ��printf�����ˣ����Ҽ����Ի���ߡ�*/
#if !defined(__MICROLIB)
//��ʹ��΢��Ļ�����Ҫ�������ĺ���
#if (__ARMCLIB_VERSION <= 6000000)
//�����������AC5  �Ͷ�����������ṹ��
struct __FILE
{
        int handle;
};
#endif

FILE __stdout;

//����_sys_exit()�Ա���ʹ�ð�����ģʽ
void _sys_exit(int x)
{
      x = x;
}
#endif

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)//��C��printf�����ض���USART
{  // ���ַ�����ch���͵�����USART1
    USART_SendData(USART1, (uint8_t)ch);

    // �ȴ���ֱ��USART1�����ݼĴ���Ϊ�գ��������Ѿ����ͳ�ȥ
    while(RESET == USART_GetFlagStatus(USART1, USART_FLAG_TXE)) {
        // ѭ���ȴ���ֱ��TXE�����ͻ�����Ϊ�գ���־λ��Ӳ������ΪSET
    }

    // ���ط��͵��ַ��������fputc������������Ϊ
    return ch;
}

void DMA_USART1_Send(uint8_t *data, uint16_t size)
{
      memcpy(DMA_USART1_TX_BUF, data, size); // �������ݵ�DMA���ͻ�����
      DMA_Cmd(DMA1_Channel4, DISABLE); // ȷ��DMA���Ա�����
      DMA_SetCurrDataCounter(DMA1_Channel4, size); // �������ݴ��䳤��
      DMA_Cmd(DMA1_Channel4, ENABLE); // ��DMAͨ������ʼ����	
}

void USART1_IRQHandler(void)  //����1�жϷ������
{
    if(USART_GetITStatus(USART1,USART_IT_IDLE)!=RESET) //�����жϴ���
    {
        usart1_recv_end_flag = 1;     // ������ɱ�־λ��1

        DMA_Cmd(DMA1_Channel5, DISABLE);  /* ��ʱ�ر�dma��������δ���� */
        usart1_rx_len = USART_MAX_LEN - DMA_GetCurrDataCounter(DMA1_Channel5);/* ��ȡ���յ������ݳ��� ��λΪ�ֽ�*/
        DMA_ClearFlag(DMA1_FLAG_TC5);     /* ��DMA��־λ */

        DMA_USART1_Send(DMA_USART1_RX_BUF, usart1_rx_len); // �����յ����ݻ���

        DMA_SetCurrDataCounter(DMA1_Channel5,USART_MAX_LEN); /* ���¸�ֵ����ֵ��������ڵ��������ܽ��յ�������֡��Ŀ */
        DMA_Cmd(DMA1_Channel5, ENABLE);   /*��DMA*/

        USART_ReceiveData(USART1);   //��������жϱ�־λ�����պ��������־λ�����ã�
    }

    if(USART_GetFlagStatus(USART1,USART_IT_TC)==RESET) //��������ж�
    {
        USART_ClearITPendingBit(USART1, USART_IT_TC); // �����������жϱ�־λ
        USART_ITConfig(USART1,USART_IT_TC,DISABLE);
        usart1_rx_len = 0;
    }
}

// DMA1_Channel5_IRQHandler����
void DMA1_Channel5_IRQHandler(void)
{
    if(DMA_GetFlagStatus(DMA1_FLAG_TC5) != RESET) // �ȴ�DMA1_Channel5�������
    {
        DMA_ClearFlag(DMA1_FLAG_TC5); // ���DMA1_Channel5������ɱ�־
        DMA_Cmd(DMA1_Channel5, DISABLE); // �ر�ʹ��
        USART_ITConfig(USART1, USART_IT_TC, ENABLE); // �򿪴��ڷ�������ж�
    }
}
