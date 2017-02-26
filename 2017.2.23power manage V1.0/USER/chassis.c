/***********˵��***************

���̿��ƴ���ȫ�������ڴ�c�ļ���

�ⲿ����chassis_task����
************˵������**********************************/


#include "chassis.h"
#include "led.h"
#include "stdio.h"

/******ȫ�ֱ���������*********/

//����4����ֻ����ң��������ģʽ
short left_x_offset;               //ң�����󰴼�ˮƽ����ƫ����          �����ò���
short left_y_offset;               //ң�����󰴼���ֱ����ƫ����          ������ʱ����     ���Ƴ���������ת(��Ӧ������̨�涯)
short right_x_offset;              //ң�����Ұ���ˮƽ����ƫ����          ���Ƴ�������ƽ��
short right_y_offset;              //ң�����Ұ�����ֱ����ƫ����          ���Ƴ���ǰ����� 

u16 decoded_key;       //���ң��������ʱ����ң������Ϣ����ɵȼ۵İ�����Ϣ������Ǽ������ʱ��ֱ�ӽ�����ֵ��ֵ���˱���
u16 last_decoded_key;  //��¼�ϴν����İ�����Ϣ�������ж�ǰ�󰴼���Ϣ�Ƿ�һ�½����ж��Ƿ����
u16 llast_decoded_key;//��¼���ϴν�������Ϣ������ͬ��

float power_limit = 0;    //�������Ʊ���
float Vx,Vy,Rz;           //Vx x����ƽ��;Vy y����ƽ��;Rz ��z����ת,������

int SetWheelSpeed[4];  //4�������趨������
int WheelSpeedOut[4]={0};  //������̵����ֵ
float RealPowerValue;     //ʵʱ����ֵ

/******ȫ�ֱ�������������*********/
volatile short RealWheelSpeed[4];  //����(���)ʵʱ�ٶ�    ��ע:������˵���ӱ���Ϊ3510���

/********�ⲿ����������************/


/********�ⲿ��������������************/

/**********�ⲿ����������***********/
extern pid_t pid_chassis_speed[4];
extern pid_t pid_chassis_current;
extern CanRxMsg CAN1_rx_message;	
extern u16 DMA_SPI1_RX_Arry[10];      //SPI1��������
extern RC_Ctl_t RC_Ctl;       //ң������Ϣ�ṹ��
/***********�ⲿ��������������*******************/




/*************************************������*****************************************************/

/**
	*  @brief:�ٶȽ���ӳ�亯��
	*  @note :��������
						�����뷶ΧΪ1600�������ΧΪ10000.����xΪ���뷶Χ������һ��ֵ�������������Ϊ��ֵ��Ӧ�����ֵ
  */
int map(int x,int in_min,int in_max,int out_min,int out_max )   //ӳ��
{
	return(int)((x-in_min)*(out_max-out_min)/(in_max-in_min)+out_min);

}


/**
	*@brief:���̵�����ݷ���
	*@note:
 */
