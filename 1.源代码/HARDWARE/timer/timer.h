#ifndef __TIMER_H
#define __TIMER_H
#include "stm32f10x.h"

void Timerx_Init(u16 arr,u16 psc);
void TIM3_Base_Init(u16 Count); 
void TIM3_Cap_Init(u16 arr,u16 psc);
void UltrasonicWave_StartMeasure(void);                //开始测距，发送一个>10us的脉冲，然后测量返回的高电平时间
//void barrier_check(void);
float barrier_check(void);
#endif
/*----------------------德飞莱 技术论坛：www.doflye.net--------------------------*/
