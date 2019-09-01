#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F429开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/11/23
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//初始化PB1为输出.并使能时钟	    
//LED IO初始化
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOB_CLK_ENABLE();           //开启GPIOB时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();           //开启GPIOB时钟
    __HAL_RCC_GPIOC_CLK_ENABLE();           //开启GPIOB时钟	
	
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1 | GPIO_PIN_2; //PB1,0  |GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_NOPULL;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FAST;     //高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
		
	  GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
	  HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	
	  GPIO_Initure.Pin=GPIO_PIN_4|GPIO_PIN_5;
	  HAL_GPIO_Init(GPIOC,&GPIO_Initure);	
	
//    GPIO_Initure.Pin=GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5; //PB1,0
//    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
//    GPIO_Initure.Pull=GPIO_NOPULL;          //上拉
//    GPIO_Initure.Speed=GPIO_SPEED_FAST;     //高速
//    HAL_GPIO_Init(GPIOH,&GPIO_Initure);
	
//    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);	//PB0置1 
  //  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);	//PB1置1  
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6,GPIO_PIN_RESET);	//置0
		
		//HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_SET);	//PB1置1  	
		//HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4|GPIO_PIN_5,GPIO_PIN_SET);	//PB1置1  
//	HAL_GPIO_WritePin(GPIOH,GPIO_PIN_2,GPIO_PIN_RESET);	//PH2=0
//	HAL_GPIO_WritePin(GPIOH,GPIO_PIN_3,GPIO_PIN_RESET);	//PH3=0
}
