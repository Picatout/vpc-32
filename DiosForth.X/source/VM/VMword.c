/*
* Copyright 2013, Jacques Deschênes
* This file is part of VPC-32.
*
*     VPC-32 is free software: you can redistribute it and/or modify
*     it under the terms of the GNU General Public License as published by
*     the Free Software Foundation, either version 3 of the License, or
*     (at your option) any later version.
*
*     VPC-32 is distributed in the hope that it will be useful,
*     but WITHOUT ANY WARRANTY; without even the implied warranty of
*     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*     GNU General Public License for more details.
*
*     You should have received a copy of the GNU General Public License
*     along with VPC-32.  If not, see <http://www.gnu.org/licenses/>.
*/
/*********************************************************************
 *
 *                  VMword
 *                  DIOS Forth
 *
 * v.1.0 - 20.12.2012, L.Pekny
 ********************************************************************/

#include <stdlib.h>
#include <math.h>

#include <plib.h>
#include <errno.h>
#include "../../../hardware/HardwareProfile.h"
#include "..\GenericTypeDefs.h"
//#include "..\HardwareProfile.h"
#ifdef VPC_32
#include "../../../console.h"
#include "../../../sound.h"
#else
#include "..\Uart\UartBuf.h"	// emit ?emit key ?key
#endif
#include "..\NVMem\NVMem.h"		// flash >flash ;flash
#ifndef VPC_32
#include "..\Main.h"			// devhead  devcall
#endif
#include "VMcore.h"
#include "VMword.h"

#ifdef VPC_32
char *vTib=NULL, *vPad=NULL, *vDict=NULL, *vMem=NULL;
#else
char  vTib[tibsize+4], vPad[padsize+4];
char  vDict[dictsize], vMem[memsize];
#endif
//char  *pDict=vDict, *pMem=vMem;
char *vHere, *vHead, *vHeap, *vCurrent, *vContext;
char *vHereBak, *vHeadBak;
BYTE  vIN=0, vSharpTib=0, vBase=16, vState=0, vErrors=0;
WORD  PrimLast=0;
BYTE  AddrRAM, AddrROM;
const char StrVer[]=VerVM;

// forward referenced functions (XC32 complain if prototypes not declared before call)
void abortf(void);
void linkg(void);
void comma(void);
void tick(void);
void crf(void);
void aborts(void);
void udot(void);
void ver(void);
void toflash(void);
void dotstring(void);

// ********** C UTIL **********

void FindLastC(void)  // Find last primitive word
{
	WORD i=0;
	while(primwords[i].wlink!=0xFF) {i++;}
	PrimLast=--i;
}


void EEcold(void)  // Find empty 4kB or find last FHere in EETAB, recovery He..
{
	UINT i, f=1, uFHere;
	UINT *pEETAB;

	i=EETAB; pEETAB=(UINT *)i;		// Cannot make pointer directly from EETAB

	vFHead=0; uFHere=(int)StrVer&~0xFFF;	// Find empty flash 4kB
	while (f)
	{ f=0;
	  uFHere+=FLASH_PAGE_SIZE;
	  for (i=uFHere; i<(uFHere+FLASH_PAGE_SIZE); i+=4)
	  {
		if (*(int *)i!=-1) {f=1; break;}
	  }
	  if (uFHere==EETAB) {f=0;}				// End of flash
	}
	vFHere=(UINT *)uFHere;

	if (*pEETAB!=-1)						// Read last vector from EETAB
	{ f=0;
	  for (i=0; i<FLASH_PAGE_SIZE; i+=4)	// Find last FHere
	  {
		if (*pEETAB==-1) {break;} else {uFHere=*pEETAB; f=1;}
		pEETAB++;
	  }
	  vFHere=(UINT *)uFHere;
	  if (f&&(*(vFHere-1)==(uFHere-4)))		// Recovery Head, Heap
	    {vFHead=(UINT *)(*(vFHere-2)); vHeap=(char *)(*(vFHere-3)); vHead=(char *)vFHead;}
	}
}


void CompileCxt(WORD i)  // compile xt primitive for C
{
	PUSH((ucell)&primwords[i].wcall); comma(); // XT ,
}


void CompileCpfa(WORD i)  // compile pfa primitive for C
{
	PUSH((ucell)primwords[i].wcall); comma();  // PFA ,
}


void CompileCcon(void *i)  // compile const for C
{
	PUSH((ucell)i); comma(); // x ,
}


// ********** STACK **********

// drop ( x --  )
void drop(void) {pDS--;}


// 2drop ( x1 x2 --  )
void twodrop(void) {pDS-=2;}


// dup (x1 -- x1 x1 )
void dup(void) {cell tmp=TOS; PUSH(tmp);}


// 2dup ( x1 x2 -- x1 x2 x1 x2 )
void twodup(void) {cell tmp=TOSi(1); PUSH(tmp); tmp=TOSi(1); PUSH(tmp);}


// ?dup ( x -- 0 | x x )
void isdup(void) {cell tmp=TOS;	if (tmp) {PUSH(tmp);}}


// nip ( x1 x2 -- x2 )
void nip(void) {TOSi(1)=TOS; pDS--;}


// over ( x y -- x y x )
void over(void) {cell tmp=TOSi(1); PUSH(tmp);}


// 2over ( x1 x2 x3 x4 -- x1 x2 x3 x4 x1 x2 )
void twoover(void) {cell tmp=TOSi(3); PUSH(tmp); tmp=TOSi(3); PUSH(tmp);}


// pick ( xu ... x1 x0 u -- xu ... x1 x0 xu )  copy xu
void pick(void) {ucell u=TOS; if (DScnt>u+1) {TOS=TOSi(u+1);} else {vErrors|=4; abortf();}}


// stick ( xu ... x1 x0 xunew u -- xunew ... x1 x0 )  overwrite xu
void stick(void) {cell u=POP, new=POP; if (DScnt>u) {TOSi(u)=new;} else {vErrors|=4; abortf();}}


// roll ( xu ... x1 x0 u -- xu-1... x1 x0 xu )  move xu to TOS
void roll(void)
{
	ucell u=POP, *p=(ucell *)pDSzero;
	cell cnt=DScnt;

	if (!cnt) {return;}
	if (cnt>u)
	{
		int i;
		cell xu=TOSi(u);
		for (i=cnt-u; i<cnt; i++) {*(pDSzero+i)=*(pDSzero+i+1);}
		TOS=xu;
	} else {vErrors|=4; abortf();}
}


// -roll ( xu ... x1 x0 u -- x0 xu... x1 )  insert TOS to xu
void minusroll(void)
{
	ucell u=POP;
	cell cnt=DScnt;

	if (!cnt) {return;}
	if (cnt>u)
	{
		int i;
		cell x0=TOS;
		for (i=cnt; i>cnt-u; i--) {*(pDSzero+i)=*(pDSzero+i-1);}
		TOSi(u)=x0;
	} else {vErrors|=4; abortf();}
}


// rot ( x1 x2 x3 -- x2 x3 x1 )
void rot(void) {cell tmp=TOS; TOS=TOSi(2); TOSi(2)=TOSi(1); TOSi(1)=tmp;}


// -rot ( x1 x2 x3 -- x3 x1 x2 )
void minusrot(void) {cell tmp=TOS; TOS=TOSi(1); TOSi(1)=TOSi(2); TOSi(2)=tmp;}


// swap ( x1 x2 -- x2 x1 )
void swap(void) {cell tmp=TOS; TOS=TOSi(1); TOSi(1)=tmp;}


