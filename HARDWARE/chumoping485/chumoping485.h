#ifndef _CHUMOPING485_H
#define _CHUMOPING485_H

#include "my_include.h"






extern u8 rece_index;


void USART4_Configuration(u32 bound);    //485--´¥ÃþÆÁÓÃµÄ´®¿Ú4

void DMA_Uart4_Init(void);

void Uart4_Start_DMA_Tx(u16 size);












#endif

