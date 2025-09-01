#include "usart.h"

// ȫ�ֱ��������ڴ洢���ڽ��յ�������
uint8_t Data[MAX_SIZE];         // ���ݻ�������MAX_SIZE����usart.h�ж���
uint8_t usart_index = 0;       // �������ݻ�������������¼��ǰ���ݴ洢λ��
uint8_t flag = 0;              // ��־����������ָʾ���ݽ��ջ���״̬

// ��������ʼ��USART2��USART6������ص�GPIO���ж�
void Init_usart2(void)
{
    // ��������GPIO��USART��NVIC���õĽṹ��
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    // ����GPIOA��GPIOG�Լ�USART2��USART6�����ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  // ����GPIOAʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);  // ����GPIOGʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); // ����USART2ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE); // ����USART6ʱ��

    // ����GPIOA������2��TX��������3��RX��ΪUSART2�ĸ��ù���ģʽ
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;         // ����Ϊ���ù���ģʽ�����ڴ���ͨ�ţ�
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; // ѡ������2������TX��������3������RX��
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;      // �����������Ϊ�������
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;        // �����ڲ���������
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;   // ����GPIO�ٶ�Ϊ50MHz
    GPIO_Init(GPIOA, &GPIO_InitStruct);              // ������Ӧ����GPIOA
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);  // ��������2ΪUSART2��TX
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2); // ��������3ΪUSART2��RX

    // ����GPIOG������9��TX��������14��RX��ΪUSART6�ĸ��ù���ģʽ
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_14; // ѡ������9������TX��������14������RX��
    GPIO_Init(GPIOG, &GPIO_InitStruct);              // ������Ӧ����GPIOG
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource9, GPIO_AF_USART6);  // ��������9ΪUSART6��TX
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource14, GPIO_AF_USART6); // ��������14ΪUSART6��RX

    // ����NVIC�Դ���USART2��USART6���ж�
    NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;              // ѡ��USART2�ж�ͨ��
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;     // ������ռ���ȼ�Ϊ1
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;            // ���������ȼ�Ϊ1
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;               // �����ж�ͨ��
    NVIC_Init(&NVIC_InitStruct);                               // Ӧ��USART2 NVIC����

    NVIC_InitStruct.NVIC_IRQChannel = USART6_IRQn;              // ѡ��USART6�ж�ͨ��
    NVIC_Init(&NVIC_InitStruct);                               // Ӧ��USART6 NVIC����

    // ����USART2�Ĳ���
    USART_InitStruct.USART_BaudRate = 115200;                   // ���ò�����Ϊ115200
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // ��Ӳ��������
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;    // ����λΪ8λ
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; // ���ý��պͷ���ģʽ
    USART_InitStruct.USART_Parity = USART_Parity_No;            // ����żУ��
    USART_InitStruct.USART_StopBits = USART_StopBits_1;        // ����ֹͣλΪ1λ
    USART_Init(USART2, &USART_InitStruct);                      // Ӧ��USART2����
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);              // ����USART2���շǿ��жϣ�RXNE��
    USART_Cmd(USART2, ENABLE);                                  // ����USART2
    USART_ClearFlag(USART2, USART_FLAG_TC);                     // ���USART2������ɱ�־

    // ����USART6�Ĳ���
    USART_Init(USART6, &USART_InitStruct);                      // Ӧ��USART6����
    USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);              // ����USART6���շǿ��жϣ�RXNE��
    USART_Cmd(USART6, ENABLE);                                  // ����USART6
    USART_ClearFlag(USART6, USART_FLAG_TC);                     // ���USART6������ɱ�־
}

// �������ض����׼�������fputc��ͨ��USART2��USART6�����ַ�
int fputc(int ch, FILE *f)
{
    // ������Ը�����Ҫѡ��ʹ��USART2��USART6
    USART_SendData(USART6, ch);                                 // ͨ��USART2���͵����ַ�
    while (USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET); // �ȴ��������
    USART_ClearFlag(USART6, USART_FLAG_TC);                     // ���������ɱ�־
    return ch;                                                  // ���ط��͵��ַ�
}