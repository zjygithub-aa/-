#include "led.h"
#include "systick.h"

int main(void)
{
	
	Init_led();
	systick_init(168);
	while(1)
	{
		led_open();
		systick_ms(1000);
		led_close();
		systick_ms(1000);
		
	} 


}
