#include "sg90.h"
#include "usart.h"


void TIM3_PWM_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3);
    
    TIM_TimeBaseStructure.TIM_Prescaler = 839;  // 84MHz / 840 = 100kHz
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 1999;    // 100kHz * 2000 = 50Hz
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = 150;  // ��ʼ90��
    
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_OC3Init(TIM3, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_OC4Init(TIM3, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
    
    TIM_ARRPreloadConfig(TIM3, ENABLE);
    TIM_Cmd(TIM3, ENABLE);
}

void Servo_SetAngle(uint8_t channel, uint8_t angle)
{
    uint16_t ccr;
    
    if (angle > 180) angle = 180;
    if (angle < 0) angle = 0;
    
    ccr = 50 + (angle * 200) / 180;  // 0.5ms(50) ~ 2.5ms(250)
    
    switch (channel) {
        case 1: TIM_SetCompare1(TIM3, ccr); break;
        case 2: TIM_SetCompare2(TIM3, ccr); break;
        case 3: TIM_SetCompare3(TIM3, ccr); break;
        case 4: TIM_SetCompare4(TIM3, ccr); break;
        default: break;
    }
}

// ���£������Ƚ���0��
void Sit(void)
{
    Servo_SetAngle(1, 0);  // ǰ��
    Servo_SetAngle(2, 0);  // ǰ ��
    Servo_SetAngle(3, 0);  // ����
    Servo_SetAngle(4, 0);  // ����
    
}
	

// վ��������������90��
void Stand(void)
{
    Servo_SetAngle(1, 90);  // ǰ��
    Servo_SetAngle(2, 90);  // ǰ��
    Servo_SetAngle(3, 90);  // ����
    Servo_SetAngle(4, 90);  // ����
  
}

// ǰ����trot��̬���Խ���ͬ����
void RunForward(uint8_t step)
{
	if (step == 0) {
        // �׶�1: ��1����4̧�� (90��), ��2����3��� (0��)
        Servo_SetAngle(1, 90);
        Servo_SetAngle(2, 0);
        Servo_SetAngle(3, 0);
        Servo_SetAngle(4, 90);
    } else {
        // �׶�2: ��2����3̧�� (90��), ��1����4��� (0��)
        Servo_SetAngle(1, 0);
        Servo_SetAngle(2, 90);
        Servo_SetAngle(3, 90);
        Servo_SetAngle(4, 0);
    }
		
	}


// ���ˣ�����trot��̬
void RunBackward(uint8_t step)
{
	if (step == 0) {
        // �׶�1: ��2����3̧��
        Servo_SetAngle(1, 0);
        Servo_SetAngle(2, 90);
        Servo_SetAngle(3, 90);
        Servo_SetAngle(4, 0);
    } else {
        // �׶�2: ��1����4̧��
        Servo_SetAngle(1, 90);
        Servo_SetAngle(2, 0);
        Servo_SetAngle(3, 0);
        Servo_SetAngle(4, 90);
    }
}
