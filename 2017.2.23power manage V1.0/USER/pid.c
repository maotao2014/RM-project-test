#include "pid.h"

/****外部变量定义区****************/
pid_t pid_chassis_speed[4];  //底盘速度PID
pid_t pid_chassis_current;   //底盘电流PID

/****外部变量定义区结束****************/
#ifdef PID_PARAM_AUTO_ADJUST
	PIDParamAuto PID_AutoTest;
#endif

#define ABS(x)		((x>0)? x: -x) 
static void abs_limit(float *a, float ABS_MAX){
	if(*a > ABS_MAX)
		*a = ABS_MAX;
	if(*a < -ABS_MAX)
		*a = -ABS_MAX;
}


/**
	* @brief:位置式\增量式pid计算
	* @param:pid_t pid 参数结构体
	         get       实际值  
					 set       设定值
  * @retval:计算结果
  * @note :
	* @test :NO!
 */
 static float pid_calc(pid_t *pid ,float get,float set)
 {
	pid->get[NOW] = get;
	pid->set[NOW] = set;
	pid->err[NOW] = set - get;	//set - measure
	
	
	if(pid->PID_Mode == POSITION_PID) //位置式pid
	{
		pid->pout = pid->p * pid->err[NOW];
		pid->iout += pid->i * pid->err[NOW];
		pid->dout = pid->d * (pid->err[NOW] - pid->err[LAST] );
		abs_limit(&(pid->iout), pid->Integral_Limit);
		pid->pos_out = pid->pout + pid->iout + pid->dout;
		abs_limit(&(pid->pos_out), pid->Max_Output);
		pid->last_pos_out = pid->pos_out;	//update last time 
	}
	else if(pid->PID_Mode == INCREASE_PID)//增量式P
	{
		//添加部分
//		if(pid->err[NOW]<-1500)
//			pid->pout = pid->p / 4.0f * (pid->err[NOW] - pid->err[LAST]);			
//		else
		pid->pout = pid->p * (pid->err[NOW] - pid->err[LAST]);
		pid->iout = pid->i * pid->err[NOW];
		pid->dout = pid->d * (pid->err[NOW] - 2*pid->err[LAST] + pid->err[LLAST]);
		
		abs_limit(&(pid->iout), pid->Integral_Limit);
		pid->inc_u = pid->pout + pid->iout + pid->dout;
		pid->inc_out = pid->last_inc_out + pid->inc_u;
		abs_limit(&(pid->inc_out), pid->Max_Output);
		pid->last_inc_out = pid->inc_out;	//update last time
	}

	
	pid->err[LLAST] = pid->err[LAST];
	pid->err[LAST] = pid->err[NOW];
	pid->get[LLAST] = pid->get[LAST];
	pid->get[LAST] = pid->get[NOW];
	pid->set[LLAST] = pid->set[LAST];
	pid->set[LAST] = pid->set[NOW];

	return pid->PID_Mode==POSITION_PID ? pid->pos_out : pid->inc_out;
 
 }

/**
	* @brief:pid参数改变
	* @param:pid_t pid 参数结构体
	         kp ki kd
  * @retval:none
  * @note :
	* @test :NO!
  */
 static void pid_reset(pid_t	*pid, float kp, float ki, float kd)
{
	pid->p = kp;
	pid->i = ki;
	pid->d = kd;
}


/**
	* @brief:pid结构体初始化函数
	* @param:pid_t pid 参数结构体
	         PID_MODE  
					 maxout    pid计算结果最大输出
					 intergral_limit  积分限幅
					 kp ki kd  
  * @note :
	* @test :NO!
 */
void PID_struct_init(	pid_t* pid,PID_MODE mode,
	                    uint32_t maxout,	uint32_t intergral_limit,
										  float 	kp, float 	ki, float 	kd)
{
  pid->PID_Mode       = mode;
	pid->Max_Output     = maxout;
	pid->Integral_Limit = intergral_limit;
	pid->p              = kp;
	pid->d              = kd;
	pid->i              = ki;
	pid->f_pid_reset    = pid_reset;
	pid->f_cal_pid      = pid_calc;
}


