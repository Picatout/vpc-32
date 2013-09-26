/*
* Copyright 2013, Jacques Desch�nes
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
 * Author: Jacques Desch�nes
 *
 * Created on 18 septembre 2013, 07:29
 * Description: un environnement de commande simple pour le VPC-32.
 *   liste des commandes:
 *      ls     liste des fichiers sur la carte SD
 *      rm     efface un fichier.
 *      mv     renomme un fichier.
 *      ed     ouvre l'�diteur
 *      as     assemble un fichier �cris en assembleur pour la machine virtuelle.
 *      run    execute un programme compil� pour la machine virtuelle.
 *      cp     copie un fichier
 *      snd    envoie un fichier vers le port s�riel
 *      rcv    re�ois un fichier par le port s�riel
 *      forth  lance l'environnement vpForth
 */

#include <string.h>
#include "hardware/HardwareProfile.h"
#include "hardware/keyboard.h"
#include "console.h"
#include "hardware/Pinguino/ff.h"
#include "hardware/Pinguino/fileio.h"
#include <plib.h>

#define MAX_LINE_LEN 80
#define MAX_TOKEN 5

#define ERR_ALLOC 0
#define ERR_USAGE 1
#define ERR_FIL_OPEN 2
#define ERR_CPY 3
const char *ERR_MSG[]={
    "Memory allocation error.\r",
    "Bad usage.\r",
    "File open error.\r",
    "Copy error.\r"
};

typedef struct{
    char buff[MAX_LINE_LEN]; // cha�ne saisie par l'utilisateur.
    unsigned char len;  // longueur de la cha�ne.
    unsigned char first; // position du premier caract�re du mot
    unsigned char next; // position du du curseur de l'analyseur.
} input_buff_t;

static input_buff_t cmd_line;
static char *cmd_tokens[MAX_TOKEN];

typedef enum CMDS {CMD_CD,CMD_DIR,CMD_DEL,CMD_REN,CMD_ED,CMD_CPY,CMD_SND,CMD_RCV,
                   CMD_FORTH, CMD_CLEAR,CMD_REBOOT,CMD_MORE,CMD_MKDIR} cmds_t;

#define CMD_LEN 13
const char *commands[CMD_LEN]={
    "cd", "dir","del","ren","edit","copy","send","receive","forth","clear","reboot","more","mkdir"};

int cmd_search(char *target){
    int i;
    for (i=CMD_LEN-1;i>=0;i--){
        if (!strcmp(target,commands[i])){
            break;
        }
    }
    return i;
}//cmd_search()

int next_token(void){
    unsigned char loop,quote,escape;
    cmd_line.first=cmd_line.next;
    while (cmd_line.first<cmd_line.len && (cmd_line.buff[cmd_line.first]==' ' ||
            cmd_line.buff[cmd_line.first]==9)){
        cmd_line.first++;
    }
    cmd_line.next=cmd_line.first;
    loop=TRUE;
    quote=FALSE;
    escape=FALSE;
    while (loop && (cmd_line.next<cmd_line.len)){
        switch (cmd_line.buff[cmd_line.next]){
            case ' ':
            case 9: // TAB
                if (!quote){
                    cmd_line.next--;
                    loop=FALSE;
                }
                break;
            case '\\':
                if (quote){
                    if (!escape){
                       escape=TRUE;
                    }
                    else{
                        escape=FALSE;
                    }
                }
                break;
            case '"':
                if (!quote){
                    quote=1;
                }
                else if (!escape){
                    loop=FALSE;
                }else{
                    escape=FALSE;
                }
                break;
            default:
                if (quote && escape){
                    escape=FALSE;
                }

        }//switch
        cmd_line.next++;
    } // while
    if (cmd_line.next>cmd_line.first)
        return 1;
    else
        return 0;
}//next_token()

void cd(int i){ // change le r�pertoire courant.
   if (i==2){
   }else{

   }
}//cd()

void del(int i){ // efface un fichier
    if (i==2){
        f_unlink(cmd_tokens[1]);
   }else{
       print(comm_channel, "delete file, USAGE: del file_name\r");
   }
}//del()

void ren(int i){ // renomme un fichier
    if (i==3){
        f_rename(cmd_tokens[1],cmd_tokens[2]);
    }else{
        print(comm_channel,"rename file, USAGE: ren name new_name\r");
    }
}//ren

void copy(int i){ // copie un fichier
    FIL *fsrc, *fnew;
    char *buff;
    int n;
    FRESULT error;
    if (i==3){
        fsrc=malloc(sizeof(FIL));
        fnew=malloc(sizeof(FIL));
        buff=malloc(512);
        error=FR_OK;
        if (fsrc && fnew && buff){
            if ((error=f_open(fsrc,cmd_tokens[1],FA_READ)==FR_OK) &&
                (error=f_open(fnew,cmd_tokens[2],FA_CREATE_NEW|FA_WRITE)==FR_OK)){
                while (error=f_read(fsrc,buff,512,&n)==FR_OK){
                    if (n){
                        if (!(error=f_write(fnew,buff,n,&n)==FR_OK)){
                            break;
                        }
                    }else{
                         break;
                    }
                }//while
                f_close(fsrc);
                f_close(fnew);
                free(buff);
                free(fsrc);
                free(fnew);
                if (error){
                    print(comm_channel,ERR_MSG[ERR_CPY]);
                    print_int(comm_channel, error,1);
                }
            }else{
                print(comm_channel,ERR_MSG[ERR_FIL_OPEN]);
            }
        }else{
            print(comm_channel,ERR_MSG[ERR_ALLOC]);
        }
    }else{
        print(comm_channel,"copy file USAGE: copy file_name new_file_name\r");
    }
}//copy()

