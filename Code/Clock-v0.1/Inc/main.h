#ifndef __MAIN_H
#define __MAIN_H

/* USER CODE BEGIN Includes */
#include "stdint.h"
#include "stdbool.h"
#include "string.h"
/* USER CODE END Includes */

/* Private define */
#define KEY1_Pin GPIO_PIN_4
#define KEY1_GPIO_Port GPIOA
#define KEY2_Pin GPIO_PIN_6
#define KEY2_GPIO_Port GPIOA
#define KEY3_Pin GPIO_PIN_3
#define KEY3_GPIO_Port GPIOA
#define KEY4_Pin GPIO_PIN_2
#define KEY4_GPIO_Port GPIOA
#define KEY5_Pin GPIO_PIN_5
#define KEY5_GPIO_Port GPIOA
#define OLED_DC_Pin GPIO_PIN_12
#define OLED_DC_GPIO_Port GPIOB
#define OLED_RSET_Pin GPIO_PIN_13
#define OLED_RSET_GPIO_Port GPIOB
#define OLED_SDA_Pin GPIO_PIN_14
#define OLED_SDA_GPIO_Port GPIOB
#define OLED_SCL_Pin GPIO_PIN_15
#define OLED_SCL_GPIO_Port GPIOB

#define delay_ms(x)	HAL_Delay(x)

#endif 
