#ifndef __LED_H__
#define __LED_H__


#include "stm32f4xx.h"


#define	digitalHi(p,i)			{p->BSRRL=i;}			//����Ϊ�ߵ�ƽ		
#define digitalLo(p,i)			{p->BSRRH=i;}				//����͵�ƽ
#define digitalToggle(p,i)		{p->ODR ^=i;}			//�����ת״̬

#define LED1_TOGGLE		digitalToggle(GPIOC,GPIO_Pin_0)
#define LED1_OFF		digitalHi(GPIOC,GPIO_Pin_0)
#define LED1_ON			digitalLo(GPIOC,GPIO_Pin_0)


#define LED2_TOGGLE		digitalToggle(GPIOC,GPIO_Pin_1)
#define LED2_OFF		digitalHi(GPIOC,GPIO_Pin_1)
#define LED2_ON			digitalLo(GPIOC,GPIO_Pin_1)

//#define LED3_TOGGLE		digitalToggle(GPIOC,GPIO_Pin_7)
//#define LED3_OFF		digitalHi(GPIOC,GPIO_Pin_7)
//#define LED3_ON			digitalLo(GPIOC,GPIO_Pin_7)

void led_init(void);

#endif 


