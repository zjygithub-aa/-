	#include "usart.h"

// ȫ�ֱ��������ڴ洢���ڽ��յ�������
uint8_t Data[MAX_SIZE];         // ���ݻ�������MAX_SIZE����usart.h�ж���
 uint8_t usart_index = 0;       // �������ݻ�������������¼��ǰ���ݴ洢λ��
 uint8_t flag = 0;              // ��־����������ָʾ���ݽ��ջ���״̬

// ��������ʼ��USART2��USART6������ص�GPIO���ж�
void Init_usart2(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    // �����������GPIO��USART�����ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOG, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB1Periph_USART3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

    // ����GPIOA������2��TX��������3��RX��ΪUSART2
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);  // PA2 as TX
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);  // PA3 as RX

    // ����GPIOB������10��TX��������11��RX��ΪUSART3
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3); // PB10 as TX
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3); // PB11 as RX

    // ����GPIOG������9��TX��������14��RX��ΪUSART6
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_14;
    GPIO_Init(GPIOG, &GPIO_InitStruct);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource9, GPIO_AF_USART6);  // PG9 as TX
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource14, GPIO_AF_USART6); // PG14 as RX

    // ����NVIC�жϣ����д�����ռ���ȼ��������ȼ�����Ϊ1
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;

    // ����USART2�ж�
    NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
    NVIC_Init(&NVIC_InitStruct);

    // ����USART3�ж�
    NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;
    NVIC_Init(&NVIC_InitStruct);

    // ����USART6�ж�
    NVIC_InitStruct.NVIC_IRQChannel = USART6_IRQn;
    NVIC_Init(&NVIC_InitStruct);

    // ����USARTͨ�ò���
    USART_InitStruct.USART_BaudRate = 115200;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;

    // ��ʼ��USART2
    USART_Init(USART2, &USART_InitStruct);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART2, ENABLE);
    USART_ClearFlag(USART2, USART_FLAG_TC);

    // ��ʼ��USART3
    USART_Init(USART3, &USART_InitStruct);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART3, ENABLE);
    USART_ClearFlag(USART3, USART_FLAG_TC);

    // ��ʼ��USART6
    USART_Init(USART6, &USART_InitStruct);
    USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART6, ENABLE);
    USART_ClearFlag(USART6, USART_FLAG_TC);
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