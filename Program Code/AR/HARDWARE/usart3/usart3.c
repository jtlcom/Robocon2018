#include "usart3.h"
#include "delay.h"

extern int res;

float pos_x=0;
float pos_y=0;
float zangle=0;
float xangle=0;
float yangle=0;
float w_z=0;

#if EN_USART3_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART3_RX_BUF[USART3_REC_LEN];     //接收缓冲,最大USART3_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART3_RX_STA=0;       //接收状态标记	

u8 aRxBuffer3[RXBUFFERSIZE3];//HAL库使用的串口接收缓冲
UART_HandleTypeDef UART3_Handler; //UART句柄

//初始化IO 串口1 
//bound:波特率
void uart3_init(u32 bound)
{	
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOB_CLK_ENABLE();			//使能GPIOA时钟
	__HAL_RCC_USART3_CLK_ENABLE();			//使能USART3时钟	
	//UART 初始化设置
	UART3_Handler.Instance=USART3;					    //USART3
	UART3_Handler.Init.BaudRate=bound;				    //波特率
	UART3_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //字长为8位数据格式
	UART3_Handler.Init.StopBits=UART_STOPBITS_1;	    //一个停止位
	UART3_Handler.Init.Parity=UART_PARITY_NONE;		    //无奇偶校验位
	UART3_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //无硬件流控
	UART3_Handler.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	HAL_UART_Init(&UART3_Handler);					    //HAL_UART_Init()会使能UART3

	GPIO_Initure.Pin=GPIO_PIN_10;			//PB10-- USART3_Tx
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
	GPIO_Initure.Speed=GPIO_SPEED_FAST;		//高速
	GPIO_Initure.Alternate=GPIO_AF7_USART3;	//复用为USART3
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//初始化PA9

	GPIO_Initure.Pin=GPIO_PIN_11;			//PB11--USART3_Rx
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//初始化PA10
	
#if EN_USART3_RX
	HAL_NVIC_EnableIRQ(USART3_IRQn);		//使能USART3中断通道
	HAL_NVIC_SetPriority(USART3_IRQn,3,2);	//抢占优先级3，子优先级3
#endif		
	HAL_UART_Receive_IT(&UART3_Handler, (u8 *)aRxBuffer3, RXBUFFERSIZE3);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
  
}

//串口1中断服务程序
void USART3_IRQHandler(void)                	
{ 

	static uint8_t ch;
	static union 
	{
		uint8_t data[24];
		float ActVal[6];
	}posture;
	static uint8_t count=0;
	static uint8_t i=0;	
	if((__HAL_UART_GET_FLAG(&UART3_Handler,UART_FLAG_RXNE)!=RESET))  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		ch=USART3->DR; 
		res=ch;
		//printf("res:%d",res);
		switch(count)
		{
			case 0:
				if(ch==0x0d)
					count++;
				else 
					count=0;
				break;
			case 1:
				if(ch==0x0a)
				{
					i=0;
					count++;
				}
				else if(ch==0x0d);
				else 
					count=0;
				break;
			case 2:
				posture.data[i]=ch;
			i++;
			if(i>=24)
			{
				i=0;
				count++;
			}
			break;
			case 3:
				if(ch==0x0a)
					count++;
				else
					count=0;
				break;
			case 4:
				if(ch==0x0d)
				{
//					printf("strat!\r\n");
					zangle=posture.ActVal[0];
					xangle=posture.ActVal[1];
					yangle=posture.ActVal[2];
					pos_x=posture.ActVal[3];
					pos_y=posture.ActVal[4];
					w_z=posture.ActVal[5];
				}
				count=0;
				break;
			default:
				count=0;
			break;
					
		}
	
	}
	HAL_UART_IRQHandler(&UART3_Handler);	
} 

//UART底层初始化，时钟使能，引脚配置，中断配置
//此函数会被HAL_UART_Init()调用
//huart:串口句柄

