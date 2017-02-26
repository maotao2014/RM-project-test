#ifndef __PID_H__
#define __PID_H__

#include "stm32f4xx.h"

/*****宏定义区*********/
#define SPEED_MAX_OUTPUT  8000
#define SPEED_INTERGRAL_LIMIT   3000
#define POWER_MAX_OUTPUT  280
#define POWER_INTERGRAL_LIMIT   100
/*****宏定义区结束**********/

//#define PID_PARAM_AUTO_ADJUST    //自动调节宏定义
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

/*****PID模式 枚举体声明*****/
typedef enum {
	LLAST	= 0,
	LAST 	= 1,
	NOW 	= 2,
	
	POSITION_PID,
	INCREASE_PID,
}PID_MODE;

/****PID结构体声明*********/
typedef struct __pid_t
{
	float p;
	float i;
	float d;
	
	float set[3];				  //目标值,包括NOW(当前),LAST(上次), LLAST(上上次)
	float get[3];				  //测量值
	float err[3];				  //误差
	
	float pout;							//p输出
	float iout;							//i输出
	float dout;							//d输出
	
	float pos_out;						//本次位置输出
	float last_pos_out;				//上次位置输出
	float inc_u;						//本次增量值
	float inc_out;					//本次增量输出 = last_delta_out + delta_u
	float last_inc_out;     //上次增量输出
	
	PID_MODE PID_Mode;        //pid模式(位置\增量)
	uint32_t Max_Output;				//最大输出限幅
	uint32_t Integral_Limit;		//积分限幅
	
	void (*f_pid_reset)(struct __pid_t *pid, float p, float i, float d);		//pid的三个参数修改
	float (*f_cal_pid)(struct __pid_t *pid, float get, float set);   			  //计算
}pid_t;



void chassis_pid_init(void);
	
/*************外部变量声明区****************/


/*************外部变量声明区结束****************/
	
	
#endif  //end of pid.h   


	