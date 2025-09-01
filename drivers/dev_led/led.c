#include "led.h"

void Init_led(void)
{
	
		GPIO_InitTypeDef GPIO_InitStruct;
	//ʱ��ʹ�� 
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	//��ʼ������	
		GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_8;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOF,&GPIO_InitStruct);
		//Ĭ�Ϲر�
		GPIO_SetBits(GPIOF,GPIO_Pin_8);
}

void led_open(void)
{
	GPIO_ResetBits(GPIOF,GPIO_Pin_8);
}

void led_close(void)
{
	GPIO_SetBits(GPIOF,GPIO_Pin_8);
}
