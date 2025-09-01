	#include "usart.h"

// 全局变量：用于存储串口接收到的数据
uint8_t Data[MAX_SIZE];         // 数据缓冲区，MAX_SIZE需在usart.h中定义
 uint8_t usart_index = 0;       // 串口数据缓冲区索引，记录当前数据存储位置
 uint8_t flag = 0;              // 标志变量，用于指示数据接收或处理状态

// 函数：初始化USART2和USART6及其相关的GPIO和中断
void Init_usart2(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    // 启用所有相关GPIO和USART外设的时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOG, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB1Periph_USART3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

    // 配置GPIOA的引脚2（TX）和引脚3（RX）为USART2
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);  // PA2 as TX
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);  // PA3 as RX

    // 配置GPIOB的引脚10（TX）和引脚11（RX）为USART3
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3); // PB10 as TX
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3); // PB11 as RX

    // 配置GPIOG的引脚9（TX）和引脚14（RX）为USART6
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_14;
    GPIO_Init(GPIOG, &GPIO_InitStruct);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource9, GPIO_AF_USART6);  // PG9 as TX
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource14, GPIO_AF_USART6); // PG14 as RX

    // 配置NVIC中断，所有串口抢占优先级和子优先级均设为1
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;

    // 配置USART2中断
    NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
    NVIC_Init(&NVIC_InitStruct);

    // 配置USART3中断
    NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;
    NVIC_Init(&NVIC_InitStruct);

    // 配置USART6中断
    NVIC_InitStruct.NVIC_IRQChannel = USART6_IRQn;
    NVIC_Init(&NVIC_InitStruct);

    // 配置USART通用参数
    USART_InitStruct.USART_BaudRate = 115200;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;

    // 初始化USART2
    USART_Init(USART2, &USART_InitStruct);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART2, ENABLE);
    USART_ClearFlag(USART2, USART_FLAG_TC);

    // 初始化USART3
    USART_Init(USART3, &USART_InitStruct);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART3, ENABLE);
    USART_ClearFlag(USART3, USART_FLAG_TC);

    // 初始化USART6
    USART_Init(USART6, &USART_InitStruct);
    USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART6, ENABLE);
    USART_ClearFlag(USART6, USART_FLAG_TC);
}


// 函数：重定向标准输出函数fputc，通过USART2或USART6发送字符
int fputc(int ch, FILE *f)
{
    // 这里可以根据需要选择使用USART2或USART6
    USART_SendData(USART6, ch);                                 // 通过USART2发送单个字符
    while (USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET); // 等待发送完成
    USART_ClearFlag(USART6, USART_FLAG_TC);                     // 清除发送完成标志
    return ch;                                                  // 返回发送的字符
}