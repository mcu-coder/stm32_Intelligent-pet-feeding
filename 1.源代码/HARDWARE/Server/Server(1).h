#ifndef __SERVER_H
#define __SERVER_H
#include "stm32f10x.h"                  // Device header

void TIM5_PWM_Init(u16 arr,u16 psc);
void SetJointAngle(float angle);
void TIM4_CH_Init(u16 Count); 	
#endif 
