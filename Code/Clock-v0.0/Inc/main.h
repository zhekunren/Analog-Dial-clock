/**
  ******************************************************************************
  * File Name          : main.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

#include "stdint.h"
#include "stdbool.h"
#include "string.h"

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define Battery_Pin GPIO_PIN_1
#define Battery_GPIO_Port GPIOA
#define KEY1_Pin GPIO_PIN_2
#define KEY1_GPIO_Port GPIOA
#define KEY2_Pin GPIO_PIN_3
#define KEY2_GPIO_Port GPIOA
#define KEY3_Pin GPIO_PIN_4
#define KEY3_GPIO_Port GPIOA
#define KEY4_Pin GPIO_PIN_5
#define KEY4_GPIO_Port GPIOA
#define KEY5_Pin GPIO_PIN_6
#define KEY5_GPIO_Port GPIOA
#define OLED_DC_Pin GPIO_PIN_12
#define OLED_DC_GPIO_Port GPIOB
#define OLED_RSET_Pin GPIO_PIN_13
#define OLED_RSET_GPIO_Port GPIOB
#define OLED_SDA_Pin GPIO_PIN_14
#define OLED_SDA_GPIO_Port GPIOB
#define OLED_SCL_Pin GPIO_PIN_15
#define OLED_SCL_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */


#define delay_ms(x)	HAL_Delay(x)


/* USER CODE END Private defines */

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
