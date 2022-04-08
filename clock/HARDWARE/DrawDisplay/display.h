#ifndef _DISPLAY_H_
#define _DISPLAY_H_


#include "main.h"
#include "draw.h"
#include "clock.h"
#include "bms.h"


void display_mode_pic(void);

void display_mode_menu_init(void);
void display_mode_menu(void);

void display_mode_timeset_init(void);
void display_mode_timeset(void);

void display_mode_alarmset_init(void);
void display_mode_alarmset(void);

void display_mode_info_init(void);
void display_mode_info(void);

void display_mode_alarmup_init(void);
void display_mode_alarmup(void);

void display_status_bar(void);

#endif
