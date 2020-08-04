#ifndef _BMS_H_
#define _BMS_H_


#include "stdlib.h"
#include "math.h"
#include "main.h"
#include "stm32f1xx_hal.h"


typedef struct BATTERY_STRUCT
{
	float voltage;		//µÁ—πmV
	float fliter_a;
}battery_t;


void bms_init(void);
void bms_get_voltage(void);


extern battery_t Battery_info;


#endif
