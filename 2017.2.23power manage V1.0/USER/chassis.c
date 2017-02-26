/***********说明***************

底盘控制代码全部包含在此c文件中

外部调用chassis_task函数
************说明结束**********************************/


#include "chassis.h"
#include "led.h"
#include "stdio.h"

/******全局变量定义区*********/

//下面4变量只用于遥控器控制模式
short left_x_offset;               //遥控器左按键水平方向偏移量          底盘用不着
short left_y_offset;               //遥控器左按键垂直方向偏移量          底盘暂时不用     控制车体左右旋转(后应考虑云台随动)
short right_x_offset;              //遥控器右按键水平方向偏移量          控制车体左右平移
short right_y_offset;              //遥控器右按键垂直方向偏移量          控制车体前后进退 

u16 decoded_key;       //针对遥控器控制时，将遥控器信息换算成等价的按键信息；如果是键鼠控制时，直接将按键值赋值给此变量
u16 last_decoded_key;  //记录上次解码后的按键信息，用作判断前后按键信息是否一致进而判断是否加速
u16 llast_decoded_key;//记录上上次解码后的信息，作用同上

float power_limit = 0;    //功率限制变量
float Vx,Vy,Rz;           //Vx x方向平移;Vy y方向平移;Rz 绕z轴旋转,即跟随

int SetWheelSpeed[4];  //4个麦轮设定的速率
int WheelSpeedOut[4]={0};  //输给底盘电机的值
float RealPowerValue;     //实时功率值

/******全局变量定义区结束*********/
volatile short RealWheelSpeed[4];  //轮子(电机)实时速度    备注:以上所说轮子本质为3510电机

/********外部变量定义区************/


/********外部变量定义区结束************/

/**********外部变量声明区***********/
extern pid_t pid_chassis_speed[4];
extern pid_t pid_chassis_current;
extern CanRxMsg CAN1_rx_message;	
extern u16 DMA_SPI1_RX_Arry[10];      //SPI1接收数组
extern RC_Ctl_t RC_Ctl;       //遥控器信息结构体
/***********外部变量声明区结束*******************/




/*************************************函数区*****************************************************/

/**
	*  @brief:速度解算映射函数
	*  @note :比例换算
						如输入范围为1600，输出范围为10000.输入x为输入范围内任意一个值，经过计算后换算为此值对应的输出值
  */
int map(int x,int in_min,int in_max,int out_min,int out_max )   //映射
{
	return(int)((x-in_min)*(out_max-out_min)/(in_max-in_min)+out_min);

}


/**
	*@brief:底盘电机数据发送
	*@note:
 */
void CAN1_3510_SetMsg()
{	
	CanTxMsg TxMessage;  
  TxMessage.StdId=0x200;						 //使用标准ID     发送底盘数据 此ID值固定
  TxMessage.IDE=CAN_ID_STD;					 //标准模式
  TxMessage.RTR=CAN_RTR_DATA;				 //发送的是数据
  TxMessage.DLC=0x08;							   //数据长度为8字节
  TxMessage.Data[0]=(u8)(WheelSpeedOut[0]>>8);//WheelSpeedOut[0]   
  TxMessage.Data[1]=(u8)(WheelSpeedOut[0]);//WheelSpeedOut[0]
  TxMessage.Data[2]=(u8)(WheelSpeedOut[1]>>8);//WheelSpeedOut[1]
  TxMessage.Data[3]=(u8)(WheelSpeedOut[1]);//WheelSpeedOut[1]
  TxMessage.Data[4]=(u8)(WheelSpeedOut[2]>>8);//WheelSpeedOut[2]
  TxMessage.Data[5]=(u8)(WheelSpeedOut[2]);//WheelSpeedOut[2]
  TxMessage.Data[6]=(u8)(WheelSpeedOut[3]>>8);//WheelSpeedOut[3]
  TxMessage.Data[7]=(u8)(WheelSpeedOut[3]);//WheelSpeedOut[3]
//	TxMessage.Data[0]=(u8)(1200>>8);//WheelSpeedOut[0]   
//  TxMessage.Data[1]=(u8)(1200);//WheelSpeedOut[0]
//  TxMessage.Data[2]=(u8)(-1200>>8);//WheelSpeedOut[1]
//  TxMessage.Data[3]=(u8)(-1200);//WheelSpeedOut[1]
//  TxMessage.Data[4]=(u8)(1200>>8);//WheelSpeedOut[2]
//  TxMessage.Data[5]=(u8)(1200);//WheelSpeedOut[2]
//  TxMessage.Data[6]=(u8)(-1200>>8);//WheelSpeedOut[3]
//  TxMessage.Data[7]=(u8)(1200);//WheelSpeedOut[3]
  CAN_Transmit(CAN1, &TxMessage);
}

/**
	* @brief  :底盘控制
	* @note   :
	* @version:
	* @test   :
  */
