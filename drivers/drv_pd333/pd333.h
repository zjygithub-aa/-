#ifndef __PD333_H
#define __PD333_H

#include "stm32f4xx.h"
#include "oled.h" // 包含 OLED 头文件

void Flame_Init(void);
void Flame_Check(void); // 封装火焰检测、蜂鸣器、OLED、串口输出

#endif
