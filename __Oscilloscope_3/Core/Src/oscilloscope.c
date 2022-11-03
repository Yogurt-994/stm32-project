/*
 * oscilloscope.c
 *
 *  Created on: Sep 29, 2022
 *      Author: Yogurt
 */

#include "oscilloscope.h"
#include "arm_math.h"
#include "algorithm.h"
#include "cmd_process.h"
#include "hmi_user_uart.h"
#include "stdio.h"
#include "tim.h"
#include "spi.h"
#include "ADS8688_SPI.h"
#include "math.h"
#include "string.h"

Sys_T tSys;
ArrayParam_T tAD_Params;
ArrayParam_T tAD_Params1;

void float2char(float slope,char*buffer,int n)  //浮点型数，存储的字符数组，字符数组的长度
{
    int temp,i,j;
    if(slope>=0)//判断是否大于0
        buffer[0] = '+';
    else
    {
        buffer[0] = '-';
        slope = -slope;
    }
    temp = (int)slope;//取整数部分
    for(i=0;temp!=0;i++)//计算整数部分的位数
        temp /=10;
    temp =(int)slope;
    for(j=i;j>0;j--)//将整数部分转换成字符串型
    {
        buffer[j] = temp%10+'0';
        temp /=10;
    }
    buffer[i+1] = '.';
    slope -=(int)slope;
    for(i=i+2;i<n-1;i++)//将小数部分转换成字符串型
    {
        slope*=10;
        buffer[i]=(int)slope+'0';
        slope-=(int)slope;
    }
    buffer[n-1] = '\0';
}

union FloatAndByte						//没用上
{
	float union_float;
	uchar union_byte[4];
}FAB;
uchar* FloatToBytes(uchar* data, float float_to_byte)
{
	FAB.union_float = float_to_byte;
	for (int i = 0; i < 4; i++)
	{
		data[i] = FAB.union_byte[i];
	}
	return data;
}

void myInit(){
	sysInit();
	ADS8688_Init(&ads8688, &hspi3, ADS8688_CS_GPIO_Port, ADS8688_CS_Pin);
	TFT_Init();
	HAL_TIM_Base_Start_IT(&htim6);
	HAL_TIM_Base_Start_IT(&htim7);
//	HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_1);
//	HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_2);
	HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_3);
	HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_4);
}

void sysInit(){
	tSys.channel = 0;
	tSys.levelSensitivity = 1;
	tSys.verticalSensitivity0 = 1;
	tSys.verticalSensitivity1 = 1;
	tSys.CH1out = 0;
	tSys.CH2out = 0;
	tSys.count0 = 0;
	tSys.count1 = 0;

	tSys.freMeaNum0 = 0;
	tSys.freMeaFlag0 = 0;

	tSys.freMeaNum1 = 0;
	tSys.freMeaFlag1 = 0;

	tSys.skewingNum = 0;
	tSys.skewingFlag = 0;
	tSys.skewingFlagTime = 0;

	tSys.triggerLevel = 50;
	tSys.triggerStartBit = 50;
	tSys.triggerSwitch = 0;
	tSys.triggerSelect = 0;

	tSys.runOrStop = 0;
	tSys.autoSet = 0;
	tSys.autoSetFlag0 = 0;
	tSys.autoSetFlag1 = 0;

	tSys.zoomFlag0 = 0;
	tSys.zoomFlag1 = 1;
}

//示波器功能
void oscilloscopeFunction(){
	switch(tSys.autoSet){
		case 0:{waveDataDisplay();}break;
		case -1:{autoSet();}break;
		default:break;
	}
}

