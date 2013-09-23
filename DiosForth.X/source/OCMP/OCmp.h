/*********************************************************************
 *
 *                  OCmp.h
 *                  DUINOMITE - DIOS Forth
 *
 * v.1.0 - 20.12.2012, L.Pekny
 ********************************************************************/

#ifndef __OCMP_H__
#define __OCMP_H__

#include "..\GenericTypeDefs.h"


#define Freq1		440
#define Freq2		31469
#define PWMsrcOff	0
#define PWMsrc2		2
#define PWMsrc3		3
#define PWM1		1
#define PWM2		2
#define PWM3		3
#define PWM4		4
#define PWM5		5


void PWMopen(void);
void PWMclose(void);
void PWMwrite(BYTE PWMport, BYTE PWMsrc, UINT PWMfreq, BYTE PWMdutyperc);
void PWMsrc(BYTE PWMport, BYTE PWMsrc);
WORD PWMperiod(UINT freq);
WORD PWMduty(WORD period, BYTE percent);


//Set value for init OCx
#define	cTim2f		GetPeripheralClock()/32/Freq1-1  // Freq1
#define	cTim3f		GetPeripheralClock()/32/Freq2-1  // Freq2
#define cTim2		(T2_ON | T2_PS_1_32)
#define cTim3		(T3_ON | T3_PS_1_32)

#define cOC2s		cTim2f/2
#define cOC2p		cOC2s
#define cOC3s		cTim3f/2
#define cOC3p		cOC3s

#define cOCon2		(OC_ON | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE)
#define cOCon3		(OC_ON | OC_TIMER3_SRC | OC_PWM_FAULT_PIN_DISABLE)
#define cOCoff		(OC_ON | OC_MODE_OFF)


const void* DevTabPWM[];


#endif		

