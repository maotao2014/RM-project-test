#ifndef __USART_H__
#define __USART_H__

#include "stm32f4xx.h" 


#define REMOTE_CONTROL

#ifdef REMOTE_CONTROL

#define DMA_USART2 2
#define DMA_USART3 3
#define DMA_USART4 4
#define DMA_USART  DMA_USART3           //������DMA���յ�USARTģ�飬���ڽ���ң������Ϣ   Ĭ��ΪUSART3

//����Ϊ��ͬUSART����ʱ����Ӧ��DMAͨ�����������������ַ�����stm32f4xxx�ο��ֲ�205ҳ
//ע�⣡ ���ʹ�����жϺ�������Ӧ���жϷ����������ֶ����  DMA����жϺ�����dma.c�ļ�����ӣ�
//       usart����жϷ�������USART.c�ļ���ӣ��������
#if   DMA_USART==DMA_USART2
	#define DMA_USART_STREAM     DMA1_Stream5
	#define DMA_USART_CHANNEL    DMA_Channel_4
	#define DMA_USART_DR         USART2->DR
	#define DMA_USART_IRQn       DMA1_Stream5_IRQn
#elif DMA_USART==DMA_USART3    
  #define DMA_USART_STREAM     DMA1_Stream1
	#define DMA_USART_CHANNEL    DMA_Channel_4
	#define DMA_USART_DR         USART3->DR
	#define DMA_USART_IRQn       DMA1_Stream1_IRQn
#elif DMA_USART==DMA_USART4
  #define DMA_USART_STREAM     DMA1_Stream2
	#define DMA_USART_CHANNEL    DMA_Channel_4
	#define DMA_USART_DR         UART4->DR
	#define DMA_USART_IRQn       DMA1_Stream2_IRQn
#endif


void    remote_usart_init(void);
#endif   //end  ifdef REMOTE_CONTROL!


void usart1_init(void);    //usart1��ʼ��

#endif


