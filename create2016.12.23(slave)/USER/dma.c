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


//DMAx�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
//DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMAͨ��ѡ��,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
//par:�����ַ
//mar:�洢����ַ
//ndtr:���ݴ�����  
void MYDMA_Config(DMA_Stream_TypeDef *DMA_Streamx,u32 chx,u32 par,u32 mar,u16 ndtr)
{ 
	 
	NVIC_InitTypeDef nvic;
	DMA_InitTypeDef  DMA_InitStructure;
	
	if((u32)DMA_Streamx>(u32)DMA2)//�õ���ǰstream������DMA2����DMA1
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2ʱ��ʹ�� 
		
	}else 
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA1ʱ��ʹ�� 
	}
  DMA_DeInit(DMA_Streamx);
	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}//�ȴ�DMA������ 
	
  /* ���� DMA Stream */
  DMA_InitStructure.DMA_Channel            	= DMA_Channel_4;  //ͨ��ѡ��
  DMA_InitStructure.DMA_PeripheralBaseAddr 	= (u32)&USART1->DR;//DMA�����ַ
  DMA_InitStructure.DMA_Memory0BaseAddr    	= (u32)sbus_rx_buffer;//DMA �洢��0��ַ
  DMA_InitStructure.DMA_DIR 				= DMA_DIR_PeripheralToMemory;//�洢��������ģʽ
  DMA_InitStructure.DMA_BufferSize     		= 18;//���ݴ����� 
  DMA_InitStructure.DMA_PeripheralInc 		= DMA_PeripheralInc_Disable;//���������ģʽ
  DMA_InitStructure.DMA_MemoryInc 			= DMA_MemoryInc_Enable;//�洢������ģʽ
  DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_Byte;//�������ݳ���:8λ
  DMA_InitStructure.DMA_MemoryDataSize 		= DMA_MemoryDataSize_Byte;//�洢�����ݳ���:8λ
  DMA_InitStructure.DMA_Mode 				= DMA_Mode_Circular;// ʹ����ͨģʽ 
  DMA_InitStructure.DMA_Priority			= DMA_Priority_VeryHigh;//�е����ȼ�
  DMA_InitStructure.DMA_FIFOMode 			= DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold 		= DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst 		= DMA_MemoryBurst_Single;//�洢��ͻ�����δ���
  DMA_InitStructure.DMA_PeripheralBurst		= DMA_PeripheralBurst_Single;//����ͻ�����δ���
  DMA_Init(DMA_Streamx, &DMA_InitStructure);//��ʼ��DMA Stream
		
		
		

	nvic.NVIC_IRQChannel = DMA2_Stream5_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority =0;
	nvic.NVIC_IRQChannelSubPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);	 
     

	DMA_ITConfig(DMA2_Stream5,DMA_IT_TC,ENABLE);
	DMA_Cmd(DMA2_Stream5,ENABLE);

	

} 
//����һ��DMA����
//DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7 
//ndtr:���ݴ�����  
void MYDMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr)
{
// 
//	DMA_Cmd(DMA_Streamx, DISABLE);                      //�ر�DMA���� 
//	
//	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}	//ȷ��DMA���Ա�����  
//		
//	DMA_SetCurrDataCounter(DMA_Streamx,ndtr);          //���ݴ�����  
 
	DMA_Cmd(DMA_Streamx, ENABLE);                      //����DMA���� 
}	  

void DMA2_Stream5_IRQHandler(void)
{	
											

	
	if(DMA_GetITStatus(DMA2_Stream5, DMA_IT_TCIF5))
		{

	DMA_ClearFlag(DMA2_Stream5, DMA_FLAG_TCIF5);
	DMA_ClearITPendingBit(DMA2_Stream5, DMA_IT_TCIF5);

	translate();										//��ȡң����ֵ
	CAN_SetMsg();										//���͵�������


//	pwm0=map(ch0,364,1684,0,200);
//	pwm1=map(ch1,364,1684,0,200);
//	pwm2=map(ch2,364,1684,100,200);
//	pwm3=map(ch3,364,1684,100,200);
//	
//		TIM_SetCompare1(TIM4,pwm1);	
//		TIM_SetCompare2(TIM4,pwm1);	//�޸ıȽ�ֵ���޸�ռ�ձ�
//		TIM_SetCompare3(TIM4,pwm1);	//�޸ıȽ�ֵ���޸�ռ�ձ�
//		TIM_SetCompare4(TIM4,pwm1);	//�޸ıȽ�ֵ���޸�ռ�ձ�
			
		}
}



