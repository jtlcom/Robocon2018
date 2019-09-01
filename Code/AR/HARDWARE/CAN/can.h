#ifndef __CAN_H
#define __CAN_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F746开发板
//CAN驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/12/29
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//CAN1接收RX0中断使能
#define CAN1_RX0_INT_ENABLE	0		//0,不使能;1,使能.
#define abs(x) ((x)>0? (x):(-(x)))

#define OpenLoop_Mode                       0x01  //开环模式
#define Current_Mode                        0x02  //电流模式
#define Velocity_Mode                       0x03  //速度模式
#define Position_Mode                       0x04  //开环模式
#define Velocity_Position_Mode              0x05  //开环模式
#define Current_Velocity_Mode               0x06  //开环模式
#define Current_Position_Mode               0x07  //开环模式
#define Current_Velocity_Position_Mode      0x08  //开环模式

u8 CAN1_Mode_Init(u32 tsjw,u32 tbs2,u32 tbs1,u16 brp,u32 mode);//CAN初始化
u8 CAN1_Send_Msg(u8* msg,u8 len);						//发送数据
u8 CAN1_Receive_Msg(u8 *buf);							//接收数据
u8 CAN1_ID_Send_Msg(uint32_t id,u8* msg,u8 len);//发送函数 字写入id
u8 CAN1_ID_Receive_Msg(u8* buf,uint32_t* stdidbuf);


void CAN_RoboModule_DRV_Reset(unsigned char Group,unsigned char Number);
void CAN_RoboModule_DRV_Mode_Choice(unsigned char Group,unsigned char Number,unsigned char Mode);
void CAN_RoboModule_DRV_Velocity_Mode(unsigned char Group,unsigned char Number,short Temp_PWM,short Temp_Velocity);
void CAN_RoboModule_DRV_Config(unsigned char Group,unsigned char Number,unsigned char Temp_Time,unsigned char Ctl1_Ctl2);
void CAN_RoboModule_DRV_Online_Check(unsigned char Group,unsigned char Number);
void CAN_RoboModule_DRV_OpenLoop_Mode(unsigned char Group,unsigned char Number,short Temp_PWM);

#endif
