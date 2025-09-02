#include "led.h"
#include "beep.h"
#include "key.h"
#include "systick.h"
#include "usart.h"
#include "sht20.h"
#include "oled.h"
#include "bmp.h"
#include "sg90.h"
#include "pd333.h"
#include "FreeRTOS.h"
#include "task.h"
#include <string.h>

int8_t dir = 0; 
uint16_t led_pwm=0;
uint8_t t=0;
volatile uint8_t motion_flag = 0; 

#define START_STACK 128
#define START_PRIORITY 1  // 启动任务优先级中等，用于创建其他任务

TaskHandle_t START_handle;

#define OLED_TASK_STACK 256  // OLED显示任务栈大小（增加以处理显示操作）
#define OLED_TASK_PRIORITY 1  // OLED任务优先级较低，因为显示不是实时性强的

TaskHandle_t OLEDTask_handle;

#define SENSOR_TASK_STACK 128  // 传感器读取任务栈大小
#define SENSOR_TASK_PRIORITY 1 // 传感器任务优先级较高，需要定期读取数据

TaskHandle_t SensorTask_handle;

#define COMMAND_TASK_STACK 256  // 命令处理任务栈大小（处理字符串比较等）
#define COMMAND_TASK_PRIORITY 1  // 命令处理优先级中等，响应用户输入

TaskHandle_t CommandTask_handle;

#define MOTION_TASK_STACK 256   // 动作任务栈
#define MOTION_TASK_PRIORITY 1  // 动作优先级：和命令一样，中等优先级

TaskHandle_t MotionTask_handle;


void start_task(void *pvParameters);
void OLED_Task(void *pvParameters);
void Sensor_Task(void *pvParameters);
void Command_Task(void *pvParameters);
void Motion_Task(void *pvParameters);

void start_task(void *pvParameters)
{
    printf("---%s---\r\n", __FUNCTION__);
    
    // 创建OLED显示任务，优先级1（较低）
    xTaskCreate(OLED_Task, "OLED_Task", OLED_TASK_STACK, NULL, OLED_TASK_PRIORITY, &OLEDTask_handle);
    
    // 创建传感器读取任务，优先级3（较高，确保定期执行）
    xTaskCreate(Sensor_Task, "Sensor_Task", SENSOR_TASK_STACK, NULL, SENSOR_TASK_PRIORITY, &SensorTask_handle);
	
		// 创建动作任务
		xTaskCreate(Motion_Task, "Motion_Task", MOTION_TASK_STACK, NULL, MOTION_TASK_PRIORITY, &MotionTask_handle);
    
    // 创建命令处理任务，优先级2（中等，平衡响应和资源）
    xTaskCreate(Command_Task, "Command_Task", COMMAND_TASK_STACK, NULL, COMMAND_TASK_PRIORITY, &CommandTask_handle);
    
    // 删除启动任务自身
    vTaskDelete(START_handle);
}

void Motion_Task(void *pvParameters)
{
    static uint8_t step = 0; // 步态阶段

    for(;;)
    {
        if (motion_flag == 1) {  // 前进
            RunForward(step);
						 OLED_Clear();
						 OLED_ShowPicture(0,0,128,64,run_bmp,1);
						 OLED_Refresh();
            step ^= 1; // 切换0/1
        }
        else if (motion_flag == 2) { // 后退
            RunBackward(step);
						 OLED_Clear();
						 OLED_ShowPicture(0,0,128,64,back_bmp,1);
						 OLED_Refresh();
            step ^= 1;
        }
        else if (motion_flag == 3) {
             Sit();
					   OLED_Clear();
						 OLED_ShowPicture(0,0,128,64,sit_bmp,1);
						 OLED_Refresh();
            motion_flag = 0;
        }
        else if (motion_flag == 4) {
            Stand();
					   OLED_Clear();
						 OLED_ShowPicture(0,0,128,64,stand_bmp,1);
						 OLED_Refresh();
            motion_flag = 0;
        }
        else {
            step = 0; // 停止时复位步态
        }

        vTaskDelay(500);  // 500ms 刷新一次动作，相当于原 systick_ms(500)
    }
}


