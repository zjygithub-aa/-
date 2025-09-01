#ifndef __SG90_H__
#define __SG90_H__
#include <stm32f4xx.h>
#include "systick.h"
#include <stdio.h>



void TIM3_PWM_Init(void);
void Servo_SetAngle(uint8_t channel, uint8_t angle);
void Sit(void);
void Stand(void);
void RunForward(void);
void RunBackward(void);

#endif
