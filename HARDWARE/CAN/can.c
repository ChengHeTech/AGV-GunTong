#include "can.h"

u8 g_Get_RFID_buff[16];
u8 g_cinavi1_RXbuff[16];  //前磁导航
u8 g_cinavi2_RXbuff[16];	
u8 g_cinavi3_RXbuff[16];  
u8 g_cinavi4_RXbuff[16];	//后磁导航

u16 g_AGV_RFID_ID;//地标  //
u8  g_flag_RFID_beep;

//CAN初始化
//tsjw:重新同步跳跃时间单元.范围:CAN_SJW_1tq~ CAN_SJW_4tq
//tbs2:时间段2的时间单元.   范围:CAN_BS2_1tq~CAN_BS2_8tq;
//tbs1:时间段1的时间单元.   范围:CAN_BS1_1tq ~CAN_BS1_16tq
//brp :波特率分频器.范围:1~1024; tq=(brp)*tpclk1
//波特率=Fpclk1/((tbs1+1+tbs2+1+1)*brp);
//mode:CAN_Mode_Normal,普通模式;CAN_Mode_LoopBack,回环模式;
//Fpclk1的时钟在初始化的时候设置为42M,如果设置CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,CAN_Mode_LoopBack);
//则波特率为:42M/((6+7+1)*6)=500Kbps
//返回值:0,初始化OK;
//    其他,初始化失败; 
u8 CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode) //PA11 PA12
{

  	GPIO_InitTypeDef GPIO_InitStructure; 
	CAN_InitTypeDef        CAN_InitStructure;
  	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
#if CAN1_RX0_INT_ENABLE 
   	NVIC_InitTypeDef  NVIC_InitStructure;
#endif
    //使能相关时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能PORTA时钟	                   											 

  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//使能CAN1时钟	
	
    //初始化GPIO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11| GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化PA11,PA12
	
	  //引脚复用映射配置
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_CAN1); //GPIOA11复用为CAN1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_CAN1); //GPIOA12复用为CAN1
	  
  	//CAN单元设置
   	CAN_InitStructure.CAN_TTCM=DISABLE;	//非时间触发通信模式   
  	CAN_InitStructure.CAN_ABOM=DISABLE;	//软件自动离线管理	  
  	CAN_InitStructure.CAN_AWUM=DISABLE;//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
  	CAN_InitStructure.CAN_NART=ENABLE;	//禁止报文自动传送 
  	CAN_InitStructure.CAN_RFLM=DISABLE;	//报文不锁定,新的覆盖旧的  
  	CAN_InitStructure.CAN_TXFP=DISABLE;	//优先级由报文标识符决定 
  	CAN_InitStructure.CAN_Mode= mode;	 //模式设置 
  	CAN_InitStructure.CAN_SJW=tsjw;	//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1tq~CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=tbs1; //Tbs1范围CAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=tbs2;//Tbs2范围CAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=brp;  //分频系数(Fdiv)为brp+1	
  	CAN_Init(CAN1, &CAN_InitStructure);   // 初始化CAN1 
    
		//配置过滤器
 	CAN_FilterInitStructure.CAN_FilterNumber=0;	  //过滤器0
  	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32位 
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32位ID
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32位MASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
   	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//过滤器0关联到FIFO0
  	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //激活过滤器0
  	CAN_FilterInit(&CAN_FilterInitStructure);//滤波器初始化
		
#if CAN1_RX0_INT_ENABLE
	
	 CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0消息挂号中断允许.		    
  
  	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // 主优先级为1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;            // 次优先级为0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
#endif
	return 0;
} 

