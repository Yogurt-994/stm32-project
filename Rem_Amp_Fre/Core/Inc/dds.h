/*
 * dds.h
 *
 *  Created on: Mar 13, 2022
 *      Author: Õı÷æ∫≠
 */

#ifndef SRC_DDS_H_
#define SRC_DDS_H_
#include "main.h"
#include "arm_math.h"
#include "arm_common_tables.h"

#define  _Size 2048
extern uint16_t sinWave[_Size];
extern uint16_t dmaRxBuffer[_Size];
extern float32_t offset[_Size/2];
extern uint16_t Jag[2000];
extern volatile uint32_t RxLength;

void DAC_Init();
void DMA1_Stream_Handler();
void sweepFrequencyOn();
void sweepFrequencyOff();

#endif /* SRC_DDS_H_ */
