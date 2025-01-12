#ifndef	__INFRARED_H
#define	__INFRARED_H


#include "stm32g0xx_hal.h"
#include "bit_band.h"
#include "delay.h"
#include "main.h"

// 缓存红外接收数据的全局变量
//------------------------------------------------------------
extern u32 Each_bit_duration[64];		// 记录每一位数据的时长

extern u8  Current_bit_CNT;				// 当前红外接收的位数

extern u16 Receive_user_code_16bit ;	// 记录16位用户码

extern u8  Receive_data_code_8bit ;		// 记录8位数据码

extern u8  Receive_data_code_opposite ;	// 记录8位数据码的反码
//------------------------------------------------------------

#define	F_decode_error		1			// 解码出错
#define	F_decode_success	0			// 解码成功



void Infrared_IE_Init_JX(void);	// 初始化红外发射管脚：Infrared_IE = PC1

void NEC_IE_Send_zero(void);	// NEC协议数据"0"

void NEC_IE_Send_one(void);		// NEC协议数据"1"

void NEC_IE_One_Data(u8 IE_One_Data);	// 将一帧数据调制为红外载波发射出去

void NEC_IE_code_message(u16 user_code, u8 data_code);	// 发送NEC编码格式的信息



void Infrared_IR_Init_JX(void);	// 初始化红外发射管脚：Infrared_IR = PC0

u8 NEC_IR_decode_message(void);	// 解码NEC编码格式的信息




	
#endif	/* __INFRARED_H */