u8 CAN2_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode) //PB5  PB6
{

  	GPIO_InitTypeDef GPIO_InitStructure; 
	CAN_InitTypeDef        CAN_InitStructure;
  	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
#if CAN2_RX0_INT_ENABLE 
   	NVIC_InitTypeDef  NVIC_InitStructure;
#endif
    //使能相关时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能PORTB时钟	                   											 

  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);//使能CAN2时钟	
	
    //初始化GPIO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5| GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化PB5,PB6
	
	  //引脚复用映射配置
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_CAN2); //GPIOB5复用为CAN2
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_CAN2); //GPIOB6复用为CAN2
	  
  	//CAN单元设置
   	CAN_InitStructure.CAN_TTCM=DISABLE;	//非时间触发通信模式   
  	CAN_InitStructure.CAN_ABOM=DISABLE;	//软件自动离线管理	  
  	CAN_InitStructure.CAN_AWUM=DISABLE;//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
  	CAN_InitStructure.CAN_NART=ENABLE;	//禁止报文自动传送 
  	CAN_InitStructure.CAN_RFLM=DISABLE;	//报文不锁定,新的覆盖旧的  
  	CAN_InitStructure.CAN_TXFP=DISABLE;	//优先级由报文标识符决定 
  	CAN_InitStructure.CAN_Mode= mode;	 //模式设置 
  	CAN_InitStructure.CAN_SJW=tsjw;	//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1tq~CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=tbs1; //Tbs1范围CAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=tbs2;//Tbs2范围CAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=brp;  //分频系数(Fdiv)为brp+1	
  	CAN_Init(CAN2, &CAN_InitStructure);   // 初始化CAN1 
    
		//配置过滤器
 	CAN_FilterInitStructure.CAN_FilterNumber=14;	  //过滤器14
  	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32位 
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32位ID
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32位MASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
   	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//过滤器0关联到FIFO0
  	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //激活过滤器14
  	CAN_FilterInit(&CAN_FilterInitStructure);//滤波器初始化
		
#if CAN2_RX0_INT_ENABLE
	
	 CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);//FIFO0消息挂号中断允许.		    
  
  	NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // 主优先级为1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;            // 次优先级为0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
#endif
	return 0;
}
 
u8 g_led_blink[5];

u8 g_Ct_chugui[4];
#if CAN1_RX0_INT_ENABLE	//使能RX0中断   
//中断服务函数			    
void CAN1_RX0_IRQHandler(void)			//磁导航采集
{
  	CanRxMsg RxMessage;
	int i=0;
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	
	if(RxMessage.StdId == 1)			//前1磁导航
	{
		for(i=0;i<RxMessage.DLC;i++)
		g_cinavi1_RXbuff[i]=RxMessage.Data[i];
		g_CDH8_qian_1 = FindSpace1(g_cinavi1_RXbuff[0],8,g_AGV_Car_fencha_dir);
//		if(g_CDH8_qian_1.Distance)	//非0存
//		{
//			g_Ct_chugui[0] = g_CDH8_qian_1.Distance;
//		}
		
		g_led_blink[1]++;
	}   
	if(RxMessage.StdId == 2)			//前2磁导航
	{
		for(i=0;i<RxMessage.DLC;i++)
		g_cinavi2_RXbuff[i]=RxMessage.Data[i]; 
		g_CDH8_qian_2 = FindSpace1(g_cinavi2_RXbuff[0],8,g_AGV_Car_fencha_dir);
//		if(g_CDH8_qian_2.Distance)	//非0存
//		{
//			g_Ct_chugui[1] = g_CDH8_qian_2.Distance;
//		}
		g_led_blink[2]++;
	}
	if(RxMessage.StdId == 3)			//后1磁导航
	{
		for(i=0;i<RxMessage.DLC;i++)
		g_cinavi3_RXbuff[i]=RxMessage.Data[i]; 
		g_CDH8_hou_1 = FindSpace1(g_cinavi3_RXbuff[0],8,g_AGV_Car_fencha_dir);
//		if(g_CDH8_qian_2.Distance)	//非0存
//		{
//			g_Ct_chugui[1] = g_CDH8_qian_2.Distance;
//		}
		g_led_blink[3]++;
	}
	if(RxMessage.StdId == 4)			//后2磁导航
	{
		for(i=0;i<RxMessage.DLC;i++)
		g_cinavi4_RXbuff[i]=RxMessage.Data[i]; 
		g_CDH8_hou_2 = FindSpace1(g_cinavi4_RXbuff[0],8,g_AGV_Car_fencha_dir);
//		if(g_CDH8_qian_2.Distance)	//非0存
//		{
//			g_Ct_chugui[1] = g_CDH8_qian_2.Distance;
//		}
		g_led_blink[4]++;
	}
	

	if(g_led_blink[1] > 50)
	{
		g_led_blink[1] = 0;	
		LED2 = 0;
	}
	if(g_led_blink[2] > 100)
	{
		g_led_blink[2] = 0;
		LED2 = 1;
	}
	
	if(g_led_blink[3] > 50)
	{
		g_led_blink[3] = 0;	
		LED3 = 0;
	}
	if(g_led_blink[4] > 100)
	{
		g_led_blink[4] = 0;
		LED3 = 1;
	}
}
#endif

