#ifndef _DRAW_CLOCK_H_
#define _DRAW_CLOCK_H_


#include "math.h"
#include "main.h"
#include "oled.h"


#define DRAW_HIGH	(64u)
#define DRAW_WIDTH	(128u)

void draw_clear(void);
void draw_load(void);
void draw_point(uint16_t y, uint16_t x, uint8_t color);
void draw_line(uint16_t start_y, uint16_t start_x, uint16_t end_y, uint16_t end_x);
void draw_rect(uint16_t start_y, uint16_t start_x, uint16_t end_y, uint16_t end_x);
void draw_circle(uint16_t y, uint16_t x, uint16_t r);


#endif
