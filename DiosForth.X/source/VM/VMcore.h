/*********************************************************************
 *
 *                  VMcore.h
 *                  DIOS Forth
 *
 * v.1.0 - 20.12.2012, L.Pekny
 ********************************************************************/

#ifndef __VMcore_H__
#define __VMcore_H__


#include "..\GenericTypeDefs.h"

#define cell 		int
#define ucell 		unsigned int
#define dcell 		long long
#define udcell 		unsigned long long
#define cellsize	4
#define dcellsize	2*cellsize
#define DSsize 		64
#define RSsize 		64

extern cell  DS[DSsize];
extern ucell RS[RSsize];
extern cell *pDS, *pDSzero;
extern ucell *pRS, *pRSzero, *pRSbak;
//extern WORD DScnt, RScnt, RSCntBak;
extern ucell PC, WORK;
extern void (*pFce)(void);

void exitw(void);
void executew(void);
void nextw(void);
void enterw(void);
void dolitw(void);
void doslitw(void);
void doconw(void);
void dovarw(void);
void dovalw(void);

void dodoes(void);
void dodefer(void);
void dodo(void);
void doisdo(void);
void doloop(void);
void doplusloop(void);
void dobranch(void);
void docbranch(void);

#define PUSH(val)	(*++pDS=val)		// DS[++DScnt]=val
#define POP			(*pDS--)			// DS[DScnt--]
#define TOS			(*pDS)				// DS[DScnt]
#define TOSi(val)	(*(pDS-(val)))		// DS[DScnt-val]
#define PUSHR(val)	(*++pRS=val)		// RS[++RScnt]=val
#define POPR		(*pRS--)			// RS[RScnt--]
#define TOSR		(*pRS)				// RS[RScnt]
#define TOSRi(val)	(*(pRS-(val)))		// RS[RScnt-val]

#define DPUSH(val)	*++pDS=(ucell)(val); *++pDS=(udcell)(val)>>32
#define DPOP(val)	val=(dcell)*pDS--; val<<=32; val|=(ucell)*pDS--

#define DScnt		(((cell)((ucell)pDS-(ucell)pDSzero))>>2)
#define RScnt		(((cell)((ucell)pRS-(ucell)pRSzero))>>2)

#define pDATA 		*(ucell *)
#define EXIT		PC=POPR;
#define EXECUTE		PC=(ucell)&TOS; pFce=(void *)pDATA POP; (*pFce)();
#define NEXT 		PC+= cellsize; pFce=(void *)pDATA(pDATA PC); (*pFce)(); 
#define ENTER 		pRSbak=pRS; PUSHR(PC); PC=pDATA PC; while(pRSbak<pRS){NEXT};
#define ENTERDOES	pRSbak=pRS; PUSHR(PC); PC=pDATA(WORK); while(pRSbak<pRS){NEXT};
#define DOLIT 		PC+=cellsize; PUSH(pDATA PC);
#define DOSLIT 		PC+=cellsize; PUSH(PC+1); PUSH(*(BYTE *)PC); PC+=TOS;
#define DOCON 		PUSH(pDATA(pDATA(PC)+cellsize));
#define DOVAR 		PUSH(pDATA(pDATA(PC)+cellsize));
#define DOVAL 		PUSH(pDATA(pDATA(pDATA(PC)+cellsize)));

#endif		
