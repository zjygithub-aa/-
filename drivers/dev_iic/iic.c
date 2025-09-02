#include "iic.h"

// 初始化 I2C 引脚（PB6 为 SCL，PB7 为 SDA）
void Init_iic(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    // 启用 GPIOB 时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    
    // 配置 PB6（SCL）和 PB7（SDA）为开漏输出模式
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;        // 输出模式
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; // PB6 和 PB7
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;       // 开漏输出（I2C 要求）
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;     // 无上拉/下拉（外部上拉电阻）
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;    // GPIO 速度为 50MHz
    GPIO_Init(GPIOB, &GPIO_InitStruct);
}

// 将 SDA（PB7）配置为输入模式（用于读取从设备数据或应答信号）
void iic_in(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    // 配置 PB7（SDA）为输入模式
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;         // 输入模式
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;           // PB7
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;      // 保持开漏模式
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;    // 无上拉/下拉
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;   // GPIO 速度为 50MHz
    GPIO_Init(GPIOB, &GPIO_InitStruct);
}

// 将 SDA（PB7）配置为输出模式（用于发送数据或控制信号）
void iic_out(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    // 配置 PB7（SDA）为输出模式
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;        // 输出模式
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;           // PB7
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;      // 开漏输出
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;    // 无上拉/下拉
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;   // GPIO 速度为 50MHz
    GPIO_Init(GPIOB, &GPIO_InitStruct);
}

// 生成 I2C 起始条件（START）：SDA 从高到低，SCL 保持高电平
void iic_start(void)
{
    SCL_L;           // 拉低 SCL
    iic_out();       // 设置 SDA 为输出模式
    SCL_H;           // 拉高 SCL
    SDA_H;           // 拉高 SDA
    delay_us(5);   // 等待 5us，确保时序稳定
    SDA_L;           // SDA 从高到低，生成起始信号
    delay_us(5);   // 等待 5us
    SCL_L;           // 拉低 SCL，完成起始条件
}

// 生成 I2C 停止条件（STOP）：SDA 从低到高，SCL 保持高电平
void iic_stop(void)
{
    SCL_L;           // 拉低 SCL
    iic_out();       // 设置 SDA 为输出模式
    SDA_L;           // 拉低 SDA
    SCL_H;           // 拉高 SCL
    delay_us(5);   // 等待 5us，确保时序稳定
    SDA_H;           // SDA 从低到高，生成停止信号
    delay_us(5);   // 等待 5us
}

// 发送 I2C 应答信号（ACK）：主机拉低 SDA，表示确认
void iic_sendACK(void)
{
    SCL_L;           // 拉低 SCL，准备发送 ACK
    iic_out();       // 设置 SDA 为输出模式
    SDA_L;           // 拉低 SDA，发送 ACK 信号
    SCL_H;           // 拉高 SCL，让从设备读取 ACK
    delay_us(5);   // 等待 5us，确保时序
    SCL_L;           // 拉低 SCL，完成 ACK 发送
}

// 发送 I2C 非应答信号（NACK）：主机拉高 SDA，表示不确认
void iic_sendNoACK(void)
{
    SCL_L;           // 拉低 SCL，准备发送 NACK
    iic_out();       // 设置 SDA 为输出模式
    SDA_H;           // 拉高 SDA，发送 NACK 信号
    SCL_H;           // 拉高 SCL，让从设备读取 NACK
    delay_us(5);   // 等待 5us，确保时序
    SCL_L;           // 拉低 SCL，完成 NACK 发送
}

// 等待从设备的应答信号（ACK/NACK）
// 返回值：ACK（从设备应答，低电平）或 NOACK（从设备无应答，高电平）
uint8_t iic_waitACK(void)
{
    uint8_t timeout = 0;
    SCL_L;           // 拉低 SCL
    iic_in();        // 设置 SDA 为输入模式，读取从设备信号
    SCL_H;           // 拉高 SCL，触发从设备发送 ACK/NACK
    while(READ_SDA)  // 循环检查 SDA 是否为低（ACK）
    {
        timeout++;
        if(timeout > 500)  // 超时 500 次循环后，认为无应答
        {
            iic_stop();    // 发送停止信号
            return NOACK;  // 返回 NACK
        }
    }
    SCL_L;           // 拉低 SCL，完成应答检测
    return ACK;      // 返回 ACK
}

// 发送一个字节数据到 I2C 总线
// 参数：data - 要发送的 8 位数据
void iic_senddata(uint8_t data)
{
    uint8_t i = 0;
    SCL_L;           // 拉低 SCL，准备发送数据
    iic_out();       // 设置 SDA 为输出模式
    for(i = 0; i < 8; i++)  // 循环发送 8 位数据（高位先传）
    {
        if(data & 0x80)  // 检查最高位
        {
            SDA_H;       // 如果为 1，SDA 拉高
        }
        else
        {
            SDA_L;       // 如果为 0，SDA 拉低
        }
        SCL_H;           // 拉高 SCL，让从设备读取数据
        delay_us(1);   // 等待 1us，确保时序
        SCL_L;           // 拉低 SCL，准备下一位
        delay_us(1);   // 等待 1us
        data = data << 1; // 数据左移，准备发送下一位
    }
}

// 从 I2C 总线读取一个字节数据
// 参数：flag_ack - ACK（发送应答）或 NOACK（发送非应答）
// 返回值：读取的 8 位数据
uint8_t iic_readdata(uint8_t flag_ack)
{
    uint8_t read_data = 0, i = 0;
    SCL_L;           // 拉低 SCL，准备读取数据
    iic_in();        // 设置 SDA 为输入模式
    for(i = 0; i < 8; i++)  // 循环读取 8 位数据（高位先读）
    {
        read_data = read_data << 1;  // 数据左移，为下一位腾出空间
        SCL_H;           // 拉高 SCL，触发从设备发送数据
        if(READ_SDA)     // 读取 SDA 状态
        {
            read_data |= 0x01;  // 如果 SDA 为高，设置最低位为 1
        }
        delay_us(1);   // 等待 1us，确保时序
        SCL_L;           // 拉低 SCL，准备下一位
        delay_us(1);   // 等待 1us
    }
    // 根据 flag_ack 参数发送 ACK 或 NACK
    if (flag_ack == ACK)
    {
        iic_sendACK();   // 发送应答信号
    }
    else
    {
        iic_sendNoACK(); // 发送非应答信号
    }
    SCL_L;           // 拉低 SCL，完成读取
    return read_data; // 返回读取的数据
}

