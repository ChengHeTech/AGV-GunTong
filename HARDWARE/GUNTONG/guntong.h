#ifndef  _GUNTONG_H
#define _GUNTONG_H
#include "my_include.h"



#define guntong_start    PCout(5)   //滚筒启动 -- 高电平触发
#define guntong_fanzhuan PCout(4) 	//滚筒反转 -- 高电平触发




extern u16 StationData[16]; 
extern u8 stopflag,startflag;
//运动控制线程
extern u8  startAsk,stopAsk,zyAsk,yyAsk;
extern u16 DstSpeed;


void guntong_init(void);
void guntong1(void);












#endif







