/*
 * key.c
 *
 *  Created on: May 7, 2022
 *      Author: 王志涵
 */
#include "key.h"
#include "pid.h"
#include "oled.h"
#include "base.h"
#include "main.h"
#include "tim.h"

key_t Key = {
	KEY_CHECK,0
};//按键状态结构体初始化

uint16_t ShakeTime = 0;		//全局变量
uint8_t key_value = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
	if (htim->Instance == htim4.Instance) {
		ShakeTime++;

		keyStatusConvert();

		setVol();			//设置输出电压
		setCurProtect();	//设置过流保护点



		//这个的作用是防止计数溢出。对这个全局变量清零的比较频繁的话可以不用这段代码
		if (ShakeTime >= 32768) {
			ShakeTime = 0;
		}
	}
}

void keyStatusConvert(void){
	switch (Key.keyState) {
		case KEY_CHECK: {
			if ((Rowline1_INPUT_Read == GPIO_PIN_RESET)||(Rowline2_INPUT_Read == GPIO_PIN_RESET)||(Rowline3_INPUT_Read == GPIO_PIN_RESET)||( Rowline4_INPUT_Read == GPIO_PIN_RESET)) {
				Key.keyState = KEY_CONFIRM;
			}
			break;
		}
		case KEY_CONFIRM: {
			if ((Rowline1_INPUT_Read == GPIO_PIN_RESET)||(Rowline2_INPUT_Read == GPIO_PIN_RESET)||(Rowline3_INPUT_Read == GPIO_PIN_RESET)||( Rowline4_INPUT_Read == GPIO_PIN_RESET)) {
				Key.keyState = KEY_RELEASE;
				//读到行低电平，按键确实按下，按键标志位置1，并进入按键释放状态
				Key.keyFlag = 1;
				Key.keyState = KEY_RELEASE;
			}
			else
			{
				Key.keyState = KEY_CHECK;
			}
			break;
		}
		case KEY_RELEASE: {
			key_value = matrix_key_scan();
			Key.keyState = KEY_CHECK;
			Key.keyFlag = 0;
		}
	}
}

uint8_t matrix_key_scan(void) {
	volatile uint8_t i = 0, keynum = 0;
	if (Rowline1_INPUT_Read == GPIO_PIN_RESET) {			//当有按键按下
		for (i = 1; i <= 4; i++) {
			if (i == 1) Column1_OUT_High;		//逐个拉高每列的IO
			else if (i == 2) Column2_OUT_High;
			else if (i == 3) Column3_OUT_High;
			else if (i == 4) Column4_OUT_High;
			if (Rowline1_INPUT_Read == GPIO_PIN_SET) {	//如果检测到行IO电平跟着变化，则判断为该按键按下
				keynum = i;
				break;
			}
		}
	}
	else if (Rowline2_INPUT_Read == GPIO_PIN_RESET) {			//当有按键按下
		for (i = 1; i <= 4; i++) {
			if (i == 1) Column1_OUT_High;		//逐个拉高每列的IO
			else if (i == 2) Column2_OUT_High;
			else if (i == 3) Column3_OUT_High;
			else if (i == 4) Column4_OUT_High;
			if (Rowline2_INPUT_Read == GPIO_PIN_SET) {	//如果检测到行IO电平跟着变化，则判断为该按键按下
				keynum = i + 4;
				break;
			}
		}
	}
	else if (Rowline3_INPUT_Read == GPIO_PIN_RESET) {			//当有按键按下
		for (i = 1; i <= 4; i++) {
			if (i == 1) Column1_OUT_High;		//逐个拉高每列的IO
			else if (i == 2) Column2_OUT_High;
			else if (i == 3) Column3_OUT_High;
			else if (i == 4) Column4_OUT_High;
			if (Rowline3_INPUT_Read == GPIO_PIN_SET) {	//如果检测到行IO电平跟着变化，则判断为该按键按下
				keynum = i + 8;
				break;
			}
		}
	}
	else if (Rowline4_INPUT_Read == GPIO_PIN_RESET) {			//当有按键按下
		for (i = 1; i <= 4; i++) {
			if (i == 1) Column1_OUT_High;		//逐个拉高每列的IO
			else if (i == 2) Column2_OUT_High;
			else if (i == 3) Column3_OUT_High;
			else if (i == 4) Column4_OUT_High;
			if (Rowline4_INPUT_Read == GPIO_PIN_SET) {	//如果检测到行IO电平跟着变化，则判断为该按键按下
				keynum = i + 12;
				break;
			}
		}
	}
	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOD, col1_Pin | col2_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOG, col3_Pin | col4_Pin, GPIO_PIN_RESET);
	return keynum;
}


