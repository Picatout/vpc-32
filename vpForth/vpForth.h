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

/* 
 * File:   vpForth.h
 * Author: Jacques Deschênes
 *
 * Created on 30 mai 2013
 */

#ifndef TINYFORTH_H
#define TINYFORTH_H
#include "opcodes.h"

#define RAM_SPACE FREE_RAM
#define FLASH_SPACE 2048

#define SYSTEM_NAME "vpFORTH "
#define SYSTEM_VERSION "0.01\r"

extern char *here; // pointeur espace code
extern unsigned char ram_code[RAM_SPACE];
extern const unsigned char flash_code[FLASH_SPACE];


int StackVM(const unsigned char *code);

void vpForth();

#endif	/* TINYFORTH_H */

