#ifndef __SHT20_H__
#define __SHT20_H__
#include <stm32f4xx.h>
#include "systick.h"
#include "iic.h"
#include "oled.h"
typedef struct{
	uint8_t temp_h;
	uint8_t temp_l;
	uint8_t hum_h;
	uint8_t hum_l;

	float tem;
	float hum;		
}sht20_val;

extern sht20_val tem_hum_data;


void Init_sht20(void);

uint16_t sht20_temdata(void);
uint16_t sht20_humdata(void);


void sht20_getdata(void);






#endif

