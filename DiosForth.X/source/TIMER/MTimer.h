/*********************************************************************
 *
 *                  MTimer.h
 *                  DUINOMITE - DIOS Forth
 *
 * v.1.0 - 19.12.2012, L.Pekny
 ********************************************************************/

#ifndef __MTIMER_H__
#define __MTIMER_H__


#include "..\GenericTypeDefs.h"


#define count1msec countPerMicroSec*1000;
#define count1sec  countPerMicroSec*1000000;

//Set value for init Timer1
#define	cTim1f		GetPeripheralClock()/32/5000-1	// 5kHz
#define cTim1		(T1_ON | T1_PS_1_64)			// 40MHz/32


long Timesec, Timemsec;


void TIM1open(void);
void TIM1close(void);
WORD TIM1read(void);
void TIM1write(UINT T1period);

void delay_us(UINT us);
void IncTimesec(void);

const void* DevTabTIM1[];


#endif		

