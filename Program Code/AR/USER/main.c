#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
//#include "usart2.h"
#include "lcd.h"
#include "sdram.h"
#include "usart3.h"
#include "can.h"
#include "exti.h"
extern float pos_x,pos_y,zangle,xangle,yangle,w_z;
// ʵ������λ�ú�Ŀ������λ��
float x1,x2,x3,x4;
float y1,y2,y3,y4;
float z1,z2,z3,z4;
int sx=0;
int res = 0;
int cnt = -1 ;
int ctr = -1 ;
int gp = -1 ;
int flyflag = 0 ;
int flycount = 0 ;
short p1=0 ;
short p2=0 ;
short p3=0 ;
// the velocity of three wheels 
short p=100 ;
short pr=50 ;
/*	
**  PB0  LEFT GAS 
**  PA1 RIGHT GAS 
**  PA6  LEFT ELE
**  PB2 RIGHT DOWN 
**  PC4  DOWN HAND 					// unuse 
**  PC5  LIGHTING 
**  PA5 COLOUR LIGHTING 
** 	PE2  RIGHT EMOTIONER 		// unuse 
*/
void lb ( ) 
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
	delay_ms ( 1000 ) ;
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
}
void lg ( ) 			// left get ball 
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
	delay_ms ( 500 ) ;
}
void rg ( ) 			// right get ball 
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
	delay_ms ( 1000 ) ;
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_SET);
}
void lt ( ) 			// left throw ball 
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_SET);
	delay_ms ( 1000 ) ;
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET);
	delay_ms ( 800 ) ;
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);
}
void rt ( ) 			// right throw ball 
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_RESET);
	delay_ms ( 500 ) ;
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
	delay_ms ( 1000 ) ;
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
	delay_ms ( 500 ) ;
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);
}
float jdz ( float ag , float zangle ) 
{
	float t = ag - zangle ;
	if ( t > 0 ) 
	{
		return t ;
	} else 
	{
		return ( - t ) ;
	}
}
void DIS() 
{
	//--------------------------������ʾ------------------------------------
	LCD_ShowString(20,200,200,16,16,"pos_x:         ");
	if(pos_x<0) 
	{
		pos_x = - pos_x ;
		LCD_ShowString(80,200,200,16,16,"-");
		LCD_ShowxNum(90,200,pos_x,6,16,0x80);
	} else		
				LCD_ShowxNum(90,200,pos_x,6,16,0x80);
	//----------------------------����y������-------------------------------------------
	LCD_ShowString(20,220,200,16,16,"pos_y:         ");
	if(pos_y<0) 
	{
		pos_y = - pos_y ;
		LCD_ShowString(80,220,200,16,16,"-");
		LCD_ShowxNum(90,220,pos_y,6,16,0x80);
	} else 
				LCD_ShowxNum(90,220,pos_y,6,16,0x80);
	//-------------------------------���̺����----------------------------------------
	LCD_ShowString(20,380,200,16,16,"zangle:         ");
	if(zangle<0) 
	{
		zangle = - zangle ;
		LCD_ShowString(80,380,200,16,16,"-");
		LCD_ShowxNum(90,380,zangle,6,16,0x80);
	} else 
				LCD_ShowxNum(90,380,zangle,6,16,0x80);
	LCD_ShowString(20,520,200,16,16,"i:         ");
	if(zangle<0) 
	{
		zangle = - zangle ;
		LCD_ShowString(80,380,200,16,16,"-");
		LCD_ShowxNum(90,380,zangle,6,16,0x80);
	} else 
				LCD_ShowxNum(90,520,cnt,6,16,0x80);
}
void start ( ) 									// - 4514   - 2320  0 -> 76 
{
	while ( 1 ) 		// x + + 
	{
		if ( p1 < 2500 ) 
		{
			delay_ms ( 50 ) ;
			p3 = p1 += 100 ;
			p1 += 100 ;
			p2 -= 200 ;
			p2 -= 200 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		}
		if ( pos_x < - 1800 ) 
		{
			break ;
		}
	}
	while ( 1 ) 
	{
		if ( p2 < - 4000 ) 
		{
			delay_ms ( 40 ) ;
			p3 = p1 += 50 ;
			p2 += 200 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		} else 
		{
			break ;
		}
	}
	delay_ms ( 50 ) ;
	p3 = p1 = 4000 ;
	p2 = - 4000 ;
	CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
	CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
	CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
	while ( 1 ) 
	{
		delay_ms ( 50 ) ;
		if ( zangle < 50 ) 
		{
			p3 = p1 = 4000 ;
			p2 = - 4000 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		} else if ( zangle > 50 && zangle < 65 ) 
		{
			p3 = p1 = 3500 ;
			p2 = - 3500 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		} else if ( zangle > 65 && zangle < 75 ) 
		{
			p3 = p1 = 2900 ;
			p2 = - 2900 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		} else if ( zangle > 75 && zangle < 85 ) 
		{
			p3 = p1 = 2200 ;
			p2 = - p1 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		} else if ( zangle > 85 && zangle < 95 ) 
		{
			p3 = p1 = 1600 ;
			p2 = - p1 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		} else if ( zangle > 95 && zangle < 100 ) 
		{
			p3 = p1 = 1100 ;
			p2 = - p1 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		}
		if ( zangle > 100 ) 
		{
			p1 = p2 = p3 = 0 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
			break ;
		}
	}
	p1 = p2 = p3 = 0 ;
	CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
	CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
	CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
}
void getb ( int x ) 
{
	short sd = 500 / x ;
	delay_ms ( 50 ) ;
	CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
	CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
	CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
	if ( pos_x < - 4100 || pos_x > - 3900 ) 
	{
		if ( pos_x < - 4100 ) 
		{
			while ( 1 ) 
			{
				p3 = p1 = - sd ;
				p2 = 2 * sd ;
				if ( pos_x > - 4100 ) 
				{
					break ;
				}
			}
			p1 = p2 = p3 = 0 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		} else 
		{
			while ( 1 ) 
			{
				p3 = p1 = sd ;
				p2 = ( -2 ) * sd ;
				CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
				CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
				CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
				if ( pos_x < - 3900 ) 
				{
					break ;
				}
			}
			p1 = p2 = p3 = 0 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		}
	} else if ( pos_y < - 2200 || pos_y > - 1800 ) 
	{
		if ( pos_y < - 2200 ) 
		{
			while ( 1 ) 
			{
				delay_ms ( 50 ) ;
				p1 = - sd ;
				p3 = sd ;
				CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
				CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
				CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
				if ( pos_y > - 2200 ) 
				{
					break ;
				}
			}
			p1 = p2 = p3 = 0 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		} else 
		{
			while ( 1 ) 
			{
				delay_ms ( 50 ) ;
				p1 = sd ;
				p3 = - sd ;
				CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
				CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
				CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
				if ( pos_y < - 1800 ) 
				{
					break ;
				}
			}
			p1 = p2 = p3 = 0 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		}
	}
	// else if    zangle
}
void zxfun ( float ag ) 
{
	delay_ms ( 100 ) ;
	while ( zangle < ( ag - 2 ) || zangle > ( ag + 2 ) ) 
	{
		if ( zangle < ( ag - 2 ) ) 
		{
			delay_ms ( 50 ) ;
			p1 = p2 = p3 = 800 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
			while ( 1 ) 
			{
				if ( jdz( ag , zangle ) > 50 ) 
				{
					delay_ms ( 50 ) ;
					p1 = p2 = p3 = 1600 ;
					CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
					CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
					CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
				} else if ( jdz( ag , zangle ) > 25 && jdz( ag , zangle ) < 50 ) 
				{
					delay_ms ( 50 ) ;
					p1 = p2 = p3 = 1200 ;
					CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
					CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
					CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
				} else if ( jdz( ag , zangle )  < 25 ) 
				{
					delay_ms ( 50 ) ;
					p1 = p2 = p3 = 800 ;
					CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
					CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
					CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
				}
				if ( zangle > ( ag - 3 ) ) 
				{
					p1 = p2 = p3 = 0 ;
					CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
					CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
					CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
					break ;
				}
			}
		} else if ( zangle > ( ag + 2 ) ) 
		{
			delay_ms ( 50 ) ;
			p1 = p2 = p3 = - 800 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
			while ( 1 ) 
			{
				if ( jdz( ag , zangle ) > 50 ) 
				{
					delay_ms ( 50 ) ;
					p1 = p2 = p3 = - 1600 ;
					CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
					CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
					CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
				} else if ( jdz( ag , zangle ) > 25 && jdz( ag , zangle ) < 50 ) 
				{
					delay_ms ( 50 ) ;
					p1 = p2 = p3 = - 1200 ;
					CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
					CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
					CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
				} else if ( jdz( ag , zangle )  < 25 ) 
				{
					delay_ms ( 50 ) ;
					p1 = p2 = p3 = - 800 ;
					CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
					CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
					CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
				}
				if ( zangle < ( ag + 3 ) ) 
				{
					p1 = p2 = p3 = 0 ;
					CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
					CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
					CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
					break ;
				}
			}
		}
	}
}
void move ( )   		 //  - 4514     - 1210    0    // - 5700  - 2350    80 
{
	p1 = p2 = p3 = 0 ;
	CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
	CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
	CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
	while ( 1 ) 			//  y + + 
	{
		if ( p3 < 2500 ) 
		{
			delay_ms ( 50 ) ;
			p3 += 200 ;
			p1 = - p3 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		}
		if ( pos_y > - 1200 ) 
		{
			break ;
		}
	}
	while ( 1 ) 
	{
		if ( p3 > 700 ) 
		{
			delay_ms ( 100 ) ;
			p3 -= 300 ;
			p1 = - p3 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		}
		if ( pos_y > - 800 ) 
		{
			break ;
		}
	}
	p1 = p2 = p3 = 0 ;
	CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
	CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
	CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
	while ( 1 ) 				// x + + 
	{
		if ( p1 < 1600 ) 
		{
			delay_ms ( 40 ) ;
			p3 = p1 += 200 ;
			p2 -= 400 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		}
		if ( pos_x < - 5800 ) 
		{
			break ;
		}
	}
	while ( 1 ) 				//  x - - 
	{
		if ( p1 > 700 ) 
		{
			delay_ms ( 100 ) ;
			p3 = p1 -= 100 ;
			p2 += 200 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		}
		if ( pos_x < - 6300 ) 
		{
			break ;
		}
	}
	delay_ms ( 50 ) ;
	p3 = p1 /= 2 ;
	p2 /= 2 ;
	CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
	CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
	CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
	delay_ms ( 50 ) ;
	p1 = p2 = p3 = 0 ;
	CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
	CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
	CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
	while ( 1 ) 			// y + + 
	{
		if ( p1 < 2300 ) 
		{
			delay_ms ( 50 ) ;
			p1 += 300 ;
			p3 = - p1 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		}
		if ( pos_y < - 1700 ) 
		{
			break ;
		}
	}
	while ( 1 ) 
	{
		if ( p1 > 700 ) 
		{
			delay_ms ( 100 ) ;
			p1 -= 300 ;
			p3 = - p1 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		}
		if ( pos_y < - 2100 ) 
		{
			break ;
		}
	}
	delay_ms ( 50 ) ;
	p1 /= 2 ;
	p3 = - p1 ;
	p2 = 0 ;
	CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
	CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
	CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
	delay_ms ( 50 ) ;
	p1 = p2 = p3 = 0 ;
	CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
	CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
	CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
}
void jzdw ( float a , float ax , float ay , float ag ) 
{
	if ( pos_x > ( ax + 50 ) || pos_x < ( ax - 50 ) ) 	// for x 
	{
		if ( pos_x > ( ax + 50 ) ) 
		{
			while ( 1 ) 
			{
				delay_ms ( 50 ) ;
				p3 = p1 = 300 ;
				p2 =  - 600 ;
				CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
				CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
				CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
				if ( pos_x < ( ax + 50 ) ) 
				{
					p1 = p2 = p3 = 0 ;
					CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
					CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
					CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
					break ;
				}
			}
		} else if ( pos_x < ( ax - 50 ) ) 
		{
			while ( 1 ) 
			{
				delay_ms ( 50 ) ;
				p3 = p1 = - 300 ;
				p2 = 600 ;
				CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
				CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
				CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
				if ( pos_x > ( ax - 50 ) ) 
				{
					p1 = p2 = p3 = 0 ;
					CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
					CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
					CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
					break ;
				}
			}
		}
	}
	if ( pos_y < ( ay - 50 )  || pos_y > ( ay + 50 ) ) 		// for y 
	{
		if ( pos_y < ( ay - 50 ) ) 
		{
			while ( 1 ) 
			{
				delay_ms ( 50 ) ;
				p1 = - 500 ;
				p2 = 0 ;
				p3 = - p1 ;
				CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
				CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
				CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
				if ( pos_y > ( ay - 50 ) ) 
				{
					p1 = p2 = p3 = 0 ;
					CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
					CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
					CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
					break ;
				}
			}
		} else if ( pos_y > ( ay + 50 ) ) 
		{
			while ( 1 ) 
			{
				delay_ms ( 50 ) ;
				p1 = 500 ;
				p2 = 0 ;
				p3 = - p1 ;
				CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
				CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
				CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
				if ( pos_y < ( ay + 50 ) ) 
				{
					p1 = p2 = p3 = 0 ;
					CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
					CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
					CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
					break ;
				}
			}
		}
	}
	zxfun ( ag ) ;
}
void dw ( float a , float ax , float ay , float ag ) 
{
	if ( pos_x > ( ax + 50 ) || pos_x < ( ax - 50 ) ) 	// for x 
	{
		if ( pos_x > ( ax + 50 ) ) 
		{
			while ( 1 ) 
			{
				delay_ms ( 50 ) ;
				p1 = - 500 ;
				p2 = 0 ;
				p3 = - p1 ;
				CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
				CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
				CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
				if ( pos_x < ( ax + 50 ) ) 
				{
					p1 = p2 = p3 = 0 ;
					CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
					CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
					CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
					break ;
				}
			}
		}
		if ( pos_x < ( ax - 50 ) ) 
		{
			while ( 1 ) 
			{
				delay_ms ( 50 ) ;
				p1 = 500 ;
				p2 = 0 ;
				p3 = - p1 ;
				CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
				CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
				CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
				if ( pos_x > ( ax - 50 ) ) 
				{
					p1 = p2 = p3 = 0 ;
					CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
					CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
					CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
					break ;
				}
			}
		}
	}
	if ( pos_y < ( ay - 50 )  || pos_y > ( ay + 50 ) ) 		// for y 
	{
		if ( pos_y < ( ay - 50 ) ) 
		{
			while ( 1 ) 
			{
				delay_ms ( 50 ) ;
				p3 = p1 = - 300 ;
				p2 = 600 ;
				CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
				CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
				CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
				if ( pos_y > ( ay - 50 ) ) 
				{
					p1 = p2 = p3 = 0 ;
					CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
					CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
					CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
					break ;
				}
			}
		} else if ( pos_y > ( ay + 50 ) ) 
		{
			while ( 1 ) 
			{
				delay_ms ( 50 ) ;
				p3 = p1 = 300 ;
				p2 = - 600 ;
				CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
				CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
				CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
				if ( pos_y < ( ay + 50 ) ) 
				{
					p1 = p2 = p3 = 0 ;
					CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
					CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
					CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
					break ;
				}
			}
		}
	}
}
void ksdw ( float a , float ax , float ay , float ag ) 
{
	if ( pos_x > ( ax + 50 ) || pos_x < ( ax - 50 ) ) 	// for x 
	{
		if ( pos_x > ( ax + 50 ) ) 
		{
			while ( 1 ) 
			{
				delay_ms ( 50 ) ;
				p1 = - 1000 ;
				p2 = 0 ;
				p3 = - p1 ;
				CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
				CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
				CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
				if ( pos_x < ( ax + 50 ) ) 
				{
					p1 = p2 = p3 = 0 ;
					CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
					CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
					CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
					break ;
				}
			}
		}
		if ( pos_x < ( ax - 50 ) ) 
		{
			while ( 1 ) 
			{
				delay_ms ( 50 ) ;
				p1 = 1000 ;
				p2 = 0 ;
				p3 = - p1 ;
				CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
				CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
				CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
				if ( pos_x > ( ax - 50 ) ) 
				{
					p1 = p2 = p3 = 0 ;
					CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
					CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
					CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
					break ;
				}
			}
		}
	}
	if ( pos_y < ( ay - 50 )  || pos_y > ( ay + 50 ) ) 		// for y 
	{
		if ( pos_y < ( ay - 50 ) ) 
		{
			while ( 1 ) 
			{
				delay_ms ( 50 ) ;
				p3 = p1 = - 700 ;
				p2 = 1400 ;
				CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
				CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
				CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
				if ( pos_y > ( ay - 50 ) ) 
				{
					p1 = p2 = p3 = 0 ;
					CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
					CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
					CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
					break ;
				}
			}
		} else if ( pos_y > ( ay + 50 ) ) 
		{
			while ( 1 ) 
			{
				delay_ms ( 50 ) ;
				p3 = p1 = 700 ;
				p2 = - 1400 ;
				CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
				CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
				CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
				if ( pos_y < ( ay + 50 ) ) 
				{
					p1 = p2 = p3 = 0 ;
					CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
					CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
					CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
					break ;
				}
			}
		}
	}
}
void fly ( ) 		//   - 5700   -6430   90 
{
	p1 = p2 = p3 = 0 ;
	CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
	CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
	CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
	while ( 1 )  		// y + + 
	{
		if ( p1 < 3000 ) 
		{
			delay_ms ( 50 ) ;
			p3 = p1 += 100 ;
			p2 -= 200 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
		}
		if ( pos_y < - 4600 ) 
		{
			break ;
		}
	}
	while ( 1 ) 
	{
		if ( p1 > 500 ) 
		{
			delay_ms ( 50 ) ;
			p3 = p1 -= 100 ;
			p2 += 200 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
		}
		if ( pos_y < - 6000 ) 
		{
			break ;
		}
	}
	delay_ms ( 50 ) ;
	p3 = p1 /= 2 ;
	p2 /= 2 ;
	CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
	CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
	CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
	delay_ms ( 50 ) ;
	p1 = p2 = p3 = 0 ;
	CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
	CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
	CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
	dw ( 90 , - 5900 , - 6430 , 90 ) ;
}
void back ( ) 
{
	p1 = p2 = p3 = 0 ;
	CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
	CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
	CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
	while ( 1 ) 
	{
		delay_ms ( 50 ) ;
		if ( p2 < 5000 ) 
		{
			p3 = p1 -= 100 ;
			p3 -= 1 ;
			p2 += 200 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
		}
		if ( pos_y > - 3800 ) 
		{
			break ;
		}
	}
	while ( 1 ) 
	{
		delay_ms ( 50 ) ;
		if ( p2 > - 3000 ) 
		{
			p2 -= 600 ;
			p3 = p1 -= 37 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
		} else 
		{
			break ;
		}
	}
	while ( 1 ) 
	{
		if ( zangle > 30 ) 
		{
			p1 = p2 = p3 = - 3000 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
		} else if ( zangle < 30 && zangle > 22 ) 
		{
			p1 = p2 = p3 = - 2000 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
		} else if ( zangle < 22 && zangle > 15 ) 
		{
			p1 = p2 = p3 = - 1300 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
		} else if ( zangle < 15 && zangle > 10 ) 
		{
			p1 = p2 = p3 = - 700 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
		}
		if ( zangle < 10 ) 
		{
			p1 = p2 = p3 = 0 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
			break ;
		}
	}
	p1 = p2 = p3 = 0 ;
	CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
	CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
	CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
	flycount ++ ;
}
int judge ( ) 
{
	while ( 1 ) 
	{
		if ( ( 0 == HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_3) ) ) 
		{
			rg ( ) ;
			lg ( ) ;
			return 1 ;
			break ;
		}
		if ( HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_5 ) == 0 ) 
		{
			lg ( ) ;
			delay_ms ( 100 ) ;
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);
		}
	}
}
void jzzx ( float ag ) 
{
	delay_ms ( 100 ) ;
	if ( zangle > ag || zangle < ag ) 
	{
		if ( zangle > ag ) 
		{
			while ( 1 ) 
			{
				delay_ms ( 40 ) ;
				p1 = p2 = p3 = - 200 ;
				CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
				CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
				CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
				if ( zangle < ( ag + 1 ) ) 
				{
					p1 = p2 = p3 = 0 ;
					CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
					CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
					CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
					break ;
				}
			}
		} else if ( zangle < ag ) 
		{
			while ( 1 ) 
			{
				delay_ms ( 40 ) ;
				p1 = p2 = p3 = 200 ;
				CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
				CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
				CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
				if ( zangle > ( ag - 1 ) ) 
				{
					p1 = p2 = p3 = 0 ;
					CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
					CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
					CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
					break ;
				}
			}
		}
	}
}
void flydw ( float a , float ax , float ay , float ag ) 
{
	if ( pos_x > ( ax + 10 ) || pos_x < ( ax - 10 ) ) 	// for x 
	{
		if ( pos_x > ( ax + 10 ) ) 
		{
			while ( 1 ) 
			{
				delay_ms ( 50 ) ;
				p1 = - 200 ;
				p2 = 0 ;
				p3 = - p1 ;
				CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
				CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
				CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
				if ( pos_x < ( ax + 10 ) ) 
				{
					p1 = p2 = p3 = 0 ;
					CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
					CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
					CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
					break ;
				}
			}
		}
		if ( pos_x < ( ax - 10 ) ) 
		{
			while ( 1 ) 
			{
				delay_ms ( 50 ) ;
				p1 = 200 ;
				p2 = 0 ;
				p3 = - p1 ;
				CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
				CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
				CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
				if ( pos_x > ( ax - 10 ) ) 
				{
					p1 = p2 = p3 = 0 ;
					CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
					CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
					CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
					break ;
				}
			}
		}
	}
	if ( pos_y < ( ay - 10 )  || pos_y > ( ay + 10 ) ) 		// for y 
	{
		if ( pos_y < ( ay - 10 ) ) 
		{
			while ( 1 ) 
			{
				delay_ms ( 50 ) ;
				p3 = p1 = - 200 ;
				p2 = 400 ;
				CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
				CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
				CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
				if ( pos_y > ( ay - 10 ) ) 
				{
					p1 = p2 = p3 = 0 ;
					CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
					CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
					CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
					break ;
				}
			}
		} else if ( pos_y > ( ay + 10 ) ) 
		{
			while ( 1 ) 
			{
				delay_ms ( 50 ) ;
				p3 = p1 = 200 ;
				p2 = - 400 ;
				CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
				CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
				CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
				if ( pos_y < ( ay - 10 ) ) 
				{
					p1 = p2 = p3 = 0 ;
					CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
					CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
					CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
					break ;
				}
			}
		}
	}
}
void kszx ( float ag ) 
{
	delay_ms ( 200 ) ;
	while ( zangle < ( ag - 5 ) || zangle > ( ag + 5 ) ) 
	{
		if ( zangle < ( ag - 5 ) ) 
		{
			delay_ms ( 50 ) ;
			p1 = p2 = p3 = 600 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
			delay_ms ( 50 ) ;
			p1 = p2 = p3 = 1200 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
			delay_ms ( 50 ) ;
			p1 = p2 = p3 = 2000 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
			while ( 1 ) 
			{
				if ( jdz( ag , zangle ) > 60 ) 
				{
					delay_ms ( 50 ) ;
					p1 = p2 = p3 = 3500 ;
					CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
					CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
					CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
				} else if ( jdz( ag , zangle ) > 35 && jdz( ag , zangle ) < 60 ) 
				{
					delay_ms ( 50 ) ;
					p1 = p2 = p3 = 2700 ;
					CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
					CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
					CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
				} else if ( jdz( ag , zangle ) > 25 && jdz( ag , zangle ) < 35 ) 
				{
					delay_ms ( 50 ) ;
					p1 = p2 = p3 = 1800 ;
					CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
					CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
					CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
				} else if ( jdz( ag , zangle ) > 15 && jdz ( ag , zangle ) < 25 ) 
				{
					delay_ms ( 50 ) ;
					p1 = p2 = p3 = 1500 ;
					CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
					CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
					CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
				} else if ( jdz( ag , zangle ) < 15 ) 
				{
					delay_ms ( 50 ) ;
					p1 = p2 = p3 = 800 ;
					CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
					CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
					CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
				}
				if ( zangle > ( ag - 4 ) ) 
				{
					p1 = p2 = p3 = 0 ;
					CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
					CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
					CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
					break ;
				}
			}
		} else if ( zangle > ( ag + 5 ) ) 
		{
			delay_ms ( 50 ) ;
			p1 = p2 = p3 = - 600 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
			delay_ms ( 50 ) ;
			p1 = p2 = p3 = - 1300 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
			delay_ms ( 50 ) ;
			p1 = p2 = p3 = - 2000 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
			while ( 1 ) 
			{
				if ( jdz( ag , zangle ) > 60 ) 
				{
					delay_ms ( 50 ) ;
					p1 = p2 = p3 = - 3500 ;
					CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
					CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
					CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
				} else if ( jdz( ag , zangle ) > 35 && jdz( ag , zangle ) < 60 ) 
				{
					delay_ms ( 50 ) ;
					p1 = p2 = p3 = - 2700 ;
					CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
					CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
					CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
				} else if ( jdz( ag , zangle ) > 20 && jdz( ag , zangle ) < 35 ) 
				{
					delay_ms ( 50 ) ;
					p1 = p2 = p3 = - 1500 ;
					CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
					CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
					CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
				} else if ( jdz( ag , zangle ) > 10 && jdz( ag , zangle ) < 20 ) 
				{
					delay_ms ( 50 ) ;
					p1 = p2 = p3 = - 1000 ;
					CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
					CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
					CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
				} else if ( jdz( ag , zangle ) < 10 ) 
				{
					delay_ms ( 50 ) ;
					p1 = p2 = p3 = - 500 ;
					CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
					CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
					CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
				}
				if ( zangle < ( ag + 4 ) ) 
				{
					p1 = p2 = p3 = 0 ;
					CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
					CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
					CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
					break ;
				}
			}
		}
	}
}
void test ( ) 
{
	while ( 1 ) 
	{
		delay_ms ( 50 ) ;
		DIS ( ) ;
		if ( HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_5 ) == 0 ) 
		{
			PE5flag = 0 ;
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_SET);
			delay_ms ( 50 ) ;
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_RESET);
			zxfun ( 76 ) ;
			jzzx ( 76 ) ;
			DIS ( ) ;
			delay_ms ( 500 ) ;
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_SET);
			delay_ms ( 50 ) ;
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_RESET);
			delay_ms ( 100 ) ;
			zxfun ( 0 ) ;
			jzzx ( 0 ) ;
			p1 = p2 = p3 = 0 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
			DIS ( ) ;
		}
	}
}
void test1 ( ) 
{
	p1 = p2 = p3 = 0 ;
	CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
	CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
	CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
	while ( 1 ) 
	{
		if ( p1 < 2000 ) 
		{
			delay_ms ( 50 ) ;
			p3 = p1 += 100 ;
			p2 -= 100 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		} else 
		{
			break ;
		}
	}
	delay_ms ( 2000 ) ;
	while ( 1 ) 
	{
		if ( p1 > 500 ) 
		{
			p3 = p1 -= 100 ;
			p2 += 100 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		} else 
		{
			break ;
		}
	}
	delay_ms ( 200 ) ;
	p1 = p2 = p3 = 0 ;
	CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
	CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
	CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
}
void move2 ( ) 			// piao yi 
{
	p1 = p2 = p3 = 0 ;
	CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
	CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
	CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
	while ( 1 ) 
	{
		delay_ms ( 30 ) ;
		if ( p3 < 3500 ) 
		{
			p1 -= 300 ;
			p2 = 0 ;
			p3 += 350 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		}
		if ( pos_y > - 1500 )       // y - 2150 
		{
			break ;
		}
	}
	while ( 1 ) 
	{
		delay_ms ( 50 ) ;
		if ( p2 > - 3000 ) 
		{
			p1 += 420 ;
			p2 -= 350 ;
			p3 = 3500 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		} else 
		{
			break ;
		}
	}
	while ( 1 ) 
	{
		delay_us ( 50000 ) ;
		if ( zangle < 50 ) 
		{
			p3 = 4000 ;
			p2 = - 4000 ;
			p1 = 1200 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		} else if ( zangle > 50 && zangle < 80 ) 
		{
			p1 = 2800 ;
			p3 = 3000 ;
			p2 = - 3000 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		} else if ( zangle > 80 && zangle < 100 ) 
		{
			p1 = 2500 ;
			p3 = 2500 ;
			p2 = - 2500 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		}
		if ( zangle > 100 ) 
		{
			p1 = p2 = p3 = 0 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
			break ;
		}
	}
	p1 = p2 = p3 = 0 ;
	CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
	CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
	CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
}
void fly2 ( ) 
{
	p1 = p2 = p3 = 0 ;
	CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
	CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
	CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
	while ( 1 )  		// y + + 
	{
		if ( p1 < 5000 ) 
		{
			delay_ms ( 50 ) ;
			p1 += 400 ;
			p3 -= 500 ;
			p2 = 0 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
		}
		if ( pos_y < - 4600 ) 
		{
			p1 = 5000 ;
			p3 = - p1 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
			break ;
		}
		if ( pos_x > - 6550 ) 
		{
			p1 -= 200 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
		} else if ( pos_x < - 6650 ) 
		{
			p1 += 200 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
		}
	}
	while ( 1 ) 
	{
		if ( p1 > 3000 ) 
		{
			delay_ms ( 50 ) ;
			p1 -= 330 ;
			p3 += 400 ;
			p2 = 0 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
		}
		if ( pos_y < - 5100 ) 
		{
			break ;
		}
	}
	while ( 1 ) 
	{
		if ( p2 < 3000 ) 
		{
			delay_ms ( 50 ) ;
			p2 += 300 ;
			p3 += 600 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
		} else 
		{
			break ;
		}
	}
	while ( 1 ) 
	{
		if ( zangle < 60 ) 
		{
			p1 = 2800 ;
			p2 = p3 = 3000 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
		} else if ( zangle > 60 && zangle < 75 ) 
		{
			p1 = p2 = p3 = 2000 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
		} else if ( zangle > 75 && zangle < 80 ) 
		{
			p1 = p2 = p3 = 1000 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
		}
		if ( zangle > 80 ) 
		{
			p1 = p2 = p3 = 0 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
			break ;
		}
	}
	p1 = p2 = p3 = 0 ;
	CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
	CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
	CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
}
void move3 ( ) 
{
	p1 = p2 = p3 = 0 ;
	CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
	CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
	CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
	while ( 1 ) 
	{
		delay_ms ( 50 ) ;
		if ( p2 < 3000 ) 
		{
			p1 -= 300 ;
			p2 += 300 ;
			p3 = 0 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
		}
		if ( pos_y > - 1300 ) 
		{
			delay_ms ( 50 ) ;
			p1 = - 3000 ;
			p2 -= 1000 ;
			p3 = 1000 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
			delay_ms ( 70 ) ;
			p1 = - 3000 ;
			p2 /= 2 ;
			p3 = 2000 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
			delay_ms ( 50 ) ;
			p1 = - 3000 ;
			p2 = 0 ;
			p3 = 3000 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
			break ;
		}
	}
	while ( 1 ) 
	{
		delay_ms ( 50 ) ;
		if ( p3 < 2900 ) 
		{
			p3 = 3000 ;
			p1 = - p3 ;
			p2 = 0 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
		}
		if ( pos_x < - 4750 ) 
		{
			delay_ms ( 50 ) ;
			p1 += 1000 ;
			p2 -= 1000 ;
			p3 = 3000 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
			delay_ms ( 70 ) ;
			p1 /= 2 ;
			p2 -= 1000 ;
			p3 = 3000 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
			delay_ms ( 50 ) ;
			p1 = 0 ;
			p2 = - 3000 ;
			p3 = 3000 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
			break ;
		}
	}
	while ( 1 ) 
	{
		delay_ms ( 50 ) ;
		if ( p3 < 2900 ) 
		{
			p1 = 0 ;
			p2 = - 3000 ;
			p3 = 3000 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
		}
		if ( pos_y < - 1350 ) 
		{
			break ;
		}
	}
	while ( 1 ) 
	{
		delay_ms ( 50 ) ;
		if ( p3 > 500 ) 
		{
			p3 -= 400 ;
			p2 = - p3 ;
			p1 = 0 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
		} else 
		{
			delay_ms ( 50 ) ;
			p1 = p2 = p3 = 0 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
			break ;
		}
	}
}
int main( ) 
{ 
	{
		//    u8 len ;	
		//	  u16 times=0; 
		//		u8 buffer[20] = "����һ�����⣿";
		HAL_Init();
		//��ʼ��HAL��   
		Stm32_Clock_Init(336,25,2,8);
		//����ʱ��,180Mhz  168Mhz
		delay_init(180);
		//��ʼ����ʱ����
		LED_Init();
		//��ʼ��LED 
		KEY_Init();
		//��ʼ������
		uart_init(115200);
		uart3_init(115200);
		//��ʼ��USART
		SDRAM_Init();
		//SDRAM��ʼ��
		LCD_Init();
		//LCD��ʼ��
		EXTI_Init();
		//��ʼ������
		POINT_COLOR=RED;
		CAN1_Mode_Init(CAN_SJW_1TQ,CAN_BS2_4TQ,CAN_BS1_9TQ,3,CAN_MODE_NORMAL);
		//CAN��ʼ��,������1Mbps ����ͨģʽ ��֪�����в� 	
		delay_ms(500);
		//�տ�ʼҪ���㹻����ʱ��ȷ���������Ѿ���ʼ����� 
		CAN_RoboModule_DRV_Reset(0,0);
		//��0���������������и�λ 
		delay_ms(500);
		//�տ�ʼҪ���㹻����ʱ��ȷ���������Ѿ���ʼ����� 
		CAN_RoboModule_DRV_Reset(0,0);
		//��0���������������и�λ 						
		delay_ms(500);
		//���͸�λָ������ʱ����Ҫ�У��ȴ��������ٴγ�ʼ�����
		//					printf("reset success!rn");				
		CAN_RoboModule_DRV_Config(0,1,100,0);
		//1������������Ϊ100ms����һ������
		delay_us(200);
		//�˴���ʱΪ�˲��ô�������ʱ��4����һ��
		CAN_RoboModule_DRV_Config(0,2,100,0);
		//2������������Ϊ100ms����һ������
		delay_us(200);
		//�˴���ʱΪ�˲��ô�������ʱ��4����һ��   
		CAN_RoboModule_DRV_Config(0,3,100,0);
		//3������������Ϊ100ms����һ������   
		delay_us(200);
		//�˴���ʱΪ�˲��ô�������ʱ��4����һ��  
		CAN_RoboModule_DRV_Config(0,4,100,0);
		//3������������Ϊ100ms����һ������
		CAN_RoboModule_DRV_Mode_Choice(0,0,Velocity_Mode);
		//0������������� �������ٶ�ģʽ
		delay_ms(500);
		//����ģʽѡ��ָ���Ҫ�ȴ�����������ģʽ������������ʱҲ������ȥ����
		p1 = 0;
		p2 = 0;
		p3 = 0;
		CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
		CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
		CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		/*
						(GPIOA,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6,GPIO_PIN_RESET);
						//		LED0  = 1;
						HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);	
						HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_RESET);	
						HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET);	
						HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);	
						HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_RESET);	
						HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_RESET);	
						HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);	
						HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);	
						*/
	}
	DIS( ) ;
	delay_ms ( 2500 ) ;
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
	delay_ms ( 8000 ) ;
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
	while ( 1 ) 
	{
		DIS ( ) ;
		if( ( 0 == HAL_GPIO_ReadPin( GPIOE,GPIO_PIN_5 ) ) ) 
		{
			p1 = p2 = p3 = 0 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
			start ( ) ;
			kszx ( 90 ) ;
			jzzx ( 90 ) ;
			ksdw ( 90 , - 4300 , - 2130 , 90 ) ;
			dw ( 90 , - 4300 , - 2130 , 90 ) ;
			kszx ( 76 ) ;
			jzzx ( 74 ) ;
			delay_ms ( 50 ) ;
			p1 = p2 = p3 = 0 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
			break ;
		}
	}
	while ( 1 ) 
	{
		DIS ( ) ;
		if ( ( 0 == HAL_GPIO_ReadPin( GPIOE,GPIO_PIN_5 ) ) ) 
		{
			lg ( ) ;
			delay_ms ( 300 ) ;
			jzzx ( 74 ) ;
			lt ( ) ;
			delay_ms ( 300 ) ;
			lb ( ) ;
			p1 = p2 = p3 = 0 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
			kszx ( 0 ) ;
			jzzx ( 0 ) ;
			jzdw ( 0 , - 4300 , - 2200 , 0 ) ;
			jzzx ( 0 ) ;
			break ;
		}
	}
	while ( 1 ) 
	{
		if ( ( 0 == HAL_GPIO_ReadPin( GPIOE,GPIO_PIN_5 ) ) ) 
		{
			lg ( ) ;
			delay_ms ( 2000 ) ;
			move3 ( ) ;
			kszx ( 90 ) ;
			jzzx ( 90 ) ;
			ksdw ( 90 , - 6300 , - 2300 , 81 ) ;
			dw ( 90 , - 6300 , - 2300 , 81 ) ;
			kszx ( 79 ) ;
			jzzx ( 79 ) ;
			delay_ms ( 800 ) ;
			lt ( ) ;
			delay_ms ( 300 ) ;
			lb ( ) ;
			kszx ( 0 ) ;
			jzzx ( 0 ) ;
			delay_ms ( 100 ) ;
			jzdw ( 0 , - 6600 , - 2200 , 0 ) ;
			jzzx ( 0 ) ;
			p1 = p2 = p3 = 0 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
			break ;
		}
	}
	while ( 1 ) 
	{
		if ( ( 0 == HAL_GPIO_ReadPin( GPIOE,GPIO_PIN_5 ) ) ) 
		{
			lg ( ) ;
			delay_ms ( 1000 ) ;
			kszx ( 90 ) ;
			jzzx ( 90 ) ;
			dw ( 90 , - 6300 , - 2300 , 81 ) ;
			kszx ( 79 ) ;
			jzzx ( 79 ) ;
			delay_ms ( 1000 ) ;
			lt ( ) ;
			delay_ms ( 500 ) ;
			lb ( ) ;
			kszx ( 0 ) ;
			jzzx ( 0 ) ;
			delay_ms ( 100 ) ;
			jzdw ( 0 , - 6600 , - 2200 , 0 ) ;
			jzzx ( 0 ) ;
			p1 = p2 = p3 = 0 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
		}
		if ( ( 0 == HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2) ) ) 
		{
			break ;
		}
	}
	while ( 1 ) 
	{
		DIS ( ) ;
		delay_ms ( 30 ) ;
		if ( ( 0 == HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2) ) ) 
		{
			rg ( ) ;
			jzzx ( 0 ) ;
			delay_ms ( 500 ) ;
			fly2 ( ) ;
			kszx ( 90 ) ;
			jzzx ( 90 ) ;
			dw ( 90 , - 6400 , - 6500 , 90 ) ;
			flydw ( 90 , - 6400 , - 6500 , 90 ) ;
			jzzx ( 90 ) ;
			delay_ms ( 1500 ) ;
			rt ( ) ;
			jzzx ( 90 ) ;
			ksdw ( 90 , - 6400 , - 6500 , 90 ) ;
			dw ( 90 , - 6400 , - 6500 , 90 ) ;
			back ( ) ;
			kszx ( 0 ) ;
			jzzx ( 0 ) ;
			jzdw ( 0 , - 6600 , - 2200 , 0 ) ;
			jzzx ( 0 ) ;
			p1 = p2 = p3 = 0 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
		}
		if ( flycount == 5 ) 
		{
			break ;
		}
	}
	while ( 1 ) 
	{
		if ( ( 0 == HAL_GPIO_ReadPin( GPIOE,GPIO_PIN_5 ) ) ) 
		{
			lg ( ) ;
			delay_ms ( 500 ) ;
			kszx ( 90 ) ;
			jzzx ( 90 ) ;
			dw ( 90 , - 6300 , - 2450 , 81 ) ;
			jzzx ( 79 ) ;
			delay_ms ( 1000 ) ;
			lt ( ) ;
			delay_ms ( 500 ) ;
			lb ( ) ;
			p1 = p2 = p3 = 0 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3) ;
		}
	}
}