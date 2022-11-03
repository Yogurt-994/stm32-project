/*
 * key.h
 *
 *  Created on: May 7, 2022
 *      Author: 王志涵
 */

#ifndef KEY_KEY_H_
#define KEY_KEY_H_

#include "base.h"
#include "gpio.h"
#include "stm32f4xx_hal.h"

/*********列IO操作宏定义*********/
#define Column1_OUT_Low  HAL_GPIO_WritePin(GPIOD, col1_Pin, GPIO_PIN_RESET)
#define Column2_OUT_Low  HAL_GPIO_WritePin(GPIOD, col2_Pin, GPIO_PIN_RESET)
#define Column3_OUT_Low  HAL_GPIO_WritePin(GPIOG, col3_Pin, GPIO_PIN_RESET)
#define Column4_OUT_Low  HAL_GPIO_WritePin(GPIOG, col4_Pin, GPIO_PIN_RESET)

#define Column1_OUT_High  HAL_GPIO_WritePin(GPIOD, col1_Pin, GPIO_PIN_SET)
#define Column2_OUT_High  HAL_GPIO_WritePin(GPIOD, col2_Pin, GPIO_PIN_SET)
#define Column3_OUT_High  HAL_GPIO_WritePin(GPIOG, col3_Pin, GPIO_PIN_SET)
#define Column4_OUT_High  HAL_GPIO_WritePin(GPIOG, col4_Pin, GPIO_PIN_SET)

/*********行IO操作宏定义*********/
#define Rowline1_INPUT_Read  HAL_GPIO_ReadPin(GPIOD, row1_Pin)
#define Rowline2_INPUT_Read  HAL_GPIO_ReadPin(GPIOD, row2_Pin)
#define Rowline3_INPUT_Read  HAL_GPIO_ReadPin(GPIOD, row3_Pin)
#define Rowline4_INPUT_Read  HAL_GPIO_ReadPin(GPIOD, row4_Pin)

/**********状态定义***********/
typedef enum{
	KEY_CHECK=0,	//按键检测状态
	KEY_CONFIRM,	//按键确认状态
	KEY_RELEASE		//按键释放状态
}keyState_t;		//状态枚举变量

typedef struct
{
	keyState_t keyState; //按键状态
	uint8_t keyFlag;     //按键按下标志
}key_t;                //按键状态结构体

extern uint8_t key_value;

uint8_t matrix_key_scan(void);
void keyStatusConvert(void);

#endif /* KEY_KEY_H_ */
