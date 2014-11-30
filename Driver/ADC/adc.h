#ifndef __ADC_H
#define	__ADC_H


#include "stm32f10x.h"


extern __IO uint16_t ADC_ConvertedValue[7];


void ADC1_Init(void);


#endif /* __ADC_H */

