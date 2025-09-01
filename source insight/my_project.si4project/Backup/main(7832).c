#include "led.h"
#include "systick.h"
#include "usart.h"
#include "sg90.h"

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Init_led();
	systick_init(168);
	Init_usart2();
	//TIM3_PWM_Init();
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
//			 Servo_SetAngle(0);   // 0бу
//        systick_ms(1000);
//        
//        Servo_SetAngle(90);  // 90бу
//       systick_ms(1000);
//        
//        Servo_SetAngle(180); // 180бу
//        systick_ms(1000);
//        
//        Servo_SetAngle(90);  // 90бу
//				systick_ms(1000);
		
	} 


}
