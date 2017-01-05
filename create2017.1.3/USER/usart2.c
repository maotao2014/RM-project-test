#include "usart2.h"
#include "stdio.h"
#include "stdlib.h"


u16 USART2_RecData;
 void usart2_init(void)
{
	
	//�����ṹ�� 
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	//����ʱ�� 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART2ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA2����ΪUSART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9 ,GPIO_AF_USART1); //GPIOA3����ΪUSART2
	 
	//USART2�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10; //GPIOA2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA2
	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //GPIOA3
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//���ù���
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�50MHz
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; //���츴�����
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //����
//	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA3
 
	//USART2 ��ʼ������
	USART_InitStructure.USART_BaudRate = 115200;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); //��ʼ������2
		
	USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���2  

	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart2 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����2�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
}


void USART1_IRQHandler(void) 
{	
	u8 ch;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		;
	}
	ch = USART_ReceiveData(USART1);
	USART_SendData(USART1,ch);
	USART_ClearFlag(USART1,USART_IT_RXNE);          //��ձ�־λ
}

