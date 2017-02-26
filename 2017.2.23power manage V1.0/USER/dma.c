#include "dma.h"																	   	  

#include "remote_control.h"

/**************�ⲿ����������*****************/
u8 rm_DMA_end_flag;
u8 DMA_SPI1_RX_Flag;
u16 DMA_SPI1_RX_Arry[10];

/**************�ⲿ��������������*****************/

/**
	* @brief:DMAx�ĸ�ͨ������
	* @param:
					DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7
					chx:DMAͨ��ѡ��,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
					par:�����ַ 
					mar:�洢����ַ
					ndtr:���ݴ�����
					peripheral_dataSize:��������λ��С,DMA_PeripheralDataSize_Byte       ((uint32_t)0x00000000) 
																						 DMA_PeripheralDataSize_HalfWord   ((uint32_t)0x00000800) 
																						 DMA_PeripheralDataSize_Word       ((uint32_t)0x00001000) 
					memory_datasize:�ڴ�����λ��С,DMA_MemoryDataSize_Byte           ((uint32_t)0x00000000) 
																				 DMA_MemoryDataSize_HalfWord       ((uint32_t)0x00002000) 
																				 DMA_MemoryDataSize_Word
	* @note:����ֻ�����˳��õ�DMA����,����ʵ����Ҫ�����и���
	*/
void DMA_Config(DMA_Stream_TypeDef *DMAy_Streamx,u32 chx,u32 par,u32 mar,u16 ndtr,u32 peripheral_dataSize,u32 memory_datasize)
{ 
	DMA_InitTypeDef  DMA_InitStructure;
	
	if((u32)DMAy_Streamx>(u32)DMA2)//�õ���ǰstream������DMA2����DMA1
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2ʱ��ʹ�� 
		
	}
	else 
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA1ʱ��ʹ�� 
	}
  DMA_DeInit(DMAy_Streamx);
	
	while (DMA_GetCmdStatus(DMAy_Streamx) != DISABLE){}//�ȴ�DMA������ 
	
  /* ���� DMA Stream */
  DMA_InitStructure.DMA_Channel            	= chx;  //ͨ��ѡ��
  DMA_InitStructure.DMA_PeripheralBaseAddr 	= par;  //DMA�����ַ
  DMA_InitStructure.DMA_Memory0BaseAddr    	= mar;  //DMA �洢����ַ
  DMA_InitStructure.DMA_DIR 				        = DMA_DIR_PeripheralToMemory;//�����赽�洢��
  DMA_InitStructure.DMA_BufferSize     	  	= ndtr; //���ݴ����� 
  DMA_InitStructure.DMA_PeripheralInc 	  	= DMA_PeripheralInc_Disable;//���������ģʽ
  DMA_InitStructure.DMA_MemoryInc 			    = DMA_MemoryInc_Enable;//�洢������ģʽ
  DMA_InitStructure.DMA_PeripheralDataSize 	= peripheral_dataSize;//�������ݳ���
  DMA_InitStructure.DMA_MemoryDataSize 		  = memory_datasize;//�洢�����ݳ���
  DMA_InitStructure.DMA_Mode 				        = DMA_Mode_Circular;// ʹ��ѭ��ģʽ                //�˴�ֵ����
  DMA_InitStructure.DMA_Priority		      	= DMA_Priority_VeryHigh;//������ȵȼ�             //�˴�ֵ����
  DMA_InitStructure.DMA_FIFOMode 		      	= DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold 	  	= DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst 	    	= DMA_MemoryBurst_Single;    //�洢��ͻ�����δ���
  DMA_InitStructure.DMA_PeripheralBurst	  	= DMA_PeripheralBurst_Single;//����ͻ�����δ���
  DMA_Init(DMAy_Streamx, &DMA_InitStructure);//��ʼ��DMA Stream
		
	DMA_Cmd(DMAy_Streamx,ENABLE);
} 