// 2swap ( x1 x2 x3 x4 -- x3 x4 x1 x2 )
void twoswap(void)
{
	cell tmp=TOS, tmp2=TOSi(1);
	TOS=TOSi(2); TOSi(1)=TOSi(3);
	TOSi(2)=tmp; TOSi(3)=tmp2;
}


// >r ( x -- ) (R: -- x ) 
void tor(void) {PUSHR(POP);}


// r> ( -- x ) (R: x -- )
void rfrom(void) {PUSH(POPR);}


// r@ ( -- x ) (R: x -- x )
void rfetch(void) {PUSH(TOSR);}


// depth (  -- x )
void depth(void) {cell tmp=DScnt; PUSH(tmp);}


// depth! ( ... u --  x1 x2 .. xu )
void depthwrite(void) {ucell tmp=POP; pDS=pDSzero+tmp;}


// rdepth (  -- u )
void rdepth(void) {cell tmp=RScnt; PUSH(tmp);}


// rdepth! ( u --  ) ( R: ... -- x1 x2 .. xu )
void rdepthwrite(void) {ucell tmp=POP; pRS=pRSzero+tmp;}


// sp@ (  -- addr )
void spfetch(void) {ucell tmp=(ucell)&TOS; PUSH(tmp);}


// rp@ (  -- u )
void rpfetch(void) {ucell tmp=(ucell)&TOSR; PUSH(tmp);}


// ********** OTHER **********


// nop ( -- )
void nop(void) { }


// emit ( char --  )
void emit(void)
{
	// while (!isUARTwrite(UARTcon));
#ifdef VPC_32
    put_char(comm_channel,POP);
#else
        if (!isUARTwrite(UARTcon)) {delay_us(20000000/UART[UARTcon].Speed);}
	UARTwrite(UARTcon,POP);
#endif
}

#ifndef VPC_32
// ?emit ( char --  )
void isemit(void) {   
    PUSH((cell) isUARTwrite(UARTcon));
}
#endif


// key (  -- char )
void key(void)
{
#ifdef VPC_32
    PUSH (wait_key(comm_channel));
#else
	while (!isUARTread(UARTcon));
	PUSH(UARTread(UARTcon));
#endif
} 


// ?key (  -- pressed? )
void iskey(void) {
#ifdef VPC_32
    PUSH((cell) get_key(comm_channel));
#else
    PUSH((cell) isUARTread(UARTcon));
#endif
}


// i ( -- i )
void loop_i(void) {PUSH(TOSR);}


// j ( -- j )
void loop_j(void) {PUSH(TOSRi(3));}


// k ( -- k )
void loop_k(void) {PUSH(TOSRi(6));}


// ********** MEMORY **********


// @ ( addr -- x )
void fetch(void) {TOS=pDATA TOS;}


// c@ ( addr -- byte )
void cfetch(void) {TOS=*(BYTE *)TOS;}


// w@ ( addr -- w )
void wfetch(void) {TOS=*(WORD *)TOS;}


// ! ( x addr -- )
void store(void) {ucell addr=POP; pDATA addr=POP;}


// c! ( byte addr -- )
void cstore(void) {ucell addr=POP; *(BYTE *) addr=POP;}


// w! ( w addr -- )
void wstore(void) {ucell addr=POP; *(WORD *) addr=POP;}


// +! ( n addr -- )
void plusstore(void) {ucell addr=POP; pDATA addr+=POP;}


// fill ( addr len c -- )
void fillf(void)  // fill c
{
	ucell value=POP, count=POP, src=POP;
	memset((char *)src, value, count);
}


// move ( src-addr dest-addr len -- )
void movef(void)  // move c
{
	ucell count=POP, dest=POP, src=POP;
	memmove((char *)dest, (char *)src, count);
}


// here ( -- u )
void here(void) {PUSH((ucell)vHere);}


// here! ( u -- )
void herewrite(void) {vHere=(char *)POP;}  // set vHere


// head (  -- u )
void head(void) {PUSH((ucell)vHead);}


// head! ( u -- )
void headwrite(void) {vHead=(char *)POP;}


// heap (  -- u )
void heap(void) {PUSH((ucell)vHeap);}


// heap! ( u -- )
void heapwrite(void) {vHeap=(char *)POP;}


// ********** ARITHMETIC **********


// + ( n1 n2 -- n )
void plus(void) {cell n2=POP; TOS=TOS+n2;}


// - ( n1 n2 -- n )
void minus(void) {cell n2=POP; TOS=TOS-n2;}


// d+ ( d1 d2 -- d )
void dplus(void) 
{
	dcell d1, d2;
	DPOP(d2); DPOP(d1);
	DPUSH(d1+d2);
}


// d- ( d1 d2 -- d )
void dminus(void)
{
	dcell d1, d2;
	DPOP(d2); DPOP(d1);
	DPUSH(d1-d2);
}


// * ( n1 n2 -- n )
void mult(void) {cell n2=POP; TOS=TOS*n2;}


// u* ( u1 u2 -- u ) same as mult
//void umult(void) {ucell u2=POP; TOS=(ucell)TOS*u2;}


// m* ( n1 n2 -- d )
void mmult(void) {cell n2=POP, n1=POP;	DPUSH((dcell)n1*n2);}


// um* ( u1 u2 -- ud )
void ummult(void) {ucell u2=POP, u1=POP; DPUSH((udcell)u1*u2);}


// sqrt ( u1 -- u )
void sqrtu(void) {ucell u=TOS; TOS=sqrtf(u);}


// um/mod ( ud u1 -- rem u )
void umdivmod(void)
{
	udcell ud, u1=POP;
	if (u1==0) {vErrors|=8; abortf();}
	else {DPOP(ud); PUSH(ud%u1); PUSH(ud/u1);}
} 


// m/mod ( d n1 -- rem n )
void mdivmod(void)
{
	dcell d, n1=POP;
	if (n1==0) {vErrors|=8; abortf();}
	else {DPOP(d); PUSH(d%n1); PUSH(d/n1);}
} 


// u/mod ( u1 u2 -- rem u )
void udivmod(void)
{
	ucell u2=TOS, u1=TOSi(1);
	if (u2==0) {vErrors|=8; abortf();}
	else {TOSi(1)=u1%u2; TOS=u1/u2;}
} 


// /mod ( n1 n2 -- rem n )
void divmod(void)
{
	cell n2=TOS, n1=TOSi(1);
	if (n2==0) {vErrors|=8; abortf();}
	else {TOSi(1)=n1%n2; TOS=n1/n2;}
} 


//  / ( n1 n2 -- n )
void divf(void)
{
	cell n2=POP, n1=TOS;
	if (n2==0) {vErrors|=8; abortf();}
	else {TOS=n1/n2;}
} 


// mod ( n1 n2 -- rem )
void modn(void)
{
	cell n2=POP, n1=TOS;
	if (n2==0) {vErrors|=8; abortf();}
	else {TOS=n1%n2;}
} 


// u*/ ( u1 u2 u3 -- u )
void umuldiv(void)
{
	udcell u3=POP, u2=POP, u1=POP;
	if (u3==0) {vErrors|=8; abortf();}
	else {PUSH((u1*u2)/u3);}
}


// */ ( n1 n2 n3 -- n )
void muldiv(void)
{
	dcell n3=POP, n2=POP, n1=POP;
	if (n3==0) {vErrors|=8; abortf();}
	else {PUSH((n1*n2)/n3);}
}


