#include "sg90.h"
void TIM3_PWM_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 使能时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  // TIM3时钟使能
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); // GPIOC时钟使能
    
    // 配置PC6为TIM3_CH1复用功能
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;         // 复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   // 高速
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;       // 推挽输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;         // 上拉
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    // 连接PC6到TIM3_CH1
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
    
    // 配置TIM3时基参数
    TIM_TimeBaseStructure.TIM_Prescaler = 839;           // 预分频器，84MHz/(839+1)=100kHz
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数
    TIM_TimeBaseStructure.TIM_Period = 1999;             // 自动重装载值，100kHz*(1999+1)=50Hz(20ms)
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;          // 时钟分频
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;      // 重复计数
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    
    // 配置PWM模式
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;     // PWM模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 输出使能
    TIM_OCInitStructure.TIM_Pulse = 150;                  // 初始CCR值(90°)
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // 输出极性高
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);              // 配置CH1
    
    // 使能预加载
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM3, ENABLE);
    
    // 启动定时器
    TIM_Cmd(TIM3, ENABLE);
}
// 设置舵机角度(0-180°)
void Servo_SetAngle(uint8_t angle)
{
    uint16_t ccr;
    
    // 限制角度范围
    if(angle > 180) angle = 180;
    if(angle < 0) angle = 0;
    
    // 角度转CCR值: 0.5ms(50) ~ 2.5ms(250)
    ccr = 50 + (angle * 200) / 180;
    
    // 设置比较值
    TIM_SetCompare1(TIM3, ccr);
}
