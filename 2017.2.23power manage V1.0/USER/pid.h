#ifndef __PID_H__
#define __PID_H__

#include "stm32f4xx.h"

/*****�궨����*********/
#define SPEED_MAX_OUTPUT  8000
#define SPEED_INTERGRAL_LIMIT   3000
#define POWER_MAX_OUTPUT  280
#define POWER_INTERGRAL_LIMIT   100
/*****�궨��������**********/

//#define PID_PARAM_AUTO_ADJUST    //�Զ����ں궨��
#ifdef PID_PARAM_AUTO_ADJUST
typedef struct PID_Param_Auto_t
{
		u16 queue_head,queue_tail;
  	u16 queue_index;
		u8 step_queue_head,step_queue_tail;
		u16 count;
		
		u16 set_stepindex[20][2];
		float error[500];
}PIDParamAuto;


#endif

/*****PIDģʽ ö��������*****/
typedef enum {
	LLAST	= 0,
	LAST 	= 1,
	NOW 	= 2,
	
	POSITION_PID,
	INCREASE_PID,
}PID_MODE;

/****PID�ṹ������*********/
typedef struct __pid_t
{
	float p;
	float i;
	float d;
	
	float set[3];				  //Ŀ��ֵ,����NOW(��ǰ),LAST(�ϴ�), LLAST(���ϴ�)
	float get[3];				  //����ֵ
	float err[3];				  //���
	
	float pout;							//p���
	float iout;							//i���
	float dout;							//d���
	
	float pos_out;						//����λ�����
	float last_pos_out;				//�ϴ�λ�����
	float inc_u;						//��������ֵ
	float inc_out;					//����������� = last_delta_out + delta_u
	float last_inc_out;     //�ϴ��������
	
	PID_MODE PID_Mode;        //pidģʽ(λ��\����)
	uint32_t Max_Output;				//�������޷�
	uint32_t Integral_Limit;		//�����޷�
	
	void (*f_pid_reset)(struct __pid_t *pid, float p, float i, float d);		//pid�����������޸�
	float (*f_cal_pid)(struct __pid_t *pid, float get, float set);   			  //����
}pid_t;



void chassis_pid_init(void);
	
/*************�ⲿ����������****************/


/*************�ⲿ��������������****************/
	
	
#endif  //end of pid.h   


	