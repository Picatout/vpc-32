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
 * File:   editor.h
 * Author: Jacques Deschênes
 *
 * Created on 18 septembre 2013, 07:18
 * Description: un éditeur simple pour le VPC-32
 *
 */

#include <stdlib.h>

#include "hardware/HardwareProfile.h"
#include "hardware/keyboard.h"
#include "console.h"
#include "shell.h"
#include "hardware/Pinguino/ff.h"

#define BUFF_SIZE 8192 // dimension tampon editeur.

static const char *ED_TEMP="ed.tmp";

static unsigned short line, col; // position du curseur texte

static char *ed_buff=NULL;
static char *fname=NULL;
static FILINFO *fi=NULL;
static FIL     *fh=NULL;

FRESULT open_for_edit(char *file_name){
    FRESULT error=FR_OK;
    if (file_name){
        fname=file_name;
        error=f_stat(fname,fi);
        if (error==FR_OK){
            error=f_open(fh,fname,FA_WRITE);

        }else if (error==FR_NO_FILE){
            error=f_open(fh,fname,FA_CREATE_NEW|FA_WRITE);
        }
    }else{
        fname=(char *)ED_TEMP;
        error=f_unlink(fname);
        error=f_open(fh,fname,FA_CREATE_NEW|FA_WRITE);
    }
    return error;
}//opne_for_edit()

void close_file(){
}//close_file()

void update_window(){
}//update_window()

void edit_file(){
    FRESULT error;
    unsigned int i;
    unsigned short c;
    if (fi->fsize > (BUFF_SIZE/2)){
        error=f_read(fh,ed_buff,(BUFF_SIZE/2),&i);
    }else{
        error=f_read(fh,ed_buff,fi->fsize,&i);
    }
    if (error==FR_OK){
        line=0;
        col=0;
        update_window();
        while (1){
            c=wait_key(comm_channel);
            switch(c){
            }//switch
        }
    }else{
        error=f_close(fh);
    }
}//edit_file()

void ed(char *file_name){
    FRESULT error;
    if (comm_channel!=LOCAL_CON){
        print(comm_channel, "This editor only works on local console.\r");
    }else{
        fi=malloc(sizeof(FILINFO));
        fh=malloc(sizeof(FIL));
        ed_buff=calloc(BUFF_SIZE,sizeof(char));
        if (fi && fh && ed_buff){
            if ((error=open_for_edit(file_name))==FR_OK){
                edit_file();
            }else{
                print_error_msg(ERR_FIO,"",error);
            }
        }else{
            print_error_msg(ERR_ALLOC,"Not enough memory\r",0);
        }
    }//else
    free(fi);
    free(fh);
    free(ed_buff);
} // ed()


