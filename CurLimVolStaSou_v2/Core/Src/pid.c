/*
 * pid.c
 *
 *  Created on: May 6, 2022
 *      Author: 王志涵
 */

#include "pid.h"
#include "stm32f4xx_hal.h"
#include "arm_math.h"
#include "arm_common_tables.h"
#include "arm_const_structs.h"
#include "adc.h"
#include "tim.h"
#include "oled.h"
#include "key.h"

PidCtrlTypedef pidCtrl;					//pid调节实例

volatile uint8_t dma_cpl_flag = 0;		//dma电压传输完成标志
volatile uint8_t dma2_cpl_flag = 0;		//dma电流传输完成标志

uint16_t adc_raw[BUF_LEN] = {0};		//adc原始采样值
uint16_t adc_raw_copy[BUF_LEN] = {0};	//adc原始采样值备份
uint16_t adc2_raw[BUF_LEN] = {0};		//adc原始电流采样值
uint16_t adc2_raw_copy[BUF_LEN] = {0};	//adc原始电流采样值备份

float cur_vol = 0.000000;						//当前电压
float cur_cur = 0.000000;						//当前电流

float DES_VOL = 3.000000;					//目标电压
float PRO_Cur = 2.000000;					//过流保护点


/* 以下为过流故障时所需要的变量 */
int pidFlag = 0;				//pid标志位
int overCur = 0;				//过流保护点标志
uint32_t faultDuty;				//过流保护故障时设置输出PWM的占空比
uint32_t curDuty;				//当前占空比
int step;						//pwm步进值

void UI(void){
	OLED_ShowString(0,0,(uint8_t*)("DES_VOL:"),12);
	OLED_ShowChar(95,0,(uint8_t)('V'),12);
	OLED_ShowString(0,12,(uint8_t*)("CUR_VOL:"),12);
	OLED_ShowChar(95,12,(uint8_t)('V'),12);
	OLED_ShowString(0,24,(uint8_t*)("PRO_CUR:"),12);
	OLED_ShowChar(95,24,(uint8_t)('A'),12);
	OLED_ShowString(0,36,(uint8_t*)("CUR_CUR:"),12);
	OLED_ShowChar(95,36,(uint8_t)('A'),12);
	OLED_ShowString(0,48,(uint8_t*)("PWM_Duty:"),12);
	OLED_ShowChar(80,48,(uint8_t)('%'),12);
	OLED_Refresh();
}

/**
 * @brief PID初始化
 * @retval None
 */
void pidInit()
{
    pidCtrl.S.Kp = 0.12;
    pidCtrl.S.Ki=0.07;
    pidCtrl.S.Kd = 0.06;
    arm_pid_init_f32(&pidCtrl.S,1);

    pidCtrl.out = 0;
}

/**
 * @biref PID执行
 * @retval None
 */
void pidExecu(float vol)
{
    float pidErr;
    float desVolConvert = DES_VOL/6.000000;
    uint32_t pwmDuty;

    pidErr = desVolConvert - vol;
	//误差不在允许范围内
	if(fabs(pidErr) > ERR_LIMIT)
    {
    	pidCtrl.out = arm_pid_f32(&pidCtrl.S,pidErr);
		//     vol			   pidCtrl.out
		//  ——————————   =    ————————————
		//	当前占空比			调节后占空比
		htim3.Instance->CCR1 = (uint32_t)(pidCtrl.out * (htim3.Instance->CCR1 + 1) / vol);

		pwmDuty = htim3.Instance->CCR1;

//		if(htim3.Instance->CCR1<=0x5F){
//			LOCK; //封锁
//			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 30);
//			OLED_ShowString(48,12,(uint8_t*)("ERROR"),12);
//			OLED_Refresh();
//		}
//		else if(htim3.Instance->CCR1>=0x05){
//			LOCK; //封锁
//			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 30);
//			OLED_ShowString(48,12,(uint8_t*)("ERROR"),12);
//			OLED_Refresh();
//		}

		OLED_ShowNum(60,48,pwmDuty/21.000000,3,12);
		OLED_Refresh();
	}

}

