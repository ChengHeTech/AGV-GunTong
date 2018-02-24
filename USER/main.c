#include "my_include.h"

//将这些优先级分配给了UCOSIII的5个系统内部任务
//优先级0：中断服务服务管理任务 OS_IntQTask()
//优先级1：时钟节拍任务 OS_TickTask()
//优先级2：定时任务 OS_TmrTask()
//优先级OS_CFG_PRIO_MAX-2：统计任务 OS_StatTask()
//优先级OS_CFG_PRIO_MAX-1：空闲任务 OS_IdleTask()

//任务优先级
#define START_TASK_PRIO		3
//任务堆栈大小	
#define START_STK_SIZE 		512
//任务控制块
OS_TCB StartTaskTCB;
//任务堆栈	
CPU_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *p_arg);                       //开始任务

//任务优先级
#define Transducer_TASK_PRIO		4
//任务堆栈大小	
#define Transducer_STK_SIZE 		512
//任务控制块
OS_TCB TransducerTaskTCB;
//任务堆栈	
CPU_STK Transducer_TASK_STK[Transducer_STK_SIZE];
void Transducer_task(void *p_arg);					//手自动转换任务


//任务优先级
#define Auto_TASK_PRIO		5
//任务堆栈大小	
#define Auto_STK_SIZE 		512
//任务控制块
OS_TCB AutoTaskTCB;
//任务堆栈	
CPU_STK Auto_TASK_STK[Auto_STK_SIZE];
//任务函数
void Auto_task(void *p_arg);						//自动模式任务

//任务优先级
#define Manual_TASK_PRIO		6
//任务堆栈大小	
#define Manual_STK_SIZE 		512
//任务控制块
OS_TCB ManualTaskTCB;
//任务堆栈	
CPU_STK Manual_TASK_STK[Manual_STK_SIZE];
//任务函数
void Manual_task(void *p_arg);						//手动模式任务

//任务优先级
#define FLOAT_TASK_PRIO		7
//任务堆栈大小
#define FLOAT_STK_SIZE		512
//任务控制块
OS_TCB	FloatTaskTCB;
//任务堆栈
__align(8) CPU_STK	FLOAT_TASK_STK[FLOAT_STK_SIZE];
//任务函数
void float_task(void *p_arg);						//浮点任务

//任务优先级
#define Screen_TASK_PRIO		8
//任务堆栈大小	
#define Screen_STK_SIZE 		512
//任务控制块
OS_TCB ScreenTaskTCB;
//任务堆栈	
CPU_STK Screen_TASK_STK[Screen_STK_SIZE];
//任务函数
void Screen_task(void *p_arg);						//屏幕任务

//任务优先级 
#define PID_TASK_PRIO		9
//任务堆栈大小	
#define PID_STK_SIZE 		512
//任务控制块
OS_TCB PIDTaskTCB;
//任务堆栈	
CPU_STK PID_TASK_STK[PID_STK_SIZE];
//任务函数
void PID_task(void *p_arg);							//PID任务


//任务优先级
#define Control_TASK_PRIO		10
//任务堆栈大小	
#define Control_STK_SIZE 		512
//任务控制块
OS_TCB ControlTaskTCB;
//任务堆栈	
CPU_STK Control_TASK_STK[Control_STK_SIZE];
//任务函数
void Control_task(void *p_arg);						//控制任务

//任务优先级
#define WIFI_TASK_PRIO		11
//任务堆栈大小	
#define WIFI_STK_SIZE 		512
//任务控制块
OS_TCB WIFITaskTCB;
//任务堆栈	
CPU_STK WIFI_TASK_STK[WIFI_STK_SIZE];
//任务函数
void WIFI_task(void *p_arg);						//WIFI任务


//任务优先级
#define guntong_TASK_PRIO		12
//任务堆栈大小	
#define guntong_STK_SIZE 		512
//任务控制块
OS_TCB guntongTaskTCB;
//任务堆栈	
CPU_STK guntong_TASK_STK[guntong_STK_SIZE];
//任务函数
void guntong_task(void *p_arg);						//滚筒任务



//任务优先级
#define DCv_TASK_PRIO		13
//任务堆栈大小	
#define DCv_STK_SIZE 		128
//任务控制块
OS_TCB DCvTaskTCB;
//任务堆栈	
CPU_STK DCv_TASK_STK[DCv_STK_SIZE];
//任务函数
void DCv_task(void *p_arg);							//电压采集





int main(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	
	delay_init(168);  	//时钟初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断分组配置
	
	LED_Init();         //LED初始化
	beep_init();   		//PA3
	rgb_init();
	button_input_init();
	//motor_init();
	guntong_init();
	
	DMA_adc1_Init();    				//电位器
	DMA_Uart2_Init();					//以太网转串口
	DMA_Uart3_Init();					//锂电池通信
	DMA_Uart4_Init();					//触摸屏	
	
	ADC1_Configuration();				//电位器采集
	USART1_Configuration(9600); 		//串口1 PA9PA10 现在换成喇叭线
	USART2_Configuration(9600);			//以太网转串口的接口
	USART3_Configuration(9600);			//现在是锂电池电量检测的口 -- 232转TTL串口
	uart4_init(9600);					//触摸屏485
	//USART6_Configuration(115200);		
	

	//DMA_Uart6_Init();
	CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,CAN_Mode_Normal);//CAN初始化环回模式,波特率500Kbps    
	CAN2_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,CAN_Mode_Normal);	
	NVIC_Configuration();

	TIM4_PWM_Init(21000-1,4-1);		//  1000,8   21000 000
	PID_Init();
	
	//UserConfigInit();				//用户参数初始化	

	
	

	OSInit(&err);		//初始化UCOSIII
	OS_CRITICAL_ENTER();//进入临界区
	//创建开始任务
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//任务控制块
								 (CPU_CHAR	* )"start task", 		//任务名字
                 (OS_TASK_PTR )start_task, 			//任务函数
                 (void		* )0,					//传递给任务函数的参数
                 (OS_PRIO	  )START_TASK_PRIO,     //任务优先级
                 (CPU_STK   * )&START_TASK_STK[0],	//任务堆栈基地址
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//任务堆栈深度限位
                 (CPU_STK_SIZE)START_STK_SIZE,		//任务堆栈大小
                 (OS_MSG_QTY  )0,					//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                 (OS_TICK	  )0,					//当使能时间片轮转时的时间片长度，为0时为默认长度，
                 (void   	* )0,					//用户补充的存储区
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
                 (OS_ERR 	* )&err);				//存放该函数错误时的返回值
	OS_CRITICAL_EXIT();	//退出临界区	 
	OSStart(&err);  //开启UCOSIII								 
}

