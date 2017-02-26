#include "stm32f4xx.h"
#include "delay.h"
#include "stdio.h"
#include "stdlib.h"
#include "USART.h"
#include "dma.h"
#include "can.h"
//#include "oled.h"
#include "timer.h"
#include "remote_control.h"
#include "spi.h"
#include "chassis.h"
#include "led.h"

//extern CanRxMsg CAN1_rx_message;	

//extern u8 CAN1_RX_INT_Flag;    //can1接收中断标志位

extern u8 rm_DMA_end_flag;        //遥控器数据DMA传输结束产生中断标志位  ,定义处在dma.c
extern u16 DMA_SPI1_RX_Arry[10];      //SPI1接收数组
extern  u8 sbus_rx_buffer[18];       //用于接收遥控器数据的数组
extern RC_Ctl_t RC_Ctl;           //遥控器信息结构体

extern int SetWheelSpeed[4];  //4个麦轮设定的速率
extern int WheelSpeedOut[4];
extern volatile short RealWheelSpeed[4];
extern u8 DMA_SPI1_RX_Flag;
extern u8 TIM4_flag;
extern float AveragePower20ms;//EnergeStorage
extern float power_limit;



short speed;
int main(void)  
{	
	delay_init(168);         //延时函数初始化   168为系统时钟频率 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2	抢占优先级可以为0~3，响应优先级可以为0~3
	
	usart1_init();             //串口1初始化,用于与PC机通信
	CAN1_Configuration();    //can1初始化，用于底盘can通信	

	
	//接收遥控器数据部分
	remote_usart_init();    
	DMA_Config(DMA_USART_STREAM,DMA_USART_CHANNEL,(u32)&DMA_USART_DR,\
						 (u32)sbus_rx_buffer,18,DMA_PeripheralDataSize_Byte,DMA_MemoryDataSize_Byte);    //接收遥控器数据部分
	DMA_NVIC_Config(DMA_USART_IRQn,DMA_USART_STREAM,DMA_IT_TC,0,2);                          //使能中断

	//SPI1初始化
	SPI1_Config(DMA_ENABLE,INTER_DISABLE);
	DMA_Config(DMA2_Stream2,DMA_Channel_3,(u32)&SPI1->DR,(u32)&DMA_SPI1_RX_Arry,SPI1_RECIVE_NUM,\
						DMA_PeripheralDataSize_HalfWord,DMA_MemoryDataSize_HalfWord);
	DMA_NVIC_Config(DMA2_Stream2_IRQn,DMA2_Stream2,DMA_IT_TC,0,2); //
	
	//底盘pid控制初始化
	chassis_pid_init();
	
	//led初始化
	led_init();
	
	//1ms定时器初始化
	TIM4_Int_Init();

	//USART通过DMA将信息传给内存，再进行按键解码
	//AD   得到的值解析为电流
	//CAN  数据帧得到电机转速
	
	//通过上诉三部分，后进行串级PID闭环
	//闭环结束后用can总线将数据帧发送到电机进行控制

	//按键按下后,经串口DMA得到信息后转化为内存值;然后得到的电流值进行电流闭环控制(内环),闭环周期为1ms;
	//经can得到的速度值进行速度换闭环控;两者的闭环控制为同时进行时电流的AD采样的值快于can数据帧的数据值,
	//所以内外环同时进行，在未得到速度值时以上次的速度值进行闭环控制；
	
	printf("\n进入主循环");
	while(1)
	{


//		if(DMA_SPI1_RX_Flag==1)
//		{
//		  printf("%f\n",DMA_SPI1_RX_Arry[0]/100.0f);
//			
//			DMA_SPI1_RX_Flag = 0;
//		}
		if(TIM4_flag==1)
		{
			TIM4_flag = 0;
			Chassis_Task();
			over_power_check();
			printf("%f %d ",power_limit,SetWheelSpeed[0]);
//			printf("%f ",DMA_SPI1_RX_Arry[0]/100.0f * 24.0f);

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
/*********************************************END OF FILE**********************/


