#include "clock.h"

extern RTC_HandleTypeDef hrtc;

clock_t Clock_state;
RTC_TimeTypeDef Clock_time;
RTC_AlarmTypeDef Clock_alarm;
RTC_TimeTypeDef Clock_time_set;
RTC_AlarmTypeDef Clock_alarm_set;

fsm_t Clock_fsm;
fsmsta_t Clock_fsmsta[] = {
		{0, NULL, clock_sta_pic},	/*****状态0--显示时钟*****/
		{1, clock_sta_menu_init, clock_sta_menu},			/*****状态1--菜单*****/
		{2, clock_sta_timeset_init, clock_sta_timeset},		/*****状态2--时间设置*****/
		{3, clock_sta_alarmset_init, clock_sta_alarmset},	/*****状态3--闹钟设置*****/
		{4, clock_sta_info_init, clock_sta_info},			/*****状态4--显示信息*****/
		{5, clock_sta_alarm_up_init, clock_sta_alarm_up},	/*****状态5--闹钟时间到*****/
};

uint8_t Draw_bmp[DRAW_HIGH][DRAW_WIDTH];//存储待显示图片
/*****状态0--显示时钟*****/
void clock_sta_pic()
{
	display_mode_pic();
	
	uint8_t key = key_scan();
	
	if(key == 1)
	{
		fsm_switch(&Clock_fsm, 1);
	}
}

void draw_point(uint16_t y, uint16_t x, uint8_t color)
{
	//color=1点亮这个点
	if(y < DRAW_HIGH)
	{
		if(x < DRAW_WIDTH)
		{
			Draw_bmp[y][x] = color;
		}
	}
}
void draw_line(uint16_t start_y, uint16_t start_x, uint16_t end_y, uint16_t end_x)
{
	uint16_t x;
	uint16_t y;
	uint16_t step;
	uint16_t step_total;
	
	//step_max是总步数，这里总步数为x、y方向长度的大值
	if(abs(end_y - start_y) > abs(end_x - start_x))
	{
		step_total = abs(end_y - start_y);
	}
	else
	{
		step_total = abs(end_x - start_x);
	}
	
	for(step = 0; step <= step_total; step++)
	{
		x = roundf(start_x + (end_x - start_x) * step / (float)step_total);//四舍五入函数（roundf）
		y = roundf(start_y + (end_y - start_y) * step / (float)step_total);
		draw_point(y, x, 1);
	}
}
void draw_circle(uint16_t oy, uint16_t ox, uint16_t r)
{
	float r2 = r * r;	//r半径平方
	uint16_t x;
	uint16_t y;
	
	for(x = r, y = 0; y <= x; y++)
	{
		x = (roundf)(sqrtf(r2 - y * y));	//四舍五入函数（roundf）

		draw_point(y + oy, x + ox, 1);
		draw_point(-y + oy, x + ox, 1);
		draw_point(y + oy, -x + ox, 1);
		draw_point(-y + oy, -x + ox, 1);
		
		draw_point(x + oy, y + ox, 1);
		draw_point(-x + oy, y + ox, 1);
		draw_point(x + oy, -y + ox, 1);
		draw_point(-x + oy, -y + ox, 1);
	}
}
void draw_clear()
{
	//memset 一般使用“0”初始化内存单元，而且通常是给数组或结构体进行初始化
	memset(Draw_bmp, false, sizeof(Draw_bmp));
}
//圆心(ox,oy)，起始与结束半径(start_r/end_r),角度(angle)
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
void display_mode_pic()
{
	draw_clear();

	draw_circle(32, 63, 31);
	draw_circle(32, 63, 2);
	
	uint16_t pin;
	for(pin = 0; pin < 360; pin += 360/12)
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
	
	oled_fill_pic((uint8_t*)&Draw_bmp);
}
/*****状态1--菜单*****/
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
		case 2://上
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
		case 4://下
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
		case 3: //返回
			if(Clock_state.menu_pointer==0)
			{
					fsm_switch(&Clock_fsm, 0);
					break;
			}else{
					break;
			}
		case 5://前进
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

