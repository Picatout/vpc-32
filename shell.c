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
 * Description: un environnement de commande simple pour le VPC-32.
 *   liste des commandes:
 *      ls     liste des fichiers sur la carte SD
 *      rm     efface un fichier.
 *      mv     renomme un fichier.
 *      ed     ouvre l'éditeur
 *      as     assemble un fichier écris en assembleur pour la machine virtuelle.
 *      run    execute un programme compilé pour la machine virtuelle.
 *      cp     copie un fichier
 *      snd    envoie un fichier vers le port sériel
 *      rcv    reçois un fichier par le port sériel
 *      forth  lance l'environnement vpForth
 */

#include <string.h>
#include "hardware/HardwareProfile.h"
#include "hardware/ps2_kbd/keyboard.h"
#include "console.h"
#include "hardware/Pinguino/ff.h"
#include "hardware/Pinguino/fileio.h"
#include <plib.h>
#include "shell.h"
#include "vpcBASIC/vpcBASIC.h"

#define MAX_LINE_LEN 80
#define MAX_TOKEN 5

/*
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
    ERR_FIO
} SH_ERROR;
*/

const char *ERR_MSG[]={
    "no error\r",
    "not implemented yet.\r",
    "Memory allocation error.\r",
    "Bad usage.\r",
    "File open error.\r",
    "Copy error.\r",
    "Mkdir error.\r",
    "file does not exist.\r",
    "operation denied.\r",
    "disk operation error, code is %d \r",
    "no SD card detected.\r"
};

void print_error_msg(SH_ERROR err_code,const char *detail,FRESULT io_code){
    char *fmt;
    if (err_code==ERR_FIO){
        fmt=malloc(64);
        if (fmt){
            sprintf(fmt,ERR_MSG[ERR_FIO],io_code);
            print(comm_channel,fmt);
            free(fmt);
        }
    }else{
       print(comm_channel,ERR_MSG[err_code]);
    }
    if (detail){
       print(comm_channel,detail);
    }
}//print_error_msg()

typedef struct{
    char buff[MAX_LINE_LEN]; // chaîne saisie par l'utilisateur.
    unsigned char len;  // longueur de la chaîne.
    unsigned char first; // position du premier caractère du mot
    unsigned char next; // position du du curseur de l'analyseur.
} input_buff_t;

static input_buff_t cmd_line;
static char *cmd_tokens[MAX_TOKEN];


typedef enum CMDS {CMD_BASIC, CMD_CD, CMD_CLEAR,CMD_CPY,CMD_DEL,CMD_DIR,CMD_ED,CMD_FORMAT,
                   CMD_FREE,CMD_HELP,CMD_MKDIR,CMD_MORE,CMD_REBOOT,CMD_RCV,CMD_REN,
                    CMD_SND,CMD_COUNT} cmds_t;

#define CMD_LEN CMD_COUNT
const char *commands[CMD_LEN]={"basic","cd","cls","copy","del","dir","edit","format",
                               "free","help","mkdir","more","reboot","receive","ren","send"};


int cmd_search(char *target){
    int i;
    for (i=CMD_LEN-1;i>=0;i--){
        if (!strcmp(target,commands[i])){
            break;
        }
    }
    return i;
}//cmd_search()

void display_cmd_list(){
    int i;
    text_coord_t pos;
    for(i=0;i<CMD_LEN;i++){
        pos=get_curpos();
        if (pos.x>(CHAR_PER_LINE-strlen(commands[i])-2)){
            put_char(comm_channel,'\r');
        }
        print(comm_channel,commands[i]);
        if (i<(CMD_LEN-1)){
            print(comm_channel," ");
        }
    }
    put_char(comm_channel,'\r');
}

void cmd_format(int i){
    if (i==2){
        print_error_msg(ERR_NOT_DONE,NULL,0);
    }else{
        print(comm_channel,"USAGE: format volume_name\r");
    }
}

