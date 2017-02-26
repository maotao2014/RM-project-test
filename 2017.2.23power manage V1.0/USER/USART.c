#include "USART.h"
#include "stdio.h"
#include "stdlib.h"

#ifdef REMOTE_CONTROL



#endif


/**
	* @brief:  USART中断配置函数
	* @param:  USARTx_IRQn        x:1~6
             USARTx             x:同上
             USART_inter_de     USART_Interrupt_definition
						 pp                 PreemptionPriority 抢占优先级
						 sp                 SubPriority        子优先级 
	* @note :  一般配置为USART传输结束产生中断
	* @test :  ok
*/
void USART_NVIC_Config(IRQn_Type USARTx_IRQn , USART_TypeDef* USARTx , u16 USART_inter_def , u8 pp , u8 sp)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USARTx_IRQn;       //中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=pp;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =sp;		  //子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			    //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	                        //根据指定的参数初始化VIC寄存器
	USART_ITConfig(USARTx, USART_inter_def, ENABLE);       //开启相关中断
}



/**
	*@usart1初始化用于与PC机通信
	*
*/
 void usart1_init(void)
{
	
	//声明结构体 
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	 
	//开启时钟 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9 ,GPIO_AF_USART1); //GPIOA9复用为USART1
	 
	//USART1端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10; //GPIOA9,10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9,PA10
	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //GPIOA3
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//复用功能
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度50MHz
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; //推挽复用输出
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //上拉
//	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA3
 
	//USART1 初始化设置
	USART_InitStructure.USART_BaudRate = 115200;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口1
		
	USART_Cmd(USART1, ENABLE);  //使能串口1  
	//USART_NVIC_Config(USART1_IRQn,USART1,USART_IT_RXNE,1,1);  //开启中断
	
}


/**
	*@brief:
	*@note :
*/
void USART1_IRQHandler(void) 
{	
	u16 ch;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		//printf("\nUSART1中断!\n");
		ch = USART_ReceiveData(USART1);
	}
	USART_ClearFlag(USART1,USART_IT_RXNE);          //清空标志位
}



#ifdef REMOTE_CONTROL
/**
	* @breif  :初始化usart用于DMA传输,用于接收遥控器信息
						 配置usart接收数据后请求DMA传输数据
	* @note   :用于接收遥控器的USART选择在USART.h中
	* @version:
	* @test   :ok
*/
void remote_usart_init()
{
	
	//声明结构体 
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
#if  DMA_USART == DMA_USART2

	//开启时钟 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART2时钟
 
	//串口2对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //
	 
	//USART2端口配置
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3|2; //GPIOA2与GPIOA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA2，PA3
 
	//USART2 初始化设置
	USART_InitStructure.USART_BaudRate = 100000;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_Even;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx ;//接收模式
	
	USART_Init(USART2, &USART_InitStructure); //初始化串口2
		
	USART_Cmd(USART2, ENABLE);  //使能串口2  

	USART_DMACmd(USART2,USART_DMAReq_Rx , ENABLE );  //配置DMA请求
	
	
	
#elif  DMA_USART==DMA_USART3
	
	//开启时钟 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //使能GPIOC时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能USART3时钟
 
	//串口3对应引脚复用映射
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_USART3); //GPIOC10复用为USART3
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_USART3); //GPIOC11复用为USART3
	 
	//USART3端口配置
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11|10; //GPIOC11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOC,&GPIO_InitStructure); //初始化PC11
 
	//USART3 初始化设置
	USART_InitStructure.USART_BaudRate = 100000;//波特率设置100000
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_Even;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//| USART_Mode_Tx;	//收发模式
	
	USART_Init(USART3, &USART_InitStructure); //初始化串口3
		
	USART_Cmd(USART3, ENABLE);  //使能串口3  
	
//	USART_NVIC_Config(USART3_IRQn,USART3,USART_IT_RXNE,2,1);
	
	USART_DMACmd(USART3,USART_DMAReq_Rx , ENABLE );


#elif  DMA_USART==DMA_USART4
  
	//开启时钟 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE); //使能USART4时钟
 
	//串口4对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_UART4); //GPIOA0复用为USART4
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_UART4); //GPIOA1复用为USART4
	 
	//USART4端口配置
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1; //GPIOA1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA1
 
	//USART4 初始化设置
	USART_InitStructure.USART_BaudRate = 100000;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_Even;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx ;//| USART_Mode_Tx;	//收发模式
	
	USART_Init(UART4, &USART_InitStructure); //初始化串口4
		
	USART_Cmd(UART4, ENABLE);  //使能串口4  

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
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		ch = USART_ReceiveData(USART3);
//		printf("\n收到串口3的数据\n");
		
	}
	USART_ClearFlag(USART3,USART_IT_RXNE);          //清空标志位
}