// >>a ( n1 u -- n )
void arshift(void) {ucell u=POP; TOS=(cell)TOS>>u;}


// >> ( x1 u -- x )
void rshift(void) {ucell u=POP; TOS=(ucell)TOS>>u;}


// << ( x1 u -- x )
void lshift(void) {ucell u=POP; TOS=(ucell)TOS<<u;}


// 2* ( n1 -- n )
void twomul(void) {TOS=TOS<<1;}


// 2/ ( n1 -- n )
void twodiv(void) {TOS=TOS>>1;}


// min ( n1 n2 -- n1|n2 )
void minf(void) {cell n2=POP; TOS=(n2<TOS) ? n2:TOS;}


// max ( n1 n2 -- n1|n2 )
void maxf(void) {cell n2=POP; TOS=(n2>TOS) ? n2:TOS;}


// abs ( n -- u )
void absf(void) {TOS=(TOS<0) ? -TOS:TOS;}


// 1+ ( n1 -- n )
void incf(void) {TOS++;} 


// 1- ( n1 -- n )
void decf(void) {TOS--;} 


// negate ( n1 -- n )
void negate(void) {TOS=-TOS;} 


// invert ( n1 -- n )
void invert(void) {TOS=TOS^-1;} 


// and ( x1 x2 -- x )
void andf(void) {cell x2=POP; TOS=TOS&x2;}


// or ( x1 x2 -- x )
void orf(void) {cell x2=POP; TOS=TOS|x2;}


// xor ( x1 x2 -- x )
void xorf(void) {cell x2=POP; TOS=TOS^x2;}


// ********** LOGIC **********


// andl ( x1 x2 -- fl )
void andl(void) {cell x2=POP; TOS=(x2&&TOS) ? -1:0;;}


// orl ( x1 x2 -- fl )
void orl(void) {cell x2=POP; TOS=(x2||TOS) ? -1:0;}


// not ( x1 -- fl )
void notl(void) {TOS=(TOS) ? 0:-1;}


// = ( n1 n2 -- fl )
void equals(void) {cell n2=POP;	TOS=(TOS==n2) ? -1:0;}


// <> ( n1 n2 -- fl )
void notequals(void) {cell n2=POP; TOS=(TOS!=n2) ? -1:0;}


// > ( n1 n2 -- fl )
void greater(void) {cell n2=POP; TOS=(TOS>n2) ? -1:0;}


// < ( n1 n2 -- fl )
void less(void) {cell n2=POP; TOS=(TOS<n2) ? -1:0;}


// >= ( n1 n2 -- fl )
void greaterequals(void) {cell n2=POP; TOS=(TOS>=n2) ? -1:0;}


// <= ( n1 n2 -- fl )
void lessequals(void) {cell n2=POP;	TOS=(TOS<=n2) ? -1:0;}


// 0= ( x1 -- fl )
void zeroequals(void) {TOS=(TOS==0) ? -1:0;}


// 0< ( n1 -- fl )
void zeroless(void) {TOS=(TOS<0) ? -1:0;}


// 0> ( x1 -- fl )
void zerogreater(void) {TOS=(TOS>0) ? -1:0;}


// u> ( u1 u2 -- fl )
void ugreater(void) {ucell u2=POP; TOS=((ucell)TOS>u2) ? -1:0;}


// u< ( u1 u2 -- fl )
void uless(void) {ucell u2=POP;	TOS=((ucell)TOS<u2) ? -1:0;}


// u>= ( u1 u2 -- fl )
void ugreaterequals(void) {ucell u2=POP; TOS=((ucell)TOS>=u2) ? -1:0;}


// u<= ( u1 u2 -- fl )
void ulessequals(void) {ucell u2=POP; TOS=((ucell)TOS<=u2) ? -1:0;}


// d= ( d1 d2 -- fl )
void dequals(void)
{
	dcell d1, d2;
	DPOP(d2); DPOP(d1);
	TOS=(d1==d2) ? -1:0;
}


// d<> ( d1 d2 -- fl )
void dnotequals(void)
{
	dcell d1, d2;
	DPOP(d2); DPOP(d1);
	TOS=(d1!=d2) ? -1:0;
}


// d> ( d1 d2 -- fl )
void dgreater(void)
{
	dcell d1, d2;
	DPOP(d2); DPOP(d1);
	TOS=(d1>d2) ? -1:0;
}


// d< ( d1 d2 -- fl )
void dless(void)
{
	dcell d1, d2;
	DPOP(d2); DPOP(d1);
	TOS=(d1<d2) ? -1:0;
}


// within ( u min max -- fl )
void within(void)  // min <= u <max
{
	ucell u2=POP, u1=POP, u=TOS;
	TOS=((u-u1)<(u2-u1)) ? -1:0;
}


// ********** HIGH FORTH **********

// ********** VARIABLE **********


// tib ( -- addr )
void tib(void) {PUSH((ucell)vTib);}


// #tib ( -- addr )
void sharptib(void) {PUSH((ucell)&vSharpTib);}


// >in ( -- addr )
void gin(void) {PUSH((ucell)&vIN);}


// base ( -- addr )
void base(void)
{PUSH((ucell)&vBase); vBase=(vBase<2) ? 2:vBase; vBase=(vBase>32) ? 32:vBase;}


// state ( -- addr )
void state(void) {PUSH((ucell)&vState);}


// pad ( -- addr )
void pad(void) {PUSH((ucell)vPad);}


// current ( -- addr )
void current(void) {PUSH((ucell)&vCurrent);}


// context ( -- addr )
void context(void) {PUSH((ucell)&vContext);}


// bl ( -- n )
void blf(void) {PUSH(Spc);}


// ********** CTRLFLOW **********


// <mark ( -- addr )
void lmark(void) {here(); TOS-=cellsize;}


// <resolve ( addr -- )
void lresolve(void) {comma();}


// >mark ( -- addr )
void gmark(void) {here(); vHere+=cellsize;}


// >resolve ( addr -- )
void gresolve(void) {here(); TOS-=cellsize; swap(); store();}


// do ( -- addr )
void dof(void) {CompileCxt(iDODO); gmark(); lmark();}


// ?do ( -- addr )
void isdof(void) {CompileCxt(iISDO); gmark(); lmark();}


// loop ( addr -- )
void loop(void) {CompileCxt(iLOOP); lresolve(); gresolve();}


// +loop ( addr -- )
void plusloop(void) {CompileCxt(iPLOOP); lresolve(); gresolve();}


// if ( -- addr )
void iff(void) {CompileCxt(iDOCBR); gmark();}


// then ( addr -- )
void thenf(void) {gresolve();}


// else ( addr1 -- addr2 )
void elsef(void) {CompileCxt(iDOBR); gmark(); swap(); gresolve();}


// begin ( -- addr )
void beginf(void) {lmark();}


// while ( dest -- orig dest )
void whilef(void) {CompileCxt(iDOCBR); gmark(); swap();}


// until ( addr -- )
void untilf(void) {CompileCxt(iDOCBR); lresolve();}


// repeat ( addr1 -- addr2 )
void repeatf(void) {CompileCxt(iDOBR); lresolve(); gresolve();}


// again ( addr -- )
void againf(void) {CompileCxt(iDOBR); lresolve();}


// leave ( -- )
void leavef(void) {pRS-=2; EXIT}


// unloop ( -- )
void unloopf(void) {pRS-=3;}


// recurse ( -- )
void recursef(void) {linkg(); comma();}


// case ( -- 0 )
void casef(void) {PUSH(0);}

