/*
* Copyright 2013, Jacques Desch�nes
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

/* 
 * File:   opcodes.h
 * Author: Jacques Desch�nes
 *
 * Created on 31 mai 2013
 */

#ifndef OPCODES_H
#define	OPCODES_H

// op codes
#define IEND 0
#define IQRX 1
#define IEMIT 2
#define ILIT 3
#define IFCALL 4
#define IRET 5
#define IBRA 6
#define IQBRA 7
#define IEXEC 8
#define ISTORE 9
#define IFETCH 10
#define ICSTORE 11
#define ICFETCH 12
#define IRPFETCH 13
#define IRPSTORE 14
#define IRFETCH 15
#define ITOR 16
#define IRFROM 17
#define ISPFETCH 18
#define ISPSTORE 19
#define IDROP 20
#define IDUP 21
#define ISWAP 22
#define IOVER 23
#define IADD 24
#define ISUB 25
#define IMUL 26
#define IDIV 27
#define IMOD 28
#define ILTZ 29
#define IAND 30
#define IOR 31
#define IXOR 32
#define INOT 33
#define IKEY 34
#define ISFR 35
#define ISET 36
#define ICLR 37
#define ITGL 38
#define ICLIT 39
#define IWLIT 40
#define IDOTQ 41
#define IRDLN 42
#define IRCALL 43
#define ITYPE 44
#define IDELAY 45
#define IQDUP  46
#define IPLUS1 47
#define IMINUS1 48
#define IPLUSSTR 49
#define IDDROP  50
#define IROT  51
#define IDDUP 52
#define IMIN 53
#define IMAX 54
#define IABS 55
#define ILSHIFT 56
#define IRSHIFT 57
#define IDIVMOD 58
#define IQBRAZ 59
#define IDOT 60
#define IZSP 61
#define IZRP 62
#define IHERE 63
#define IBASE 64
#define IEQUAL 65
#define INEQUAL 66
#define ILESS  67
#define IGREATER 68
#define ILTEZ 69
#define IGTEZ 70
#define IIEMIT 71

#define LASTOP 71


#define TOK_COUNT (LASTOP+1)
#define IBADOP -TOK_COUNT

#define RAM_SEG  0xA000
#define CODE_SEG 0x9D00
#define SFR_SEG  0xBF88


/* constantes */
#define FLASH_SIZE 2048
#define CELL_SIZE 4

/* utilisation des regitres MIPS */
#define ip s0  /* pointeur d'instruction VM */
#define dp s1  /* pointeur pile de donn�es */
#define rp s2  /* pointeur pile de contr�le */
#define ptr s3 /* pointeur de travail */

#define _dpush  addiu dp,dp,CELL_SIZE
#define _dpop   addiu dp,dp,-CELL_SIZE
#define _rpush  addiu rp,rp,CELL_SIZE
#define _rpop   addiu rp,rp,-CELL_SIZE
#define _tos    0(dp)
#define _next    addiu ip,ip,1
#define _bytecode 0(ip)






#endif	/* OPCODES_H */

