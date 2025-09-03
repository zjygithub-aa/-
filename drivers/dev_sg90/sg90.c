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
    TIM_OCInitStructure.TIM_Pulse = 150;  // 初始90°
    
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

// 坐下：所有腿降至0°
void Sit(void)
{
    Servo_SetAngle(1, 0);  // 前左
    Servo_SetAngle(2, 0);  // 前 右
    Servo_SetAngle(3, 0);  // 后左
    Servo_SetAngle(4, 0);  // 后右
    
}
	

// 站立：所有腿升至90°
void Stand(void)
{
    Servo_SetAngle(1, 90);  // 前左
    Servo_SetAngle(2, 90);  // 前右
    Servo_SetAngle(3, 90);  // 后左
    Servo_SetAngle(4, 90);  // 后右
  
}

// 前奔：trot步态（对角腿同步）
void RunForward(uint8_t step)
{
	if (step == 0) {
        // 阶段1: 腿1和腿4抬腿 (90°), 腿2和腿3落地 (0°)
        Servo_SetAngle(1, 90);
        Servo_SetAngle(2, 0);
        Servo_SetAngle(3, 0);
        Servo_SetAngle(4, 90);
    } else {
        // 阶段2: 腿2和腿3抬腿 (90°), 腿1和腿4落地 (0°)
        Servo_SetAngle(1, 0);
        Servo_SetAngle(2, 90);
        Servo_SetAngle(3, 90);
        Servo_SetAngle(4, 0);
    }
		
	}


// 后退：反向trot步态
void RunBackward(uint8_t step)
{
	if (step == 0) {
        // 阶段1: 腿2和腿3抬腿
        Servo_SetAngle(1, 0);
        Servo_SetAngle(2, 90);
        Servo_SetAngle(3, 90);
        Servo_SetAngle(4, 0);
    } else {
        // 阶段2: 腿1和腿4抬腿
        Servo_SetAngle(1, 90);
        Servo_SetAngle(2, 0);
        Servo_SetAngle(3, 0);
        Servo_SetAngle(4, 90);
    }
}
