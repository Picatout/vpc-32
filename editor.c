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
#include "hardware/ps2_kbd/keyboard.h"
#include "console.h"
#include "shell.h"
#include "hardware/Pinguino/ff.h"

#define BUFF_SIZE 8192 // dimension tampon editeur.
#define LOWER 0   // quel buffer utilisé
#define UPPER 1

static const char *ED_TEMP="ed.tmp";


#define  MODIFIED 1
#define  NEW 2
#define  F_INSERT 4  // 1=insert, 0=overwrite


static unsigned char edFlags=0;  // indicateurs booléens.
static unsigned int fsize=0;    // grosseur du fichier.
static unsigned short sline=0; // position du curseur
static unsigned short scol=0;  // à l'écran.
static unsigned int bline=0;    // position du curseur
static unsigned int bcol=0;     // dans le texte.
static char *ipos=0;  // pointeur du point d'insertion dans le tampon.
static char *tpos=0;  // position de la fin du texte après la fente.
static char *ed_buff=NULL;  // pointeur début du tampon texte.
static char *ftemp=NULL;   // nom du fichier temporaire
static char *fname=NULL;   // nom du fichier
static FILINFO *fi=NULL;   // information fichier (File Control Block).
static FIL     *fn=NULL;  // handle fichier édité.
static FIL     *ft=NULL;  // handle fichier temporaire.


/*
 * copie le fichier source dans le fichier temporaire.
 * les fins de ligne CRLF sont converties en LF.
 * les TAB sont convertis en espaces.
 */
void read_file(unsigned int i){
    FRESULT error=FR_OK;
    unsigned int j,cnt, size=0,line,col;
    unsigned char prev, c;
    for(;i && !error;i--){
        error=f_read(fn,ed_buff,512,&cnt);
        j=0;
        c=0;
        line=0;
        col=0;
        tpos=ed_buff+516;
        while (cnt){
            prev=c;
            c= *ed_buff++;
            cnt--;
            switch (c){
                case CR:
                    *ipos++=LF;
                    col=0;
                    line++;
                    j++;
                    break;
                case LF:
                    if (prev!=CR){
                        *ipos++=LF;
                        col=0;
                        line++;
                        j++;
                    }
                    break;
                case TAB:
                    *ipos++=SPACE;
                    col++;
                    j++;
                    while(col % TAB_WIDTH){
                        *ipos++=SPACE;
                        col++;
                        j++;
                    }
                    break;
                default:
                    if (c<32 || c>127){
                        *ipos++=127;
                    }else{
                        *ipos++=c;
                    }
                    col++;
                    j++;
            }
        }//while(j)
        error=f_write(ft,ed_buff+516,j,&cnt);
        size+=cnt;
    }
    error=f_close(fn);
    fsize=size;
    error=f_lseek(ft,0);
    if (size>BUFF_SIZE/3){
        error=f_read(ft,ed_buff+BUFF_SIZE*2/3,BUFF_SIZE/3,&cnt);
    }else{
        error=f_read(ft,ed_buff+BUFF_SIZE-size,size,&cnt);
    }

}//read_file()

void update_window(){
    text_coord_t curpos;
    
}//update_window()

void close_file(){
}//close_file()

void edit_file(){
    FRESULT error;
    unsigned int i;
    unsigned short c;
    if (error==FR_OK){
        bline=0;
        bcol=0;
        sline=0;
        scol=0;
        edFlags |= F_INSERT;
        set_cursor(CR_UNDER);
        clear_screen();
        update_window();
        while (1){
            c=wait_key(comm_channel);
            switch(c){
            }//switch
        }//while
    }else{
        error=f_close(fn);
        error=f_close(ft);
        error=f_unlink(ftemp);
    }
}//edit_file()

FRESULT open_for_edit(char *file_name){
    FRESULT error=FR_OK;
    // fichier de travail temporaire
    ftemp=(char *)ED_TEMP;
    error=f_unlink(ftemp); // supprime s'il existe déjà.
    error=f_open(ft,ftemp,FA_CREATE_NEW|FA_WRITE);
    edFlags=0;
    if (!error && file_name){
        fname=file_name;
        error=f_stat(fname,fi);
        if (error==FR_OK){
            error=f_open(fn,fname,FA_READ);
            read_file(fn->fsize);
        }else if (error==FR_NO_FILE){
            fn=NULL;
            edFlags |= NEW;
        }
    }
    return error;
}//open_for_edit()

void ed(char *file_name){
    FRESULT error;
    if (comm_channel!=LOCAL_CON){
        print(comm_channel, "This editor only works on local console.\r");
    }else{
        fi=malloc(sizeof(FILINFO));
        fn=malloc(sizeof(FIL));
        ft=malloc(sizeof(FIL));
        ed_buff=calloc(BUFF_SIZE,sizeof(char));
        if (fi && fn && ft && ed_buff){
            if ((error=open_for_edit(file_name))==FR_OK){
                edit_file();
            }else{
                print_error_msg(ERR_FIO,"",error);
            }
        }else{
            print_error_msg(ERR_ALLOC,"Not enough memory.\r",0);
        }
    }//else
    free(fi);
    free(fn);
    free(ft);
    free(ed_buff);
} // ed()


