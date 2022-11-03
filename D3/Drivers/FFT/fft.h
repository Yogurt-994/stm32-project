#ifndef __FFT_H
#define __FFT_H
#include "base.h"

#define   NPT   1024

extern u32 lBufInArray[NPT];
extern u32 lBufOutArray[NPT];
extern u32 lBufMagArray[NPT];
extern u32 MagBufArray[NPT/2];
extern u32 Wave;

void GetPowerMag(void);
void InitBufInArray(void);
void InitBufInArray2(void);
void lcd_print_fft(unsigned int *p);
void lcd_show_fft(unsigned int *p);
void select_max(float *f,float *a);

//void window();
#endif