const void *xt_over=over;
const void *xt_equal=equals;
const void *xt_drop=drop;
const void *xt_if=iff;
const void *xt_then=thenf;
const void *xt_else=elsef;


// of ( #of -- orig #of+1 / x -- )
void caseof(void)
{
	TOS++; tor();
	CompileCcon(&xt_over);
	CompileCcon(&xt_equal);
	PUSH((ucell)&xt_if); executew();
	CompileCcon(&xt_drop);
	rfrom();
}


// endof ( orig1 #of -- orig2 #of )
void endof(void)
{
	tor(); PUSH((ucell)&xt_else); executew(); rfrom();
}


// endcase ( orig 1..orign #of -- )
void endcase(void)
{
	ucell i, u=POP;
	CompileCcon(&xt_drop);
	if (u>0) {for (i=0; i<u; i++) {PUSH((ucell)&xt_then); executew();}}
}


// abort ( -- )
void abortf(void) {pDS=pDSzero; pRS=pRSzero; vIN=0; vSharpTib=0; vState=0;}
const void *xt_abort=abortf;


// abort" ( x -- )
void aborts(void) {iff(); dotstring(); CompileCcon(&xt_abort); thenf();}


// bye ( -- )
void bye(void) {
#ifdef VPC_32
    free(vPad);
    vPad=NULL;
    free(vTib);
    vTib=NULL;
    free(vDict);
    vDict=NULL;
    free(vMem);
    vMem=NULL;
#endif
    vState=EndState;

}


// ********** Interpreter **********


// [ ( -- )
void lbracket(void) {vState=0;}  // Interprete


// ] ( -- )
void rbracket(void) {vState=1;}  // Compile


// bin ( -- )
void binf(void) {vBase=2;}


// decimal ( -- )
void decimal(void) {vBase=10;}


// hex ( -- )
void hexf(void) {vBase=16;}


cell  KeyBak=0;

// accept ( addr n1 -- n2 )
void accept(void)  // read until A,D,AD,DA
{
	char fRun=1, k, *pTib, maxnum, num=0;
	
	maxnum=POP; pTib=(char *)POP;
	while (fRun)
	{
	  key(); k=POP;
	  if ((k==BackSpc)&&(num>0))  {num--; *--pTib=Spc; PUSH(k); emit();PUSH(Spc); emit();PUSH(k); emit();}
	  if ((k>=Spc)&&(num<maxnum)) {num++; *pTib++=k; PUSH(k); emit();}
	  if ((k==CRA)&&(KeyBak==CRD)) {k=0;}
	  if ((k==CRD)&&(KeyBak==CRA)) {k=0;}
	  if ((k==CRD)||(k==CRA)) {fRun=0; crf();}
	  if (num>=maxnum) {fRun=0;}
	  KeyBak=k;
	}
	PUSH(num); 
}


// refill ( -- f )
void refill(void) {tib(); PUSH(tibsize); accept(); vSharpTib=POP; vIN=0; PUSH(-1);}


// source ( -- addr u )
void source(void) {tib(); PUSH(vSharpTib);}


// count ( addr -- addr+1 u )
void count(void) {dup(); cfetch(); TOSi(1)++;}


// word ( c -- addr )
void wordf(void)
{
	BYTE c=POP, i=1;

	if (vSharpTib)
	{
	  while((vTib[vIN]==c)&&(vIN<vSharpTib)) {vIN++;}  // Skip spaces
	  while((vTib[vIN]!=c)&&(vIN<vSharpTib))           // Until c
	  {
		vPad[i]=vTib[vIN]; i++; vIN++;  // Copy to PAD
	  }
	  if((vTib[vIN]==c)&&(vIN<vSharpTib)) {vIN++;}  // Behind c delimiter 
	}
	vPad[0]=--i;  // Length
	PUSH((ucell)vPad);
}


// parse ( c -- addr u )
void parse(void)
{
	BYTE c=POP, i=0;

	if (vSharpTib)
	{
	  PUSH((ucell)&vTib[vIN]);         // Addr
	  while((vTib[vIN]!=c)&&(vIN<vSharpTib)) {i++; vIN++;}  // Until c
	  PUSH((ucell)i);                  // Length
	  if ((vTib[vIN]==c)&&(vIN<vSharpTib)) {vIN++;}         // Behind c delimiter
	}
}


// number ( addr -- n )
void number(void)
{
	char *pStart, *pEnd;
	ucell x, y=0, len;

	count(); len=POP; pStart=(char *)POP;
	*(pStart+len)=0;  // Write end of number!
	if (*pStart=='-') {pStart++; len--; y=1;}

	base(); drop();
	if (len)
	{
	  x=strtoul(pStart, &pEnd, vBase);
	  if (y) {x=-x;};
	  if (((ucell)pStart+len)==(ucell)pEnd) {PUSH(x);} else {vErrors|=2; abortf();}
	} else {vErrors|=1;}
}


// find ( addr -- addr 0 | xt +-1 )
void find(void)
{
	short int i=PrimLast, j, len;
	char *p1=(char *)POP, *pbak=p1, *p2;
	ucell *Link, Linkbak, k=0;

	len=*p1 & 0x1F;
	if (vHead) {Link=(ucell *)vHead; k=1;}  // Link to new word
	while (k)  // Forth words
	{
	  if (((*Link>>24)&0x1F)==len)   // Compare length
	  {
		  Linkbak=(ucell)Link+cellsize;        // Begin of name
		  j=len; p1=pbak; p2=(char *)Linkbak;
		  while((j>0)&&(*++p1==*p2++)) {j--;}  // Compare text
		  if (!j)
		  {
			Linkbak+=len;
			if (Linkbak&3) {Linkbak=(Linkbak&~3)+cellsize;}		// Align
			PUSH(Linkbak);										// XT
			if ((*Link>>24)&im) {PUSH(1);} else {PUSH(-1);}		// 1 Immed
			k=0; i=-1;											// True
		  }
	  }
	  if (k) {Linkbak=*Link&0x7FFFFF;} else {Linkbak=0;}		// Last,zero?
	  if (!Linkbak) {k=0;}
	  if (k)
	  {
		if (*Link&0x800000) {Linkbak+=AddrRAM<<24;} else {Linkbak+=AddrROM<<24;}
		Link=(ucell *)Linkbak;
	  }
	}

//	if (primwords[i].wlen&0x20)  // Primitives?
//	{
	  while(i>=0)
	  {
		if ((primwords[i].wlen&0x1F)==len)     // Compare length
		{
		  j=len; p1=pbak; p2=primwords[i].wname;
		  while((j>0)&&(*++p1==*p2++)) {j--;}  // Compare text
		  if (!j)
		  {
			PUSH((ucell)&primwords[i].wcall);  // XT
			if (primwords[i].wlen&im) {PUSH(1);} else {PUSH(-1);}
			i=-1;
		  }
		}
		if (i==0) {PUSH((ucell)pbak); PUSH(0);} // False
		i--; 
	  }
//	}
}


// ********** Compiler **********


// link> ( -- xt )
void linkg(void)
{
	ucell Linkbak=cellsize+(ucell)vHead;
	Linkbak+=(*(ucell *)vHead>>24)&0x1F;			// Head+len
	if (Linkbak&3) {Linkbak=(Linkbak&~3)+cellsize;}	// Align
	PUSH(Linkbak);  // lfa->cfa
}


// >body ( xt -- addr )
void gbody(void) {TOS+=cellsize;}


