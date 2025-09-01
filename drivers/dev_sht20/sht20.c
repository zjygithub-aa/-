#include "sht20.h"
//#include "seg.h"

// ȫ�ֱ��������ڴ洢SHT20��������ȡ���¶Ⱥ�ʪ������
sht20_val tem_hum_data;

// ��������ʼ��SHT20��������ͨ����ʼ��I2C�ӿڣ�
void Init_sht20(void)
{
    Init_iic();  // ����I2C��ʼ������������I2Cͨ�Žӿ�
}

// ��������ȡSHT20���������¶�����
uint16_t sht20_temdata(void)
{
    uint8_t data[2];   // �洢��ȡ�������ֽ�����
    uint16_t ret = 0;  // �洢���ص�ԭʼ�¶�����

    iic_start();  // ����I2C��ʼ�ź�
    iic_senddata(0x80);  // ����SHT20�Ĵ��豸��ַ��д��������ַΪ0x40������1λ��Ϊ0x80��
    if (iic_waitACK() != ACK)  // �ȴ����豸Ӧ��
    {
        iic_stop();  // �����Ӧ�𣬷���ֹͣ�źŲ����ش���
        return NOACK;  // ������Ӧ�������
    }
    iic_senddata(0xE3);  // �����¶Ȳ�����������¶Ȳ������ޱ���ģʽ��
    if (iic_waitACK() != ACK)  // �ȴ����豸Ӧ��
    {
        iic_stop();  // �����Ӧ�𣬷���ֹͣ�źŲ����ش���
        return NOACK;  // ������Ӧ�������
    }
    iic_start();  // �����µ�I2C��ʼ�ź�
    iic_senddata(0x81);  // ����SHT20�Ĵ��豸��ַ������������ַΪ0x40������1λ���ö�λΪ0x81��
    if (iic_waitACK() != ACK)  // �ȴ����豸Ӧ��
    {
        iic_stop();  // �����Ӧ�𣬷���ֹͣ�źŲ����ش���
        return NOACK;  // ������Ӧ�������
    }
    systick_ms(85);  // �ȴ�85ms��ȷ��SHT20����¶Ȳ��������������ֲᣩ
    data[0] = iic_readdata(ACK);   // ��ȡ��8λ���ݲ�����ACK
    data[1] = iic_readdata(NOACK); // ��ȡ��8λ���ݲ�����NACK
    iic_stop();  // ����I2Cֹͣ�ź�
    ret = (data[0] << 8) | data[1];  // �ϲ��ߵ��ֽ�Ϊ16λ�¶�����
    return ret;  // ����ԭʼ�¶�����
}

// ��������ȡSHT20��������ʪ������
uint16_t sht20_humdata(void)
{
    uint8_t data[2];   // �洢��ȡ�������ֽ�����
    uint16_t ret = 0;  // �洢���ص�ԭʼʪ������

    iic_start();  // ����I2C��ʼ�ź�
    iic_senddata(0x80);  // ����SHT20�Ĵ��豸��ַ��д��������ַΪ0x40������1λ��Ϊ0x80��
    if (iic_waitACK() != ACK)  // �ȴ����豸Ӧ��
    {
        iic_stop();  // �����Ӧ�𣬷���ֹͣ�źŲ����ش���
        return NOACK;  // ������Ӧ�������
    }
    iic_senddata(0xE5);  // ����ʪ�Ȳ����������ʪ�Ȳ������ޱ���ģʽ��
    if (iic_waitACK() != ACK)  // �ȴ����豸Ӧ��
    {
        iic_stop();  // �����Ӧ�𣬷���ֹͣ�źŲ����ش���
        return NOACK;  // ������Ӧ�������
    }
    iic_start();  // �����µ�I2C��ʼ�ź�
    iic_senddata(0x81);  // ����SHT20�Ĵ��豸��ַ������������ַΪ0x40������1λ���ö�λΪ0x81��
    if (iic_waitACK() != ACK)  // �ȴ����豸Ӧ��
    {
        iic_stop();  // �����Ӧ�𣬷���ֹͣ�źŲ����ش���
        return NOACK;  // ������Ӧ�������
    }
    systick_ms(29);  // �ȴ�29ms��ȷ��SHT20���ʪ�Ȳ��������������ֲᣩ
    data[0] = iic_readdata(ACK);   // ��ȡ��8λ���ݲ�����ACK
    data[1] = iic_readdata(NOACK); // ��ȡ��8λ���ݲ�����NACK
    iic_stop();  // ����I2Cֹͣ�ź�
    ret = (data[0] << 8) | data[1];  // �ϲ��ߵ��ֽ�Ϊ16λʪ������
    return ret;  // ����ԭʼʪ������
}