/**
 * @brief 根据原始采样值计算PWM等效电压值
 * @retval None
 */
float calVol()
{
	int i;
	float res;
	float sum = 0;
	float tempVal;
	for(i = 0;i < BUF_LEN; i++)
	{
		tempVal = adc_raw_copy[i] * 3.300000 / 4095;
		sum += tempVal;
	}

	res = sum / BUF_LEN;

//	if(res < 0.570000){
//		res = res - 0.006400;
//	}
//	else if(res >= 0.570000&&res<0.607100){
//		res = res - 0.006540;
//	}
//	else if(res>=0.607100&&res<0.673000){
//		res = res - 0.007900;
//	}
//	else if(res>=0.673000&&res<0.780000){
//		res = res - 0.080900;
//	}
//	else if(res>=0.780000&&res<0.820000){
//		res = res - 0.080900;
//	}
//	else if(res>=0.820000&&res<1.000180){
//		res = res - 0.010180;
//	}
//	else if(res>=1.0001800&&res<1.340000){
//		res = res - 0.013180;
//	}
//	else if(res>=1.340000&&res<1.688400){
//		res = res - 0.017400;
//	}
//	else if(res>=1.688400&&res<2.057000){
//		res = res - 0.02100;
//	}
//	else if(res>=2.057000&&res<2.100000){
//		res = res - 0.046700;
//	}
//	else if(res>=2.100000&&res<2.270000){
//		res = res - 0.028600;
//	}
//	else if(res>=2.2700000&&res<2.500000){
//		res = res - 0.032000;
//	}
//	else if(res>=2.500000&&res<3.299000){
//		res = res - 0.037900;
//	}

	if(res < 0.570000){
		res = res - 0.006400 + 0.026000;
	}
	else if(res >= 0.570000&&res<0.607100){
		res = res - 0.006540 + 0.026000;
	}
	else if(res>=0.607100&&res<0.673000){
		res = res - 0.007900 + 0.026000;
	}
	else if(res>=0.673000&&res<0.780000){
		res = res - 0.080900 + 0.026000;
	}
	else if(res>=0.780000&&res<0.820000){
		res = res - 0.080900 + 0.026000;
	}
	else if(res>=0.820000&&res<1.000180){
		res = res - 0.011180 + 0.026000;
	}
	else if(res>=1.0001800&&res<1.340000){
		res = res - 0.013180 + 0.026000;
	}
	else if(res>=1.340000&&res<1.688400){
		res = res - 0.017400 + 0.026000;
	}
	else if(res>=1.688400&&res<2.057000){
		res = res - 0.02400 + 0.026000;
	}
	else if(res>=2.057000&&res<2.100000){
		res = res - 0.026700 + 0.026000;
	}
	else if(res>=2.100000&&res<2.270000){
		res = res - 0.028600 + 0.026000;
	}
	else if(res>=2.2700000&&res<2.450000){
		res = res - 0.032000 + 0.026000;
	}
	else if(res>=2.450000&&res<2.780000){
		res = res - 0.034200 + 0.026000;
	}
	else if(res>=2.780000&&res<2.980000){
		res = res - 0.034200 + 0.026000;
	}
	else if(res>=2.980000&&res<3.299000){
		res = res - 0.036500 + 0.026000;
	}


	if(res<=0.95000){
		if(cur_cur>0.480000&&cur_cur<0.660000){
			res = res - 0.003000;
		}
		else if(cur_cur>=0.760000&&cur_cur<0.850000){
			res = res - 0.004000;
		}
		else if(cur_cur>=0.850000&&cur_cur<0.103000){
			res = res - 0.004000;
		}
		else if(cur_cur>=1.103000&&cur_cur<1.124000){
			res = res - 0.005500;
		}
		else if(cur_cur>=1.124000&&cur_cur<1.156000){
			res = res - 0.007000;
		}
		else if(cur_cur>=1.156000&&cur_cur<1.72000){
			res = res - 0.01000;
		}
		else if(cur_cur>=1.720000&&cur_cur<1.860000){
			res = res - 0.0110000;
		}
		else if(cur_cur>=1.860000){
			res = res - 0.0120000;
		}
	}
	else if(res>0.95000&&res<2.800000){
		if(cur_cur>0.480000&&cur_cur<0.660000){
			res = res - 0.002000;
		}
		else if(cur_cur>=0.760000&&cur_cur<0.850000){
			res = res - 0.003000;
		}
		else if(cur_cur>=0.850000&&cur_cur<1.103000){
			res = res - 0.0033000;
		}
		else if(cur_cur>=1.103000&&cur_cur<1.124000){
			res = res - 0.003500;
		}
		else if(cur_cur>=1.124000&&cur_cur<1.156000){
			res = res - 0.005000;
		}
		else if(cur_cur>=1.156000&&cur_cur<1.72000){
			res = res - 0.008000;
		}
		else if(cur_cur>=1.720000&&cur_cur<1.860000){
			res = res - 0.009000;
		}
		else if(cur_cur>=1.860000){
			res = res - 0.010000;
		}
	}
	else if(res>=2.800000){
		if(cur_cur>0.480000&&cur_cur<0.660000){
			res = res - 0.001000;
		}
		else if(cur_cur>=0.760000&&cur_cur<0.850000){
			res = res - 0.002000;
		}
		else if(cur_cur>=0.850000&&cur_cur<1.103000){
			res = res - 0.0023000;
		}
		else if(cur_cur>=1.103000&&cur_cur<1.124000){
			res = res - 0.003500;
		}
		else if(cur_cur>=1.124000&&cur_cur<1.156000){
			res = res - 0.004500;
		}
		else if(cur_cur>=1.156000&&cur_cur<1.72000){
			res = res - 0.007000;
		}
		else if(cur_cur>=1.720000&&cur_cur<1.860000){
			res = res - 0.008000;
		}
		else if(cur_cur>=1.860000){
			res = res - 0.009000;
		}
	}

//	if(res<=2.75000){
//		if(cur_cur>0.480000&&cur_cur<0.660000){
//			res = res + 0.003000;
//		}
//		else if(cur_cur>=0.760000&&cur_cur<0.850000){
//			res = res + 0.004000;
//		}
//		else if(cur_cur>=0.850000&&cur_cur<0.103000){
//			res = res + 0.005000;
//		}
//		else if(cur_cur>=0.103000&&cur_cur<0.124000){
//			res = res + 0.007500;
//		}
//		else if(cur_cur>=0.124000&&cur_cur<0.156000){
//			res = res + 0.010000;
//		}
//		else if(cur_cur>=0.156000&&cur_cur<1.72000){
//			res = res + 0.01200;
//		}
//		else if(cur_cur>=1.720000&&cur_cur<1.860000){
//			res = res + 0.0130000;
//		}
//		else if(cur_cur>=1.860000){
//			res = res + 0.0150000;
//		}
//	}


	cur_vol = res;

	return res;
}

