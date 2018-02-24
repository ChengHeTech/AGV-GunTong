#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"





#define PWM_1 PDout(12)
#define PWM_2 PDout(13)
#define PWM_3 PDout(14)
#define PWM_4 PDout(15)


void TIM14_PWM_Init(u32 arr,u32 psc);
void TIM4_PWM_Init(u32 arr,u32 psc);
#endif
