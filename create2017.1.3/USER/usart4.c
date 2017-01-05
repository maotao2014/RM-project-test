#include "usart4.h"
#include "stdio.h"
#include "stdlib.h"
unsigned char receive_data[7];
unsigned char receive,move_time=0;


u16 res4;
int  res;
extern float glp;
extern float gli;
extern float gld;

extern float P;
extern float I;
extern float D;

extern float gllim;

 void usart4_init(void)
{
	//�����ṹ�� 
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	//����ʱ�� 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);//ʹ��USART1ʱ��
 
	//����4��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4); //GPIOA9����ΪUSART2
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4); //GPIOA20����ΪUSART2
	 
	//USART4�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOC10��GPIOC11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOC,&GPIO_InitStructure); //��ʼ��PC10��PC11
 
	//USART4 ��ʼ������
	USART_InitStructure.USART_BaudRate = 115200;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(UART4, &USART_InitStructure); //��ʼ������1
		
	USART_Cmd(UART4, ENABLE);  //ʹ�ܴ���1  

	
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart2 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;//����2�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
}


void UART4_IRQHandler(void) 
{	
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		
		/******************ʵ��*************************/
//		res4 = USART_ReceiveData(UART4);//(USART1->DR);	//��ȡ���յ�������
		
		receive = USART_ReceiveData(UART4);//(USART1->DR);	//��ȡ���յ�������
		receive_data[move_time]=receive;
		if(receive_data[0]=='S')
		{
			move_time++;
			if(move_time==6)
			{
				res=(receive_data[2]-'0')*1000+(receive_data[3]-'0')*100+(receive_data[4]-'0')*10+(receive_data[5]-'0');
				if(receive_data[1]=='+')
					res=-res;
//		  if(abs(res)<30)res=0;
				move_time=0;
			}	
		}
		
		
		
//		USART_SendData(UART4, res4);		
	/*************PID����****************************/	
//				switch (res4)
//				{
//					case '1':glp=glp+(float)0.01;
//						break;
//					case '2':glp=glp-(float)0.01;
//						break;
//					case '3':gli=gli+(float)0.01;
//						break;
//					case '4':gli=gli-(float)0.01;
//						break;
//					case '5':gld=gld+(float)0.01;
//						break;
//					case '6':gld=gld-(float)0.01;
//						break;
//					
//					case 'q':P=P+(float)0.01;
//						break;
//					case 'w':P=P-(float)0.01;
//						break;
//					case 'e':I=I+(float)0.01;
//						break;
//					case 'r':I=I-(float)0.01;
//						break;
//					case 't':D=D+(float)0.01;
//						break;
//					case 'y':D=D-(float)0.01;
//						break;
//					case 'z':gllim=gllim+1;
//						break;
//					case 'x':gllim=gllim-1;
//						break;
//					default:res4=0;
//						break;
//				}
//			res4=0;
		/******************����ϵͳ************************/
//		resbuffer[counter] = USART_ReceiveData(USART4);	

//		if(counter == 0 && resbuffer[0] != 0xA5) return;   
//		counter++; 
//		if(counter==43)
//		{ 
//			counter=0; 
//			//sign=1;
//			jiaoyan=(resbuffer[4]|(resbuffer[5]<<8));
//			if(jiaoyan==0001)
//			{for(t1=0; t1<43; t1++)	caipan[t1]	= resbuffer[t1];}
//			else
//			{for(t1=0; t1<43; t1++)	caipan[t1]	= 0;}	
//		}

		
		
	}
		//USART_SendData(USART2, res+2);
			USART_ClearFlag(UART4,USART_IT_RXNE);
}

