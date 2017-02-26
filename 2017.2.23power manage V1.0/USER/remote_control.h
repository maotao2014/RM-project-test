#ifndef __REMOTE_CONTROL_H__
#define __REMOTE_CONTROL_H__

#include "stm32f4xx.h"

/*
	brief:定义遥控器信息结构体
				将接收到遥控器的信息先进行滤波，然后解码为按键信息
*/

//结构体定义
typedef struct
{
	struct
	{
		uint16_t ch0;
		uint16_t ch1;
		uint16_t ch2;
		uint16_t ch3;
		uint8_t s1;
		uint8_t s2;
	}rc;
	struct
	{
		int16_t x;
		int16_t y;
		int16_t z;
		uint8_t press_l;
		uint8_t press_r;
	}mouse;
	struct
	{
		uint16_t v;
	}key;
}RC_Ctl_t;


void RC_Decode(void);        //遥控器解码
#endif