//自动设置功能
void autoSet(){

	volatile int freCH1,freCH2;

	/* 水平灵敏度设置 */
	if(tSys.freMeaFlag0 >= 1){
		freCH1 = tSys.freMeaNum0;
		tSys.autoSetFlag0++;
	    tSys.freMeaNum0 = 0;
	    tSys.freMeaFlag0 = 0;
	    HAL_TIM_Base_Start_IT(&htim7);
	}
	if(tSys.freMeaFlag1 >= 1){
		freCH2 = tSys.freMeaNum1;
		tSys.autoSetFlag1++;
	    tSys.freMeaNum1 = 0;
	    tSys.freMeaFlag1 = 0;
	    HAL_TIM_Base_Start_IT(&htim6);
	}

	if(tSys.CH1out&tSys.CH2out){
		if(freCH1 > 110&&freCH2 > 110){
			tSys.levelSensitivity = 1;
			SetTFTText(0,9,"%ld",(long)tSys.levelSensitivity);
		}
		else if(freCH1 < 20&&freCH2 < 20){
			tSys.levelSensitivity = 100;
			SetTFTText(0,9,"%ld",(long)tSys.levelSensitivity);
		}
		else{
			tSys.levelSensitivity = 10;
			SetTFTText(0,9,"%ld",(long)tSys.levelSensitivity);
		}
	}
	else if(tSys.CH1out&~tSys.CH2out){
		if(freCH1 > 110){
			tSys.levelSensitivity = 1;
			SetTFTText(0,9,"%ld",(long)tSys.levelSensitivity);
		}
		else if(freCH1 < 20){
			tSys.levelSensitivity = 100;
			SetTFTText(0,9,"%ld",(long)tSys.levelSensitivity);
		}
		else{
			tSys.levelSensitivity = 10;
			SetTFTText(0,9,"%ld",(long)tSys.levelSensitivity);
		}
	}
	else if(~tSys.CH1out&tSys.CH2out){
		if(freCH2 > 110){
			tSys.levelSensitivity = 1;
			SetTFTText(0,9,"%ld",(long)tSys.levelSensitivity);
		}
		else if(freCH2 < 20){
			tSys.levelSensitivity = 100;
			SetTFTText(0,9,"%ld",(long)tSys.levelSensitivity);
		}
		else{
			tSys.levelSensitivity = 10;
			SetTFTText(0,9,"%ld",(long)tSys.levelSensitivity);
		}
	}


	/* 垂直灵敏度设置 */
	if(tAD_Params.Vpp>1000){
		tSys.verticalSensitivity0 = 500;
		SetTFTText(0,3,"%ld",(long)tSys.verticalSensitivity0);
	}
	else if(tAD_Params.Vpp <= 1000&&tAD_Params.Vpp > 100){
		tSys.verticalSensitivity0 = 100;
		SetTFTText(0,3,"%ld",(long)tSys.verticalSensitivity0);
	}
	else if(tAD_Params.Vpp <= 100&&tAD_Params.Vpp > 10){
		tSys.verticalSensitivity0 = 10;
		SetTFTText(0,3,"%ld",(long)tSys.verticalSensitivity0);
	}
	else{
		tSys.verticalSensitivity0 = 1;
		SetTFTText(0,3,"%ld",(long)tSys.verticalSensitivity0);
	}

	if(tAD_Params1.Vpp>=1000){
		tSys.verticalSensitivity1 = 500;
		SetTFTText(0,4,"%ld",(long)tSys.verticalSensitivity1);
	}
	else if(tAD_Params1.Vpp <= 1000&&tAD_Params1.Vpp > 100){
		tSys.verticalSensitivity1 = 100;
		SetTFTText(0,4,"%ld",(long)tSys.verticalSensitivity1);
	}
	else if(tAD_Params1.Vpp <= 100&&tAD_Params1.Vpp > 10){
		tSys.verticalSensitivity1 = 10;
		SetTFTText(0,4,"%ld",(long)tSys.verticalSensitivity1);
	}
	else{
		tSys.verticalSensitivity1 = 1;
		SetTFTText(0,4,"%ld",(long)tSys.verticalSensitivity1);
	}

	if(tSys.autoSetFlag0 >= 2&&tSys.autoSetFlag1 >= 2){
		tSys.autoSet = 0;
		tSys.autoSetFlag0 = 0;
		tSys.autoSetFlag1 = 0;
		HAL_TIM_Base_Stop(&htim3);
		ADarr_Init();
	}

}