//void HAL_UART_MspInit(UART_HandleTypeDef *huart)
//{
//    //GPIO端口设置
//	GPIO_InitTypeDef GPIO_Initure;
//	
//	if(huart->Instance==USART3)//如果是串口1，进行串口1 MSP初始化
//	{
//		__HAL_RCC_GPIOB_CLK_ENABLE();			//使能GPIOA时钟
//		__HAL_RCC_USART3_CLK_ENABLE();			//使能USART3时钟
//	
//		GPIO_Initure.Pin=GPIO_PIN_10;			//PB10-- USART3_Tx
//		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
//		GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
//		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//高速
//		GPIO_Initure.Alternate=GPIO_AF7_USART3;	//复用为USART3
//		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//初始化PA9

//		GPIO_Initure.Pin=GPIO_PIN_11;			//PB11--USART3_Rx
//		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//初始化PA10
//		
//#if EN_USART3_RX
//		HAL_NVIC_EnableIRQ(USART3_IRQn);		//使能USART3中断通道
//		HAL_NVIC_SetPriority(USART3_IRQn,3,3);	//抢占优先级3，子优先级3
//#endif	
//	}

//}

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//	if(huart->Instance==USART3)//如果是串口1
//	{
//		if((USART3_RX_STA&0x8000)==0)//接收未完成
//		{
//			if(USART3_RX_STA&0x4000)//接收到了0x0d
//			{
//				if(aRxBuffer3[0]!=0x0a)USART3_RX_STA=0;//接收错误,重新开始
//				else USART3_RX_STA|=0x8000;	//接收完成了 
//			}
//			else //还没收到0X0D
//			{	
//				if(aRxBuffer3[0]==0x0d)USART3_RX_STA|=0x4000;
//				else
//				{
//					USART3_RX_BUF[USART3_RX_STA&0X3FFF]=aRxBuffer3[0] ;
//					USART3_RX_STA++;
//					if(USART3_RX_STA>(USART3_REC_LEN-1))USART3_RX_STA=0;//接收数据错误,重新开始接收	  
//				}		 
//			}
//		}

//	}
//}
// 
////串口1中断服务程序
//void USART3_IRQHandler(void)                	
//{ 
//	u32 timeout=0;
//#if SYSTEM_SUPPORT_OS	 	//使用OS
//	OSIntEnter();    
//#endif
//	
//	HAL_UART_IRQHandler(&UART3_Handler);	//调用HAL库中断处理公用函数
//	
//	timeout=0;
//    while (HAL_UART_GetState(&UART3_Handler) != HAL_UART_STATE_READY)//等待就绪
//	{
//	 timeout++;////超时处理
//     if(timeout>HAL_MAX_DELAY) break;		
//	}
//     
//	timeout=0;
//	while(HAL_UART_Receive_IT(&UART3_Handler, (u8 *)aRxBuffer3, RXBUFFERSIZE3) != HAL_OK)//一次处理完成之后，重新开启中断并设置RxXferCount为1
//	{
//	 timeout++; //超时处理
//	 if(timeout>HAL_MAX_DELAY) break;	
//	}
//#if SYSTEM_SUPPORT_OS	 	//使用OS
//	OSIntExit();  											 
//#endif
//} 
#endif	


/*下面代码我们直接把中断控制逻辑写在中断服务函数内部。
 说明：采用HAL库处理逻辑，效率不高。*/
/*


//串口1中断服务程序
void USART3_IRQHandler(void)                	
{ 
	u8 Res;
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntEnter();    
#endif
	if((__HAL_UART_GET_FLAG(&UART3_Handler,UART_FLAG_RXNE)!=RESET))  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
        HAL_UART_Receive(&UART3_Handler,&Res,1,1000); 
		if((USART3_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART3_RX_STA&0x4000)//接收到了0x0d
			{
				if(Res!=0x0a)USART3_RX_STA=0;//接收错误,重新开始
				else USART3_RX_STA|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(Res==0x0d)USART3_RX_STA|=0x4000;
				else
				{
					USART3_RX_BUF[USART3_RX_STA&0X3FFF]=Res ;
					USART3_RX_STA++;
					if(USART3_RX_STA>(USART3_REC_LEN-1))USART3_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}   		 
	}
	HAL_UART_IRQHandler(&UART3_Handler);	
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntExit();  											 
#endif
} 
#endif	
*/
 




