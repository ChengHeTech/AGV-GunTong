#ifndef _BATEERY_H
#define _BATEERY_H

#include "my_include.h"

#define g_battery_RXbuff_len 250
#define g_battery_TXbuff_len 250







void USART3_Configuration(u32 bound);

void DMA_Uart3_Init(void);  //锂电池电量监测232转TTL

void Uart3_Start_DMA_Tx(u16 size);









#endif