u16 g_SD_buff[16];


#if CAN2_RX0_INT_ENABLE	//使能RX0中断    
//中断服务函数			    
void CAN2_RX0_IRQHandler(void)  //地标传感器 中断处理
{
  	CanRxMsg RxMessage;
	int j=0;
    CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);
	
	if(RxMessage.StdId == 16)			//地标传感器ID: 16
	{
		for(j=0;j<RxMessage.DLC;j++)
		g_Get_RFID_buff[j]=RxMessage.Data[j]; 
		g_AGV_RFID_ID = g_Get_RFID_buff[1]<<8|g_Get_RFID_buff[0];
		g_flag_RFID_beep = 1;
		
	}  	

	//0:   //0:停止1:前进 2:后退 3:左转 4:右转 5左上 6右上 7左下 8右下 9左旋 10右旋 
	//1:   //速度 0-100
	//2:   //按键

	if(RxMessage.StdId == 2)			//手持摇杆遥控器  //10ms一次
	{
		for(j=0;j<RxMessage.DLC;j++)
		g_SD_buff[j]=RxMessage.Data[j]; 
		
		g_AGV_shoudong_dir 	 = g_SD_buff[0];
		g_AGV_shoudong_Speed = g_SD_buff[1] * 30 * g_AGV_shoudong_Speed_bili/10;		//0-100*30//0-3000
		
		
	}  	
	
	
}
#endif
//		//解析地标  --  旧版
//		//rece_buf
//		if(rece2_index>0)
//		{
//			p=strstr(rece_buf,l);
//			if(p)
//			{
//				dst[0]=p[15];
//				dst[1]=p[16];
//				dst[2]=p[17];
//				for(i=0;i<rece2_index;i++)
//				rece_buf[i]=0;
//				rece2_index=0;
//				g_AGV_RFID_ID=(dst[0]-0x30)*100+(dst[1]-0x30)*10+(dst[2]-0x30);
//				PLC_Data[1]=g_AGV_RFID_ID;
//			}
//		} 	





//can发送一组数据(固定格式:ID为0X12,标准帧,数据帧)	
//len:数据长度(最大为8)				     
//msg:数据指针,最大为8个字节.
//返回值:0,成功;
//		 其他,失败;
u8 CAN1_Send_Msg(u16 ID,char* msg,u8 len)
{	
  u8 mbox;
  u16 i=0;
  CanTxMsg TxMessage;
  TxMessage.StdId=ID;	 // 标准标识符为0
  TxMessage.ExtId=ID;	 // 设置扩展标示符（29位）
  TxMessage.IDE=0;		  // 使用扩展标识符
  TxMessage.RTR=0;		  // 消息类型为数据帧，一帧8位
  TxMessage.DLC=len;							 // 发送两帧信息
  for(i=0;i<len;i++)
  TxMessage.Data[i]=msg[i];				 // 第一帧信息          
  mbox= CAN_Transmit(CAN1, &TxMessage);   
  i=0;
  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//等待发送结束
  if(i>=0XFFF)return 1;
  return 0;		

}
//can口接收数据查询
//buf:数据缓存区;	 
//返回值:0,无数据被收到;
//		 其他,接收的数据长度;
//u8 CAN1_Receive_Msg(void)
//{		   		   
// 	u32 i;
//	CanRxMsg RxMessage;
//    if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;		//没有接收到数据,直接退出 
//	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//读取数据	
//	
//	if(RxMessage.StdId == 5)
//	{
//		for(i=0;i<RxMessage.DLC;i++)
//		g_cinavi5_RXbuff[i]=RxMessage.Data[i];
//		
//	}   
//	if(RxMessage.StdId == 6)
//	{
//		for(i=0;i<RxMessage.DLC;i++)
//		g_cinavi6_RXbuff[i]=RxMessage.Data[i]; 
//	}
//	
//	return RxMessage.DLC;	
//}

