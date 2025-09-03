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
void delay_us(u32 nus)
{
    u32 ticks;
    u32 told,tnow,reload,tcnt=0;

    reload = SysTick->LOAD;                     //获取重装载寄存器值
    ticks = nus * (SystemCoreClock / 1000000);  //计数时间值   括号里的代表1us秒嘀嗒定时器的value会向下降多少值
    told=SysTick->VAL;                          //获取当前数值寄存器值（开始时数值）

    while(1)
    {
          tnow=SysTick->VAL;          //获取当前数值寄存器值
          if(tnow!=told)              //当前值不等于开始值说明已在计数
          {         

                 if(tnow<told)             //当前值小于开始数值，说明未计到0
                      tcnt+=told-tnow;     //计数值=开始值-当前值

                 else                  //当前值大于开始数值，说明已计到0并重新计数
                        tcnt+=reload-tnow+told;   //计数值=重装载值-当前值+开始值  （已
                                                  //从开始值计到0） 
                 told=tnow;                //更新开始值
                 if(tcnt>=ticks)break;     //时间超过/等于要延迟的时间,则退出.
          } 
    }     
}
