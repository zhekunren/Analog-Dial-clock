#ifndef _CLOCK_H_
#define _CLOCK_H_

#include "main.h"
#include "stm32f1xx_hal.h"
#include "fsm.h"
#include "key.h"
#include "math.h"
#include "oled.h"

#define DRAW_HIGH	(64u)
#define DRAW_WIDTH	(128u)

typedef struct COLOK_STRUCT
{
	bool is_alarm_on;		//闹钟使能
	bool is_alarm_up;		//闹钟时间到标志
	bool is_alarm_on_shadow;	//闹钟使能，在设置中使用，设置完成后写入is_alarm_on
	int8_t menu_pointer;
	int8_t timeset_pointer;
}clock_t;

extern clock_t Clock_state;
extern RTC_TimeTypeDef Clock_time;
extern RTC_AlarmTypeDef Clock_alarm;
extern RTC_TimeTypeDef Clock_time_set;
extern RTC_AlarmTypeDef Clock_alarm_set;

void clock_check_alarm(void);
void clock_get_rtc(void);
void clock_init(void);
void clock_loop(void);

/*****状态0--显示时钟*****/
void clock_sta_pic(void);

void draw_point(uint16_t y, uint16_t x, uint8_t color);
void draw_circle(uint16_t y, uint16_t x, uint16_t r);
void draw_clear(void);
void draw_line(uint16_t start_y, uint16_t start_x, uint16_t end_y, uint16_t end_x);
void display_mode_pic(void);
/*****状态1--菜单*****/
void clock_sta_menu_init(void);
void clock_sta_menu(void);

void display_mode_menu_init(void);
void display_mode_menu(void);
/*****状态3--时间设置*****/
void clock_sta_timeset_init(void);
void clock_sta_timeset(void);

void display_mode_timeset_init(void);
void display_mode_timeset(void);
/*****状态4--闹钟设置*****/
void clock_sta_alarmset_init(void);
void clock_sta_alarmset(void);

void display_mode_alarmset_init(void);
void display_mode_alarmset(void);
/*****状态5--显示信息*****/
void clock_sta_info_init(void);
void clock_sta_info(void);

void display_mode_info_init(void);
void display_mode_info(void);
/*****状态6--闹钟时间到*****/
void clock_sta_alarm_up_init(void);
void clock_sta_alarm_up(void);

void display_mode_alarmup_init(void);
void display_mode_alarmup(void);

#endif
