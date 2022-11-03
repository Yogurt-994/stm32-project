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
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "delay.h"
#include "sys.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//下面的方式是通过直接操作HAL库函数方式读取IO
#define KEY0        HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5)  //KEY0按键PC5
#define KEY1        HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15) //KEY1按键PA15
#define WK_UP       HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)  //WKUP按键PA0

#define KEY0_PRES 	1
#define KEY1_PRES	2
#define WKUP_PRES   3

#define LED0 PAout(8)   	//LED0
#define LED1 PDout(2)   	//LED1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
int KEY=0;				//按键中断获取按键返回值
int PWM1_Flag=1;		//PWM1标志变量
int __PWM2=1;			//基本要求PWM2中断使能与否
static int __Funcition=0;		//功能切换按键
int Flag_u_d=-1;
uint16_t PWM1_ARR[10]={499,499,272,499,319,380,380,199,272,49};		//PWM1自动重载值
uint16_t PWM1_PSC[10]={63,31,38,15,19,13,11,19,12,63};					//PWM1预分频值
//uint16_t PWM1_CRR[10]={500,500,273,500,320,2500,100,200,273,50};		//PWM1捕获比较值
//uint16_t PWM2_CRR[17]={10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90};		//PWM捕获比较值
uint16_t PWM2_CRR[17]={90,85,80,75,70,65,60,55,50,45,40,35,30,25,20,15,10};		//PWM捕获比较值

int PWM2_Flag=0;
int size=100;
uint16_t PWM_Phase_Position[5]={0,20,40,60,80};					//发挥部分PWM相位步进
uint16_t PWM_Phase_Position_2[5]={0,80,60,40,20};	
uint16_t SPWM[]={1600,1700,1800,1899,1997,2094,2188,2281,2370,2457,2540,2619,2695,2766,2832,2894,
2950,3002,3047,3087,3121,3149,3171,3187,3196,3200,3196,3187,3171,3149,3121,3087,
3047,3002,2950,2894,2832,2766,2695,2619,2540,2457,2370,2281,2188,2094,1997,1899,
1800,1700,1600,1499,1399,1300,1202,1105,1011,918,829,742,659,580,504,433,
367,305,249,197,152,112,78,50,28,12,3,0,3,12,28,50,
78,112,152,197,249,305,367,433,504,580,659,742,829,918,1011,1105,
1202,1300,1399,1499};
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
  MX_TIM4_Init();
  MX_TIM3_Init();
  MX_TIM5_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_OC_Start(&htim4,TIM_CHANNEL_2);		//使能定时器4通道2
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);		//使能定时器3通道2 
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);		//使能定时器3通道1 
  HAL_TIM_Base_Start_IT(&htim5);
  HAL_TIM_Base_Start_IT(&htim2);
  __HAL_TIM_MOE_ENABLE(&htim4);
  __HAL_TIM_ENABLE(&htim4);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	if(__Funcition==0)		//基本要求
	{
	  if(KEY==1)
	  {
		  KEY=0;		//按键置零
		  __HAL_TIM_SET_PRESCALER(&htim4,PWM1_PSC[PWM1_Flag]);		//设置预分频
		  __HAL_TIM_SET_AUTORELOAD(&htim4,PWM1_ARR[PWM1_Flag]);		//设置自动重载值	
		  PWM1_Flag++;
		  if(PWM1_Flag==10)	
			  PWM1_Flag=0;
	  }
	  else if(KEY==2)
	  {
		  KEY=0;		//按键置零
		  if(__PWM2==0)
		  {
			  LED1=1;
			  HAL_TIM_Base_Start_IT(&htim5);
			  __PWM2=1;
		  }
		  else if(__PWM2==1)
		  {
			  LED1=0;
			  HAL_TIM_Base_Stop_IT(&htim5);
			  __PWM2=0;
		  }
	  }
	  else if(KEY==3)
	  {
		  KEY=0;
		  HAL_TIM_Base_Stop_IT(&htim5);
		  __Funcition=1;
	  }
  }
	else if(__Funcition==1)		//发挥部分
	{
		if(KEY==1)
		{
			KEY=0;
			if(Flag_u_d==0)
			{
				PWM2_Flag=PWM2_Flag+1;
			}
			Flag_u_d=1;
			__HAL_TIM_SET_PRESCALER(&htim4,31);					//设置预分频
			__HAL_TIM_SET_AUTORELOAD(&htim4,99);				//设置自动重载值
			__HAL_TIM_SET_PRESCALER(&htim3,63);					//设置预分频
			__HAL_TIM_SET_AUTORELOAD(&htim3,99);				//设置自动重载值
			__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_2,PWM_Phase_Position[PWM2_Flag]);
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,50);
			PWM2_Flag++;
			if(PWM2_Flag==5)
				PWM2_Flag=0;
		}
		else if(KEY==2)
		{
			KEY=0;
			if(Flag_u_d==1)
			{
				PWM2_Flag=PWM2_Flag-1;
			}
			Flag_u_d=0;
			__HAL_TIM_SET_PRESCALER(&htim4,31);					//设置预分频
			__HAL_TIM_SET_AUTORELOAD(&htim4,99);				//设置自动重载值
			__HAL_TIM_SET_PRESCALER(&htim3,63);					//设置预分频
			__HAL_TIM_SET_AUTORELOAD(&htim3,99);				//设置自动重载值
			__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_2,PWM_Phase_Position[PWM2_Flag]);
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,50);
			PWM2_Flag--;
			if(PWM2_Flag==-1)
				PWM2_Flag=4;
		}
		else if(KEY==3)
		{
			KEY=0;
			__Funcition=0;
			HAL_TIM_Base_Stop_IT(&htim2);
		}
	}
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
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
}

/* USER CODE BEGIN 4 */
//中断服务程序中需要做的事情
//在HAL库中所有的外部中断服务函数都会调用此函数
//GPIO_Pin:中断引脚号
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin==KEY0_Pin)
	{
		KEY=KEY0_PRES;		//1
		LED1=!LED1;
	}
	else if(GPIO_Pin==KEY1_Pin)
	{
		KEY=KEY1_PRES;		//2
		LED1=!LED1;
	}
	else if(GPIO_Pin==WK_UP_Pin)
	{
		KEY=WKUP_PRES;		//3
		LED1=!LED1;
	}
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static int PWM2_Flag=0;
	static int i=0;
	if(++i==100) i=0;
	if(htim==(&htim2))
	{
		__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_1,SPWM[i]);
	}
	if(htim==(&htim5))
	{
		LED0=!LED0;
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,PWM2_CRR[PWM2_Flag]);
		PWM2_Flag++;
		if(PWM2_Flag==18)
			PWM2_Flag=0;
	}
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
