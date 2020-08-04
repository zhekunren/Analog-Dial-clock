#include "oled.h"
#include "oledfont.h"


static void oled_set_pos(uint8_t x, uint8_t y);
static void oled_write_cmd(uint8_t cmd);
static void oled_write_data(uint8_t data);



void oled_init(void)
{
	OLED_SDA_SET;
	OLED_SCL_SET;
	OLED_DC_SET;

	OLED_RSET_RESET;
	delay_ms(10);
	OLED_RSET_SET;

	//初始化SSD1306
	oled_write_cmd(0xae);//--turn off oled panel
	oled_write_cmd(0x00);//---set low column address
	oled_write_cmd(0x10);//---set high column address
	oled_write_cmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	oled_write_cmd(0x81);//--set contrast control register
	oled_write_cmd(0xcf); // Set SEG Output Current Brightness
	oled_write_cmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	oled_write_cmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	oled_write_cmd(0xa6);//--set normal display
	oled_write_cmd(0xa8);//--set multiplex ratio(1 to 64)
	oled_write_cmd(0x3f);//--1/64 duty
	oled_write_cmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	oled_write_cmd(0x00);//-not offset
	oled_write_cmd(0xd5);//--set display clock divide ratio/oscillator frequency
	oled_write_cmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	oled_write_cmd(0xd9);//--set pre-charge period
	oled_write_cmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	oled_write_cmd(0xda);//--set com pins hardware configuration
	oled_write_cmd(0x12);
	oled_write_cmd(0xdb);//--set vcomh
	oled_write_cmd(0x40);//Set VCOM Deselect Level
	oled_write_cmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	oled_write_cmd(0x02);//
	oled_write_cmd(0x8d);//--set Charge Pump enable/disable
	oled_write_cmd(0x14);//--set(0x10) disable
	oled_write_cmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
	oled_write_cmd(0xa6);// Disable Inverse Display On (0xa6/a7)
	oled_write_cmd(0xaf);//--turn on oled panel

	oled_clear();
}

static void oled_write_cmd(uint8_t cmd)
{
	uint8_t i;
	
	//OLED_RS = OLED_CMD; //写命令
	OLED_DC_RESET;
	
	for(i = 0; i < 8; i++)
	{
		OLED_SCL_RESET;
		if(cmd & 0x80)
		{
			OLED_SDA_SET;
		}
		else
		{
			OLED_SDA_RESET;
		}
		OLED_SCL_SET;
		cmd <<= 1;
	}
	OLED_DC_SET;
}

static void oled_write_data(uint8_t data)
{
	uint8_t i;
	
	//OLED_RS = OLED_DATA; //写命令
	OLED_DC_SET;
	
	for(i = 0; i < 8; i++)
	{
		OLED_SCL_RESET;
		if(data & 0x80)
		{
			OLED_SDA_SET;
		}
		else
		{
			OLED_SDA_RESET;
		}
		OLED_SCL_SET;
		data <<= 1;
	}
	OLED_DC_SET
}

//设置坐标
//x:0~127
//page:0~7
static void oled_set_pos(uint8_t x, uint8_t page)
{
	oled_write_cmd(0xb0 + page);	//设置页地址（0~7）
	oled_write_cmd(0x00 | (x & 0x0f));			//设置显示位置—列低地址
	oled_write_cmd(0x10 | ((x >> 4) & 0x0f));	//设置显示位置—列高地址
}

void oled_fill(uint8_t data)
{
	uint8_t	y, x;

	for(y = 0; y < 8; y++)
	{
		oled_set_pos(0, y);
		
		for(x = 0; x < OLED_WIDTH; x++)
		{
			oled_write_data(data);
		}
	}
}

void oled_clear()
{
	uint8_t	y, x;

	for(y = 0; y < 8; y++)
	{
		oled_set_pos(0, y);
		
		for(x = 0; x < OLED_WIDTH; x++)
		{
			oled_write_data(0x00);
		}
	}
}

//画点，坐标值(x,y),x的范围0～127，y的范围0～64
void oled_draw_point(uint8_t x, uint8_t y)
{
	uint8_t temp;

	oled_set_pos(x, y >> 3);
	temp = 0x01 << (y % 8);
	
	oled_write_data(temp);
}

void oled_print(uint8_t x, uint8_t page, uint8_t *pstr)
{
	uint8_t i, c, cntX = x;
	
	oled_set_pos(x, page);
	while((*pstr >= ' ') && (*pstr <= '~'))
	{
		c = *pstr - ' ';
		for(i = 0; i < 6 && cntX + i < OLED_WIDTH; i++)
		{
			oled_write_data(Font_6_8[c][i]);
		}
		cntX += 6;
		pstr++;
	}
}

//显示数字
void oled_print_num(uint8_t x, uint8_t page, uint8_t len, int16_t num)
{
	static uint8_t numTab[7];
	int8_t i;
	int8_t minusFlag = 0;
	
	if(len > 6)
	{
		len = 6;
	}
	
	if(num < 0)
	{
		num = -num;
		minusFlag = 1;
	}
	
	numTab[6] = '\0';
	for(i = 5; i >= 0; i--)
	{
		numTab[i] = num % 10 + '0';
		num /= 10;
		if(num == 0)
		{
			i--;
			break;
		}
	}

	if(minusFlag)
	{
		numTab[i--] = '-';
	}
	
	for(;i >= 0; i--)
	{
		numTab[i] = ' ';
	}
	
//	oled_print(x + (6 - len) * 6, page, numTab + i);	//右对齐
 	oled_print(x , page, numTab + (6 - len));			//左对齐
}

void oled_print_icon(uint8_t x, uint8_t page, uint16_t icon_num)
{
	uint8_t i, cntX = x;
	
	oled_set_pos(x, page);

	for(i = 0; i < 12 && cntX + i < OLED_WIDTH; i++)
	{
		oled_write_data(Icon_12_8[icon_num][i]);
	}

}

//显示图像，图像大小一定为64*128
void oled_fill_pic(uint8_t* bmp)
{
	uint8_t	y, x;
	uint16_t pos;
	uint8_t data;
	uint8_t cnt;

	for(y = 0; y < 8; y++)
	{
		oled_set_pos(0, y);
		
		for(x = 0; x < OLED_WIDTH; x++)
		{
			data = 0;
			pos = y * 1024 + x;
			for(cnt = 0; cnt < 8; cnt++)
			{
				data >>= 1;
				if(*(bmp + pos))
				{
					data |= 0x80;
				}
				pos += 128;
			}
			oled_write_data(data);
		}
	}
}