// ascii ( -- c )
void ascii(void)
{
	blf(); wordf(); TOS++; TOS=*(BYTE *)TOS;	// c to stack
	if (vState) {CompileCxt(iDOLIT); comma();}	// c to Here
}


// align ( -- )
void align(void)
{
	if ((ucell)vHere&3)	{vHere=(char *)(((ucell)vHere&~3)+cellsize);}
}


// allot ( n -- )
void allot(void) {vHeap+=POP;}


// , ( x -- )
void comma(void)
{
//	*(ucell *)vHere=POP; vHere+=cellsize;
	ucell x=POP;
	if ((ucell)vHere>>24==AddrRAM) {*(ucell *)vHere=x; vHere+=cellsize;}
	else {
	  if (NVMwriteBuf(x)) {vHere+=cellsize;}
	  else {vErrors|=0x10; abortf();}
	}
}


// c, ( c -- )
void ccomma(void)
{
//	*vHere++=POP;
	char c=POP;
	if ((ucell)vHere>>24==AddrRAM) {*vHere++=c;}
	else { 
	  if (NVMwriteBufc(c)) {vHere++;}
	  else {vErrors|=0x10; abortf();}
	}
}


// w, ( x -- )
void wcomma(void)
{
//	*(WORD *)vHere=POP; vHere+=2;
	WORD w=POP;
	if ((ucell)vHere>>24==AddrRAM) {*(WORD *)vHere=w; vHere+=2;}
	else {
	  if (NVMwriteBufw(w)) {vHere+=2;}
	  else {vErrors|=0x10; abortf();}
	}
}


// s, ( addr u -- )
void scomma(void)
{
	BYTE len=POP;
	char *addr1=(char *)POP;
	PUSH(len); ccomma();
	while (len>0) {PUSH(*addr1++); ccomma(); len--;}
}


// compile ( -- )
void compile(void)
{
	PC+=cellsize; PUSH(PC); fetch(); comma(); 
}
const void *xt_compile=compile;


// [compile] ( -- )
void bracketcompile(void)
{
	tick(); comma();
}


// ' ( -- xt )
void tick(void)
{
	blf(); wordf(); find();
	if (!POP) {vErrors|=0x20; abortf();}
}


// ['] ( -- )
void brackettick(void)
{
	CompileCxt(iDOLIT); tick(); comma();
}


// postpone ( -- )
void postpone(void)
{
	blf(); wordf(); find(); 
	if ((cell)TOS<0) {pDS--; CompileCcon(&xt_compile); comma(); return;} // noimmed
	if ((cell)TOS)   {pDS--; comma(); return;}  // [compile] immed
	pDS-=2; vErrors|=0x20; abortf();
}


// (create) ( name -- )
void docreate(void)						// |Flg+Len|Link|Name Align|
{
	char *bakH;
	BYTE Len;
	ucell Link;

	blf(); wordf(); count(); Len=TOS;  // ( -- addr n )
	if (Len>0)
	{
	  bakH=vHead; vHead=vHere;			// Update Head
	  PUSH((ucell)bakH); wcomma();		// Link low 16b
	  Link=((ucell)bakH>>16)&0x7F;		// Link hi 23b 
	  Link|=(((ucell)bakH>>24)==AddrROM) ? 0:0x80;  // ROM/RAM
	  PUSH(Link); ccomma(); scomma();	// Link hi, Length+Name
	  align();							// 4B align
	  if ((ucell)vHere>>24==AddrROM) {toflash();}  // Write to flash
	} else {twodrop();}
}


// create ( name -- )
void createf(void)						// |Flg+Len|Link|Name Align|(con)|PFA|
{
	docreate(); CompileCpfa(iDOCON);	// comma PFA docon
	PUSH((ucell)vHere+cellsize); comma(); // comma PFA new word
}


// <builds ( name -- )
void builds(void)						// |Flg+Len|Link|Name Align|-1|-1|
{
	docreate();
	vHere+=cellsize; vHere+=cellsize;	// Cells for Flash overwrite
}


// does> ( -- )
void does(void)
{
	char *bak=vHere;
	linkg(); herewrite(); CompileCpfa(iDODOES);	// Overwrite xt by dodoes
	PUSH(PC); comma();							// Overwrite xt+1 by PC does>
	vHere=bak; EXIT
}


// : ( -- )
void colon(void)
{
	docreate(); CompileCpfa(iENTER); /* PFA docolon */ rbracket();
}


// ; ( -- )
void semicolon(void)
{
	CompileCxt(iEXIT); /* doexit */ lbracket();
}


// immediate ( -- )
void immediate(void)
{
	char *pLen=vHead+3;
	*pLen|=im;
}


// s" ( -- addr n)
void squote(void)
{
	PUSH('"'); parse();
	if (vState) {CompileCxt(iDOSLIT); scomma(); align();}
}


// literal ( x -- )
void literal(void)
{
	CompileCxt(iDOLIT); /* doliteral */ comma();
}


// constant ( x -- )
void constant(void)
{
	docreate(); CompileCpfa(iDOCON); /* PFA doconstant */ comma();
}


// variable ( -- ) 
void variable(void)
{
	docreate(); CompileCpfa(iDOVAR); /* PFA dovariable */
	heap(); comma(); vHeap+=cellsize;
}


// value ( x name -- ) 
void value(void)
{
	docreate(); CompileCpfa(iDOVAL); /* PFA dovalue */
	pDATA vHeap=POP;  // value x
	heap(); comma(); vHeap+=cellsize;
}


// (to) ( x -- )
void dotof(void)
{
	PC+=cellsize; pDATA PC=POP;
}
const void *xt_doto=dotof;


// to ( x name -- ) 
void tof(void)
{
	tick(); TOS+=cellsize; fetch();
	if(vState) {CompileCcon(&xt_doto); comma();} else {store();}
}


// defer ( name -- )
void defer(void)
{
	docreate(); CompileCpfa(iDODEF); /* PFA dodefer */
	heap(); comma(); vHeap+=cellsize;
}


// defer@ ( xt1 -- xt2 )
void deferfetch(void)
{
	TOS+=cellsize; TOS=pDATA(pDATA(TOS));
}


// defer! ( xt2 xt1 -- )
void deferstore(void)
{
	ucell x1=POP+cellsize;
	pDATA(pDATA(x1))=POP;
}


// interpret ( ? -- ? )
void interpret(void)
{
	char fRun=1;
	
	vErrors=0;
	while (fRun)
	{	
	  blf(); wordf(); dup();
	  cfetch();
	  if (!POP) {drop(); vErrors|=1;}  // Empty TIB
	  else
	  {
		find();	isdup();
		zeroequals();
		if (POP) {number(); if (vState) {CompileCxt(iDOLIT); comma();}}  // compile number
		else
		{
		  zerogreater();
		  if (POP) {executew();}  // immediate
		  else
		  {
			if (vState) {comma();} else {executew();}  // compile/interpreter
		  }
		}
	  }
	  if (vErrors) {fRun=0;}
	  if (vState==EndState) {fRun=0;}
	}
}


// quit ( -- )
void quit(void)
{
	pDS=pDSzero; pRS=pRSzero; lbracket();
	do {
	  if (!vState) {crf(); PUSH('>'); emit(); blf(); emit();}
	  refill(); POP;
	  vErrors=0; interpret();
	  if (vErrors<=1) {blf(); emit(); PUSH('o'); emit(); PUSH('k'); emit();}
	  else {blf(); emit(); PUSH('?'); emit(); PUSH('?'); emit(); PUSH(vErrors); udot();}
	} while (vState!=EndState);
}


