#include "USART.h"
#include "stdio.h"
#include "stdlib.h"

#ifdef REMOTE_CONTROL



#endif


/**
	* @brief:  USART�ж����ú���
	* @param:  USARTx_IRQn        x:1~6
             USARTx             x:ͬ��
             USART_inter_de     USART_Interrupt_definition
						 pp                 PreemptionPriority ��ռ���ȼ�
						 sp                 SubPriority        �����ȼ� 
	* @note :  һ������ΪUSART������������ж�
	* @test :  ok
*/
void USART_NVIC_Config(IRQn_Type USARTx_IRQn , USART_TypeDef* USARTx , u16 USART_inter_def , u8 pp , u8 sp)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USARTx_IRQn;       //�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=pp;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =sp;		  //�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			    //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	                        //����ָ���Ĳ�����ʼ��VIC�Ĵ���
	USART_ITConfig(USARTx, USART_inter_def, ENABLE);       //��������ж�
}



/**
	*@usart1��ʼ��������PC��ͨ��
	*
*/
 void usart1_init(void)
{
	
	//�����ṹ�� 
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	 
	//����ʱ�� 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9 ,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	 
	//USART1�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10; //GPIOA9,10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9,PA10
	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //GPIOA3
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//���ù���
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�50MHz
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; //���츴�����
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //����
//	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA3
 
	//USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = 115200;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
		
	USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1  
	//USART_NVIC_Config(USART1_IRQn,USART1,USART_IT_RXNE,1,1);  //�����ж�
	
}


/**
	*@brief:
	*@note :
*/
void USART1_IRQHandler(void) 
{	
	u16 ch;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		//printf("\nUSART1�ж�!\n");
		ch = USART_ReceiveData(USART1);
	}
	USART_ClearFlag(USART1,USART_IT_RXNE);          //��ձ�־λ
}



#ifdef REMOTE_CONTROL
/**
	* @breif  :��ʼ��usart����DMA����,���ڽ���ң������Ϣ
						 ����usart�������ݺ�����DMA��������
	* @note   :���ڽ���ң������USARTѡ����USART.h��
	* @version:
	* @test   :ok
*/
void remote_usart_init()
{
	
	//�����ṹ�� 
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
#if  DMA_USART == DMA_USART2

	//����ʱ�� 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART2ʱ��
 
	//����2��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //
	 
	//USART2�˿�����
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3|2; //GPIOA2��GPIOA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA2��PA3
 
	//USART2 ��ʼ������
	USART_InitStructure.USART_BaudRate = 100000;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_Even;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx ;//����ģʽ
	
	USART_Init(USART2, &USART_InitStructure); //��ʼ������2
		
	USART_Cmd(USART2, ENABLE);  //ʹ�ܴ���2  

	USART_DMACmd(USART2,USART_DMAReq_Rx , ENABLE );  //����DMA����
	
	
	
#elif  DMA_USART==DMA_USART3
	
	//����ʱ�� 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //ʹ��GPIOCʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��USART3ʱ��
 
	//����3��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_USART3); //GPIOC10����ΪUSART3
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_USART3); //GPIOC11����ΪUSART3
	 
	//USART3�˿�����
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11|10; //GPIOC11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOC,&GPIO_InitStructure); //��ʼ��PC11
 
	//USART3 ��ʼ������
	USART_InitStructure.USART_BaudRate = 100000;//����������100000
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_Even;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//| USART_Mode_Tx;	//�շ�ģʽ
	
	USART_Init(USART3, &USART_InitStructure); //��ʼ������3
		
	USART_Cmd(USART3, ENABLE);  //ʹ�ܴ���3  
	
//	USART_NVIC_Config(USART3_IRQn,USART3,USART_IT_RXNE,2,1);
	
	USART_DMACmd(USART3,USART_DMAReq_Rx , ENABLE );


#elif  DMA_USART==DMA_USART4
  
	//����ʱ�� 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE); //ʹ��USART4ʱ��
 
	//����4��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_UART4); //GPIOA0����ΪUSART4
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_UART4); //GPIOA1����ΪUSART4
	 
	//USART4�˿�����
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1; //GPIOA1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA1
 
	//USART4 ��ʼ������
	USART_InitStructure.USART_BaudRate = 100000;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_Even;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx ;//| USART_Mode_Tx;	//�շ�ģʽ
	
	USART_Init(UART4, &USART_InitStructure); //��ʼ������4
		
	USART_Cmd(UART4, ENABLE);  //ʹ�ܴ���4  

	USART_DMACmd(UART4,USART_DMAReq_Rx , ENABLE );
#endif
}
#endif     //end  #ifdef REMOTE_CONTROL


/**
	*@brief:
	*@note :
 */
void USART3_IRQHandler(void) 
{	
	u16 ch;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		ch = USART_ReceiveData(USART3);
//		printf("\n�յ�����3������\n");
		
	}
	USART_ClearFlag(USART3,USART_IT_RXNE);          //��ձ�־λ
}




