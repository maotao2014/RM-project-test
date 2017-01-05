#include "stm32f4xx.h"
#include "delay.h"
#include "led.h" 
#include "stdio.h"
#include "stdlib.h"
#include "USART.h"
#include "dma.h"
#include "pwm2.h"
#include "can.h"
#include "oled.h"
#include "timer.h"
#include "remote_control.h"



int main(void)  
{	
	delay_init(168);                               //��ʱ������ʼ��   168Ϊϵͳʱ��Ƶ�� 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2	��ռ���ȼ�����Ϊ0~3����Ӧ���ȼ�����Ϊ0~3
	
	usart1_init();                                 //����1��ʼ��,������PC��ͨ��
	
	CAN1_Configuration();                          //can1��ʼ�������ڵ���canͨ��
	
	//������
	remote_usart_init();                                                                        //����ң�������ݲ���
	DMA_Config(DMA_USART_STREAM,DMA_USART_CHANNEL,(u32)&DMA_USART_DR,(u32)sbus_rx_buffer,18);    //����ң�������ݲ���
	DMA_NVIC_Config(DMA_USART_IRQn,DMA_USART_STREAM,DMA_IT_TC,1,2);                          //ʹ���ж�

	//USARTͨ��DMA����Ϣ�����ڴ棬�ٽ��а�������
	//AD   �õ���ֵ����Ϊ����
	//CAN  ����֡�õ����ת��
	
	//ͨ�����������֣�����д���PID�ջ�
	//�ջ���������can���߽�����֡���͵�������п���

	//�������º�,������DMA�õ���Ϣ��ת��Ϊ�ڴ�ֵ;Ȼ��õ��ĵ���ֵ���е����ջ�����(�ڻ�),�ջ�����Ϊ1ms;
	//��can�õ����ٶ�ֵ�����ٶȻ��ջ���;���ߵıջ�����Ϊͬʱ����ʱ������AD������ֵ����can����֡������ֵ,
	//�������⻷ͬʱ���У���δ�õ��ٶ�ֵʱ���ϴε��ٶ�ֵ���бջ����ƣ�

	printf("CANͨ�Ų���");
	printf("\n����\n");
	while(1)
	{
			delay_ms(20);	
			CAN1_SetMsg();
		  if(rm_DMA_end_flag==1)
			{
				rm_DMA_end_flag = 0;
				//���������Ϣ��
				RC_Decode();// �˺���������
				printf("\n��������Ϣ:\n");
				printf("%d\n",RC_Ctl.rc.ch0);
				printf("%d\n",RC_Ctl.rc.ch1);
				printf("%d\n",RC_Ctl.rc.ch2);
				printf("%d\n",RC_Ctl.rc.ch3);
				printf("%d\n",RC_Ctl.rc.s1);
				printf("%d\n",RC_Ctl.rc.s2);
				printf("%d\n",RC_Ctl.mouse.x);
				printf("%d\n",RC_Ctl.mouse.y);
				printf("%d\n",RC_Ctl.mouse.z);	
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

