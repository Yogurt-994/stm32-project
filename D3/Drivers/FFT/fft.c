#include "fft.h"
#include "math.h"
#include "lcd.h"
#include "base.h"
u32 lBufInArray[NPT];
u32 lBufOutArray[NPT];
u32 lBufMagArray[NPT];
u32 lBUFPHASE[NPT];
u32 MagBufArray[NPT/2];
float PI2=6.28318530717959;
float PI=3.14159265358979;
u32 Fs=2000;              
u32 Wave;
/******************************************************************
函数名称:InitBufInArray()
函数功能:模拟采样数据，采样数据中包含3种频率正弦波
参数说明:
备    注:在lBufInArray数组中，每个数据的高16位存储采样数据的实部，
          低16位存储采样数据的虚部(总是为0)
*******************************************************************/
void InitBufInArray(void)
{
    unsigned short i;
    float fx;                                       //Fn=i*Fs/NPT		//由于此处i是从0开始的，所以不需要再减1
    for(i=0; i<NPT; i++)                            //频率分辨率Fs/1024=187.5Hz,所以每一个i就代表187.5Hz
    {                                               //因为采样频率=Fs，所以屏幕上的一个格子的长度代表(1/Fs)秒
        fx = 1000 * sin(PI2 * i * 350.0 / Fs) +     //2pi*f*t=2pi*i*f1/Fs,那么f=f1
             3000 * sin(PI2 * i * 8400.0 / Fs) +
             4000 * sin(PI2 * i * 10000.0 / Fs);
        lBufInArray[i] = ((signed short)fx) << 16;
    }
} 

void InitBufInArray2(void)
{
    unsigned short i;
    float fx;
    for(i=0; i<NPT; i++)
    {
        fx = 4000 * sin(PI2 * i * 375.2 / Fs) +
             1000 * sin(PI2 * i * 564.7 / Fs) +
             1500 * sin(PI2 * i * 937.1 / Fs)+1000;   //加了1000只是直流分量加了1000，其他都不变
        lBufInArray[i] = ((signed short)fx) << 16;
    }
}
/******************************************************************
函数名称:GetPowerMag()
函数功能:计算各次谐波幅值                          short 的范围，是-32767 到 32767 。也就是 -(2^15 - 1)到(2^15 - 1)。
参数说明:
备　　注:先将lBufOutArray分解成实部(X)和虚部(Y)，然后计算幅值(sqrt(X*X+Y*Y)
*******************************************************************/
void GetPowerMag(void)
{
    signed short lX,lY;                                                  //算频率的话Fn=i*Fs/NPT		//由于此处i是从0开始的，所以不需要再减1
    float X,Y,Mag;                                                      
    unsigned short i;
    for(i=0; i<NPT/2; i++)                                                  //经过FFT后，每个频率点处的真实幅值  A0=lBufOutArray[0]/NPT
    {                                                                       //                                 Ai=lBufOutArray[i]*2/NPT
        lX  = (lBufOutArray[i] << 16) >> 16;  //lX  = lBufOutArray[i];
        lY  = (lBufOutArray[i] >> 16);
			                                 
        X = NPT * ((float)lX) / 32768;//除以32768再乘65536是为了符合浮点数计算规律,不管他
        Y = NPT * ((float)lY) / 32768;
        Mag = sqrt(X * X + Y * Y) / NPT;
        if(i == 0)
            lBufMagArray[i] = (unsigned long)(Mag * 32768);   //0Hz是直流分量，直流分量不需要乘以2
        else
            lBufMagArray[i] = (unsigned long)(Mag * 65536);
    }
}


void PowerPhase(u16 nfill)

{
    unsigned short i;
    signed short lX,lY;  

		for (i=0; i < NPT/2; i++)
		{
						lX= (lBufOutArray[i]<<16)>>16; /* 取低16bit，sine_cosine --> cos */
						lY= (lBufOutArray[i] >> 16);   /* 取高16bit，sine_cosine --> sin */    
						{
								float X=  NPT*((float)lX)/32768;
								float Y = NPT*((float)lY)/32768;
								float phase = atan(Y/X);
								 if (Y>=0)
								 {
										if (X>=0)
											;
										else
										 phase+=PI;  
								 }
								 else
								 {             
										if (X>=0)
											phase+=PI2;                  
										else 
											phase+=PI;                    
								 }                            
								lBUFPHASE[i] = phase*180.0/PI;
						}    
				}
}

