#include "usart2.h"
#include "delay.h"
////////////////////////////////////////////////////////////////////////////////// 	 
extern int res;

float pos_x=0;
float pos_y=0;
float zangle=0;
float xangle=0;
float yangle=0;
float w_z=0;


#if EN_USART2_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART2_RX_BUF[USART2_REC_LEN];     //接收缓冲,最大USART2_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART2_RX_STA=0;       //接收状态标记	

u8 aRxBuffer2[RXBUFFERSIZE2];//HAL库使用的串口接收缓冲
UART_HandleTypeDef UART2_Handler; //UART句柄

//初始化IO 串口1 
//bound:波特率
void uart2_init(u32 bound)
{	
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOA_CLK_ENABLE();			//使能GPIOA时钟
	__HAL_RCC_USART2_CLK_ENABLE();			//使能USART2时钟
	//UART 初始化设置
	UART2_Handler.Instance=USART2;					    //USART2
	UART2_Handler.Init.BaudRate=bound;				    //波特率
	UART2_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //字长为8位数据格式
	UART2_Handler.Init.StopBits=UART_STOPBITS_1;	    //一个停止位
	UART2_Handler.Init.Parity=UART_PARITY_NONE;		    //无奇偶校验位
	UART2_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //无硬件流控
	UART2_Handler.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	HAL_UART_Init(&UART2_Handler);					    //HAL_UART_Init()会使能UART2


	
	GPIO_Initure.Pin=GPIO_PIN_2;			//PA2-- USART2_Tx
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
	GPIO_Initure.Speed=GPIO_SPEED_FAST;		//高速
	GPIO_Initure.Alternate=GPIO_AF7_USART2;	//复用为USART2
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA9

	GPIO_Initure.Pin=GPIO_PIN_3;			//PA3--USART2_Rx
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA10
	
#if EN_USART2_RX
	HAL_NVIC_EnableIRQ(USART2_IRQn);		//使能USART2中断通道
	HAL_NVIC_SetPriority(USART2_IRQn,3,3);	//抢占优先级3，子优先级3
#endif		
	HAL_UART_Receive_IT(&UART2_Handler, (u8 *)aRxBuffer2, RXBUFFERSIZE2);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
  
}


//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//	if(huart->Instance==USART2)//如果是串口1
//	{
//		if((USART2_RX_STA&0x8000)==0)//接收未完成
//		{
//			if(USART2_RX_STA&0x4000)//接收到了0x0d
//			{
//				if(aRxBuffer2[0]!=0x0a)USART2_RX_STA=0;//接收错误,重新开始
//				else USART2_RX_STA|=0x8000;	//接收完成了 
//			}
//			else //还没收到0X0D
//			{	
//				if(aRxBuffer2[0]==0x0d)USART2_RX_STA|=0x4000;
//				else
//				{
//					USART2_RX_BUF[USART2_RX_STA&0X3FFF]=aRxBuffer2[0] ;
//					USART2_RX_STA++;
//					if(USART2_RX_STA>(USART2_REC_LEN-1))USART2_RX_STA=0;//接收数据错误,重新开始接收	  
//				}		 
//			}
//		}

//	}
//}
// 
////串口1中断服务程序
//void USART2_IRQHandler(void)                	
//{ 
//	u32 timeout=0;
//#if SYSTEM_SUPPORT_OS	 	//使用OS
//	OSIntEnter();    
//#endif
//	
//	HAL_UART_IRQHandler(&UART2_Handler);	//调用HAL库中断处理公用函数
//	
//	timeout=0;
//    while (HAL_UART_GetState(&UART2_Handler) != HAL_UART_STATE_READY)//等待就绪
//	{
//	 timeout++;////超时处理
//     if(timeout>HAL_MAX_DELAY) break;		
//	}
//     
//	timeout=0;
//	while(HAL_UART_Receive_IT(&UART2_Handler, (u8 *)aRxBuffer2, RXBUFFERSIZE2) != HAL_OK)//一次处理完成之后，重新开启中断并设置RxXferCount为1
//	{
//	 timeout++; //超时处理
//	 if(timeout>HAL_MAX_DELAY) break;	
//	}
//#if SYSTEM_SUPPORT_OS	 	//使用OS
//	OSIntExit();  											 
//#endif
//} 
//#endif	


/*下面代码我们直接把中断控制逻辑写在中断服务函数内部。
 说明：采用HAL库处理逻辑，效率不高。*/



//串口1中断服务程序
void USART2_IRQHandler(void)                	
{ 

	static uint8_t ch;
	static union 
	{
		uint8_t data[24];
		float ActVal[6];
	}posture;
	static uint8_t count=0;
	static uint8_t i=0;	
	if((__HAL_UART_GET_FLAG(&UART2_Handler,UART_FLAG_RXNE)!=RESET))  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		ch=USART2->DR; 
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
					printf("strat!\r\n");
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
	HAL_UART_IRQHandler(&UART2_Handler);	
} 
#endif	

 




