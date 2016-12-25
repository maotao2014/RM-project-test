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
	delay_init(168);                               //延时函数初始化   168为系统时钟频率 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2	//usart1_init();
	usart1_init();                                 //串口2初始化,用于与PC机通信
	CAN1_Configuration();

	//USART通过DMA将信息传给内存，再进行按键解码
	//AD   得到的值解析为电流
	//CAN  数据帧得到电机转速
	
	//通过上诉三部分，后进行串级PID闭环
	//闭环结束后用can总线将数据帧发送到电机进行控制

	//按键按下后,经串口DMA得到信息后转化为内存值;然后得到的电流值进行电流闭环控制(内环),闭环周期为1ms;
	//经can得到的速度值进行速度换闭环控;两者的闭环控制为同时进行时电流的AD采样的值快于can数据帧的数据值,
	//所以内外环同时进行，在未得到速度值时以上次的速度值进行闭环控制；
  
	printf(" CAN通信测试");
	printf("\n从机\n");
	while(1)
	{
		 ;	
	}
}


/// 重定向c库函数printf到USART2
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到USART1 */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		
	
		return (ch);
}

