/*
 * core.c
 *
 *  Created on: Jul 27, 2022
 *      Author: Kierti
 */

#include "main.h"
#include "core.h"
#include "ADS8688_SPI.h"
#include "AD9959.h"
#include "hmi_user_uart.h"
#include "cmd_process.h"
#include "math.h"

/* 参数初始化 */
Sys_T tSys;
void sysInit(void){

	tSys.mode = waitMeasure;

	tSys.carrierFre = 10000000;
	tSys.freStep = 500000;
	tSys.frePointNum = 41;

	tSys.judegMax = 0;		//用于判断是否已调
	tSys.maxIndex = 0;

	tSys.Fs = 40960;		//Hz
	tSys.fftNum = 0;

	tSys.ma = 0;
	tSys.AMfre = 0;
	tSys.AMoffset = 0;

	tSys.mf = 0;
	tSys.FMfre = 0;
	tSys.FMfre = 0;
}


void __measureAM(void){

	float fftTemp[1024] = {0,};
	float Window = 0;
	volatile float directVol_A,positiveVol_A,negativeVol_A,abnormal_A;
	volatile int dirIndex,posIndex,negIndex,abnormalIndex;
	volatile int recordFlag = 0;

	ad9959_write_frequency(AD9959_CHANNEL_0,10000000);
	delay_ms(25);
	get_AD_Results();
	calc_FFT(AD_arr,tSys.fftAmp);

//	for(int i = 0;i<AD_Size;i++){
//		Window = 0.35875 - 0.48829 * cos(  2 * PI * (float)i / 1024 ) +
//				0.14128 * cos(  4 * PI * (float)i/  1024 ) -
//				0.01168 * cos(  6 * PI * (float)i/  1024 );
//		tSys.fftAmp[i] = tSys.fftAmp[i]*Window;
//	}

	for(int i = 0; i < AD_Size / 2;i++){
		if(tSys.fftAmp[i] > 20 && recordFlag == 0){
			directVol_A = tSys.fftAmp[i];
			dirIndex = i;
			recordFlag++;
		}
		else if(tSys.fftAmp[i] > 60 && recordFlag == 1){
			abnormal_A = tSys.fftAmp[i];
			abnormalIndex = i;
			i += 8;
			recordFlag++;
		}
		else if(tSys.fftAmp[i] > 18 &&recordFlag == 2){
			positiveVol_A = tSys.fftAmp[i];
			posIndex = i;
			i += 5;
			recordFlag++;
		}
		else if(tSys.fftAmp[i] > 18 &&recordFlag == 3){
			negativeVol_A = tSys.fftAmp[i];
			negIndex = i;
			recordFlag++;
		}
	}
	if(abnormal_A < tSys.fftAmp[abnormalIndex+1]){
		abnormal_A = tSys.fftAmp[abnormalIndex+1];
	}
	if(positiveVol_A < tSys.fftAmp[posIndex+1]){
		positiveVol_A = tSys.fftAmp[posIndex+1];
		if(positiveVol_A < tSys.fftAmp[posIndex+2]){
			positiveVol_A = tSys.fftAmp[posIndex+2];
		}
	}
	if(negativeVol_A < tSys.fftAmp[negIndex+1]){
		negativeVol_A = tSys.fftAmp[negIndex+1];
		if(negativeVol_A < tSys.fftAmp[negIndex+2]){
			negativeVol_A = tSys.fftAmp[negIndex+2];
		}
	}

	delay_ms(200);

	tSys.ma = (positiveVol_A + negativeVol_A) / 54.4f /4;					//AM调幅度ma
	tSys.AMfre = (posIndex + negIndex) / 2 * tSys.Fs / AD_Size;				//AM调制频率
	tSys.AMoffset = directVol_A;											//AM调制波偏置

	//显示AM调制度ma
    SetTFTText(0,7,"AM");
    SetTextValueFloat(0,8,tSys.ma);
    SetTextValueFloat(0,9,tSys.AMfre/1000.f);
    SetTextValueFloat(0,10,10);


}

void __measureFM(void){

	float FMamp[20] = {0,};
	int FMampIndex[20] = {0,};
	int index = 0;
	double __Jx;	//贝塞尔函数Jα(x)

	ad9959_write_frequency(AD9959_CHANNEL_0,10000000);
	delay_ms(25);
	get_AD_Results();

	for(int i = 0;i < AD_Size;i++){
		if(tSys.fftAmp[i] > 10){
			FMampIndex[index] = i;
			FMamp[index] = tSys.fftAmp[i];
			index++;
		}
	}

	__Jx = tSys.fftAmp[1] / tSys.fftAmp[0];
	if(fabs(bessi(0,__Jx) - bessi(1,__Jx)) <= 0.3){
		tSys.mf = (bessi(0,__Jx) + bessi(1,__Jx)) / 2;
	}
	else{
		tSys.mf = bessi0(__Jx);							//FM调频度mf
	}
	tSys.FMfre = FMampIndex[1] * tSys.Fs / AD_Size;		//FM调频波频率
	tSys.FMoffset = FMamp[0];							//FM调频波偏置

    ad9959_write_frequency(AD9959_CHANNEL_1, tSys.FMfre);
    ad9959_write_phase(AD9959_CHANNEL_1, 0);
    ad9959_write_amplitude(AD9959_CHANNEL_1, 500);

	//显示mf
    SetTFTText(0,7,"FM");
    SetTextValueFloat(0,8,tSys.mf);
    SetTextValueFloat(0,9,tSys.FMfre/1000.f);
    SetTextValueFloat(0,10,tSys.curCarrFre/1000000.f);
}