/**
 * @brief 设置输出电压
 * @retval None
 *
 */
void setVol(void){

	if(key_value == 1){
		key_value=0;
		//+0.1v
		if(DES_VOL<18.000000){
			DES_VOL+=0.100000;
		}
	}
	if(key_value == 2){
		key_value=0;
		//-0.1v
		if(DES_VOL>3.000000){
			DES_VOL-=0.100000;
		}
	}
	if(key_value == 3){
		key_value=0;
		//+0.5v
		if(DES_VOL<17.600000){
			DES_VOL+=0.500000;
		}
	}
	if(key_value == 4){
		key_value=0;
		//-0.5v
		if(DES_VOL>3.400000){
			DES_VOL-=0.500000;
		}
	}
	if(key_value == 16){
		key_value = 0;
//		UNLOCK;
		pidFlag = 1;
	}
	if(key_value == 12){
		key_value = 0;
//		LOCK;
		pidFlag = 0;
		htim3.Instance->CCR1 = 100;
		OLED_ShowNum(60,48,5,3,12);	//初始PWM的CCR
		OLED_Refresh();
	}

}

/**
 * @brief 根据原始采样值计算电流值
 * @retval None
 */
float calCur()
{
	int i;
	float res;
	float sum = 0;
	float tempCur;
	for(i = 0;i < BUF_LEN; i++)
	{
		tempCur = adc2_raw_copy[i] * 3.3 / 4095;		//此为电流计算
		sum += tempCur;
	}

	res = sum / BUF_LEN;

	if(res < 0.570000){
		res = res - 0.006400 + 0.026000;
	}
	else if(res >= 0.570000&&res<0.607100){
		res = res - 0.006540 + 0.026000;
	}
	else if(res>=0.607100&&res<0.673000){
		res = res - 0.007900 + 0.026000;
	}
	else if(res>=0.673000&&res<0.780000){
		res = res - 0.080900 + 0.026000;
	}
	else if(res>=0.780000&&res<0.820000){
		res = res - 0.080900 + 0.026000;
	}
	else if(res>=0.820000&&res<1.000180){
		res = res - 0.010180 + 0.026000;
	}
	else if(res>=1.0001800&&res<1.340000){
		res = res - 0.013180 + 0.026000;
	}
	else if(res>=1.340000&&res<1.688400){
		res = res - 0.017400 + 0.026000;
	}
	else if(res>=1.688400&&res<2.057000){
		res = res - 0.02100 + 0.026000;
	}
	else if(res>=2.057000&&res<2.100000){
		res = res - 0.046700 + 0.026000;
	}
	else if(res>=2.100000&&res<2.270000){
		res = res - 0.028600 + 0.026000;
	}
	else if(res>=2.2700000&&res<2.500000){
		res = res - 0.032000 + 0.026000;
	}
	else if(res>=2.500000&&res<3.299000){
		res = res - 0.037900 + 0.026000;
	}

//	if(res < 0.606000){
//		res = res - 0.006000;
//	}
//	else if(res>=0.606000&&res<0.7740000){
//		res = res - 0.007100;
//	}
//	else if(res>=0.774000&&res<=0.945200){
//		res = res - 0.0100000;
//	}
//	else if(res>=0.945200&&res<1.116000){
//		res = res - 0.0120000;
//	}
//	else if(res>=1.116000&&res<1.366700){
//		res = res - 0.014500;
//	}
//	else if(res>=1.366700&&res<1.607700){
//		res = res - 0.018000;
//	}
//	else if(res>=1.607700&&res<1.758400){
//		res = res - 0.020100;
//	}
//	else if(res>=1.758400&&res<1.939100){
//		res = res - 0.0224000;
//	}
//	else if(res>=1.939100&&res<2.098700){
//		res = res - 0.0245000;
//	}
//	else if(res>=2.098700&&res<2.339600){
//		res = res - 0.027800;
//	}
//	else if(res>=2.339600&&res<2.550700){
//		res = res - 0.030700;
//	}
//	else if(res>=2.550700&&res<2.771000){
//		res = res - 0.034200;
//	}
//	else if(res>=2.771000&&res<3.031700){
//		res = res - 0.036400;
//	}
//	else if(res>=3.031700&&res<3.203000){
//		res = res - 0.038200;
//	}
//	else if(res>=3.20300&&res<3.3000000){
//		res = res - 0.040000;
//	}

//	if(res < 0.570000){
//		res = res - 0.006500;
//	}
//	else if(res >= 0.570000&&res<0.607100){
//		res = res - 0.006540;
//	}
//	else if(res>=0.607100&&res<0.834000){
//		res = res - 0.013900;
//	}
//	else if(res>=0.834000&&res<1.114180){
//		res = res - 0.014180;
//	}
//	else if(res>=1.114180&&res<1.688400){
//		res = res - 0.017400;
//	}
//	else if(res>=1.688400&&res<2.057000){
//		res = res - 0.018400;
//	}
//	else if(res>=2.057000&&res<2.100000){
//		res = res - 0.046700;
//	}
//	else if(res>=2.100000&&res<2.270000){
//		res = res - 0.047600;
//	}
//	else if(res>=2.2700000&&res<2.500000){
//		res = res - 0.049000;
//	}
//	else if(res>=2.500000&&res<3.299000){
//		res = res - 0.050900;
//	}


	cur_cur = res;

	return res;
}

