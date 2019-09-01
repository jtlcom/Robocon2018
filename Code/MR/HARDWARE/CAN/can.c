#include "can.h"
#include "usart.h"
#include "delay.h"
#include "led.h"





CAN_HandleTypeDef   CAN1_Handler;   //CAN1句柄
CanTxMsgTypeDef     TxMessage;      //发送消息
CanRxMsgTypeDef     RxMessage;      //接收消息

unsigned int CAN_Time_Out = 0;
unsigned char can_tx_success_flag = 0;


short Real_Current_Value[4] = {0};
short Real_Velocity_Value[4] = {0};
long Real_Position_Value[4] = {0};
char Real_Online[4] = {0};
char Real_Ctl1_Value[4] = {0};
char Real_Ctl2_Value[4] = {0};

static void CAN_Delay_Us(unsigned int t)
{
	int i;
	for(i=0;i<t;i++)
	{
		int a=40;
		while(a--);
	}
}

////CAN初始化
//tsjw:重新同步跳跃时间单元.范围:CAN_SJW_1TQ~CAN_SJW_4TQ
//tbs2:时间段2的时间单元.   范围:CAN_BS2_1TQ~CAN_BS2_8TQ;
//tbs1:时间段1的时间单元.   范围:CAN_BS1_1TQ~CAN_BS1_16TQ
//brp :波特率分频器.范围:1~1024; tq=(brp)*tpclk1
//波特率=Fpclk1/((tbs1+tbs2+1)*brp); 其中tbs1和tbs2我们只用关注标识符上标志的序号，例如CAN_BS2_1TQ，我们就认为tbs2=1来计算即可。
//mode:CAN_MODE_NORMAL,普通模式;CAN_MODE_LOOPBACK,回环模式;
//Fpclk1的时钟在初始化的时候设置为45M,如果设置CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_8tq,6,CAN_MODE_LOOPBACK);
//则波特率为:45M/((6+8+1)*6)=500Kbps
//返回值:0,初始化OK;
//    其他,初始化失败; 

u8 CAN1_Mode_Init(u32 tsjw,u32 tbs2,u32 tbs1,u16 brp,u32 mode)
{
    CAN_FilterConfTypeDef  CAN1_FilerConf;
    
    CAN1_Handler.Instance=CAN1; 
    CAN1_Handler.pTxMsg=&TxMessage;     //发送消息
    CAN1_Handler.pRxMsg=&RxMessage;     //接收消息
    CAN1_Handler.Init.Prescaler=brp;    //分频系数(Fdiv)为brp+1
    CAN1_Handler.Init.Mode=mode;        //模式设置 
    CAN1_Handler.Init.SJW=tsjw;         //重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1TQ~CAN_SJW_4TQ
    CAN1_Handler.Init.BS1=tbs1;         //tbs1范围CAN_BS1_1TQ~CAN_BS1_16TQ
    CAN1_Handler.Init.BS2=tbs2;         //tbs2范围CAN_BS2_1TQ~CAN_BS2_8TQ
    CAN1_Handler.Init.TTCM=DISABLE;     //非时间触发通信模式 
    CAN1_Handler.Init.ABOM=DISABLE;     //软件自动离线管理
    CAN1_Handler.Init.AWUM=DISABLE;     //睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
    CAN1_Handler.Init.NART=DISABLE;      //禁止报文自动传送 
    CAN1_Handler.Init.RFLM=DISABLE;     //报文不锁定,新的覆盖旧的 
    CAN1_Handler.Init.TXFP=ENABLE;     //优先级由报文标识符决定 
	
    if(HAL_CAN_Init(&CAN1_Handler)!=HAL_OK) return 1;   //初始化
    
    CAN1_FilerConf.FilterIdHigh=0X0000;     //32位ID
    CAN1_FilerConf.FilterIdLow=0X0000;
    CAN1_FilerConf.FilterMaskIdHigh=0X0000; //32位MASK
    CAN1_FilerConf.FilterMaskIdLow=0X0000;  
    CAN1_FilerConf.FilterFIFOAssignment=CAN_FILTER_FIFO0;//过滤器0关联到FIFO0
    CAN1_FilerConf.FilterNumber=0;          //过滤器0
    CAN1_FilerConf.FilterMode=CAN_FILTERMODE_IDMASK;
    CAN1_FilerConf.FilterScale=CAN_FILTERSCALE_32BIT;
    CAN1_FilerConf.FilterActivation=ENABLE; //激活滤波器0
    CAN1_FilerConf.BankNumber=14;
	
    if(HAL_CAN_ConfigFilter(&CAN1_Handler,&CAN1_FilerConf)!=HAL_OK) return 2;//滤波器初始化
	
    return 0;
}

