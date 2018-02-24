#include "exti.h"


 

 
//PA6--TIM3-CH1
//PA7--TIM3-CH2
//PE5--TIM9-CH1
//PE6--TIM9-CH2	  

//外部中断初始化程序
void EXTIX_Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOF, &GPIO_InitStructure);//???GPIOE2,3,4

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource4);//PA6 连接到中断线6
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource5);//PA7 连接到中断线7

	/* 配置EXTI_Line567 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line5 ;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
	EXTI_Init(&EXTI_InitStructure);//配置

	EXTI_InitStructure.EXTI_Line = EXTI_Line4 ;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
	EXTI_Init(&EXTI_InitStructure);//配置

	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;//外部中断0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置	


	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//外部中断0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置	   
}


//PA6--TIM3-CH1
//PA7--TIM3-CH2
//PE5--TIM9-CH1
//PE6--TIM9-CH2
 
u8 g_flag_butt_stop;
u8 g_flag_butt_start;

void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4) == SET)
	{
		if(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_4)==0)  //停止按键
		{	
			MotoStop(1);  
			g_flag_MOTORstart = 0; //电机启动信号
			g_flag_butt_stop = 1;
			g_flag_butt_start = 0;
			
		}
		EXTI_ClearITPendingBit(EXTI_Line4); //清除LINE5上的中断标志位 		
	}	
}



//外部中断0服务程序
void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line5) == SET)	 
	{
		if(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_5)==0)  //启动按键
		{
			MotoStart(1);
//			g_flag_MOTORstart = 1; //电机启动信号
			g_flag_butt_start = 1;
			g_flag_butt_stop  = 0;
			g_xunzheng_ok = 0;
		}
		
		EXTI_ClearITPendingBit(EXTI_Line5); //清除LINE5上的中断标志位 
	}	

//	if(EXTI_GetITStatus(EXTI_Line6) == SET)	 
//	{
//		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==0)
//		{
//			g_val1++;
//		}
//		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6)==0)
//		{
//			g_val4++;
//		}
//		
//		 
//		
//		EXTI_ClearITPendingBit(EXTI_Line6); //清除LINE5上的中断标志位 
//	}	
//	
//	if(EXTI_GetITStatus(EXTI_Line7) == SET)	 
//	{
//		 g_val2++;
//		
//		EXTI_ClearITPendingBit(EXTI_Line7); //清除LINE5上的中断标志位 
//	}	
	
	 
}	












