#ifndef _RC_H_
#define _RC_H_
#include "stm32f10x.h"
typedef struct {
	      u8  NRF24L01_RXDATA[32];
	      u8  NRF24L01_TXDATA[32];
				int16_t ROLL;
				int16_t PITCH;
				int16_t THROTTLE;
				int16_t YAW;
				int16_t AUX1;}RC_TRANSDATA;


extern  RC_TRANSDATA RC_Data;

extern int airBatt_Buffer[100];
void RC_Sent(void);
void sentCorrect(u8 ent);
#endif
