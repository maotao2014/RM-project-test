/**
  ******************************************************************************
  * @file    bsp_xxx.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   2.4g����ģ��/nrf24l01+/master Ӧ��bsp
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
#include "bsp_spi_nrf.h"
#include "spi.h"

 u8 RX_BUF[RX_PLOAD_WIDTH];		//�������ݻ���
 u8 TX_BUF[TX_PLOAD_WIDTH];		//�������ݻ���
 u8 TX_ADDRESS[TX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01};  // ����һ����̬���͵�ַ
 u8 RX_ADDRESS[RX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01};

 u8 rx_buff[RX_PLOAD_WIDTH];
 u8 tx_buff[RX_PLOAD_WIDTH];
void Delay(__IO u32 nCount)
{
  for(; nCount != 0; nCount--);
} 

/**
  * @brief  SPI�� I/O����
  * @param  ��
  * @retval ��
  */
 void SPI_NRF_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
  /*������ӦIO�˿ڵ�ʱ��*/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//��SYSCFGʱ��

	SPI2_Config(DMA_DISABLE,INTER_DISABLE);  //SPI2��ʼ��

  /*����SPI_NRF_SPI��CE����*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);     //��ʱ����CE ����ΪPC4


   /*����SPI_NRF_SPI��IRQ����*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN ;  //����
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOC, &GPIO_InitStructure); //��ʱ��������PC5
	//�����ⲿ�ж�

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource5);   
  EXTI_InitStructure.EXTI_Line = EXTI_Line5;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	//�����ж����ȼ�

  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
		  
  /* �����Զ���ĺ꣬��������csn���ţ�NRF�������״̬ */
  NRF_CSN_HIGH(); 
 
}





/**
  * @brief   ������NRF��/дһ�ֽ�����
  * @param   д�������
  *		@arg dat 
  * @retval  ��ȡ�õ�����
  */
u8 SPI_NRF_RW(u8 dat)
{  	
   /* �� SPI���ͻ������ǿ�ʱ�ȴ� */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
  
   /* ͨ�� SPI2����һ�ֽ����� */
  SPI_I2S_SendData(SPI2, dat);		
 
   /* ��SPI���ջ�����Ϊ��ʱ�ȴ� */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI2);
}

/**
  * @brief   ������NRF�ض��ļĴ���д������
  * @param   
  *		@arg reg:NRF������+�Ĵ�����ַ
  *		@arg dat:��Ҫ��Ĵ���д�������
  * @retval  NRF��status�Ĵ�����״̬
  */
u8 SPI_NRF_WriteReg(u8 reg,u8 dat)
{
 	u8 status;

	/*�õ�CSN��ʹ��SPI����*/
    NRF_CSN_LOW();
				
	/*��������Ĵ����� */
	status = SPI_NRF_RW(reg);
		 
	 /*��Ĵ���д������*/
    SPI_NRF_RW(dat); 
	          
	/*CSN���ߣ����*/	   
  	NRF_CSN_HIGH();	
	
	/*����״̬�Ĵ�����ֵ*/
   	return(status);
}

/**
  * @brief   ���ڴ�NRF�ض��ļĴ�����������
  * @param   
  *		@arg reg:NRF������+�Ĵ�����ַ
  * @retval  �Ĵ����е�����
  */
u8 SPI_NRF_ReadReg(u8 reg)
{
 	u8 reg_val;

	/*�õ�CSN��ʹ��SPI����*/
 	NRF_CSN_LOW();
				
  	 /*���ͼĴ�����*/
	SPI_NRF_RW(reg); 

	 /*��ȡ�Ĵ�����ֵ */
	reg_val = SPI_NRF_RW(NOP);
	            
   	/*CSN���ߣ����*/
	NRF_CSN_HIGH();		
   	
	
	return reg_val;
}	

/**
  * @brief   ������NRF�ļĴ����ж���һ������
  * @param   
  *		@arg reg : NRF������+�Ĵ�����ַ
  *		@arg pBuf�����ڴ洢���������ļĴ������ݵ����飬�ⲿ����
  * 	@arg bytes: pBuf�����ݳ���
  * @retval  NRF��status�Ĵ�����״̬
  */
u8 SPI_NRF_ReadBuf(u8 reg,u8 *pBuf,u8 bytes)
{
 	u8 status, byte_cnt;

	/*�õ�CSN��ʹ��SPI����*/
	NRF_CSN_LOW();
		
	/*���ͼĴ�����*/		
	status = SPI_NRF_RW(reg); 

 	/*��ȡ����������*/
	 for(byte_cnt=0;byte_cnt<bytes;byte_cnt++)		  
	   pBuf[byte_cnt] = SPI_NRF_RW(NOP); //��NRF24L01��ȡ����  

	 /*CSN���ߣ����*/
	NRF_CSN_HIGH();
	
 	return status;		//���ؼĴ���״ֵ̬
}

/**
  * @brief   ������NRF�ļĴ�����д��һ������
  * @param   
  *		@arg reg : NRF������+�Ĵ�����ַ
  *		@arg pBuf���洢�˽�Ҫд��д�Ĵ������ݵ����飬�ⲿ����
  * 	@arg bytes: pBuf�����ݳ���
  * @retval  NRF��status�Ĵ�����״̬
  */
