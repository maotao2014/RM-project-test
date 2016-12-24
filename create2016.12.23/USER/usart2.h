#ifndef __USART2_H
#define __USART2_H


#include "stm32f4xx.h" 

void usart2_init(void);

extern u8 USART2_RX_Flag;

extern u8 Res1;
//接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	
//如果想串口中断接收，请不要注释以下宏定义

#endif
