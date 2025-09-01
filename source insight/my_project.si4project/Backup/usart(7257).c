#include "usart.h"

// 全局变量：用于存储串口接收到的数据
uint8_t Data[MAX_SIZE];         // 数据缓冲区，MAX_SIZE需在usart.h中定义
uint8_t usart_index = 0;       // 串口数据缓冲区索引，记录当前数据存储位置
uint8_t flag = 0;              // 标志变量，用于指示数据接收或处理状态

// 函数：初始化USART2和USART6及其相关的GPIO和中断
void Init_usart2(void)
{
    // 声明用于GPIO、USART和NVIC配置的结构体
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    // 启用GPIOA和GPIOG以及USART2和USART6外设的时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  // 启用GPIOA时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);  // 启用GPIOG时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); // 启用USART2时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE); // 启用USART6时钟

    // 配置GPIOA的引脚2（TX）和引脚3（RX）为USART2的复用功能模式
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;         // 设置为复用功能模式（用于串口通信）
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; // 选择引脚2（发送TX）和引脚3（接收RX）
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;      // 设置输出类型为推挽输出
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;        // 启用内部上拉电阻
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;   // 设置GPIO速度为50MHz
    GPIO_Init(GPIOA, &GPIO_InitStruct);              // 将配置应用于GPIOA
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);  // 配置引脚2为USART2的TX
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2); // 配置引脚3为USART2的RX

    // 配置GPIOG的引脚9（TX）和引脚14（RX）为USART6的复用功能模式
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_14; // 选择引脚9（发送TX）和引脚14（接收RX）
    GPIO_Init(GPIOG, &GPIO_InitStruct);              // 将配置应用于GPIOG
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource9, GPIO_AF_USART6);  // 配置引脚9为USART6的TX
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource14, GPIO_AF_USART6); // 配置引脚14为USART6的RX

    // 配置NVIC以处理USART2和USART6的中断
    NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;              // 选择USART2中断通道
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;     // 设置抢占优先级为1
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;            // 设置子优先级为1
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;               // 启用中断通道
    NVIC_Init(&NVIC_InitStruct);                               // 应用USART2 NVIC配置

    NVIC_InitStruct.NVIC_IRQChannel = USART6_IRQn;              // 选择USART6中断通道
    NVIC_Init(&NVIC_InitStruct);                               // 应用USART6 NVIC配置

    // 配置USART2的参数
    USART_InitStruct.USART_BaudRate = 115200;                   // 设置波特率为115200
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件流控制
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;    // 数据位为8位
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; // 启用接收和发送模式
    USART_InitStruct.USART_Parity = USART_Parity_No;            // 无奇偶校验
    USART_InitStruct.USART_StopBits = USART_StopBits_1;        // 设置停止位为1位
    USART_Init(USART2, &USART_InitStruct);                      // 应用USART2配置
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);              // 启用USART2接收非空中断（RXNE）
    USART_Cmd(USART2, ENABLE);                                  // 启用USART2
    USART_ClearFlag(USART2, USART_FLAG_TC);                     // 清除USART2发送完成标志

    // 配置USART6的参数
    USART_Init(USART6, &USART_InitStruct);                      // 应用USART6配置
    USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);              // 启用USART6接收非空中断（RXNE）
    USART_Cmd(USART6, ENABLE);                                  // 启用USART6
    USART_ClearFlag(USART6, USART_FLAG_TC);                     // 清除USART6发送完成标志
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