void CAN1_3510_SetMsg()
{	
	CanTxMsg TxMessage;  
  TxMessage.StdId=0x200;						 //ʹ�ñ�׼ID     ���͵������� ��IDֵ�̶�
  TxMessage.IDE=CAN_ID_STD;					 //��׼ģʽ
  TxMessage.RTR=CAN_RTR_DATA;				 //���͵�������
  TxMessage.DLC=0x08;							   //���ݳ���Ϊ8�ֽ�
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
	* @brief  :���̿���
	* @note   :
	* @version:
	* @test   :
  */
void Chassis_Task(void)
{
	 u8  i;
	 float  IdealPowerValue; //�������ջ�������õ��Ĺ���ֵ����������������ֵ
	//������Ϣ���㲿��
	 if(RC_Ctl.rc.s2 == 1)              //ң��������
	 {
			right_x_offset = RC_Ctl.rc.ch0 - 1024;        //ҡ�˴����м�λ��ʱ��Ӧ��ֵΪ1024,��ͬ
			right_y_offset = RC_Ctl.rc.ch1 - 1024;
			left_x_offset  = RC_Ctl.rc.ch2 - 1024;
		  left_y_offset  = RC_Ctl.rc.ch3 - 1024;
		 //��Ӧ����������״̬����Ϊƫ��������ֵ����100ʱ���϶�Ϊ��Ч���Ȼ�󽫶�Ӧ��λ��һ����������
			//��ҡ�˷���
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
			//��ҡ�˷���
			if(left_x_offset>100)
				Rz = -30;
			else if(left_x_offset<-100)
				Rz = 30;
			else 
				Rz = 0;
			
			
	 }//end  if(RC_Ctl.rc.s1==1)
	 else if(RC_Ctl.rc.s2 == 3)         //��������
	 {
			decoded_key = RC_Ctl.key.v;
	 
	 }//end  else if(RC_Ctl.rc.s1==2)
		
	 //���ʱջ�
	 RealPowerValue = DMA_SPI1_RX_Arry[0]/100.0f * 24.0f;
	 IdealPowerValue = pid_chassis_current.f_cal_pid(&pid_chassis_current,RealPowerValue,power_limit);
//		//������
//	 IdealPowerValue = power_limit;
	 
	 //���ʷ���
	 if(last_decoded_key!=decoded_key)
	 {
		 //���ԭ��������ֵ
			pid_chassis_current.err[NOW] = 0;pid_chassis_current.err[LAST] = 0;
			pid_chassis_current.err[LLAST]=0;pid_chassis_current.last_inc_out=0;
			power_limit = 0;
			Vx = 0;Vy = 0;
			last_decoded_key = decoded_key;       //��ֵ����һ�εĽ���ֵ 
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
	 //�����ٶȽ���,���ѹ��ʱջ������õ���ֵ���ݰ���ֵ�����4���֣��ó�ʵ�ֲ�ͬ״̬���ƶ�
	 SetWheelSpeed[0] = (Vx + Vy - Rz)*5;  //�Ŵ�5��
	 SetWheelSpeed[1] = -(Vx - Vy + Rz)*5;
	 SetWheelSpeed[2] = (Vx - Vy - Rz)*5;
	 SetWheelSpeed[3] = -(Vx + Vy + Rz)*5;
	 //����������Ϊ3510�����������趨ֵ
	 SetWheelSpeed[0] =map(SetWheelSpeed[0],-800,800,-5000,5000);
	 SetWheelSpeed[1] =map(SetWheelSpeed[1],-800,800,-5000,5000);
	 SetWheelSpeed[2] =map(SetWheelSpeed[2],-800,800,-5000,5000);
	 SetWheelSpeed[3] =map(SetWheelSpeed[3],-800,800,-5000,5000);	
	 
	 //����ٶȱջ�
	 for(i=0;i<4;i++)  //����ջ�
	 {
			WheelSpeedOut[i] = pid_chassis_speed[i].f_cal_pid(&pid_chassis_speed[i],RealWheelSpeed[i],SetWheelSpeed[i]); 	 
	 }
	 
	 CAN1_3510_SetMsg();  //��������

}


/**
	*@brief:���ʼ�⣬ģ�����ϵͳ
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
		AveragePower20ms = TotalPower20ms / 20.0f;  // �õ�20ms����ƽ��ֵ
		EnergeStorage = EnergeStorage - (AveragePower20ms - 80.0f)*0.02f;  //�õ�������ʣ��ֵ
		if(EnergeStorage<0)  //����ֵ�����꣬��Ѫ
		{
			LED1_OFF;
		}
		else if(EnergeStorage>=60)
			EnergeStorage = 60.0f;
		TotalPower20ms = 0;
//		printf("%f\n",AveragePower20ms);
	}
}

