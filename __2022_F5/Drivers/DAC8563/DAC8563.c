/*
 * DAC8563.c
 *
 *  Created on: Jul 4, 2022
 *      Author: 王志涵
 *      brief: 若直接移植DAC8563驱动，不通过cubemx配置IO引脚，可直接将DAC8563_GPIO_Init()取消注释，并取消头文件中引脚重命名注释
 */


#include "DAC8563.h"

void dac_delay_us(u32 length)
{
   length = length*12;
   while(length--);
}
void DAC8563_WRITE(uint8_t cmd,uint16_t data)
{
  uint8_t s=0;
  DAC8563_SYNC_H;
  dac_delay_us(30);
  DAC8563_SYNC_L;
  DAC8563_SCLK_L;
  for( s=0;s<8;s++)
  {
 if((cmd&0x80)==0x80){DAC8563_DIN_H;}
    else{DAC8563_DIN_L;}
    dac_delay_us(4);
    DAC8563_SCLK_H;
    dac_delay_us(4);
    cmd<<=1;
    DAC8563_SCLK_L;
    dac_delay_us(4);
  }
  for( s=0;s<16;s++)
  {
    if((data&0x8000)==0x8000){DAC8563_DIN_H;}
    else{DAC8563_DIN_L;}
    dac_delay_us(4);
    DAC8563_SCLK_H;
    dac_delay_us(4);
    data<<=1;
    DAC8563_SCLK_L;
    dac_delay_us(4);
  }
}

//void DAC8563_GPIO_Init(void){
//
//	  GPIO_InitTypeDef GPIO_InitStruct = {0};
//
//	  /* GPIO Ports Clock Enable */
//	  __HAL_RCC_GPIOC_CLK_ENABLE();
//	  __HAL_RCC_GPIOH_CLK_ENABLE();
//	  __HAL_RCC_GPIOE_CLK_ENABLE();
//	  __HAL_RCC_GPIOB_CLK_ENABLE();
//	  __HAL_RCC_GPIOA_CLK_ENABLE();
//
//	  /*Configure GPIO pin Output Level */
//	  HAL_GPIO_WritePin(GPIOE, DAC8563_SCK_Pin|DAC8563_LD_Pin, GPIO_PIN_RESET);
//
//	  /*Configure GPIO pin Output Level */
//	  HAL_GPIO_WritePin(GPIOB, DAC8563_SYN_Pin|DAC8563_DIN_Pin|DAC8563_CLR_Pin, GPIO_PIN_RESET);
//
//	  /*Configure GPIO pin : PtPin */
//	  GPIO_InitStruct.Pin = DAC8563_SCK_Pin;
//	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	  GPIO_InitStruct.Pull = GPIO_NOPULL;
//	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//	  HAL_GPIO_Init(DAC8563_SCK_GPIO_Port, &GPIO_InitStruct);
//
//	  /*Configure GPIO pin : PtPin */
//	  GPIO_InitStruct.Pin = DAC8563_LD_Pin;
//	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	  GPIO_InitStruct.Pull = GPIO_NOPULL;
//	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;;
//	  HAL_GPIO_Init(DAC8563_LD_GPIO_Port, &GPIO_InitStruct);
//
//	  /*Configure GPIO pins : PBPin PBPin PBPin */
//	  GPIO_InitStruct.Pin = DAC8563_SYN_Pin|DAC8563_DIN_Pin|DAC8563_CLR_Pin;
//	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	  GPIO_InitStruct.Pull = GPIO_NOPULL;
//	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//	  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//
//    DAC8563_LDAC_H;
//}

void DAC8563_Init(void)
{
    DAC8563_CLR_L;
    DAC8563_LDAC_H;
    DAC8563_WRITE(CMD_RESET_ALL_REG, DATA_RESET_ALL_REG);   //复位
    HAL_Delay(10);
    DAC8563_WRITE(CMD_PWR_UP_A_B, DATA_PWR_UP_A_B);    //上电DAC-A和DAC-B
    DAC8563_WRITE(CMD_INTERNAL_REF_EN, DATA_INTERNAL_REF_EN);// 启用内部参考和复位DAC以获得增益 = 2
    DAC8563_WRITE(CMD_GAIN, DATA_GAIN_B2_A2);      // Set Gains
    HAL_Delay(200);

    DAC_OutAB(0,0);
}

void DAC_OutA(float data_a) //单位 mV
{
    DAC8563_WRITE(CMD_SETA_UPDATEA, data_a*mV_to_DA-3234);
    DAC8563_LDAC_L;
    dac_delay_us(4);
    DAC8563_LDAC_H;
}
void DAC_OutB(float data_b) //单位 mV
{
    DAC8563_WRITE(CMD_SETB_UPDATEB, data_b*mV_to_DA+32767);
    DAC8563_LDAC_L;
    dac_delay_us(4);
    DAC8563_LDAC_H;
}
void DAC_OutAB(float data_a, float data_b) //单位 mV
{
    DAC8563_WRITE(CMD_SETA_UPDATEA, data_a*mV_to_DA-3234);
    DAC8563_WRITE(CMD_SETB_UPDATEB, data_b*mV_to_DA+32767);
    DAC8563_LDAC_L;
    dac_delay_us(4);
    DAC8563_LDAC_H;
}
