#include "sys.h"
#include "usart.h"	
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART6->SR&0X40)==0);//循环发送,直到发送完毕   
	USART6->DR = (u8) ch;      
	return ch;
}
#endif



//void USART6_Configuration(u32 bound)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); 
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);
//	
//	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6);
//	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6);
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//	GPIO_Init(GPIOC,&GPIO_InitStructure);
//	
//	USART_InitStructure.USART_BaudRate            = bound  ;
//	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
//	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
//	USART_InitStructure.USART_Parity              = USART_Parity_No ;
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
//	USART_Init(USART6, &USART_InitStructure);
//	USART_ITConfig(USART6, USART_IT_IDLE, ENABLE);	
//	USART_Cmd(USART6, ENABLE);  
//	USART_ClearFlag(USART6, USART_FLAG_TC);
//	USART_DMACmd(USART6, USART_DMAReq_Rx, ENABLE);
////	USART_DMACmd(USART6, USART_DMAReq_Tx, ENABLE);
//}




//void DMA_Uart6_Init(void)  //锂电池电量监测232转TTL
//{
//    DMA_InitTypeDef DMA_InitStructure;  
//    /* DMA clock enable */
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

//	/*--- LUMMOD_UART_Tx_DMA_Channel DMA Config ---*/
// 
//	//DMA_Cmd(DMA2_Stream7, DISABLE);
//	//DMA_DeInit(DMA2_Stream7);
//	//DMA_InitStructure.DMA_Channel = DMA_Channel_5;
//	//DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART6->DR);
//	//DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)sendBuf;
//	//DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
//	//DMA_InitStructure.DMA_BufferSize = UART_TX_LEN;
//	//DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//	//DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//	//DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//	//DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//	//DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	
//	//DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable; 
//	//DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;	
//	//DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
//	//DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
//	//DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
//	//DMA_Init(DMA2_Stream7, &DMA_InitStructure);
//	//DMA_Cmd(DMA2_Stream7, DISABLE);
//	//DMA_ITConfig(DMA2_Stream7, DMA_IT_TC, ENABLE);

//	///*--- LUMMOD_UART_Rx_DMA_Channel DMA Config ---*/
// 
//    DMA_Cmd(DMA2_Stream1, DISABLE);
//    DMA_DeInit(DMA2_Stream1);
//	DMA_InitStructure.DMA_Channel = DMA_Channel_5;
//    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART6->DR);
//    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)g_ethernet_RXbuff;
//    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory ;
//    DMA_InitStructure.DMA_BufferSize = UART6_RX_LEN;
//    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
//	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable; 
//    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;	
//    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
//	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
//	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
//    DMA_Init(DMA2_Stream1, &DMA_InitStructure);
//    DMA_Cmd(DMA2_Stream1, ENABLE);   
//}
////void Uart6_Start_DMA_Tx(u16 size)
////{
////    DMA_SetCurrDataCounter(DMA2_Stream7,size);
////    DMA_Cmd(DMA2_Stream7, ENABLE);
////}










//void USART6_IRQHandler(void)         	//串口TTL通信接口,哈工大目前不用
//{
//#if SYSTEM_SUPPORT_OS 	
//	OSIntEnter();    
//#endif
//	
//	if(USART_GetITStatus(USART6, USART_IT_IDLE) != RESET) 
//	{
//		DMA_Cmd(DMA2_Stream1,DISABLE);
//		rece6_index = USART6->SR;
//		rece6_index = USART6->DR; //清除IDLE标志
//		rece6_index = UART6_RX_LEN - DMA_GetCurrDataCounter(DMA2_Stream1); 
//		DMA2_Stream1->NDTR = UART6_RX_LEN;
//		
//		AnalyzeDbus();   
//		
//		DMA_Cmd(DMA2_Stream1, ENABLE);
//	} 
//	
//#if SYSTEM_SUPPORT_OS 	
//	OSIntExit();  											 
//#endif
//}


void USART_OUT(USART_TypeDef* USARTx, char *Data,uint8_t len)
{ 
	USART_GetFlagStatus(USARTx, USART_FLAG_TC);
	while(len--)
		{				                          //判断是否到达字符串结束符
			USART_SendData(USARTx, *Data++);
		  while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
	  }
}





















