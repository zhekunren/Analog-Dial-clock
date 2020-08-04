#include "key.h"

static uint8_t key_get_value()
{
	if(HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == GPIO_PIN_RESET)
	{
		return 1;
	}
	if(HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin) == GPIO_PIN_RESET)
	{
		return 2;
	}
	if(HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin) == GPIO_PIN_RESET)
	{
		return 3;
	}
	if(HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin) == GPIO_PIN_RESET)
	{
		return 4;
	}
	if(HAL_GPIO_ReadPin(KEY5_GPIO_Port, KEY5_Pin) == GPIO_PIN_RESET)
	{
		return 5;
	}
	return 0;
}

uint8_t key_scan()
{
	static uint8_t pressFlag = 0;	//按下标志位，用于松手检测
	uint8_t key_value = key_get_value();
	
	if(key_value != 0)
	{
		if(pressFlag == 1)
		{
			return 0;
		}
		
		delay_ms(5);	//按键消抖
		
		if(key_get_value() == key_value)
		{
			pressFlag = 1;
			return key_value;
		}
	}
	else
	{
		pressFlag = 0;
	}
	
	return 0;
}
