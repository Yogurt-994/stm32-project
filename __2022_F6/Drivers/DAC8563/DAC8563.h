/*
 * DAC8563.h
 *
 *  Created on: Jul 4, 2022
 *      Author: Õı÷æ∫≠
 */

#ifndef DAC8563_DAC8563_H_
#define DAC8563_DAC8563_H_

#include "main.h"
//void DAC8563_GPIO_Init(void);
void DAC8563_Init(void);
void DAC_OutAB(float data_a, float data_b);
void DAC_OutA(float data_a);
void DAC_OutB(float data_b);
void dac_delay_us(u32 length);

//#define DAC8563_SCK_Pin GPIO_PIN_14
//#define DAC8563_SCK_GPIO_Port GPIOE
//#define DAC8563_LD_Pin GPIO_PIN_15
//#define DAC8563_LD_GPIO_Port GPIOE
//#define DAC8563_SYN_Pin GPIO_PIN_10
//#define DAC8563_SYN_GPIO_Port GPIOB
//#define DAC8563_DIN_Pin GPIO_PIN_11
//#define DAC8563_DIN_GPIO_Port GPIOB
//#define DAC8563_CLR_Pin GPIO_PIN_12
//#define DAC8563_CLR_GPIO_Port GPIOB

#define DAC8563_CLR_H   HAL_GPIO_WritePin(DAC8563_CLR_GPIO_Port,DAC8563_CLR_Pin,GPIO_PIN_SET)
#define DAC8563_CLR_L   HAL_GPIO_WritePin(DAC8563_CLR_GPIO_Port,DAC8563_CLR_Pin,GPIO_PIN_RESET)
#define DAC8563_SYNC_H  HAL_GPIO_WritePin(DAC8563_SYN_GPIO_Port,DAC8563_SYN_Pin,GPIO_PIN_SET)
#define DAC8563_SYNC_L  HAL_GPIO_WritePin(DAC8563_SYN_GPIO_Port,DAC8563_SYN_Pin,GPIO_PIN_RESET)
#define DAC8563_SCLK_H  HAL_GPIO_WritePin(DAC8563_SCK_GPIO_Port,DAC8563_SCK_Pin,GPIO_PIN_SET)
#define DAC8563_SCLK_L  HAL_GPIO_WritePin(DAC8563_SCK_GPIO_Port,DAC8563_SCK_Pin,GPIO_PIN_RESET)
#define DAC8563_DIN_H   HAL_GPIO_WritePin(DAC8563_DIN_GPIO_Port,DAC8563_DIN_Pin,GPIO_PIN_SET)
#define DAC8563_DIN_L   HAL_GPIO_WritePin(DAC8563_DIN_GPIO_Port,DAC8563_DIN_Pin,GPIO_PIN_RESET)
#define DAC8563_LDAC_H  HAL_GPIO_WritePin(DAC8563_LD_GPIO_Port ,DAC8563_LD_Pin,GPIO_PIN_SET)
#define DAC8563_LDAC_L  HAL_GPIO_WritePin(DAC8563_LD_GPIO_Port ,DAC8563_LD_Pin,GPIO_PIN_RESET)
#define CMD_SETA_UPDATEA        0x18
#define CMD_SETB_UPDATEB        0x19
#define CMD_UPDATE_ALL_DACS      0x0F
// Gain
#define CMD_GAIN          			0x02
#define DATA_GAIN_B2_A2       	0x00
#define DATA_GAIN_B2_A1         0x01
#define DATA_GAIN_B1_A2         0x02
#define DATA_GAIN_B1_A1         0x03
#define CMD_PWR_UP_A_B         	0x20
#define DATA_PWR_UP_A           0x0001
#define DATA_PWR_UP_B           0x0002
#define DATA_PWR_UP_A_B         0x0003
#define DATA_PWR_DOWN1_A        0x0041
#define DATA_PWR_DOWN1_B        0x0042
#define DATA_PWR_DOWN1_A_B      0x0043
#define DATA_PWR_DOWN2_A        0x0081
#define DATA_PWR_DOWN2_B        0x0082
#define DATA_PWR_DOWN2_A_B      0x0083
#define DATA_PWR_DOWN3_A        0x00C1
#define DATA_PWR_DOWN3_B        0x00C2
#define DATA_PWR_DOWN3_A_B      0x00C3
#define CMD_RESET_ALL_REG       0x28
#define DATA_RESET_ALL_REG      0x0001
#define DATA_RESET_ADCAB        0x0000
#define CMD_LDAC_DIS            0x30
#define DATA_LDAC_AB            0x0000
#define DATA_LDAC_B             0x0001
#define DATA_LDAC_A             0x0002
#define DATA_LDAC_NAB           0x0003
#define CMD_INTERNAL_REF_DIS    0x38
#define DATA_INTERNAL_REF_DIS   0x0000
#define CMD_INTERNAL_REF_EN     0x38
#define DATA_INTERNAL_REF_EN    0x0001

#define mV_to_DA 3.2768f
#define mV_to_DA2 6.5534f

#endif /* DAC8563_DAC8563_H_ */