//开始任务函数
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//统计任务                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	 //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	
	OS_CRITICAL_ENTER();	//进入临界区
	
	//创建Transducer任务
	OSTaskCreate((OS_TCB 	* )&TransducerTaskTCB,			//4
				 (CPU_CHAR	* )"Transducer task", 		
                 (OS_TASK_PTR )Transducer_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )Transducer_TASK_PRIO,     
                 (CPU_STK   * )&Transducer_TASK_STK[0],	
                 (CPU_STK_SIZE)Transducer_STK_SIZE/10,	
                 (CPU_STK_SIZE)Transducer_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);								 
	
	//创建自动任务
	OSTaskCreate((OS_TCB 	* )&AutoTaskTCB,				//5
				 (CPU_CHAR	* )"Auto task", 		
                 (OS_TASK_PTR )Auto_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )Auto_TASK_PRIO,     	
                 (CPU_STK   * )&Auto_TASK_STK[0],	
                 (CPU_STK_SIZE)Auto_STK_SIZE/10,	
                 (CPU_STK_SIZE)Auto_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);
				 
	//创建手动任务
	OSTaskCreate((OS_TCB 	* )&ManualTaskTCB,				//6
				 (CPU_CHAR	* )"Manual task", 		
                 (OS_TASK_PTR )Manual_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )Manual_TASK_PRIO,     	
                 (CPU_STK   * )&Manual_TASK_STK[0],	
                 (CPU_STK_SIZE)Manual_STK_SIZE/10,	
                 (CPU_STK_SIZE)Manual_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);
				 
	//创建浮点测试任务
	OSTaskCreate((OS_TCB 	* )&FloatTaskTCB,				//7
				 (CPU_CHAR	* )"float test task", 		
                 (OS_TASK_PTR )float_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )FLOAT_TASK_PRIO,     	
                 (CPU_STK   * )&FLOAT_TASK_STK[0],	
                 (CPU_STK_SIZE)FLOAT_STK_SIZE/10,	
                 (CPU_STK_SIZE)FLOAT_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);	

	//创建触摸屏任务
	OSTaskCreate((OS_TCB 	* )&ScreenTaskTCB,				//8
				 (CPU_CHAR	* )"Screen task", 		
                 (OS_TASK_PTR )Screen_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )Screen_TASK_PRIO,     	
                 (CPU_STK   * )&Screen_TASK_STK[0],	
                 (CPU_STK_SIZE)Screen_STK_SIZE/10,	
                 (CPU_STK_SIZE)Screen_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);	

	//PID
	OSTaskCreate((OS_TCB 	* )&PIDTaskTCB,					//9
				 (CPU_CHAR	* )"PID task", 		
                 (OS_TASK_PTR )PID_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )PID_TASK_PRIO,     	
                 (CPU_STK   * )&PID_TASK_STK[0],	
                 (CPU_STK_SIZE)PID_STK_SIZE/10,	
                 (CPU_STK_SIZE)PID_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);
	//控制
	OSTaskCreate((OS_TCB 	* )&ControlTaskTCB,				//10
				 (CPU_CHAR	* )"Control task", 		
                 (OS_TASK_PTR )Control_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )Control_TASK_PRIO,     	
                 (CPU_STK   * )&Control_TASK_STK[0],	
                 (CPU_STK_SIZE)Control_STK_SIZE/10,	
                 (CPU_STK_SIZE)Control_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);				 
	//WIFI
	OSTaskCreate((OS_TCB 	* )&WIFITaskTCB,				//11
				 (CPU_CHAR	* )"WIFI task", 		
                 (OS_TASK_PTR )WIFI_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )WIFI_TASK_PRIO,     	
                 (CPU_STK   * )&WIFI_TASK_STK[0],	
                 (CPU_STK_SIZE)WIFI_STK_SIZE/10,	
                 (CPU_STK_SIZE)WIFI_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);				 
				 
	//WIFI
	OSTaskCreate((OS_TCB 	* )&guntongTaskTCB,				//12
				 (CPU_CHAR	* )"guntong task", 		
                 (OS_TASK_PTR )guntong_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )guntong_TASK_PRIO,     	
                 (CPU_STK   * )&guntong_TASK_STK[0],	
                 (CPU_STK_SIZE)guntong_STK_SIZE/10,	
                 (CPU_STK_SIZE)guntong_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);		
				 
	//电压采集		
	OSTaskCreate((OS_TCB 	* )&DCvTaskTCB,					//13
				 (CPU_CHAR	* )"DCv task", 		
                 (OS_TASK_PTR )DCv_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )DCv_TASK_PRIO,     	
                 (CPU_STK   * )&DCv_TASK_STK[0],	
                 (CPU_STK_SIZE)DCv_STK_SIZE/10,	
                 (CPU_STK_SIZE)DCv_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);					 
				 
				 
				 
				 
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//挂起开始任务			 
	OS_CRITICAL_EXIT();	//进入临界区
								 
}




void Transducer_task(void *p_arg)  	//开关量输入采集
{
	u8 temp_l=0;
	OS_ERR err;
	p_arg = p_arg;	
	
	while(1)
	{
		
		//只剩下红外避障的程序跟状态灯
		
		
//		agv_24V_input(); 	//外部24V输入信号采集
//		AGV_Stop2Start();	//启动停止信号的采集,并控制车的停止启动
		
		
//		//行走和停止的状态灯
//		if(g_Start_flag.Start_Manu_PID || g_Start_flag.Start_Auto_PID)		
//		{
//			if(temp_l >= 25)
//			{
//				Car_LED_Blue = !Car_LED_Blue;
//				temp_l = 0;
//			}	
//		}
//		else
//		{
//			Car_LED_Blue = 1;	//停止 -- 蓝灯常亮
//		}
//		
//		
//		//红外和机械避障的状态灯
//		if(!g_Start_flag.Start_IR || !g_Start_flag.button_Start)	//停止
//		{
//			Car_LED_Yellow = 1;			//黄灯闪烁
//		}
//		else
//		{
//			Car_LED_Yellow = 0;
//		}
//		

//		if(temp_l>200)
//		{
//			temp_l = 0;
//		}
//		temp_l++;
		
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err); //每10ms检测输入状态
	}
}

//自动操作任务
void Auto_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		
		//speek("手动任务");
		
		
		
		
		
		
		
		
		OSTimeDlyHMSM(0,0,2,0,OS_OPT_TIME_HMSM_STRICT,&err); //延时20ms
	}
}