//波形数据处理与显示
void waveDataDisplay(){
	switch(tSys.levelSensitivity){
		case 1:{
			if(pAD_arr1 >= pAD_arr_end1){

				tSys.count0++;						//显示测量参数部分
				if(tSys.count0 == 49){
					tSys.skewingFlagTime = 1;

					Calc_array(&tAD_Params,AD_arr1[0],300);
					Calc_array1(&tAD_Params1,AD_arr1[1],300);

					tSys.count0 = 0;

					waveAttributeDisplay();
				}

				for(int i = 0;i<600;i++){			//波形曲线显示数据处理
					if(i%2==0){
						tSys.waveData0[i] = (unsigned char)(AD_arr1[0][i/2]*200/(tSys.verticalSensitivity0*10)+101);
					}
					if(i%2==1){
						tSys.waveData0[i] = (unsigned char)((AD_arr1[0][i/2-1]+AD_arr1[0][i/2])/2 *200/(tSys.verticalSensitivity0*10)+101);
					}
				}

				for(int i = 0;i<600;i++){
					if(i%2==0){
						tSys.waveData1[i] = (unsigned char)(AD_arr1[1][i/2]*200/(tSys.verticalSensitivity1*10)+101);
					}
					if(i%2==1){
						tSys.waveData1[i] = (unsigned char)((AD_arr1[1][i/2-1]+AD_arr1[1][i/2])/2 *200/(tSys.verticalSensitivity1*10)+101);
					}
				}

				triggerModeAndDisplay();

				if(tSys.count0 >= 49) tSys.count0 = 0;

				ADarr_Init();						//AD采样复位初始化
			}

			freAndSkewingDataDisplay();
		};break;
		case 10:{
			if(pAD_arr2 >= pAD_arr_end2){

				tSys.count1++;
				if(tSys.count1 == 9){
					tSys.skewingFlagTime = 1;

					Calc_array(&tAD_Params,AD_arr2[0],1200);
					Calc_array1(&tAD_Params1,AD_arr2[1],1200);
					tSys.count1 = 0;

					waveAttributeDisplay();
				}

				for(int i = 0;i<600;i++){
						tSys.waveData0[i] = (unsigned char)(AD_arr2[0][i*2]*200/(tSys.verticalSensitivity0*10)+101);
				}
				for(int i = 0;i<600;i++){
						tSys.waveData1[i] = (unsigned char)(AD_arr2[1][i*2]*200/(tSys.verticalSensitivity1*10)+101);
				}

				triggerModeAndDisplay();

				if(tSys.count1 >= 9) tSys.count1 = 0;

				ADarr_Init();
			}

			freAndSkewingDataDisplay();
		};break;
		case 100:{
			if(pAD_arr3 >= pAD_arr_end3){

				tSys.skewingFlagTime = 1;

				Calc_array(&tAD_Params,AD_arr3[0],3000);
				Calc_array1(&tAD_Params1,AD_arr3[1],3000);

				waveAttributeDisplay();

				for(int i = 0;i<600;i++){
						tSys.waveData0[i] = (unsigned char)(AD_arr3[0][i*5]*200/(tSys.verticalSensitivity0*10)+101);
				}
				for(int i = 0;i<600;i++){
						tSys.waveData1[i] = (unsigned char)(AD_arr3[1][i*5]*200/(tSys.verticalSensitivity1*10)+101);
				}

				triggerModeAndDisplay();

				ADarr_Init();
			}

			freAndSkewingDataDisplay();
		};break;
	}
}

