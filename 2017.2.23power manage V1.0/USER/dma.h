#ifndef __DMA_H
#define	__DMA_H	   
#include "stm32f4xx.h"

//#include "delay.h"	
//#include "stdio.h"
//#include "USART.h"
//#include "spi.h"

/******外部变量申明区********/

/******外部变量申明区结束********/

void DMA_Config(DMA_Stream_TypeDef *DMAy_Streamx,u32 chx,u32 par,u32 mar,u16 ndtr,u32 peripheral_dataSize,u32 memory_datasize);//配置DMAx_CHx
void DMA_NVIC_Config(IRQn_Type DMAx_Streamy_IRQn,DMA_Stream_TypeDef* DMAx_Streamy,u32 DMA_inter_en_def,u8 pp,u8 sb);
void DMA_Enable(DMA_Stream_TypeDef *DMA_Streamx);	                                   //使能一次DMA传输		   
#endif

