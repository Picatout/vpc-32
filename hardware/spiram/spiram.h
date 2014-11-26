/*
* Copyright 2013, 2014 Jacques Deschênes
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
 * Author: jacques
 *
 * Created on 17 novembre 2014, 20:42
 */

#ifndef SPIRAM_H
#define	SPIRAM_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "../store/store_spi.h"

//sram commands
#define SRAM_READ (3)
#define SRAM_WRITE (2)
#define SRAM_EDIO (0x3b)
#define SRAM_EQIO (0x38)
#define SRAM_RSTIO (0xff)
#define SRAM_RDMR (5)
#define SRAM_WRMR (1)
#define SRAM_BTMOD (0x00)
#define SRAM_PGMD (0x80)
#define SRAM_SQMD (0x40)
// 23LC1024 size in bytes
#define SRAM_SIZE (131072)

    void sram_init();
    unsigned char sram_read_byte(unsigned addr);
    void sram_write_byte(unsigned addr, unsigned char byte);
    void sram_read_block(unsigned addr, unsigned char buffer[],unsigned count);
    void sram_write_block(unsigned addr, unsigned char buffer[],unsigned count);
    void sram_write_mode(unsigned char mode);
    unsigned char sram_read_mode();
    void sram_clear();

#ifdef	__cplusplus
}
#endif

#endif	/* SPIRAM_H */