void display_mode_menu_init()
{
	oled_clear();
	
	oled_print(50, 0,(uint8_t*)"Menu");
	
	oled_print(0, 2,(uint8_t*)"  c1: Clock Display");
	oled_print(0, 3,(uint8_t*)"  c2: Time Setting");
	oled_print(0, 4,(uint8_t*)"  c3: Alarm Setting");
	oled_print(0, 5,(uint8_t*)"  c4: Infomation");
	
	oled_print_icon(115, 7, 1);	//前进图标
}

void display_mode_menu()
{
	if(Clock_state.menu_pointer==0){
		oled_print_icon(0, 7, 0);	//返回图标
	}else{
		oled_print(0, 7,(uint8_t*)"  ");
	}
	for(uint8_t p = 0; p < 4; p++)
	{
		if(p == Clock_state.menu_pointer)
		{
			oled_print(0, 2 + p,(uint8_t *)">");
		}
		else
		{
			oled_print(0, 2 + p,(uint8_t *)" ");
		}
	}
}
/*****状态2--时间设置*****/
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
	
	if(key == 1)//退出回主菜单
	{
		is_change = false;
		fsm_switch(&Clock_fsm, 1);
	}
	else if(key == 2)	//上：时间数值加
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
	else if(key == 4)	//下：时间数值减
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
	else if(key == 5)//右：位置加
	{
		if(Clock_state.timeset_pointer < 2)
		{
			Clock_state.timeset_pointer++;
		}else{
			Clock_state.timeset_pointer=0;
		}
	}
	else if(key == 3)//左：位置减
	{
		if(Clock_state.timeset_pointer > 0)
		{
			Clock_state.timeset_pointer--;
		}else{
			Clock_state.timeset_pointer=2;
		}
	}
	
	if(is_change)
	{
		HAL_RTC_SetTime(&hrtc, &Clock_time_set, RTC_FORMAT_BIN);
	}
}

void display_mode_timeset_init()
{
	oled_clear();
	
	oled_print(0, 0,(uint8_t*)"Time setting");
	
	oled_print(0, 2, (uint8_t*)"Time :   :  :");
	oled_print(0, 4, (uint8_t*)"Alarm:   :  :");
	
}

void display_mode_timeset()
{
	const uint8_t pos_tab[] = {42, 60, 78};
	
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
	
	oled_print_num(42, 4, 2, Clock_alarm.AlarmTime.Hours);
	oled_print_num(60, 4, 2, Clock_alarm.AlarmTime.Minutes);
	oled_print_num(78, 4, 2, Clock_alarm.AlarmTime.Seconds);
	if(Clock_state.is_alarm_on_shadow)
	{
		oled_print(100, 4, (uint8_t*)"ON ");
	}
	else
	{
		oled_print(100,4, (uint8_t*)"OFF");
	}
}
/*****状态3--闹钟设置*****/
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
		if(is_change)//更新闹钟并退出
		{
			HAL_RTC_SetAlarm_IT(&hrtc, &Clock_alarm_set, RTC_FORMAT_BIN);
			Clock_state.is_alarm_on = Clock_state.is_alarm_on_shadow;
			is_change = false;
		}
		fsm_switch(&Clock_fsm, 1);
	}
	else if(key == 2)	//上：闹钟数值加
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
	else if(key == 4)	//下：闹钟数值减
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
	else if(key == 5)//右：位置加
	{
		if(Clock_state.timeset_pointer < 3)
		{
			Clock_state.timeset_pointer++;
		}
	}
	else if(key == 3)//左：位置减
	{
		if(Clock_state.timeset_pointer > 0)
		{
			Clock_state.timeset_pointer--;
		}
	}
}

void display_mode_alarmset_init()
{
	oled_clear();
	
	oled_print(0, 0,(uint8_t*)"Alarm setting");
	
	oled_print(0, 2, (uint8_t*)"Time :   :  :");
	oled_print(0, 3, (uint8_t*)"Alarm:   :  :");
	
}