//波形属性(最值/平均值/峰峰值)显示
void waveAttributeDisplay(){

	char VppBuffer[7];
	char MaxBuffer[7];
	char MinBuffer[7];
	char AverBuffer[7];

	char VppBuffer1[7];
	char MaxBuffer1[7];
	char MinBuffer1[7];
	char AverBuffer1[7];

	tSys.waveDataMax0 = (unsigned char)(tAD_Params.max*200/(tSys.verticalSensitivity0*10)+100);
	tSys.waveDataMin0 = (unsigned char)(tAD_Params.min*200/(tSys.verticalSensitivity0*10)+100);
	tSys.waveDataMax1 = (unsigned char)(tAD_Params1.max*200/(tSys.verticalSensitivity1*10)+100);
	tSys.waveDataMin1 = (unsigned char)(tAD_Params1.min*200/(tSys.verticalSensitivity1*10)+100);

	if(tSys.runOrStop == 0){
		if(tSys.CH1out){
			float2char(tAD_Params.Vpp,VppBuffer,7);
			SetTFTText(0,21,"%smv",VppBuffer);
			float2char(tAD_Params.max,MaxBuffer,7);
			SetTFTText(0,25,"%smv",MaxBuffer);
			float2char(tAD_Params.min,MinBuffer,7);
			SetTFTText(0,27,"%smv",MinBuffer);
			float2char(tAD_Params.Aver,AverBuffer,7);
			SetTFTText(0,39,"%smv",AverBuffer);
		}
		else{
			SetTFTText(0,21," ");
			SetTFTText(0,25," ");
			SetTFTText(0,27," ");
			SetTFTText(0,39," ");
		}
		if(tSys.CH2out){
			float2char(tAD_Params1.Vpp,VppBuffer1,7);
			SetTFTText(0,22,"%smv",VppBuffer1);
			float2char(tAD_Params1.max,MaxBuffer1,7);
			SetTFTText(0,26,"%smv",MaxBuffer1);
			float2char(tAD_Params1.min,MinBuffer1,7);
			SetTFTText(0,28,"%smv",MinBuffer1);
			float2char(tAD_Params1.Aver,AverBuffer1,7);
			SetTFTText(0,40,"%smv",AverBuffer1);
		}
		else{
			SetTFTText(0,22," ");
			SetTFTText(0,26," ");
			SetTFTText(0,28," ");
			SetTFTText(0,40," ");
		}

		waveRecognition();		//波形识别
	}
}

//频率/相位差测量及数据显示
void freAndSkewingDataDisplay(){

	char SkewingBuffer[7];

	if(tSys.freMeaFlag0 >= 1){
		tSys.skewing = 360 * (tSys.skewingNum*4) / (1*1000000/tSys.freMeaNum0) % 360;		//相位差计算

		if(tSys.CH1out){
			SetTFTText(0,23,"%ldHz",(long)tSys.freMeaNum0);
		}
		else{
			SetTFTText(0,23," ");
		}

		if(tSys.CH1out != 0&&tSys.CH2out != 0){
			float2char(tSys.skewing,SkewingBuffer,7);
			SetTFTText(0,34,"%s",SkewingBuffer);			//相位差显示
		}
		else{
			SetTFTText(0,34," ");
		}

	    tSys.freMeaNum0 = 0;
	    tSys.freMeaFlag0 = 0;
	    HAL_TIM_Base_Start_IT(&htim7);
	}
	if(tSys.freMeaFlag1 >= 1){
		if(tSys.CH2out){
			SetTFTText(0,24,"%ldHz",(long)tSys.freMeaNum1);
		}
		else{
			SetTFTText(0,24," ");
		}
	    tSys.freMeaNum1 = 0;
	    tSys.freMeaFlag1 = 0;
	    HAL_TIM_Base_Start_IT(&htim6);
	}
}

