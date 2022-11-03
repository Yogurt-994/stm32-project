/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "base.h"
#include "lcd.h"
#include "24cxx.h"
#include "touch.h"
#include "math.h"
#include "stm32_dsp.h"
#include "table_fft.h"
#include "fft.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
void Set_DAC(float v);
void DAC1_Set_Vol(u16 vol);
void SineWave_Data( u16 num,uint32_t *D,float U);
void Key_staset(u16 x,u16 y,u8 keyx,u8 sta);//触摸屏按键捕获状态函数
u8 Get_key(u16 x,u16 y);					//触摸屏按键获取函数
void Function(void);							//功能实现
void UI(void);									//界面
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define PI 3.14159265358979
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
u8 KEY=0;
u8 KEY_TOUCH=0;

u16 ADC_value[NPT];
u16 ADC_value_convert[NPT];
int ADC_value_convert_c[NPT];
int ADC_value_self[NPT];
int i;
u16 j;
u16 Max_Val=0;
u16 Row;					//行，数组定位
u16 Freq;					//频率
uint32_t AD_Total=0;		//ADC的单次DMA采集信号总和
uint32_t AD_Ave;			//AD采集平均值
uint32_t AD_Peak;			//AD采集最大值
uint32_t AD_Peak_D;			//AD采集最小值
uint32_t AD_P_to_P;			//AD采集峰峰值
uint32_t AD_Peak_up;		//AD采集正幅值
uint32_t AD_DcCom;			//AD采集信号直流分量
uint32_t AD_RMS;			//AD采集RMS
int Count=0;
float Vpp_Rate;

uint32_t x=0;
float temp;

//u16 sinx[100]={2080,2209,2337,2463,2588,2711,2832,2949,3063,3172,3278,3378,3473,3563,3646,3723,
//3794,3858,3914,3964,4005,4039,4065,4083,4093,4095,4089,4075,4053,4023,3985,3940,
//3887,3827,3760,3686,3605,3519,3426,3328,3225,3118,3006,2891,2772,2650,2526,2400,
//2273,2144,2016,1887,1759,1633,1508,1385,1264,1147,1033,924,818,718,623,533,
//450,373,302,238,182,132,91,57,31,13,3,1,7,21,43,73,
//111,156,209,269,336,410,491,577,670,768,871,978,1090,1205,1324,1446,
//1570,1696,1823,1952};
u16 sinx[50]={2048,2305,2557,2802,3034,3251,3449,3625,3776,3900,3995,4059,4091,4091,4059,3995,
3900,3776,3625,3449,3251,3034,2802,2557,2305,2048,1791,1539,1294,1062,845,647,
471,320,196,101,37,5,5,37,101,196,320,471,647,845,1062,1294,
1539,1791};
int Sin_Flag=1;
int Sin_Psc[4]={23,23,23,1};
int Sin_Crr[4]={1199,119,11,1};

float rate;
int PSC=11,ARR;
short sinXX[100]={0,129,257,384,509,633,754,872,987,1097,1204,1305,1402,1493,1578,1657,
1729,1795,1853,1904,1948,1984,2012,2032,2044,2046,2044,2032,2012,1984,1948,1904,
1853,1795,1729,1657,1578,1493,1402,1305,1204,1097,987,872,754,633,509,384,
257,129,0,-129,-257,-384,-509,-633,-754,-872,-987,-1097,-1204,-1305,-1402,-1493,
-1578,-1657,-1729,-1795,-1853,-1904,-1948,-1984,-2012,-2032,-2044,-2046,-2044,-2032,-2012,-1984,
-1948,-1904,-1853,-1795,-1729,-1657,-1578,-1493,-1402,-1305,-1204,-1097,-987,-872,-754,-633,
-509,-384,-257,-129};
u16 sin_convert[100];

int Square[100]={2046,2046,2046,2046,2046,2046,2046,2046,2046,2046,
	2046,2046,2046,2046,2046,2046,2046,2046,2046,2046,
	2046,2046,2046,2046,2046,2046,2046,2046,2046,2046,
	2046,2046,2046,2046,2046,2046,2046,2046,2046,2046,
	2046,2046,2046,2046,2046,2046,2046,2046,2046,2046,
	-2046,-2046,-2046,-2046,-2046,-2046,-2046,-2046,-2046,-2046,
	-2046,-2046,-2046,-2046,-2046,-2046,-2046,-2046,-2046,-2046,
	-2046,-2046,-2046,-2046,-2046,-2046,-2046,-2046,-2046,-2046,
	-2046,-2046,-2046,-2046,-2046,-2046,-2046,-2046,-2046,-2046,
	-2046,-2046,-2046,-2046,-2046,-2046,-2046,-2046,-2046,-2046,};
