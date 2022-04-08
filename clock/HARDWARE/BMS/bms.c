/***********************************
文件：bms.c
描述：电池电压检测
************************************/

#include "bms.h"


extern ADC_HandleTypeDef hadc1;


battery_t Battery_info;


void bms_init()
{
	HAL_ADC_Start(&hadc1);
	
	Battery_info.voltage = 3700.0f;
	Battery_info.fliter_a = 0.005f ;
}

//获取电压
void bms_get_voltage()
{
	uint32_t voltage;
	float a = Battery_info.fliter_a;
	
	voltage = HAL_ADC_GetValue(&hadc1);
	
	voltage = 1.0232f * voltage * 3013 * 7 / 4096 / 4;
	
	if(abs(voltage - Battery_info.voltage) > 10)
	{
		a = 0.6;
	}
	
	Battery_info.voltage = a * voltage + (1.0f - a) * Battery_info.voltage;
}
