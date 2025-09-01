#include "sg90.h"

void Init_TIM2_Servo(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_OCInitTypeDef TIM_OCInitStruct;

    // 启用 TIM2 和 GPIOA 时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // 配置 PA0 (TIM2_CH1) 为复用推挽输出
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM2);

    // 配置 TIM2 基本参数 (50Hz, 20ms 周期)
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period = 5000 - 1; 
    TIM_TimeBaseInitStruct.TIM_Prescaler = 84-1;     
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);

    // 配置 PWM 模式 (通道 1)
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_Pulse = 0; // 初始脉宽 1.5ms (90°)
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM2, &TIM_OCInitStruct);

    // 启用预加载和 PWM 输出
    TIM_ARRPreloadConfig(TIM2, ENABLE);
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_CtrlPWMOutputs(TIM2, ENABLE);

    // 启动定时器
    TIM_Cmd(TIM2, ENABLE);
}

void Set_TIM2_PWM(uint16_t pulse_width_us)
{
    if (pulse_width_us < 500) pulse_width_us = 500;  // 限制最小 0.5ms
    if (pulse_width_us > 2500) pulse_width_us = 2500; // 限制最大 2.5ms
    TIM_SetCompare1(TIM2, pulse_width_us); // 设置 PWM 脉宽 (单位: us)
}