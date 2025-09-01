#include "key.h"

//初始化四个按键，KEY1->PC9 KEY2->PC8 KEY3->PC3 USER->PF9
void Init_key(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_5;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
		
	GPIO_Init(GPIOC,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOF,&GPIO_InitStruct);

}
//马达->PC7 风扇->PC6
void Init_motor_fan(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_6;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
		
	GPIO_Init(GPIOC,&GPIO_InitStruct);

	GPIO_ResetBits(GPIOC,GPIO_Pin_7);
	GPIO_ResetBits(GPIOC,GPIO_Pin_6);
	

}

//按键检测
void key_check(void)
{
	if((GPIOC->IDR & GPIO_Pin_9)==0)
	{
		systick_ms(100);
		if((GPIOC->IDR & GPIO_Pin_9)==0)
		{
			GPIO_ToggleBits(GPIOA,GPIO_Pin_15);
		}	
	}
		if((GPIOC->IDR & GPIO_Pin_9)==0)
	{
		systick_ms(100);
		if((GPIOC->IDR & GPIO_Pin_9)==0)
		{
			GPIO_ToggleBits(GPIOA,GPIO_Pin_15);
		}	
	}
		if((GPIOC->IDR & GPIO_Pin_9)==0)
	{
		systick_ms(100);
		if((GPIOC->IDR & GPIO_Pin_9)==0)
		{
			GPIO_ToggleBits(GPIOA,GPIO_Pin_15);
		}	
	}
		if((GPIOC->IDR & GPIO_Pin_9)==0)
	{
		systick_ms(100);
		if((GPIOC->IDR & GPIO_Pin_9)==0)
		{
			GPIO_ToggleBits(GPIOA,GPIO_Pin_15);
		}	
	}


}