void Manual_task(void *p_arg)  //手动任务
{
	OS_ERR err;
	p_arg = p_arg;
	
	Motec_init();
	DwqXunZheng_QH();				//电位器寻正
	
	while(1)
	{
		//前进左右转 -- 后轮打直,前轮负责转向
		//后退左右转 -- 前轮打直,后轮负责转向
		
		
		if(g_AGV_Car_mode) //1://手动模式
		{	
			if(g_AGV_shoudong_Speed > 10 ) 	
			{
				if(1)			//1:启动
				{

					g_Start_flag.Start_Manu_PID = 1;	//1:启动
					g_XZ_Ok = 0;
					PID_SD_Adjust(g_AGV_shoudong_Speed,g_SD_Kp,g_SD_Ki,g_SD_Kd);
				}		
			}		
			else 
			{
				MotoStop(0);
				if(!g_XZ_Ok)
				{
					DwqXunZheng_QH();	//电位器寻正	
				}
				
				g_Start_flag.Start_Manu_PID = 0;			
			}					
		}
		
		
//		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err); //延时5ms
	}
}
void float_task(void *p_arg)		//红外和机械避障
{
	OS_ERR err;
	p_arg = p_arg;	
	

	while(1)
	{	
//		if(!g_AGV_Car_dir)	//0:前进
//		{
//			if(!g_flag_IR_qian_jin)				//近红外避障 0触发
//			{
//				delay_rtos(0,0,0,10);
//				if(!g_flag_IR_qian_jin)		
//				{
//					g_Start_flag.Start_IR = 0;	//0停止
//				}
//			}	
//			else
//			{
//				delay_rtos(0,0,0,10);
//				if(g_flag_IR_qian_jin)		
//				{
//					g_Start_flag.Start_IR = 1;	//1启动
//				}		
//			}	

//			
//			if(!g_flag_fangzhuang_qian)				//jixie避障 0触发
//			{
//				delay_rtos(0,0,0,10);
//				if(!g_flag_fangzhuang_qian)		
//				{
//					g_Start_flag.Start_jixie = 0;	//0停止
//				}
//			}	
//			else
//			{
//				delay_rtos(0,0,0,10);
//				if(g_flag_fangzhuang_qian)		
//				{
//					g_Start_flag.Start_jixie = 1;	//1启动
//				}		
//			}
//			
//			
//		}
//		else
//		{
//			if(!g_flag_IR_hou_jin)				//近红外避障 0触发
//			{
//				delay_rtos(0,0,0,10);
//				if(!g_flag_IR_hou_jin)		
//				{
//					g_Start_flag.Start_IR = 0;	//0停止
//				}
//			}	
//			else
//			{
//				delay_rtos(0,0,0,10);
//				if(g_flag_IR_hou_jin)		
//				{
//					g_Start_flag.Start_IR = 1;	//1启动
//				}		
//			}

//			if(!g_flag_fangzhuang_hou)				//jixie避障 0触发
//			{
//				delay_rtos(0,0,0,10);
//				if(!g_flag_fangzhuang_hou)		
//				{
//					g_Start_flag.Start_jixie = 0;	//0停止
//				}
//			}	
//			else
//			{
//				delay_rtos(0,0,0,10);
//				if(g_flag_fangzhuang_hou)		
//				{
//					g_Start_flag.Start_jixie = 1;	//1启动
//				}		
//			}


//			
//		}
		
		
		

		
		
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err); //延时ms
	}
}