/**
	*brief:
	*note :
 */
void chassis_pid_init(void)
{
	u8 i;
	for(i=0;i<4;i++)
		PID_struct_init(&pid_chassis_speed[i],INCREASE_PID,SPEED_MAX_OUTPUT,SPEED_INTERGRAL_LIMIT,
									2.5,0.014,0.3);//2.5,0.014,0.41.5,0.014,0
	PID_struct_init(&pid_chassis_current,INCREASE_PID,POWER_MAX_OUTPUT,POWER_INTERGRAL_LIMIT,
									0.21,0.01,0);
}

#ifdef PID_PARAM_AUTO_ADJUST
void PIDAutoProcess(pid_t *PIDParam)
{
	u8 i;
	short temp;
	u16 proce_num;
	//入队操作
	PID_AutoTest.error[PID_AutoTest.queue_head] = PIDParam->err[NOW];
	PID_AutoTest.queue_head ++;
	if(PID_AutoTest.queue_head>=500)
		PID_AutoTest.queue_head = 0;
	if(PID_AutoTest.queue_head-PID_AutoTest.queue_tail==0)  //头追到尾了
	{
		PID_AutoTest.queue_tail++;
		if(PID_AutoTest.queue_tail>=500)
			PID_AutoTest.queue_tail =0;
	}
	// 设定值改变记录
	PID_AutoTest.count++;      //调用计数
	if(PIDParam->set[NOW]!=PIDParam->set[LAST])
	{
		PID_AutoTest.step_queue_head++;
		if(PID_AutoTest.step_queue_head>=20)
		{
			PID_AutoTest.set_stepindex[PID_AutoTest.step_queue_head-1][0] = PID_AutoTest.count;  //当设定值改变时，记的为上一次的set值输入后的次数
			PID_AutoTest.step_queue_head = 0;
			PID_AutoTest.set_stepindex[PID_AutoTest.step_queue_head][1] = PID_AutoTest.queue_head;//记录当次set值法发生改变对头的位置
		}
		else
		{
			PID_AutoTest.set_stepindex[PID_AutoTest.step_queue_head][1] = PID_AutoTest.queue_head;//记录当次set值法发生改变对头的位置
			PID_AutoTest.set_stepindex[PID_AutoTest.step_queue_head-1][0] = PID_AutoTest.count;  //当设定值改变时，记的为上一次的set值输入后的次数		
		}
		if(PID_AutoTest.step_queue_tail==PID_AutoTest.step_queue_head)
		{
			PID_AutoTest.step_queue_tail++;
			if(PID_AutoTest.step_queue_tail>=20)
				PID_AutoTest.step_queue_tail = 0;
		}
		PID_AutoTest.count = 0;   //重新开始计数
	}
	for(PID_AutoTest.set_stepindex[PID_AutoTest.step_queue_head][0]=PID_AutoTest.count,i = PID_AutoTest.step_queue_head;
			i!=PID_AutoTest.step_queue_tail;)
	{
		proce_num = proce_num + PID_AutoTest.set_stepindex[i][0];
		if(proce_num>=500)
		{
//			if(proce_num-PID_AutoTest.set_stepindex[i][0]>=460)   //队尾的set值调节作用在errror中的数值太少，无法使用，舍弃
//			{
//				i++;
//				temp = 0;
//			}
//			else                                                 //更新队尾的set的调用值
				PID_AutoTest.set_stepindex[i][0] = 500-proce_num + PID_AutoTest.set_stepindex[i][0];
			break;		
		}
	 if(i == 0)
			i = 20;
	 i--;
	}       
	//此时i值的信息比较重要，不能清零
	while(1)
	{
		if(PID_AutoTest.set_stepindex[i][0]<40)    //errror中的数值太少，无法使用，舍弃
			i++;
		else //数据分析
		{
			
			;
		}
		i++;    //
		if(i>=20)
			i = 0;
		if(i==PID_AutoTest.step_queue_head)
			break;
		
	}	
	
	

		

}


#endif






