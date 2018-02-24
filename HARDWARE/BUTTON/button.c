#include "button.h"

//PF4 -- 停止
//PF5 -- 启动
//PF6 -- 左_滚筒红外对射 	-- (原急停)

//PF14-- 前_红外避障 		-- (牵引杆开入2)   远
//PF10-- 后_红外避障 		-- (牵引杆开入1)	远

//PF9 -- 前红外---近
//PF8 -- 后红外--近

//PA6 -- 前防撞
//PA7 -- 后防撞

//PF7 -- 右_滚筒红外对射 	-- (备用开入1) 


//外部中断初始化程序
void button_input_init(void)//上拉输入,低电平触发
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	

	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4| GPIO_Pin_5| GPIO_Pin_6| GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9|
	GPIO_Pin_10| GPIO_Pin_14; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	

}


//#define  g_agv_stop 					PFin(4)
//#define  g_agv_start					PFin(5)

//#define  g_flag_IR_qian_yuan				PFin(14)
//#define  g_flag_IR_hou_yuan				PFin(10)

//#define  g_flag_fangzhuang_qian		PFin(9)
//#define  g_flag_fangzhuang_hou		PFin(8)

//#define  g_flag_guntong_zuo			PFin(6)
//#define  g_flag_guntong_you			PFin(7)


u8 AGV_input_24V_buff[16];

//AGV全局状态
u8 g_AGV_Car_STOP = 1;		//AGV默认停止	
//u8 g_AGV_Car_START;

void agv_24V_input(void)	//未做按键消抖,后面做
{
	if(!g_button_stop)						//g_jixieKEy_stop
	{
		AGV_input_24V_buff[0] = g_jixieKEy_stop= 1;
	}
	else
	{
		AGV_input_24V_buff[0] = g_jixieKEy_stop = 0;
	}
		
	if(!g_button_start)					//g_jixieKEY_start
	{
		AGV_input_24V_buff[1] = g_jixieKEY_start = 1;
	}
	else
	{
		AGV_input_24V_buff[1] = g_jixieKEY_start = 0;
	}		
	
	if(!g_flag_IR_qian_yuan)					//前远红外  	//g_IR_yuan_CAR_qian
	{
		AGV_input_24V_buff[2] = g_IR_yuan_CAR_qian = 1;
	}
	else
	{
		AGV_input_24V_buff[2] = g_IR_yuan_CAR_qian = 0;
	}	

	
	if(!g_flag_IR_hou_yuan)						//后远红外		//g_IR_yuan_CAR_hou
	{
		AGV_input_24V_buff[3] = g_IR_yuan_CAR_hou= 1;
	}
	else
	{
		AGV_input_24V_buff[3] = g_IR_yuan_CAR_hou = 0;
	}	
	
	if(!g_flag_fangzhuang_qian)			//g_jiexie_qian
	{
		AGV_input_24V_buff[4] = g_jiexie_qian = 1;
	}
	else
	{
		AGV_input_24V_buff[4] = g_jiexie_qian= 0;
	}
		
	if(!g_flag_fangzhuang_hou)			//g_jiexie_hou
	{
		AGV_input_24V_buff[5] = g_jiexie_hou =1;
	}
	else
	{
		AGV_input_24V_buff[5] = g_jiexie_hou = 0;
	}		
	
	if(!g_flag_guntong_zuo)				//g_IR_guntong_left
	{
		AGV_input_24V_buff[6] = g_IR_guntong_left = 1;
	}
	else
	{
		AGV_input_24V_buff[6] = g_IR_guntong_left = 0;
	}	
	
	if(!g_flag_guntong_you)				//g_IR_guntong_right
	{
		AGV_input_24V_buff[7] = g_IR_guntong_right = 1;
	}
	else
	{
		AGV_input_24V_buff[7] = g_IR_guntong_right = 0;
	}	
	
	
	if(!g_flag_IR_qian_jin)					//前近红外  	//g_IR_jin_CAR_qian
	{
		AGV_input_24V_buff[8] = g_IR_jin_CAR_qian = 1; 
	}
	else
	{
		AGV_input_24V_buff[8] = g_IR_jin_CAR_qian = 0;
	}	

	if(!g_flag_IR_hou_jin)					//后近红外  	//g_IR_jin_CAR_hou
	{
		AGV_input_24V_buff[9] = g_IR_jin_CAR_hou = 1; 
	}
	else
	{
		AGV_input_24V_buff[9] = g_IR_jin_CAR_hou = 0;
	}	






}








