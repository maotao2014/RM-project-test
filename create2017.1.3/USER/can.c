#include "can.h"
#include "usart2.h"
       
#include "stdio.h"     //调试添加


/*****全局变量********/
short wheel_speed = 500;
u8 CAN_Wheel_Speed[8]={7,6,5,4,3,2,1,0};   //can发送的控制底盘轮子转速数组 每两位组成一个轮数 低位数组所存数据为速度高位 

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
    can.CAN_SJW  = CAN_SJW_1tq;
    can.CAN_BS1 = CAN_BS1_9tq;
    can.CAN_BS2 = CAN_BS2_4tq;
    can.CAN_Prescaler = 3;   //CAN BaudRate 42/(1+9+4)/3=1Mbps
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
		//printf("\n主机发送成功\n");
}


void CAN1_SetMsg(void)
{	
	CanTxMsg TxMessage;  
  TxMessage.StdId=0x200;						 //使用标准ID     发送底盘数据 此ID值固定
 // TxMessage.ExtId=0x200;					 //使用的扩展ID
  TxMessage.IDE=CAN_ID_STD;					 //标准模式
  TxMessage.RTR=CAN_RTR_DATA;				 //发送的是数据
  TxMessage.DLC=0x08;							   //数据长度为8字节
//  TxMessage.Data[0]=CAN_Wheel_Speed[0];
//  TxMessage.Data[1]=CAN_Wheel_Speed[1];
//  TxMessage.Data[2]=CAN_Wheel_Speed[2];
//  TxMessage.Data[3]=CAN_Wheel_Speed[3];
//  TxMessage.Data[4]=CAN_Wheel_Speed[4];
//  TxMessage.Data[5]=CAN_Wheel_Speed[5];
//  TxMessage.Data[6]=CAN_Wheel_Speed[6];
//  TxMessage.Data[7]=CAN_Wheel_Speed[7];
  TxMessage.Data[0]=(u8)(400>>8);
  TxMessage.Data[1]=(u8)(400);
  TxMessage.Data[2]=(u8)(400>>8);
  TxMessage.Data[3]=(u8)(400);
  TxMessage.Data[4]=(u8)(400>>8);
  TxMessage.Data[5]=(u8)(400);
  TxMessage.Data[6]=(u8)(1200>>8);
  TxMessage.Data[7]=(u8)(1200);
  CAN_Transmit(CAN1, &TxMessage);
}

/**
	*  @brief:can1 接收中断函数
					 打印标准ID到PC
	*  @note :根据实际需要更改此函数内容
*/
void CAN1_RX0_IRQHandler(void)
{
    CanRxMsg rx_message;         
    if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET) 
		{
        CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
        CAN_Receive(CAN1, CAN_FIFO0, &rx_message);       
       	
				//打印can接受到的数据，检查是否正常传输
				printf("%x\n",rx_message.StdId);
		}
}







