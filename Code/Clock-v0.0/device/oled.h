#ifndef _OLED_H_
#define _OLED_H	_		  	 


#include "stdlib.h"
#include "main.h"
#include "stm32f1xx_hal.h"
 

//OLED模式设置:4线串行模式			  
#define OLED_SCL_RESET	{HAL_GPIO_WritePin(OLED_SCL_GPIO_Port, OLED_SCL_Pin, GPIO_PIN_RESET);}
#define OLED_SCL_SET	{HAL_GPIO_WritePin(OLED_SCL_GPIO_Port, OLED_SCL_Pin, GPIO_PIN_SET);}
#define OLED_SDA_RESET	{HAL_GPIO_WritePin(OLED_SDA_GPIO_Port, OLED_SDA_Pin, GPIO_PIN_RESET);}
#define OLED_SDA_SET	{HAL_GPIO_WritePin(OLED_SDA_GPIO_Port, OLED_SDA_Pin, GPIO_PIN_SET);}
#define OLED_RSET_RESET	{HAL_GPIO_WritePin(OLED_RSET_GPIO_Port, OLED_RSET_Pin, GPIO_PIN_RESET);}
#define OLED_RSET_SET	{HAL_GPIO_WritePin(OLED_RSET_GPIO_Port, OLED_RSET_Pin, GPIO_PIN_SET);}
#define OLED_DC_RESET	{HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET);}
#define OLED_DC_SET		{HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET);}


#define OLED_WIDTH	128


//OLED控制用函数   				   		    
void oled_init(void);
void oled_fill(uint8_t data);
void oled_clear(void);
void oled_draw_point(uint8_t x, uint8_t y);
void oled_print(uint8_t x, uint8_t page, uint8_t *pstr);
void oled_print_num(uint8_t x, uint8_t page, uint8_t len, int16_t num);
void oled_print_icon(uint8_t x, uint8_t page, uint16_t icon_num);
void oled_fill_pic(uint8_t* bmp);


#endif
