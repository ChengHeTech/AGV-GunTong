#include "rgb.h"




//PB10 -- ��ɫ -- ����   (4���Ƶ��źŲ���һ��)
//PB0  -- ��ɫ -- ����	(4���Ƶ��źŲ���һ��)
//�ߵ�ƽ���� -- ��̬����
void rgb_init(void) //ǣ���˿���--PB10--�ĳ�ʼ��Ҳ������
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��	
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_10;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	GPIO_ResetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_10);
	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_8;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	//GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
	//GPIO_SetBits(GPIOA,GPIO_Pin_15 | GPIO_Pin_8);



}