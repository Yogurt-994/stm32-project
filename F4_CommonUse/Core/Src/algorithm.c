/*
 * algorithm.c
 *
 *  Created on: Jul 5, 2022
 *      Author: 王志涵
 */

#include "algorithm.h"
#include "arm_math.h"
#include "fir.h"

/*  傅里叶变换  */
#define FFT_SIZE AD_Size
static float FFT_Buffer[FFT_SIZE];
/*
 * @brief 计算FFT后幅值
 */
void calc_FFT(float*Input,float*Output)
{
    //FIR_calc(Input);
    arm_rfft_fast_instance_f32 S;//结构体
    arm_rfft_fast_init_f32(&S,FFT_SIZE);//初始化该结构体
    arm_rfft_fast_f32(&S, Input, FFT_Buffer, 0);//ifft_flag=0是正变换， 为1则是逆变换
    arm_cmplx_mag_f32(FFT_Buffer, Output, FFT_SIZE);
    arm_scale_f32(Output,2.0f/FFT_SIZE,Output,FFT_SIZE);    //换算成真实V
    Output[0] *= 0.5f;
}

/*
* @name PowerPhaseRadians_f32
* @function: 求相位
* @param _ptr  复位地址，含实部和虚部
*        _phase 求出相位，单位角度制，范围(-180, 180]
*        _usFFTPoints  复数个数，每个复数是两个float32_t数值
*        _uiCmpValue  比较值，需要求出相位的数值
* @retval 无
*/
void PowerPhaseRadians_f32(float32_t *_ptr, float32_t *_phase, uint16_t _usFFTPoints, float32_t _uiCmpValue)
{
	float32_t lX, lY;
	uint16_t i;
	float32_t phase;
	float32_t mag;

	for (i=0; i < _usFFTPoints; i++)
	{
		lX= _ptr[2*i];  	  /* 实部 */
		lY= _ptr[2*i + 1];    /* 虚部 */

 		phase = atan2f(lY, lX);    		  				 /* atan2求解的结果范围是(-pi, pi], 弧度制 */
		arm_sqrt_f32((float32_t)(lX*lX+ lY*lY), &mag);   /* 求模 */

//		if(_uiCmpValue < mag)
//		{
//			_phase[i] = 0;
//		}
//		else
//		{
			_phase[i] = phase* 180.0f/3.1415926f;   /* 将求解的结果由弧度转换为角度 */
//		}
	}
}
/*
 * @brief 计算FFT后相位
 */
void cal_fftPhase(float*Input,float*Output){
	arm_rfft_fast_instance_f32 S;
	arm_rfft_fast_init_f32(&S,FFT_SIZE);	//初始化该结构体
	arm_rfft_fast_f32(&S,Input,FFT_Buffer,0);//ifft_flag=0是正变换， 为1则是逆变换
	PowerPhaseRadians_f32(FFT_Buffer,Output,FFT_SIZE,500);
}



/*  AD采样  */
ArrayParam_T tAD_Params;
float
AD_arr[AD_Size] = {0,},
*pAD_arr, *pAD_arr_end,

AD_sum_arr[AD_Size] = {0,},
*pAD_sum_arr, *pAD_sum_arr_end
;

u8
TFT_arr[AD_Size] = {0,},
*pTFT_arr, *pTFT_arr_end
;

void ADarr_Init(void)
{
    pAD_arr = AD_arr;
    pAD_arr_end = AD_arr + AD_Size;
    arm_fill_f32(0,AD_arr,AD_Size);
}

/* 周期采样并返回平均值
float get_AD_Results()
{
    pAD_arr=AD_arr;pAD_arr_end=AD_arr+AD_N;
    HAL_TIM_Base_Start_IT(&htim3);
    while(pAD_arr!=pAD_arr_end){};
    HAL_TIM_Base_Stop(&htim3);
    Calc_array(&AD_Params,AD_arr,AD_N);
    return AD_Params.Aver;
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim == &htim3)
    {
        if(pAD_arr < pAD_arr_end)
        {
            *pAD_arr++ = (get_ADS_ch1()-ADS8688_ZERO)*AD_mV_Scale;
        }
    }
}
*/

