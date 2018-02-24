#include "battery.h"


char g_battery_RXbuff[g_battery_RXbuff_len];
char g_battery_TXbuff[g_battery_TXbuff_len];



void USART3_Configuration(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); //使能GPIOD时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能USART3时钟

	//串口3对应引脚复用映射
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_USART1); //GPIOD8复用为USART3
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_USART1); //GPIOD9复用为USART3

	//USART3端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9; //GPIOD8与GPIOD9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        //速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOD,&GPIO_InitStructure); //初始化PD8，PD9

	//USART3 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;        //收发模式
	USART_Init(USART3, &USART_InitStructure); //初始化串口3
	
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
	USART_Cmd(USART3, ENABLE);  //使能串口1 
	USART_ClearFlag(USART1, USART_FLAG_TC);
	USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);
	USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);
	
   
}


void USART3_IRQHandler(void)             
{
	u8 temp_data3=0;
#if SYSTEM_SUPPORT_OS 	
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET) 
	{
		DMA_Cmd(DMA1_Stream1,DISABLE);
		temp_data3 = USART3->SR;
		temp_data3 = USART3->DR; //清除IDLE标志
		temp_data3 = g_battery_RXbuff_len - DMA_GetCurrDataCounter(DMA1_Stream1); 
		DMA1_Stream1->NDTR = g_battery_RXbuff_len;
		
//		if((g_battery_RXbuff[0] == 0x7E)&&(g_battery_RXbuff[127] == 0x0D))
//		{
//			lidian.voltage = (HexToChar(g_battery_RXbuff[101])<<12)|(HexToChar(g_battery_RXbuff[102])<<8)|(HexToChar(g_battery_RXbuff[103])<<4)|(HexToChar(g_battery_RXbuff[104]));
//			lidian.MAh 	= (HexToChar(g_battery_RXbuff[105])<<12)|(HexToChar(g_battery_RXbuff[106])<<8)|(HexToChar(g_battery_RXbuff[107])<<4)|(HexToChar(g_battery_RXbuff[108]));
//			lidian.Current = (HexToChar(g_battery_RXbuff[97])<<12)|(HexToChar(g_battery_RXbuff[98])<<8)|(HexToChar(g_battery_RXbuff[99])<<4)|(HexToChar(g_battery_RXbuff[100]));
//			DianChiDianLiang = lidian.MAh;
//			DianChiDianLiu 	= lidian.Current;
//			
//		}		
		
		DMA_Cmd(DMA1_Stream1, ENABLE);
	}
	temp_data3 = temp_data3;
#if SYSTEM_SUPPORT_OS 	
	OSIntExit();  											 
#endif
}




void DMA_Uart3_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;	
    DMA_InitTypeDef DMA_InitStructure;  
    /* DMA clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

	/*--- LUMMOD_UART_Tx_DMA_Channel DMA Config ---*/
 
    DMA_Cmd(DMA1_Stream3, DISABLE);
    DMA_DeInit(DMA1_Stream3);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART3->DR);
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)g_battery_TXbuff;         //发送buff
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize = g_battery_TXbuff_len;					 //发送buff长度
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable; 
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;	
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
    DMA_Init(DMA1_Stream3, &DMA_InitStructure);
	DMA_Cmd(DMA1_Stream3, DISABLE);
    DMA_ITConfig(DMA1_Stream3, DMA_IT_TC, ENABLE);
	
/*--- LUMMOD_UART_Rx_DMA_Channel DMA Config ---*/
 
    DMA_Cmd(DMA1_Stream1, DISABLE);
    DMA_DeInit(DMA1_Stream1);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART3->DR);
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)g_battery_RXbuff;			//接收buff
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory ;
    DMA_InitStructure.DMA_BufferSize = g_battery_RXbuff_len;					//接收buff长度
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable; 
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;	
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
    DMA_Init(DMA1_Stream1, &DMA_InitStructure);
    DMA_Cmd(DMA1_Stream1, ENABLE); 
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream3_IRQn;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
}
void Uart3_Start_DMA_Tx(u16 size)
{
    DMA_SetCurrDataCounter(DMA1_Stream3,size);
    DMA_Cmd(DMA1_Stream3, ENABLE);
}






