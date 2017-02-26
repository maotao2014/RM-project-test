#ifndef __USART_H__
#define __USART_H__

#include "stm32f4xx.h" 


#define REMOTE_CONTROL

#ifdef REMOTE_CONTROL

#define DMA_USART2 2
#define DMA_USART3 3
#define DMA_USART4 4
#define DMA_USART  DMA_USART3           //定义用DMA接收的USART模块，用于接收遥控器信息   默认为USART3

//以下为不同USART接收时所对应的DMA通道、数据流、外设地址，详见stm32f4xxx参考手册205页
//注意！ 如果使能了中断函数，相应的中断服务函数必须手动添加  DMA相关中断函数在dma.c文件中添加，
//       usart相关中断服务函数在USART.c文件添加，方便管理
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


void usart1_init(void);    //usart1初始化

#endif


