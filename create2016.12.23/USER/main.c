#include "stm32f4xx.h"
#include "delay.h"
#include "led.h" 
#include "stdio.h"
#include "stdlib.h"
#include "usart1.h"
#include "usart2.h"
#include "usart4.h"
#include "dma.h"
#include "pwm2.h"
#include "can.h"
#include "oled.h"
#include "timer.h"




int main(void)
{	
	u16  USART2_RecData;
	delay_init(168);                               //��ʱ������ʼ��   168Ϊϵͳʱ��Ƶ�� 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2	//usart1_init();
	usart2_init();                                 //����2��ʼ��,������PC��ͨ��
	CAN1_Configuration();

	//USARTͨ��DMA����Ϣ�����ڴ棬�ٽ��а�������
	//AD   �õ���ֵ����Ϊ����
	//CAN  ����֡�õ����ת��
	
	//ͨ�����������֣�����д���PID�ջ�
	//�ջ���������can���߽�����֡���͵�������п���

	//�������º�,������DMA�õ���Ϣ��ת��Ϊ�ڴ�ֵ;Ȼ��õ��ĵ���ֵ���е����ջ�����(�ڻ�),�ջ�����Ϊ1ms;
	//��can�õ����ٶ�ֵ�����ٶȻ��ջ���;���ߵıջ�����Ϊͬʱ����ʱ������AD������ֵ����can����֡������ֵ,
	//�������⻷ͬʱ���У���δ�õ��ٶ�ֵʱ���ϴε��ٶ�ֵ���бջ����ƣ�
  

	while(1)
	{
//		if(USART2_RX_Flag==1)
//		{
//					USART2_RecData = USART_ReceiveData(USART2);  //������Ϣ
//					USART_SendData(USART2,USART2_RecData);       //���ͽ��ܵ�������
//		}		
			delay_ms(200);
			USART_SendData(USART2,'0'); 
			USART_SendData(USART2,'2');
			USART_SendData(USART2,'1'); 
	}
}


/// �ض���c�⺯��printf��USART2
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ�USART1 */
		USART_SendData(USART2, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);		
	
		return (ch);
}

