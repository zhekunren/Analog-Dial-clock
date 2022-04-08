#include "clock.h"

extern RTC_HandleTypeDef hrtc;

clock_t Clock_state;
RTC_TimeTypeDef Clock_time;
RTC_AlarmTypeDef Clock_alarm;

RTC_TimeTypeDef Clock_time_set;
RTC_AlarmTypeDef Clock_alarm_set;

fsm_t Clock_fsm;
fsmsta_t Clock_fsmsta[] = {
		{0, NULL, clock_sta_pic},							//显示时钟
		{1, clock_sta_menu_init, clock_sta_menu},			//菜单
		{2, clock_sta_timeset_init, clock_sta_timeset},		//时间设置
		{3, clock_sta_alarmset_init, clock_sta_alarmset},	//闹钟设置
		{4, clock_sta_info_init, clock_sta_info},			//显示信息
		{5, clock_sta_alarm_up_init, clock_sta_alarm_up},	//闹钟时间到
	};

void clock_init()
{
	fsm_init(&Clock_fsm, Clock_fsmsta, sizeof(Clock_fsmsta));
}

void clock_loop()
{
	clock_get_rtc();
	clock_check_alarm();
	fsm_loop(&Clock_fsm);
}

void clock_check_alarm()
{
	if(Clock_state.is_alarm_up)
	{
		Clock_state.is_alarm_up = false;
		fsm_switch(&Clock_fsm, 5);
	}
}

void clock_get_rtc()
{
	HAL_RTC_GetTime(&hrtc, &Clock_time, RTC_FORMAT_BIN);
	HAL_RTC_GetAlarm(&hrtc, &Clock_alarm, 1, RTC_FORMAT_BIN);
}

void clock_sta_pic()
{
	display_mode_pic();
	
	uint8_t key = key_scan();
	
	if(key == 1)
	{
		fsm_switch(&Clock_fsm, 1);
	}
}

void clock_sta_menu_init()
{
	display_mode_menu_init();
}

void clock_sta_menu()
{
	display_mode_menu();
	
	uint8_t key = key_scan();
	
	switch(key)
	{
		case 1:
		{
			Clock_state.menu_pointer = 0;
			break;
		}
		case 2:
		{
			if(Clock_state.menu_pointer - 1 < 0)
			{
				Clock_state.menu_pointer = 3;
			}
			else
			{
				Clock_state.menu_pointer--;
			}
			break;
		}
		case 4:
		{
			if(Clock_state.menu_pointer + 1 >= 4)
			{
				Clock_state.menu_pointer = 0;
			}
			else
			{
				Clock_state.menu_pointer++;
			}
			break;
		}
		case 3:
		case 5:
		{
			switch(Clock_state.menu_pointer)
			{
				case 0:
				{
					fsm_switch(&Clock_fsm, 0);
					break;
				}
				case 1:
				{
					fsm_switch(&Clock_fsm, 2);
					break;
				}
				case 2:
				{
					fsm_switch(&Clock_fsm, 3);
					break;
				}
				case 3:
				{
					fsm_switch(&Clock_fsm, 4);
					break;
				}
				default:;
			}
			break;
		}
		case 0:
		default:;
	}
}

void clock_sta_info_init()
{
	display_mode_info_init();
}

void clock_sta_info()
{
	display_mode_info();
	
	uint8_t key = key_scan();
	
	if(key == 1)
	{
		fsm_switch(&Clock_fsm, 1);
	}
}

void clock_sta_timeset_init()
{
	display_mode_timeset_init();
	
	Clock_state.timeset_pointer = 0;
}

void clock_sta_timeset()
{
	display_mode_timeset();
	memcpy(&Clock_time_set, &Clock_time, sizeof(Clock_time_set));
	
	uint8_t key = key_scan();
	bool is_change = false;
	
	if(key == 1)
	{
		fsm_switch(&Clock_fsm, 1);
	}
	else if(key == 2)	//上
	{
		is_change = true;
		switch(Clock_state.timeset_pointer)
		{
			case 0:
			{
				
				if(Clock_time_set.Hours >= 23)
				{
					Clock_time_set.Hours = 0;
				}
				else
				{
					Clock_time_set.Hours++;
				}
				
				break;
			}
			case 1:
			{
				if(Clock_time_set.Minutes >= 59)
				{
					Clock_time_set.Minutes = 0;
				}
				else
				{
					Clock_time_set.Minutes++;
				}
				break;
			}
			case 2:
			{
				if(Clock_time_set.Seconds >= 59)
				{
					Clock_time_set.Seconds = 0;
				}
				else
				{
					Clock_time_set.Seconds++;
				}
				break;
			}
			default:;
		}
	}
	else if(key == 4)	//下
	{
		is_change = true;
		switch(Clock_state.timeset_pointer)
		{
			case 0:
			{
				
				if(Clock_time_set.Hours == 0)
				{
					Clock_time_set.Hours = 23;
				}
				else
				{
					Clock_time_set.Hours--;
				}
				
				break;
			}
			case 1:
			{
				if(Clock_time_set.Minutes == 0)
				{
					Clock_time_set.Minutes = 59;
				}
				else
				{
					Clock_time_set.Minutes--;
				}
				break;
			}
			case 2:
			{
				if(Clock_time_set.Seconds == 0)
				{
					Clock_time_set.Seconds = 59;
				}
				else
				{
					Clock_time_set.Seconds--;
				}
				break;
			}
			default:;
		}
	}
	else if(key == 5)
	{
		if(Clock_state.timeset_pointer < 2)
		{
			Clock_state.timeset_pointer++;
		}
	}
	else if(key == 3)
	{
		if(Clock_state.timeset_pointer > 0)
		{
			Clock_state.timeset_pointer--;
		}
	}
	
	if(is_change)
	{
		HAL_RTC_SetTime(&hrtc, &Clock_time_set, RTC_FORMAT_BIN);
	}
}

