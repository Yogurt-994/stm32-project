/*
 * ad_da.h
 *
 *  Created on: Mar 28, 2022
 *      Author: Õı÷æ∫≠
 */

#ifndef INC_AD_DA_H_
#define INC_AD_DA_H_
#include "main.h"
#include "hmi_driver.h"
#include "hmi_user_uart.h"
#include "cmd_queue.h"
#include "cmd_process.h"

#define ad_daLength 80

extern uint16_t adData[ad_daLength];
extern uint16_t daData[ad_daLength];
extern uint8 uartData[ad_daLength];
extern uint8_t getKey;

void ad_daStartGet();
//void DMA2_Stream0_Handler();

#endif /* INC_AD_DA_H_ */
