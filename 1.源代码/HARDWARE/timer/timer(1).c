/*-------------------------------------------------------------------------------
文件名称：timer.c
文件描述：通用定时器配置        
备    注：无
---------------------------------------------------------------------------------*/
#include "timer.h"
#include "usart.h"
#include "delay.h"
//#include "GPIO_COM.H"
///#include "PWM_COM.H"
u32 UltrasonicWave_Distance=0;  
// extern int auto_flag;
/*-------------------------------------------------------------------------------
程序名称：TIM2_Base_Init
程序描述：定时器TIM2通用定时功能
输入参数：定时器重装值
返回参数：无
备    注：无
---------------------------------------------------------------------------------*/

void TIM3_Base_Init(u16 Count)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	  NVIC_InitTypeDef NVIC_InitStructure;
	
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//使能定时器时钟
	  TIM_TimeBaseStructure.TIM_Period=Count-1;//设置初值
	  TIM_TimeBaseStructure.TIM_Prescaler=7200-1;//设置预分频系数
	  TIM_TimeBaseStructure.TIM_ClockDivision=0;
	  TIM_TimeBaseStructure.TIM_CounterMode= TIM_CounterMode_Up;//向上计数模式
	  TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//把上述值写进对应寄存器中
	
	  TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//使能指定TIM中断
	  
	  NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级0级
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;//子优先级2级；
	  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	  NVIC_Init(&NVIC_InitStructure);
	
	  TIM_Cmd(TIM3,ENABLE);//使能TIM3外设

}



TIM_ICInitTypeDef  TIM3_ICInitStructure;

void TIM3_Cap_Init(u16 arr,u16 psc)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能TIM2时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //使能GPIOA时钟

	     	
	
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;  //PA0 清除之前设置  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //PA0 输入  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 // 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化
 
	
	
	
	//初始化定时器2 TIM2	 
	TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
  
	//初始化TIM2输入捕获参数
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	选择输入端 IC1映射到TI1上
  	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
  	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  	TIM3_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
  	TIM_ICInit(TIM3, &TIM3_ICInitStructure);
	
	//中断分组初始化
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	
	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_CC1,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
	
  TIM_Cmd(TIM3,ENABLE ); 	//使能定时器2
 
}




void Timerx_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM2, //TIM2
		TIM_IT_Update  |  //TIM 中断源
		TIM_IT_Trigger,   //TIM 触发中断源 
		ENABLE  //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM2, ENABLE);  //使能TIMx外设
							 
}

//void TIM2_IRQHandler(void)   //TIM2中断
//{
//	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
//		{
//		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
//		}
//}


u32 time50ms=0;		//50ms  时间片
 u8 disFlag=0;		//读取温湿度标志  定时器中处理
u8 qiFlag=0;
unsigned int qixing_time=0;
void TIM2_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
			time50ms++;
			if(time50ms % 2 == 0)//定时时间到
			{
				disFlag = 1;
			}	
if(qiFlag==1)	
{
 qixing_time++;
	
}
		}
		
}






u8  TIM3CH1_CAPTURE_STA=0;	//输入捕获状态		    				
u16	TIM3CH1_CAPTURE_VAL;	  //输入捕获值
 

//定时器5中断服务程序	 
void TIM3_IRQHandler(void)
{ 

 	if((TIM3CH1_CAPTURE_STA&0X80)==0)//还未成功捕获	
	{	  
		if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
		 
		{	    
			if(TIM3CH1_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM3CH1_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
				{
					TIM3CH1_CAPTURE_STA|=0X80;//标记成功捕获了一次
					TIM3CH1_CAPTURE_VAL=0XFFFF;
				}else TIM3CH1_CAPTURE_STA++;
			}	 
		}
	if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)//捕获1发生捕获事件
		{ 	
			
			if(TIM3CH1_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
			{	  			
				TIM3CH1_CAPTURE_STA|=0X80;		//标记成功捕获到一次高电平脉宽（必须是下降沿和上降沿都捕获到之后）
				TIM3CH1_CAPTURE_VAL=TIM_GetCapture1(TIM3);//获取当前的捕获值
		    TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
			}else  								//还未开始,第一次捕获上升沿
			{
				TIM3CH1_CAPTURE_STA=0;			//清除标志位
				TIM3CH1_CAPTURE_VAL=0;      //清除捕获值
	 				TIM_SetCounter(TIM3,0);	   //清除定时器计数器
				TIM3CH1_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
					
		   	TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
			}		    
		}	
		
		}			
		
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC1|TIM_IT_Update); //清除中断标志位
	
}



void UltrasonicWave_StartMeasure(void)
{
  GPIO_SetBits(GPIOB,GPIO_Pin_5); 		  //送>10US的高电平RIG_PORT,TRIG_PIN这两个在define中有?
  delay_us(15);		                      //延时20US
  GPIO_ResetBits(GPIOB,GPIO_Pin_5);
	
}

float barrier_check(void)
{    
	  
    UltrasonicWave_StartMeasure();                //开始测距，发送一个>10us的脉冲，然后测量返回的高电平时间
				delay_ms(10);
				if(TIM3CH1_CAPTURE_STA&0X80)//成功捕获到了一次高电平
				{
					
						UltrasonicWave_Distance=TIM3CH1_CAPTURE_STA&0X3F;
						UltrasonicWave_Distance*=65536;					//溢出时间总和
						UltrasonicWave_Distance+=TIM3CH1_CAPTURE_VAL;		//得到总的高电平时间
						UltrasonicWave_Distance=UltrasonicWave_Distance*170/10000;//得出距离
						TIM3CH1_CAPTURE_STA=0;			//开启下一次捕获
					
				}
      return UltrasonicWave_Distance;
  
    
			}			
 void bi_zhang(void)
 {
	// u32 l,r;
	
	 barrier_check();
	// printf("测到的距离值为：%d\n\r",UltrasonicWave_Distance);
}
 
 
			
			
			
			
			
