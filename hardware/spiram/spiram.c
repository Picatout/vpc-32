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

#define sram_enable() {SRAM_PORT &= ~SRAM_SEL;}
#define sram_disable() {SRAM_PORT |= SRAM_SEL;}


void sram_cmd(unsigned char cmd, unsigned addr){
    writeSPI(cmd);
    writeSPI(addr>>16);
    writeSPI(addr>>8);
    writeSPI(addr&0xff);
}


// périphérique SPI2 initialisé par initSD() dans sdmmc.c
void sram_init(){
    SRAM_PORT |= SRAM_SEL;  // garde SPIRAM désactivée
    SRAM_TRIS &= ~SRAM_SEL; // CS1 en mode sortie
}


void sram_write_mode(unsigned char mode){
    sram_enable();
    writeSPI(SRAM_WRMR);
    writeSPI(mode);
    sram_disable();
}

unsigned char sram_read_mode(){
    unsigned char mode;
    sram_enable();
    writeSPI(SRAM_RDMR);
    mode=writeSPI(0);
    sram_disable();
    return mode;
}

void sram_clear(){
    unsigned i;
    sram_enable();
    sram_cmd(SRAM_WRITE,0);
    for (i=0;i<SRAM_SIZE;i++) writeSPI(0);
    sram_disable();
}


unsigned char sram_read_byte(unsigned addr){
    unsigned char b;

    sram_enable();
    sram_cmd(SRAM_READ,addr);
    b=writeSPI(0);
    sram_disable();
    return b;
}

void sram_write_byte(unsigned addr, unsigned char byte){
    sram_enable();
    sram_cmd(SRAM_WRITE,addr);
    writeSPI(byte);
    sram_disable();
}

void sram_read_block(unsigned addr, unsigned char buffer[], unsigned count){
    unsigned i;
    sram_enable();
    sram_cmd(SRAM_READ,addr);
    for (i=0;i<count;i++) buffer[i]=writeSPI(0);
    sram_disable();
}

void sram_write_block(unsigned addr, unsigned char buffer[],unsigned count){
    unsigned i;
    sram_enable();
    sram_cmd(SRAM_WRITE,addr);
    for (i=0;i<count;i++) writeSPI(buffer[i]);
    sram_disable();
}