// cold ( -- )
void cold(void)
{

      // Default init
#ifdef VPC_32
    char error=0;
    if (vPad==NULL){
        vPad=malloc(padsize*sizeof(char));
        if (!vPad) error=1;
    }
    if (vTib==NULL){
        vTib=malloc(padsize*sizeof(char));
        if (!vTib) error=1;
    }
    if (vDict!=NULL){free(vDict);}
    vDict=malloc(dictsize*sizeof(char));
    if (!vDict) error=1;
    if (vMem!=NULL){free(vMem);}
    vMem=malloc(memsize*sizeof(char));
    if (!vMem) error=1;
    if (error){
        print(comm_channel,"not enough memory\r");
        return;
    }
#endif
	AddrRAM=(ucell)vDict>>24; AddrROM=(ucell)primwords>>24;
	pDS=pDSzero; pRS=pRSzero; 
	vIN=0; vSharpTib=0;
#ifdef VPC_32
        vBase=10;
#else
        vBase=16;
#endif
        vState=0; vErrors=0;
	FindLastC(); vHead=0; vHere=vDict; vHeap=vMem;
	// From flash init
	EEcold();
	ver();
	quit();
}


// ********** EMIT **********


// cr ( -- )
void crf(void) {PUSH(CRD); emit(); PUSH(CRA); emit();}


// space ( -- )
void spacef(void) {blf(); emit();}


// spaces ( n -- )
void spaces(void) {cell i, n=POP; if (n>0) {for(i=0; i<n; i++) {blf(); emit();}}}


// type ( addr u -- )
void typef(void)
{
	ucell i, u=POP;
	char *p=(char *)(ucell)POP;

	if (u>0) {for(i=0; i<u; i++) {PUSH(*p++); emit();}}
}
const void *xt_type=typef;


// ." ( -- )
void dotstring(void) {squote(); if (vState) {CompileCcon(&xt_type);} else {typef();}}


// .( ( -- )
void dotlparen(void) {PUSH(')'); parse(); typef();}


// ( ( -- )
void lparen(void) {PUSH(')'); parse(); twodrop();}


// hold ( c -- )
void hold(void)
{
	cell c=POP, len=vPad[0], i;
	char *p1, *p2;

	if (len<padsize-1)
	{
	  len++; vPad[0]=len; p1=vPad+len; p2=p1++;
	  for (i=0; i<len; i++) {*p1--=*p2--;}
	  //memmove(&vPad[1], &vPad[2], vPad[0]);
	}
	vPad[1]=c;
}


// <# ( -- )
void sharpl(void) {vPad[0]=0;}


// # ( x1 -- x2 )
void sharp(void)
{
	cell x;
	base(); cfetch(); udivmod(); swap(); x=POP;
	if (x>9) {x+=7;}
	PUSH(x+0x30); hold();
}


// #s ( x -- 0 )
void sharps(void) {do{sharp();} while (TOS!=0);}


// #> ( x -- addr len )
void sharpg(void) {TOS=(ucell)vPad; count();}


// u.r ( u n -- )
void udotr(void)
{
	cell n=POP, len;
	sharpl(); sharps(); sharpg(); len=TOS; // ( u -- addr len )
	PUSH(n-len); spaces(); typef();
}


// u. ( u -- )
void udot(void) {PUSH(0); udotr(); blf(); emit();}


// .r ( n1 n2 -- )
void dotr(void)
{
	cell n2=POP, n1=TOS, len;
	absf(); sharpl(); sharps(); if (n1<0) {PUSH('-'); hold();}
	sharpg(); len=TOS; // ( u -- addr len )
	PUSH(n2-len); spaces(); typef();
}


// . ( n -- )
void dot(void) {PUSH(0); dotr(); blf(); emit();}


// \ ( -- )
void backslash(void) {vIN=vSharpTib;}


// dump ( addr u -- )
void dump(void)
{
	ucell i, u=POP;
	ucell *p=(ucell *)POP;

	if (u>0) {for(i=0; i<u; i++)
	{
	  PUSH(*p++);
	  if (vBase==10) {dot();} else {udot();}
	  blf(); emit();
	}}
	crf();
}


// .s ( -- )
void dots(void)
{
	cell i, n=DScnt;
	ucell *p=(ucell *)pDSzero;

	if (n>0) {for(i=0; i<n; i++)
	{
	  PUSH(*++p);
	  if (vBase==10) {dot();} else {udot();}
	  blf(); emit();
	}}
	crf();
}


// ver ( -- )
void ver(void) {
#ifdef VPC_32
    print(comm_channel,StrVer);
    crf();
#else
    UARTwriteS(UARTcon,StrVer); crf();
#endif
}


// ********** VOCABULARY **********


// words ( -- )
void wordsf(void)
{
	short int i=PrimLast, j, len;
	char  *p;
	ucell *Link, Linkbak, k=0;

	if (vHead) {Link=(ucell *)vHead; k=1;}	// Link to last word
	while (k)								// Forth words
	{
	  len=(short int)(*Link>>24)&0x1F ;
	  if (len)								// Length?
	  {
		  Linkbak=(ucell)Link+cellsize;		// Begin of name
		  j=len; p=(char *)Linkbak;
		  for(j=0; j<len; j++) {PUSH(*p++); emit();}
		  blf(); emit();
	  }
	  if (k) {Linkbak=*Link&0x7FFFFF;} else {Linkbak=0;}	// Last,zero?
	  if (!Linkbak) {k=0;}
	  if (k)
	  {
		if (*Link&0x800000) {Linkbak+=AddrRAM<<24;} else {Linkbak+=AddrROM<<24;}
		Link=(ucell *)Linkbak;
	  }
	}

	if (primwords[i].wlen&pr)  // Primitives?
	{
	 while(i>=0)
	 {
	  if (primwords[i].wlen&0x1F)  // Length?
	  {
#ifdef VPC_32
                print(comm_channel,primwords[i].wname);
#else
		UARTwriteS(UARTcon,primwords[i].wname);
#endif
		blf(); emit();
	  }
	 i--;
	 }
	} crf();
}


// ********** DEVICE **********


// coretim ( -- u )
void coretim(void) {PUSH(ReadCoreTimer());}

// msectim ( -- u)  milliseconds timer
void msectim(void) {PUSH(ticks());}

// pause ( u -- )
void pause(void) {delay_us(POP*1000);}


// flash ( -- )
void flash(void)
{
	NVMopen(vFHere);
	vHereBak=vHere; vHere=(char*)vFHere;	// Swap RAM/ROM
	vHeadBak=vHead; vHead=(char*)vFHead;
}


// >flash ( -- )
void toflash(void)
{
	if (!NVMflushBuf()) {vErrors|=0x10; abortf();}	// Copy Buf to flash
}


// ;flash ( -- )
void endflash(void)
{
	ucell Bak;

	PUSH((ucell)vHeap); comma();
	PUSH((ucell)vHead); comma();
	PUSH((ucell)vHere); comma();
	vFHead=(ucell*)vHead; vHead=vHeadBak;		// Swap RAM/ROM
	vHere=vHereBak;	
	
	Bak=*(ucell *)vDict&0xFF000000;				// Update link RAM to FHead
	Bak|=((ucell)vFHead&0x7FFFFF); *(ucell *)vDict=Bak;
	if (!(ucell)vHead) {vHead=(char*)vFHead;};	// Head=0?, Head=FHead;

	NVMclose();
}

#ifndef VPC_32
// devhead (  -- u )
void devhead(void) {PUSH((ucell)DevHead);}


