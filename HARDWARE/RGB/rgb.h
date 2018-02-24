#ifndef _RGB_H
#define _RGB_H

#include "my_include.h"


//PB10 -- 蓝色 -- 常亮   (4个灯的信号并在一起)
//PB0  -- 黄色 -- 流动	(4个灯的信号并在一起)
#define  Car_LED_Blue   PBout(10)   //高电平触发
#define  Car_LED_Yellow PBout(0)    //高电平触发







void rgb_init(void);










#endif

