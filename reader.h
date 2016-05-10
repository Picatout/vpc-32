/*
 * Copyright 2015, 2016, Jacques Deschenes
 * 
 * This file is part of PV16SOG project.
 * 
 * ***  LICENCE ****
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; See 'copying.txt' in root directory of source.
 * If not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 *  
 * to contact the author:  jd_temp@yahoo.fr
 * 
 */

/*
 * File: reader.h
 * Description: lit une ligne à partir de la source spécifiée.
 * Date: 2016-02-10 
 */

#ifndef READER_H_
#define READER_H_
#include <xc.h>
#include <stdint.h>

typedef enum {eDEV_KBD,eDEV_SPIRAM,eDEV_SDCARD,eDEV_FLASH} reader_src_t;


#define READER_BUFFER_SIZE (256)

typedef struct _sram_dev{
    uint32_t first; //position début fichier
    uint32_t pos; // position courante
    uint32_t fsize; // grandeur fichier
}sram_device_t;


typedef struct _flash_dev{
    uint32_t saddr; //position début dans mémoire flash
    uint32_t pos; // position courante dans mémoire flash
    uint32_t fsize; // grandeur fichier
}flash_device_t;

typedef struct _reader{
    reader_src_t device;
    char buffer[READER_BUFFER_SIZE];
    int count; //nombre d'octets dans le buffer
    int inp;  //position dans le buffer
    bool eof; // fin de fichier atteint.
    void *handle; //handle device
}reader_t;


#define sram_first(h) (((sram_device_t*)h)->first)
#define sram_pos(h) (((sram_device_t*)h)->pos)
#define sram_fsize(h)(((sram_device_t*)h)->fsize)
#define sram_left(h) (sram_fsize(h)-sram_pos(h))
#define sram_addr(h) (sram_first(h)+sram_pos(h)) 

extern int complevel;
extern void reader_init(reader_t *reader, reader_src_t device, void *handle);
extern char reader_getc(reader_t *reader);
extern inline void reader_ungetc(reader_t *reader);
//extern int reader_gets(reader_t *reader, char *buffer, int len);

#endif // READER_H_