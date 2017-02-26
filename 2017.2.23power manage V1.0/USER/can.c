#include "stm32f4xx.h"                  // Device header
#include "can.h"
#include <stdio.h>



/*****ȫ�ֱ���********/
 
/*****ȫ�ֱ���********/

/******�ⲿ��������**********/
CanRxMsg CAN1_rx_message; 
//u8 CAN1_RX_INT_Flag;  //can1�����жϱ�־λ

//CanRxMsg CAN2_rx_message;
//u8 CAN2_RX_INT_Flag;
/******�ⲿ�����������**********/

/*********�ⲿ����������************/
extern volatile short RealWheelSpeed[4];;

/*********�ⲿ��������************/


/**
	*@brief:can1��ʼ��
	*@note :����canʱʹ�����ж�  ��ռ���ȼ�Ϊ1 ��Ӧ���ȼ�Ϊ1
					����can��������Ҫ���õĲ���̫������ÿ��can������������������
*/
void CAN1_Configuration(void)
{
    CAN_InitTypeDef        can;
    CAN_FilterInitTypeDef  can_filter;
    GPIO_InitTypeDef       gpio;
    NVIC_InitTypeDef       nvic;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_CAN1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_CAN1);

    gpio.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    GPIO_Init(GPIOB, &gpio);
    
    nvic.NVIC_IRQChannel = CAN1_RX0_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 2;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
    
//    nvic.NVIC_IRQChannel = CAN1_TX_IRQn;
//    nvic.NVIC_IRQChannelPreemptionPriority = 1;
//    nvic.NVIC_IRQChannelSubPriority = 1;
//    nvic.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&nvic); 
    
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


//unsigned char can_tx_success_flag=0;
//void CAN1_TX_IRQHandler(void) //CAN TX
//{
//    if (CAN_GetITStatus(CAN1,CAN_IT_TME)!= RESET) 
//	  {
//			 CAN_ClearITPendingBit(CAN1,CAN_IT_TME);
//			 can_tx_success_flag=1;
//    }
//		//printf("\n�������ͳɹ�\n");
//}

/**
	*  @brief:can1 �����жϺ���
					 ��ӡ��׼ID��PC
	*  @note :����ʵ����Ҫ���Ĵ˺�������
*/
void CAN1_RX0_IRQHandler(void)
{     
    if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET) 
		{
        CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
        CAN_Receive(CAN1, CAN_FIFO0, &CAN1_rx_message);       
				CAN1Data_Analyze();
//				CAN1_RX_INT_Flag = 1;
		}       

}

/**  
	*@note :��CAN1�����ж�
*/
void CAN1Data_Analyze(void)
{
	 if(CAN1_rx_message.StdId==0x201)     // ���̵��IDֵ
	 {
			RealWheelSpeed[0] = CAN1_rx_message.Data[2];
			RealWheelSpeed[0]<<=8;
			RealWheelSpeed[0]|= CAN1_rx_message.Data[3];
	 }	
	 if(CAN1_rx_message.StdId==0x202)  // ���̵��IDֵ
	 {
			RealWheelSpeed[1] = CAN1_rx_message.Data[2];
			RealWheelSpeed[1]<<=8;
			RealWheelSpeed[1]|= CAN1_rx_message.Data[3];
	 }
	 if(CAN1_rx_message.StdId==0x203)   // ���̵��IDֵ
	 {
			RealWheelSpeed[2] = CAN1_rx_message.Data[2];
			RealWheelSpeed[2]<<=8;
			RealWheelSpeed[2]|= CAN1_rx_message.Data[3];
	 }
	 if(CAN1_rx_message.StdId==0x204)   // ���̵��IDֵ
	 {
			RealWheelSpeed[3] = CAN1_rx_message.Data[2];
			RealWheelSpeed[3]<<=8;
			RealWheelSpeed[3]|= CAN1_rx_message.Data[3];
	 }  		 
}


///**
//	*@brief:can1��ʼ��
//	*@note :����canʱʹ�����ж�  ��ռ���ȼ�Ϊ1 ��Ӧ���ȼ�Ϊ1
//					����can��������Ҫ���õĲ���̫������ÿ��can������������������
//*/
//void CAN2_Configuration(void)
//{
//    CAN_InitTypeDef        can;
//    CAN_FilterInitTypeDef  can_filter;
//    GPIO_InitTypeDef       gpio;
//    NVIC_InitTypeDef       nvic;

//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);

//    GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_CAN2);
//    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_CAN2);