void OLED_Task(void *pvParameters)
{
    // 将OLED初始化和显示代码移到这个任务中，确保只执行一次或按需
    OLED_Init();
    OLED_Clear();
    OLED_ShowChinese(0,0,0,16,1); //中
    OLED_ShowChinese(18,0,1,16,1); //景
    OLED_ShowChinese(36,0,2,16,1); //园
    OLED_ShowChinese(54,0,3,16,1); //电
    OLED_ShowChinese(72,0,4,16,1); //子
    OLED_ShowChinese(90,0,5,16,1); //技
    OLED_ShowChinese(108,0,6,16,1); //术
    OLED_ShowString(8,16,"ZHONGJINGYUAN",16,1);
    OLED_ShowString(20,32,"2014/05/01",16,1);
    OLED_ShowString(0,48,"ASCII:",16,1);  
    OLED_ShowString(63,48,"CODE:",16,1);
    OLED_ShowChar(48,48,t,16,1); //显示ASCII字符	   
    t++;
    if(t>'~') t=' ';
    OLED_ShowNum(103,48,t,3,16,1);
    OLED_Refresh();
    vTaskDelay(1000);  // 替换systick_ms(1000)
    // OLED_ScrollDisplay(11,4,1);  // 如果需要，取消注释
    OLED_Clear();
    
    // 可以添加循环显示或其他，但这里保持原样，只执行一次初始显示
    // 如果需要循环显示图片，取消以下注释并调整
    /*
    for(;;)
    {
        OLED_Clear();
        OLED_ShowPicture(0,0,128,64,nohappy128,1);
        OLED_Refresh();
        vTaskDelay(3000);
        OLED_Clear();
        OLED_ShowPicture(32,0,64,64,liuhan_BMP64,1);
        OLED_Refresh();
        vTaskDelay(3000);
        OLED_Clear();
        OLED_ShowPicture(0,0,128,64,smile2,1);
        OLED_Refresh();
        vTaskDelay(3000);
    }
    */
    
    // 如果不需要循环，删除自身或挂起
    vTaskDelete(NULL);
}

void Sensor_Task(void *pvParameters)
{
    for(;;)
    {
        // sht20_getdata();  // 如果需要定期读取，取消注释
        // Flame_Check();    // 如果需要定期检查，取消注释
        vTaskDelay(1000);  // 每1秒检查一次，调整延迟以匹配需求
    }
}

void Command_Task(void *pvParameters)
{
		
    for(;;)
    {
     
        if (flag)
        {
            flag = 0;
            if (strcmp((char*)Data, "fan_change") == 0)
            {
                GPIO_ToggleBits(GPIOC, GPIO_Pin_6);
                printf("Fan is Change\r\n");
            }
            else if (strcmp((char*)Data, "motor_change") == 0)
            {
                GPIO_ToggleBits(GPIOC, GPIO_Pin_7);
                printf("Motor is Change\r\n");
            }
            else if (strcmp((char*)Data, "beep_change") == 0)
            {
                GPIO_ToggleBits(GPIOA, GPIO_Pin_15);
                printf("Beep is Change\r\n");
            }
            else if (strcmp((char*)Data, "led_change") == 0)
            {
                GPIO_ToggleBits(GPIOF, GPIO_Pin_8);
                printf("Led is Change\r\n");
            }
            if (strcmp((char*)Data, "run_go") == 0) {
                motion_flag = 1;
                printf("Dog is Running Forward\r\n");
            }
            else if (strcmp((char*)Data, "back") == 0) {
                motion_flag = 2;
                printf("Dog is Running Backward\r\n");
            }
            else if (strcmp((char*)Data, "sit_down") == 0) {
                motion_flag = 3;
                printf("Dog is Sitting\r\n");
            }
            else if (strcmp((char*)Data, "stand_up") == 0) {
                motion_flag = 4;
                printf("Dog is Standing\r\n");
            }
            else if (strcmp((char*)Data, "weather") == 0)
            {
                sht20_getdata();
                printf("Get Weather\r\n");
            }
            else if (strcmp((char*)Data, "flame_check") == 0)
            {
                Flame_Check(); // 调用封装的火焰检测函数
            }
            else
            {
                printf("Unknown command: %s\r\n", Data);
            }
	
        }
        
        // beep_change();  // 如果需要，取消注释
        // key_check();    // 如果需要，取消注释
        vTaskDelay(100);  // 每100ms检查一次flag，调整以避免CPU占用
    }
}

int main(void)
{
    systick_init(168);
    systick_ms(1000); //让外设反应
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    Init_led();
    Init_beep();
    //Init_key();
    Init_motor_fan();
    Init_usart2();
    Init_sht20();
    TIM3_PWM_Init();
    Flame_Init();
		OLED_Init();
		OLED_Clear();
    
    printf("jiayangdada\r\n");
    
    // 创建启动任务，优先级2
    xTaskCreate(start_task, "start_task", START_STACK, NULL, START_PRIORITY, &START_handle);
    
    // 启动调度器
    vTaskStartScheduler();
    
    // 此处不会执行，因为调度器启动后不会返回
    while(1)
    {
//			sht20_getdata();
//			systick_ms(1000);
        // 如果有需要idle处理的代码，可以移到Idle任务钩子函数中
    }
}