//CAN底层驱动，引脚配置，时钟配置，中断配置
//此函数会被HAL_CAN_Init()调用
//hcan:CAN句柄
void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_CAN1_CLK_ENABLE();                //使能CAN1时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();			    //开启GPIOA时钟
	
    GPIO_Initure.Pin=GPIO_PIN_11|GPIO_PIN_12;   //PA11,12
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;          //推挽复用
    GPIO_Initure.Pull=GPIO_PULLUP;              //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FAST;         //快速
    GPIO_Initure.Alternate=GPIO_AF9_CAN1;       //复用为CAN1
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);         //初始化
    
#if CAN1_RX0_INT_ENABLE
    __HAL_CAN_ENABLE_IT(&CAN1_Handler,CAN_IT_FMP0);//FIFO0消息挂起中断允许.	  
    //CAN1->IER|=1<<1;		//FIFO0消息挂起中断允许.	
    HAL_NVIC_SetPriority(CAN1_RX0_IRQn,1,2);    //抢占优先级1，子优先级2
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);          //使能中断
#endif	
}

/*************************************************************************
                          CAN1_TX_IRQHandler
描述：CAN1的发送中断函数
*************************************************************************/
void CAN1_TX_IRQHandler(void)
{
  if(__HAL_CAN_GET_IT_SOURCE(&CAN1_Handler,CAN_IT_TME)!= RESET) 
	{
	   __HAL_CAN_CLEAR_FLAG(&CAN1_Handler,CAN_IT_TME);
      can_tx_success_flag=1;
   }
}

/****************************************************************************************
                                       复位指令
Group   取值范围 0-7
Number  取值范围 0-15，其中Number==0时，为广播发送
*****************************************************************************************/
void CAN_RoboModule_DRV_Reset(unsigned char Group,unsigned char Number)
{
//	printf("enter reset!!!\r\n");
	u8  res;
  u8 resetbuffer[8] = {0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55};
  unsigned short can_id = 0x000;  
  
    if((Group<=7)&&(Number<=15))
    {
        can_id |= Group<<8;
        can_id |= Number<<4;
    }
    else
    {
        return;
    }
  can_tx_success_flag = 0;
	res = CAN1_ID_Send_Msg(can_id,resetbuffer,8);
//		printf("res:%d\r\n",res);
	CAN_Time_Out = 0;
	while(can_tx_success_flag == 0)
	{
			CAN_Delay_Us(1);
			CAN_Time_Out++;
			if(CAN_Time_Out>100)
			{
//				  printf("fuwei chaoshi tuichu!!!\r\n");
					break;
			}
	}
}
/****************************************************************************************
                                     模式选择指令
Group   取值范围 0-7
Number  取值范围 0-15，其中Number==0时，为广播发送

Mode    取值范围

OpenLoop_Mode                       0x01
Current_Mode                        0x02
Velocity_Mode                       0x03
Position_Mode                       0x04
Velocity_Position_Mode              0x05
Current_Velocity_Mode               0x06
Current_Position_Mode               0x07
Current_Velocity_Position_Mode      0x08
*****************************************************************************************/
void CAN_RoboModule_DRV_Mode_Choice(unsigned char Group,unsigned char Number,unsigned char Mode)
{
    unsigned short can_id = 0x001;
    u8 buffer[8] = {0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55};
    if((Group<=7)&&(Number<=15))
    {
        can_id |= Group<<8;
        can_id |= Number<<4;
    }
    else
    {
        return;
    }
    buffer[0] = Mode;
		can_tx_success_flag = 0;
    CAN1_ID_Send_Msg(can_id,buffer,8);
    
    CAN_Time_Out = 0;
    while(can_tx_success_flag == 0)
    {
        CAN_Delay_Us(1);
        CAN_Time_Out++;
        if(CAN_Time_Out>100)
        {
            break;
        }
    }
}