/* 检波 */
uint32_t detectForFre(void){

	for(int i = 0;i < tSys.frePointNum;i++){
		ad9959_write_frequency(AD9959_CHANNEL_0,tSys.carrierFre + i * tSys.freStep);
		delay_ms(25);
		get_AD_Results();		//采样
		arm_mean_f32(AD_arr,AD_Size,tSys.judgeCarrFre+i);		//求平均
	}

	for(int i = 0;i < tSys.frePointNum;i++){
		arm_max_f32(tSys.judgeCarrFre,tSys.frePointNum,&tSys.judegMax,&tSys.maxIndex);
	}

	if(tSys.judegMax < 20){		//判断是否已调
		return 0;
	}
	else{
		tSys.curCarrFre = tSys.carrierFre + tSys.maxIndex * tSys.freStep;
		return tSys.curCarrFre;		//返回载波频率
	}
}

/* 识别 */
moduType_E idenModuType(void){

	if(detectForFre() == 0){	//未调
		return No;
	}
	else{		//判断是AM还是FM
		ad9959_write_frequency(AD9959_CHANNEL_0,tSys.curCarrFre);
		delay_ms(25);
		get_AD_Results();
		calc_FFT(AD_arr,tSys.fftAmp);

		for(int i = 0;i < AD_Size;i++){
			if(tSys.fftAmp[i] > 20){
				tSys.fftNum ++;
			}
		}
		//目前通过低通滤波，FFT后的判断幅频分量个数来测试
		if(tSys.fftNum == 3){
			return AM;
		}
		if(tSys.fftNum >=5){
			return FM;
		}
	}
	return  No;
}

/* 提取 */
uint32_t extractModuWave(void){

	float fftTemp[1024] = {0,};
	float FMamp[20] = {0,};
	int FMampIndex[20] = {0,};
	volatile float directVol_A,positiveVol_A,negativeVol_A;
	volatile int dirIndex,posIndex,negIndex;

	if(idenModuType() == 1){	//AM

		for(int i = 0; i < AD_Size;i++){
			fftTemp[i] = tSys.fftAmp[0];
		}
		for(int i = 0;i < AD_Size;i++){
			quickSort(fftTemp,0,1023);
		}
		directVol_A   = fftTemp[0];
		positiveVol_A = fftTemp[1];
		negativeVol_A = fftTemp[2];

		for(int i = 0;i < AD_Size;i++){				//计算FFT直流分量，频率分量所处位置
			if(directVol_A == tSys.fftAmp[i]){
				dirIndex = i;
			}
			if(positiveVol_A == tSys.fftAmp[i]){
				posIndex = i;
			}
			if(negativeVol_A == tSys.fftAmp[i]){
				negIndex = i;
			}
		}

		tSys.ma = 2 * (positiveVol_A + negativeVol_A) / 2 / directVol_A;		//AM调幅度ma
		tSys.AMfre = posIndex * tSys.Fs / AD_Size;								//AM调制频率
		tSys.AMoffset = directVol_A;											//AM调制波偏置

		return 1;

	}
	if(idenModuType() == 2){	//FM

		int index = 0;
		double __Jx;	//贝塞尔函数Jα(x)

		for(int i = 0;i < AD_Size;i++){
			if(tSys.fftAmp[i] > 10){
				FMampIndex[index] = i;
				FMamp[index] = tSys.fftAmp[i];
				index++;
			}
		}

		__Jx = tSys.fftAmp[1] / tSys.fftAmp[0];
		if(fabs(bessi(0,__Jx) - bessi(1,__Jx)) <= 0.3){
			tSys.mf = (bessi(0,__Jx) + bessi(1,__Jx)) / 2;
		}
		else{
			tSys.mf = bessi0(__Jx);							//FM调频度mf
		}
		tSys.FMfre = FMampIndex[1] * tSys.Fs / AD_Size;		//FM调频波频率
		tSys.FMoffset = FMamp[0];							//FM调频波偏置
		tSys.maxFreDev = tSys.FMfre * tSys.mf;				//FM最大频偏

		return 2;
	}
	return 0;
}

/* 解调 */
void demodulationOut(void){

	if(extractModuWave() == 0){
	    SetTFTText(0,7,"未调");
	    SetTFTText(0,8," ");
	    SetTFTText(0,9," ");
	    SetTFTText(0,10," ");
	}
	if(extractModuWave() == 1){
	    ad9959_write_frequency(AD9959_CHANNEL_1, tSys.AMfre);
	    ad9959_write_phase(AD9959_CHANNEL_1, 0);
	    ad9959_write_amplitude(AD9959_CHANNEL_1, 500);
	    SetTFTText(0,7,"AM");
	    SetTextValueFloat(0,8,tSys.ma);
	    SetTextValueFloat(0,9,tSys.AMfre/1000.f);
	    SetTextValueFloat(0,10,tSys.curCarrFre/1000000.f);

	}
	if(extractModuWave() == 2){
		ad9959_write_frequency(AD9959_CHANNEL_1, tSys.FMfre);
		ad9959_write_phase(AD9959_CHANNEL_1, 0);
		ad9959_write_amplitude(AD9959_CHANNEL_1, 500);
	    SetTFTText(0,7,"FM");
	    SetTextValueFloat(0,8,tSys.mf);
	    SetTextValueFloat(0,9,tSys.FMfre/1000.f);
	    SetTextValueFloat(0,10,tSys.curCarrFre/1000000.f);
	}
}
