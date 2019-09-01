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
#include "pstwo.h"
extern float pos_x,pos_y,zangle,xangle,yangle,w_z;
// ʵ������λ�ú�Ŀ������λ��
int res = 0;
short p1 = 0 , 
			p2 = 0 , 
			p3 = 0 ;
int flage_mode ;
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
void DIS( ) 
{
	//--------------------------������ʾ------------------------------------
	LCD_ShowString(20,200,200,16,16,"pos_x:         ");
	if(pos_x<0) 
	{
		pos_x=-pos_x;
		LCD_ShowString(80,200,200,16,16,"-");
		LCD_ShowxNum(90,200,pos_x,6,16,0x80);
	} else		
				LCD_ShowxNum(90,200,pos_x,6,16,0x80);
	//----------------------------����y������-------------------------------------------
	LCD_ShowString(20,220,200,16,16,"pos_y:         ");
	if(pos_y<0) 
	{
		pos_y=-pos_y;
		LCD_ShowString(80,220,200,16,16,"-");
		LCD_ShowxNum(90,220,pos_y,6,16,0x80);
	} else 
				LCD_ShowxNum(90,220,pos_y,6,16,0x80);
	//-------------------------------���̺����----------------------------------------
	LCD_ShowString(20,380,200,16,16,"zangle:         ");
	if(zangle<0) 
	{
		zangle=-zangle;
		LCD_ShowString(80,380,200,16,16,"-");
		LCD_ShowxNum(90,380,zangle,6,16,0x80);
	} else 
				LCD_ShowxNum(90,380,zangle,6,16,0x80);
	LCD_ShowxNum(90,150,flage_mode,6,16,0x80);
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
void start ( ) 
{
	p1 = p2 = p3 = 0 ;
	CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
	CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
	CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
	kszx ( 0 ) ;
	jzzx ( 0 ) ;
	while ( 1 ) 
	{
		if ( p3 < 2000 ) 
		{
			delay_ms ( 50 ) ;
			p3 += 400 ;
			p1 = - p3 ;
			p2 = 0 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		}
		if ( pos_y > 1650 ) 
		{
			break ;
		}
	}
	while ( 1 ) 
	{
		delay_ms ( 50 ) ;
		p1 /= 2 ;
		p2 /= 2 ;
		p3 /= 2 ;
		CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
		CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
		CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		delay_ms ( 50 ) ;
		p1 /= 2 ;
		p2 /= 2 ;
		p3 /= 2 ;
		CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
		CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
		CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		delay_ms ( 50 ) ;
		p1 = p2 = p3 = 0 ;
		CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
		CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
		CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		break ;
	}
}
void totz1 ( ) 
{
	p1 = p2 = p3 = 0 ;
	CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
	CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
	CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
	while ( 1 ) 
	{
		delay_ms ( 50 ) ;
		if ( p1 < 5000 ) 
		{
			p1 += 400 ;
			p3 -= 400 ;
			p2 = 0 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		}
		if ( pos_y < - 5500 ) 
		{
			p1 = 3000 ;
			p3 = - 3000 ;
			p2 = 0 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
			break ;
		}
		if ( zangle > 0 ) 
		{
			p1 -= 200 ;
			p3 += 200 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		} else if ( zangle < 0 ) 
		{
			p1 += 200 ;
			p3 -= 200 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		}
		if ( zangle < 2 && zangle > - 2 ) 
		{
			p3 = - p1 ;
			p2 = 0 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		}
	}
	p1 /= 2 ;
	p2 /= 2 ;
	p3 /= 2 ;
	CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
	CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
	CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
	p1 = p2 = p3 = 0 ;
	CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
	CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
	CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
	delay_ms ( 200 ) ;
	kszx ( - 90 ) ;
	jzzx ( - 90 ) ;
}
void totz2 ( ) 
{
	p1 = p2 = p3 = 0 ;
	CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
	CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
	CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
	while ( 1 ) 
	{
		delay_ms ( 50 ) ;
		if ( p1 < 5000 ) 
		{
			p1 += 400 ;
			p3 -= 400 ;
			p2 = 0 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		}
		if ( pos_y < - 6800 ) 
		{
			p1 = 4000 ;
			p3 = - 4000 ;
			p2 = 0 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
			break ;
		}
		if ( zangle > 0 ) 
		{
			p1 -= 200 ;
			p3 += 200 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		} else if ( zangle < 0 ) 
		{
			p1 += 200 ;
			p3 -= 200 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		}
		if ( zangle < 2 && zangle > - 2 ) 
		{
			p3 = - p1 ;
			p2 = 0 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		}
	}
	delay_ms ( 50 ) ;
	p1 /= 2 ;
	p2 /= 2 ;
	p3 /= 2 ;
	CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
	CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
	CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
	delay_ms ( 100 ) ;
	p1 /= 2 ;
	p2 /= 2 ;
	p3 /= 2 ;
	CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
	CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
	CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
	delay_ms ( 50 ) ;
	p1 = p2 = p3 = 0 ;
	CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
	CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
	CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
	kszx ( - 90 ) ;
	jzzx ( - 90 ) ;
}
void back ( ) 
{
	p1 = p2 = p3 = 0 ;
	CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
	CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
	CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
	kszx ( - 90 ) ;
	jzzx ( - 90 ) ;
	while ( 1 ) 
	{
		delay_ms ( 50 ) ;
		if ( p2 > - 3900 ) 
		{
			p2 -= 400 ;
			p3 = p1 += 200 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		}
		if ( pos_y > 200 ) 
		{
			break ;
		}
		if ( zangle < - 90 ) 
		{
			p2 -= 200 ;
			p3 = p1 += 100 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		} else if ( zangle > - 90 ) 
		{
			p2 += 200 ;
			p3 = p1 -= 100 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		}
		if ( zangle < - 91 && zangle > - 89 ) 
		{
			p3 = p1 = ( p2 / 2 ) ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		}
	}
	delay_ms ( 50 ) ;
	p1 /= 2 ;
	p2 /= 2 ;
	p3 /= 2 ;
	CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
	CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
	CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
	delay_ms ( 100 ) ;
	p1 /= 2 ;
	p2 /= 2 ;
	p3 /= 2 ;
	CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
	CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
	CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
	delay_ms ( 50 ) ;
	p1 = p2 = p3 = 0 ;
	CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
	CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
	CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
}
void up ( ) 
{
	while ( 1 ) 
	{
		flage_mode = PS2_DataKey ( ) ;
		delay_ms ( 50 ) ;
		if ( p2 < 900 ) 
		{
			p2 += 500 ;
			p3 = p1 -= 250 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		}
		if ( flage_mode != 5 ) 
		{
			break ;
		}
	}
}
void down ( ) 
{
	while ( 1 ) 
	{
		flage_mode = PS2_DataKey ( ) ;
		delay_ms ( 50 ) ;
		if ( p2 > - 900 ) 
		{
			p2 -= 500 ;
			p3 = p1 += 250 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		}
		if ( flage_mode != 7 ) 
		{
			break ;
		}
	}
}
void left ( ) 
{
	while ( 1 ) 
	{
		flage_mode = PS2_DataKey ( ) ;
		delay_ms ( 50 ) ;
		if ( p3 < 700 ) 
		{
			p3 += 400 ;
			p1 -= 400 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		}
		if ( flage_mode != 8 ) 
		{
			break ;
		}
		if ( zangle > 0 ) 
		{
			p3 -= 200 ;
			p1 += 200 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		} else if ( zangle < 0 ) 
		{
			p3 += 200 ;
			p1 -= 200 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		}
		if ( zangle < 2 && zangle > - 2 ) 
		{
			p3 = - p1 ;
			p2 = 0 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		}
	}
}
void right ( ) 
{
	while ( 1 ) 
	{
		flage_mode = PS2_DataKey ( ) ;
		delay_ms ( 50 ) ;
		if ( p1 < 1000 ) 
		{
			p1 += 400 ;
			p3 -= 400 ;
			p2 = 0 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		}
		if ( flage_mode != 6 ) 
		{
			break ;
		}
		if ( zangle > 0 ) 
		{
			p1 -= 200 ;
			p3 += 200 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		} else if ( zangle < 0 ) 
		{
			p1 += 200 ;
			p3 -= 200 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		}
		if ( zangle < 2 && zangle > - 2 ) 
		{
			p3 = - p1 ;
			p2 = 0 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
		}
	}
}
void zx0 ( ) 
{
	kszx ( -5 ) ;
	jzzx ( 0 ) ;
}
void zx90 ( ) 
{
	kszx ( - 85 ) ;
	jzzx ( - 90 ) ;
}
void gup ( ) 
{
	delay_ms ( 50 ) ;
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET);
}
void gback ( ) 
{
	delay_ms ( 50 ) ;
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET);
}
void lup ( ) 
{
	delay_ms ( 50 ) ;
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);
}
void lback ( ) 
{
	delay_ms ( 50 ) ;
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);
}
void rup ( ) 
{
	delay_ms ( 50 ) ;
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
}
void rback ( ) 
{
	delay_ms ( 50 ) ;
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
}
void lrup ( ) 
{
	lup ( ) ;
	rup ( ) ;
}
int main(void) 
{ 
	{
		//    u8 len;	
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
		//    KEY_Init();                     //��ʼ������
		uart_init(115200);
		uart3_init(115200);
		//��ʼ��USART
		PS2_Init();
		//=====ps2�����˿ڳ�ʼ��
		PS2_SetInit();
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
		//0������������� �����뿪��ģʽ
		delay_ms(500);
		//����ģʽѡ��ָ���Ҫ�ȴ�����������ģʽ������������ʱҲ������ȥ����
		p1 = 0;
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6,GPIO_PIN_RESET);
		//		LED0  = 1;
	}
	lback ( ) ;
	while ( 1 ) 
	{
		flage_mode = PS2_DataKey() ;
		DIS ( ) ;
		switch ( flage_mode ) 
		{
			case 1 : 
							break ;
			case 2 : 	
							zx0 ( ) ;
			break ;
			case 3 : 
							zx90 ( ) ;
			break ;
			case 4 : 
							lrup ( ) ;
			break ;
			case 5 : 
							left ( ) ;
			break ;
			case 6 : 
							up ( ) ;
			break ;
			case 7 : 
							right ( ) ;
			break ;
			case 8 :  
							down ( ) ;
			break ;
			case 9 : 
							totz2 ( ) ;
			break ;
			case 10 : 					// R2 
			back ( ) ;
			break ;
			case 11 : 
							totz1 ( ) ;
			break ;
			case 12 : 					// R1 
			start ( ) ;
			break ;
			case 13 : 
							gup ( ) ;
			break ;
			case 14 : 
							rback ( ) ;
			break ;
			case 15 : 
							gback ( ) ;
			break ;
			case 16 : 
							lback ( ) ;
			break ;
			default : 
							p1 = p2 = p3 = 0 ;
			CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,p1);
			CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,p2);
			CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,p3);
			break ;
		}
	}
}