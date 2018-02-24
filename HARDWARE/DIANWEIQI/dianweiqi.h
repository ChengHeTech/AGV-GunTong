#ifndef _DIANWEIQI_H
#define _DIANWEIQI_H


#include "my_include.h"

#define ADC1_DR_Addr   ((uint32_t)0x4001204C)


typedef struct
{
	u16 qianlun_zhong_val;	//1.05
	u16 qianlun_L_val;		//0.60
	u16 qianlun_R_val;		//1.70

	u16 houlun_zhong_val;	//1.05
	u16 houlun_L_val;		//
	u16 houlun_R_val;		//

}dianweiqi;





//电位器0对应前驱动
//电位器1对应后驱动
extern dianweiqi g_DWQ;







extern u16 g_AD_Value[30][3];
extern double g_ADC_dianweiqi[2];

extern u16 g_After_filter[3];

extern double g_dianweiqi_zhong ;



void ADC1_Configuration(void); 

void DMA_adc1_Init(void);













#endif


