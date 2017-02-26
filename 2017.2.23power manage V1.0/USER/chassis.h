
/*************说明*************
底盘控制代码头文件


*************说明结束**************************/

#ifndef __CHASSIS_H__
#define __CHASSIS_H__

#include "stm32f4xx.h"
#include "remote_control.h"   
#include "can.h"
#include "pid.h"

//typedef union POWER_VAVLUE
//{
//	u8 cPowerValue[8];
//	float fPowerValue;
//}Power_Value_t;



#define MAX_POWER  48



void Chassis_Task(void);
void over_power_check(void);


#endif


