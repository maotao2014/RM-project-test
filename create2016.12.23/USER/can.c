#include "can.h"
#include "usart2.h"
       
#include "stdio.h"     //调试添加


/*****全局变量********/
u16 Wheel_Speed[8]={7,6,5,4,3,2,1,0};       //存储8个轮子的速度

/*****全局变量********/

//VP230---CAN_TX---PA12(CANTX) 
//VP230---CAN_RX---PA11(CANRX) 

/*************************************************************************
                          CAN1_Configuration
描述：初始化CAN1配置为1M波特率
*************************************************************************/
void CAN1_Configuration(void)
{
    CAN_InitTypeDef        can;
    CAN_FilterInitTypeDef  can_filter;
    GPIO_InitTypeDef       gpio;
    NVIC_InitTypeDef       nvic;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1);

    gpio.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    GPIO_Init(GPIOA, &gpio);
    
    nvic.NVIC_IRQChannel = CAN1_RX0_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
    
    nvic.NVIC_IRQChannel = CAN1_TX_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic); 
    
    CAN_DeInit(CAN1);
    CAN_StructInit(&can);
    
    can.CAN_TTCM = DISABLE;
    can.CAN_ABOM = DISABLE;
    can.CAN_AWUM = DISABLE;
    can.CAN_NART = DISABLE;
    can.CAN_RFLM = DISABLE;
    can.CAN_TXFP = ENABLE;
    can.CAN_Mode = CAN_Mode_Normal;
    can.CAN_SJW  = CAN_SJW_2tq;
    can.CAN_BS1 = CAN_BS1_6tq;
    can.CAN_BS2 = CAN_BS2_4tq;
    can.CAN_Prescaler = 7;   //CAN BaudRate 42/(1+9+4)/3=1Mbps
    CAN_Init(CAN1, &can);

    can_filter.CAN_FilterNumber=0;
    can_filter.CAN_FilterMode=CAN_FilterMode_IdMask;
    can_filter.CAN_FilterScale=CAN_FilterScale_32bit;
    can_filter.CAN_FilterIdHigh=0x0000;
    can_filter.CAN_FilterIdLow=0x0000;
    can_filter.CAN_FilterMaskIdHigh=0x0000;
    can_filter.CAN_FilterMaskIdLow=0x0000;
    can_filter.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//the message which pass the filter save in fifo0
    can_filter.CAN_FilterActivation=ENABLE;
    CAN_FilterInit(&can_filter);
    
    CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
    CAN_ITConfig(CAN1,CAN_IT_TME,ENABLE); 
}


unsigned char can_tx_success_flag=0;
void CAN1_TX_IRQHandler(void) //CAN TX
{
    if (CAN_GetITStatus(CAN1,CAN_IT_TME)!= RESET) 
	  {
			 CAN_ClearITPendingBit(CAN1,CAN_IT_TME);
			 can_tx_success_flag=1;
    }
		printf("\n主机发送成功\n");
}


void CAN_SetMsg(void)
{	
	CanTxMsg TxMessage;  
  TxMessage.StdId=0x200;						 
 // TxMessage.ExtId=0x200;					 //使用的扩展ID
  TxMessage.IDE=CAN_ID_STD;					 //扩展模式
  TxMessage.RTR=CAN_RTR_DATA;				 //发送的是数据
  TxMessage.DLC=0x08;							   //数据长度为2字节
  TxMessage.Data[0]=Wheel_Speed[0];
  TxMessage.Data[1]=Wheel_Speed[1];
  TxMessage.Data[2]=Wheel_Speed[2];
  TxMessage.Data[3]=Wheel_Speed[3];
  TxMessage.Data[4]=Wheel_Speed[4];
  TxMessage.Data[5]=Wheel_Speed[5];
  TxMessage.Data[6]=Wheel_Speed[6];
  TxMessage.Data[7]=Wheel_Speed[7];
  CAN_Transmit(CAN1, &TxMessage);
}


void CAN1_RX0_IRQHandler(void)
{
    CanRxMsg rx_message;    
    u8 i;                  //测试用        
		printf("接收到从机信息\n");	
    if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET) 
		{
        CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
        CAN_Receive(CAN1, CAN_FIFO0, &rx_message);       
       	
				//打印can接受到的数据，检查是否正常传输
				printf("%d",rx_message.ExtId);
				for(i=0;i<8;i++)
				{
					printf("%d",rx_message.Data[i]);
				}
		}

		//CAN_SetMsg();//发送数据
}







