#include "remote_control.h"
#include "USART.h"
#include "led.h"
#include "delay.h"

/*******外部变量定义区**********/

RC_Ctl_t RC_Ctl;           //遥控器信息结构体
u8 sbus_rx_buffer[18];          //用于接收遥控器数据的数组
/*******外部变量定义区**********/


/**
	*@brief:遥控器信息解码
	*@note :解码所得的信息放在外部变量RC_Ctl中
  */
void RC_Decode(void)
{
	RC_Ctl.rc.ch0 = (sbus_rx_buffer[0]| (sbus_rx_buffer[1] << 8)) & 0x07ff;         //!< Channel 0
	RC_Ctl.rc.ch1 = ((sbus_rx_buffer[1] >> 3) | (sbus_rx_buffer[2] << 5)) & 0x07ff; //!< Channel 1
	RC_Ctl.rc.ch2 = ((sbus_rx_buffer[2] >> 6) | (sbus_rx_buffer[3] << 2) |               
							    (sbus_rx_buffer[4] << 10)) & 0x07ff;                            //!< Channel 2
	RC_Ctl.rc.ch3 = ((sbus_rx_buffer[4] >> 1) | (sbus_rx_buffer[5] << 7)) & 0x07ff; //!< Channel 3
	RC_Ctl.rc.s1  = ((sbus_rx_buffer[5] >> 4)& 0x000C) >> 2; //!< Switch left
	RC_Ctl.rc.s2  = ((sbus_rx_buffer[5] >> 4)& 0x0003); //!< Switch right

	RC_Ctl.mouse.x = sbus_rx_buffer[6] | (sbus_rx_buffer[7] << 8); //!< Mouse X axis
	RC_Ctl.mouse.y = sbus_rx_buffer[8] | (sbus_rx_buffer[9] << 8); //!< Mouse Y axis
	RC_Ctl.mouse.z = sbus_rx_buffer[10] | (sbus_rx_buffer[11] << 8); //!< Mouse Z axis
	RC_Ctl.mouse.press_l = sbus_rx_buffer[12]; //!< Mouse Left Is Press ?
	RC_Ctl.mouse.press_r = sbus_rx_buffer[13]; //!< Mouse Right Is Press ?
	RC_Ctl.key.v = sbus_rx_buffer[14] | (sbus_rx_buffer[15] << 8); //!< KeyBoard value
}

/**
	*@brief:遥控器校验
	*@note:遥控器正常连接返回
 */
void RC_Check()
{
	u8 i;
	while(i!=0)
	{
		if(RC_Ctl.rc.ch0==1024&&RC_Ctl.rc.ch1==1024&&RC_Ctl.rc.ch2==1024&&RC_Ctl.rc.ch3==1024)
		{
			i = 0;
		}
		LED2_TOGGLE;
		LED2_TOGGLE;
		delay_ms(500);
		RC_Decode();
	}

}

