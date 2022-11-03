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

const float Bessi0[27] = {0.765,0.671,0.567,0.455,0.340,0.224,0.110,0.003,0.097,
		0.185,0.260,0.320,0.364,0.392,0.450,0.397,0.377,0.342,0.296,0.240,0.178,
		0.110,0.041,0.027,0.092,0.151,0.202};
const float __Bessi0[28] = {0.862,0.768,0.685,0.617,0.467,0.346,0.229,0.113,0.523,
		0.099,0.176,0.265,0.327,0.374,0.399,0.396,0.404,0.386,0.350,0.354,0.346,
		0.180,0.111,0.354,0.336,0.092,0.154,0.284};
const float Bessi1[27] = {0.440,0.498,0.542,0.570,0.590,0.570,0.556,0.520,0.471,
		0.410,0.339,0.261,0.179,0.095,0.013,0.066,0.139,0.203,0.257,0.298,0.328,
		0.343,0.345,0.334,0.311,0.277,0.233};
const float __Bessi1[28] = {0.376,0.571,0.508,0.538,0.573,0.591,0.584,0.564,0.413,
		0.474,0.424,0.345,0.263,0.180,0.111,0.097,0.107,0.140,0.205,0.200,0.211,
		0.334,0.351,0.833,0.139,0.316,0.282,0.185};

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
	tSys.maxFreDev = 0;
}


void __measureAM(void){

	float fftTemp[1024] = {0,};
	volatile float directVol_A,positiveVol_A,negativeVol_A,abnormal_A;
	volatile int dirIndex,posIndex,negIndex,abnormalIndex;
	volatile int recordFlag = 0;

	ad9959_write_frequency(AD9959_CHANNEL_0,10000000);
	delay_ms(25);
	get_AD_Results();
	calc_FFT(AD_arr,tSys.fftAmp);

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

	delay_ms(400);

	tSys.ma = (positiveVol_A + negativeVol_A) / 54.4f /4;					//AM调幅度ma
	tSys.AMfre = (posIndex + negIndex) / 2 * tSys.Fs / AD_Size;				//AM调制频率
	tSys.AMoffset = directVol_A;											//AM调制波偏置

	//显示AM调制度ma
    SetTFTText(0,7,"AM");
    SetTextValueFloat(0,8,tSys.ma);
    SetTextValueFloat(0,9,tSys.AMfre/1000.f);
    SetTextValueFloat(0,10,10);
}

//判断一个数是否在某个数周围
uint16_t isRound(float newNum,float smaller,float bigger)
{
    if(  (newNum>smaller)
       &&(newNum<bigger)
      )
    return 1;
    else return 0;
}