void Screen_task(void *p_arg)    	//触摸屏界面操作
{
	OS_ERR err;
	p_arg = p_arg;	
	
	g_AGV_Car_Speed = g_sys_set_agvSPEED = 1;		//AGV车的全局运行速度 0:低速 1:中速 2:高速
	g_screen_qianlun_jiaodu = g_ADC_dianweiqi[0] * 100;
	g_screen_houlun_jiaodu  = g_ADC_dianweiqi[1] * 100;	

	while(1)
	{

		#if 0
		
	    switch(HmiScreenSetGet)
        {

			//开发者界面程序段
			case ShouDongJieMian: 		//--3					//3//手动操作界面
			{
				while(HmiScreenSetGet == ShouDongJieMian)		
				{
					
					 if(g_shoudong_manual == 1)						//屏幕的手动按键按下时
					 {
							//Shou_Dong();		//切到手动程序
						 
							g_AGV_Car_mode = 1;
					 }
					 else if(g_shoudong_auto == 1)				//屏幕的自动按键按下时
					 {
							//Zi_Dong();			//切到自动程序
							g_AGV_Car_mode = 0;
					 }
					 else if(g_shoudong_guntong_zuozhuan == 1)				//辊筒左转
					 {
							//Cha_Sheng();
					 }
					 else if(g_shoudong_guntong_youzhuan == 1)				//辊筒右转
					 {
							//Cha_Jiang();
					 }
					 else if(g_shoudong_turn_left == 1)				//左转
					 {
							//Zuo_Fen();
					 }
					 else if(g_shoudong_turn_right == 1)				//右转
					 {
							//You_Fen();
					 }
					 if(g_AGV_Car_mode == 0)				//0:自动模式 1:自动模式
					 {
							if(g_shoudong_start == 1)			//启动
							{
								//Qi_Dong();		//启动的语音
							}
							if(g_shoudong_stop == 1)		 	//停止
							{
								//Ting_Zhi();		//停止的语音
							}
							else if(g_shoudong_goto == 1)		//设置前进方向,自动模式下,设置行走方向
							{
								//Qian_Jin();		//前进模式
							}
							else if(g_shoudong_goback == 1) 	//设置后退方向,自动模式下,设置行走方向
							{
								//Hou_You();		//后退模式
							}
					 }
					 /*********当屏幕上参数发生改变时记录参数到flash开始********/
					 if(g_AGV_shoudong_Speed_bili != g_shoudong_screen_speed)		//PLC_Data[3]	
					 {
							g_AGV_shoudong_Speed_bili = g_shoudong_screen_speed;				//更新屏幕的速度显示值
							
						 SystemParameter[3] = g_AGV_shoudong_Speed_bili;		//手动速度比例:  x/10

							SetOneParameterToSystem(SystemParameter[3], 3);
					 }
					 /*********当屏幕上参数发生改变时记录参数到flash结束********/

					 osdelay_ms(10);
				}
			}
			break;
			//路径规划段界面	
			case LuJingSheZhiJieMian: 				//9//路径设置界面（第一次选择规划路径时的界面）
			{
				//如果路径号为零，则置一
				if(HmiRouteNum == 0)
				{
					 HmiRouteNum = 2;
				}
			}
			break;
			case LuJingGuiHuaJieMian:				//路径规划界面
			case FuZhiLuJingTanChuJieMian:			//复制路径界面
			case QueDingChaRuZhanDianJieMian: 	//插入路径界面
			case QueDingShanChuZhanDianJieMian:	//删除路径界面
			case XvNiDiBiaoJieMian:  				//虚拟地标界面
			{
				while(HmiScreenSetGet == LuJingGuiHuaJieMian || HmiScreenSetGet == FuZhiLuJingTanChuJieMian ||
						HmiScreenSetGet == QueDingChaRuZhanDianJieMian || HmiScreenSetGet == QueDingShanChuZhanDianJieMian ||
						HmiScreenSetGet == XvNiDiBiaoJieMian)
				{
	//				 //复制路径
	//				 if(HmiScreenSetGet == FuZhiLuJingTanChuJieMian)
	////				 {
	////						//被复制路径号初始化为1
	////						HmiRouteCopiedNum = 1;
	////						while(HmiScreenSetGet == FuZhiLuJingTanChuJieMian) //处于复制路径界面
	////						{
	////							//被复制路径号为零时置1
	////							if(HmiRouteCopiedNum == 0)
	////								 HmiRouteCopiedNum = 1;
	////							if(HmiRouteCopiedNum > RouteNum)
	////							{
	////								 HmiRouteCopiedNum = 1;
	////							}

	////							//输入的被复制流程号变化时更新
	////							if(HmiRouteCopiedNumTemp != HmiRouteCopiedNum)
	////								 HmiRouteCopiedCount = GetRouteStationNum(HmiRouteCopiedNum);


	////							//确定复制
	////							if(HmiRoutemode == 1)
	////							{
	////								 //读取被复制信息到当前
	////								 GetRouteData(HmiRouteCopiedNum);

	////								 //保存并更新被复制信息
	////								 SetRouteData(HmiRouteNum);

	////								 UpdataStationToHmi();

	////								 //设置当前站点为1
	////								 if(HmiStationNum != 0)
	////										HmiStationSerialNum = 1;

	////								 //退出复制模式
	////								 HmiRoutemode = 0;
	////							}
	////							osdelay_ms(10);
	////						}
	////				 }

	//				 //添加站点
	//				 if(HmiRoutemode == 2)
	//				 {
	//						//站点数加一
	//						HmiStationNum ++;
	//						//站点号等于站点数
	//						HmiStationSerialNum = HmiStationNum;

	//						//地标号清零
	//						keynumber = 0;

	//						ClearStationToHmi();
	//				 
	//						while(HmiRoutemode == 2)
	//						{
	//							//如果读到地标，更新到显示器
	//							if(keynumber != 0)
	//							{
	//								 HmiRfidNum = keynumber;//刷新地标号
	//								 keynumber = 0;
	//							}

	//							//确定
	//							if(HmiRouteEnter == 1)
	//							{
	//								 HmiRouteEnter = 0;
	//								 //更新站点数据到当前数据
	//								 UpdataStationToNow();
	//								 //保存当前站点数据
	//								 SetRouteData(HmiRouteNum);
	//								 //退出添加模式
	//								 HmiRoutemode =	0;
	//							}
	//							else
	//								 //取消
	//								 if(HmiRouteEnter == 2)
	//								 {
	//										HmiRouteEnter = 0;
	//										//站点数减一
	//										HmiStationNum--;
	//										//站点号等于站点数
	//										HmiStationSerialNum = HmiStationNum;
	//										//退出添加模式
	//										HmiRoutemode =	0;
	//								 }
	//							osdelay_ms(10);
	//						}
	//				 }
	//				 //插入站点
	//				 if(HmiRoutemode == 3)
	//				 {
	//						//站点数加一
	//						HmiStationNum ++;
	//						//站点号为零时加一
	//						if(HmiStationNum == 0)
	//							HmiStationSerialNum = 1;

	//						//地标号清零
	//						keynumber = 0;

	//						ClearStationToHmi();

	//						while(HmiRoutemode == 3)
	//						{
	//							//如果读到地标，更新到显示器
	//							if(keynumber != 0)
	//							{
	//								 HmiRfidNum = keynumber;//刷新地标号
	//								 keynumber = 0;
	//							}

	//							//确定
	//							if(HmiRouteEnter == 1)
	//							{
	//								 HmiRouteEnter = 0;

	//								 //站点信息向后移动一个位置，并插入当前站点信息
	//								 for(i = HmiStationNum; i >= HmiStationSerialNum; i--)
	//								 {
	//										for(j = 0; j < StaiionInfor; j++)
	//										{
	//											NowRouteInfor[i][j] = NowRouteInfor[i - 1][j];
	//										}
	//								 }

	//								 //更新站点信息到当前
	//								 UpdataStationToNow();

	//								 //保存路径信息
	//								 SetRouteData(HmiRouteNum);

	//								 //退出添加模式
	//								 HmiRoutemode =	0;
	//							}
	//							else
	//								 //取消
	//								 if(HmiRouteEnter == 2)
	//								 {
	//										HmiRouteEnter = 0;
	//										//站点数减一
	//										HmiStationNum--;
	//										//站点号为1时置零
	//										if(HmiStationSerialNum == 1)
	//											HmiStationSerialNum = 0;
	//										//退出添加模式
	//										HmiRoutemode =	0;
	//								 }

	//							osdelay_ms(10);
	//						}

	//				 }
	//				 //更改站点
	//				 if(HmiRoutemode == 4)
	//				 {
	//						keynumber = 0;
	//						while(HmiRoutemode == 4)
	//						{
	//							//如果读到地标，更新到显示器
	//							if(keynumber != 0)
	//							{
	//								 HmiRfidNum = keynumber;//刷新地标号
	//								 keynumber = 0;
	//							}

	//							//确定
	//							if(HmiRouteEnter == 1)
	//							{
	//								 HmiRouteEnter = 0;
	//								 //更新站点数据到当前数据
	//								 UpdataStationToNow();
	//								 //保存当前站点数据
	//								 SetRouteData(HmiRouteNum);
	//								 //退出添加模式
	//								 HmiRoutemode =	0;
	//							}
	//							else
	//								 //取消
	//								 if(HmiRouteEnter == 2)
	//								 {
	//										HmiRouteEnter = 0;
	//										//更新到界面
	//										UpdataStationToHmi();
	//										//退出添加模式
	//										HmiRoutemode =	0;
	//								 }
	//							osdelay_ms(10);
	//						}
	//				 }

	//				 //删除站点
	//				 if(HmiRoutemode == 5)
	//				 {
	//						if(HmiStationNum > 0)
	//						{
	//							//将后面所数据向前移动一位
	//							if((HmiStationNum > 0) && ((HmiStationNum) != HmiStationSerialNum))
	//							{
	//								 //站点信息向前移动一个位置
	//								 for(i = HmiStationSerialNum; i < HmiStationNum; i++)
	//								 {
	//										for(j = 0; j < StaiionInfor; j++)
	//										{
	//											NowRouteInfor[i - 1][j] = NowRouteInfor[i][j];
	//										}
	//								 }
	//							}

	//							HmiStationNum--;
	//							if(HmiStationNum == 0)
	//								 HmiStationSerialNum = 0;
	//							else if((HmiStationNum + 1) == HmiStationSerialNum)
	//								 HmiStationSerialNum--;
	//							//保存路径信息
	//							SetRouteData(HmiRouteNum);

	//							//更新界面
	//							UpdataStationToHmi();
	//						}
	//						HmiRoutemode = 0;
	//				 }

	//				 //查询站点
	//				 if(HmiRoutemode == 0)
	//				 {
	//						//当路径站点数大于0，并且序号为0时，设置序号为1，并更新显示内容
	//						if((HmiStationNum > 0) && (HmiStationSerialNum == 0))
	//						{
	//							HmiStationSerialNum = 1;
	//							//更新到界面
	//							UpdataStationToHmi();
	//						}
	//						else if(HmiStationNum == 0)
	//						{
	//							HmiStationSerialNum = 0;
	//						}

	//						//路径号变化时，更新界面
	//						if(HmiRouteNumFlag != HmiRouteNum)
	//						{
	//							HmiRouteNumFlag = HmiRouteNum;
	//							//获取路径信息
	//							GetRouteData(HmiRouteNum);
	//							if(HmiStationNum > 0)
	//								 HmiStationSerialNum = 1;
	//							//更新到界面
	//							UpdataStationToHmi();
	//						}

	//						//上一个
	//						if(HmiStationLast == 1)
	//						{
	//							HmiStationLast = 0;
	//							if(HmiStationSerialNum > 1)
	//								 HmiStationSerialNum--;
	//							else
	//								 HmiStationSerialNum = HmiStationNum;
	//						}
	//						//下一个
	//						if(HmiStationNext == 2)
	//						{
	//							HmiStationNext = 0;
	//							if(HmiStationSerialNum < HmiStationNum)
	//								 HmiStationSerialNum++;
	//							else if(HmiStationSerialNum != 0)
	//								 HmiStationSerialNum = 1;
	//						}

	//						if(HmiStationNum == 0)		//没有站点
	//						{
	//							//清除站点信息
	//							ClearStationToHmi();
	//						}
	//						else
	//						{
	//							//更新站点信息
	//							UpdataStationToHmi();
	//						}
	//				 }
					osdelay_ms(10);
				}
			}
			break;
			//流程规划段界面
			case LiuChengSheZhiJieMian: 			//10//流程设置界面（第一次选择规划流程时的界面）
			{
				//如果流程号为零，则置一
				if(HmiProcessNum == 0)
				{
					 HmiProcessNum = 1;
				}
			}
			break;
			case LiuChengGuiHuaJieMian:				//流程规划界面
			case FuZhiLiuChengTanChuJieMian:		//复制流程界面
			case QueDingChaRuLiuChengJieMian:   	//插入流程界面
			case QueDingShanChuLiuChengJieMian: 	//删除流程界面
			{
				while(HmiScreenSetGet == LiuChengGuiHuaJieMian || HmiScreenSetGet == FuZhiLiuChengTanChuJieMian || HmiScreenSetGet == QueDingChaRuLiuChengJieMian || HmiScreenSetGet == QueDingShanChuLiuChengJieMian)
				{
//					 //查看模式
//					 if(HmiProcessMode == 0)
//					 {
//							//当前流程步数大于0，并且界面序号为0时，设置序号为1，并更新显示内容
//							if((HmiStepNum > 0) && (HmiProcessSerialNum == 0))
//							{
//								HmiProcessSerialNum = 1;
//								GetProcessData();
//							}
//							else if(HmiStepNum == 0)
//							{
//								HmiProcessSerialNum = 0;
//								HmiProcessStartNum = 0;
//								HmiProcessStopNum = 0;
//								HmiProcessAction = 0;
//							}

//							//流程号变化时，更新界面
//							if(HmiProcessNumFlag != HmiProcessNum)
//							{
//								HmiProcessNumFlag = HmiProcessNum;
//								GetProcessData();
//							}

//							if(HmiProcessLast == 1) //上一个
//							{
//								HmiProcessLast = 0;
//								if(HmiProcessSerialNum > 1)
//									 HmiProcessSerialNum--;
//								else
//									 HmiProcessSerialNum = HmiStepNum;
//							}
//							else if(HmiProcessNext == 2) //下一个
//							{
//								HmiProcessNext = 0;
//								if(HmiProcessSerialNum < HmiStepNum)
//									 HmiProcessSerialNum++;
//								else if(HmiProcessSerialNum != 0)
//									 HmiProcessSerialNum = 1;
//							}

//							if(HmiStepNum == 0)
//							{
//								ClearStepToHmi();
//							}
//							else
//							{
//								UpdataProcessToHmi();
//							}

//					 }

//					 //复制流程
//					 if(HmiScreenSetGet == FuZhiLiuChengTanChuJieMian)
//					 {
//							//初始化别复制流程为1
//							HmiProcessCopiedNum = 1;
//							while(HmiScreenSetGet == FuZhiLiuChengTanChuJieMian)
//							{
//								//被复制流程号为零时置1
//								if(HmiProcessCopiedNum == 0)
//									 HmiProcessCopiedNum = 1;
//								if(HmiProcessCopiedNum > ProcessNum)
//									 HmiProcessCopiedNum = 1;

//								//输入的被复制流程号变化时更新
//								if(HmiProcessCopiedNumTemp != HmiProcessCopiedNum)
//									 HmiProcessCopiedCount = GetProcessStepNum(HmiProcessCopiedNum);

//								//确定复制
//								if(HmiProcessMode == 1)
//								{
//									 //读取被复制信息到当前
//									 GetProcessDataFrom(HmiProcessCopiedNum);
//									 //保存被复制信息
//									 SetProcessData();

//									 //设置当前序号为1
//									 if(HmiStepNum != 0)
//											HmiProcessSerialNum = 1;

//									 //退出复制模式
//									 HmiProcessMode = 0;
//								}
//								osdelay_ms(10);
//							}
//					 }

//					 //添加流程
//					 if(HmiProcessMode == 2)
//					 {
//							//流程步数加一
//							HmiStepNum++;
//							//序号等于流程数
//							HmiProcessSerialNum = HmiStepNum;

//							ClearStepToHmi();

//							while(HmiProcessMode == 2)
//							{
//								switch(HmiProcessAction)
//								{
//								case 0://单次执行
//									 break;
//								case 1://顺序执行到
//									 if(HmiProcessStopNum <= HmiProcessStartNum)
//									 {
//											if(HmiProcessStartNum == ProcessNum)
//												HmiProcessStopNum = HmiProcessStartNum;
//											else
//												HmiProcessStopNum = HmiProcessStartNum + 1;
//									 }
//									 break;
//								case 2://倒序执行
//									 if(HmiProcessStopNum >= HmiProcessStartNum)
//									 {
//											if(HmiProcessStartNum == 1)
//												HmiProcessStopNum = HmiProcessStartNum;
//											else
//												HmiProcessStopNum = HmiProcessStartNum - 1;
//									 }
//									 break;
//								case 3://循环执行
//									 if(HmiProcessStopNum == 0)
//									 {
//											HmiProcessStopNum = 1;
//									 }
//									 break;
//								default:
//									 break;
//								}

//								//确定
//								if(HmiProcessEnter == 1)
//								{
//									 HmiProcessEnter = 0;
//									 //保存并更新数据
//									 SetProcessData();

//									 //退出添加模式
//									 HmiProcessMode = 0;
//								}
//								//取消
//								else if(HmiProcessEnter == 2)
//								{
//									 HmiProcessEnter = 0;

//									 //流程步数减一
//									 HmiStepNum--;
//									 //序号等于流程数
//									 HmiProcessSerialNum = HmiStepNum;

//									 //退出添加模式
//									 HmiProcessMode = 0;
//								}
//								osdelay_ms(10);
//							}
//					 }


//					 //插入流程
//					 if(HmiProcessMode == 3)
//					 {
//							//流程步数加一
//							HmiStepNum++;
//							//序号为零时加一
//							if(HmiProcessSerialNum == 0)
//								HmiProcessSerialNum = 1;

//							ClearStepToHmi();

//							while(HmiProcessMode == 3)
//							{
//								switch(HmiProcessAction)
//								{
//								case 0://单次执行
//									 break;
//								case 1://顺序执行到
//									 if(HmiProcessStopNum <= HmiProcessStartNum)
//									 {
//											if(HmiProcessStartNum == ProcessNum)
//												HmiProcessStopNum = HmiProcessStartNum;
//											else
//												HmiProcessStopNum = HmiProcessStartNum + 1;
//									 }
//									 break;
//								case 2://倒序执行
//									 if(HmiProcessStopNum >= HmiProcessStartNum)
//									 {
//											if(HmiProcessStartNum == 1)
//												HmiProcessStopNum = HmiProcessStartNum;
//											else
//												HmiProcessStopNum = HmiProcessStartNum - 1;
//									 }
//									 break;
//								case 3://循环执行
//									 if(HmiProcessStopNum == 0)
//									 {
//											HmiProcessStopNum = 1;
//									 }
//									 break;
//								default:
//									 break;
//								}

//								//确定
//								if(HmiProcessEnter == 1)
//								{
//									 HmiProcessEnter = 0;

//									 //保存并更新数据
//									 InsertProcessData();

//									 //退出插入模式
//									 HmiProcessMode = 0;
//								}
//								//取消
//								else if(HmiProcessEnter == 2)
//								{
//									 HmiProcessEnter = 0;

//									 //流程步数减一
//									 HmiStepNum--;
//									 //序号为1时减一
//									 if(HmiProcessSerialNum == 1)
//											HmiProcessSerialNum = 0;

//									 //退出添加模式
//									 HmiProcessMode = 0;
//								}
//								osdelay_ms(10);
//							}
//					 }

//					 //更改流程
//					 if(HmiProcessMode == 4)
//					 {
//							while(HmiProcessMode == 4)
//							{
//								switch(HmiProcessAction)
//								{
//								case 0://单次执行
//									 break;
//								case 1://顺序执行到
//									 if(HmiProcessStopNum <= HmiProcessStartNum)
//									 {
//											if(HmiProcessStartNum == ProcessNum)
//												HmiProcessStopNum = HmiProcessStartNum;
//											else
//												HmiProcessStopNum = HmiProcessStartNum + 1;
//									 }
//									 break;
//								case 2://倒序执行
//									 if(HmiProcessStopNum >= HmiProcessStartNum)
//									 {
//											if(HmiProcessStartNum == 1)
//												HmiProcessStopNum = HmiProcessStartNum;
//											else
//												HmiProcessStopNum = HmiProcessStartNum - 1;
//									 }
//									 break;
//								case 3://循环执行
//									 if(HmiProcessStopNum == 0)
//									 {
//											HmiProcessStopNum = 1;
//									 }
//									 break;
//								default:
//									 break;
//								}

//								//确定
//								if(HmiProcessEnter == 1)
//								{
//									 HmiProcessEnter = 0;
//									 //保存并更新数据
//									 SetProcessData();

//									 //退出更改模式
//									 HmiProcessMode = 0;
//								}
//								//取消
//								else if(HmiProcessEnter == 2)
//								{
//									 HmiProcessEnter = 0;

//									 //退出添加模式
//									 HmiProcessMode = 0;
//								}
//								osdelay_ms(10);
//							}
//					 }

//					 //删除流程
//					 if(HmiProcessMode == 5)
//					 {
//							DeleteProcessData();
//							//退出删除模式
//							HmiProcessMode = 0;
//					 }

					 osdelay_ms(10);
				}
			}
			break;
			//执行任务段界面
			case ZhiXingLuJingJieMian: 				//18//执行路径界面
			{
				while(HmiScreenSetGet == ZhiXingLuJingJieMian)
				{
//					 if(ZhiXingLuJingQueDing == 1)
//					 {
//							//保存路径号
//							SetOneParameterToSystem(HmiRouteNum, 24);
//							SystemParameter[26] = HmiTask;
//							//保存任务号
//							SetOneParameterToSystem(HmiTask, 26);
//							ZhiXingLuJingQueDing = 0;

//							//加载自动界面信息
//							HmiAutoReload();

//							HmiTask = 1;

//							FX_flag = 1; //方向切换到前进

//							//跳转到自动界面
//							HmiScreenSetGet = ZiDongJieMian;
//					 }
					 osdelay_ms(10);
				}
			}
			break;
			case ZhiXingLiuChengJieMian:  			//执行流程界面
			{
				while(HmiScreenSetGet == ZhiXingLiuChengJieMian)
				{
//					 if(ZhiXingLiuChengQueDing == 1)
//					 {
//							ActiveProcess();
//							ZhiXingLiuChengQueDing = 0;
//					 }
					 osdelay_ms(10);
				}
			}
			break;
			case ZiDongJieMian: 						//自动运行界面
			{
				while(HmiScreenSetGet == ZiDongJieMian)
				{
//					 if(PLC_OutPut[4] == 1)						//屏幕的手动按键按下时
//					 {
//							Shou_Dong();		//切到手动程序
//					 }
//					 else if(PLC_OutPut[5] == 1)				//屏幕的自动按键按下时
//					 {
//							Zi_Dong();			//切到自动程序
//					 }
//					 if(Mode_flag == 1)
//					 {
//							if(PLC_OutPut[11] == 1)			//启动
//							{
//								Qi_Dong();
//							}
//							if(PLC_OutPut[12] == 1)		 	//停止
//							{
//								Ting_Zhi();
//							}
//					 }
					 osdelay_ms(10);
				}
			}
			break;
			case QuXiaoRenWuJieMian: 				//取消任务界面
			{
				while(HmiScreenSetGet == QuXiaoRenWuJieMian)
				{
//					 if(QuXiaoRenWuQueDing == 1)
//					 {
//							QuXiaoRenWuQueDing = 0;

//							HmiTask = 0;

//							//任务状态为空闲
//							HmiTaskState = 0;

//							//停车
//							Ting_Zhi();

//							//清除开门信号
//							PLC2_Data[22] = 1; //(1,弹起开门按钮，10，按下开门按钮)

//							//切换方向标识为前进
//							FX_flag = 1;

//							//清除请求进入
//							PLC2_Data[32] = 0;
//							//清除设备内信号
//							PLC2_Data[38] = 0;
//							PLC2_Data[39] = 0;
//							PLC2_Data[40] = 0;
//							PLC2_Data[41] = 0;

//							//跳转界面，返回之前界面
//				//							  if(lastscreenNum != 0)
//				//							  {
//				//									HmiScreenSetGet = lastscreenNum;
//				//									lastscreenNum = 0;
//				//							  }
//				//							  else
//								HmiScreenSetGet = 2;

//							//删除任务
//							OSTaskDel((OS_TCB*)&Task5_TaskTCB, &err);
//							//重新创建任务
//							OSTaskCreate((OS_TCB 	* )&Task5_TaskTCB,
//											(CPU_CHAR	* )"Task5 task",
//											(OS_TASK_PTR )Task5_task,
//											(void		* )0,
//											(OS_PRIO	  )Task5_TASK_PRIO,
//											(CPU_STK   * )&Task5_TASK_STK[0],
//											(CPU_STK_SIZE)Task5_STK_SIZE / 10,
//											(CPU_STK_SIZE)Task5_STK_SIZE,
//											(OS_MSG_QTY  )0,
//											(OS_TICK	  )0,
//											(void   	* )0,
//											(OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
//											(OS_ERR 	* )&err);
//					 }
					 osdelay_ms(10);
				}
			}
			break;
			//数据调试及系统参数界面段程序
			case ChuanGanQiJieMian:					//22//各个传感器的状态更新到屏幕上
			{
				while(HmiScreenSetGet == ChuanGanQiJieMian)
				{
//					PLC_OutPut[25] = CBBZ2;	//左叉臂红外避障
//					PLC_OutPut[26] = CBBZ1;	//右叉臂红外避障
//					PLC_OutPut[27] = QBZ1N;	//车前左上雷达
//					PLC_OutPut[28] = QBZ1F; //车前右上雷达
//					PLC_OutPut[29] = QBZ2N; //车前左下雷达
//					PLC_OutPut[30] = QBZ2F; //车前右下雷达
//					PLC_OutPut[46] = JXBZ; 	//机械避障
//					PLC_OutPut[47] = ZZBZ1; //左转上雷达避障1
//					PLC_OutPut[48] = ZZBZ2; //左转下雷达避障2
//					PLC_OutPut[49] = YZBZ1; //右转上雷达避障1
//					PLC_OutPut[50] = YZBZ2; //右转下雷达避障2
					
					osdelay_ms(20);
				}
			}
			break;
			case JiBenShuJuJieMian:					//23//基本数据测试界面
			{
				while(HmiScreenSetGet == JiBenShuJuJieMian)
				{
					 PID.Kp = g_screen_Auto_Kp;		           //更新自动Kp
					 PID.Ki = g_screen_Auto_Ki;                 //更新自动ki
					 PID.Kd = g_screen_Auto_Kd;                //更新自动kd

					 g_SD_Kp = g_screen_Manu_Kp;	           //更新手动kp
					 g_SD_Ki = g_screen_Manu_Ki;               //更新手动ki
					 g_SD_Kd = g_screen_Manu_Kd;               //更新手动kd

					 g_PID_time = g_screen_control_TIME;						//测试前进的PID控制周期
					
					 g_DWQ.qianlun_L_val 	 = g_screen_qianlun_ZuoZhi;					//前轮左极限
					 g_DWQ.qianlun_zhong_val = g_screen_qianlun_ZhongZhi;					//前轮中值
					 g_DWQ.qianlun_R_val 	 = g_screen_qianlun_YouZhi;					//前轮右极限
					
					 g_DWQ.houlun_L_val 	 = g_screen_houlun_ZuoZhi;					//后轮左极限
					 g_DWQ.houlun_zhong_val  = g_screen_houlun_ZhongZhi;					//后轮中值
					 g_DWQ.houlun_R_val 	 = g_screen_houlun_YouZhi;					//后轮右极限					

					 SystemParameter[50] = g_screen_Auto_Kp;
					 SystemParameter[51] = g_screen_Auto_Ki;
					 SystemParameter[52]= g_screen_Auto_Kd;
					 
					 SystemParameter[53] = g_screen_Manu_Kp;
					 SystemParameter[54] = g_screen_Manu_Ki;
					 SystemParameter[55] = g_screen_Manu_Kd;	
					 
					 SystemParameter[56]= g_screen_control_TIME;
					 
					 SystemParameter[57]= g_screen_qianlun_ZuoZhi;
					 SystemParameter[58]= g_screen_qianlun_ZhongZhi;
					 SystemParameter[59]= g_screen_qianlun_YouZhi;
					 
					 SystemParameter[60] = g_screen_houlun_ZuoZhi;
					 SystemParameter[61] = g_screen_houlun_ZhongZhi; 	
					 SystemParameter[62] = g_screen_houlun_YouZhi; 
					 
					 osdelay_ms(20);
				}

				W25QXX_Write_16(SystemParameter, 50, 13);			//将当前屏幕参数写入到flash中
			}
			break;
			case ChaBiShuJuJieMian:					//24//叉臂磁导航数据调试
			{
				while(HmiScreenSetGet == ChaBiShuJuJieMian)
				{
	//				if(SystemParameter[60] != PLC_Data[60] || SystemParameter[61] != PLC_Data[61] || SystemParameter[62] != PLC_Data[62] ||
	//							SystemParameter[63] != PLC_Data[63] || SystemParameter[64] != PLC_Data[64] || SystemParameter[65] != PLC_Data[65] ||
	//							SystemParameter[66] != PLC_Data[66] || SystemParameter[67] != PLC_Data[67] || SystemParameter[68] != PLC_Data[68] ||
	//							SystemParameter[69] != PLC_Data[69] || SystemParameter[70] != PLC_Data[70] || SystemParameter[71] != PLC_Data[71] ||
	//							SystemParameter[72] != PLC_Data[72] || SystemParameter[73] != PLC_Data[73] || SystemParameter[74] != PLC_Data[74] ||
	//							SystemParameter[75] != PLC_Data[75] || SystemParameter[76] != PLC_Data[76])
	//				{
	//					 SystemParameter[60]	= PLC_Data[60];
	//					 SystemParameter[61]	= PLC_Data[61];
	//					 SystemParameter[62]	= PLC_Data[62];
	//					 SystemParameter[63]	=	PLC_Data[63];
	//					 SystemParameter[64]	=	PLC_Data[64];
	//					 SystemParameter[65]	=	PLC_Data[65];
	//					 SystemParameter[66]	= PLC_Data[66];
	//					 SystemParameter[67]	= PLC_Data[67];
	//					 SystemParameter[68]	= PLC_Data[68];
	//					 SystemParameter[69]	=	PLC_Data[69];
	//					 SystemParameter[70]	=	PLC_Data[70];
	//					 SystemParameter[71]	=	PLC_Data[71];
	//					 SystemParameter[72]	= PLC_Data[72];
	//					 SystemParameter[73]	= PLC_Data[73];
	//					 SystemParameter[74]	= PLC_Data[74];
	//					 SystemParameter[75]	= PLC_Data[75];
	//					 SystemParameter[76]	= PLC_Data[76];
	//									
	//					 
	//				}
					osdelay_ms(20); 
				}
				//W25QXX_Write_16(&SystemParameter[60], 60, 17);			//将当前屏幕参数写入到flash中
			}
			break;
			case LeiDaShuJuJieMian:					//25//雷达原始数据 
			{
				while(HmiScreenSetGet == LeiDaShuJuJieMian)
				{
	//				 PLC_Data[131] = g_GetUT_buff[0];
	//				 PLC_Data[132] = g_GetUT_buff[1];
	//				 PLC_Data[133] = g_GetUT_buff[2];
	//				 PLC_Data[134] = g_GetUT_buff[3];
	//				 PLC_Data[135] = g_GetUT_buff[4];
	//				 PLC_Data[136] = g_GetUT_buff[5];
	//				 PLC_Data[137] = g_GetUT_buff[6];
	//				 PLC_Data[138] = g_GetUT_buff[7];

					 osdelay_ms(20);
				}
			}
			break;
			case DianTiTiaoShiJieMian:				//26//电梯调试界面(在车上对电梯的一切进行操控和显示电梯状态)
			{
				while(HmiScreenSetGet == ChuanGanQiJieMian)
				{
//					if(PLC_OutPut[31] == 1) //三楼外呼按钮
//					{
//						 PLC2_Data[18] = 0;
//					}
//					else if(PLC_OutPut[32] == 1) //一楼外呼按钮
//					{
//						 PLC2_Data[19] = 0;
//					}
//					else if(PLC_OutPut[33] == 1) //3按钮
//					{
//						 PLC2_Data[20] = 0;
//					}
//					else if(PLC_OutPut[34] == 1) //1按钮
//					{
//						 PLC2_Data[21] = 0;
//					}
//					else if(PLC_OutPut[35] == 1) //开门
//					{
//						 PLC2_Data[22] = 0;
//					}
//					else if(PLC_OutPut[36] == 1) //关门
//					{
//						 PLC2_Data[23] = 0;
//					}
//					PLC_OutPut[37] = PLC2_Data[24]; //三楼平层（电梯在三楼）
//					PLC_OutPut[38] = PLC2_Data[25]; //一楼平层（电梯在一楼）
//					PLC_OutPut[39] = PLC2_Data[26]; //三楼门状态（开/关）
//					PLC_OutPut[40] = PLC2_Data[27]; //一楼门状态（开/关）
					
					osdelay_ms(20);
				}
			}
			break;
			case XiTongCanShuJieMian:				//8//系统参数界面，速度的档位设置、雷达阈值的设定
			{
				while(HmiScreenSetGet == XiTongCanShuJieMian)
				{
					//30开始
					 SystemParameter[30] = g_AGV_ID; 				//车号	  
					 SystemParameter[31] = g_AGV_speaker_val;     	//系统音量      
					 SystemParameter[32] = g_sys_set_agvSPEED;    	//AGV车速 -- 低中高 
					 SystemParameter[33] = g_AGV_speaker_key ;	  	//系统语音开关     
					 SystemParameter[34] = g_AGV_LED_car_state ;  	//车身状态灯    
					 SystemParameter[35] = HmiDiSu ;      			//低速    
					 SystemParameter[36] = HmiZhongSu;     			//中速    
					 SystemParameter[37] = HmiGaoSu  ;    			//高速
					 SystemParameter[38] = g_AGV_speed_duijie;		//工位对接速度
                                                                                                               
					 osdelay_ms(20);
				}

				W25QXX_Write_16(SystemParameter, 30, 9);	//将当前屏幕参数写入到flash中
			}
			break; 
			//用户界面程序段
			case YongHuShouYeJieMian:				//27//用户首页
			{
				while(HmiScreenSetGet == YongHuShouYeJieMian)
				{
//					 if(PLC_Data[107] == 1)		//去充电流程
//					 {
//							HmiProcessNum = 1;
//							PLC_Data[107] = 0;

//							//执行流程
//							ActiveProcess();
//					 }
//					 else if(PLC_Data[107] == 2)
//					 {
//							HmiProcessNum = 2;	//充电回流程
//							PLC_Data[107] = 0;

//							//执行流程
//							ActiveProcess();
//					 }
					 osdelay_ms(20);
				}
			}
			break;
			
			default:
			break;
        }		

		#endif

			
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err);
	}	
}



