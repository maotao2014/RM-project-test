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
#include "remote_control.h"



int main(void)  
{	
	delay_init(168);                               //延时函数初始化   168为系统时钟频率 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2	抢占优先级可以为0~3，响应优先级可以为0~3
	
	usart1_init();                                 //串口1初始化,用于与PC机通信
	
	CAN1_Configuration();                          //can1初始化，用于底盘can通信
	
	//待测试
	remote_usart_init();                                                                        //接收遥控器数据部分
	DMA_Config(DMA_USART_STREAM,DMA_USART_CHANNEL,(u32)&DMA_USART_DR,(u32)sbus_rx_buffer,18);    //接收遥控器数据部分
	DMA_NVIC_Config(DMA_USART_IRQn,DMA_USART_STREAM,DMA_IT_TC,1,2);                          //使能中断

	//USART通过DMA将信息传给内存，再进行按键解码
	//AD   得到的值解析为电流
	//CAN  数据帧得到电机转速
	
	//通过上诉三部分，后进行串级PID闭环
	//闭环结束后用can总线将数据帧发送到电机进行控制

	//按键按下后,经串口DMA得到信息后转化为内存值;然后得到的电流值进行电流闭环控制(内环),闭环周期为1ms;
	//经can得到的速度值进行速度换闭环控;两者的闭环控制为同时进行时电流的AD采样的值快于can数据帧的数据值,
	//所以内外环同时进行，在未得到速度值时以上次的速度值进行闭环控制；

	printf("CAN通信测试");
	printf("\n主机\n");
	while(1)
	{
			delay_ms(20);	
			CAN1_SetMsg();
		  if(rm_DMA_end_flag==1)
			{
				rm_DMA_end_flag = 0;
				//处理接收信息区
				RC_Decode();// 此函数待测试
				printf("\n解码后的信息:\n");
				printf("%d\n",RC_Ctl.rc.ch0);
				printf("%d\n",RC_Ctl.rc.ch1);
				printf("%d\n",RC_Ctl.rc.ch2);
				printf("%d\n",RC_Ctl.rc.ch3);
				printf("%d\n",RC_Ctl.rc.s1);
				printf("%d\n",RC_Ctl.rc.s2);
				printf("%d\n",RC_Ctl.mouse.x);
				printf("%d\n",RC_Ctl.mouse.y);
				printf("%d\n",RC_Ctl.mouse.z);	
			}
	}
}


/// 重定向c库函数printf到USART1
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到USART1 */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		
	
		return (ch);
}

