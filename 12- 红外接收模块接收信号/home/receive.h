#ifndef __RECEIVE_H
#define __RECEIVE_H
#include "stm32f10x.h"                  // Device header

#define IR_POWER		0xA2
#define IR_MODE			0x62
#define IR_MUTE			0xE2
#define IR_START_STOP	0x22
#define IR_PREVIOUS		0x02
#define IR_NEXT			0xC2
#define IR_EQ			0xE0
#define IR_VOL_MINUS	0xA8
#define IR_VOL_ADD		0x90
#define IR_0			0x68
#define IR_RPT			0x98
#define IR_USD			0xB0
#define IR_1			0x30
#define IR_2			0x18
#define IR_3			0x7A
#define IR_4			0x10
#define IR_5			0x38
#define IR_6			0x5A
#define IR_7			0x42
#define IR_8			0x4A
#define IR_9			0x52

void receive_Init(void);
void Infrared_low(uint32_t *low_time);
void Infrared_high(uint32_t *high_time);
uint8_t Guide_Repeat_Judgment(void);
uint8_t Infrared_Data_True(uint8_t *value);
void Receiving_Infrared_Data(void);
//获取红外发送过来的命令
uint8_t Get_Infrared_Command(void);
//获取红外发送过来的地址
uint8_t Get_Infrared_Address(void);
//清除红外发送过来的数据
void Clear_Infrared_Command(void);

#endif