static int next_token(void){
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

void cd(int i){ // change le répertoire courant.
    char *path;
    if (!SDCardReady){
        if (!mount(0)){
            print_error_msg(ERR_NO_SDCARD,NULL,0);
            return;
        }else{
            SDCardReady=TRUE;
        }
    }
    FRESULT error=FR_OK;
   if (i==2){
       error=f_chdir(cmd_tokens[1]);
   }
   if (!error){
       path=malloc(255);
       if (path){
          error=f_getcwd(path,255);
          if(!error){
              print(comm_channel,path);
              put_char(comm_channel,'\r');
          }
          free(path);
       }
   }
}//cd()

void del(int i){ // efface un fichier
    FILINFO *fi;
    if (!SDCardReady){
        if (!mount(0)){
            print_error_msg(ERR_NO_SDCARD,NULL,0);
            return;
        }else{
            SDCardReady=TRUE;
        }
    }
    FRESULT error=FR_OK;
    if (i==2){
        fi=malloc(sizeof(FILINFO));
        if (fi){
            error=f_stat(cmd_tokens[1],fi);
            if (!error){
                if (fi->fattrib & (ATT_DIR|ATT_RO)){
                    print_error_msg(ERR_DENIED,"can't delete directory or read only file.\r",0);
                }
                else{
                    error=f_unlink(cmd_tokens[1]);
                }
            }
            free(fi);
            if (error){
                print_error_msg(ERR_FIO,"",error);
            }
        }else{
               print_error_msg(ERR_ALLOC,"delete failed.\r",0);
        }
   }else{
       print_error_msg(ERR_USAGE, "delete file USAGE: del file_name\r",0);
   }
}//del()

void ren(int i){ // renomme un fichier
    if (!SDCardReady){
        if (!mount(0)){
            print_error_msg(ERR_NO_SDCARD,NULL,0);
            return;
        }else{
            SDCardReady=TRUE;
        }
    }
    if (i==3){
        f_rename(cmd_tokens[1],cmd_tokens[2]);
    }else{
        print_error_msg(ERR_USAGE,"rename file, USAGE: ren name new_name\r",0);
    }
}//ren

void copy(int i){ // copie un fichier
    FIL *fsrc, *fnew;
    char *buff;
    int n;
    if (!SDCardReady){
        if (!mount(0)){
            print_error_msg(ERR_NO_SDCARD,NULL,0);
            return;
        }else{
            SDCardReady=TRUE;
        }
    }
    FRESULT error;
    if (i==3){
        fsrc=malloc(sizeof(FIL));
        fnew=malloc(sizeof(FIL));
        buff=malloc(512);
        error=FR_OK;
        if (fsrc && fnew && buff){
            if ((error=f_open(fsrc,cmd_tokens[1],FA_READ)==FR_OK) &&
                (error=f_open(fnew,cmd_tokens[2],FA_CREATE_NEW|FA_WRITE)==FR_OK)){
                while ((error=f_read(fsrc,buff,512,&n))==FR_OK){
                    if (n){
                        if (!((error=f_write(fnew,buff,n,&n))==FR_OK)){
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
            }
            if (error){
                print_error_msg(ERR_FIO,"copy failed.\r",error);
            }
        }else{
            print(comm_channel,ERR_MSG[ERR_ALLOC]);
        }
    }else{
        print_error_msg(ERR_USAGE,"copy file USAGE: copy file_name new_file_name\r",0);
    }
}//copy()

void send(int i){ // envoie un fichier via uart
    // to do
   if (i==2){
       print_error_msg(ERR_NOT_DONE,NULL,0);
   }else{
       print(comm_channel, "send file via serial, USAGE: send file_name\r");
   }
}//send()

void receive(int i){ // reçois un fichier via uart
    // to do
   if (i==2){
       print_error_msg(ERR_NOT_DONE,NULL,0);
   }else{
       print(comm_channel, "receive file from serial, USAGE: receive file_name\r");
   }
}//receive()


void more(int i){ // affiche à l'écran le contenu d'un fichier texte
    FIL *fh;
    char *fmt, *buff, *rbuff, c,key;
    int n;
    text_coord_t cpos;
    if (!SDCardReady){
        if (!mount(0)){
            print_error_msg(ERR_NO_SDCARD,NULL,0);
            return;
        }else{
            SDCardReady=TRUE;
        }
    }
    FRESULT error=FR_OK;
    if (i==2){
        fh=malloc(sizeof(FIL));
        if (fh && ((error=f_open(fh,cmd_tokens[1],FA_READ))==FR_OK)){
            buff=malloc(512);
            fmt=malloc(64);
            if (fmt && buff){
                sprintf(fmt,"File: %s, size %d bytes\r",cmd_tokens[1],fh->fsize);
                print(comm_channel,fmt);
                key=0;
                while (key!=ESC && f_read(fh,buff,512,&n)==FR_OK){
                    if (!n) break;
                    rbuff=buff;
                    for(;n;n--){
                        c=*rbuff++;
                        if ((c!=TAB && c!=CR) && (c<32 || c>126)) {c=32;}
                        put_char(comm_channel,c);
                        if (comm_channel==LOCAL_CON){
                            cpos=get_curpos();
                            if (cpos.x==0){
                                if (cpos.y>=(LINE_PER_SCREEN-1)){
                                    cpos.y=LINE_PER_SCREEN-1;
                                    invert_video(TRUE);
                                    print(comm_channel,"-- next --");
                                    invert_video(FALSE);
                                    key=wait_key(comm_channel);
                                    if (key=='q' || key==ESC){key=ESC; break;}
                                    if (key==CR){
                                        set_curpos(cpos.x,cpos.y);
                                        clear_eol();
                                    }else{
                                        clear_screen();
                                    }
                                }
                            }
                        }
                    }
                }
                f_close(fh);
                free(fh);
                free(buff);
                free(fmt);
            }else{
                print_error_msg(ERR_ALLOC,"Can't display file.\r",0);
            }
        }else{
            print_error_msg(ERR_FIO,"File open failed.\r",error);
        }
   }else{
       print_error_msg(ERR_USAGE, "USAGE: more file_name\r",0);
   }
}//more

void editor(int i){ // lance l'éditeur de texte
    if (i>1){
        ed(cmd_tokens[1]);
    }else{
        ed(NULL);
    }
}//editor()

void mkdir(int i){
    FRESULT error=FR_OK;
    char *fmt;
    if (!SDCardReady){
        if (!mount(0)){
            print_error_msg(ERR_NO_SDCARD,NULL,0);
            return;
        }else{
            SDCardReady=TRUE;
        }
    }
    if (i==2){
        fmt=malloc(CHAR_PER_LINE+1);
        if (fmt && (error=f_mkdir(cmd_tokens[1])==FR_OK)){
            sprintf(fmt,"directory %s created\r",cmd_tokens[1]);
            print(comm_channel,fmt);
        }else{
            if (!fmt){
                print(comm_channel,ERR_MSG[ERR_ALLOC]);
            }else{
                print(comm_channel,ERR_MSG[ERR_MKDIR]);
            }
        }
    }else{
        print_error_msg(ERR_USAGE,"mkdir create a directory, USAGE: mkdir dir_name\r",0);
    }
}// mkdir()

void list_directory(int i){
    FRESULT error;
    FIL *fh;
    char fmt[55];
    if (!SDCardReady){
        if (!mount(0)){
            print_error_msg(ERR_NO_SDCARD,NULL,0);
            return;
        }else{
            SDCardReady=TRUE;
        }
    }
    if (i>1){
        error=listDir(cmd_tokens[1]);
        if (error==FR_NO_PATH){// not a directory, try file
            fh=malloc(sizeof(FIL));
            if (fh && ((error=f_open(fh,cmd_tokens[1],FA_READ))==FR_OK)){
                sprintf(fmt,"File: %s, size %d bytes\r",cmd_tokens[1],fh->fsize);
                print(comm_channel,fmt);
                f_close(fh);
                free(fh);
            }
        }
    }else{
        error=listDir(".");
    }
    if (error) print_error_msg(ERR_FIO,"",error);
}//list_directory()

//display heap status
void cmd_free(){
    char fmt[55];
    sprintf(fmt,"free RAM %d/%d BYTES\r",free_heap(),heap_size);
    print(comm_channel,fmt);
}

void execute_cmd(int i){
        switch (cmd_search(cmd_tokens[0])){
            case CMD_HELP:
                display_cmd_list();
                break;
            case CMD_CD:
                cd(i);
                break;
            case CMD_DIR: // liste des fichiers sur la carte SD
                list_directory(i);
                break;
            case CMD_FORMAT:
                cmd_format(i);
                break;
            case CMD_FREE:
                cmd_free();
                break;
            case CMD_MKDIR:
                mkdir(i);
                break;
            case CMD_DEL: // efface un fichier
                del(i);
                break;
            case CMD_REN: // renomme ou déplace un fichier
                ren(i);
                break;
            case CMD_ED: // editeur
                editor(i);
                break;
            case CMD_BASIC: // lance DIOS forth
               vpcBasic();
                break;
            case CMD_SND:  // envoie un fichier vers la sortie uart
                send(i);
                break;
            case CMD_RCV:  // reçoit un fichier du uart
                receive(i);
                break;
            case CMD_CPY:   // copie un fichier
                copy(i);
                break;
            case CMD_CLEAR: // efface l'écran
                if (comm_channel==LOCAL_CON){
                    clear_screen();
                }else{
                    print(comm_channel,"\E[2J\E[H"); // VT100 commands
                }
                break;
            case CMD_MORE:
                more(i);
                break;
            case CMD_REBOOT: // redémarrage à froid.
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

int tokenize(){ // découpe la ligne d'entrée en mots
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
    
    print(comm_channel,"VPC-32 shell\rfree RAM (bytes): ");
    print_int(comm_channel,free_heap(),0);
    crlf();
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

