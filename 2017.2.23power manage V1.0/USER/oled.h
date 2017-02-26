/******************** 2013                  ������ӿƼ� **************************
 * �ļ���  ��main.c
 * ����    ��LED��ˮ�ƣ�Ƶ�ʿɵ�����         
 * ʵ��ƽ̨��STM32������
 * ��汾  ��ST3.5.0
 *
 * ����    ��iGeek_Hao

**********************************************************************************/
#define _OLED_H_
#ifdef  _OLED_H_
#include "stm32f4xx.h"


#define byte unsigned char 
#if 0
sbit lcd_cs1=P3^4;   /*�ӿڶ���:lcd_cs1����LCD��cs1*/

sbit lcd_rs=P1^0;    /*�ӿڶ���:lcd_rs����LCD��rs*/
sbit lcd_reset=P1^2; /*�ӿڶ���:lcd_reset����LCD��reset*/
sbit lcd_sid=P1^4;   /*�ӿڶ���:lcd_sid����LCD��sid*/
sbit lcd_sclk=P1^6;  /*�ӿڶ���:lcd_sclk����LCD��sclk*/

#endif

/* the macro definition to trigger the led on or off 
 * 1 - off
 - 0 - on
 */
#define ON  0
#define OFF 1



#define lcd_sid(a)	if (a)	\
					GPIO_SetBits(GPIOA,GPIO_Pin_6);\
					else		\
					GPIO_ResetBits(GPIOA,GPIO_Pin_6)

#define lcd_sclk(a)	if (a)	\
					GPIO_SetBits(GPIOA,GPIO_Pin_5);\
					else		\
					GPIO_ResetBits(GPIOA,GPIO_Pin_5)


extern unsigned char const bmp1[];
extern unsigned char const jiong1[];
extern unsigned char const lei1[];

void OLED_GPIO_Config(void);
/*дָ�LCDģ��*/
void transfer_command_lcd(unsigned char data1);   
/*д���ݵ�LCDģ��*/
void transfer_data_lcd(unsigned char data1);
/*��ʱ*/
void delay(int n_ms);               
/*LCDģ���ʼ��*/
void initial_lcd(void);
void lcd_address(unsigned char page,unsigned char column);
/*ȫ������*/
void clear_screen(void);
/*��ʾ128x64����ͼ��*/
void display_128x64(const unsigned char *dp);
/*��ʾ132x64����ͼ��*/
//void display_132x64(unsigned char *dp);
/*��ʾ16x16����ͼ�񡢺��֡���Ƨ�ֻ�16x16���������ͼ��*/
void display_graphic_16x16(unsigned int page,unsigned int column,const unsigned char *dp);
/*��ʾ8x16����ͼ��ASCII, ��8x16����������ַ�������ͼ��*/
void display_graphic_8x16(unsigned int page,unsigned char column,unsigned char *dp);
/*��ʾ5*7����ͼ��ASCII, ��5x7����������ַ�������ͼ��*/
void display_graphic_5x7(unsigned int page,unsigned char column,unsigned char *dp);


void LED_P6x8Char(byte x,byte y,byte ch);
void LED_P6x8Str(byte x,byte y,byte ch[]); 
void LED_P8x16Str(byte x,byte y,byte ch[]);

#endif
