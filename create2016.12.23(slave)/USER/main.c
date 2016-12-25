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




int main(void)
{	
	delay_init(168);                               //��ʱ������ʼ��   168Ϊϵͳʱ��Ƶ�� 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2	//usart1_init();
	usart1_init();                                 //����2��ʼ��,������PC��ͨ��
	CAN1_Configuration();

	//USARTͨ��DMA����Ϣ�����ڴ棬�ٽ��а�������
	//AD   �õ���ֵ����Ϊ����
	//CAN  ����֡�õ����ת��
	
	//ͨ�����������֣�����д���PID�ջ�
	//�ջ���������can���߽�����֡���͵�������п���

	//�������º�,������DMA�õ���Ϣ��ת��Ϊ�ڴ�ֵ;Ȼ��õ��ĵ���ֵ���е����ջ�����(�ڻ�),�ջ�����Ϊ1ms;
	//��can�õ����ٶ�ֵ�����ٶȻ��ջ���;���ߵıջ�����Ϊͬʱ����ʱ������AD������ֵ����can����֡������ֵ,
	//�������⻷ͬʱ���У���δ�õ��ٶ�ֵʱ���ϴε��ٶ�ֵ���бջ����ƣ�
  
	printf(" CANͨ�Ų���");
	printf("\n�ӻ�\n");
	while(1)
	{
		 ;	
	}
}


/// �ض���c�⺯��printf��USART2
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ�USART1 */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		
	
		return (ch);
}

