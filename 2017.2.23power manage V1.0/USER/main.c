#include "stm32f4xx.h"
#include "delay.h"
#include "stdio.h"
#include "stdlib.h"
#include "USART.h"
#include "dma.h"
#include "can.h"
//#include "oled.h"
#include "timer.h"
#include "remote_control.h"
#include "spi.h"
#include "chassis.h"
#include "led.h"

//extern CanRxMsg CAN1_rx_message;	

//extern u8 CAN1_RX_INT_Flag;    //can1�����жϱ�־λ

extern u8 rm_DMA_end_flag;        //ң��������DMA������������жϱ�־λ  ,���崦��dma.c
extern u16 DMA_SPI1_RX_Arry[10];      //SPI1��������
extern  u8 sbus_rx_buffer[18];       //���ڽ���ң�������ݵ�����
extern RC_Ctl_t RC_Ctl;           //ң������Ϣ�ṹ��

extern int SetWheelSpeed[4];  //4�������趨������
extern int WheelSpeedOut[4];
extern volatile short RealWheelSpeed[4];
extern u8 DMA_SPI1_RX_Flag;
extern u8 TIM4_flag;
extern float AveragePower20ms;//EnergeStorage
extern float power_limit;



short speed;
int main(void)  
{	
	delay_init(168);         //��ʱ������ʼ��   168Ϊϵͳʱ��Ƶ�� 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2	��ռ���ȼ�����Ϊ0~3����Ӧ���ȼ�����Ϊ0~3
	
	usart1_init();             //����1��ʼ��,������PC��ͨ��
	CAN1_Configuration();    //can1��ʼ�������ڵ���canͨ��	

	
	//����ң�������ݲ���
	remote_usart_init();    
	DMA_Config(DMA_USART_STREAM,DMA_USART_CHANNEL,(u32)&DMA_USART_DR,\
						 (u32)sbus_rx_buffer,18,DMA_PeripheralDataSize_Byte,DMA_MemoryDataSize_Byte);    //����ң�������ݲ���
	DMA_NVIC_Config(DMA_USART_IRQn,DMA_USART_STREAM,DMA_IT_TC,0,2);                          //ʹ���ж�

	//SPI1��ʼ��
	SPI1_Config(DMA_ENABLE,INTER_DISABLE);
	DMA_Config(DMA2_Stream2,DMA_Channel_3,(u32)&SPI1->DR,(u32)&DMA_SPI1_RX_Arry,SPI1_RECIVE_NUM,\
						DMA_PeripheralDataSize_HalfWord,DMA_MemoryDataSize_HalfWord);
	DMA_NVIC_Config(DMA2_Stream2_IRQn,DMA2_Stream2,DMA_IT_TC,0,2); //
	
	//����pid���Ƴ�ʼ��
	chassis_pid_init();
	
	//led��ʼ��
	led_init();
	
	//1ms��ʱ����ʼ��
	TIM4_Int_Init();

	//USARTͨ��DMA����Ϣ�����ڴ棬�ٽ��а�������
	//AD   �õ���ֵ����Ϊ����
	//CAN  ����֡�õ����ת��
	
	//ͨ�����������֣�����д���PID�ջ�
	//�ջ���������can���߽�����֡���͵�������п���

	//�������º�,������DMA�õ���Ϣ��ת��Ϊ�ڴ�ֵ;Ȼ��õ��ĵ���ֵ���е����ջ�����(�ڻ�),�ջ�����Ϊ1ms;
	//��can�õ����ٶ�ֵ�����ٶȻ��ջ���;���ߵıջ�����Ϊͬʱ����ʱ������AD������ֵ����can����֡������ֵ,
	//�������⻷ͬʱ���У���δ�õ��ٶ�ֵʱ���ϴε��ٶ�ֵ���бջ����ƣ�
	
	printf("\n������ѭ��");
	while(1)
	{


//		if(DMA_SPI1_RX_Flag==1)
//		{
//		  printf("%f\n",DMA_SPI1_RX_Arry[0]/100.0f);
//			
//			DMA_SPI1_RX_Flag = 0;
//		}
		if(TIM4_flag==1)
		{
			TIM4_flag = 0;
			Chassis_Task();
			over_power_check();
			printf("%f %d ",power_limit,SetWheelSpeed[0]);
//			printf("%f ",DMA_SPI1_RX_Arry[0]/100.0f * 24.0f);

		}

	}
}


/// �ض���c�⺯��printf��USART1
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ�USART1 */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		
	
		return (ch);
}
/*********************************************END OF FILE**********************/


