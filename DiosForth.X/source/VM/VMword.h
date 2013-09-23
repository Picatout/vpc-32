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
 *                  VMword.h
 *                  DIOS Forth
 *
 * v.1.0 - 20.12.2012, L.Pekny  Copyright 2012
 ********************************************************************/

#ifndef __VMword_H__
#define __VMword_H__

#include "../../../hardware/HardwareProfile.h"
#include "..\GenericTypeDefs.h"

typedef struct
{
	WORD wlink;		// Link
	WORD wlen;		// Flg+length
	char *wname;	// Name
	void *wcall;	// Call fce
} PRIMWORD;


const PRIMWORD primwords[];



#ifdef VPC_32
#define VerVM		"DIOS Forth 1.0      (c)2012 L.P.\rPorted to VPC-32 by J.D., (c)2013"
#define memsize		0x1000	// 4kB ram
#else
#define VerVM		"DIOS Forth 1.0      (c)2012 L.P."
#define memsize		0x0400	// 1kB ram
#endif
#define dictsize	0x0400	// 1kB dictionary
#define tibsize		80		// terminal input buf
#define padsize		80		// pad buf
#define primsize	sizeof(PRIMWORD)  // 12B for primitive word

#define im			0x80	// Type immediate
#define rs			0x40	// Type reserve
#define pr			0x20	// Type primitive

#define CRD			0x0D	// Cursor return
#define CRA			0x0A	// LF
#define BackSpc		0x08	// Backspace
#define Spc			0x20	// Space
#define EndState	0xEE	// Bye

#define iEXIT		0x00	// Index of (exit)
#define iEXEC		0x01	// Index of execute
#define iNEXT		0x02	// Index of next
#define iENTER		0x03	// Index of (colon)
#define iDOLIT		0x04	// Index of (lit)
#define iDOSLIT		0x05	// Index of (slit)
#define iDOCON		0x06	// Index of (con)
#define iDOVAR		0x07	// Index of (var)
#define iDODOES		0x08	// Index of (does>)
#define iDODEF		0x09	// Index of (dodefer)
#define iDODO		0x0A	// Index of (do)
#define iISDO		0x0B	// Index of (?do)
#define iLOOP		0x0C	// Index of (loop)
#define iPLOOP		0x0D	// Index of (+loop)
#define iDOBR		0x0E	// Index of (branch)
#define iDOCBR		0x0F	// Index of (?branch)
#define iDOVAL		0x10	// Index of (val)

#endif		