/****************************************************************************************
                                   速度模式下的数据指令
Group   取值范围 0-7

Number  取值范围 0-15，其中Number==0时，为广播发送

temp_pwm的取值范围如下：
0 ~ +5000，满值5000，其中temp_pwm = 5000时，最大输出电压为电源电压

temp_velocity的取值范围如下：
-32768 ~ +32767，单位RPM

*****************************************************************************************/
void CAN_RoboModule_DRV_Velocity_Mode(unsigned char Group,unsigned char Number,short Temp_PWM,short Temp_Velocity)
{
    unsigned short can_id = 0x004;
		u8 buffer[8] = {0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55};
    
    if((Group<=7)&&(Number<=15))
    {
        can_id |= Group<<8;
        can_id |= Number<<4;
    }
    else
    {
        return;
    }
    

    if(Temp_PWM > 5000)
    {
        Temp_PWM = 5000;
    }
    else if(Temp_PWM < -5000)
    {
        Temp_PWM = -5000;
    }
    
    if(Temp_PWM < 0)
    {
        Temp_PWM = abs(Temp_PWM);
    }
    
    buffer[0] = (unsigned char)((Temp_PWM>>8)&0xff);
    buffer[1] = (unsigned char)(Temp_PWM&0xff);
    buffer[2] = (unsigned char)((Temp_Velocity>>8)&0xff);
    buffer[3] = (unsigned char)(Temp_Velocity&0xff);
    
    can_tx_success_flag = 0;
    CAN1_ID_Send_Msg(can_id,buffer,8);
    
    CAN_Time_Out = 0;
    while(can_tx_success_flag == 0)
    {
        CAN_Delay_Us(1);
        CAN_Time_Out++;
        if(CAN_Time_Out>100)
        {
            break;
        }
    }
}

/****************************************************************************************
                                   开环模式下的数据指令
Group   取值范围 0-7

Number  取值范围 0-15，其中Number==0时，为广播发送

temp_pwm的取值范围如下：
0 ~ +5000，满值5000，其中temp_pwm = ±5000时，最大输出电压为电源电压

*****************************************************************************************/
void CAN_RoboModule_DRV_OpenLoop_Mode(unsigned char Group,unsigned char Number,short Temp_PWM)
{
    unsigned short can_id = 0x002;
		u8 buffer[8] = {0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55};
    
    
    if((Group<=7)&&(Number<=15))
    {
        can_id |= Group<<8;
        can_id |= Number<<4;
    }
    else
    {
        return;
    }
    

    if(Temp_PWM > 5000)
    {
        Temp_PWM = 5000;
    }
    else if(Temp_PWM < -5000)
    {
        Temp_PWM = -5000;
    }
    
    buffer[0] = (unsigned char)((Temp_PWM>>8)&0xff);
    buffer[1] = (unsigned char)(Temp_PWM&0xff);
		
    can_tx_success_flag = 0;
    CAN1_ID_Send_Msg(can_id,buffer,8);
    
    CAN_Time_Out = 0;
    while(can_tx_success_flag == 0)
    {
        CAN_Delay_Us(1);
        CAN_Time_Out++;
        if(CAN_Time_Out>100)
        {
//					  printf("kaihuan chaoshi tuichu\r\n");
            break;
        }
    }
}

/****************************************************************************************
                                      配置指令
Temp_Time的取值范围: 0 ~ 255，为0时候，为关闭电流速度位置反馈功能
Ctl1_Ctl2的取值范围：0 or 1 ，当不为0 or 1，则认为是0，为关闭左右限位检测功能
特别提示：Ctl1，Ctl2的功能仅存在于102 301，其余版本驱动器，Ctl1_Ctl2 = 0 即可
*****************************************************************************************/
void CAN_RoboModule_DRV_Config(unsigned char Group,unsigned char Number,unsigned char Temp_Time,unsigned char Ctl1_Ctl2)
{
    unsigned short can_id = 0x00A;
    u8 buffer[8] = {0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55};
    
    if((Group<=7)&&(Number<=15))
    {
        can_id |= Group<<8;
        can_id |= Number<<4;
    }
    else
    {
        return;
    }
    
    if((Ctl1_Ctl2 != 0x00)&&(Ctl1_Ctl2 != 0x01))
    {
        Ctl1_Ctl2 = 0x00;
    }
    
    
    buffer[0] = Temp_Time;
    buffer[1] = Ctl1_Ctl2;
    
    can_tx_success_flag = 0;
    CAN1_ID_Send_Msg(can_id,buffer,8);
    
    CAN_Time_Out = 0;
    while(can_tx_success_flag == 0)
    {
        CAN_Delay_Us(1);
        CAN_Time_Out++;
        if(CAN_Time_Out>100)
        {
//					  printf("peizhi chaoshi tuichu\r\n");
            break;
        }
    }
}

