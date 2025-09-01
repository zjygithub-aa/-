#ifndef __SYSTICK_H__
#define __SYSTICK_H__
#include <stm32f4xx.h>

void systick_init(uint8_t systick);
void systick_ms(uint16_t ms);
void systick_us(uint32_t us);
	
#endif