/**
	* @brief:  DMA�ж����ú���
	* @param:  DMAx_Streamy_IRQn x:1~2   y:0~7  
						 DMAx_Streamy      ͬ�� 
						 DMA_inter_en_def   DMA_interrupt_enable_definitions 
																������Ϊ DMA_IT_TC
                                         DMA_IT_HT 
																				 DMA_IT_TE 
																				 DMA_IT_DME	
                                         DMA_IT_FE
						 pp              	 PreemptionPriority ��ռ���ȼ�
						 sb                SubPriority        �����ȼ� 
	* @note :  һ������ΪDMA������������ж� DMA_IT_TC
*/
void DMA_NVIC_Config(IRQn_Type DMAx_Streamy_IRQn,DMA_Stream_TypeDef* DMAx_Streamy,u32 DMA_inter_en_def,u8 pp,u8 sb)
{
	NVIC_InitTypeDef nvic;
	nvic.NVIC_IRQChannel = DMAx_Streamy_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = pp;
	nvic.NVIC_IRQChannelSubPriority = sb;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);	 
	DMA_ITConfig(DMAx_Streamy,DMA_inter_en_def,ENABLE);  //ʹ���ж�
}

/**
	* @brief:  ����һ��DMA����
	* @param:  DMA_Streamx   :DMA������,DMA1_Stream0~7/DMA2_Stream0~7 
*/
void DMA_Enable(DMA_Stream_TypeDef *DMA_Streamx)
{
 
//	DMA_Cmd(DMA_Streamx, DISABLE);                      //�ر�DMA���� 
//	
//	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}	//ȷ��DMA���Ա�����  
//		
//	DMA_SetCurrDataCounter(DMA_Streamx,ndtr);          //���ݴ�����  
 
	DMA_Cmd(DMA_Streamx, ENABLE);                      //����DMA���� 
}	  


/**
  *  @brief:
	*  @note :usart1���պ������DMA����
*/
void DMA2_Stream5_IRQHandler(void)
{											
	if(DMA_GetITStatus(DMA2_Stream5, DMA_IT_TCIF5))
	{
		DMA_ClearFlag(DMA2_Stream5, DMA_FLAG_TCIF5);
		DMA_ClearITPendingBit(DMA2_Stream5, DMA_IT_TCIF5);	
	  rm_DMA_end_flag = 1;
		RC_Decode();  //ң������Ϣ����
	}
}



/**
	*  @brief:USART2 DMA
	*  @note:��ͬ��������Ӧ���жϱ�־λҲҪ����
*/
void DMA1_Stream5_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_Stream5, DMA_IT_TCIF5))
	{
		DMA_ClearFlag(DMA1_Stream5, DMA_FLAG_TCIF5);
		DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_TCIF5);	
		rm_DMA_end_flag = 1;
		RC_Decode();  //ң������Ϣ����
	}
}

/**
	*  @brief:USART3 DMA
	*  @note:��ͬ��������Ӧ���жϱ�־λҲҪ����
  */
void DMA1_Stream1_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_Stream1, DMA_IT_TCIF1))
	{
		DMA_ClearFlag(DMA1_Stream1, DMA_FLAG_TCIF1);
		DMA_ClearITPendingBit(DMA1_Stream1, DMA_IT_TCIF1);	
		//��������
		rm_DMA_end_flag = 1;
		RC_Decode();  //ң������Ϣ����
	}
}

/**
	*  @brief:USART4
	*  @note:��ͬ��������Ӧ���жϱ�־λҲҪ����
*/
void DMA1_Stream2_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_Stream2, DMA_IT_TCIF2))
	{
		DMA_ClearFlag(DMA1_Stream2, DMA_FLAG_TCIF2);
		DMA_ClearITPendingBit(DMA1_Stream2, DMA_IT_TCIF2);	
		rm_DMA_end_flag = 1;
		RC_Decode();  //ң������Ϣ����
	}
}


/**
	*  @brief:SPI1 RX DMA���� �жϷ�����
	*  @note:��ͬ��������Ӧ���жϱ�־λҲҪ����
*/
void DMA2_Stream2_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA2_Stream2, DMA_IT_TCIF2))
	{
		DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2);
		DMA_ClearITPendingBit(DMA2_Stream2, DMA_IT_TCIF2);	
		//��������
		DMA_SPI1_RX_Flag = 1;
	}
}


