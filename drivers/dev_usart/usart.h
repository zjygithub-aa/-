#ifndef __USART_H__
#define __USART_H__
#include <stm32f4xx.h>
#include <stdio.h>
#include <string.h>
void Init_usart2(void);
#define MAX_SIZE 100

extern uint8_t Data[MAX_SIZE];
extern  uint8_t usart_index;
extern  uint8_t flag;
#endif





