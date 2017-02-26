/*

		说明: 将spi初始化配置封装为函数
		      



*/


#include "spi.h"


/********内部宏定义**********/

/********内部宏定义结束**********/


/*******外部变量定义*******/

/*******外部变量定义结束**************/

/**
	 * @brief: SPI 配置函数
	 * @param: 
             dma_enable  DMA传输使能      0 关断DMA传输  1使能DMA传输
					   inter_enable  传输中断使能   
	 * @note :
	 * @test :
	 */
void SPI1_Config(SPI_DMA_ENABLE dma_enable,SPI_INTER_ENABLE inter_enable)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef   SPI_InitStructure;
	//时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	//引脚初始化
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1); //SCK    复用脚PB3
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_SPI1); //MISO   复用脚PB4
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1); //MOSI   复用脚PB5
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource4,GPIO_AF_SPI1); 
		
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;       //复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化A5 A6 A7
		
	//控制引脚初始化
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP; //不上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);
		
	//SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
	SPI_InitStructure.SPI_Mode              = SPI_Mode_Slave ;
	SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_CPHA              = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_CPOL              = SPI_CPOL_Low;
	SPI_InitStructure.SPI_DataSize          = SPI_DataSize_16b;
	SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial     = 7;                //0x07位复位值 stm32f4XXX 765页
	SPI_InitStructure.SPI_NSS               = SPI_NSS_Hard;
	if(dma_enable == DMA_ENABLE)
	{
	  SPI_I2S_DMACmd(SPI1,SPI_DMAReq_Rx,ENABLE);
	}
		
	  //SPI1->CR1 |=0x1000;         //CRC校验使能
		//SPI1->CR2 |=0x20;           //错误中断使能
	SPI1->CR2 &=0xFFFB;           
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1,ENABLE);
}

/**
	 * @brief: SPI2 配置函数
	 * @param: 
             dma_enable  DMA传输使能      0 关断DMA传输  1使能DMA传输
					   inter_enable  传输中断使能   
	 * @note :
	 * @test :
	 */
void SPI2_Config(SPI_DMA_ENABLE dma_enable,SPI_INTER_ENABLE inter_enable)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef   SPI_InitStructure;
	//时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
	//引脚初始化
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource12,GPIO_AF_SPI2); //NSS    
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI2); //SCK   
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_SPI2); //MISO   
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI2); //MOSI
		
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;       //复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure); //初始化B13 B14 B15
		
	//控制引脚初始化
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);
		
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;
	SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_CPHA              = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_CPOL              = SPI_CPOL_Low;
	SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;
	SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial     = 7;                //0x07位复位值 stm32f4XXX 765页
	SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;
	if(dma_enable == DMA_ENABLE)
	{
	  SPI_I2S_DMACmd(SPI2,SPI_DMAReq_Tx,ENABLE);  //注意！！！发送DMA
	}
		
	  //SPI1->CR1 |=0x1000;         //CRC校验使能
		//SPI1->CR2 |=0x20;           //错误中断使能
	SPI2->CR2 &=0xFFFB;           
	SPI_Init(SPI2, &SPI_InitStructure);
	SPI_Cmd(SPI2,ENABLE);
}

void SPI1_SendByte(u16 byte)
{
  /* Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

  /* Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(SPI1, byte);


	
  /* Return the byte read from the SPI bus */
 // return SPI_I2S_ReceiveData(SPI1);
}	