/*
 * @brief 计算波形数组的参数
 */
void Calc_array(ArrayParam_T *params,float* calc_addr,u32 calc_len)
{
	uint32_t useless;
    //arm_offset_f32(calc_addr,-32768,calc_addr,calc_len);        		//去掉原始数据的直流偏置
    //arm_scale_f32(calc_addr,AD_mV_Scale,calc_addr,calc_len);    		//换算成真实V
    //FIR_calc(calc_addr); calc_addr += FIR_Shift;  calc_len = FIR_NUM-FIR_Shift;
    arm_max_f32(calc_addr,calc_len,&params->max,&useless);  			//计算最大值
    arm_min_f32(calc_addr,calc_len,&params->min,&useless);  			//计算最小值
    params->Vpp=params->max-params->min;                   				//计算出峰峰值
    arm_mean_f32(calc_addr,calc_len,&params->Aver);    					//计算平均值
    arm_rms_f32(calc_addr,calc_len,&params->Rms);      					//计算有效值
}

/**
 * @brief 生成正弦波表函数
 * @param points:正弦波表点数
 * 		  amplitude：幅值
 * 		  mid_value：中值
 * 		  _sinWave：正弦波表数组/指针
 * @retval _sinWave 正弦波表数组指针
 */
int * __sinWaveData(int points,int amplitude,int mid_value,int * _sinWave)
{
    int index = 0, halfPoint = 0;
    float radian = 0.0;        	//弧度
    float peak = 0.0;       	//峰值
    int tem = 0;
    halfPoint = points / 2;			//水平线位置 单片机没有负电压 水平线为点值数量的一半
    radian = PI / halfPoint;        	// π/2 内每一个点对应的弧度值
    for( index = points/4; index < points+points/4; index++ )
    {
    	peak = amplitude * sin( radian * index + PI / 2 + PI ) + mid_value;
        tem = (int )(peak * 1.0);
        _sinWave[index - points/4] = tem;
    }
    return _sinWave;
}

/* 数据决策 */
float many_in_array(float *p)   //返回数组中的众数
{
	u32 max=0,most = 0;
	u32 i = 0, j = 0;
	u32 cnt_arr[CHOICE_LEN] = { 0, };
	for (i = 0; i < CHOICE_LEN; i++)
	{
		for (j = i; j < CHOICE_LEN; j++)
		{
			if (p[i] == p[j])
				cnt_arr[i]++;
		}
	}
	for (i = 0; i < CHOICE_LEN; i++)
	{
		if (max < cnt_arr[i])
		{
			max = cnt_arr[i];
			most = i;
		}
	}
	return p[most];
}
void Sort(float *a, u8 n)   //对有n位的数组进行升序排序
{
	u32 i, j, imin;
    float s;
	for (i = 0; i < n - 1; i++)
	{
		imin = i;
		for (j = i + 1; j<n; j++)
            if (a[imin]>a[j]) imin = j;
		if (i != imin){
			s = a[i]; a[i] = a[imin]; a[imin] = s;
		}
	}
}
/*
判断当前数据是否认定为平滑:最大值\最小值均未超过阈值
当小于阈值返回平均值,认定缓冲区内数据已经平滑
*/
float is_Smooth(float* arr,u32 len)
{
    float result,max,min,useless;
    arm_mean_f32(arr,len,&result);
    arm_max_f32(arr,len,&max,(u32*)&useless);  //计算最大值
    arm_min_f32(arr,len,&min,(u32*)&useless);  //计算最小值
    if(\
        ( (max-result)/result<THRESHOLD )\
      &&( (result-min)/result<THRESHOLD )\
    ) return result;
    else return 0;
}
/*
支持多路决策
*/
float Make_Choices(float choice,u16 row,u16 mode)
{
    /* 变量/静态变量初始化 */
    static float Choices[ROW][CHOICE_LEN]={0,};
    static float temp_arr[CHOICE_LEN]={0,};
    static float good_choice=0;
	static u16 i[ROW]={0,};
    float result=0,useless;

    /* 循环刷新缓冲 */
	Choices[row][i[row]]=choice;
	if(++i[row]==CHOICE_LEN)i[row]=0;

    /* 滤波函数处理 */
    if(mode == AVER)    /*除去最大值\最小值求平均值*/
    {
        float max,min;
        arm_mean_f32(Choices[row],CHOICE_LEN,&result);			   //计算平均值
        arm_max_f32(Choices[row],CHOICE_LEN,&max,(u32*)&useless);  //计算最大值
        arm_min_f32(Choices[row],CHOICE_LEN,&min,(u32*)&useless);  //计算最小值
        result = (result*CHOICE_LEN-max-min)/(CHOICE_LEN-2);
    }
    else if(mode == MANY)   /* 得到众数 */
    {
        return many_in_array(Choices[row]);
    }
    else if(mode == MID)   /* 得到中位数 */
    {
        arm_copy_f32(Choices[row], temp_arr,CHOICE_LEN);
        Sort(temp_arr,CHOICE_LEN);
        return temp_arr[CHOICE_LEN/2];
    }
    else if(mode == SMOOTH)   /* 判断平稳?更新数据:显示旧平稳数据 */
    {
        result=is_Smooth(Choices[row],CHOICE_LEN);
        if (result>0) //如果平稳,更新显示
        {
            good_choice=result;
        }
        return good_choice;
    }
    else if(mode == MAX)   /* 得到最大值 */
    {
        arm_max_f32(Choices[row],CHOICE_LEN,&result,(u32*)&useless);  //计算最大值
        return result;
    }
    else if(mode == MIN)   /* 得到最小值 */
    {
        arm_min_f32(Choices[row],CHOICE_LEN,&result,(u32*)&useless);  //计算最小值
        return result;
    }
    else if(mode == NON)   /* 不作任何操作,返回 0 */
    {
        return 0;
    }
	return result;
}

