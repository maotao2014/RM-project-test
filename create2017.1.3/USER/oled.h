/******************** 2013                  信意电子科技 **************************
 * 文件名  ：main.c
 * 描述    ：LED流水灯，频率可调……         
 * 实验平台：STM32开发板
 * 库版本  ：ST3.5.0
 *
 * 作者    ：iGeek_Hao

**********************************************************************************/
#define _OLED_H_
#ifdef  _OLED_H_
#include "stm32f4xx.h"


#define byte unsigned char 
#if 0
sbit lcd_cs1=P3^4;   /*接口定义:lcd_cs1就是LCD的cs1*/

sbit lcd_rs=P1^0;    /*接口定义:lcd_rs就是LCD的rs*/
sbit lcd_reset=P1^2; /*接口定义:lcd_reset就是LCD的reset*/
sbit lcd_sid=P1^4;   /*接口定义:lcd_sid就是LCD的sid*/
sbit lcd_sclk=P1^6;  /*接口定义:lcd_sclk就是LCD的sclk*/

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
/*写指令到LCD模块*/
void transfer_command_lcd(unsigned char data1);   
/*写数据到LCD模块*/
void transfer_data_lcd(unsigned char data1);
/*延时*/
void delay(int n_ms);               
/*LCD模块初始化*/
void initial_lcd(void);
void lcd_address(unsigned char page,unsigned char column);
/*全屏清屏*/
void clear_screen(void);
/*显示128x64点阵图像*/
void display_128x64(const unsigned char *dp);
/*显示132x64点阵图像*/
//void display_132x64(unsigned char *dp);
/*显示16x16点阵图像、汉字、生僻字或16x16点阵的其他图标*/
void display_graphic_16x16(unsigned int page,unsigned int column,const unsigned char *dp);
/*显示8x16点阵图像、ASCII, 或8x16点阵的自造字符、其他图标*/
void display_graphic_8x16(unsigned int page,unsigned char column,unsigned char *dp);
/*显示5*7点阵图像、ASCII, 或5x7点阵的自造字符、其他图标*/
void display_graphic_5x7(unsigned int page,unsigned char column,unsigned char *dp);


void LED_P6x8Char(byte x,byte y,byte ch);
void LED_P6x8Str(byte x,byte y,byte ch[]); 
void LED_P8x16Str(byte x,byte y,byte ch[]);

#endif
