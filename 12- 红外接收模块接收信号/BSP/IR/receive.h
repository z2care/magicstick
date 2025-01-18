#ifndef __RECEIVE_H
#define __RECEIVE_H
#include "stm32f10x.h"                  // Device header

void receive_Init(void);
void Infrared_low(uint32_t *low_time);
void Infrared_high(uint32_t *high_time);
uint8_t Guide_Repeat_Judgment(void);
uint8_t Infrared_Data_True(uint8_t *value);
void Receiving_Infrared_Data(void);
//��ȡ���ⷢ�͹���������
uint8_t Get_Infrared_Command(void);
//������ⷢ�͹���������
void Clear_Infrared_Command(void);

#endif

