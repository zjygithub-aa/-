#include "time.h"


//ʵ����TIM3���Ʒ���ת�ٵ�λ��������Ҫ��ʼ���������Ϊ���sg90��ʹ�õ�TIM3��ʱ������ѵ����
void Init_TIM3(void)
{ GPIO_InitTypeDef GPIO_InitStruct;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_OCInitTypeDef TIM_OCInitStruct;

    // ����ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);


    // ����PC6�����ȣ�TIM3��
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);

    // ��ʼ��TIM3�����ȣ�
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period = 5000 - 1; // 2kHz PWM
    TIM_TimeBaseInitStruct.TIM_Prescaler = 84 - 1;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);


    // TIM3 PWM���ã����ȣ�ͨ��1��
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High; // ���ݷ��ȵ���
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_Pulse = 0;
    TIM_OC1Init(TIM3, &TIM_OCInitStruct);

    // ����Ԥ���غ�PWM���
    TIM_ARRPreloadConfig(TIM3, ENABLE);
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_CtrlPWMOutputs(TIM3, ENABLE);

    // ������ʱ��
    TIM_Cmd(TIM3, ENABLE);

}


void delay_init(void) 
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    
    TIM_TimeBaseStructure.TIM_Period = 83;
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
}                                    
 
void tim4_delay_ms(uint16_t ms)
{
    while(ms > 0)
    {
        uint32_t chunk = (ms > 65) ? 65 : ms; // �������65ms����ֹuint16_t�����
        
        // ����1us�ж�һ�Σ��ۼ�chunk*1000��
        TIM_SetAutoreload(TIM4, 84 - 1);
        TIM_SetCounter(TIM4, 0);
        TIM_Cmd(TIM4, ENABLE);
        
        for(uint32_t i = 0; i < chunk * 1000; i++)
        {
            while(!TIM_GetFlagStatus(TIM4, TIM_FLAG_Update));
            TIM_ClearFlag(TIM4, TIM_FLAG_Update);
        }
        
        TIM_Cmd(TIM4, DISABLE);
        ms -= chunk;
    }
}   

//��ʱnus
//nusΪҪ��ʱ��us��.                                               
void tim4_delay_us(u32 nus)
{
    u32 cnt;
    cnt = nus;
    TIM_Cmd(TIM4, ENABLE);
    while(cnt--)
    {
        while(TIM_GetFlagStatus(TIM4, TIM_FLAG_Update) == RESET);
        TIM_ClearFlag(TIM4, TIM_FLAG_Update);
    }
    TIM_Cmd(TIM4, DISABLE);
}