u16 Square_convert[100];

int Triangle[100]={-2046,-1967,-1886,-1805,-1724,-1643,-1562,-1481,
	-1400,-1319,-1238,-1157,-1076,-995,-914,-833,-752,-671,-590,
	-509,-428,-347,-266,-185,-104,-23,58,139,220,301,382,463,
	544,625,706,787,868,949,1030,1111,1192,1273,1354,1435,1516,
	1597,1678,1759,1840,1921,204,1967,1886,1805,1724,1643,
	1562,1481,1400,1319,1238,1157,1076,995,914,833,752,671,
	590,509,428,347,266,185,104,23,-58,-139,-220,-301,-382,
	-463,-544,-625,-706,-787,-868,-949,-1030,-1111,-1192,
-1273,-1354,-1435,-1516,-1597,-1678,-1759,-1840,-1921};
u16 Triangle_convert[100];
	
int Jag[100]={-2048,-2007,-1966,-1925,-1884,-1843,-1802,-1761,-1720,
	-1679,-1638,-1597,-1556,-1515,-1474,-1433,-1392,
	-1351,-1310,-1269,-1228,-1187,-1146,-1105,-1064,
	-1023,-982,-941,-900,-859,-818,-777,-736,-695,
	-654,-613,-572,-531,-490,-449,-408,-367,-326,
	-285,-244,-203,-162,-121,-80,-39,2,43,84,125,
	166,207,248,289,330,371,412,453,494,535,576,
	617,658,699,740,781,822,863,904,945,986,1027,
	1068,1109,1150,1191,1232,1273,1314,1355,1396,
	1437,1478,1519,1560,1601,1642,1683,1724,1765,1806,
	1847,1888,1929,1970,2011};
