#ifndef __LED_H
#define __LED_H
#include "sys.h"
#include "my_include.h"
	
//LED端口定义
#define LED1 PBout(8)  //TIM4CH3
#define LED2 PBout(9)
#define LED3 PEout(0)
#define LED4 PEout(1)






void LED_Init(void);//初始化	






#endif
