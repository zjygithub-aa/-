#include "pd333.h"
#include "systick.h"
#include <stdio.h>

#define ADC_CHANNEL ADC_Channel_1  // PA1 for PD333-3C/H0/L2
uint16_t adc_value = 0;
void Flame_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

    // PF5  ‰»Î
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;  // …œ¿≠
    GPIO_Init(GPIOF, &GPIO_InitStruct);
}

void Flame_Check(void)
{
    uint8_t flame_detected = (GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_5) == Bit_RESET);

    if (flame_detected) {
        printf("ª—ÊºÏ≤‚µΩ!\r\n");
        GPIO_SetBits(GPIOA, GPIO_Pin_15); // ∑‰√˘∆˜
        systick_ms(500);
        GPIO_ResetBits(GPIOA, GPIO_Pin_15);
        OLED_ShowString(0, 0, "Flame Detected!", 16, 1);
    } else {
        printf("Œﬁª—Ê.\r\n");
        OLED_ShowString(0, 0, "No Flame       ", 16, 1);
    }
    OLED_Refresh();
}
