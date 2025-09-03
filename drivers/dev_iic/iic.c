#include "iic.h"

// ��ʼ�� I2C ���ţ�PB6 Ϊ SCL��PB7 Ϊ SDA��
void Init_iic(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    // ���� GPIOB ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    
    // ���� PB6��SCL���� PB7��SDA��Ϊ��©���ģʽ
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;        // ���ģʽ
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; // PB6 �� PB7
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;       // ��©�����I2C Ҫ��
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;     // ������/�������ⲿ�������裩
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;    // GPIO �ٶ�Ϊ 50MHz
    GPIO_Init(GPIOB, &GPIO_InitStruct);
}

// �� SDA��PB7������Ϊ����ģʽ�����ڶ�ȡ���豸���ݻ�Ӧ���źţ�
void iic_in(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    // ���� PB7��SDA��Ϊ����ģʽ
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;         // ����ģʽ
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;           // PB7
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;      // ���ֿ�©ģʽ
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;    // ������/����
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;   // GPIO �ٶ�Ϊ 50MHz
    GPIO_Init(GPIOB, &GPIO_InitStruct);
}

// �� SDA��PB7������Ϊ���ģʽ�����ڷ������ݻ�����źţ�
void iic_out(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    // ���� PB7��SDA��Ϊ���ģʽ
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;        // ���ģʽ
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;           // PB7
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;      // ��©���
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;    // ������/����
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;   // GPIO �ٶ�Ϊ 50MHz
    GPIO_Init(GPIOB, &GPIO_InitStruct);
}

// ���� I2C ��ʼ������START����SDA �Ӹߵ��ͣ�SCL ���ָߵ�ƽ
void iic_start(void)
{
    SCL_L;           // ���� SCL
    iic_out();       // ���� SDA Ϊ���ģʽ
    SCL_H;           // ���� SCL
    SDA_H;           // ���� SDA
    delay_us(5);   // �ȴ� 5us��ȷ��ʱ���ȶ�
    SDA_L;           // SDA �Ӹߵ��ͣ�������ʼ�ź�
    delay_us(5);   // �ȴ� 5us
    SCL_L;           // ���� SCL�������ʼ����
}

// ���� I2C ֹͣ������STOP����SDA �ӵ͵��ߣ�SCL ���ָߵ�ƽ
void iic_stop(void)
{
    SCL_L;           // ���� SCL
    iic_out();       // ���� SDA Ϊ���ģʽ
    SDA_L;           // ���� SDA
    SCL_H;           // ���� SCL
    delay_us(5);   // �ȴ� 5us��ȷ��ʱ���ȶ�
    SDA_H;           // SDA �ӵ͵��ߣ�����ֹͣ�ź�
    delay_us(5);   // �ȴ� 5us
}

// ���� I2C Ӧ���źţ�ACK������������ SDA����ʾȷ��
void iic_sendACK(void)
{
    SCL_L;           // ���� SCL��׼������ ACK
    iic_out();       // ���� SDA Ϊ���ģʽ
    SDA_L;           // ���� SDA������ ACK �ź�
    SCL_H;           // ���� SCL���ô��豸��ȡ ACK
    delay_us(5);   // �ȴ� 5us��ȷ��ʱ��
    SCL_L;           // ���� SCL����� ACK ����
}

// ���� I2C ��Ӧ���źţ�NACK������������ SDA����ʾ��ȷ��
void iic_sendNoACK(void)
{
    SCL_L;           // ���� SCL��׼������ NACK
    iic_out();       // ���� SDA Ϊ���ģʽ
    SDA_H;           // ���� SDA������ NACK �ź�
    SCL_H;           // ���� SCL���ô��豸��ȡ NACK
    delay_us(5);   // �ȴ� 5us��ȷ��ʱ��
    SCL_L;           // ���� SCL����� NACK ����
}

// �ȴ����豸��Ӧ���źţ�ACK/NACK��
// ����ֵ��ACK�����豸Ӧ�𣬵͵�ƽ���� NOACK�����豸��Ӧ�𣬸ߵ�ƽ��
uint8_t iic_waitACK(void)
{
    uint8_t timeout = 0;
    SCL_L;           // ���� SCL
    iic_in();        // ���� SDA Ϊ����ģʽ����ȡ���豸�ź�
    SCL_H;           // ���� SCL���������豸���� ACK/NACK
    while(READ_SDA)  // ѭ����� SDA �Ƿ�Ϊ�ͣ�ACK��
    {
        timeout++;
        if(timeout > 500)  // ��ʱ 500 ��ѭ������Ϊ��Ӧ��
        {
            iic_stop();    // ����ֹͣ�ź�
            return NOACK;  // ���� NACK
        }
    }
    SCL_L;           // ���� SCL�����Ӧ����
    return ACK;      // ���� ACK
}

// ����һ���ֽ����ݵ� I2C ����
// ������data - Ҫ���͵� 8 λ����
void iic_senddata(uint8_t data)
{
    uint8_t i = 0;
    SCL_L;           // ���� SCL��׼����������
    iic_out();       // ���� SDA Ϊ���ģʽ
    for(i = 0; i < 8; i++)  // ѭ������ 8 λ���ݣ���λ�ȴ���
    {
        if(data & 0x80)  // ������λ
        {
            SDA_H;       // ���Ϊ 1��SDA ����
        }
        else
        {
            SDA_L;       // ���Ϊ 0��SDA ����
        }
        SCL_H;           // ���� SCL���ô��豸��ȡ����
        delay_us(1);   // �ȴ� 1us��ȷ��ʱ��
        SCL_L;           // ���� SCL��׼����һλ
        delay_us(1);   // �ȴ� 1us
        data = data << 1; // �������ƣ�׼��������һλ
    }
}

// �� I2C ���߶�ȡһ���ֽ�����
// ������flag_ack - ACK������Ӧ�𣩻� NOACK�����ͷ�Ӧ��
// ����ֵ����ȡ�� 8 λ����
uint8_t iic_readdata(uint8_t flag_ack)
{
    uint8_t read_data = 0, i = 0;
    SCL_L;           // ���� SCL��׼����ȡ����
    iic_in();        // ���� SDA Ϊ����ģʽ
    for(i = 0; i < 8; i++)  // ѭ����ȡ 8 λ���ݣ���λ�ȶ���
    {
        read_data = read_data << 1;  // �������ƣ�Ϊ��һλ�ڳ��ռ�
        SCL_H;           // ���� SCL���������豸��������
        if(READ_SDA)     // ��ȡ SDA ״̬
        {
            read_data |= 0x01;  // ��� SDA Ϊ�ߣ��������λΪ 1
        }
        delay_us(1);   // �ȴ� 1us��ȷ��ʱ��
        SCL_L;           // ���� SCL��׼����һλ
        delay_us(1);   // �ȴ� 1us
    }
    // ���� flag_ack �������� ACK �� NACK
    if (flag_ack == ACK)
    {
        iic_sendACK();   // ����Ӧ���ź�
    }
    else
    {
        iic_sendNoACK(); // ���ͷ�Ӧ���ź�
    }
    SCL_L;           // ���� SCL����ɶ�ȡ
    return read_data; // ���ض�ȡ������
}

