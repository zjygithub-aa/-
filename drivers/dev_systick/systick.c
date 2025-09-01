#include "systick.h"

static uint16_t fs_us = 0;
static uint16_t fs_ms = 0;

void systick_init(uint8_t systick)
{
    SysTick->CTRL &= ~0x05;  // 关闭 SysTick
    fs_us = systick / 8;     // 计算每微秒的时钟周期数
    fs_ms = fs_us * 1000;    // 计算每毫秒的时钟周期数
}

void systick_ms(uint16_t ms)
{
    uint32_t temp = 0;

    // 拆分延时，防止超过 SysTick 最大值（16777215）
    while (ms > 0)
    {
        uint32_t delay = fs_ms * ms;

        if (delay > 16777215)  // 如果延时大于 SysTick 最大值
        {
            SysTick->LOAD = 16777215;  // 设置为最大值
            ms -= 16777215 / fs_ms;    // 减去已处理的延时
        }
        else
        {
            SysTick->LOAD = delay;  // 设置为剩余延时
            ms = 0;                 // 延时完成
        }

        SysTick->VAL = 0;  // 清空当前计数器
        SysTick->CTRL |= 0x01;  // 启动计时器

        // 等待延时完成
        do
        {
            temp = SysTick->CTRL;
        } while (!(temp & (0x01 << 16)));  // 等待溢出标志

        SysTick->CTRL &= ~0x01;  // 关闭计时器
    }
}


void systick_us(uint32_t us)
{
    uint32_t temp = 0;

    // 拆分延时，防止超过 SysTick 最大值（16777215）
    while (us > 0)
    {
        uint32_t delay = fs_us * us;

        if (delay > 16777215)  // 如果延时大于 SysTick 最大值
        {
            SysTick->LOAD = 16777215;  // 设置为最大值
            us -= 16777215 / fs_us;    // 减去已处理的延时
        }
        else
        {
            SysTick->LOAD = delay;  // 设置为剩余延时
            us = 0;                 // 延时完成
        }

        SysTick->VAL = 0;  // 清空当前计数器
        SysTick->CTRL |= 0x01;  // 启动计时器

        // 等待延时完成
        do
        {
            temp = SysTick->CTRL;
        } while (!(temp & (0x01 << 16)));  // 等待溢出标志

        SysTick->CTRL &= ~0x01;  // 关闭计时器
    }
}
