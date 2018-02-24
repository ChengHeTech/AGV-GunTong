#include "guntong.h"


//PC5 -- Run A  --高电平触发 -- 常态下拉
//PC4 -- 反转   --高电平触发
//GND -- Run B
void guntong_init(void) 
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	//

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4| GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			//输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//推挽
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;			//下拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);					//
	GPIO_ResetBits(GPIOC,GPIO_Pin_4| GPIO_Pin_5);
		
	
}





u8	sta_gunt=0;

u8  USART2_TX_BUF[100];
void u2_printf(char* fmt,...)  
{  
	u16 i,j;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART2_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART2_TX_BUF);//?????????
	for(j=0;j<i;j++)//??????
	{
	  while(USART_GetFlagStatus(USART6,USART_FLAG_TC)==RESET);  //???????? 
		USART_SendData(USART6,(uint8_t)USART2_TX_BUF[j]); 	 //???????6 
	}
}
void guntong1(void)
{
//	OS_ERR err;
	

	//if(sta_gunt==0)
//	{
//		stopAsk = 1;
//		while(stopAsk)
//		{
//			OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err);
//		}		//完全停住
//		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err); //延时2秒等待完全停住
//		
//		
//		Dbus_Data[0]=0;
//		
//		while( ! WriteStr(7,0,"ZZ"))   //0失败 1成功
//		{
//			OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err);
//		}		

//		GPIO_ResetBits(GPIOB,GPIO_Pin_5);//对应PCB板子电机3  FR方向
////		Motor_Zzhuan(3,400);										//滚筒正转
//		
//		Dbus_Data[0]=0;  //清到位信号
//		
////		while(g_ethernet_RXbuff[0]!='D'||g_ethernet_RXbuff[1]!='W')
//		while(!ComperStr(0,"DW"))
//		{
//			OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err);
//		}	
//		Dbus_Data[0]=0;
//		GPIO_SetBits(GPIOB,GPIO_Pin_5); //滚筒停止
//		
//		startAsk = 1;                                             //启动
//		while(startAsk)
//		{OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err);}

//		sta_gunt = 1;   
//		
//	}
//	else if(sta_gunt==1)     //第二圈过来
//	{
//		stopAsk = 1;
//		while(stopAsk)
//		{
//			OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err);
//		}		//完全停住
//		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);      //延时2秒等待完全停住

//		while( ! WriteStr(7,0,"FZ"))   //0失败 1成功
//		{
//			OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err);
//		}
//		
//		GPIO_ResetBits(GPIOB,GPIO_Pin_5);//对应PCB板子电机3  FR方向
////		Motor_Fzhuan(3,400); //滚筒

//		
//		while(1)   // 一对红外  (旧板子PE12-PE13) 
//		{
//			if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_12)==SET||GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_13)==SET)
//			{
//				OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err);
//				if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_12)==SET||GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_13)==SET)
//				{
//					OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err);
//					if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_12)==SET||GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_13)==SET)
//					{					     
//						GPIO_SetBits(GPIOB,GPIO_Pin_5); //滚筒停止					
//						
//						
//						while( ! WriteStr(7,0,"DW"))   //0失败 1成功
//						{
//							OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err);
//						}						
//											
//						startAsk = 1;                                             //启动
////						while(startAsk)
//						{OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err);}

//						sta_gunt =0;
//						break;			
//					}
//				}		
//			}
//			OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err);
//		}	
//	}
}
	