//外部中断初始化程序
//void button_input_init(void)//上拉输入,低电平触发
//{	
//	GPIO_InitTypeDef GPIO_InitStructure;
//	
//	EXTI_InitTypeDef   EXTI_InitStructure;
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
//		
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4| GPIO_Pin_5| GPIO_Pin_6| GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9|
//	GPIO_Pin_10| GPIO_Pin_14; 
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
//	GPIO_Init(GPIOF, &GPIO_InitStructure);
//	
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource4);	//PF4 连接到中断线4
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource5);	//PF5 连接到中断线5
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource6);	//PF6 连接到中断线6
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource7);	//PF7 连接到中断线7
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource8);	//PF8 连接到中断线8
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource9);	//PF9 连接到中断线9
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource10);	//PF10 连接到中断线10
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource14);	//PF14 连接到中断线14
//	
//	/* 配置EXTI_Line4 */
//	EXTI_InitStructure.EXTI_Line = EXTI_Line4 ;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
//	EXTI_Init(&EXTI_InitStructure);//配置
//	
//	/* 配置EXTI_Line5 */
//	EXTI_InitStructure.EXTI_Line = EXTI_Line5 ;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
//	EXTI_Init(&EXTI_InitStructure);//配置

//	/* 配置EXTI_Line6 */
//	EXTI_InitStructure.EXTI_Line = EXTI_Line6 ;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
//	EXTI_Init(&EXTI_InitStructure);//配置
//	
//	/* 配置EXTI_Line7 */
//	EXTI_InitStructure.EXTI_Line = EXTI_Line7 ;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
//	EXTI_Init(&EXTI_InitStructure);//配置
//	
//	/* 配置EXTI_Line8 */
//	EXTI_InitStructure.EXTI_Line = EXTI_Line8 ;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
//	EXTI_Init(&EXTI_InitStructure);//配置
//	
//	/* 配置EXTI_Line9 */
//	EXTI_InitStructure.EXTI_Line = EXTI_Line9 ;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
//	EXTI_Init(&EXTI_InitStructure);//配置
//		/* 配置EXTI_Line10 */
//	EXTI_InitStructure.EXTI_Line = EXTI_Line10 ;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
//	EXTI_Init(&EXTI_InitStructure);//配置
//	
//	/* 配置EXTI_Line14 */
//	EXTI_InitStructure.EXTI_Line = EXTI_Line14 ;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
//	EXTI_Init(&EXTI_InitStructure);//配置
//}



// 
//u8 g_flag_butt_stop;
//u8 g_flag_butt_start;

//void EXTI4_IRQHandler(void)
//{
//	if(EXTI_GetITStatus(EXTI_Line4) == SET)
//	{
//		if(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_4)==0)  //停止按键
//		{	
////			MotoStop(1);  
////			g_flag_MOTORstart = 0; //电机启动信号
////			g_flag_butt_stop = 1;
////			g_flag_butt_start = 0;
//			
//			g_agv_stop = 1;
//		}
//		EXTI_ClearITPendingBit(EXTI_Line4); //清除LINE4上的中断标志位 		
//	}	
//}



////外部中断服务程序
//void EXTI9_5_IRQHandler(void)
//{
//	if(EXTI_GetITStatus(EXTI_Line5) == SET)	 
//	{
//		if(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_5)==0)  //启动按键
//		{
//			g_agv_stop = 0;
//		}
//		
//		EXTI_ClearITPendingBit(EXTI_Line5); //清除LINE5上的中断标志位 
//	}	

//	if(EXTI_GetITStatus(EXTI_Line6) == SET)	 			//左_滚筒红外对射 	-- (原急停)
//	{
//		if(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_6)==0)
//		{
//			
//		}
//		EXTI_ClearITPendingBit(EXTI_Line6); //清除LINE6上的中断标志位 
//	}	
//	
//	if(EXTI_GetITStatus(EXTI_Line7) == SET)	 
//	{
//		if(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_7)==0)	//右_滚筒红外对射 	-- (备用开入1) 
//		{
//			
//		}
//		
//		EXTI_ClearITPendingBit(EXTI_Line7); //清除LINE7上的中断标志位 
//	}	
//	if(EXTI_GetITStatus(EXTI_Line8) == SET)	 
//	{
//		if(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_8)==0)		//后防撞
//		{
//			
//		}
//		
//		EXTI_ClearITPendingBit(EXTI_Line8); //清除LINE8上的中断标志位 
//	}	
//	if(EXTI_GetITStatus(EXTI_Line9) == SET)	 
//	{
//		if(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_9)==0)		//前防撞
//		{
//			
//		}
//		
//		EXTI_ClearITPendingBit(EXTI_Line9); //清除LINE9上的中断标志位 
//	}	
//	
//	 
//}	

//void EXTI15_10_IRQHandler(void)
//{
//	if(EXTI_GetITStatus(EXTI_Line10) == SET)	 
//	{
//		if(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_10)==0)        //后_红外避障 		-- (牵引杆开入1)
//		{
//			
//		}
//		
//		EXTI_ClearITPendingBit(EXTI_Line10); //清除LINE10上的中断标志位 
//	}	

//	if(EXTI_GetITStatus(EXTI_Line14) == SET)	 
//	{
//		if(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_14)==0)        //前_红外避障 		-- (牵引杆开入2)
//		{
//			g_flag_IR_qian_yuan = 1;
//		}
//		
//		EXTI_ClearITPendingBit(EXTI_Line14); //清除LINE14上的中断标志位 
//	}	



//}