// devcall ( u1 u2 -- )		// u1-ptr DevTabxxx, u2-index fce
void devcall(void)			// 0 ADC 4 devcall (readADC)
{
	ucell index=POP;
	void (*pFce)(void *)=(void *)*(ucell *)((index<<2)+POP);
	(*pFce)((void *)pDS);
}
#endif

// c call ( u1 u2 -- )  	// u1-ptr data, u2-ptr C function
void ccall(void)			// 0 sp@ ADC 10 + @ ccall (readADC)
{
	void (*pFce)(void *)=(void *)POP;
	(*pFce)((void *)POP);
}
#ifdef VPC_32
// sound tone ( duration freq -- )
void beep(void){
    tone(POP,POP);
} //tone()

// joue une mélodie
void play_tune(void){
    tune((unsigned int *)POP);
}//play_tune


#endif

#ifndef VPC_32
// device TIM1 ( -- addr )
void devTIM1(void) {PUSH((ucell)DevTab[0].dcall);}

// device COM ( -- addr )
void devCOM(void) {PUSH((ucell)DevTab[1].dcall);}

// device PIN ( -- addr )
void devPIN(void) {PUSH((ucell)DevTab[2].dcall);}

// device ADC ( -- addr )
void devADC(void) {PUSH((ucell)DevTab[3].dcall);}

// device PWM ( -- addr )
void devPWM(void) {PUSH((ucell)DevTab[4].dcall);}
#endif

// ********** DICTIONARY **********

const PRIMWORD primwords[] =
{	// VMcore
	{0,pr|6,"(exit)",exitw}, {1,pr|7,"execute",executew}, {2,pr|0,"",nextw}, {3,pr|7,"(colon)",enterw},
	{4,pr|5,"(lit)",dolitw}, {5,pr|6,"(slit)",doslitw}, {6,pr|5,"(con)",doconw}, {7,pr|5,"(var)",dovarw},
	{8,pr|6,"(does)",dodoes}, {9,pr|7,"(defer)",dodefer}, {10,pr|4,"(do)",dodo}, {11,pr|5,"(?do)",doisdo},
	{12,pr|6,"(loop)",doloop}, {13,pr|7,"(+loop)",doplusloop}, {14,pr|8,"(branch)",dobranch}, {15,pr|9,"(?branch)",docbranch},
	{16,pr|5,"(val)",dovalw},
	// Stack
	{1,pr|4,"drop",drop}, {2,pr|5,"2drop",twodrop}, {3,pr|3,"dup",dup}, {4,pr|4,"2dup",twodup},
	{5,pr|4,"?dup",isdup}, {6,pr|3,"nip",nip}, {7,pr|4,"over",over}, {8,pr|5,"2over",twoover},
	{9,pr|4,"pick",pick}, {10,pr|5,"stick",stick}, {11,pr|4,"roll",roll}, {12,pr|5,"-roll",minusroll},
	{13,pr|3,"rot",rot}, {14,pr|4,"-rot",minusrot}, {15,pr|4,"swap",swap}, {16,pr|5,"2swap",twoswap},
	{17,pr|2,">r",tor}, {18,pr|2,"r>",rfrom}, {19,pr|2,"r@",rfetch}, {20,pr|5,"depth",depth},
	{21,pr|6,"depth!",depthwrite}, {22,pr|6,"rdepth",rdepth}, {23,pr|7,"rdepth!",rdepthwrite},
	{24,pr|3,"sp@",spfetch}, {25,pr|3,"rp@",rpfetch},
	// Other
	{1,pr|4,"emit",emit},
#ifndef VPC_32
        {2,pr|5,"?emit",isemit},
#endif
        {3,pr|3,"key",key}, {4,pr|4,"?key",iskey},
	{5,pr|1,"i",loop_i}, {6,pr|1,"j",loop_j},  {7,pr|1,"k",loop_k}, {8,pr|3,"nop",nop},
 	// Memory
	{1,pr|1,"@",fetch}, {2,pr|2,"c@",cfetch}, {3,pr|2,"w@",wfetch}, {4,pr|1,"!",store},
	{5,pr|2,"c!",cstore}, {6,pr|2,"w!",wstore}, {7,pr|2,"+!",plusstore},
	{8,pr|4,"fill",fillf}, {9,pr|4,"move",movef}, {10,pr|4,"here",here}, {11,pr|5,"here!",herewrite},
	{12,pr|4,"head",head}, {13,pr|5,"head!",headwrite}, {14,pr|4,"heap",heap}, {15,pr|5,"heap!",heapwrite},
	// Arithmetic
	{1,pr|1,"+",plus}, {2,pr|1,"-",minus}, {3,pr|2,"d+",dplus}, {4,pr|2,"d-",dminus},
	{5,pr|1,"*",mult}, {6,pr|2,"m*",mmult}, {7,pr|3,"um*",ummult}, {8,pr|4,"sqrt",sqrtu},
	{9,pr|6,"um/mod",umdivmod}, {10,pr|5,"m/mod",mdivmod}, {11,pr|5,"u/mod",udivmod}, {12,pr|4,"/mod",divmod},
	{13,pr|1,"/",divf}, {14,pr|3,"mod",modn}, {15,pr|3,"u*/",umuldiv}, {16,pr|2,"*/",muldiv},
	{17,pr|3,">>a",arshift}, {18,pr|2,">>",rshift}, {19,pr|2,"<<",lshift}, {20,pr|2,"2*",twomul},
	{21,pr|2,"2/",twodiv}, {22,pr|3,"min",minf}, {23,pr|3,"max",maxf}, {24,pr|3,"abs",absf},
	{25,pr|2,"1+",incf}, {26,pr|2,"1-",decf}, {27,pr|6,"negate",negate}, {28,pr|6,"invert",invert},
	{29,pr|3,"and",andf}, {30,pr|2,"or",orf}, {31,pr|3,"xor",xorf},
	// Logic
	{1,pr|4,"andl",andl}, {2,pr|3,"orl",orl}, {3,pr|3,"not",notl}, {4,pr|1,"=",equals},
	{5,pr|2,"<>",notequals}, {6,pr|1,">",greater}, {7,pr|1,"<",less}, {8,pr|2,">=",greaterequals},
	{9,pr|2,"<=",lessequals}, {10,pr|2,"0=",zeroequals}, {11,pr|2,"0<",zeroless}, {12,pr|2,"0>",zerogreater},
	{13,pr|2,"u>",ugreater}, {14,pr|2,"u<",uless}, {15,pr|3,"u>=",ugreaterequals}, {16,pr|3,"u<=",ulessequals},
	{17,pr|2,"d=",dequals}, {18,pr|3,"d<>",dnotequals}, {19,pr|2,"d>",dgreater}, {20,pr|2,"d<",dless},
	{21,pr|6,"within",within},

	// High FORTH
	// Variable
	{1,pr|2,"bl",blf}, {2,pr|4,"base",base}, {3,pr|3,">in",gin}, {4,pr|3,"pad",pad},
	{5,pr|5,"state",state}, {6,pr|3,"tib",tib}, {7,pr|4,"#tib",sharptib},
//	{8,pr|7,"current",current}, {9,pr|7,"context",context},
	// CtrlFlow
	{1,pr|5,"<mark",lmark}, {2,pr|8,"<resolve",lresolve}, {3,pr|5,">mark",gmark}, {4,pr|8,">resolve",gresolve},
	{5,pr|im|2,"do",dof}, {6,pr|im|3,"?do",isdof}, {7,pr|im|4,"loop",loop}, {8,pr|im|5,"+loop",plusloop},
	{9,pr|im|2,"if",iff}, {10,pr|im|4,"then",thenf}, {11,pr|im|4,"else",elsef}, {12,pr|im|5,"begin",beginf},
	{13,pr|im|5,"while",whilef}, {14,pr|im|5,"until",untilf}, {15,pr|im|6,"repeat",repeatf}, {16,pr|im|5,"again",againf},
	{17,pr|4,"exit",exitw}, {18,pr|im|7,"recurse",recursef}, {19,pr|5,"leave",leavef}, {20,pr|6,"unloop",unloopf},
	{21,pr|im|4,"case",casef}, {22,pr|im|2,"of",caseof}, {23,pr|im|5,"endof",endof}, {24,pr|im|7,"endcase",endcase},
	{25,pr|5,"abort",abortf}, {26,pr|im|6,"abort\"",aborts}, {27,pr|3,"bye",bye},
	// Interpreter
	{1,pr|im|1,"[",lbracket}, {2,pr|1,"]",rbracket}, {3,pr|3,"bin",binf}, {4,pr|7,"decimal",decimal},
	{5,pr|3,"hex",hexf}, {6,pr|6,"accept",accept}, {7,pr|5,"count",count}, {8,pr|4,"find",find},
	{9,pr|6,"number",number}, {10,pr|5,"parse",parse}, {11,pr|4,"word",wordf}, {12,pr|9,"interpret",interpret},
	{13,pr|6,"refill",refill}, {14,pr|6,"source",source}, {15,pr|4,"quit",quit}, {16,pr|4,"cold",cold},
	// Compiler	
	{1,pr|5,">body",gbody}, {2,pr|5,"link>",linkg}, {3,pr|5,"align",align}, {4,pr|5,"allot",allot},
	{5,pr|im|5,"ascii",ascii}, {6,pr|1,",",comma}, {7,pr|2,"c,",ccomma}, {8,pr|2,"w,",wcomma},
	{9,pr|2,"s,",scomma}, {10,pr|1,"'",tick}, {11,pr|im|3,"[']",brackettick}, {12,pr|7,"compile",compile},
 	{13,pr|im|9,"[compile]",bracketcompile}, {14,pr|8,"(create)",docreate}, {15,pr|6,"create",createf}, {16,pr|7,"<builds",builds},
	{17,pr|5,"does>",does}, {18,pr|9,"immediate",immediate}, {19,pr|im|8,"postpone",postpone}, {20,pr|1,":",colon},
	{21,pr|im|1,";",semicolon}, {22,pr|im|2,"s\"",squote}, {23,pr|im|7,"literal",literal}, {24,pr|8,"constant",constant},
	{25,pr|8,"variable",variable}, {26,pr|5,"value",value}, {27,pr|4,"(to)",tof}, {28,pr|im|2,"to",tof},
	{29,pr|5,"defer",defer}, {30,pr|6,"defer@",deferfetch}, {31,pr|6,"defer!",deferstore},
 	// Emit
	{1,pr|2,"cr",crf}, {2,pr|5,"space",spacef}, {3,pr|6,"spaces",spaces}, {4,pr|4,"type",typef},
	{5,pr|1,".",dot}, {6,pr|2,"u.",udot}, {7,pr|2,".r",dotr}, {8,pr|3,"u.r",udotr},
	{9,pr|im|2,".\"",dotstring}, {10,pr|im|2,".(",dotlparen}, {11,pr|im|1,"(",lparen}, {12,pr|4,"hold",hold},
	{13,pr|2,"<#",sharpl}, {14,pr|1,"#",sharp}, {15,pr|2,"#s",sharps}, {16,pr|2,"#>",sharpg},
	{17,pr|im|1,"\\",backslash}, {18,pr|4,"dump",dump}, {19,pr|2,".s",dots}, {20,pr|3,"ver",ver},
	// Vocabulary
//	{1,pr|4,"also",nop}, {2,pr|11,"definitions",nop}, {3,pr|6,"forget",nop}, {4,pr|5,"forth",nop},
//	{5,pr|4,"only",nop}, {6,pr|5,"order",nop}, {7,pr|10,"vocabulary",nop},
	{8,pr|5,"words",wordsf},
	// Device
	{1,pr|7,"coretim",coretim}, {2,pr|2,"pause",pause}, {3,pr|5,"flash",flash}, {4,pr|6,">flash",toflash},
        {5,pr|6,";flash",endflash},
#ifndef VPC_32    
        {6,pr|7,"devhead",devhead}, {7,pr|7,"devcall",devcall},
#endif
        {8,pr|5,"ccall",ccall},
#ifndef VPC_32
	{9,pr|4,"TIM1",devTIM1}, {10,pr|3,"COM",devCOM}, {11,pr|3,"PIN",devPIN}, {12,pr|3,"ADC",devADC},
	{13,pr|3,"PWM",devPWM},
#endif
#ifdef VPC_32
        {9,pr|4,"beep",beep},
        {10,pr|4,"tune",play_tune},
        {11,pr|7,"msectim",msectim},
#endif
	{0xFF,pr,"",nop}

};


