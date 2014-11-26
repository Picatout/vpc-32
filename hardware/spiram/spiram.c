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

#include "../HardwareProfile.h"
#include "spiram.h"
#include "../Pinguino/sdmmc.h"



void sram_cmd(unsigned char cmd, unsigned addr){
    writeSPI(cmd);
    writeSPI(addr>>16);
    writeSPI(addr>>8);
    writeSPI(addr&0xff);
}



void sram_write_mode(unsigned char mode){
    _sram_enable();
    writeSPI(SRAM_WRMR);
    writeSPI(mode);
    _sram_disable();
}


unsigned char sram_read_mode(){
    unsigned char mode;
    _sram_enable();
    writeSPI(SRAM_RDMR);
    mode=writeSPI(0);
    _sram_disable();
    return mode;
}

// use sequencial mode
void sram_init(){
    if (!store_initialized) store_spi_init();
    sram_write_mode(SRAM_SQMD);
}


void sram_clear(){
    unsigned i;
    _sram_enable();
    sram_cmd(SRAM_WRITE,0);
    for (i=0;i<SRAM_SIZE;i++) writeSPI(0);
    _sram_disable();
}


unsigned char sram_read_byte(unsigned addr){
    unsigned char b;

    _sram_enable();
    sram_cmd(SRAM_READ,addr);
    b=writeSPI(0);
    _sram_disable();
    return b;
}

void sram_write_byte(unsigned addr, unsigned char byte){
    _sram_enable();
    sram_cmd(SRAM_WRITE,addr);
    writeSPI(byte);
    _sram_disable();
}

void sram_read_block(unsigned addr, unsigned char buffer[], unsigned count){
    unsigned i;
    _sram_enable();
    sram_cmd(SRAM_READ,addr);
    for (i=0;i<count;i++) buffer[i]=writeSPI(0);
    _sram_disable();
}

void sram_write_block(unsigned addr, unsigned char buffer[],unsigned count){
    unsigned i;
    _sram_enable();
    sram_cmd(SRAM_WRITE,addr);
    for (i=0;i<count;i++) writeSPI(buffer[i]);
    _sram_disable();
}



