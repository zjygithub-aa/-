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
#define START_PRIORITY 1  // �����������ȼ��еȣ����ڴ�����������

TaskHandle_t START_handle;

#define OLED_TASK_STACK 256  // OLED��ʾ����ջ��С�������Դ�����ʾ������
#define OLED_TASK_PRIORITY 1  // OLED�������ȼ��ϵͣ���Ϊ��ʾ����ʵʱ��ǿ��

TaskHandle_t OLEDTask_handle;

#define SENSOR_TASK_STACK 128  // ��������ȡ����ջ��С
#define SENSOR_TASK_PRIORITY 1 // �������������ȼ��ϸߣ���Ҫ���ڶ�ȡ����

TaskHandle_t SensorTask_handle;

#define COMMAND_TASK_STACK 256  // ���������ջ��С�������ַ����Ƚϵȣ�
#define COMMAND_TASK_PRIORITY 1  // ��������ȼ��еȣ���Ӧ�û�����

TaskHandle_t CommandTask_handle;

#define MOTION_TASK_STACK 256   // ��������ջ
#define MOTION_TASK_PRIORITY 1  // �������ȼ���������һ�����е����ȼ�

TaskHandle_t MotionTask_handle;


void start_task(void *pvParameters);
void OLED_Task(void *pvParameters);
void Sensor_Task(void *pvParameters);
void Command_Task(void *pvParameters);
void Motion_Task(void *pvParameters);

void start_task(void *pvParameters)
{
    printf("---%s---\r\n", __FUNCTION__);
    
    // ����OLED��ʾ�������ȼ�1���ϵͣ�
    xTaskCreate(OLED_Task, "OLED_Task", OLED_TASK_STACK, NULL, OLED_TASK_PRIORITY, &OLEDTask_handle);
    
    // ������������ȡ�������ȼ�3���ϸߣ�ȷ������ִ�У�
    xTaskCreate(Sensor_Task, "Sensor_Task", SENSOR_TASK_STACK, NULL, SENSOR_TASK_PRIORITY, &SensorTask_handle);
	
		// ������������
		xTaskCreate(Motion_Task, "Motion_Task", MOTION_TASK_STACK, NULL, MOTION_TASK_PRIORITY, &MotionTask_handle);
    
    // ����������������ȼ�2���еȣ�ƽ����Ӧ����Դ��
    xTaskCreate(Command_Task, "Command_Task", COMMAND_TASK_STACK, NULL, COMMAND_TASK_PRIORITY, &CommandTask_handle);
    
    // ɾ��������������
    vTaskDelete(START_handle);
}

void Motion_Task(void *pvParameters)
{
    static uint8_t step = 0; // ��̬�׶�

    for(;;)
    {
        if (motion_flag == 1) {  // ǰ��
            RunForward(step);
						 OLED_Clear();
						 OLED_ShowPicture(0,0,128,64,run_bmp,1);
						 OLED_Refresh();
            step ^= 1; // �л�0/1
        }
        else if (motion_flag == 2) { // ����
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
            step = 0; // ֹͣʱ��λ��̬
        }

        vTaskDelay(500);  // 500ms ˢ��һ�ζ������൱��ԭ systick_ms(500)
    }
}


void OLED_Task(void *pvParameters)
{
    // ��OLED��ʼ������ʾ�����Ƶ���������У�ȷ��ִֻ��һ�λ���
    OLED_Init();
    OLED_Clear();
    OLED_ShowChinese(0,0,0,16,1); //��
    OLED_ShowChinese(18,0,1,16,1); //��
    OLED_ShowChinese(36,0,2,16,1); //԰
    OLED_ShowChinese(54,0,3,16,1); //��
    OLED_ShowChinese(72,0,4,16,1); //��
    OLED_ShowChinese(90,0,5,16,1); //��
    OLED_ShowChinese(108,0,6,16,1); //��
    OLED_ShowString(8,16,"ZHONGJINGYUAN",16,1);
    OLED_ShowString(20,32,"2014/05/01",16,1);
    OLED_ShowString(0,48,"ASCII:",16,1);  
    OLED_ShowString(63,48,"CODE:",16,1);
    OLED_ShowChar(48,48,t,16,1); //��ʾASCII�ַ�	   
    t++;
    if(t>'~') t=' ';
    OLED_ShowNum(103,48,t,3,16,1);
    OLED_Refresh();
    vTaskDelay(1000);  // �滻systick_ms(1000)
    // OLED_ScrollDisplay(11,4,1);  // �����Ҫ��ȡ��ע��
    OLED_Clear();
    
    // �������ѭ����ʾ�������������ﱣ��ԭ����ִֻ��һ�γ�ʼ��ʾ
    // �����Ҫѭ����ʾͼƬ��ȡ������ע�Ͳ�����
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
    
    // �������Ҫѭ����ɾ����������
    vTaskDelete(NULL);
}

void Sensor_Task(void *pvParameters)
{
    for(;;)
    {
        // sht20_getdata();  // �����Ҫ���ڶ�ȡ��ȡ��ע��
        // Flame_Check();    // �����Ҫ���ڼ�飬ȡ��ע��
        vTaskDelay(1000);  // ÿ1����һ�Σ������ӳ���ƥ������
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
                Flame_Check(); // ���÷�װ�Ļ����⺯��
            }
            else
            {
                printf("Unknown command: %s\r\n", Data);
            }
	
        }
        
        // beep_change();  // �����Ҫ��ȡ��ע��
        // key_check();    // �����Ҫ��ȡ��ע��
        vTaskDelay(100);  // ÿ100ms���һ��flag�������Ա���CPUռ��
    }
}

int main(void)
{
    systick_init(168);
    systick_ms(1000); //�����跴Ӧ
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
    
    // ���������������ȼ�2
    xTaskCreate(start_task, "start_task", START_STACK, NULL, START_PRIORITY, &START_handle);
    
    // ����������
    vTaskStartScheduler();
    
    // �˴�����ִ�У���Ϊ�����������󲻻᷵��
    while(1)
    {
//			sht20_getdata();
//			systick_ms(1000);
        // �������Ҫidle����Ĵ��룬�����Ƶ�Idle�����Ӻ�����
    }
}