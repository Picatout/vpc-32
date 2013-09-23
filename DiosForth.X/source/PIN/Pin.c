/*********************************************************************
 *
 *                  Pin I/O
 *                  DUINOMITE - DIOS Forth
 *
 * v.1.0 - 20.12.2012, I/O, digital, analog, L.Pekny
 ********************************************************************/

#include <plib.h>
#include "..\GenericTypeDefs.h"
#include "Pin.h"


void ADCopen(void)
{
	PORTSetPinsAnalogIn(IOPORT_B, cADC);

	CloseADC10();
	SetChanADC10(PARAM0);
    OpenADC10( PARAM1, PARAM2, PARAM3, PARAM4, PARAM5 );
    EnableADC10();
}


void ADCclose(void)
{
	PORTSetPinsDigitalIn(IOPORT_B, cADC);
	CloseADC10();
}


WORD ADCread(void)
{
	WORD value;

	value = ReadADC10(0);
	return value;
}


void PINopen(void)
{
	mJTAGPortEnable(0);

//	PORTWrite(IOPORT_A, cSETA);
//	PORTSetPinsDigitalIn(IOPORT_A, cINA);
//	PORTSetPinsDigitalOut(IOPORT_A, ~cINA);

	PORTWrite(IOPORT_B, cSETB);
    PORTSetPinsDigitalIn(IOPORT_B, cINB);
    PORTSetPinsDigitalOut(IOPORT_B, ~cINB);
/*
	PORTWrite(IOPORT_C, cSETC);
    PORTSetPinsDigitalIn(IOPORT_C, cINC);
    PORTSetPinsDigitalOut(IOPORT_C, ~cINC);

	PORTWrite(IOPORT_D, cSETD);
    PORTSetPinsDigitalIn(IOPORT_D, cIND);
    PORTSetPinsDigitalOut(IOPORT_D, ~cIND);

	PORTWrite(IOPORT_E, cSETE);
    PORTSetPinsDigitalIn(IOPORT_E, cINE);
    PORTSetPinsDigitalOut(IOPORT_E, ~cINE);

	PORTWrite(IOPORT_F, cSETF);
    PORTSetPinsDigitalIn(IOPORT_F, cINF);
    PORTSetPinsDigitalOut(IOPORT_F, ~cINF);

	PORTWrite(IOPORT_G, cSETG);
    PORTSetPinsDigitalIn(IOPORT_G, cING);
    PORTSetPinsDigitalOut(IOPORT_G, ~cING); */
}


void PINclose(void)
{
//	PORTResetPins(IOPORT_A, -1);
	PORTResetPins(IOPORT_B, -1);
/*	PORTResetPins(IOPORT_C, -1);
	PORTResetPins(IOPORT_D, -1);
	PORTResetPins(IOPORT_E, -1);
	PORTResetPins(IOPORT_F, -1);
	PORTResetPins(IOPORT_G, -1); */
}


void PINdir(UINT portpin, char pindir)  // PINdir(pLED1|pLED2,0), 0-out, stejny port
{
	if (pindir) {PORTSetPinsDigitalIn((portpin>>16)&0xFFFF, portpin&0xFFFF);}  // 1-in
	       else {PORTSetPinsDigitalOut((portpin>>16)&0xFFFF, portpin&0xFFFF);} // 0-out
}


void PINwrite(UINT portpin, char pinstate)  // PINwrite(pLED1|pLED2,1), 1-H, stejny port
{
	if (pinstate) {PORTSetBits((portpin>>16)&0xFFFF, portpin&0xFFFF);}
	         else {PORTClearBits((portpin>>16)&0xFFFF, portpin&0xFFFF);}
}


WORD PINread(UINT portpin)  //  PINread(pJD_CTS|pD_DTR)!=0, cte port pres masku
{
	UINT pinstate;

	pinstate=PORTReadBits((portpin>>16)&0xFFFF, portpin&0xFFFF);
	return pinstate;
}


// ********** DIOS **********

void fhADCread(void *addr)
{
	*(int *)addr=ADCread();			// TOS=@ADC
}


void fhPINread(void *addr)
{
	int portpin=*(int *)addr;		// portpin=TOS
	*(int *)addr=PINread(portpin);	// TOS=@PIN
}


void fhPINwrite(void *addr)
{
	int pinstate, portpin;

	portpin=*(int *)addr;			// portpin=TOS
	pinstate=*(int *)(addr-4);		// pinstate=NOS
	PINwrite(portpin,pinstate);
}


void fhPINdir(void *addr)
{
	int portpin, pindir;

	portpin=*(int *)addr;			// portpin=TOS
	pindir=*(int *)(addr-4);		// pindir=NOS
	PINdir(portpin,pindir);
}


const void* DevTabADC[] =
{
	0, 0, ADCopen, ADCclose,		// init, release, open, close
	fhADCread, 0, 0, 0				// read, write, ?read, ?write
};


const void* DevTabPIN[] =
{
	0, 0, PINopen, PINclose,		// init, release, open, close
	fhPINread, fhPINwrite, 0, 0,	// read, write, ?read, ?write
	fhPINdir						// direction
};
