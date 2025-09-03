#include "systick.h"

static uint16_t fs_us = 0;
static uint16_t fs_ms = 0;

void systick_init(uint8_t systick)
{
    SysTick->CTRL &= ~0x05;  // �ر� SysTick
    fs_us = systick / 8;     // ����ÿ΢���ʱ��������
    fs_ms = fs_us * 1000;    // ����ÿ�����ʱ��������
}

void systick_ms(uint16_t ms)
{
    uint32_t temp = 0;

    // �����ʱ����ֹ���� SysTick ���ֵ��16777215��
    while (ms > 0)
    {
        uint32_t delay = fs_ms * ms;

        if (delay > 16777215)  // �����ʱ���� SysTick ���ֵ
        {
            SysTick->LOAD = 16777215;  // ����Ϊ���ֵ
            ms -= 16777215 / fs_ms;    // ��ȥ�Ѵ������ʱ
        }
        else
        {
            SysTick->LOAD = delay;  // ����Ϊʣ����ʱ
            ms = 0;                 // ��ʱ���
        }

        SysTick->VAL = 0;  // ��յ�ǰ������
        SysTick->CTRL |= 0x01;  // ������ʱ��

        // �ȴ���ʱ���
        do
        {
            temp = SysTick->CTRL;
        } while (!(temp & (0x01 << 16)));  // �ȴ������־

        SysTick->CTRL &= ~0x01;  // �رռ�ʱ��
    }
}


void systick_us(uint32_t us)
{
    uint32_t temp = 0;

    // �����ʱ����ֹ���� SysTick ���ֵ��16777215��
    while (us > 0)
    {
        uint32_t delay = fs_us * us;

        if (delay > 16777215)  // �����ʱ���� SysTick ���ֵ
        {
            SysTick->LOAD = 16777215;  // ����Ϊ���ֵ
            us -= 16777215 / fs_us;    // ��ȥ�Ѵ������ʱ
        }
        else
        {
            SysTick->LOAD = delay;  // ����Ϊʣ����ʱ
            us = 0;                 // ��ʱ���
        }

        SysTick->VAL = 0;  // ��յ�ǰ������
        SysTick->CTRL |= 0x01;  // ������ʱ��

        // �ȴ���ʱ���
        do
        {
            temp = SysTick->CTRL;
        } while (!(temp & (0x01 << 16)));  // �ȴ������־

        SysTick->CTRL &= ~0x01;  // �رռ�ʱ��
    }
}
void delay_us(u32 nus)
{
    u32 ticks;
    u32 told,tnow,reload,tcnt=0;

    reload = SysTick->LOAD;                     //��ȡ��װ�ؼĴ���ֵ
    ticks = nus * (SystemCoreClock / 1000000);  //����ʱ��ֵ   ������Ĵ���1us����શ�ʱ����value�����½�����ֵ
    told=SysTick->VAL;                          //��ȡ��ǰ��ֵ�Ĵ���ֵ����ʼʱ��ֵ��

    while(1)
    {
          tnow=SysTick->VAL;          //��ȡ��ǰ��ֵ�Ĵ���ֵ
          if(tnow!=told)              //��ǰֵ�����ڿ�ʼֵ˵�����ڼ���
          {         

                 if(tnow<told)             //��ǰֵС�ڿ�ʼ��ֵ��˵��δ�Ƶ�0
                      tcnt+=told-tnow;     //����ֵ=��ʼֵ-��ǰֵ

                 else                  //��ǰֵ���ڿ�ʼ��ֵ��˵���ѼƵ�0�����¼���
                        tcnt+=reload-tnow+told;   //����ֵ=��װ��ֵ-��ǰֵ+��ʼֵ  ����
                                                  //�ӿ�ʼֵ�Ƶ�0�� 
                 told=tnow;                //���¿�ʼֵ
                 if(tcnt>=ticks)break;     //ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
          } 
    }     
}