u16 Jag_convert[100];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_DAC_Init();
  MX_ADC3_Init();
  MX_TIM3_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */
  delay_init(72);
  LCD_Init();							//LCD屏初始化
  tp_dev.init();				   		//触摸屏初始化 
  
  HAL_TIM_Base_Start(&htim7);
  HAL_ADC_Start_DMA(&hadc3,(uint32_t*)&ADC_value,NPT);					//开启ADC3的DMA通道
  HAL_DAC_Start_DMA(&hdac,DAC1_CHANNEL_1,(uint32_t*)sinx,50,DAC_ALIGN_12B_R);		//开启DAC通道1的DMA
  HAL_TIM_Base_Start(&htim3);
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
  
  UI();
  
  HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,2048);		//设置DA初始值为0
  AD_Peak=ADC_value[0];
  AD_Peak_D=ADC_value[0];
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  Function();
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void Function(void)
{
	KEY=KEY_Scan(0);
	KEY_TOUCH=Get_key(0,160);
	if(KEY==KEY0_PRES||KEY_TOUCH==7)			//改变正弦波频率
	{
		_LED0=!_LED0;
		HAL_DAC_Stop_DMA(&hdac,DAC1_CHANNEL_1);
		HAL_DAC_Start_DMA(&hdac,DAC1_CHANNEL_1,(uint32_t*)sinx,50,DAC_ALIGN_12B_R);		//开启DAC通道1的DMA
		__HAL_TIM_SET_PRESCALER(&htim7,Sin_Psc[Sin_Flag]);
		__HAL_TIM_SET_AUTORELOAD(&htim7,Sin_Crr[Sin_Flag]);
		Sin_Flag++;
		if(Sin_Flag==4)
			Sin_Flag=0;
	}
	for(i=0,AD_Total=0;i<NPT;i++)
	{
		if(AD_Peak<ADC_value[i])
			AD_Peak=ADC_value[i];						//求出波形峰最大值
		AD_Total+=ADC_value[i];
	}
	for(i=0;i<NPT;i++)
	{
		if(AD_Peak>ADC_value[i])      
			AD_Peak_D=ADC_value[i];
	}
	AD_P_to_P=AD_Peak-AD_Peak_D;						//AD采集电压峰峰值
	AD_Ave=AD_Total/1024;								//AD采集电压平均值	
	AD_Peak_up=AD_Peak-2048;							//AD采集电压峰值
//	for(i=0,AD_Total=0;i<NPT;i++)
//	{
//		lBufInArray[i]=ADC_value[i]<<16;
//	}
	for(j=2;j<NPT/2;j++)								//测量AD采集波形频率
	{
		if(lBufMagArray[j]>Max_Val)
		{
			Max_Val=lBufMagArray[j];
			Row=j;
		}
	}
	Freq=10000.0*Row/1024;	
	
	cr4_fft_1024_stm32(lBufOutArray, lBufInArray, NPT);	
	GetPowerMag();							//计算各次谐波幅值
	
////*********波形DA输出****************
	if(KEY==WKUP_PRES||KEY_TOUCH==6)					//波形输出
	{
		HAL_DAC_Stop_DMA(&hdac,DAC1_CHANNEL_1);
		__HAL_TIM_SET_PRESCALER(&htim7,23);
		__HAL_TIM_SET_AUTORELOAD(&htim7,299);
		HAL_DAC_Start_DMA(&hdac,DAC1_CHANNEL_1,(uint32_t*)ADC_value,NPT,DAC_ALIGN_12B_R);	
	}
////************************************
//***********Vpp放大缩小/自适应*********
	if(KEY_TOUCH==1)				//Vpp放大5倍
	{
		Vpp_Rate=AD_Peak_up*1000/2048;
		if(Vpp_Rate/1000<=0.2)
		{
			HAL_DAC_Stop_DMA(&hdac,DAC1_CHANNEL_1);
			for(i=0;i<NPT;i++)
				ADC_value_convert_c[i]=ADC_value[i];
			for(i=0;i<NPT;i++)
				ADC_value_convert_c[i]=(ADC_value_convert_c[i]-2047)*5+2047;
			for(i=0;i<NPT;i++)
				ADC_value_convert[i]=ADC_value_convert_c[i];
			HAL_DAC_Start_DMA(&hdac,DAC1_CHANNEL_1,(uint32_t*)ADC_value_convert,NPT,DAC_ALIGN_12B_R);
			LCD_ShowString(20,80,300,16,16,"Vpp_amplificate 5   ");
		}
		else
			LCD_ShowString(20,80,300,16,16,"Can't amplificate 5   ");
	}
	else if(KEY_TOUCH==2)			//Vpp放大10倍
	{
		Vpp_Rate=AD_Peak_up*1000/2048;
		if(Vpp_Rate/1000<=0.1)
		{
			HAL_DAC_Stop_DMA(&hdac,DAC1_CHANNEL_1);
			for(i=0;i<NPT;i++)
				ADC_value_convert_c[i]=ADC_value[i];
			for(i=0;i<NPT;i++)
				ADC_value_convert_c[i]=(ADC_value_convert_c[i]-2047)*10+2047;
			for(i=0;i<NPT;i++)
				ADC_value_convert[i]=ADC_value_convert_c[i];
			HAL_DAC_Start_DMA(&hdac,DAC1_CHANNEL_1,(uint32_t*)ADC_value_convert,NPT,DAC_ALIGN_12B_R);
			LCD_ShowString(20,80,300,16,16,"Vpp_amplificate 10   ");
		}
		else
			LCD_ShowString(20,80,300,16,16,"Can't amplificate 10");
	}
	else if(KEY_TOUCH==3)			//Vpp缩小5倍
	{
		Vpp_Rate=AD_Peak_up*1000/2048;
		if(Vpp_Rate/1000>=0.083)
		{
			HAL_DAC_Stop_DMA(&hdac,DAC1_CHANNEL_1);
			for(i=0;i<NPT;i++)
				ADC_value_convert_c[i]=ADC_value[i];
			for(i=0;i<NPT;i++)
				ADC_value_convert_c[i]=(ADC_value_convert_c[i]-2047)/5+2047;
			for(i=0;i<NPT;i++)
				ADC_value_convert[i]=ADC_value_convert_c[i];
			HAL_DAC_Start_DMA(&hdac,DAC1_CHANNEL_1,(uint32_t*)ADC_value_convert,NPT,DAC_ALIGN_12B_R);
			LCD_ShowString(20,80,300,16,16,"Vpp_narrow 5         ");
		}
		else
			LCD_ShowString(20,80,300,16,16,"Can't Vpp_narrow 5   ");
			
	}
	else if(KEY_TOUCH==4)			//Vpp缩小10倍
	{
		Vpp_Rate=AD_Peak_up*1000/2048;
		if(Vpp_Rate/1000>=0.167)
		{
			HAL_DAC_Stop_DMA(&hdac,DAC1_CHANNEL_1);
			for(i=0;i<NPT;i++)
				ADC_value_convert_c[i]=ADC_value[i];
			for(i=0;i<NPT;i++)
				ADC_value_convert_c[i]=(ADC_value_convert_c[i]-2047)/10+2047;
			for(i=0;i<NPT;i++)
				ADC_value_convert[i]=ADC_value_convert_c[i];
			HAL_DAC_Start_DMA(&hdac,DAC1_CHANNEL_1,(uint32_t*)ADC_value_convert,NPT,DAC_ALIGN_12B_R);
			LCD_ShowString(20,80,300,16,16,"Vpp_narrow 10       ");
		}
		else
			LCD_ShowString(20,80,300,16,16,"Can't Vpp_narrow 10  ");
	}
	else if(KEY_TOUCH==5)			//自适应
	{
		Vpp_Rate=AD_Peak_up*1000/2048*0.001;
		HAL_DAC_Stop_DMA(&hdac,DAC1_CHANNEL_1);
		for(i=0;i<NPT;i++)
			ADC_value_self[i]=ADC_value[i];
		for(i=0;i<NPT;i++)
			ADC_value_convert_c[i]=(ADC_value_self[i]-2047)*(1/Vpp_Rate)+2047;
		for(i=0;i<NPT;i++)
			ADC_value_convert[i]=ADC_value_convert_c[i];
		HAL_DAC_Start_DMA(&hdac,DAC1_CHANNEL_1,(uint32_t*)ADC_value_convert,NPT,DAC_ALIGN_12B_R);
		LCD_ShowString(20,80,300,16,16,"Self-Adaptation        ");
	}
//**************************************
//**********************************
//直流分量计算
	if(Wave==1)					//锯齿波
	{	AD_DcCom=2048;}
	else if(Wave==2)			//方波
	{	AD_DcCom=2048;	}
	else if(Wave==3)			//三角波
	{	AD_DcCom=2048;	}
	else if(Wave==4)			//正弦波
	{	AD_DcCom=2048;	}
	
	LCD_ShowxNum(50,20,(u32)Freq,4,16,0);
	LCD_ShowxNum(60,40,(u32)((AD_Peak-2048)*2*3.3/4096),1,16,0);
	LCD_ShowxNum(80,40,(u32)((AD_Peak-2048)*2*3300/4096%1000-100),3,16,0);	
	LCD_ShowxNum(80,60,(u32)(AD_DcCom*3.3/4096),1,16,0);
	LCD_ShowxNum(100,60,(u32)(AD_DcCom*3300/4096%1000),3,16,0);	

	x=HAL_DAC_GetValue(&hdac,DAC_CHANNEL_1);	//读取前面设置DAC的值
	LCD_ShowxNum(50,100,x,4,16,0);     	    	//显示DAC寄存器值
	temp=(float)x*(3.3/4096);			    	//得到DAC电压值
	x=temp;
	LCD_ShowxNum(60,120,temp,1,16,0);     	    //显示电压值整数部分
	temp-=x;
	temp*=1000;
	LCD_ShowxNum(80,120,temp,3,16,0X80); 	    //显示电压值的小数部分

	LCD_ShowxNum(60,140,(u32)(AD_Ave*3.3/4096),1,16,0);			//ADC采集电压整数部分
	LCD_ShowxNum(80,140,(u32)(AD_Ave*3300/4096%1000),3,16,0);	//ADC采集电压小数部分
	
	lcd_print_fft(lBufMagArray);			//波形识别cr4_fft_1024_stm32	
  }
