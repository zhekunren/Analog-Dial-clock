/***********************************
文件：draw.c
作者：张涵宇
版本：
日期：2016/11/9
描述：在缓存中画图
************************************/


#include "draw.h"


uint8_t Draw_bmp[DRAW_HIGH][DRAW_WIDTH];

void draw_clear()
{
	memset(Draw_bmp, false, sizeof(Draw_bmp));
}

void draw_load()
{
	oled_fill_pic((uint8_t*)&Draw_bmp);
}

void draw_point(uint16_t y, uint16_t x, uint8_t color)
{
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
		x = roundf(start_x + (end_x - start_x) * step / (float)step_total);
		y = roundf(start_y + (end_y - start_y) * step / (float)step_total);
		draw_point(y, x, 1);
	}
}

void draw_rect(uint16_t start_y, uint16_t start_x, uint16_t end_y, uint16_t end_x)
{
	draw_line(start_y, start_x, end_y, start_x);
	draw_line(start_y, end_x, end_y, end_x);
	draw_line(start_y, start_x, start_y, end_x);
	draw_line(end_y, start_x, end_y, end_x);
}

void draw_circle(uint16_t oy, uint16_t ox, uint16_t r)
{
	float r2 = r * r;	//r半径平方
	uint16_t x;
	uint16_t y;
	
	for(x = r, y = 0; y <= x; y++)
	{
		x = (uint16_t)(sqrtf(r2 - y * y) + 0.5f);	//+0.5是为了四舍五入

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
