#ifndef __TIME_H__
#define __TIME_H__
#include <stm32f4xx.h>
#include <stdio.h>

void Init_TIM3(void);
void delay_init(void);
void tim4_delay_ms(uint16_t ms);
void tim4_delay_us(u32 nus);
#endif