void __measureFM(void){

	float FMamp[40] = {0,};
	int FMampIndex[40] = {0,};
	int index = 0;
//	float modAmp1 = 0,modAmp2 = 0,exAmp = 0;
//	int modIndex1 = 0,modIndex2 = 0,exIndex = 0;

	double __Jx0,__Jx1;	//贝塞尔函数Jα(x)
	float Jx0Amp[2]={0,};
	int Jx0Index[2]={0,};
	float Jx1Amp[2]={0,};
	int Jx1Index[2]={0,};
	volatile int Jx0Flag = 0,Jx1Flag = 0;
	int bessi0I = 0,bessi1I = 0;
	int bessi0Index[4]={0,},bessi1Index[4]={0,},__bessi = 0;;
	float bessi0X[4] = {0,},bessi1X[4] = {0,};

	ad9959_write_frequency(AD9959_CHANNEL_0,10000000);
	delay_ms(25);
	ADarr_Init();
	get_AD_Results();
	calc_FFT(AD_arr,tSys.fftAmp);

	for(int i = 1;i < AD_Size/2;i++){		//求频幅
		if(tSys.fftAmp[i] > 35){
			FMampIndex[index] = i;
			FMamp[index] = tSys.fftAmp[i];
			index++;

			if(Jx0Flag == 0){
				Jx0Amp[0] = tSys.fftAmp[i];
				Jx0Index[0] = i;
				i += 5;
				Jx0Flag = 1;
			}
			else if(Jx0Flag == 1){
				Jx0Amp[1] = tSys.fftAmp[i];
				Jx0Index[1] = i;
				i += 5;
				Jx0Flag = 2;
				Jx1Flag = 1;
			}
			else if(Jx1Flag == 1){
				Jx1Amp[0] = tSys.fftAmp[i];
				Jx1Index[0] = i;
				i += 5;
				Jx1Flag = 2;
			}
			else if(Jx1Flag == 2){
				Jx1Amp[1] = tSys.fftAmp[i];
				Jx1Index[1] = i;
				Jx1Flag = 3;
			}
		}
	}
	if(Jx0Amp[0] < tSys.fftAmp[Jx0Index[0] + 1]){
		Jx0Amp[0] = tSys.fftAmp[Jx0Index[0] + 1];
		Jx0Index[0] +=1;
		if(Jx0Amp[0] < tSys.fftAmp[Jx0Index[0] + 1]){
			Jx0Amp[0] = tSys.fftAmp[Jx0Index[0] + 1];
			Jx0Index[0]+=1;
			if(Jx0Amp[0] < tSys.fftAmp[Jx0Index[0] + 1]){
					Jx0Amp[0] = tSys.fftAmp[Jx0Index[0] + 1];
					Jx0Index[0]+=1;
			}
		}
	}
	if(Jx0Amp[1] < tSys.fftAmp[Jx0Index[1] + 1]){
		Jx0Amp[1] = tSys.fftAmp[Jx0Index[1] + 1];
		Jx0Index[1]+=1;
		if(Jx0Amp[1] < tSys.fftAmp[Jx0Index[1] + 1]){
			Jx0Amp[1] = tSys.fftAmp[Jx0Index[1] + 1];
			Jx0Index[1]+=1;
			if(Jx0Amp[1] < tSys.fftAmp[Jx0Index[1] + 1]){
				Jx0Amp[1] = tSys.fftAmp[Jx0Index[1] + 1];
				Jx0Index[1]+=1;
			}
		}
	}
	if(Jx1Amp[0] < tSys.fftAmp[Jx1Index[0] + 1]){
		Jx1Amp[0] = tSys.fftAmp[Jx1Index[0] + 1];
		Jx1Index[0]+=1;
		if(Jx1Amp[0] < tSys.fftAmp[Jx1Index[0] + 1]){
			Jx1Amp[0] = tSys.fftAmp[Jx1Index[0] + 1];
			Jx1Index[0]+=1;if(Jx1Amp[0] < tSys.fftAmp[Jx1Index[0] + 1]){
				Jx1Amp[0] = tSys.fftAmp[Jx1Index[0] + 1];
				Jx1Index[0]+=1;
			}

		}
	}
	if(Jx1Amp[1] < tSys.fftAmp[Jx1Index[1] + 1]){
		Jx1Amp[1] = tSys.fftAmp[Jx1Index[1] + 1];
		Jx1Index[1]+=1;
		if(Jx1Amp[1] < tSys.fftAmp[Jx1Index[1] + 1]){
			Jx1Amp[1] = tSys.fftAmp[Jx1Index[1] + 1];
			Jx1Index[1]+=1;
			if(Jx1Amp[1] < tSys.fftAmp[Jx1Index[1] + 1]){
				Jx1Amp[1] = tSys.fftAmp[Jx1Index[1] + 1];
				Jx1Index[1]+=1;
			}
		}
	}
	if(fabs(Jx0Amp[0] - Jx0Amp[1])<=30){
		if(fabs(Jx0Index[0] - Jx0Index[1]) < 20){
			tSys.FMfre = (Jx0Index[0] + Jx0Index[1]) / 2 * tSys.Fs / AD_Size;		//FM调频波频率
			__Jx0 = (Jx0Amp[0] + Jx0Amp[1]) / 2 / 5 /100;
			if(fabs(Jx1Amp[1] - Jx1Amp[0])<=20){
				__Jx1 = (Jx1Amp[0] + Jx1Amp[1]) / 2 / 5 /100;
			}
			else{
				__Jx1 = Jx1Amp[0] / 5 / 100;
			}
		}
	}
	else if(fabs(Jx0Amp[0] - Jx0Amp[1])>30&&fabs(Jx0Amp[1] - Jx1Amp[0])<=30){
		if(fabs(Jx0Index[0] - Jx0Index[1]) > fabs(Jx0Index[1] - Jx1Index[0])){
			tSys.FMfre = (Jx0Index[1] + Jx1Index[0]) / 2 * tSys.Fs / AD_Size;		//FM调频波频率
			__Jx0 = Jx0Amp[0] / 5 / 100;
			__Jx1 = Jx0Amp[1] / 5 / 100;
		}
	}

	int flag0 = 0,flag1 = 0,tick0 = 0,tick1 = 0;
	for(int i = 0;i < 26;i++){
//		int flag0 = 0,flag1 = 0;
		if((__Jx0  > Bessi0[i] && __Jx0 < Bessi0[i+1])||(__Jx0  < Bessi0[i] && __Jx0 > Bessi0[i+1])){
			bessi0X[bessi0I] = Bessi0[i];
			bessi0Index[bessi0I] = i;
			bessi0I++;
//			tick0++;
			flag0 = 1;
		}
		if((__Jx1  > Bessi1[i] && __Jx1 < Bessi1[i+1])||(__Jx1  < Bessi1[i] && __Jx1 > Bessi1[i+1])){
			bessi1X[bessi1I] = Bessi1[i];
			bessi1Index[bessi1I] = i;
			bessi1I++;
//			tick1++;
			flag1 = 1;
		}
		if(flag0 ==1&&flag1==1){
			__bessi = i;
		}
		if(flag0 == 1) {tick0++;}
		if(flag1 == 1) {tick1++;}
		if(tick1 == 2) {flag0 = 0;tick0 = 0;}
		if(tick1 == 2) {flag1 = 0;tick1 = 0;}
	}
	tSys.mf = 1 + __bessi * 0.2;
	tSys.maxFreDev = tSys.mf * tSys.FMfre/1000.f;

	delay_ms(200);
	//显示mf
    SetTFTText(0,7,"FM");
    SetTextValueFloat(0,8,tSys.mf);
    SetTextValueFloat(0,9,tSys.FMfre/1000.f);
    SetTextValueFloat(0,10,10);
    SetTextValueFloat(0,13,tSys.maxFreDev);
}


