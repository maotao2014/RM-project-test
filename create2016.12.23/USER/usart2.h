#ifndef __USART2_H
#define __USART2_H


#include "stm32f4xx.h" 

void usart2_init(void);


extern u8 Res1;
//���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
//����봮���жϽ��գ��벻Ҫע�����º궨��

#endif