/**
 * @brief 设置过流保护点
 * @retval None
 */
void setCurProtect(void){

	if(key_value == 5){
		key_value=0;
		//+0.1A
		if(PRO_Cur<2.000000){
			PRO_Cur+=0.100000;
		}
	}
	if(key_value == 6){
		key_value=0;
		//-0.1A
		if(PRO_Cur>0.500000){
			PRO_Cur-=0.100000;
		}
	}


}

//代码核心部分
void core(void){

	OLED_ShowFNum(48,0,DES_VOL,12);

	OLED_ShowFNum(48,24,PRO_Cur,12);



	if(pidFlag == 1){
		if(dma_cpl_flag == 1){		//dma电压采集完成标志
			dma_cpl_flag = 0;
			pidExecu(calVol());
		}
	}

	calVol();
	OLED_ShowFNum(48,12,cur_vol*6.000000,12);
	OLED_Refresh();

	if(dma2_cpl_flag == 1){		//dma电流采集完成标志
		dma2_cpl_flag = 0;
		calCur();

		OLED_ShowFNum(48,36,cur_cur/1.500000,12);
		OLED_Refresh();

		if(calCur()>(PRO_Cur*1.500000)&&overCur==0){//当前采集电流>过流保护点
			LOCK; //封锁
			pidFlag = 0;	//关闭PI
			overCur = 1;	//过流标志
			faultDuty = 2100*DES_VOL/30;		//故障时设置步进到的占空比
			htim3.Instance->CCR1 = 100;			//故障时设置初始占空比
			step = (faultDuty-100)/70;			//故障时设置步进值

			OLED_ShowChar(108,0,'!',16);
			OLED_Refresh();
			delay_ms(100);
		}
		if(overCur==1&&calCur()<=(PRO_Cur*1.500000)){
			UNLOCK;
			curDuty = htim3.Instance->CCR1;		//当前占空比
			if(curDuty<=faultDuty){				//缓慢步进
				htim3.Instance->CCR1 = curDuty + step;
				delay_ms(40);
			}
			else{
				overCur = 0;
				pidFlag = 1;	//开启PI
				OLED_ShowChar(108,0,' ',16);
				OLED_Refresh();
				delay_ms(40);
			}
		}

//		if(htim3.Instance->CCR1<=90){
//			LOCK; //封锁
//			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 30);
//			OLED_ShowString(48,36,(uint8_t*)("ERROR"),12);
//			OLED_Refresh();
//		}
//		if(htim3.Instance->CCR1>=10){
//			LOCK; //封锁
//			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 30);
//			OLED_ShowString(48,36,(uint8_t*)("ERROR"),12);
//			OLED_Refresh();
//		}

	}
}

/* ADC-DMA全传输完成回调 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc){
	if(hadc->Instance == hadc1.Instance){
		memcpy((void *)adc_raw_copy,(void *)adc_raw,sizeof(adc_raw));
		dma_cpl_flag = 1;
	}
	if(hadc->Instance == hadc2.Instance){
		memcpy((void *)adc2_raw_copy,(void *)adc2_raw,sizeof(adc2_raw));
		dma2_cpl_flag = 1;
	}

}