/****************************************************************************************
                                      在线检测
*****************************************************************************************/
void CAN_RoboModule_DRV_Online_Check(unsigned char Group,unsigned char Number)
{
    unsigned short can_id = 0x00F;
		u8 buffer[8] = {0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55};
    
    if((Group<=7)&&(Number<=15))
    {
        can_id |= Group<<8;
        can_id |= Number<<4;
    }
    else
    {
        return;
    }
    
    can_tx_success_flag = 0;
    CAN1_ID_Send_Msg(can_id,buffer,8);
    
    CAN_Time_Out = 0;
    while(can_tx_success_flag == 0)
    {
        CAN_Delay_Us(1);
        CAN_Time_Out++;
        if(CAN_Time_Out>100)
        {
            break;
        }
    }
}


//本接收数据的函数，默认为4个驱动器，都挂在0组，编号为1、2、3、4
/*************************************************************************
                          CAN1_RX0_IRQHandler
描述：CAN1的接收中断函数
*************************************************************************/
void CAN1_RX0_IRQHandler(void)
{
//    CanRxMsgTypeDef rx_message;
	  uint32_t stdidbuf[1];
		  u8      buf[8];
//  if(__HAL_CAN_GET_IT_SOURCE(&CAN1_Handler,CAN_IT_TME)!= RESET) 
//	{
//	   __HAL_CAN_CLEAR_FLAG(&CAN1_Handler,CAN_IT_TME);
//      can_tx_success_flag=1;
//   }    
    if (__HAL_CAN_GET_IT_SOURCE(&CAN1_Handler,CAN_IT_FMP0)!= RESET)
	{
        __HAL_CAN_CLEAR_FLAG(&CAN1_Handler,CAN_IT_FMP0);
//    if(HAL_CAN_Receive(&CAN1_Handler,CAN_FIFO0,0)!=HAL_OK) return 0;//接收数据，超时时间设置为0	
        CAN1_ID_Receive_Msg( buf, stdidbuf);

				if(stdidbuf == (u32 *)0x1B)
				{
						Real_Current_Value[0] = (buf[0]<<8)|(buf[1]);
						Real_Velocity_Value[0] = (buf[2]<<8)|(buf[3]);
						Real_Position_Value[0] = ((buf[4]<<24)|(buf[5]<<16)|(buf[6]<<8)|(buf[7]));
				}
				else if(stdidbuf== (u32 *)0x2B)
				{
						Real_Current_Value[1] = (buf[0]<<8)|(buf[1]);
						Real_Velocity_Value[1] = (buf[2]<<8)|(buf[3]);
						Real_Position_Value[1] = ((buf[4]<<24)|(buf[5]<<16)|(buf[6]<<8)|(buf[7]));
				}
				else if(stdidbuf== (u32 *)0x3B)
				{
						Real_Current_Value[2] = (buf[0]<<8)|(buf[1]);
						Real_Velocity_Value[2] = (buf[2]<<8)|(buf[3]);
						Real_Position_Value[2] = ((buf[4]<<24)|(buf[5]<<16)|(buf[6]<<8)|(buf[7]));
				}
				else if(stdidbuf== (u32 *)0x4B)
				{
						Real_Current_Value[3] = (buf[0]<<8)|(buf[1]);
						Real_Velocity_Value[3] = (buf[2]<<8)|(buf[3]);
						Real_Position_Value[3] = ((buf[4]<<24)|(buf[5]<<16)|(buf[6]<<8)|(buf[7]));
				}
				else if(stdidbuf== (u32 *)0x1F)
				{
						Real_Online[0] = 1;
				}
				else if(stdidbuf== (u32 *)0x2F)
				{
						Real_Online[1] = 1;
				}
				else if(stdidbuf== (u32 *)0x3F)
				{
						Real_Online[2] = 1;
				}
				else if(stdidbuf== (u32 *)0x4F)
				{
						Real_Online[3] = 1;
				}
				else if(stdidbuf== (u32 *)0x1C)
				{
						Real_Ctl1_Value[0] = buf[0];
						Real_Ctl2_Value[0] = buf[1];
				}
				else if(stdidbuf== (u32 *)0x2C)
				{
						Real_Ctl1_Value[1] = buf[0];
						Real_Ctl2_Value[1] = buf[1];
				}
				else if(stdidbuf== (u32 *)0x3C)
				{
						Real_Ctl1_Value[2] = buf[0];
						Real_Ctl2_Value[2] = buf[1];
				}
				else if(stdidbuf== (u32 *)0x4C)
				{
						Real_Ctl1_Value[3] = buf[0];
						Real_Ctl2_Value[3] = buf[1];
				}

        
                
    }
}
//#if CAN1_RX0_INT_ENABLE                         //使能RX0中断
////CAN中断服务函数
//void CAN1_RX0_IRQHandler(void)
//{
//    HAL_CAN_IRQHandler(&CAN1_Handler);//此函数会调用CAN_Receive_IT()接收数据
//}

