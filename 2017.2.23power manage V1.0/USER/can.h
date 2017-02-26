#ifndef __CAN_H
#define __CAN_H	 
#include "stm32f4xx.h"	    
	
//CAN1接收RX0中断使能
#define CAN1_RX0_INT_ENABLE	0		//0,不使能;1,使能.								    

void CAN1_Configuration(void);//CAN初始化
//void CAN2_Configuration(void);
//void CAN2_3510_SetMsg(u8 *Can_Wheel_Speed);						//发送数据
void CAN1Data_Analyze(void);
#endif

















