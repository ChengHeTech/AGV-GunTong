#include "chumoping485.h"







void USART4_Configuration(u32 bound)    //485--触摸屏用的串口4
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_UART4);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_UART4);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);	
	GPIO_ResetBits(GPIOA,GPIO_Pin_2);
	
	USART_InitStructure.USART_BaudRate            = bound  ;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART4, &USART_InitStructure);	
	
	USART_ITConfig(UART4, USART_IT_IDLE, ENABLE);
	USART_Cmd(UART4, ENABLE);  
	USART_ClearFlag(UART4, USART_FLAG_TC);
	USART_DMACmd(UART4, USART_DMAReq_Tx, ENABLE);
    USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE);
}


void UART4_IRQHandler(void)     		//485--触摸屏用的串口4           
{
	
#if SYSTEM_SUPPORT_OS 	
	OSIntEnter();    
#endif
	
	if(USART_GetITStatus(UART4, USART_IT_IDLE) != RESET) 
	{
		DMA_Cmd(DMA1_Stream2,DISABLE);
        rece_index = UART4->SR; 
        rece_index = UART4->DR; 
        rece_index = UART_RX_LEN - DMA_GetCurrDataCounter(DMA1_Stream2); 
        DMA1_Stream2->NDTR = UART_RX_LEN;
		if(rece_index > 0)
		{
			if(receBuf[0]==0x01||receBuf[0]==0x00)
			{
				AnalyzeRecieve();  			//分析并生成响应报文 -- modbus
				if(receBuf[0]==0x01)
				send_comm();
			}
		} 		
        DMA_Cmd(DMA1_Stream2, ENABLE);
	} 
	if(USART_GetITStatus(UART4, USART_IT_TC) != RESET)
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_2);
		NVIC_ClearPendingIRQ(UART4_IRQn);
		USART_ClearITPendingBit(UART4,USART_IT_TC);
	}
#if SYSTEM_SUPPORT_OS 	
	OSIntExit();  											 
#endif
}


void DMA_Uart4_Init(void)
{
    DMA_InitTypeDef DMA_InitStructure;  
    /* DMA clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
      
	/*--- LUMMOD_UART_Tx_DMA_Channel DMA Config ---*/
 
    DMA_Cmd(DMA1_Stream4, DISABLE);
    DMA_DeInit(DMA1_Stream4);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&UART4->DR);
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)sendBuf;           //发送buff
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize = UART_TX_LEN;								 //发送buff长度
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
    DMA_Init(DMA1_Stream4, &DMA_InitStructure);
	DMA_Cmd(DMA1_Stream4, DISABLE);
    DMA_ITConfig(DMA1_Stream4, DMA_IT_TC, ENABLE);
   
/*--- LUMMOD_UART_Rx_DMA_Channel DMA Config ---*/
 
    DMA_Cmd(DMA1_Stream2, DISABLE);
    DMA_DeInit(DMA1_Stream2);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&UART4->DR);
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)receBuf;			//接收buff
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory ;
    DMA_InitStructure.DMA_BufferSize = UART_RX_LEN;						//接收buff长度
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
    DMA_Init(DMA1_Stream2, &DMA_InitStructure);
    DMA_Cmd(DMA1_Stream2, ENABLE);   
}
void Uart4_Start_DMA_Tx(u16 size)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_2);
    DMA_SetCurrDataCounter(DMA1_Stream4,size);
    DMA_Cmd(DMA1_Stream4, ENABLE);
}

void  DMA1_Stream4_IRQHandler(void)//uart4发送DMA中断服务函数
{
    if(DMA_GetITStatus(DMA1_Stream4, DMA_IT_TCIF4))			//传输完成标志为真时
    {
			USART_ITConfig(UART4, USART_IT_TC, ENABLE);				//使能发送完成中断

			DMA_ClearITPendingBit(DMA1_Stream4, DMA_IT_TCIF4);//清传输完成标志
			DMA_Cmd(DMA1_Stream4, DISABLE);										//失能“流”
    }
}