void Chassis_Task(void)
{
	 u8  i;
	 float  IdealPowerValue; //电流环闭环结束后得到的功率值，即理想的输出功率值
	//控制信息解算部分
	 if(RC_Ctl.rc.s2 == 1)              //遥控器操作
	 {
			right_x_offset = RC_Ctl.rc.ch0 - 1024;        //摇杆处于中间位置时对应的值为1024,下同
			right_y_offset = RC_Ctl.rc.ch1 - 1024;
			left_x_offset  = RC_Ctl.rc.ch2 - 1024;
		  left_y_offset  = RC_Ctl.rc.ch3 - 1024;
		 //对应按键的四种状态，认为偏移量绝对值大于100时就认定为有效命令，然后将对应的位置一，否则置零
			//右摇杆分析
		 if(right_y_offset>300)
				decoded_key = decoded_key|0x01;
			else
				decoded_key = decoded_key&0xfe;  
			if(right_y_offset<-300)
				decoded_key = decoded_key|0x02;
			else
				decoded_key = decoded_key&0xfd;
			if(right_x_offset<-300)
				decoded_key = decoded_key|0x04;
		  else
				decoded_key = decoded_key&0xfb;
			if(right_x_offset>300)
				decoded_key = decoded_key|0x08;
			else
				decoded_key = decoded_key&0xf7;
			//左摇杆分析
			if(left_x_offset>100)
				Rz = -30;
			else if(left_x_offset<-100)
				Rz = 30;
			else 
				Rz = 0;
			
			
	 }//end  if(RC_Ctl.rc.s1==1)
	 else if(RC_Ctl.rc.s2 == 3)         //按键操作
	 {
			decoded_key = RC_Ctl.key.v;
	 
	 }//end  else if(RC_Ctl.rc.s1==2)
		
	 //功率闭环
	 RealPowerValue = DMA_SPI1_RX_Arry[0]/100.0f * 24.0f;
	 IdealPowerValue = pid_chassis_current.f_cal_pid(&pid_chassis_current,RealPowerValue,power_limit);
//		//调试用
//	 IdealPowerValue = power_limit;
	 
	 //功率分配
	 if(last_decoded_key!=decoded_key)
	 {
		 //清空原方向的误差值
			pid_chassis_current.err[NOW] = 0;pid_chassis_current.err[LAST] = 0;
			pid_chassis_current.err[LLAST]=0;pid_chassis_current.last_inc_out=0;
			power_limit = 0;
			Vx = 0;Vy = 0;
			last_decoded_key = decoded_key;       //赋值给上一次的解码值 
	 }
	 switch(decoded_key)
	 {
		 case 0x05:Vx=IdealPowerValue/2.0f;Vy=-IdealPowerValue/2.0f;break;//W&A
		 case 0x09:Vx=IdealPowerValue/2.0f;Vy=IdealPowerValue/2.0f ;break;//W&D
		 case 0x06:Vx=-IdealPowerValue/2.0f;Vy=-IdealPowerValue/2.0f;break;//S&A
		 case 0x0A:Vx=-IdealPowerValue/2.0f;Vy=IdealPowerValue/2.0f;break;//S&D
		 case 0x01:Vx=IdealPowerValue;Vy=0;break;//W
		 case 0x02:Vx=-IdealPowerValue;Vy=0;break;//S
		 case 0x04:Vx=0;Vy=-IdealPowerValue;break;//A 
		 case 0x08:Vx=0;Vy=IdealPowerValue;break;//D
		 default:Vx=0;Vy=0;power_limit=0;
	 }
	 if(power_limit<MAX_POWER&&decoded_key!=0x00)
		 power_limit=power_limit+0.2f;
	 //麦轮速度解算,即把功率闭环后所得到的值根据按键值分配给4个轮，让车实现不同状态的移动
	 SetWheelSpeed[0] = (Vx + Vy - Rz)*5;  //放大5倍
	 SetWheelSpeed[1] = -(Vx - Vy + Rz)*5;
	 SetWheelSpeed[2] = (Vx - Vy - Rz)*5;
	 SetWheelSpeed[3] = -(Vx + Vy + Rz)*5;
	 //按比例换算为3510电机的输入的设定值
	 SetWheelSpeed[0] =map(SetWheelSpeed[0],-800,800,-5000,5000);
	 SetWheelSpeed[1] =map(SetWheelSpeed[1],-800,800,-5000,5000);
	 SetWheelSpeed[2] =map(SetWheelSpeed[2],-800,800,-5000,5000);
	 SetWheelSpeed[3] =map(SetWheelSpeed[3],-800,800,-5000,5000);	
	 
	 //电机速度闭环
	 for(i=0;i<4;i++)  //电机闭环
	 {
			WheelSpeedOut[i] = pid_chassis_speed[i].f_cal_pid(&pid_chassis_speed[i],RealWheelSpeed[i],SetWheelSpeed[i]); 	 
	 }
	 
	 CAN1_3510_SetMsg();  //发送数据

}


/**
	*@brief:功率检测，模拟裁判系统
	*@note :
*/
float EnergeStorage = 60.0;
u8 Count_20ms;
float AveragePower20ms = 0;
float TotalPower20ms = 0;
void over_power_check()
{
	Count_20ms++;
	TotalPower20ms = TotalPower20ms + DMA_SPI1_RX_Arry[0]/100.0f * 24.0f;
	if(Count_20ms>=20)
	{
		Count_20ms = 0;
		AveragePower20ms = TotalPower20ms / 20.0f;  // 得到20ms功率平均值
		EnergeStorage = EnergeStorage - (AveragePower20ms - 80.0f)*0.02f;  //得到能量槽剩余值
		if(EnergeStorage<0)  //能量值已用完，扣血
		{
			LED1_OFF;
		}
		else if(EnergeStorage>=60)
			EnergeStorage = 60.0f;
		TotalPower20ms = 0;
//		printf("%f\n",AveragePower20ms);
	}
}

