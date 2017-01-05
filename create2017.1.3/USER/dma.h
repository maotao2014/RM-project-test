#ifndef __DMA_H
#define	__DMA_H	   
#include "stm32f4xx.h"
#include "param.h"


void DMA_Config(DMA_Stream_TypeDef *DMA_Streamx,u32 chx,u32 par,u32 mar,u16 ndtr);//配置DMAx_CHx
void DMA_NVIC_Config(IRQn_Type DMAx_Streamy_IRQn,DMA_Stream_TypeDef* DMAx_Streamy,u32 DMA_inter_en_def,u8 pp,u8 sb);
void DMA_Enable(DMA_Stream_TypeDef *DMA_Streamx);	                                   //使能一次DMA传输		   
#endif

