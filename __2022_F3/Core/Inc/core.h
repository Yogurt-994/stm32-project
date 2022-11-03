/*
 * core.h
 *
 *  Created on: Jul 27, 2022
 *      Author: Kierti
 */

#ifndef INC_CORE_H_
#define INC_CORE_H_

#include "main.h"

#define waitMeasure 0
#define measureAM   1
#define measureFM   2
#define measureAuto 3


typedef enum{
	No = 0,
	AM = 1,
	FM = 2,
}moduType_E;

void sysInit(void);
void __measureAM(void);
void __measureFM(void);
uint32_t detectForFre(void);
moduType_E idenModuType(void);
uint32_t extractModuWave(void);
void demodulationOut(void);

#endif /* INC_CORE_H_ */