/* FIR滤波器 */
//uint16_t FIR_Shift=FIR_SHIFT ;                  //移位阶数的一半
//uint16_t calc_FIR_W(uint32_t Ts,uint32_t Tc)   //采样周期、被采信号周期
//{
//    uint16_t w;
//    w=20*Ts/Tc+2;
//    if(w>9)return 9;
//    else return w;
//}   //返回值在1-9之间
//static float FIR_State[BLOCK_SIZE+FIR_ORDER] ;   //FIR滤波器状态缓存
//static arm_fir_instance_f32 FIR_Sta; //定义结构体S
//void calc_FIR(float *input,uint16_t w) //可控制截止频率的FIR
//{
//    arm_fir_init_f32(&FIR_Sta, FIR_LEN, (float*)LowPassFIR[w-1], FIR_State, BLOCK_SIZE); //初始化结构体
//    for(u16 i=0;i<NUMBLOCKS;i++)
//    {
//        arm_fir_f32(&FIR_Sta, input + (i * BLOCK_SIZE), input + (i * BLOCK_SIZE),BLOCK_SIZE);
//    }
//}
//void FIR_calc(float *input) //固定截止频率的FIR
//{
//    u16 i;
//    float FIR_State[BLOCK_SIZE+FIR_ORDER] ;   //FIR滤波器状态缓存
//    arm_fir_instance_f32 FIR_Sta; //定义结构体S
//    arm_fir_init_f32(&FIR_Sta, FIR_LEN, (float*)LowPass64, FIR_State, BLOCK_SIZE); //初始化结构体
//    for(i=0;i<NUMBLOCKS;i++)
//    {
//        arm_fir_f32(&FIR_Sta, input + (i * BLOCK_SIZE), input + (i * BLOCK_SIZE),BLOCK_SIZE);
//    }
//    for(i=0;i<AD_Size;i++) //数据移位
//    {
//        input[i] = input[i+FIR_SHIFT];
//    }
//    for(i=AD_Size;i<AD_Size+FIR_SHIFT;i++) //数据移位
//    {
//        input[i] = 0;
//    }
//}