//触发模式和波形数据显示
void triggerModeAndDisplay(void){
	switch(tSys.triggerSwitch){
		case 0:{
			if(tSys.runOrStop == 0){
				if(tSys.CH1out){					//波形曲线数据显示
					GraphChannelDataAdd(0,2,0,tSys.waveData0,600);
				}
				else{
					GraphChannelDataClear(0,2,0);
				}
				if(tSys.CH2out){
					GraphChannelDataAdd(0,2,1,tSys.waveData1,600);
				}
				else{
					GraphChannelDataClear(0,2,1);
				}
			}

		};break;
		case -1:{
			switch(tSys.triggerSelect){
				case 0:{		//通道1电平触发
					if(tSys.runOrStop == 0){
						if((tSys.waveData0[tSys.triggerStartBit*6] == (unsigned char)(tSys.triggerLevel*2))){
							if(tSys.CH1out){					//波形曲线数据显示
								GraphChannelDataAdd(0,2,0,tSys.waveData0,600);
							}
							else{
								GraphChannelDataClear(0,2,0);
							}
						}
						if((tSys.waveDataMax0 < (unsigned char)(tSys.triggerLevel*2))||
								(tSys.waveDataMin0 > (unsigned char)(tSys.triggerLevel*2))){
							if(tSys.CH1out){					//波形曲线数据显示
								GraphChannelDataAdd(0,2,0,tSys.waveData0,600);
							}
							else{
								GraphChannelDataClear(0,2,0);
							}
						}
						if(tSys.CH2out){
							GraphChannelDataAdd(0,2,1,tSys.waveData1,600);
						}
						else{
							GraphChannelDataClear(0,2,1);
						}
					};break;
					case 1:{		//通道1上升沿触发
						if((unsigned char)(tSys.triggerLevel*2) < tSys.waveData0[tSys.triggerStartBit*6]&&
							(unsigned char)(tSys.triggerLevel*2) > tSys.waveData0[tSys.triggerStartBit*6-1]){
							if(tSys.CH1out){					//波形曲线数据显示
								GraphChannelDataAdd(0,2,0,tSys.waveData0,600);
							}
							else{
								GraphChannelDataClear(0,2,0);
							}
						}
						else if((tSys.waveDataMax0 < (unsigned char)(tSys.triggerLevel*2))||
								(tSys.waveDataMin0 > (unsigned char)(tSys.triggerLevel*2))){
							if(tSys.CH1out){					//波形曲线数据显示
								GraphChannelDataAdd(0,2,0,tSys.waveData0,600);
							}
							else{
								GraphChannelDataClear(0,2,0);
							}
						}
						if(tSys.CH2out){
							GraphChannelDataAdd(0,2,1,tSys.waveData1,600);
						}
						else{
							GraphChannelDataClear(0,2,1);
						}
					}
				};break;
				case 2:{		//通道1下降沿触发
					if(tSys.runOrStop == 0){
						if((unsigned char)(tSys.triggerLevel*2) > tSys.waveData0[tSys.triggerStartBit*6]&&
							(unsigned char)(tSys.triggerLevel*2) < tSys.waveData0[tSys.triggerStartBit*6-1]){
							if(tSys.CH1out){					//波形曲线数据显示
								GraphChannelDataAdd(0,2,0,tSys.waveData0,600);
							}
							else{
								GraphChannelDataClear(0,2,0);
							}
						}
						else if((tSys.waveDataMax0 < (unsigned char)(tSys.triggerLevel*2))||
								(tSys.waveDataMin0 > (unsigned char)(tSys.triggerLevel*2))){
							if(tSys.CH1out){					//波形曲线数据显示
								GraphChannelDataAdd(0,2,0,tSys.waveData0,600);
							}
							else{
								GraphChannelDataClear(0,2,0);
							}
						}
						if(tSys.CH2out){
							GraphChannelDataAdd(0,2,1,tSys.waveData1,600);
						}
						else{
							GraphChannelDataClear(0,2,1);
						}
					}
				};break;
				case 3:{		//通道1双边沿触发
					if(tSys.runOrStop == 0){
						if(((unsigned char)(tSys.triggerLevel*2) > tSys.waveData0[tSys.triggerStartBit*6]&&
							(unsigned char)(tSys.triggerLevel*2) < tSys.waveData0[tSys.triggerStartBit*6-1])||
								((unsigned char)(tSys.triggerLevel*2) < tSys.waveData0[tSys.triggerStartBit*6]&&
								(unsigned char)(tSys.triggerLevel*2) > tSys.waveData0[tSys.triggerStartBit*6-1])){
							if(tSys.CH1out){					//波形曲线数据显示
								GraphChannelDataAdd(0,2,0,tSys.waveData0,600);
							}
							else{
								GraphChannelDataClear(0,2,0);
							}
						}
						else if((tSys.waveDataMax0 < (unsigned char)(tSys.triggerLevel*2))||
								(tSys.waveDataMin0 > (unsigned char)(tSys.triggerLevel*2))){
							if(tSys.CH1out){					//波形曲线数据显示
								GraphChannelDataAdd(0,2,0,tSys.waveData0,600);
							}
							else{
								GraphChannelDataClear(0,2,0);
							}
						}
						if(tSys.CH2out){
							GraphChannelDataAdd(0,2,1,tSys.waveData1,600);
						}
						else{
							GraphChannelDataClear(0,2,1);
						}
					}
				};break;
				case 4:{		//通道2水平触发
					if(tSys.runOrStop == 0){
						if((tSys.waveData1[tSys.triggerStartBit*6] == (unsigned char)(tSys.triggerLevel*2))){
							if(tSys.CH2out){					//波形曲线数据显示
								GraphChannelDataAdd(0,2,1,tSys.waveData1,600);
							}
							else{
								GraphChannelDataClear(0,2,1);
							}
						}
						else if((tSys.waveDataMax1 < (unsigned char)(tSys.triggerLevel*2))||
								(tSys.waveDataMin1 > (unsigned char)(tSys.triggerLevel*2))){
							if(tSys.CH2out){					//波形曲线数据显示
								GraphChannelDataAdd(0,2,1,tSys.waveData1,600);
							}
							else{
								GraphChannelDataClear(0,2,1);
							}
						}
						if(tSys.CH1out){
							GraphChannelDataAdd(0,2,0,tSys.waveData0,600);
						}
						else{
							GraphChannelDataClear(0,2,0);
						}
					}
				};break;
				case 5:{		//通道2上升沿触发
					if(tSys.runOrStop == 0){
						if((unsigned char)(tSys.triggerLevel*2) < tSys.waveData1[tSys.triggerStartBit*6]&&
							(unsigned char)(tSys.triggerLevel*2) > tSys.waveData1[tSys.triggerStartBit*6-1]){
							if(tSys.CH2out){					//波形曲线数据显示
								GraphChannelDataAdd(0,2,1,tSys.waveData1,600);
							}
							else{
								GraphChannelDataClear(0,2,1);
							}
						}
						else if((tSys.waveDataMax1 < (unsigned char)(tSys.triggerLevel*2))||
								(tSys.waveDataMin1 > (unsigned char)(tSys.triggerLevel*2))){
							if(tSys.CH2out){					//波形曲线数据显示
								GraphChannelDataAdd(0,2,1,tSys.waveData1,600);
							}
							else{
								GraphChannelDataClear(0,2,1);
							}
						}
						if(tSys.CH1out){
							GraphChannelDataAdd(0,2,0,tSys.waveData0,600);
						}
						else{
							GraphChannelDataClear(0,2,0);
						}
					}
				};break;
				case 6:{		//通道2下降沿触发
					if(tSys.runOrStop == 0){
						if((unsigned char)(tSys.triggerLevel*2) > tSys.waveData1[tSys.triggerStartBit*6]&&
							(unsigned char)(tSys.triggerLevel*2) < tSys.waveData1[tSys.triggerStartBit*6-1]){
							if(tSys.CH2out){					//波形曲线数据显示
								GraphChannelDataAdd(0,2,1,tSys.waveData1,600);
							}
							else{
								GraphChannelDataClear(0,2,1);
							}
						}
						else if((tSys.waveDataMax1 < (unsigned char)(tSys.triggerLevel*2))||
								(tSys.waveDataMin1 > (unsigned char)(tSys.triggerLevel*2))){
							if(tSys.CH2out){					//波形曲线数据显示
								GraphChannelDataAdd(0,2,1,tSys.waveData1,600);
							}
							else{
								GraphChannelDataClear(0,2,1);
							}
						}
						if(tSys.CH1out){
							GraphChannelDataAdd(0,2,0,tSys.waveData0,600);
						}
						else{
							GraphChannelDataClear(0,2,0);
						}
					}
				};break;
				case 7:{		//通道2双边沿触发
					if(tSys.runOrStop == 0){
						if(((unsigned char)(tSys.triggerLevel*2) > tSys.waveData1[tSys.triggerStartBit*6]&&
							(unsigned char)(tSys.triggerLevel*2) < tSys.waveData1[tSys.triggerStartBit*6-1])||
								((unsigned char)(tSys.triggerLevel*2) < tSys.waveData1[tSys.triggerStartBit*6]&&
								(unsigned char)(tSys.triggerLevel*2) > tSys.waveData1[tSys.triggerStartBit*6-1])){
							if(tSys.CH2out){					//波形曲线数据显示
								GraphChannelDataAdd(0,2,1,tSys.waveData1,600);
							}
							else{
								GraphChannelDataClear(0,2,1);
							}
						}
						else if((tSys.waveDataMax1 < (unsigned char)(tSys.triggerLevel*2))||
								(tSys.waveDataMin1 > (unsigned char)(tSys.triggerLevel*2))){
							if(tSys.CH2out){					//波形曲线数据显示
								GraphChannelDataAdd(0,2,1,tSys.waveData1,600);
							}
							else{
								GraphChannelDataClear(0,2,1);
							}
						}
						if(tSys.CH1out){
							GraphChannelDataAdd(0,2,0,tSys.waveData0,600);
						}
						else{
							GraphChannelDataClear(0,2,0);
						}
					}
				};break;
			}
		};break;
	}
}

