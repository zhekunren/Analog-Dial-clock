#ifndef _CLOCK_H_
#define _CLOCK_H_


#include "main.h"
#include "stm32f1xx_hal.h"
#include "fsm.h"
#include "display.h"
#include "key.h"


typedef struct COLOK_STRUCT
{
	bool is_alarm_on;		//闹钟使能
	bool is_alarm_up;		//闹钟时间到标志
	bool is_alarm_on_shadow;	//闹钟使能，在设置中使用，设置完成后写入is_alarm_on
	int8_t menu_pointer;
	int8_t timeset_pointer;
}clock_t;


void clock_init(void);
void clock_loop(void);

void clock_sta_pic(void);
void clock_sta_menu_init(void);
void clock_sta_menu(void);
void clock_sta_timeset_init(void);
void clock_sta_timeset(void);
void clock_sta_alarmset_init(void);
void clock_sta_alarmset(void);
void clock_sta_info_init(void);
void clock_sta_info(void);
void clock_sta_alarm_up_init(void);
void clock_sta_alarm_up(void);

void clock_check_alarm(void);
void clock_get_rtc(void);


extern clock_t Clock_state;
extern RTC_TimeTypeDef Clock_time;
extern RTC_AlarmTypeDef Clock_alarm;
extern RTC_TimeTypeDef Clock_time_set;
extern RTC_AlarmTypeDef Clock_alarm_set;


#endif