void display_mode_alarmset()
{
	const uint8_t pos_tab[] = {42, 60, 78, 100};

	oled_print_num(42, 2, 2, Clock_time.Hours);
	oled_print_num(60, 2, 2, Clock_time.Minutes);
	oled_print_num(78, 2, 2, Clock_time.Seconds);
	
	oled_print_num(42, 3, 2, Clock_alarm_set.AlarmTime.Hours);
	oled_print_num(60, 3, 2, Clock_alarm_set.AlarmTime.Minutes);
	oled_print_num(78, 3, 2, Clock_alarm_set.AlarmTime.Seconds);

	if(Clock_state.is_alarm_on_shadow)
	{
		oled_print(100, 3, (uint8_t*)"ON ");
	}
	else
	{
		oled_print(100,3, (uint8_t*)"OFF");
	}
	
	for(uint8_t p = 0; p < 4; p++)
	{
		if(p == Clock_state.timeset_pointer)
		{
			oled_print(pos_tab[p] + 6, 4,(uint8_t*)"^");
		}
		else
		{
			oled_print(pos_tab[p] + 6, 4,(uint8_t*)" ");
		}
	}
}

/*****状态4--显示信息*****/
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

void display_mode_info_init()
{
	oled_clear();
	
	oled_print(0, 0,(uint8_t*)"Infomation");
	
	oled_print(0, 2, (uint8_t*)"Time :   :  :");
	oled_print(0, 3, (uint8_t*)"Alarm:   :  :");
	
	oled_print(0, 5, (uint8_t*)"  Made by Eric Ren ");
	oled_print(0, 6, (uint8_t*)"SchoolID:5120154316");
	
}

void display_mode_info()
{
	
	oled_print_num(42, 2, 2, Clock_time.Hours);
	oled_print_num(60, 2, 2, Clock_time.Minutes);
	oled_print_num(78, 2, 2, Clock_time.Seconds);
	
	oled_print_num(42, 3, 2, Clock_alarm.AlarmTime.Hours);
	oled_print_num(60, 3, 2, Clock_alarm.AlarmTime.Minutes);
	oled_print_num(78, 3, 2, Clock_alarm.AlarmTime.Seconds);
	
	
	if(Clock_state.is_alarm_on)
	{
		oled_print(100, 3, (uint8_t*)"ON ");
	}
	else
	{
		oled_print(100, 3, (uint8_t*)"OFF");
	}
}
/*****状态5--闹钟时间到*****/
void clock_sta_alarm_up_init()
{
	display_mode_alarmup_init();
}

void clock_sta_alarm_up()
{
	display_mode_alarmup();
	
	uint8_t key = key_scan();
	
	if(key == 1)
	{
		fsm_switch(&Clock_fsm, 0);
	}
}

void display_mode_alarmup_init()
{
	oled_clear();
	
	oled_print(0, 0,(uint8_t*)"Alarm");
	
	oled_print(12, 2,(uint8_t*)"> Time is up!!! <");
	
	oled_print(0, 4, (uint8_t*)"Time :   :  :");
	oled_print(0, 5, (uint8_t*)"Alarm:   :  :");
	
}

void display_mode_alarmup()
{
	oled_print_num(42, 4, 2, Clock_time.Hours);
	oled_print_num(60, 4, 2, Clock_time.Minutes);
	oled_print_num(78, 4, 2, Clock_time.Seconds);
	
	oled_print_num(42, 5, 2, Clock_alarm.AlarmTime.Hours);
	oled_print_num(60, 5, 2, Clock_alarm.AlarmTime.Minutes);
	oled_print_num(78, 5, 2, Clock_alarm.AlarmTime.Seconds);
}


/********************alarm_clock_fuction********************/
void clock_init()
{
	fsm_init(&Clock_fsm, Clock_fsmsta, sizeof(Clock_fsmsta));
}

void clock_get_rtc()
{
	HAL_RTC_GetTime(&hrtc, &Clock_time, RTC_FORMAT_BIN);
	HAL_RTC_GetAlarm(&hrtc, &Clock_alarm, 1, RTC_FORMAT_BIN);
}
void clock_check_alarm()
{
	if(Clock_state.is_alarm_up)
	{
		Clock_state.is_alarm_up = false;
		fsm_switch(&Clock_fsm, 5);
	}
}
void clock_loop()
{
	clock_get_rtc();
	clock_check_alarm();
	fsm_loop(&Clock_fsm);
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)	//RTC闹钟中断回调函数
{
	if(Clock_state.is_alarm_on)
	{
		Clock_state.is_alarm_up = true;
	}
}