void clock_sta_alarmset_init()
{
	display_mode_alarmset_init();
	
	Clock_state.timeset_pointer = 0;
	
	memcpy(&Clock_alarm_set, &Clock_alarm, sizeof(Clock_alarm_set));
	Clock_state.is_alarm_on_shadow = Clock_state.is_alarm_on;
}

void clock_sta_alarmset()
{
	display_mode_alarmset();
	
	uint8_t key = key_scan();
	static bool is_change = false;
	
	if(key == 1)
	{
		if(is_change)
		{
			HAL_RTC_SetAlarm_IT(&hrtc, &Clock_alarm_set, RTC_FORMAT_BIN);
			Clock_state.is_alarm_on = Clock_state.is_alarm_on_shadow;
			is_change = false;
		}
		fsm_switch(&Clock_fsm, 1);
	}
	else if(key == 2)	//上
	{
		is_change = true;
		switch(Clock_state.timeset_pointer)
		{
			case 0:
			{
				if(Clock_alarm_set.AlarmTime.Hours >= 23)
				{
					Clock_alarm_set.AlarmTime.Hours = 0;
				}
				else
				{
					Clock_alarm_set.AlarmTime.Hours++;
				}
				
				break;
			}
			case 1:
			{
				if(Clock_alarm_set.AlarmTime.Minutes >= 59)
				{
					Clock_alarm_set.AlarmTime.Minutes = 0;
				}
				else
				{
					Clock_alarm_set.AlarmTime.Minutes++;
				}
				break;
			}
			case 2:
			{
				if(Clock_alarm_set.AlarmTime.Seconds >= 59)
				{
					Clock_alarm_set.AlarmTime.Seconds = 0;
				}
				else
				{
					Clock_alarm_set.AlarmTime.Seconds++;
				}
				break;
			}
			case 3:
			{
				Clock_state.is_alarm_on_shadow = !Clock_state.is_alarm_on_shadow;
				break;
			}
			default:;
		}
	}
	else if(key == 4)	//下
	{
		is_change = true;
		switch(Clock_state.timeset_pointer)
		{
			case 0:
			{
				
				if(Clock_alarm_set.AlarmTime.Hours == 0)
				{
					Clock_alarm_set.AlarmTime.Hours = 23;
				}
				else
				{
					Clock_alarm_set.AlarmTime.Hours--;
				}
				
				break;
			}
			case 1:
			{
				if(Clock_alarm_set.AlarmTime.Minutes == 0)
				{
					Clock_alarm_set.AlarmTime.Minutes = 59;
				}
				else
				{
					Clock_alarm_set.AlarmTime.Minutes--;
				}
				break;
			}
			case 2:
			{
				if(Clock_alarm_set.AlarmTime.Seconds == 0)
				{
					Clock_alarm_set.AlarmTime.Seconds = 59;
				}
				else
				{
					Clock_alarm_set.AlarmTime.Seconds--;
				}
				break;
			}
			case 3:
			{
				Clock_state.is_alarm_on_shadow = !Clock_state.is_alarm_on_shadow;
				break;
			}
			default:;
		}
	}
	else if(key == 5)
	{
		if(Clock_state.timeset_pointer < 3)
		{
			Clock_state.timeset_pointer++;
		}
	}
	else if(key == 3)
	{
		if(Clock_state.timeset_pointer > 0)
		{
			Clock_state.timeset_pointer--;
		}
	}
}

void clock_sta_alarm_up_init()
{
	display_mode_alarmup_init();
}

void clock_sta_alarm_up()
{
	display_mode_alarmup();
	
	uint8_t key = key_scan();
	
	if(key == 1 || key == 3 || key == 5)
	{
		fsm_switch(&Clock_fsm, 0);
	}
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)	//RTC闹钟中断回调函数
{
	if(Clock_state.is_alarm_on)
	{
		Clock_state.is_alarm_up = true;
	}
}
