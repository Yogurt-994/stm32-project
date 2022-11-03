/*
 * pid.h
 *
 *  Created on: May 6, 2022
 *      Author: 王志涵
 */

#ifndef INC_PID_H_
#define INC_PID_H_

#include "stm32f4xx_hal.h"
#include "arm_math.h"
#include "arm_common_tables.h"
#include "arm_const_structs.h"

#define BUF_LEN 			400			//采样数组长度
#define PWM_PERIOD_CCR1		8000		//PWM周期-计数值
#define ERR_LIMIT			0.0005		//误差限制
#define LOCK 		HAL_GPIO_WritePin(LOCK_Pin_GPIO_Port, LOCK_Pin_Pin, GPIO_PIN_RESET)
#define UNLOCK 		HAL_GPIO_WritePin(LOCK_Pin_GPIO_Port, LOCK_Pin_Pin, GPIO_PIN_SET)

typedef struct{
    arm_pid_instance_f32  S;
    float                 out;
}PidCtrlTypedef;						//pid调节结构体

volatile extern uint8_t dma_cpl_flag;		//dma电压传输完成标志
volatile extern uint8_t dma2_cpl_flag;		//dma电流传输完成标志

extern uint16_t adc_raw[BUF_LEN];			//adc原始电压采样值
extern uint16_t adc_raw_copy[BUF_LEN];		//adc原始电压采样值备份
extern uint16_t adc2_raw[BUF_LEN];			//adc原始电流采样值
extern uint16_t adc2_raw_copy[BUF_LEN];		//adc原始电流采样值备份

extern float cur_vol;						//当前电压
extern float cur_cur;						//当前电流

extern float DES_VOL;			//目标电压
extern float PRO_Cur;			//过流保护点


/* UI界面 */
void UI(void);
/* 根据原始采样值计算PWM等效电压值 */
float calVol();
/* 设置输出电压 */
void setVol(void);
/* 根据原始采样值计算电流值 */
float calCur();
/* 设置过流保护点 */
void setCurProtect(void);
/* PID初始化 */
void pidInit();
/* PID执行 */
void pidExecu(float vol);

void core(void);

#endif /* INC_PID_H_ */
