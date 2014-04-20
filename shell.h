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
 * File:   shell.h
 * Author: Jacques Deschênes
 *
 * Created on 18 septembre 2013, 07:29
 */

#ifndef SHELL_H
#define	SHELL_H

#ifdef	__cplusplus
extern "C" {
#endif
#include "hardware/Pinguino/ff.h"
    
typedef enum {
    ERR_NONE=0,
    ERR_NOT_DONE,
    ERR_ALLOC,
    ERR_USAGE,
    ERR_FIL_OPEN,
    ERR_CPY,
    ERR_MKDIR,
    ERR_NOTEXIST,
    ERR_DENIED,
    ERR_FIO,
    ERR_NO_SDCARD
} SH_ERROR;



void print_error_msg(SH_ERROR err_code, const char *detail, FRESULT io_code);

void shell(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SHELL_H */

