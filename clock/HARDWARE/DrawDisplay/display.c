/***********************************
文件：display.c
描述：界面显示
************************************/

#include "display.h"


void draw_clock_line(uint16_t oy, uint16_t ox, uint16_t start_r, uint16_t end_r,  int16_t angle)
{
	int16_t start_x;
	int16_t start_y;
	int16_t end_x;
	int16_t end_y;
	
	angle = 90 - angle;
	
	float cosx = cosf(angle * 3.14159f / 180.0f);
	float sinx = sinf(angle * 3.14159f / 180.0f);
	
	start_x = roundf(start_r * cosx) + ox;
	start_y = -roundf(start_r * sinx) + oy;
	
	end_x = roundf(end_r * cosx) + ox;
	end_y = -roundf(end_r * sinx) + oy;
	
	draw_line(start_y, start_x, end_y , end_x);
}

void draw_clock_pin(uint16_t oy, uint16_t ox, uint16_t r, int16_t angle)
{
	int16_t x;
	int16_t y;
	
	angle = 90 - angle;
	
	x = roundf(r * cosf(angle * 3.14159f / 180.0f));
	y = roundf(r * sinf(angle * 3.14159f / 180.0f));
	
	draw_line(oy, ox, oy - y , ox + x);
}

void display_mode_pic()
{
	draw_clear();

	draw_circle(32, 63, 31);
	draw_circle(32, 63, 2);
	
	uint16_t pin;
	for(pin = 0; pin < 360; pin += 30)
	{
		if(pin % 90 == 0)
		{
			draw_clock_line(32, 63, 27, 30, pin);
		}
		else
		{
			draw_clock_line(32, 63, 29, 30, pin);
		}
	}
	
	draw_clock_line(32, 63, 3, 14, Clock_time.Hours * 360 / 12 + Clock_time.Minutes * 30 / 60);
	draw_clock_line(32, 63, 3, 19, Clock_time.Minutes * 360 / 60);
	draw_clock_line(32, 63, 3, 24, Clock_time.Seconds * 360 / 60);
	
	draw_load();
}

void display_mode_timeset_init()
{
	oled_clear();
	
	oled_print_icon(0, 0, 11);	//设置图标
	oled_print(15, 0,(uint8_t*)"Time setting");
	
	oled_print(0, 2, (uint8_t*)"Time :   :  :");
	
	oled_print_icon(0, 7, 0);	//返回图标
}

void display_mode_timeset()
{
	const uint8_t pos_tab[] = {42, 60, 78};

	display_status_bar();
	
	oled_print_num(42, 2, 2, Clock_time.Hours);
	oled_print_num(60, 2, 2, Clock_time.Minutes);
	oled_print_num(78, 2, 2, Clock_time.Seconds);
	
	for(uint8_t p = 0; p < 3; p++)
	{
		if(p == Clock_state.timeset_pointer)
		{
			oled_print(pos_tab[p] + 6, 3,(uint8_t*)"^");
		}
		else
		{
			oled_print(pos_tab[p] + 6, 3,(uint8_t*)" ");
		}
	}
}

void display_mode_alarmset_init()
{
	oled_clear();
	
	oled_print_icon(0, 0, 10);	//设置图标
	oled_print(15, 0,(uint8_t*)"Alarm setting");
	
	oled_print(0, 2, (uint8_t*)"Alarm:   :  :");
	
	oled_print_icon(0, 7, 0);	//返回图标
}

void display_mode_alarmset()
{
	const uint8_t pos_tab[] = {42, 60, 78, 100};

	display_status_bar();
	
	oled_print_num(42, 2, 2, Clock_alarm_set.AlarmTime.Hours);
	oled_print_num(60, 2, 2, Clock_alarm_set.AlarmTime.Minutes);
	oled_print_num(78, 2, 2, Clock_alarm_set.AlarmTime.Seconds);

	if(Clock_state.is_alarm_on_shadow)
	{
		oled_print(100, 2, (uint8_t*)"ON ");
	}
	else
	{
		oled_print(100, 2, (uint8_t*)"OFF");
	}
	
	for(uint8_t p = 0; p < 4; p++)
	{
		if(p == Clock_state.timeset_pointer)
		{
			oled_print(pos_tab[p] + 6, 3,(uint8_t*)"^");
		}
		else
		{
			oled_print(pos_tab[p] + 6, 3,(uint8_t*)" ");
		}
	}
}

