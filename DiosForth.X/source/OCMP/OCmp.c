/*********************************************************************
 *
 *                  OCmp
 *                  DUINOMITE - DIOS Forth
 *
 * v.1.0 - 20.12.2012, Output Compare PWM, L.Pekny
 ********************************************************************/

#include <plib.h>
#include "..\GenericTypeDefs.h"
#include "..\HardwareProfile.h"
#include "OCmp.h"


void PWMopen(void)
{
	OpenTimer2(cTim2, cTim2f);
	OpenTimer3(cTim3, cTim3f);

    OpenOC1(cOCon2, cOC2s, cOC2p);
    OpenOC5(cOCon3, cOC3s, cOC3p);
}


void PWMclose(void)
{
	CloseOC1();
	CloseOC5();
}


void PWMwrite(BYTE PWMport, BYTE PWMsrc, UINT PWMfreq, BYTE PWMdutyperc)
{
	WORD PWMperiod,PWMdutycycle;
	UINT bak;

	PWMperiod=GetPeripheralClock()/64/PWMfreq-1;
	bak=(PWMperiod*PWMdutyperc)/100;
	PWMdutycycle=(WORD) bak;

	switch(PWMport)
	{
	  case 1:
			OC1CON=0; OC1RS=PWMdutycycle;
			if (PWMsrc==2) {PR2=PWMperiod; OC1CON=cOCon2;}
			if (PWMsrc==3) {PR3=PWMperiod; OC1CON=cOCon3;}
			if (PWMsrc<=1) {OC1CON=cOCoff;}
		break;
	  case 2:
			OC2CON=0; OC2RS=PWMdutycycle;
			if (PWMsrc==2) {PR2=PWMperiod; OC2CON=cOCon2;}
			if (PWMsrc==3) {PR3=PWMperiod; OC2CON=cOCon3;}
			if (PWMsrc<=1) {OC2CON=cOCoff;}
		break;
	  case 3:
			OC3CON=0; OC3RS=PWMdutycycle;
			if (PWMsrc==2) {PR2=PWMperiod; OC3CON=cOCon2;}
			if (PWMsrc==3) {PR3=PWMperiod; OC3CON=cOCon3;}
			if (PWMsrc<=1) {OC3CON=cOCoff;}
		break;
	  case 4:
			OC4CON=0; OC4RS=PWMdutycycle;
			if (PWMsrc==2) {PR2=PWMperiod; OC4CON=cOCon2;}
			if (PWMsrc==3) {PR3=PWMperiod; OC4CON=cOCon3;}
			if (PWMsrc<=1) {OC4CON=cOCoff;}
		break;
	  case 5:
			OC5CON=0; OC5RS=PWMdutycycle;
			if (PWMsrc==2) {PR2=PWMperiod; OC5CON=cOCon2;}
			if (PWMsrc==3) {PR3=PWMperiod; OC5CON=cOCon3;}
			if (PWMsrc<=1) {OC5CON=cOCoff;}
		break;
	  default:
		break;
	}
}


void PWMsrc(BYTE PWMport, BYTE PWMsrc)
{
	switch(PWMport)
	{
	  case 2:
			OC2CON=0;
			if (PWMsrc==2) {OC2CON=cOCon2;}
			if (PWMsrc==3) {OC2CON=cOCon3;}
			if (PWMsrc<=1) {OC2CON=cOCoff;}
		break;
	  case 3:
			OC3CON=0;
			if (PWMsrc==2) {OC3CON=cOCon2;}
			if (PWMsrc==3) {OC3CON=cOCon3;}
			if (PWMsrc<=1) {OC3CON=cOCoff;}
		break;
	  case 4:
			OC4CON=0;
			if (PWMsrc==2) {OC4CON=cOCon2;}
			if (PWMsrc==3) {OC4CON=cOCon3;}
			if (PWMsrc<=1) {OC4CON=cOCoff;}
		break;
	  case 5:
			OC5CON=0;
			if (PWMsrc==2) {OC5CON=cOCon2;}
			if (PWMsrc==3) {OC5CON=cOCon3;}
			if (PWMsrc<=1) {OC5CON=cOCoff;}
		break;
	  default:
		break;
	}
}


WORD PWMperiod(UINT freq)  // 20..1250000Hz
{
	return (WORD) GetPeripheralClock()/64/freq-1;
}


WORD PWMduty(WORD period, BYTE percent)  // 0..100%
{
	UINT bak;

	bak=(period*percent)/100;
	return (WORD) bak;
}


// ********** DIOS **********


void fhPWMwrite(void *addr)
{
	int port,src,freq,perc;

	perc=*(int *)addr;				// perc=TOS
	freq=*(int *)(addr-4);			// freq=NOS
	src =*(int *)(addr-8);
	port=*(int *)(addr-12);
	PWMwrite(port,src,freq,perc);
}


const void* DevTabPWM[] =
{
	0, 0, PWMopen, PWMclose,		// init, release, open, close
	0, fhPWMwrite, 0, 0				// read, write, ?read, ?write
};