/* 检波 */
uint32_t detectForFre(void){

	float fftTemp[AD_Size] = {0,};

	for(int i = 0;i < tSys.frePointNum;i++){

		float totalAmp = 0;

		ad9959_write_frequency(AD9959_CHANNEL_0,tSys.carrierFre + i * tSys.freStep);
		HAL_Delay(50);
		get_AD_Results();		//采样
		calc_FFT(AD_arr,fftTemp);

		for(int j = 1;j <= 10;j++){
			totalAmp += fftTemp[j];
		}
		if(totalAmp < 10){		//判断是否已调
			continue;
		}
		else{
			tSys.curCarrFre = tSys.carrierFre + i * tSys.freStep;
			return tSys.curCarrFre;		//返回载波频率
		}
	}
	return 0;

}

/* 识别 */
moduType_E idenModuType(void){

	volatile int recordFlag = 0;
	volatile float _1Vol = 0,_2Vol=0,_3Vol=0,_4Vol=0,_5Vol=0,_6Vol=0,_7Vol=0;
	volatile int _1F=0,_2F=0,_3F=0,_4F=0,_5F=0,_6F=0,_7F=0;

	if(detectForFre() == 0){	//未调
		return No;
	}
	else{		//判断是AM还是FM
		ad9959_write_frequency(AD9959_CHANNEL_0,tSys.curCarrFre);
		HAL_Delay(50);
		get_AD_Results();
		calc_FFT(AD_arr,tSys.fftAmp);

		for(int i = 1;i < AD_Size/2;i++){
			if(tSys.fftAmp[i] > 20){			//
				if(tSys.fftNum == 0){
					_1Vol = tSys.fftAmp[i];
					_1F = i;
					i+=5;
					tSys.fftNum++;
				}
				else if(tSys.fftNum == 1){
					_2Vol = tSys.fftAmp[i];
					_2F = i;
					i+=5;
					tSys.fftNum++;
				}
				else if(tSys.fftNum == 2){
					_3Vol = tSys.fftAmp[i];
					_3F = i;
					i+=5;
					tSys.fftNum++;
				}
				else if(tSys.fftNum == 3){
					_4Vol = tSys.fftAmp[i];
					_4F = i;
					i+=5;
					tSys.fftNum++;
				}
				else if(tSys.fftNum == 4){
					_5Vol = tSys.fftAmp[i];
					_5F = i;
					i+=5;
					tSys.fftNum++;
				}
				else if(tSys.fftNum == 5){
					_6Vol = tSys.fftAmp[i];
					_6F = i;
					i+=5;
					tSys.fftNum++;
				}
				else if(tSys.fftNum == 6){
					_7Vol = tSys.fftAmp[i];
					_7F = i;
					i+=5;
					tSys.fftNum++;
				}
			}
		}
		if(_1Vol < tSys.fftAmp[_1F]){
			_1Vol = tSys.fftAmp[_1F+1];
			_1F+=1;
			if(_1Vol < tSys.fftAmp[_1F]){
				_1Vol = tSys.fftAmp[_1F+1];
				_1F+=1;
				if(_1Vol < tSys.fftAmp[_1F]){
					_1Vol = tSys.fftAmp[_1F+1];
					_1F+=1;
					if(_1Vol < tSys.fftAmp[_1F]){
						_1Vol = tSys.fftAmp[_1F+1];
						_1F+=1;
						if(_1Vol < tSys.fftAmp[_1F]){
							_1Vol = tSys.fftAmp[_1F+1];
							_1F+=1;
						}
					}
				}
			}
		}
		if(_2Vol < tSys.fftAmp[_2F]){
			_2Vol = tSys.fftAmp[_2F+1];
			_2F+=1;
			if(_2Vol < tSys.fftAmp[_2F]){
				_2Vol = tSys.fftAmp[_2F+1];
				_2F+=1;
				if(_2Vol < tSys.fftAmp[_2F]){
					_2Vol = tSys.fftAmp[_2F+1];
					_2F+=1;
					if(_2Vol < tSys.fftAmp[_2F]){
						_2Vol = tSys.fftAmp[_2F+1];
						_2F+=1;
						if(_2Vol < tSys.fftAmp[_2F]){
							_2Vol = tSys.fftAmp[_2F+1];
							_2F+=1;
						}
					}
				}
			}
		}
		if(_3Vol < tSys.fftAmp[_3F]){
			_3Vol = tSys.fftAmp[_3F+1];
			_3F+=1;
			if(_3Vol < tSys.fftAmp[_3F]){
				_3Vol = tSys.fftAmp[_3F+1];
				_3F+=1;
				if(_3Vol < tSys.fftAmp[_3F]){
					_3Vol = tSys.fftAmp[_3F+1];
					_3F+=1;
					if(_3Vol < tSys.fftAmp[_3F]){
						_3Vol = tSys.fftAmp[_3F+1];
						_3F+=1;
						if(_3Vol < tSys.fftAmp[_3F]){
							_3Vol = tSys.fftAmp[_3F+1];
							_3F+=1;
						}
					}
				}
			}
		}
		if(_4Vol < tSys.fftAmp[_4F]){
			_4Vol = tSys.fftAmp[_4F+1];
			_4F+=1;
			if(_4Vol < tSys.fftAmp[_4F]){
				_4Vol = tSys.fftAmp[_4F+1];
				_4F+=1;
				if(_4Vol < tSys.fftAmp[_4F]){
					_4Vol = tSys.fftAmp[_4F+1];
					_4F+=1;
					if(_4Vol < tSys.fftAmp[_4F]){
						_4Vol = tSys.fftAmp[_4F+1];
						_4F+=1;
						if(_4Vol < tSys.fftAmp[_4F]){
							_4Vol = tSys.fftAmp[_4F+1];
							_4F+=1;
						}
					}
				}
			}
		}
		if(_5Vol < tSys.fftAmp[_5F]){
			_5Vol = tSys.fftAmp[_5F+1];
			_5F+=1;
			if(_5Vol < tSys.fftAmp[_5F]){
				_5Vol = tSys.fftAmp[_5F+1];
				_5F+=1;
				if(_5Vol < tSys.fftAmp[_5F]){
					_5Vol = tSys.fftAmp[_5F+1];
					_5F+=1;
					if(_5Vol < tSys.fftAmp[_5F]){
						_5Vol = tSys.fftAmp[_5F+1];
						_5F+=1;
						if(_5Vol < tSys.fftAmp[_5F]){
							_5Vol = tSys.fftAmp[_5F+1];
							_5F+=1;
						}
					}
				}
			}
		}
		if(_6Vol < tSys.fftAmp[_6F]){
			_6Vol = tSys.fftAmp[_6F+1];
			_6F+=1;
			if(_6Vol < tSys.fftAmp[_6F]){
				_6Vol = tSys.fftAmp[_6F+1];
				_6F+=1;
				if(_6Vol < tSys.fftAmp[_6F]){
					_6Vol = tSys.fftAmp[_6F+1];
					_6F+=1;
					if(_6Vol < tSys.fftAmp[_6F]){
						_6Vol = tSys.fftAmp[_6F+1];
						_6F+=1;
						if(_6Vol < tSys.fftAmp[_6F]){
							_6Vol = tSys.fftAmp[_6F+1];
							_6F+=1;
						}
					}
				}
			}
		}
		if(_7Vol < tSys.fftAmp[_7F]){
			_7Vol = tSys.fftAmp[_7F+1];
			_7F+=1;
			if(_7Vol < tSys.fftAmp[_7F]){
				_7Vol = tSys.fftAmp[_7F+1];
				_7F+=1;
				if(_7Vol < tSys.fftAmp[_7F]){
					_7Vol = tSys.fftAmp[_7F+1];
					_7F+=1;
					if(_7Vol < tSys.fftAmp[_7F]){
						_7Vol = tSys.fftAmp[_7F+1];
						_2F+=1;
						if(_7Vol < tSys.fftAmp[_7F]){
							_7Vol = tSys.fftAmp[_7F+1];
							_7F+=1;
						}
					}
				}
			}
		}
		//目前通过低通滤波，FFT后的判断幅频分量个数来测试
		if(tSys.fftNum == 3){
			tSys.fftNum = 0;
			return AM;
		}
		if(tSys.fftNum >=5){
			tSys.fftNum = 0;
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