//can发送一组数据(固定格式:ID为0X12,标准帧,数据帧)	
//len:数据长度(最大为8)				     
//msg:数据指针,最大为8个字节.
//返回值:0,成功;
//		 其他,失败;
u8 CAN2_Send_Msg(u8* msg,u8 len)
{	
  u8 mbox;
  u16 i=0;
  CanTxMsg TxMessage;
  TxMessage.StdId=0x12;	 // 标准标识符为0
  TxMessage.ExtId=0x12;	 // 设置扩展标示符（29位）
  TxMessage.IDE=0;		  // 使用扩展标识符
  TxMessage.RTR=0;		  // 消息类型为数据帧，一帧8位
  TxMessage.DLC=len;							 // 发送两帧信息
  for(i=0;i<len;i++)
  TxMessage.Data[i]=msg[i];				 // 第一帧信息          
  mbox= CAN_Transmit(CAN2, &TxMessage);   
  i=0;
  while((CAN_TransmitStatus(CAN2, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//等待发送结束
  if(i>=0XFFF)return 1;
  return 0;		

}
//can口接收数据查询
//buf:数据缓存区;	 
//返回值:0,无数据被收到;
//		 其他,接收的数据长度;
u8 CAN2_Receive_Msg(u8 *buf)
{		   		   
 	u32 i;
	CanRxMsg RxMessage;
    if( CAN_MessagePending(CAN2,CAN_FIFO0)==0)return 0;		//没有接收到数据,直接退出 
	

	CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);//读取数据	
    for(i=0;i<RxMessage.DLC;i++)
    buf[i]=RxMessage.Data[i];  
	//LED4 = !LED4;
	return RxMessage.DLC;	
}


char CanOpen_buff[8];

void AGV_CanOpen_Send(void)		//换到 CAN2
{
	
	CanOpen_buff[0] = 1;
	CanOpen_buff[1] = 0;
	

	CAN1_Send_Msg(0,CanOpen_buff,2);
	delay_rtos(0,0,0,20);


}

void AGV_CanOpen_Send1(void)
{
	
	CanOpen_buff[0] = 6;
	CanOpen_buff[1] = 0;
	CanOpen_buff[2] = 0;
	CanOpen_buff[3] = 0;	
	CanOpen_buff[4] = 0;
	CanOpen_buff[5] = 0;	

	CAN1_Send_Msg(0x201,CanOpen_buff,6);
	delay_rtos(0,0,0,20);
	CAN1_Send_Msg(0x202,CanOpen_buff,6);
	delay_rtos(0,0,0,20);
	CAN1_Send_Msg(0x203,CanOpen_buff,6);
	delay_rtos(0,0,0,20);
	CAN1_Send_Msg(0x204,CanOpen_buff,6);
	delay_rtos(0,0,0,20);


}

void AGV_CanOpen_Send2(void)
{
	
	CanOpen_buff[0] = 7;
	CanOpen_buff[1] = 0;
	CanOpen_buff[2] = 0;
	CanOpen_buff[3] = 0;	
	CanOpen_buff[4] = 0;
	CanOpen_buff[5] = 0;	

	CAN1_Send_Msg(0x201,CanOpen_buff,6);
	delay_rtos(0,0,0,20);
	CAN1_Send_Msg(0x202,CanOpen_buff,6);
	delay_rtos(0,0,0,20);
	CAN1_Send_Msg(0x203,CanOpen_buff,6);
	delay_rtos(0,0,0,20);
	CAN1_Send_Msg(0x204,CanOpen_buff,6);
	delay_rtos(0,0,0,20);

}
//u8 g_flag_Motec_init;
void Motec_init(void)		
{

	delay_rtos(0,0,2,100);
	AGV_CanOpen_Send();
	AGV_CanOpen_Send1();
	AGV_CanOpen_Send2();
		
	
	//g_flag_Motec_init = 1;		//初始化完成标志	

}

//速度
void AGV_CanOpen_Send3(u8 node_id,int speed) 
{
	u16 temp_COB_ID=0;
	
	if(node_id > 127)					//节点ID不能超过127
	{
		node_id = 127;
	}
	temp_COB_ID = 0x200+node_id;		//11位COB_ID
	
	
	
	CanOpen_buff[0] = 0x0f;
	CanOpen_buff[1] = 0;
	CanOpen_buff[2] = speed;
	CanOpen_buff[3] = speed>>8;	
	

	CAN1_Send_Msg(temp_COB_ID,CanOpen_buff,4);
	delay_rtos(0,0,0,20);								//发送延时motec驱动发送延时
}







