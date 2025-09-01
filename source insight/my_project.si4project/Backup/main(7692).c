#include "led.h"
#include "beep.h"
#include "key.h"
#include "systick.h"
#include "usart.h"
#include "oled.h"
#include "bmp.h"
#include "sg90.h"


int8_t dir = 0; 
uint16_t led_pwm=0;
uint8_t t=0;

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Init_led();
	Init_beep();
	Init_key();
	Init_motor_fan();
	systick_init(168);
	Init_usart2();
	TIM3_PWM_Init();
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
		OLED_Clear();
		OLED_ShowChinese(0,0,0,16,1);  //16*16 中
	  OLED_ShowChinese(16,0,0,24,1); //24*24 中
		OLED_ShowChinese(24,20,0,32,1);//32*32 中
	  OLED_ShowChinese(64,0,0,64,1); //64*64 中
		OLED_Refresh();
	  systick_ms(500);
  	OLED_Clear();
		OLED_ShowString(0,0,"ABC",8,1);//6*8 “ABC”
		OLED_ShowString(0,8,"ABC",12,1);//6*12 “ABC”
	  OLED_ShowString(0,20,"ABC",16,1);//8*16 “ABC”
		OLED_ShowString(0,36,"ABC",24,1);//12*24 “ABC”
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
            else if (strcmp((char*)Data, "sit_down") == 0)
            {
                Sit();
                printf("Dog is Sitting\r\n");
            }
            else if (strcmp((char*)Data, "stand_up") == 0)
            {
                Stand();
                printf("Dog is Standing\r\n");
            }
            else if (strcmp((char*)Data, "run_go") == 0)
            {
                RunForward();
                printf("Dog is Running Forward\r\n");
            }
            else if (strcmp((char*)Data, "back_up") == 0)
            {
                RunBackward();
                printf("Dog is Running Backward\r\n");
            }
            else
            {
                printf("Unknown command: %s\r\n", Data);
            }
        }
//		OLED_Clear();
//		OLED_ShowPicture(0,0,128,64,BMP1,1);
//		OLED_Refresh();
//		systick_ms(3000);
//		OLED_Clear();
//		OLED_ShowChinese(0,16,0,16,1);//中
//		OLED_ShowChinese(18,16,1,16,1);//景
//		OLED_ShowChinese(36,16,2,16,1);//园
//		OLED_ShowChinese(54,16,3,16,1);//电
//		OLED_ShowChinese(72,16,4,16,1);//子
//		OLED_ShowChinese(90,16,5,16,1);//技
//		OLED_ShowChinese(108,16,6,16,1);//术
//		OLED_Refresh();
//		systick_ms(1000);
		
	} 


}