void UI(void)
{
	POINT_COLOR=BLUE;				//以下为LCD屏显示
	LCD_ShowString(20,00,200,16,16,"WaveForm:");
	LCD_ShowString(20,20,200,16,16,"Fre:");
	LCD_ShowString(85,20,200,16,16,"Hz");
	LCD_ShowString(20,40,200,16,16,"Peak:");
	LCD_ShowString(70,40,200,16,16,".");
	LCD_ShowString(110,40,200,16,16,"V");
	LCD_ShowString(20,60,200,16,16,"Dc_Com:");
	LCD_ShowString(90,60,30,16,16,".");
	LCD_ShowString(130,60,200,16,16,"V");

	LCD_ShowString(110,140,200,16,16,"V");
	LCD_ShowString(70,140,30,16,16,".");
	LCD_ShowString(12,140,200,16,16," ADC:");

	LCD_ShowString(20,100,200,16,16,"VAL:");
	LCD_ShowString(20,120,200,16,16,"DAC:");
	LCD_ShowString(70,120,30,16,16,".");
	LCD_ShowString(110,120,200,16,16,"V");

	LCD_DrawRectangle(0,160,60,240);
	LCD_DrawRectangle(60,160,120,240);
	LCD_DrawRectangle(120,160,180,240); 
	LCD_DrawRectangle(180,160,240,240); 
	LCD_DrawRectangle(0,240,60,320);
	LCD_DrawRectangle(60,240,120,320);
	LCD_DrawRectangle(120,240,180,320); 
	LCD_DrawRectangle(180,240,240,320);		
	LCD_ShowString(15,190,120,80,24,"5U");
	LCD_ShowString(75,190,120,80,24,"10U");
	LCD_ShowString(135,190,120,80,24,"5D");
	LCD_ShowString(195,190,120,80,24,"10D");
	LCD_ShowString(15,270,120,80,24,"S_A");
	LCD_ShowString(75,270,120,80,24,"Out");
	LCD_ShowString(135,270,120,80,24,"Sin");
	LCD_ShowString(195,270,120,80,24,"DDS");
}
//触摸屏按键捕获状态函数
void Key_staset(u16 x,u16 y,u8 keyx,u8 sta)
{
	u16 i=keyx/4,j=keyx%4;		//j:横轴|i:竖轴
	if(keyx>9) return;
	if(sta&&keyx!=9)
		LCD_Fill(x+j*60+1,y+i*80+1,x+j*60+59,y+i*80+79,BLUE);	//当区域被按下区域被填充为红色
	else if(keyx!=9)
		LCD_Fill(x+j*60+1,y+i*80+1,x+j*60+59,y+i*80+79,WHITE);
	
	
	if(keyx!=9)		//区域被填充后重新显示
	{
		LCD_ShowString(15,190,120,80,24,"5U");
		LCD_ShowString(75,190,120,80,24,"10U");
		LCD_ShowString(135,190,120,80,24,"5D");
		LCD_ShowString(195,190,120,80,24,"10D");
		LCD_ShowString(15,270,120,80,24,"S_A");
		LCD_ShowString(75,270,120,80,24,"Out");
		LCD_ShowString(135,270,120,80,24,"Sin");
		LCD_ShowString(195,270,120,80,24,"DDS");
	}
}
//触摸屏按键获取函数
u8 Get_key(u16 x,u16 y)
{
	u16 i,j;
	static u8 Key_x=0;
	u8 key=0;
	tp_dev.scan(0);		//触摸屏扫描
	if(tp_dev.sta&TP_PRES_DOWN)		//触摸屏被按下
	{
		for(i=0;i<2;i++)
		{
			for(j=0;j<4;j++)
			{
				if(tp_dev.x[0]<(x+j*60+60)&&tp_dev.x[0]>(x+j*60)&&tp_dev.y[0]<(y+i*80+80)&&tp_dev.y[0]>(y+i*80))
				{
					key=i*4+j+1;		//i代表行，每一行变化需要两个值j代表列，每一列变化只需要变化一个值
					break;
				}
			}
			if(key)
			{
				if(Key_x==key) key=0;
				else
				{
					Key_staset(x,y,Key_x-1,0);
					Key_x=key;
					Key_staset(x,y,Key_x-1,1);
				}
				break;
			}
		}
	}
	else if(Key_x)		//每次从0开始，不记录上次的状态
	{
		Key_staset(x,y,Key_x-1,0);
		Key_x=0;
	}
	return key;
}
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if(hadc==&hadc3)
	{
//		HAL_ADC_Stop_DMA(&hadc3);
//		__HAL_ADC_CLEAR_FLAG(&hadc3, ADC_FLAG_STRT);		//清除ADC挂起的标志
		for(i=0,AD_Total=0;i<NPT;i++)
		{
			lBufInArray[i]=ADC_value[i]<<16;
		}
//		HAL_ADC_Start_DMA(&hadc3,(uint32_t*)&ADC_value,NPT);
	}
}
//num:要在一个正弦波中采集多少点
//*D:创建的一个数组用来存放正弦波各个点的数值的
//U:输出电压的峰值(0~1.5V)
//Pi:3.1415926 自己定义
void SineWave_Data( u16 num,uint32_t *D,float U)
{
    u16 i;
    for( i=0;i<num;i++)
    {
        D[i]=(uint32_t)((U*sin(( 1.0*i/(num-1))*2*PI)+U)*4095/3.3);
    }
}
void Set_DAC(float v)
{
	HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,(uint32_t)(v*4096/3.3));
}

//设置通道1输出电压
//vol:0~3300,代表0~3.3V
void DAC1_Set_Vol(u16 vol)
{
	double temp=vol;
	temp/=1000;
	temp=temp*4096/3.3;
    HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,temp);//12位右对齐数据格式设置DAC值
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
