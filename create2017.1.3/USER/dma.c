#include "dma.h"																	   	  
#include "delay.h"	
#include "stdio.h"
#include "USART.h"

/**************外部变量声明区*****************/
u8 rm_DMA_end_flag;

/**************外部变量声明区*****************/

//DMAx的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMA通道选择,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
//par:外设地址
//mar:存储器地址
//ndtr:数据传输量  
void DMA_Config(DMA_Stream_TypeDef *DMAy_Streamx,u32 chx,u32 par,u32 mar,u16 ndtr)
{ 
	DMA_InitTypeDef  DMA_InitStructure;
	
	if((u32)DMAy_Streamx>(u32)DMA2)//得到当前stream是属于DMA2还是DMA1
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2时钟使能 
		
	}
	else 
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA1时钟使能 
	}
  DMA_DeInit(DMAy_Streamx);
	
	while (DMA_GetCmdStatus(DMAy_Streamx) != DISABLE){}//等待DMA可配置 
	
  /* 配置 DMA Stream */
  DMA_InitStructure.DMA_Channel            	= chx;  //通道选择
  DMA_InitStructure.DMA_PeripheralBaseAddr 	= par;  //DMA外设地址
  DMA_InitStructure.DMA_Memory0BaseAddr    	= mar;  //DMA 存储器地址
  DMA_InitStructure.DMA_DIR 				        = DMA_DIR_PeripheralToMemory;//由外设到存储器
  DMA_InitStructure.DMA_BufferSize     	  	= ndtr; //数据传输量 
  DMA_InitStructure.DMA_PeripheralInc 	  	= DMA_PeripheralInc_Disable;//外设非增量模式
  DMA_InitStructure.DMA_MemoryInc 			    = DMA_MemoryInc_Enable;//存储器增量模式
  DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_Byte;//外设数据长度:8位
  DMA_InitStructure.DMA_MemoryDataSize 		  = DMA_MemoryDataSize_Byte;//存储器数据长度:8位
  DMA_InitStructure.DMA_Mode 				        = DMA_Mode_Circular;// 使用循环模式                //此处值待定
  DMA_InitStructure.DMA_Priority		      	= DMA_Priority_VeryHigh;//最高优先等级             //此处值待定
  DMA_InitStructure.DMA_FIFOMode 		      	= DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold 	  	= DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst 	    	= DMA_MemoryBurst_Single;    //存储器突发单次传输
  DMA_InitStructure.DMA_PeripheralBurst	  	= DMA_PeripheralBurst_Single;//外设突发单次传输
  DMA_Init(DMAy_Streamx, &DMA_InitStructure);//初始化DMA Stream
		
	DMA_Cmd(DMAy_Streamx,ENABLE);
} 

/**
	* @brief:  DMA中断配置函数
	* @param:  DMAx_Streamy_IRQn x:1~2   y:0~7  
						 DMAx_Streamy      同上 
						 DMA_inter_en_def   DMA_interrupt_enable_definitions 
																参数可为 DMA_IT_TC
                                         DMA_IT_HT 
																				 DMA_IT_TE 
																				 DMA_IT_DME	
                                         DMA_IT_FE
						 pp              	 PreemptionPriority 抢占优先级
						 sb                SubPriority        子优先级 
	* @note :  一般配置为DMA传输结束产生中断 DMA_IT_TC
*/
void DMA_NVIC_Config(IRQn_Type DMAx_Streamy_IRQn,DMA_Stream_TypeDef* DMAx_Streamy,u32 DMA_inter_en_def,u8 pp,u8 sb)
{
	NVIC_InitTypeDef nvic;
	nvic.NVIC_IRQChannel = DMAx_Streamy_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = pp;
	nvic.NVIC_IRQChannelSubPriority = sb;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);	 
	DMA_ITConfig(DMAx_Streamy,DMA_inter_en_def,ENABLE);  //使能中断
}

/**
	* @brief:  开启一次DMA传输
	* @param:  DMA_Streamx   :DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7 
*/
void DMA_Enable(DMA_Stream_TypeDef *DMA_Streamx)
{
 
//	DMA_Cmd(DMA_Streamx, DISABLE);                      //关闭DMA传输 
//	
//	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}	//确保DMA可以被设置  
//		
//	DMA_SetCurrDataCounter(DMA_Streamx,ndtr);          //数据传输量  
 
	DMA_Cmd(DMA_Streamx, ENABLE);                      //开启DMA传输 
}	  


/**
  *  @brief:
	*  @note :usart1接收后出发的DMA传输
*/
void DMA2_Stream5_IRQHandler(void)
{											
	if(DMA_GetITStatus(DMA2_Stream5, DMA_IT_TCIF5))
	{
		DMA_ClearFlag(DMA2_Stream5, DMA_FLAG_TCIF5);
		DMA_ClearITPendingBit(DMA2_Stream5, DMA_IT_TCIF5);	
	  //处理内容
	}
}



/**
	*  @brief:
	*  @note:不同数据流对应的中断标志位也要更改
*/
void DMA1_Stream1_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_Stream1, DMA_IT_TCIF1))
	{
		DMA_ClearFlag(DMA1_Stream1, DMA_FLAG_TCIF1);
		DMA_ClearITPendingBit(DMA1_Stream1, DMA_IT_TCIF1);	
		//处理内容
		rm_DMA_end_flag = 1;
	}
}


/**
	*  @brief:
	*  @note:不同数据流对应的中断标志位也要更改
*/
void DMA1_Stream5_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_Stream5, DMA_IT_TCIF1))
	{
		DMA_ClearFlag(DMA1_Stream5, DMA_FLAG_TCIF5);
		DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_TCIF5);	
		//处理内容
		rm_DMA_end_flag = 1;
	}
}

/**
	*  @brief:
	*  @note:不同数据流对应的中断标志位也要更改
*/
void DMA1_Stream2_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_Stream2, DMA_IT_TCIF2))
	{
		DMA_ClearFlag(DMA1_Stream2, DMA_FLAG_TCIF2);
		DMA_ClearITPendingBit(DMA1_Stream2, DMA_IT_TCIF2);	
		//处理内容
		rm_DMA_end_flag = 1;
	}
}



