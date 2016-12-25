#include "dma.h"																	   	  
#include "delay.h"	
#include "stdio.h"

extern void translate(void);
extern u16  ch0,ch1,ch2,ch3;

extern u16  pwm0,pwm1,pwm2,pwm3;

extern u8  sbus_rx_buffer[18];
extern u8 s1,s2;
extern void suduzhuanhuan(void);


extern  int map(long x,int in_min,int in_max,int out_min,int out_max ); 

extern void CAN_SetMsg(void);

extern int sudu;


//DMAx的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMA通道选择,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
//par:外设地址
//mar:存储器地址
//ndtr:数据传输量  
void MYDMA_Config(DMA_Stream_TypeDef *DMA_Streamx,u32 chx,u32 par,u32 mar,u16 ndtr)
{ 
	 
	NVIC_InitTypeDef nvic;
	DMA_InitTypeDef  DMA_InitStructure;
	
	if((u32)DMA_Streamx>(u32)DMA2)//得到当前stream是属于DMA2还是DMA1
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2时钟使能 
		
	}else 
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA1时钟使能 
	}
  DMA_DeInit(DMA_Streamx);
	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}//等待DMA可配置 
	
  /* 配置 DMA Stream */
  DMA_InitStructure.DMA_Channel            	= DMA_Channel_4;  //通道选择
  DMA_InitStructure.DMA_PeripheralBaseAddr 	= (u32)&USART1->DR;//DMA外设地址
  DMA_InitStructure.DMA_Memory0BaseAddr    	= (u32)sbus_rx_buffer;//DMA 存储器0地址
  DMA_InitStructure.DMA_DIR 				= DMA_DIR_PeripheralToMemory;//存储器到外设模式
  DMA_InitStructure.DMA_BufferSize     		= 18;//数据传输量 
  DMA_InitStructure.DMA_PeripheralInc 		= DMA_PeripheralInc_Disable;//外设非增量模式
  DMA_InitStructure.DMA_MemoryInc 			= DMA_MemoryInc_Enable;//存储器增量模式
  DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_Byte;//外设数据长度:8位
  DMA_InitStructure.DMA_MemoryDataSize 		= DMA_MemoryDataSize_Byte;//存储器数据长度:8位
  DMA_InitStructure.DMA_Mode 				= DMA_Mode_Circular;// 使用普通模式 
  DMA_InitStructure.DMA_Priority			= DMA_Priority_VeryHigh;//中等优先级
  DMA_InitStructure.DMA_FIFOMode 			= DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold 		= DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst 		= DMA_MemoryBurst_Single;//存储器突发单次传输
  DMA_InitStructure.DMA_PeripheralBurst		= DMA_PeripheralBurst_Single;//外设突发单次传输
  DMA_Init(DMA_Streamx, &DMA_InitStructure);//初始化DMA Stream
		
		
		

	nvic.NVIC_IRQChannel = DMA2_Stream5_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority =0;
	nvic.NVIC_IRQChannelSubPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);	 
     

	DMA_ITConfig(DMA2_Stream5,DMA_IT_TC,ENABLE);
	DMA_Cmd(DMA2_Stream5,ENABLE);

	

} 
//开启一次DMA传输
//DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7 
//ndtr:数据传输量  
void MYDMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr)
{
// 
//	DMA_Cmd(DMA_Streamx, DISABLE);                      //关闭DMA传输 
//	
//	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}	//确保DMA可以被设置  
//		
//	DMA_SetCurrDataCounter(DMA_Streamx,ndtr);          //数据传输量  
 
	DMA_Cmd(DMA_Streamx, ENABLE);                      //开启DMA传输 
}	  

void DMA2_Stream5_IRQHandler(void)
{	
											

	
	if(DMA_GetITStatus(DMA2_Stream5, DMA_IT_TCIF5))
		{

	DMA_ClearFlag(DMA2_Stream5, DMA_FLAG_TCIF5);
	DMA_ClearITPendingBit(DMA2_Stream5, DMA_IT_TCIF5);

	translate();										//获取遥控器值
	CAN_SetMsg();										//发送底盘数据


//	pwm0=map(ch0,364,1684,0,200);
//	pwm1=map(ch1,364,1684,0,200);
//	pwm2=map(ch2,364,1684,100,200);
//	pwm3=map(ch3,364,1684,100,200);
//	
//		TIM_SetCompare1(TIM4,pwm1);	
//		TIM_SetCompare2(TIM4,pwm1);	//修改比较值，修改占空比
//		TIM_SetCompare3(TIM4,pwm1);	//修改比较值，修改占空比
//		TIM_SetCompare4(TIM4,pwm1);	//修改比较值，修改占空比
			
		}
}



