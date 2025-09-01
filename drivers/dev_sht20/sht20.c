#include "sht20.h"
//#include "seg.h"

// 全局变量：用于存储SHT20传感器读取的温度和湿度数据
sht20_val tem_hum_data;

// 函数：初始化SHT20传感器（通过初始化I2C接口）
void Init_sht20(void)
{
    Init_iic();  // 调用I2C初始化函数，配置I2C通信接口
}

// 函数：读取SHT20传感器的温度数据
uint16_t sht20_temdata(void)
{
    uint8_t data[2];   // 存储读取的两个字节数据
    uint16_t ret = 0;  // 存储返回的原始温度数据

    iic_start();  // 发起I2C起始信号
    iic_senddata(0x80);  // 发送SHT20的从设备地址（写操作，地址为0x40，左移1位后为0x80）
    if (iic_waitACK() != ACK)  // 等待从设备应答
    {
        iic_stop();  // 如果无应答，发送停止信号并返回错误
        return NOACK;  // 返回无应答错误码
    }
    iic_senddata(0xE3);  // 发送温度测量命令（触发温度测量，无保持模式）
    if (iic_waitACK() != ACK)  // 等待从设备应答
    {
        iic_stop();  // 如果无应答，发送停止信号并返回错误
        return NOACK;  // 返回无应答错误码
    }
    iic_start();  // 发起新的I2C起始信号
    iic_senddata(0x81);  // 发送SHT20的从设备地址（读操作，地址为0x40，左移1位并置读位为0x81）
    if (iic_waitACK() != ACK)  // 等待从设备应答
    {
        iic_stop();  // 如果无应答，发送停止信号并返回错误
        return NOACK;  // 返回无应答错误码
    }
    systick_ms(85);  // 等待85ms以确保SHT20完成温度测量（根据数据手册）
    data[0] = iic_readdata(ACK);   // 读取高8位数据并发送ACK
    data[1] = iic_readdata(NOACK); // 读取低8位数据并发送NACK
    iic_stop();  // 发送I2C停止信号
    ret = (data[0] << 8) | data[1];  // 合并高低字节为16位温度数据
    return ret;  // 返回原始温度数据
}

// 函数：读取SHT20传感器的湿度数据
uint16_t sht20_humdata(void)
{
    uint8_t data[2];   // 存储读取的两个字节数据
    uint16_t ret = 0;  // 存储返回的原始湿度数据

    iic_start();  // 发起I2C起始信号
    iic_senddata(0x80);  // 发送SHT20的从设备地址（写操作，地址为0x40，左移1位后为0x80）
    if (iic_waitACK() != ACK)  // 等待从设备应答
    {
        iic_stop();  // 如果无应答，发送停止信号并返回错误
        return NOACK;  // 返回无应答错误码
    }
    iic_senddata(0xE5);  // 发送湿度测量命令（触发湿度测量，无保持模式）
    if (iic_waitACK() != ACK)  // 等待从设备应答
    {
        iic_stop();  // 如果无应答，发送停止信号并返回错误
        return NOACK;  // 返回无应答错误码
    }
    iic_start();  // 发起新的I2C起始信号
    iic_senddata(0x81);  // 发送SHT20的从设备地址（读操作，地址为0x40，左移1位并置读位为0x81）
    if (iic_waitACK() != ACK)  // 等待从设备应答
    {
        iic_stop();  // 如果无应答，发送停止信号并返回错误
        return NOACK;  // 返回无应答错误码
    }
    systick_ms(29);  // 等待29ms以确保SHT20完成湿度测量（根据数据手册）
    data[0] = iic_readdata(ACK);   // 读取高8位数据并发送ACK
    data[1] = iic_readdata(NOACK); // 读取低8位数据并发送NACK
    iic_stop();  // 发送I2C停止信号
    ret = (data[0] << 8) | data[1];  // 合并高低字节为16位湿度数据
    return ret;  // 返回原始湿度数据
}

// 函数：获取并处理SHT20传感器的温度和湿度数据，并显示到OLED
void sht20_getdata(void)
{
    uint16_t tem_temp = 0;  // 存储原始温度数据
    uint16_t hum_temp = 0;  // 存储原始湿度数据
    char buf[20];           // 缓冲区，用于存储格式化的字符串

    // 获取原始温度和湿度数据
    tem_temp = sht20_temdata();  // 调用函数读取温度数据
    hum_temp = sht20_humdata();  // 调用函数读取湿度数据

    // 将原始数据转换为实际温度和湿度值（根据SHT20数据手册公式）
    tem_hum_data.tem = -46.85 + 175.72 * (float)tem_temp / 65536;  // 温度公式
    tem_hum_data.hum = -6 + 125 * (float)hum_temp / 65536;         // 湿度公式

    // 通过串口打印温度和湿度值（可选，保留原功能）
    printf("Temperature=%.2f\r\n", tem_hum_data.tem);
    printf("Humidity=%.2f\r\n", tem_hum_data.hum);

    // 清屏准备显示新数据
    OLED_Clear();

    // 显示温度： "Temp: XX.XX C"
    OLED_ShowString(16, 10, "Temp: ", 16, 1);  // 显示 "Temp: " 从 (0,0) 开始，字体16，正常显示
    sprintf(buf, "%.2f C", tem_hum_data.tem);  // 格式化温度为字符串，保留两位小数
    OLED_ShowString(64, 10, (u8*)buf, 16, 1);   // 从 x=48 (大约 "Temp: " 后) 显示温度值

    // 显示湿度： "Hum: XX.XX %"
    OLED_ShowString(16, 34, "Hum: ", 16, 1);   // 显示 "Hum: " 从 (0,16) 开始（下一行）
    sprintf(buf, "%.2f %%", tem_hum_data.hum); // 格式化湿度为字符串，保留两位小数，注意 %% 为转义 %
    OLED_ShowString(64, 34, (u8*)buf, 16, 1);  // 从 x=40 显示湿度值（调整x以对齐）

    // 更新OLED显示
    OLED_Refresh();
		systick_ms(3000);
		OLED_Clear();

//    // 将温度值分解为各位数字以显示到数码管
//    tem1 = (int)((tem_hum_data.tem * 100) / 1000) % 10;  // 提取十位数字
//    tem2 = (int)((tem_hum_data.tem * 100) / 100) % 10;  // 提取个位数字
//    tem3 = (int)((tem_hum_data.tem * 100) / 10) % 10;   // 提取小数点后第一位
//    tem4 = (int)(tem_hum_data.tem * 100) % 10;          // 提取小数点后第二位

//    // 将分解的数字显示到数码管上（假设seg_display函数控制4位数码管）
//    seg_display(3, tem1);  // 显示十位数字
//    seg_display(2, tem2);  // 显示个位数字
//    seg_display(1, tem3);  // 显示小数点后第一位
//    seg_display(0, tem4);  // 显示小数点后第二位
}