// ��������ȡ������SHT20���������¶Ⱥ�ʪ�����ݣ�����ʾ��OLED
void sht20_getdata(void)
{
    uint16_t tem_temp = 0;  // �洢ԭʼ�¶�����
    uint16_t hum_temp = 0;  // �洢ԭʼʪ������
    char buf[20];           // �����������ڴ洢��ʽ�����ַ���

    // ��ȡԭʼ�¶Ⱥ�ʪ������
    tem_temp = sht20_temdata();  // ���ú�����ȡ�¶�����
    hum_temp = sht20_humdata();  // ���ú�����ȡʪ������

    // ��ԭʼ����ת��Ϊʵ���¶Ⱥ�ʪ��ֵ������SHT20�����ֲṫʽ��
    tem_hum_data.tem = -46.85 + 175.72 * (float)tem_temp / 65536;  // �¶ȹ�ʽ
    tem_hum_data.hum = -6 + 125 * (float)hum_temp / 65536;         // ʪ�ȹ�ʽ

    // ͨ�����ڴ�ӡ�¶Ⱥ�ʪ��ֵ����ѡ������ԭ���ܣ�
    printf("Temperature=%.2f\r\n", tem_hum_data.tem);
    printf("Humidity=%.2f\r\n", tem_hum_data.hum);

    // ����׼����ʾ������
    OLED_Clear();

    // ��ʾ�¶ȣ� "Temp: XX.XX C"
    OLED_ShowString(16, 10, "Temp: ", 16, 1);  // ��ʾ "Temp: " �� (0,0) ��ʼ������16��������ʾ
    sprintf(buf, "%.2f C", tem_hum_data.tem);  // ��ʽ���¶�Ϊ�ַ�����������λС��
    OLED_ShowString(64, 10, (u8*)buf, 16, 1);   // �� x=48 (��Լ "Temp: " ��) ��ʾ�¶�ֵ

    // ��ʾʪ�ȣ� "Hum: XX.XX %"
    OLED_ShowString(16, 34, "Hum: ", 16, 1);   // ��ʾ "Hum: " �� (0,16) ��ʼ����һ�У�
    sprintf(buf, "%.2f %%", tem_hum_data.hum); // ��ʽ��ʪ��Ϊ�ַ�����������λС����ע�� %% Ϊת�� %
    OLED_ShowString(64, 34, (u8*)buf, 16, 1);  // �� x=40 ��ʾʪ��ֵ������x�Զ��룩

    // ����OLED��ʾ
    OLED_Refresh();
		systick_ms(3000);
		OLED_Clear();

//    // ���¶�ֵ�ֽ�Ϊ��λ��������ʾ�������
//    tem1 = (int)((tem_hum_data.tem * 100) / 1000) % 10;  // ��ȡʮλ����
//    tem2 = (int)((tem_hum_data.tem * 100) / 100) % 10;  // ��ȡ��λ����
//    tem3 = (int)((tem_hum_data.tem * 100) / 10) % 10;   // ��ȡС������һλ
//    tem4 = (int)(tem_hum_data.tem * 100) % 10;          // ��ȡС�����ڶ�λ

//    // ���ֽ��������ʾ��������ϣ�����seg_display��������4λ����ܣ�
//    seg_display(3, tem1);  // ��ʾʮλ����
//    seg_display(2, tem2);  // ��ʾ��λ����
//    seg_display(1, tem3);  // ��ʾС������һλ
//    seg_display(0, tem4);  // ��ʾС�����ڶ�λ
}
