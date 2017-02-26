#ifndef __SPI_H__
#define __SPI_H__

#include "stm32f4xx.h"
#include "param.h"


//SPI 请求DMA传输枚举体
typedef enum
{
	DMA_ENABLE,           
	DMA_DISABLE,
}SPI_DMA_ENABLE;

//SPI传输结束后允许中断枚举体
typedef enum
{
  INTER_ENABLE,
	INTER_DISABLE,
}SPI_INTER_ENABLE;


#define SPI_CS_HIGH()      GPIO_SetBits(GPIOA, GPIO_Pin_4)
#define SPI_CS_LOW()       GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define SPI1_RECIVE_NUM   4              //一个通道电流采样值的存放个数


/*******外部变量声明*******/


/*******外部变量声明结束**************/


void SPI1_Config(SPI_DMA_ENABLE dma_enable,SPI_INTER_ENABLE inter_enable);
void SPI2_Config(SPI_DMA_ENABLE dma_enable,SPI_INTER_ENABLE inter_enable);
void SPI1_SendByte(u16 byt);

#endif


