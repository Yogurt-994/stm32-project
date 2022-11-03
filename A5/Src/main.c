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
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "sys.h"
#include "delay.h"
#include "lcd.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define KEY0 HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5)		
#define KEY1 HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)
#define WK_UP HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)
#define KEY0_PRES 	1
#define KEY1_PRES	2
#define WKUP_PRES   3
#define LED0 PAout(8)   	//LED0
#define LED1 PDout(2)   	//LED1
#define PWMout	PCout(4)		//PWM
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Menu(void);
int KEY_Scan(void);
void EssentialRequirements(void);
void BreathLamp(void);
void PWM(void);
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
  delay_init(72);
  LCD_Init();
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  LCD_ShowString(30,40,200,24,24,"Mini STM32F103");	
  Menu();		//菜单
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

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

  /** Initializes the CPU, AHB and APB busses clocks 
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
  /** Initializes the CPU, AHB and APB busses clocks 
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
//按键功能
int KEY_Scan(void)
{
	static int key=1;			//用来判断按键是否按下
	if(key && (KEY0==0||KEY1==0||WK_UP==1))
	{
		HAL_Delay(1);
		key=0;					//置零防止多次判断按键
		if(KEY0==0) 		return 1;
		else if(KEY1==0)	return 2;
		else if(WK_UP==1)	return 3;
	}
	else if(KEY0==1&&KEY1==1&&WK_UP==0)		//无按键按下
		key=1;
	return 0;
}

//基本要求
void EssentialRequirements(void)
{
	int key;
	HAL_Delay(100);
	while(1)
	{
		LCD_ShowString(30,40,200,24,24,"Mini STM32F103");	
		LCD_ShowString(20,70,200,24,24," Horse Race Lamp ");	
		LED0=0;		//跑马灯	
		LED1=1;
		HAL_Delay(50);
		LED0=1;
		LED1=0;
		HAL_Delay(50);
		key=KEY_Scan();
		if(key==1)
		{
			HAL_Delay(100);
			while(1)
			{
				LCD_ShowString(30,40,200,24,24,"Mini STM32F103");	
				LCD_ShowString(20,70,200,24,24," LED Flashing    ");
				LED0=1;		//LED灯闪烁状态
				LED1=1;
				HAL_Delay(50);
				key=KEY_Scan();
				HAL_Delay(50);
				if(key==1||key==2||key==3)
				{break;}
				LED0=0;
				LED1=0;
				HAL_Delay(50);
				key=KEY_Scan();
				HAL_Delay(50);
				if(key==1||key==2||key==3)
				{break;}
			}
		}
		if(key==1||key==2||key==3)
		{
			LCD_ShowString(30,40,200,24,24,"Mini STM32F103");	
			LCD_ShowString(20,70,200,24,24," LED Is Off      ");	
			LED0=1;
			LED1=1;
			HAL_Delay(200);
			break;
		}
	}
}

//呼吸灯模块
void BreathLamp(void)
{
	int a,b=1001;
	int key;
	delay_ms(200);
	while(1)
	{
		LCD_ShowString(30,40,200,24,24,"Mini STM32F103");	
		LCD_ShowString(20,70,200,24,24," Breath Lamp     ");
		for(a=1;a<b;a++)
		{
			LED0=1;
			LED1=1;
			delay_us(a);
			LED0=0;
			LED1=0;
			delay_us(b-a);
			key=KEY_Scan();
			if(key==1||key==2||key==3)	break;
		}
		if(key==1||key==2||key==3)		//跳出呼吸灯，熄灭
		{
			LCD_ShowString(30,40,200,24,24,"Mini STM32F103");	
			LCD_ShowString(20,70,200,24,24," Breath Lamp Off");
			LED0=1;
			LED1=1;
			HAL_Delay(200);
			break;
		}
		for(a=b;a>0;a--)
		{
			LED0=1;
			LED1=1;
			delay_us(a);
			LED0=0;
			LED1=0;
			delay_us(b-a+1);
			key=KEY_Scan();
			if(key==1||key==2||key==3)	break;
		}
		if(key==1||key==2||key==3)		//跳出呼吸灯，熄灭
		{
			LCD_ShowString(30,40,200,24,24,"Mini STM32F103");	
			LCD_ShowString(20,70,200,24,24," Breath Lamp Off");
			LED0=1;
			LED1=1;
			HAL_Delay(200);
			break;
		}
	}
}
	



//菜单模块
void Menu(void)
{
	int key;
	HAL_Delay(100);  	//防止连续判断
	while(1)
	{
		key=KEY_Scan();
		switch(key)		//菜单1
		{
			case 0:break;
			case 1:EssentialRequirements();break;		//KEY0按键实现基本要求
			case 2:BreathLamp();break;					//KEY1按键实现呼吸灯
			case 3:PWM();break;							//WK_UP按键实现PWM矩形波输出
		}
	}
}


//PWM输出
//占空比和频率
void PWM(void)
{
	int key;
//	static int out=0;			//静态按键扫描变量,用于跳出PWM()函数
								//使用静态变量有bug,后采取将out变量在第一个while循环里面定义
	PWM_100HZ_50();
	HAL_Delay(100);
	while(1)
	{		
		int out;
		//100HZ,50%PWM
		PWMout=1;
		delay_ms(5);
		PWMout=0;
		delay_ms(5);
		key=KEY_Scan();
		if(key==3)
		{
			HAL_Delay(100);
			LCD_Clear(WHITE);
			PWM_100HZ_90();
			while(1)
			{
				//100HZ,90%PWM
				PWMout=0;
				delay_ms(9);
				PWMout=1;
				delay_ms(1);
				key=KEY_Scan();
				if(key==3)
				{
					HAL_Delay(100);
					LCD_Clear(WHITE);
					PWM_100HZ_25();
					while(1)
					{
						//100HZ,25%PWM
						PWMout=0;
						delay_us(2500);
						PWMout=1;
						delay_us(7500);
						key=KEY_Scan();
						if(key==3)
						{
							HAL_Delay(100);
							LCD_Clear(WHITE);
							PWM_50HZ_50();
							while(1)
							{
								//50HZ,50%PWM
								PWMout=0;
								delay_ms(10);
								PWMout=1;
								delay_ms(10);
								key=KEY_Scan();
								if(key==3)
								{
									HAL_Delay(100);
									LCD_Clear(WHITE);
									PWM_50HZ_90();
									while(1)
									{
										//50HZ,90%PWM
										PWMout=0;
										delay_ms(18);
										PWMout=1;
										delay_ms(2);
										key=KEY_Scan();
										if(key==3)
										{
											HAL_Delay(100);
											LCD_Clear(WHITE);
											PWM_50HZ_25();
											while(1)
											{
												//50HZ,25%PWM
												PWMout=0;
												delay_us(5000);
												PWMout=1;
												delay_us(15000);
												key=KEY_Scan();
												if(key==3)
												{
													HAL_Delay(100);
													LCD_Clear(WHITE);
													PWM_250HZ_50();
													while(1)
													{
														//250HZ,50%PWM
														PWMout=0;
														delay_ms(2);
														PWMout=1;
														delay_ms(2);
														key=KEY_Scan();
														if(key==3)
														{
															HAL_Delay(100);
															LCD_Clear(WHITE);
															PWM_250HZ_90();
															while(1)
															{
																//250HZ,90%PWM																	//250HZ,90%PWM
																PWMout=0;
																delay_us(3600);
																PWMout=1;
																delay_us(400);
																key=KEY_Scan();
																if(key==3)
																{
																	HAL_Delay(100);
																	LCD_Clear(WHITE);
																	PWM_250HZ_25();
																	while(1)
																	{
																		//250HZ,25%PWM
																		PWMout=0;
																		delay_us(1000);
																		PWMout=1;
																		delay_us(3000);																
																		out=KEY_Scan();
																		if(out==1||out==2||out==3)
																			{LCD_Clear(WHITE);
																			LCD_ShowString(30,40,200,24,24,"Mini STM32F103");
																			break;}
																
																	}
																}
																if(out==1||out==2||out==3)
																{break;}
															}
														}
														if(out==1||out==2||out==3)
																{break;}
													}
												}
												if(out==1||out==2||out==3)
													{break;}
											}
										}
										if(out==1||out==2||out==3)
											{break;}
									}
								}
								if(out==1||out==2||out==3)
									{break;}
							}
						}
						if(out==1||out==2||out==3)
							{break;}
					}
				}
				if(out==1||out==2||out==3)
					{break;}
			}
		}
		if(out==1||out==2||out==3)
			{break;}
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