//		VMCORE
//	(exit)  execute  ""  (colon)  (lit)  (slit)  (con)  (var)
//	(does)  (defer)  (do)  (?do)  (loop)  (+loop)  (branch)  (?branch)
//	(val)
//		STACK
//	drop  2drop  dup  2dup  ?dup  nip  over  2over
//	pick  stick  roll  -roll  rot  -rot  swap  2swap
//	>r  r>  r@  depth  depth!  rdepth  rdepth!  sp@  rp@
//		OTHER
//	emit  ?emit  key  ?key  i  j  k  nop 
//		MEMORY
//	@  c@  w@  !  c!  w!  +!
//	fill  move  here  here!  head  head!  heap  heap! 
//		ARITHMETIC
//	+  -  d+  d-  *  m*  um*  sqrt
//	um/mod   m/mod  u/mod  /mod  /  mod  u*/  */
//	>>a  >>  <<  2*  2/  min  max  abs
//	1+  1-  negate  invert  and  or  xor   
//		LOGIC
//	andl  orl  not
//	=  <>  >  <  >=  <=  0=  0<  0>
//	u>  u<  u>=  u<=  d=  d<>  d>  d<
//	within
//
//		High FORTH
//		VARIABLE
//	bl  base  >in  pad  state  tib  #tib
//	current  context
//		CTRLFLOW
//	<mark  <resolve  >mark  >resolve  do  ?do  loop  +loop  
//	if  then  else  begin  while  until  repeat  again
//	exit  recurse  leave  unloop  case  of  endof  endcase
//	abort  abort"  bye
//		INTERPRETER
//	[  ]  bin  decimal  hex  accept  count  find
//	number  parse  word  interpret  refill  source  quit  cold  
//		COMPILER
//	>body  link>  align  allot  ascii  ,  c,  w,
//	s,  '  [']  compile  [compile]  (create)  create  <builds
//	does>  immediate  postpone  :  ;  s"  literal  constant
//	variable  value  (to)  to  defer  defer@  defer!
//		EMIT
//	cr  space  spaces  type  .  u.  .r  u.r
//	."  .(  (  hold  <#  #  #s  #>
//	\  dump  .s  ver
//		VOCABULARY
//	also  definitions  forget  forth  only  order  vocabulary  words
//		DEVICE
//	coretim  ms  flash  >flash  ;flash  devhead  devcall  ccall
//	TIM1  COM  PIN  ADC  PWM