//波形识别
void waveRecognition(){
	if(tSys.CH1out){
		if(tAD_Params.rmsDivideVpp < 0.32&&tAD_Params.rmsDivideVpp > 0.27){			//三角波
			SetTFTText(0,31,"Triangle");
		}
		else if(tAD_Params.rmsDivideVpp < 0.38&&tAD_Params.rmsDivideVpp > 0.32){	//正弦波
			SetTFTText(0,31,"Sine");
		}
		else if(tAD_Params.rmsDivideVpp < 0.53&&tAD_Params.rmsDivideVpp > 0.47){	//方波
			SetTFTText(0,31,"Square");
		}
	}
	else{
		SetTFTText(0,31," ");
	}

	if(tSys.CH2out){
		if(tAD_Params1.rmsDivideVpp < 0.32&&tAD_Params1.rmsDivideVpp > 0.27){			//三角波
			SetTFTText(0,32,"Triangle");
		}
		else if(tAD_Params1.rmsDivideVpp < 0.38&&tAD_Params1.rmsDivideVpp > 0.32){		//正弦波
			SetTFTText(0,32,"Sine");
		}
		else if(tAD_Params1.rmsDivideVpp < 0.53&&tAD_Params1.rmsDivideVpp > 0.47){		//方波
			SetTFTText(0,32,"Square");
		}
	}
	else{
		SetTFTText(0,32," ");
	}
}