//    gpio.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
//    gpio.GPIO_Mode = GPIO_Mode_AF;
//    GPIO_Init(GPIOB, &gpio);
//    
//    nvic.NVIC_IRQChannel = CAN2_RX0_IRQn;
//    nvic.NVIC_IRQChannelPreemptionPriority = 0;
//    nvic.NVIC_IRQChannelSubPriority = 1;
//    nvic.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&nvic);
//    
////    nvic.NVIC_IRQChannel = CAN1_TX_IRQn;
////    nvic.NVIC_IRQChannelPreemptionPriority = 1;
////    nvic.NVIC_IRQChannelSubPriority = 1;
////    nvic.NVIC_IRQChannelCmd = ENABLE;
////    NVIC_Init(&nvic); 
//    
//    CAN_DeInit(CAN2);
//    CAN_StructInit(&can);
//    
//    can.CAN_TTCM = DISABLE;
//    can.CAN_ABOM = DISABLE;
//    can.CAN_AWUM = DISABLE;
//    can.CAN_NART = DISABLE;
//    can.CAN_RFLM = DISABLE;
//    can.CAN_TXFP = ENABLE;
//    can.CAN_Mode = CAN_Mode_Normal;
//    can.CAN_SJW  = CAN_SJW_1tq;
//    can.CAN_BS1 = CAN_BS1_9tq;
//    can.CAN_BS2 = CAN_BS2_4tq;
//    can.CAN_Prescaler = 3;   //CAN BaudRate 42/(1+9+4)/3=1Mbps
//    CAN_Init(CAN2, &can);

//    can_filter.CAN_FilterNumber=0;
//    can_filter.CAN_FilterMode=CAN_FilterMode_IdMask;
//    can_filter.CAN_FilterScale=CAN_FilterScale_32bit;
//    can_filter.CAN_FilterIdHigh=0x0000;
//    can_filter.CAN_FilterIdLow=0x0000;
//    can_filter.CAN_FilterMaskIdHigh=0x0000;
//    can_filter.CAN_FilterMaskIdLow=0x0000;
//    can_filter.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//the message which pass the filter save in fifo0
//    can_filter.CAN_FilterActivation=ENABLE;
//    CAN_FilterInit(&can_filter);
//    
//    CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);
//    CAN_ITConfig(CAN2,CAN_IT_TME,ENABLE); 
//}

///**
//	*  @brief:can1 �����жϺ���
//					 ��ӡ��׼ID��PC
//	*  @note :����ʵ����Ҫ���Ĵ˺�������
//*/
//void CAN2_RX0_IRQHandler(void)
//{     
//    if (CAN_GetITStatus(CAN2,CAN_IT_FMP0)!= RESET) 
//		{
//        CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);
//        CAN_Receive(CAN2, CAN_FIFO0, &CAN2_rx_message);       
//				CAN2_RX_INT_Flag = 1;
//		}       

//}

  

//void CAN2_3510_SetMsg(u8 *CAN_Wheel_Speed)
//{	
//	CanTxMsg TxMessage;  
//  TxMessage.StdId=0x200;						 //ʹ�ñ�׼ID     ���͵������� ��IDֵ�̶�
//  TxMessage.IDE=CAN_ID_STD;					 //��׼ģʽ
//  TxMessage.RTR=CAN_RTR_DATA;				 //���͵�������
//  TxMessage.DLC=0x08;							   //���ݳ���Ϊ8�ֽ�
////  TxMessage.Data[0]=CAN_Wheel_Speed[0];
////  TxMessage.Data[1]=CAN_Wheel_Speed[1];
////  TxMessage.Data[2]=CAN_Wheel_Speed[2];
////  TxMessage.Data[3]=CAN_Wheel_Speed[3];
////  TxMessage.Data[4]=CAN_Wheel_Speed[4];
////  TxMessage.Data[5]=CAN_Wheel_Speed[5];
////  TxMessage.Data[6]=CAN_Wheel_Speed[6];
////  TxMessage.Data[7]=CAN_Wheel_Speed[7];
//  TxMessage.Data[0]=(u8)(-600>>8);
//  TxMessage.Data[1]=(u8)(-600);
//  TxMessage.Data[2]=(u8)(400>>8);
//  TxMessage.Data[3]=(u8)(400);
//  TxMessage.Data[4]=(u8)(400>>8);
//  TxMessage.Data[5]=(u8)(400);
//  TxMessage.Data[6]=(u8)(1200>>8);
//  TxMessage.Data[7]=(u8)(1200);
//  CAN_Transmit(CAN2, &TxMessage);
//}