void PID_task(void *p_arg)			//自动模式--底层PID函数
{
	OS_ERR err;
	p_arg = p_arg;
	
	while(1)
	{
		if(g_AGV_Car_Speed>0 && g_AGV_Car_mode == 0) 
		{
			if(g_Start_flag.Start_Auto_PID)		//1:找到磁条
			{
				PID_Adjust(g_AGV_Car_Speed,g_Auto_Kp,g_Auto_Ki,g_Auto_Kd);
			}
			
		}
		else
		{
			OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err); 		//暂定5ms		
		}
	}
}

//1.开机电位器寻正
//2.启动按键被按下磁导航寻轨
//3.前后都上轨才能go

char iii[10];
void Control_task(void *p_arg)		//暂未使用
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{	


//		g_AGV_RFID_ID=0;
//		
//		while(g_AGV_RFID_ID==0)
//		{
//			OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err);
//		}		
//		AGV_System_Stop();
//		delay_rtos(0,0,2,0);
//		g_AGV_Car_dir = 1;
//		AGV_System_Start();
//		
//		delay_rtos(0,0,1,0);
//		
//		g_AGV_RFID_ID=0;
//		
//		while(g_AGV_RFID_ID==0)
//		{
//			OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err);
//		}		
//		AGV_System_Stop();
//		delay_rtos(0,0,2,0);
//		g_AGV_Car_dir = 0;
//		AGV_System_Start();
		

		
	
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err);
	}
}