void display_mode_menu_init()
{
	oled_clear();
	
	oled_print_icon(0, 0, 12);	//菜单图标
	oled_print(15, 0,(uint8_t*)"Menu");
	
	oled_print(0, 2,(uint8_t*)"     Clock");
	oled_print(0, 3,(uint8_t*)"     Time Setting");
	oled_print(0, 4,(uint8_t*)"     Alarm Setting");
	oled_print(0, 5,(uint8_t*)"     Info");
	
	//电池图标
// 	oled_print_icon(115, 0, 2);	//电池（4/4）图标
// 	oled_print_icon(115, 1, 3);	//电池（3/4）图标
// 	oled_print_icon(115, 2, 4);	//电池（2/4）图标
// 	oled_print_icon(115, 3, 5);	//电池（1/4）图标
// 	oled_print_icon(115, 4, 6);	//电池（0/4）图标
// 	oled_print_icon(115, 5, 7);	//电池（充电）图标
	
	//菜单们的图标
	oled_print_icon(12, 2, 8);	//时钟图标
	oled_print_icon(12, 3, 11);	//设置图标
	oled_print_icon(12, 4, 10);	//闹钟图标
	oled_print_icon(12, 5, 9);	//信息图标
	
	oled_print_icon(0, 7, 0);	//返回图标
	oled_print_icon(115, 7, 1);	//前进图标
}

void display_mode_menu()
{
	display_status_bar();
	
	for(uint8_t p = 0; p < 4; p++)
	{
		if(p == Clock_state.menu_pointer)
		{
			oled_print(0, 2 + p,(uint8_t*)">");
		}
		else
		{
			oled_print(0, 2 + p,(uint8_t*)" ");
		}
	}
}

void display_mode_info_init()
{
	oled_clear();
	
	oled_print(15, 0,(uint8_t*)"Info");
	oled_print_icon(0, 0, 9);	//信息图标
	
	oled_print(0, 2, (uint8_t*)"Time :   :  :");
	oled_print(0, 3, (uint8_t*)"Alarm:   :  :");
	oled_print(0, 4, (uint8_t*)"Battery:     mV");
	
	oled_print_icon(0, 7, 0);	//返回图标
}

void display_mode_info()
{
	display_status_bar();
	
	oled_print_num(42, 2, 2, Clock_time.Hours);
	oled_print_num(60, 2, 2, Clock_time.Minutes);
	oled_print_num(78, 2, 2, Clock_time.Seconds);
	
	oled_print_num(42, 3, 2, Clock_alarm.AlarmTime.Hours);
	oled_print_num(60, 3, 2, Clock_alarm.AlarmTime.Minutes);
	oled_print_num(78, 3, 2, Clock_alarm.AlarmTime.Seconds);
	
	oled_print_num(46, 4, 5, Battery_info.voltage);
	
	
	if(Clock_state.is_alarm_on)
	{
		oled_print(100, 3, (uint8_t*)"ON ");
	}
	else
	{
		oled_print(100, 3, (uint8_t*)"OFF");
	}
}

void display_mode_alarmup_init()
{
	oled_clear();
	
	oled_print(15, 0,(uint8_t*)"Alarm");
	oled_print_icon(0, 0, 10);	//信息图标
	
	oled_print(12, 2,(uint8_t*)"> Time is up!!! <");
	
	oled_print(0, 4, (uint8_t*)"Time :   :  :");
	oled_print(0, 5, (uint8_t*)"Alarm:   :  :");
	
	oled_print_icon(0, 7, 0);	//返回图标
}

void display_mode_alarmup()
{
	display_status_bar();
	
	oled_print_num(42, 4, 2, Clock_time.Hours);
	oled_print_num(60, 4, 2, Clock_time.Minutes);
	oled_print_num(78, 4, 2, Clock_time.Seconds);
	
	oled_print_num(42, 5, 2, Clock_alarm.AlarmTime.Hours);
	oled_print_num(60, 5, 2, Clock_alarm.AlarmTime.Minutes);
	oled_print_num(78, 5, 2, Clock_alarm.AlarmTime.Seconds);
}

void display_status_bar()
{
	if(Battery_info.voltage > 4000)
	{
		oled_print_icon(115, 0, 2);	//电池（4/4）图标
	}
	else if(Battery_info.voltage > 3800)
	{
		oled_print_icon(115, 0, 3);	//电池（3/4）图标
	}
	else if(Battery_info.voltage > 3600)
	{
		oled_print_icon(115, 0, 4);	//电池（2/4）图标
	}
	else if(Battery_info.voltage > 3400)
	{
		oled_print_icon(115, 0, 5);	//电池（1/4）图标
	}
	else
	{
		oled_print_icon(115, 0, 6);	//电池（0/4）图标
	}
	//oled_print_icon(115, 5, 7);	//电池（充电）图标
	
	if(Clock_state.is_alarm_on)
	{
		oled_print_icon(100, 0, 10);	//闹钟图标
	}
}
