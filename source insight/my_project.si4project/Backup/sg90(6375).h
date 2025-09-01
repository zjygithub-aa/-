#ifndef __SG90_H__
#define __SG90_H__
#include <stm32f4xx.h>
#include <stdio.h>

void Init_TIM2_Servo(void);
void Set_TIM2_PWM(uint16_t pulse_width_us);

#endif