void lcd_show_fft(unsigned int *p)
{
	unsigned int *pp = p+1;             //p+1相当于我直接把0HZ部分滤掉了
	unsigned int i = 0;
	for(i = 0;i<480;i++)
	{
		//分辨率hz
		//每个小矩形宽度为1，其实这里没有显示完所有的
    //512个值，频率可达到 Hz	
    //0.11是我根据屏幕显示高度调整的一个值,频谱闪的话记得改这个值！！！！！ 320*240屏幕		320*=780
		LCD_Fill(0,        i, *pp*0.11, (i+1), WHITE);     //有效部分白色       
		LCD_Fill(*pp*0.11, i, 270,       (i+1), BLACK);   //其他就黑色
    pp++;
	}
	
}
/***********************************************
找最大值，次大值……对应的频率，分析波形
*************************************************/
void select_max(float *f,float *a)
{
	  int i;
	  float k,k1,m;
	  float aMax =0.0,aSecondMax = 0.0,aThirdMax = 0.0,aFourthMax=0.0;
      float fMax =0.0,fSecondMax = 0.0,fThirdMax = 0.0,fFourthMax=0.0;
	  int nMax=0,nSecondMax=0,nThirdMax=0,nFourthMax=0;
	  for ( i = 1; i < NPT/2; i++)//i必须是1，是0的话，会把直流分量加进去！！！！
    {
        if (a[i]>aMax)
        {
            aMax = a[i]; 
			nMax=i;
			fMax=f[nMax];
        }
    }
		for ( i=1; i < NPT/2; i++)
		{
				if (nMax == i)
				{
						continue;//跳过原来最大值的下标，直接开始i+1的循环
				}
        if (a[i]>aSecondMax&&a[i]>a[i+1]&&a[i]>a[i-1])
        {
            aSecondMax = a[i]; 
					  nSecondMax=i;
					  fSecondMax=f[nSecondMax];
        }
		}
		for ( i=1; i < NPT/2; i++)
    {
				if (nMax == i||nSecondMax==i)
				{
						continue;//跳过原来最大值的下标，直接开始i+1的循环
				}
        if (a[i]>aThirdMax&&a[i]>a[i+1]&&a[i]>a[i-1])
        {
            aThirdMax = a[i]; 
					  nThirdMax=i;
					  fThirdMax=f[nThirdMax];
        }
    }
		for ( i=1; i < NPT/2; i++)
    {
				if (nMax == i||nSecondMax==i||nThirdMax==i)
				{
						continue;//跳过原来最大值的下标，直接开始i+1的循环
				}
        if (a[i]>aFourthMax&&a[i]>a[i+1]&&a[i]>a[i-1])
        {
            aFourthMax = a[i]; 
					  nFourthMax=i;
					  fFourthMax=f[nFourthMax];
        }
    }
		
		k=fabs(2*fMax-fSecondMax);
		k1=fabs(3*fMax-fSecondMax);
		m=fabs((float)(aMax-3.0*aSecondMax));
		
		if(k<=5)
		{
			LCD_ShowString(100,00,12*4,16,16,"Jag      ");
			Wave=1;
		}
		else if(k1<=5&&m<0.3)
		{			
			LCD_ShowString(100,00,12*4,16,16,"Square  ");
			Wave=2;
		}
		else if(k1<=5&&m>=0.3)
		{
			LCD_ShowString(100,00,12*4,16,16,"Triangle");
			Wave=3;
		}
		else 
		{				
			LCD_ShowString(100,00,12*4,16,16,"Sin     ");
			Wave=4;
		}
}


void lcd_print_fft(unsigned int *p)
{
	unsigned int *pp = p;             //p+1相当于我直接把0HZ部分滤掉了(改成了不过滤)
	unsigned int i = 0,j = 0;
	float f[NPT/2]={0.00},a[NPT/2]={0.00};
	for(i=0;i<NPT/2;i++)
  {
	  if(*pp>80)//看情况调，若是数字太跳就调大,把小的幅值过滤，以防干扰
		{
			f[j]=(float)i*Fs/NPT;
			//LCD_ShowFloat4(0,j*12,f[j],6,12);
			a[j]=(float)*pp*(3.3/4096);
			//LCD_ShowFloat4(100,j*12,a[j],2,12);
			j++;
		}
		pp++;
  }
	select_max(f,a);
}

