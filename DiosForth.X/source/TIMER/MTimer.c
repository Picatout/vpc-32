/*********************************************************************
 *
 *                  MTimer
 *                  DUINOMITE - DIOS Forth
 *
 * v.1.0 - 19.12.2012, Timer, Delay, L.Pekny
 ********************************************************************/

#include <plib.h>
#include "..\GenericTypeDefs.h"
#include "..\HardwareProfile.h"
#include "MTimer.h"


const unsigned int countPerMicroSec = ((SYS_FREQ/1000000)/2);


long Timesec=0, Timemsec=0;
UINT _backupCoreTime=0, _targetCoreTime=0;
UINT _backupCoreTimems=0, _targetCoreTimems=0;


void TIM1open(void)
{
	OpenTimer1(cTim1, cTim1f);
	ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_2);
}


void TIM1close(void)
{
	CloseTimer1();
}


WORD TIM1read(void)
{
	return ReadTimer1();
}


void TIM1write(UINT T1period)
{
	WritePeriod1(T1period);
}


void isrTIM1(void)  // Threads
{
	IncTimesec();   // Inc clock [s,ms]
}


void __ISR(_TIMER_1_VECTOR, ipl2) _Timer1Handler(void)
{
	mT1ClearIntFlag();  // clear the interrupt flag
	isrTIM1();
}


void delay_us(UINT us)
{
    UINT targetCount;  
    UINT backupCount; 

    backupCount = ReadCoreTimer();  // Backup current count

    targetCount = countPerMicroSec*us;      

	while((ReadCoreTimer()-backupCount) < targetCount);
}


void IncTimesec(void)  // For clock from CoreTimer
{
	UINT baktime;

	baktime=ReadCoreTimer();

	if ((baktime-_backupCoreTimems)>=_targetCoreTimems)
	{
	  _backupCoreTimems=baktime;
	  _targetCoreTimems=count1msec;  // 1ms
	  if (Timemsec<999) {Timemsec++;}
	}

	if ((baktime-_backupCoreTime)>=_targetCoreTime)
	{
	  _backupCoreTime=baktime;
	  _targetCoreTime=count1sec;  // 1s
	  Timesec++; Timemsec=0;
	}
}


// ********** DIOS **********


void fhTIM1read(void *addr)
{
	*(int *)addr=TIM1read();		// TOS=@TIM1
}


void fhTIM1write(void *addr)
{
	int period=*(int *)addr;		// period=TOS
	TIM1write(period);
}


void fhTIM1gettime(void *addr)
{
	*(int *)addr=Timesec;			// TOS=Timesec
}


void fhTIM1getmsec(void *addr)
{
	*(int *)addr=Timemsec;			// TOS=Timemsec
}


void fhTIM1settime(void *addr)
{
	Timesec=*(int *)addr;			// sec=TOS
}


void fhTIM1setmsec(void *addr)
{
	Timemsec=*(int *)addr;			// msec=TOS
}


const void* DevTabTIM1[] =
{
	0, 0, TIM1open, TIM1close,		// init, release, open, close
	fhTIM1read, fhTIM1write, 0, 0,	// read, write, ?read, ?write
	fhTIM1gettime, fhTIM1getmsec,	// get time, get msec
	fhTIM1settime, fhTIM1setmsec	// set time, set msec
};
