/*
 * oscilloscope.h
 *
 *  Created on: Sep 29, 2022
 *      Author: Yogurt
 */

#ifndef INC_OSCILLOSCOPE_H_
#define INC_OSCILLOSCOPE_H_

#include "main.h"

/*  系统参数  */
typedef struct _Sys_T
{
    short channel;		 //通道

    int levelSensitivity;	//水平灵敏度

    int verticalSensitivity0;	//通道1垂直灵敏度
    int verticalSensitivity1;	//通道2垂直灵敏度

    int CH1out;
    int CH2out;

    int count0;					//水平灵敏度1ms/div计数
    int count1;					//水平灵敏度10ms/div计数

    unsigned char waveData0[600];
    unsigned char waveData1[600];

    int freMeaNum0;				//通道1频率输入捕获num
    int freMeaFlag0;			//通道1频率输入捕获标志位

    int freMeaNum1;
    int freMeaFlag1;

    int skewingNum;				//相位差计数
    int skewingFlag;			//相位差测量标志位
    int skewingFlagTime;		//相位差测量频率标志位
    float skewing;				//相位差

    unsigned long triggerLevel;			//触发电平
    unsigned long triggerStartBit;		//触发起始位

    int triggerSwitch;					//触发开关
    unsigned char triggerSelect;		//触发选择

    unsigned char waveDataMax0;			//串口屏通道1波形显示数据最大值
    unsigned char waveDataMin0;			//串口屏通道1波形显示数据最小值
    unsigned char waveDataMax1;
    unsigned char waveDataMin1;

    int runOrStop;
    int autoSet;
    int autoSetFlag0;					//自动设置通道1频率测量标志位
    int autoSetFlag1;					//自动设置通道2频率测量标志位

    int zoomFlag0;
    int zoomFlag1;
}Sys_T;
extern Sys_T tSys;

void float2char(float slope,char*buffer,int n);
void myInit(void);
void sysInit(void);
void oscilloscopeFunction(void);
void autoSet(void);
void waveDataDisplay(void);
void waveAttributeDisplay(void);
void freAndSkewingDataDisplay(void);
void triggerModeAndDisplay(void);
void waveRecognition(void);

#endif /* INC_OSCILLOSCOPE_H_ */