////CAN中断处理过程
////此函数会被CAN_Receive_IT()调用
////hcan:CAN句柄
//void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan)
//{
//    int i=0;
//    //CAN_Receive_IT()函数会关闭FIFO0消息挂号中断，因此我们需要重新打开
//    __HAL_CAN_ENABLE_IT(&CAN1_Handler,CAN_IT_FMP0);//重新开启FIF00消息挂号中断
//    printf("id:%d\r\n",CAN1_Handler.pRxMsg->StdId);
//    printf("ide:%d\r\n",CAN1_Handler.pRxMsg->IDE);
//    printf("rtr:%d\r\n",CAN1_Handler.pRxMsg->RTR);
//    printf("len:%d\r\n",CAN1_Handler.pRxMsg->DLC);
//    for(i=0;i<8;i++)
//    printf("rxbuf[%d]:%d\r\n",i,CAN1_Handler.pRxMsg->Data[i]);
//}
//#endif	

//can发送一组数据(固定格式:ID为0X12,标准帧,数据帧)	
//len:数据长度(最大为8)				     
//msg:数据指针,最大为8个字节.
//返回值:0,成功;
//		 其他,失败;
u8 CAN1_Send_Msg(u8* msg,u8 len)
{	
    u16 i=0;
    CAN1_Handler.pTxMsg->StdId=0X12;        //标准标识符
    CAN1_Handler.pTxMsg->ExtId=0x12;        //扩展标识符(29位)
    CAN1_Handler.pTxMsg->IDE=CAN_ID_STD;    //使用标准帧
    CAN1_Handler.pTxMsg->RTR=CAN_RTR_DATA;  //数据帧
    CAN1_Handler.pTxMsg->DLC=len;                
    for(i=0;i<len;i++)
    CAN1_Handler.pTxMsg->Data[i]=msg[i];
    if(HAL_CAN_Transmit(&CAN1_Handler,10)!=HAL_OK) return 1;     //发送
    return 0;		
}
//can发送一组数据(固定格式:ID为0X12,标准帧,数据帧)	
//len:数据长度(最大为8)				     
//msg:数据指针,最大为8个字节.
//返回值:0,成功;
//		 其他,失败;
u8 CAN1_ID_Send_Msg(uint32_t id,u8* msg,u8 len)
{	
    u16 i=0;
	  HAL_StatusTypeDef  res;
    CAN1_Handler.pTxMsg->StdId=id;        //标准标识符
    CAN1_Handler.pTxMsg->ExtId=0x12;        //扩展标识符(29位)
    CAN1_Handler.pTxMsg->IDE=CAN_ID_STD;    //使用标准帧
    CAN1_Handler.pTxMsg->RTR=CAN_RTR_DATA;  //数据帧
    CAN1_Handler.pTxMsg->DLC=len;                
    for(i=0;i<len;i++)
    CAN1_Handler.pTxMsg->Data[i]=msg[i];
		res = HAL_CAN_Transmit(&CAN1_Handler,10);
//	  printf("send res:%x\r\n",res);
	  if(res != HAL_OK) return 1;     //发送
//    if(HAL_CAN_Transmit(&CAN1_Handler,10)!=HAL_OK) return 1;     //发送
    return 0;		
}
//can口接收数据查询
//buf:数据缓存区;	 
//返回值:0,无数据被收到;
//		 其他,接收的数据长度;
u8 CAN1_Receive_Msg(u8 *buf)
{		   		   
 	u32 i;
    if(HAL_CAN_Receive(&CAN1_Handler,CAN_FIFO0,0)!=HAL_OK) return 0;//接收数据，超时时间设置为0	
    for(i=0;i<CAN1_Handler.pRxMsg->DLC;i++)
    buf[i]=CAN1_Handler.pRxMsg->Data[i];
	return CAN1_Handler.pRxMsg->DLC;	
}
//can口接收数据查询
//buf:数据缓存区;	 
//返回值:0,无数据被收到;
//		 其他,接收的数据长度;
u8 CAN1_ID_Receive_Msg(u8* buf,uint32_t* stdidbuf)
{		   		   
 	u32 i;
    if(HAL_CAN_Receive(&CAN1_Handler,CAN_FIFO0,0)!=HAL_OK) return 0;//接收数据，超时时间设置为0	
    for(i=0;i<CAN1_Handler.pRxMsg->DLC;i++)
    buf[i]=CAN1_Handler.pRxMsg->Data[i];
		stdidbuf[0] = CAN1_Handler.pRxMsg->StdId;
	return CAN1_Handler.pRxMsg->DLC;	
}

