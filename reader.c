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
 * File: reader.c
 * Description: lit une ligne à partir de la source spécifiée.
 * Date: 2016-02-10 
 */

#include <stdlib.h>
//#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "hardware/HardwareProfile.h"
#include "hardware/spiram/spiram.h"
#include "hardware/Pinguino/fileio.h"
#include "hardware/Pinguino/ff.h"
#include "console.h"
#include "reader.h"
#include "font.h"




int complevel=0;

static void display_prompt(){
    int i;
    crlf();
    for(i=0;i<=complevel;i++) put_char(comm_channel,'>');
}//f

//rempli le buffer
static bool read_sram(reader_t *reader){
    if (sram_pos(reader->handle)==sram_fsize(reader->handle)){
        reader->eof=true;
        return false;
    }
    reader->count=min(READER_BUFFER_SIZE,sram_left(reader->handle));
    sram_read_block(sram_addr(reader->handle),(uint8_t*)reader->buffer,reader->count);
    reader->inp=0;
    sram_pos(reader->handle)+=reader->count;
    return true;
}

static bool fill_buffer(reader_t *reader){
    FRESULT result;
    switch (reader->device){
        case eDEV_KBD:
            display_prompt();
            reader->count=readline(comm_channel,reader->buffer,CHAR_PER_LINE);
            reader->buffer[reader->count++]='\r';
            reader->inp=0;
            return true;
            break;
        case eDEV_SPIRAM:
            return read_sram(reader);
            break;
        case eDEV_SDCARD:
            result=f_read(reader->handle,(uint8_t*)reader->buffer,READER_BUFFER_SIZE,&reader->count);
            reader->inp=0;
            if (reader->count<=0){
                reader->count=0;
                reader->eof=true;
                return false;
            }
            return true;
            break;
        case eDEV_FLASH:
            break;
    }
    return false;
}//f()

void reader_init(reader_t *reader, reader_src_t device, void *handle){
    reader->inp=0;
    reader->count=0;
    reader->eof=false;
    reader->device=device;
    reader->handle=handle;
}//f()

char reader_getc(reader_t *reader){
    char c=-1;
    if (reader->eof) return c;
    if (reader->inp < reader->count){
        c=reader->buffer[reader->inp++];
    }else if (fill_buffer(reader)){
        c=reader->buffer[reader->inp++];
    }
    return c;
}

inline void reader_ungetc(reader_t *reader){
    reader->inp--;
}

// retourne la longueur de la chaîne ou -1 pour fin de fichier
// chaîne terminée par CR, LF, CRLF, \0
//int reader_gets(reader_t *reader, char *buffer, int len){
//    char c=-1;
//    int i=0;
//    
//    if (reader->eof) return -1;
//    while (!reader->eof && i<len && c){
//        c=reader_getc(reader);
//        switch(c){
//            case '\r':
//                c=reader_getc(reader);
//                if (c!='\n') reader_ungetc(reader);
//            case '\n':
//                c=0;
//                break;
//            default:
//                if (c<32) buffer[i++]=' '; else buffer[i++]=c;
//        }//switch
//    }//while
//    buffer[i]=0;
//    return i;
//}//f

