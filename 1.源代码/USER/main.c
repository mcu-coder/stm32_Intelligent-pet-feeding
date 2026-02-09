#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "dht11.h"
#include "oled.h"
#include "Server.h"
//#include "timer.h"
#include "exti.h"
#include "key.h"
#include "stdio.h"

unsigned short phasecw[4] ={0x0800,0x0400,0x0040,0x0020};// D-C-B-A   反转
unsigned short phaseccw[4]={0x0020,0x0040,0x0400,0x0800};// A-B-C-D   正转

//引脚初始化
void Moto_Init(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure;
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_10 | GPIO_Pin_11 ;//引脚按着INT1顺序接就行了
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(GPIOB,&GPIO_InitStructure);
	 GPIO_ResetBits(GPIOB,GPIO_Pin_5 | GPIO_Pin_6 |GPIO_Pin_10 |GPIO_Pin_11);
}

void MotoRcw(void)  //反转
{  
    int  i;  
  
    for(i=0;i<4;i++)  
    {  
        GPIO_Write(GPIOB,phasecw[i]);  
        delay_ms(3);  
    }  
}

void MotoRccw(void)  //正转
{  
    int i;  
    for(i=0;i<4;i++)  
    {  
        GPIO_Write(GPIOB,phaseccw[i]);  
        delay_ms(3);  
    }  
}

void MotorStop(void) //停止
{  
    GPIO_Write(GPIOB,0x0000);  
}

//控制电机正转还是反转某个角度
//direction方向，1为正转，0为反转
//angle角度，可为0-360具有实际意义
void Motor_Ctrl_Direction_Angle(int direction, int angle)
{
	u16 j;
	if(direction == 1)
	{
		for(j=0;j<64*angle/45;j++) 
		{
			MotoRccw();//正转
		}
		 MotorStop();//停止
  }
	else
	{
		for(j=0;j<64*angle/45;j++) 
		{
			MotoRcw();//反转
		}
		 MotorStop();//停止
	}
}



 int main(void)
 {  	    
	u8 temperature=0;  	    
	u8 humidity=0;
  extern  u8 foodnum;  //设置温度
  extern int auto_flag; 
	delay_init();	    	 //延时函数初始化	  
	uart_init(9600);	 	//串口初始化为9600
	LED_Init();		  		//初始化与LED连接的硬件接口
	TIM5_PWM_Init(9999,143); 
 //TIM4_CH_Init(200); 
	 //SystemInit();
	 
	DHT11_Init();
	KEY_Init();
	EXTIX_Init();
	OLED_Init();
	OLED_Clear();	
 	//Adc_Init();		  		//ADC初始化
	Moto_Init();	   			//电机
//	delay_init();	    	 //延时函数初始化	  
	
	 
  	
	
	OLED_ShowCHinese(0,0,0);	
	OLED_ShowCHinese(16,0,2);
	OLED_ShowString(32,0,":      C"); 
	
	OLED_ShowCHinese(0,2,1);	
	OLED_ShowCHinese(16,2,2);
	OLED_ShowString(32,2,":      %"); 
	
	OLED_ShowCHinese(0,4,3);	
	OLED_ShowCHinese(16,4,4);
	OLED_ShowString(32,4,":"); 
	OLED_ShowCHinese(84,4,5);	
	
//  OLED_ShowString(0,2,"NowTemp:  C"); 
//  OLED_ShowString(0,4,"Foodnum:  ");	 
 	SetJointAngle(90);
	while(1)
	{ 
		 DHT11_Read_Data(&temperature,&humidity);
 
	// SetJointAngle(90);
		
	 OLED_ShowInt(64,0,temperature);
   OLED_ShowInt(64,2,humidity);
	 OLED_ShowInt(64,4,foodnum);

		printf("温度: %d  湿度: %d  喂食次数: %d \r\n",temperature,humidity,foodnum);
		//printf("kcal:%d\r\n",temperature);
		delay_ms(1000);	

	}											    
}	
