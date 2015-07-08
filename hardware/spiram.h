/*
* Copyright 2013,2014 Jacques Deschênes
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
 * File:   spiram.h
 * Author: Jacques Deschênes
 *
 * Created on 2 juin 2014, 11:31
 */

#ifndef SPIRAM_H
#define	SPIRAM_H

#ifdef	__cplusplus
extern "C" {
#endif

// SPI RAM commands set
#define SPIRAM_WMRG (1) // write mode register
#define SPIRAM_RMRG (5) // read mode register
#define SPIRAM_WRITE (2) // write
#define SPIRAM_READ (3)  // read
#define SPIRAM_EDIO (0x3B) // enter dual I/O access
#define SPIRAM_EQIO (0x38) // enter quad I/O access
#define SPIRAM_RSTIO (x0ff) // reset dual and quad I/O access

// access type constant used as argument to spiram_access()
// VPC-32v2 support only ACCESS_SPI which is the default
#define ACCESS_SPI SPIRAM_RSTIO
#define ACCESS_DUAL SPIRAM_EDIO
#define ACCESS_QUAD SPIRAM_EQIO

// mode constant used as argument spiram_write_mode()
#define SPIRAM_MODE_BYTE (0)
#define SPIRAM_MODE_PAGE (0x80)
#define SPIRAM_MODE_SEQ  (0x40)

void spiram_init(); // peripheral initialisation
void spiram_access(int access); // set access mode
void spiram_read_mode();// read mode register
void spiram_write_mode(int mode); // set mode register
void spiram_write(unsigned address, unsigned count, const char* data); // write to SPIRAM
void spiram_read(unsigned address,unsigned count, char* buffer); // read from SPIRAM

#ifdef	__cplusplus
}
#endif

#endif	/* SPIRAM_H */