void WIFI_task(void *p_arg)			//暂未使用 
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		
		
		
		
		
		
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err);
	}
}

void guntong_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		if(g_AGV_Car_mode) //1://手动模式
		{
			if(g_AGV_shoudong_dir == 9)			//摇杆左旋--滚筒左转
			{
				guntong_start = 1;
			}
			else if(g_AGV_shoudong_dir == 10)	//摇杆右旋--滚筒右转
			{
				guntong_fanzhuan = 1;
			}
			else
			{
				guntong_start = 0;
				guntong_fanzhuan = 0;
			}
		
		}
		
	
		
		
//		//清空地标
//		g_AGV_RFID_ID=0;
//		while(g_AGV_RFID_ID==0)
//		{
//			OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err);
//		}

//		guntong1();
		
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err);
	}
}


//4x32  显控屏地址
//g_After_filter[i]
void DCv_task(void *p_arg)											//电压采集	
{
	u8 temp_i=0;
	u8 temp_j=0;
	
	OS_ERR err;
	p_arg = p_arg;

	while(1)
	{		
		//读到地标响蜂鸣器						//叫250ms
		if(g_flag_RFID_beep)
		{
			BEEP = 1;
			temp_j++;
			if(temp_j == 5)
			{
				BEEP = 0;
				g_flag_RFID_beep = 0;	
				temp_j = 0;				
			}

		}

		//触摸屏通信指示灯和板子运行指示灯LED1		//500ms闪烁
		temp_i++;
		if(temp_i == 10)
		{
			LED1 = 0;
			P_RUN_LED = 1;
		}
		if(temp_i == 20)
		{
			LED1 = 1;
			P_RUN_LED = 0;
			temp_i = 0;
		}
		
		
		OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err);	
	}
}