/*
 * @brief:Function of Timer
 * @tim2:输入捕获，用于测量两通道频率
 * @tim3:用于ADS8686定时采集
 * @tim4:用于测量两通道相位差
 * @tim6/tim7:用于定时，设置tim2输入捕获时长周期
 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM2){
		if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_3){
			tSys.freMeaNum0 ++;
			if(tSys.freMeaNum0 > 10000000){
				tSys.freMeaNum0 = 0;
			}

			if(tSys.CH1out == -1&&tSys.CH2out == -1&&tSys.skewingFlagTime&&tSys.skewingFlag == 0){		//两路同时输出时,测量相位差
				tSys.skewingNum = 0;
				HAL_TIM_Base_Start_IT(&htim4);
				tSys.skewingFlag = 1;
			}
		}
		if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_4){
			tSys.freMeaNum1 ++;
			if(tSys.freMeaNum1 > 10000000){
				tSys.freMeaNum1 = 0;
			}

			if(tSys.CH1out == -1&&tSys.CH2out == -1&&tSys.skewingFlag&&tSys.skewingFlagTime){
				HAL_TIM_Base_Stop(&htim4);
				tSys.skewingFlag = 0;
				tSys.skewingFlagTime = 0;
			}
		}
	}
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim4){
		tSys.skewingNum++;
		if(tSys.skewingNum > 10000000){
			tSys.skewingNum = 0;
		}
	}
    if(htim == &htim3){
    	uint8_t rxbuf[4] = {0};
    	uint8_t txbuf=0;
    	switch(tSys.levelSensitivity){
			case 1:{
				if(pAD_arr1 < pAD_arr_end1){
					SAMPLE_END;		//将这里GPIO电平翻转操作改为直接对寄存器操作，采样速率可以再提升
					SAMPLE_BEGIN;
					HAL_SPI_TransmitReceive(&hspi3, &txbuf, rxbuf, 2,2);
					switch(tSys.channel){
						case 0:{
							*pAD_arr1 = (*(u16*)(&rxbuf[2])-ADS8688_ZERO)*AD_mV_Scale/4;
							pAD_arr1 += AD_Size1;
						};break;
						case 1:{
							*pAD_arr1 = (*(u16*)(&rxbuf[2])-ADS8688_ZERO)*AD_mV_Scale/4;
							pAD_arr1 -= AD_Size1 - 1;
						};break;
					}
					switch(tSys.channel){
						case 0:tSys.channel = 1;break;
						case 1:tSys.channel = 0;break;
					}
				}else{
					HAL_TIM_Base_Stop(&htim3);
				}
			};break;
			case 10:{
				if(pAD_arr2 < pAD_arr_end2){
					SAMPLE_END;
					SAMPLE_BEGIN;
					HAL_SPI_TransmitReceive(&hspi3, &txbuf, rxbuf, 2,2);
					switch(tSys.channel){
						case 0:{
							*pAD_arr2 = (*(u16*)(&rxbuf[2])-ADS8688_ZERO)*AD_mV_Scale/4;
							pAD_arr2 += AD_Size2;
						};break;
						case 1:{
							*pAD_arr2 = (*(u16*)(&rxbuf[2])-ADS8688_ZERO)*AD_mV_Scale/4;
							pAD_arr2 -= AD_Size2 - 1;
						};break;
					}
					switch(tSys.channel){
						case 0:tSys.channel = 1;break;
						case 1:tSys.channel = 0;break;
					}
				}else{
					HAL_TIM_Base_Stop(&htim3);
				}
			};break;
			case 100:{
				if(pAD_arr3 < pAD_arr_end3){
					SAMPLE_END;
					SAMPLE_BEGIN;
					HAL_SPI_TransmitReceive(&hspi3, &txbuf, rxbuf, 2,2);
					switch(tSys.channel){
						case 0:{
							*pAD_arr3 = (*(u16*)(&rxbuf[2])-ADS8688_ZERO)*AD_mV_Scale/4;
							pAD_arr3 += AD_Size3;
						};break;
						case 1:{
							*pAD_arr3 = (*(u16*)(&rxbuf[2])-ADS8688_ZERO)*AD_mV_Scale/4;
							pAD_arr3 -= AD_Size3 - 1;
						};break;
					}
					switch(tSys.channel){
						case 0:tSys.channel = 1;break;
						case 1:tSys.channel = 0;break;
					}
				}else{
					HAL_TIM_Base_Stop(&htim3);
				}
			};break;
    	}
    }
    if(htim == &htim7){
       	tSys.freMeaFlag0++;
       	if(tSys.freMeaFlag0 >= 1){
       		HAL_TIM_Base_Stop(&htim7);
       	}
    }
    if(htim == &htim6){
       	tSys.freMeaFlag1++;
       	if(tSys.freMeaFlag1 >= 1){
       		HAL_TIM_Base_Stop(&htim6);
       	}
    }
}
