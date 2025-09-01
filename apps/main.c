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
#include <string.h>

int8_t dir = 0; 
uint16_t led_pwm=0;
uint8_t t=0;

int main(void)
{
	systick_init(168);
	systick_ms(1000);//让外设反应
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Init_led();
	Init_beep();
	Init_key();
	Init_motor_fan();
	Init_usart2();
	Init_sht20();
	TIM3_PWM_Init();
	Flame_Init();
	OLED_Init();
		OLED_Clear();
		OLED_ShowChinese(0,0,0,16,1);//中
		OLED_ShowChinese(18,0,1,16,1);//景
		OLED_ShowChinese(36,0,2,16,1);//园
		OLED_ShowChinese(54,0,3,16,1);//电
		OLED_ShowChinese(72,0,4,16,1);//子
		OLED_ShowChinese(90,0,5,16,1);//技
		OLED_ShowChinese(108,0,6,16,1);//术
		OLED_ShowString(8,16,"ZHONGJINGYUAN",16,1);
		OLED_ShowString(20,32,"2014/05/01",16,1);
		OLED_ShowString(0,48,"ASCII:",16,1);  
		OLED_ShowString(63,48,"CODE:",16,1);
		OLED_ShowChar(48,48,t,16,1);//显示ASCII字符	   
		t++;
		if(t>'~')t=' ';
		OLED_ShowNum(103,48,t,3,16,1);
		OLED_Refresh();
		systick_ms(1000);
//		OLED_ScrollDisplay(11,4,1);
		OLED_Clear();
	printf("jiayangdada\r\n");
	
	while(1)
	{
		//beep_change();
		//systick_ms(1000);
//		key_check();
		
//		OLED_Clear();
//		OLED_ShowPicture(0,0,128,64,nohappy128,1);
//		OLED_Refresh();
//		systick_ms(3000);
//		OLED_Clear();
//		OLED_ShowPicture(32,0,64,64,liuhan_BMP64,1);
//		OLED_Refresh();
//		systick_ms(3000);
//		OLED_Clear();
//		OLED_ShowPicture(0,0,128,64,smile2,1);
//		OLED_Refresh();
//		systick_ms(3000);
		
//			sht20_getdata();
//			systick_ms(1000);
		 Flame_Check();
//		 if (flag)
//        {
//            flag = 0;
//            if (strcmp((char*)Data, "fan_change") == 0)
//            {
//                GPIO_ToggleBits(GPIOC, GPIO_Pin_6);
//                printf("Fan is Change\r\n");
//            }
//            else if (strcmp((char*)Data, "motor_change") == 0)
//            {
//                GPIO_ToggleBits(GPIOC, GPIO_Pin_7);
//                printf("Motor is Change\r\n");
//            }
//            else if (strcmp((char*)Data, "beep_change") == 0)
//            {
//                GPIO_ToggleBits(GPIOA, GPIO_Pin_15);
//                printf("Beep is Change\r\n");
//            }
//            else if (strcmp((char*)Data, "led_change") == 0)
//            {
//                GPIO_ToggleBits(GPIOF, GPIO_Pin_8);
//                printf("Led is Change\r\n");
//            }
//            else if (strcmp((char*)Data, "sit_down") == 0)
//            {
//                Sit();
//                printf("Dog is Sitting\r\n");
//            }
//            else if (strcmp((char*)Data, "stand_up") == 0)
//            {
//                Stand();
//                printf("Dog is Standing\r\n");
//            }
//            else if (strcmp((char*)Data, "run_go") == 0)
//            {
//                RunForward();
//                printf("Dog is Running Forward\r\n");
//            }
//            else if (strcmp((char*)Data, "back_up") == 0)
//            {
//                RunBackward();  
//                printf("Dog is Running Backward\r\n");
//            }
//						else if (strcmp((char*)Data, "weather") == 0)
//            {
//                sht20_getdata();
//                printf("Get Weather\r\n");
//            }
//						else if (strcmp((char*)Data, "flame_check") == 0)
//            {
//                Flame_Check(); // 调用封装的火焰检测函数
//            }
//            else
//            {
//                printf("Unknown command: %s\r\n", Data);
//            }
//           
//        }
      
		
	} 


}
