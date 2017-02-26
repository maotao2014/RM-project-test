#include "led.h"

void led_init()
{
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //Ê¹ÄÜGPIOCÊ±ÖÓ
	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	
//		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;
		GPIO_Init(GPIOC,&GPIO_InitStructure); 
	
		LED1_ON;
		LED2_OFF;
}


