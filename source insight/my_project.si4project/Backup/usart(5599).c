#include "usart.h"


// ȫ�ֱ��������ڴ洢���ڽ��յ�������
uint8_t Data[MAX_SIZE];         // ���ݻ�������MAX_SIZE����usart.h�ж���
uint8_t usart_index = 0;       // �������ݻ�������������¼��ǰ���ݴ洢λ��
uint8_t flag = 0;              // ��־����������ָʾ���ݽ��ջ���״̬

// ��������ʼ��USART6������ص�GPIO���ж�
void Init_usart2(void)
{
    // ��������GPIO��USART��NVIC���õĽṹ��
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    // ����GPIOG��USART6�����ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  // ����GPIOGʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); // ����USART6ʱ��

    // ����GPIOG������9��TX��������14��RX��Ϊ���ù���ģʽ
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;         // ����Ϊ���ù���ģʽ�����ڴ���ͨ�ţ�
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; // ѡ������2������TX��������3������RX��
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;      // �����������Ϊ�������
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;        // �����ڲ���������
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;   // ����GPIO�ٶ�Ϊ50MHz
    GPIO_Init(GPIOA, &GPIO_InitStruct);              // ������Ӧ����GPIOA

    // ����NVIC�Դ���USART6���ж�
    NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;              // ѡ��USART6�ж�ͨ��
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;     // ������ռ���ȼ�Ϊ1
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;            // ���������ȼ�Ϊ1
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;               // �����ж�ͨ��
    NVIC_Init(&NVIC_InitStruct);                               // Ӧ��NVIC����

    // ��GPIOG����9������14����ΪUSART6�ĸ��ù���
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);  // ��������2ΪUSART2��TX
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2); // ��������3ΪUSART2��RX

    // ����USART2�Ĳ���
    USART_InitStruct.USART_BaudRate = 115200;                   // ���ò�����Ϊ115200
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // ��Ӳ��������
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;    // ����λΪ8λ
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; // ���ý��պͷ���ģʽ
    USART_InitStruct.USART_Parity = USART_Parity_No;            // ����żУ��
    USART_InitStruct.USART_StopBits = USART_StopBits_1;        // ����ֹͣλΪ1λ
    USART_Init(USART2, &USART_InitStruct);                      // Ӧ��USART2����

    // ����USART6�Ľ����жϲ�����
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);              // ���ý��շǿ��жϣ�RXNE��
    USART_Cmd(USART2, ENABLE);                                  // ����USART2
    USART_ClearFlag(USART2, USART_FLAG_TC);                     // ���������ɱ�־��ȷ����ʼ״̬�ɾ�
}

// �������ض����׼�������fputc��ͨ��USART6�����ַ�
int fputc(int ch, FILE *f)
{
    USART_SendData(USART2, ch);                                 // ͨ��USART6���͵����ַ�
    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET); // �ȴ��������
    USART_ClearFlag(USART2, USART_FLAG_TC);                     // ���������ɱ�־
    return ch;                                                  // ���ط��͵��ַ�
}