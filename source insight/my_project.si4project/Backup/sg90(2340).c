#include "sg90.h"
void TIM3_PWM_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // ʹ��ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  // TIM3ʱ��ʹ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); // GPIOCʱ��ʹ��
    
    // ����PC6ΪTIM3_CH1���ù���
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;         // ���ù���
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   // ����
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;       // �������
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;         // ����
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    // ����PC6��TIM3_CH1
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
    
    // ����TIM3ʱ������
    TIM_TimeBaseStructure.TIM_Prescaler = 839;           // Ԥ��Ƶ����84MHz/(839+1)=100kHz
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // ���ϼ���
    TIM_TimeBaseStructure.TIM_Period = 1999;             // �Զ���װ��ֵ��100kHz*(1999+1)=50Hz(20ms)
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;          // ʱ�ӷ�Ƶ
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;      // �ظ�����
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    
    // ����PWMģʽ
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;     // PWMģʽ1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // ���ʹ��
    TIM_OCInitStructure.TIM_Pulse = 150;                  // ��ʼCCRֵ(90��)
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // ������Ը�
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);              // ����CH1
    
    // ʹ��Ԥ����
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM3, ENABLE);
    
    // ������ʱ��
    TIM_Cmd(TIM3, ENABLE);
}
// ���ö���Ƕ�(0-180��)
void Servo_SetAngle(uint8_t angle)
{
    uint16_t ccr;
    
    // ���ƽǶȷ�Χ
    if(angle > 180) angle = 180;
    if(angle < 0) angle = 0;
    
    // �Ƕ�תCCRֵ: 0.5ms(50) ~ 2.5ms(250)
    ccr = 50 + (angle * 200) / 180;
    
    // ���ñȽ�ֵ
    TIM_SetCompare1(TIM3, ccr);
}
