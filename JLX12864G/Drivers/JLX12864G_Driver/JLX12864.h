/*
 * JLX12864.h
 *
 *  Created on: May 2, 2022
 *      Author: 王志涵
 */

#ifndef JLX12864G_DRIVER_JLX12864_H_
#define JLX12864G_DRIVER_JLX12864_H_

#include "stm32f4xx_hal.h"
#include "base.h"

#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long

//PIN12-PIN1
#define lcd_cs1_h HAL_GPIO_WritePin(GPIOB,CS_Pin,GPIO_PIN_SET)
#define lcd_cs1_l HAL_GPIO_WritePin(GPIOB,CS_Pin,GPIO_PIN_RESET)

#define lcd_reset_h HAL_GPIO_WritePin(GPIOB,RST_Pin,GPIO_PIN_SET)
#define lcd_reset_l HAL_GPIO_WritePin(GPIOB,RST_Pin,GPIO_PIN_RESET)

#define lcd_rs_h HAL_GPIO_WritePin(GPIOB,RS_Pin,GPIO_PIN_SET)
#define lcd_rs_l HAL_GPIO_WritePin(GPIOB,RS_Pin,GPIO_PIN_RESET)

#define lcd_sid_h HAL_GPIO_WritePin(GPIOB,SDA_Pin,GPIO_PIN_SET)
#define lcd_sid_l HAL_GPIO_WritePin(GPIOB,SDA_Pin,GPIO_PIN_RESET)

#define lcd_sclk_h HAL_GPIO_WritePin(GPIOB,SCLK_Pin,GPIO_PIN_SET)
#define lcd_sclk_l HAL_GPIO_WritePin(GPIOB,SCLK_Pin,GPIO_PIN_RESET)

#define Rom_CS_h HAL_GPIO_WritePin(GPIOB,ROM_CS_Pin,GPIO_PIN_SET)
#define Rom_CS_l HAL_GPIO_WritePin(GPIOB,ROM_CS_Pin,GPIO_PIN_RESET)

#define Rom_SCK_h HAL_GPIO_WritePin(GPIOB,ROM_SCK_Pin,GPIO_PIN_SET)
#define Rom_SCK_l HAL_GPIO_WritePin(GPIOB,ROM_SCK_Pin,GPIO_PIN_RESET)

#define Rom_IN_h HAL_GPIO_WritePin(GPIOE,ROM_IN_Pin,GPIO_PIN_SET)
#define Rom_IN_l HAL_GPIO_WritePin(GPIOE,ROM_IN_Pin,GPIO_PIN_RESET)

#define Rom_OUT_h HAL_GPIO_WritePin(GPIOE,ROM_OUT_Pin,GPIO_PIN_SET)
#define Rom_OUT_l HAL_GPIO_WritePin(GPIOE,ROM_OUT_Pin,GPIO_PIN_RESET)

#define	Rom_OUT_cin   HAL_GPIO_ReadPin(GPIOE,ROM_OUT_Pin)

//IO设置
#define ROM_OUT_IN()  {GPIOB->CRH&=0xFFFF0FFF;GPIOC->CRH|=0x00000008;}				//低八位引脚的PB8脚定义为输入
#define ROM_OUT_OUT() {GPIOB->CRH&=0xFFFF0FFF;GPIOC->CRH|=0x00000003;}  	   	//低八位引脚的PB8脚定义为输出

void JLX12864G_086_GPIOInit(void);
void transfer_command_lcd(int data1);
void transfer_data_lcd(int data1);
void initial_lcd(void);
void lcd_address(uint page,uint column);
void clear_screen(void);
void display_128x64(uchar *dp);
void display_graphic_16x16(uchar page,uchar column,uchar *dp);
void display_graphic_8x16(uchar page,uchar column,uchar *dp);
void display_graphic_5x8(uchar page,uchar column,uchar *dp);
void send_command_to_ROM( uchar datu );
uchar get_data_from_ROM(void);
void get_and_write_16x16(ulong fontaddr,uchar page,uchar column);
void get_and_write_8x16(ulong fontaddr,uchar page,uchar column);
void get_and_write_5x8(ulong fontaddr,uchar page,uchar column);
void display_GB2312_string(uchar page,uchar column,uchar *text);
void display_string_5x8(uchar page,uchar column,uchar *text);

#endif /* JLX12864G_DRIVER_JLX12864_H_ */