u8 SPI_NRF_WriteBuf(u8 reg ,u8 *pBuf,u8 bytes)
{
	 u8 status,byte_cnt;
	
   	/*�õ�CSN��ʹ��SPI����*/
	 NRF_CSN_LOW();			

	 /*���ͼĴ�����*/	
  	 status = SPI_NRF_RW(reg); 
 	
  	  /*�򻺳���д������*/
	 for(byte_cnt=0;byte_cnt<bytes;byte_cnt++)
		SPI_NRF_RW(*pBuf++);	//д���ݵ������� 	 
	  	   
	/*CSN���ߣ����*/
	NRF_CSN_HIGH();			

  return (status);	//����NRF24L01��״̬ 		
}
   

/**
  * @brief  ���÷���ģʽ
  * @param  ��
  * @retval ��
  */
void NRF_TX_Mode(void)
{  
	NRF_CE_LOW();		

	 SPI_NRF_WriteReg(FLUSH_RX,0xff);									//���RX FIFO�Ĵ���
   
	 SPI_NRF_WriteBuf(NRF_WRITE_REG+TX_ADDR,TX_ADDRESS,TX_ADR_WIDTH);    //дTX�ڵ��ַ 

   SPI_NRF_WriteBuf(NRF_WRITE_REG+RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK   

   SPI_NRF_WriteReg(NRF_WRITE_REG+EN_AA,0x01);     //ʹ��ͨ��0���Զ�Ӧ��    

   SPI_NRF_WriteReg(NRF_WRITE_REG+EN_RXADDR,0x01); //ʹ��ͨ��0�Ľ��յ�ַ  

   SPI_NRF_WriteReg(NRF_WRITE_REG+SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��

   SPI_NRF_WriteReg(NRF_WRITE_REG+RF_CH,CHANAL);       //����RFͨ��ΪCHANAL

   SPI_NRF_WriteReg(NRF_WRITE_REG+RF_SETUP,0x0f);  //����TX�������,0db����,2Mbps,���������濪��   
	
   SPI_NRF_WriteReg(NRF_WRITE_REG+CONFIG,0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	
	 SPI_NRF_WriteReg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ�� 

/*CE���ߣ����뷢��ģʽ*/	
  NRF_CE_HIGH();
    Delay(0xffff); //CEҪ����һ��ʱ��Ž��뷢��ģʽ
}

/**
  * @brief  ��Ҫ����NRF��MCU�Ƿ���������
  * @param  ��
  * @retval SUCCESS/ERROR ��������/����ʧ��
  */
u8 NRF_Check(void)
{
	u8 buf[5]={0xC2,0xC2,0xC2,0xC2,0xC2};
	u8 buf1[5];
	u8 i; 
	 
	/*д��5���ֽڵĵ�ַ.  */  
	SPI_NRF_WriteBuf(NRF_WRITE_REG+TX_ADDR,buf,5);

	/*����д��ĵ�ַ */
	SPI_NRF_ReadBuf(TX_ADDR,buf1,5); 
	 
	/*�Ƚ�*/               
	for(i=0;i<5;i++)
	{
		if(buf1[i]!=0xC2)
		break;
	} 
	       
	if(i==5)
		return SUCCESS ;        //MCU��NRF�ɹ����� 
	else
		return ERROR ;        //MCU��NRF����������
}

/**
  * @brief   ������NRF�ķ��ͻ�������д������
  * @param   
  *		@arg txBuf���洢�˽�Ҫ���͵����ݵ����飬�ⲿ����	
  * @retval  ���ͽ�����ɹ�����TXDS,ʧ�ܷ���MAXRT��ERROR
  */
void NRF_Tx_Dat()
{

	NRF_CE_LOW();   	//NRF�������ģʽ������д��
	
	SPI_NRF_WriteReg(NRF_WRITE_REG+CONFIG,0x0e);  //�л�Ϊ����ģʽ
						
  SPI_NRF_WriteBuf(WR_TX_PLOAD,tx_buff,TX_PLOAD_WIDTH);  /*д���ݵ�TX BUF ��� 32���ֽ�*/
  		
	NRF_CE_HIGH();//  NRF���빤��ģʽ
} 



/**
	*@brief:NRF�¼���־λ
	*@note
	*/
void EXTI9_5_IRQHandler()
{
	u8 state;
	if(EXTI_GetITStatus(EXTI_Line5) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line5);
		
		//������       
		NRF_CE_LOW();   	//NRF�������ģʽ������д��
		
		state=SPI_NRF_ReadReg(STATUS);		/*��ȡstatus�Ĵ�����ֵ  */    
		SPI_NRF_WriteReg(NRF_WRITE_REG+STATUS,state);	/* ����жϱ�־*/ 
    
		if(state&RX_DR)                                 //���յ�����
		{
			SPI_NRF_ReadBuf(RD_RX_PLOAD,rx_buff,RX_PLOAD_WIDTH);//��ȡ����
			SPI_NRF_WriteReg(FLUSH_RX,NOP);          //���RX FIFO�Ĵ���
		}
		else if((state&TX_DS) | (state&MAX_RT))    //���ͳɹ����ߴﵽ����ʹ���
		{
			SPI_NRF_WriteReg(FLUSH_TX,NOP);    //���TX FIFO�Ĵ��� 
			SPI_NRF_WriteReg(NRF_WRITE_REG+CONFIG,0x0f);  //�������ģʽ	
		}
		else   //����
		{
			SPI_NRF_WriteReg(FLUSH_RX,NOP);          //���RX FIFO�Ĵ���
			SPI_NRF_WriteReg(FLUSH_TX,NOP);    //���TX FIFO�Ĵ��� 		
		}	
		NRF_CE_HIGH();   	//NRF���빤��ģʽ
	}

}




/*********************************************END OF FILE**********************/
