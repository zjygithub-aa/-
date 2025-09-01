#include "led.h"
#include "systick.h"
#include "usart.h"
#include "sg90.h"


		uint16_t angle = 0;
   uint8_t direction = 1;
	 uint16_t pulse_width;
	 volatile int i;
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Init_led();
	systick_init(168);
	Init_usart2();
	Init_TIM2_Servo();
	printf("jiayangdada\r\n");
	
	while(1)
	{
//		 if (flag)
//       {
//					
//           flag = 0;
//				
//					if (strcmp((char*)Data, "fan_change") == 0)
//           {
//               GPIO_ToggleBits(GPIOC,GPIO_Pin_6);
//               printf("Fan is Change\r\n");
//           }
//           else if (strcmp((char*)Data, "motor_change") == 0)
//           {
//              GPIO_ToggleBits(GPIOC,GPIO_Pin_7);
//               printf("Motor is Change\r\n");
//           }
//					else if (strcmp((char*)Data, "beep_change") == 0)
//           {
//              GPIO_ToggleBits(GPIOA,GPIO_Pin_15);
//               printf("Beep is Change\r\n");
//           }
//					else if (strcmp((char*)Data, "led_change") == 0)
//           {
//              GPIO_ToggleBits(GPIOF,GPIO_Pin_8);
//               printf("Led is Change\r\n");
//           }
//           else
//           {
//               printf("Unknown command: %s\r\n",Data);
//           }
//       }
				uint16_t pulse_width = 500 + (angle * 2000 / 180); // 线性映射
        Set_TIM2_PWM(pulse_width); // 设置 PWM 脉宽 (单位: us)

        // 调试输出
        printf("Angle: %d°, Pulse: %d us\r\n", angle, pulse_width);

        // 角度变化
        if (direction)
        {
            if (angle < 180) angle += 5;
            else direction = 0;
        }
        else
        {
            if (angle > 0) angle -= 5;
            else direction = 1;
        }

        // 粗略延时约 20ms
        for ( i = 0; i < 400000; i++);
		
	} 


}
