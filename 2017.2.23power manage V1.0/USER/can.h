#ifndef __CAN_H
#define __CAN_H	 
#include "stm32f4xx.h"	    
	
//CAN1����RX0�ж�ʹ��
#define CAN1_RX0_INT_ENABLE	0		//0,��ʹ��;1,ʹ��.								    

void CAN1_Configuration(void);//CAN��ʼ��
//void CAN2_Configuration(void);
//void CAN2_3510_SetMsg(u8 *Can_Wheel_Speed);						//��������
void CAN1Data_Analyze(void);
#endif

















