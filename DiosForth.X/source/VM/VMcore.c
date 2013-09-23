/*********************************************************************
 *
 *                  VMcore
 *                  DIOS Forth
 *
 * v.1.0 - 20.12.2012, L.Pekny
 ********************************************************************/

#include <plib.h>
#include "..\GenericTypeDefs.h"
#include "../../../hardware/HardwareProfile.h"
//#include "..\HardwareProfile.h"
#include "VMcore.h"


cell  DS[DSsize];
ucell RS[RSsize];
cell  *pDSzero=DS+8, *pDS=DS+8; 
ucell *pRSzero=RS+8, *pRS=RS+8, *pRSbak;
//WORD DScnt, RScnt;
ucell PC, WORK;
void (*pFce)(void) = NULL;


void exitw(void)  {EXIT}

void executew(void) {EXECUTE}

void nextw(void)  {NEXT}

void enterw(void) {ucell *pRSbak; ENTER}

void dolitw(void) {DOLIT}

void doslitw(void){DOSLIT  PC&=~3;}  // align (PC-cellsize)

void doconw(void) {DOCON}

void dovarw(void) {DOVAR}

void dovalw(void) {DOVAL}


void dodoes(void)
{
	WORK=pDATA(PC);	PUSH(WORK+dcellsize);	// PFA
	WORK+=cellsize;				// Must be used WORK
	ucell *pRSbak; ENTERDOES	// Words behind does>
}


void dodefer(void)
{
	DOVAL EXECUTE
}


void dodo(void)
{
	cell vStart=POP, vEnd=POP;
	PC+=cellsize;
	PUSHR(pDATA PC);  // For leave
	PUSHR(vEnd); PUSHR(vStart);
}


void doisdo(void)
{
	cell vStart=POP, vEnd=POP;
	PC+=cellsize;
	if (vStart==vEnd) {PC=pDATA PC;}
	 else {PUSHR(pDATA PC); PUSHR(vEnd); PUSHR(vStart);}
}


void doloop(void)
{
	cell vStart=POPR, vEnd=POPR; 
	PC+=cellsize; vStart++;

	if (vStart<vEnd)
	{
		PC=pDATA PC;
		PUSHR(vEnd); PUSHR(vStart);
	}
	else {pRS--;}  // POPR or leave
}


void doplusloop(void)
{
	cell vInc=POP, vStart=POPR, vEnd=POPR;
	char k; 

	PC+=cellsize; vStart+=vInc;
	if (vInc>=0) {k=(char)(vStart<vEnd);} else {k=(char)(vStart>vEnd);}
	if (k)
	{
		PC=pDATA PC;
		PUSHR(vEnd); PUSHR(vStart);
	}
	else {pRS--;}  // POPR or leave
}


void dobranch(void)
{
	PC+=cellsize;
	PC=pDATA PC;
}


void docbranch(void)
{
	PC+=cellsize;
	if (!POP) {PC=pDATA PC;}
}
