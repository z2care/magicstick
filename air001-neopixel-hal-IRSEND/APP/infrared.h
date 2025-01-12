#ifndef	__INFRARED_H
#define	__INFRARED_H


#include "air001xx_hal.h"
//#include "bit_band.h"
#include "delay.h"
#include "main.h"

// �������������ݵ�ȫ�ֱ���
//------------------------------------------------------------
extern uint32_t Each_bit_duration[64];		// ��¼ÿһλ���ݵ�ʱ��

extern uint8_t  Current_bit_CNT;				// ��ǰ������յ�λ��

extern uint16_t Receive_user_code_16bit ;	// ��¼16λ�û���

extern uint8_t  Receive_data_code_8bit ;		// ��¼8λ������

extern uint8_t  Receive_data_code_opposite ;	// ��¼8λ������ķ���
//------------------------------------------------------------

#define	F_decode_error		1			// �������
#define	F_decode_success	0			// ����ɹ�

void Infrared_IE_Init_JX(void);	// ��ʼ�����ⷢ��ܽţ�Infrared_IE = PC1

void NEC_IE_Send_zero(void);	// NECЭ������"0"

void NEC_IE_Send_one(void);		// NECЭ������"1"

void NEC_IE_One_Data(uint8_t IE_One_Data);	// ��һ֡���ݵ���Ϊ�����ز������ȥ

void NEC_IE_code_message(uint16_t user_code, uint8_t data_code);	// ����NEC�����ʽ����Ϣ

void Infrared_IR_Init_JX(void);	// ��ʼ�����ⷢ��ܽţ�Infrared_IR = PC0

uint8_t NEC_IR_decode_message(void);	// ����NEC�����ʽ����Ϣ

#endif	/* __INFRARED_H */