void send(int i){ // envoie un fichier via uart
   if (i==2){
   }else{
       print(comm_channel, "send file via serial, USAGE: send file_name\r");
   }
}//send()

void receive(int i){ // re�ois un fichier via uart
   if (i==2){
   }else{
       print(comm_channel, "receive file from serial, USAGE: receive file_name\r");
   }
}//receive()


void more(int i){ // affiche � l'�cran le contenu d'un fichier texte
    FIL *fh;
    char *fmt, *buff, *rbuff, c, prev;
    int n,lcnt;
    text_coord_t cpos;
    if (i==2){
        fh=malloc(sizeof(FIL));
        if (fh && (f_open(fh,cmd_tokens[1],FA_READ)==FR_OK)){
            buff=malloc(512);
            fmt=malloc(64);
            if (fmt && buff){
                sprintf(fmt,"File: %s, size %d bytes\r",cmd_tokens[1],fh->fsize);
                print(comm_channel,fmt);
                lcnt=0;
                prev=0;
                while (f_read(fh,buff,512,&n)==FR_OK){
                    if (!n) break;
                    rbuff=buff;
                    for(;n;n--){
                        c=*rbuff++;
                        if (c=='\n'){
                            if (prev=='\r'){c=32;}else{c='\r';}
                        }
                        if ((c!=9 && c!='\r') && (c<32 || c>126)) {c=32;}
                        put_char(comm_channel,c);
                        prev=c;
                        cpos=get_curpos();
                        if (cpos.x==0){
                            lcnt++;
                            if (lcnt==(LINE_PER_SCREEN-1)){
                                print(comm_channel,"-- next --");
                                wait_key(comm_channel);
                                set_curpos(cpos.x,cpos.y);
                                clear_eol();
                                lcnt=0;
                            }
                        }
                    }
                }
                f_close(fh);
                free(fh);
                free(buff);
                free(fmt);
            }else{
                print(comm_channel,ERR_MSG[ERR_ALLOC]);
            }
        }else{
            print(comm_channel,"file open failed.\r");
        }
   }else{
       print(comm_channel, "display text file 1 screen fill at time.\r");
       print(comm_channel, "USAGE: more file_name\r");
   }
}//more

void editor(int i){ // lance l'�diteur de texte

}//editor()

void mkdir(int i){
    if (i==2){
    }else{
        print(comm_channel,"mkdir create a directory, USAGE: mkdir dir_name\r");
    }
}// mkdir()

void list_directory(int i){
    if (i>1){
        listDir(cmd_tokens[1]);
    }else{
        listDir("/");
    }
}//list_directory()

void execute_cmd(int i){
        switch (cmd_search(cmd_tokens[0])){
            case CMD_CD:
                cd(i);
                break;
            case CMD_DIR: // liste des fichiers sur la carte SD
                list_directory(i);
                break;
            case CMD_MKDIR:
                mkdir(i);
                break;
            case CMD_DEL: // efface un fichier
                del(i);
                break;
            case CMD_REN: // renomme ou d�place un fichier
                ren(i);
                break;
            case CMD_ED: // editeur
                editor(i);
                break;
            case CMD_FORTH: // lance DIOS forth
                cold();
                break;
            case CMD_SND:  // envoie un fichier vers la sortie uart
                send(i);
                break;
            case CMD_RCV:  // re�oit un fichier du uart
                receive(i);
                break;
            case CMD_CPY:   // copie un fichier
                copy(i);
                break;
            case CMD_CLEAR: // efface l'�cran
                clear_screen();
                break;
            case CMD_MORE:
                more(i);
                break;
            case CMD_REBOOT: // red�marrage � froid.
                asm("lui $t0, 0xbfc0"); // _on_reset
                asm("j  $t0");
                break;
            default:
                print(comm_channel,"unknown command!\r");
    }
}// execute_cmd()

const char *prompt="\r#";

void free_tokens(){
    int i;
    for (i=MAX_TOKEN-1;i>=0;i--){
        if (cmd_tokens[i]){
            free(cmd_tokens[i]);
            cmd_tokens[i]=NULL;
        }
    }
}//free_tokens()

int tokenize(){ // d�coupe la ligne d'entr�e en mots
    int i;
    char *token;
    i=0;
    while ((i<MAX_TOKEN) && next_token()){
        token=malloc(sizeof(char)*(cmd_line.next-cmd_line.first+1));
        memcpy(token,&cmd_line.buff[cmd_line.first],cmd_line.next-cmd_line.first);
        *(token+cmd_line.next-cmd_line.first)=(char)0;
        cmd_tokens[i]=token;
        i++;
    }//while
    return i;
}//tokenize()

void shell(void){
    int i;
    print(comm_channel,"VPC-32 shell\r16384 bytes free RAM.\r");
    free_tokens();
    while (1){
        print(comm_channel,prompt);
        cmd_line.first=0;
        cmd_line.next=0;
        cmd_line.len=readline(comm_channel,cmd_line.buff,CHAR_PER_LINE);
        if (cmd_line.len){
            i=tokenize();
            if (i) {
                execute_cmd(i);
                free_tokens();
            } // if
        }// if
    }//while(1)
    asm("lui $t0, 0xbfc0"); // _on_reset
    asm("j  $t0");
}//shell()

