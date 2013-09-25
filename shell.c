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
typedef struct{
    char buff[MAX_LINE_LEN]; // cha�ne saisie par l'utilisateur.
    unsigned char len;  // longueur de la cha�ne.
    unsigned char first; // position du premier caract�re du mot
    unsigned char next; // position du du curseur de l'analyseur.
} input_buff_t;

static input_buff_t cmd_line;

typedef enum CMDS {CMD_LS,CMD_RM,CMD_MV,CMD_ED,CMD_CP,CMD_SND,CMD_RCV,
                   CMD_FORTH, CMD_CLEAR,CMD_REBOOT} cmds_t;

#define CMD_LEN 10
const char *commands[CMD_LEN]={
    "ls","rm","mv","ed","cp","send","receive","forth","clear","reboot"};

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
    cmd_line.first=cmd_line.next;
    while (cmd_line.first<cmd_line.len && cmd_line.buff[cmd_line.first]==' '){cmd_line.first++;}
    cmd_line.next=cmd_line.first;
    while (cmd_line.next<cmd_line.len && cmd_line.buff[cmd_line.next]!=' '){cmd_line.next++;}
    if (cmd_line.next<(cmd_line.len)){
        cmd_line.buff[cmd_line.next++]=(char)(0);
    }
    if (cmd_line.next>cmd_line.first)
        return 1;
    else
        return 0;
}//next_token()

void list_directory(){
    listDir(".");
}//list_directory()

void parse_execute(void){
    while (next_token()){
        switch (cmd_search(&cmd_line.buff[cmd_line.first])){
            case CMD_LS: // liste des fichiers sur la carte SD
                list_directory();
                break;
            case CMD_RM: // efface un fichier
                break;
            case CMD_MV: // renomme ou d�place un fichier
                break;
            case CMD_ED: // editeur
                break;
            case CMD_FORTH: // lance DIOS forth
                cold();
                break;
            case CMD_SND:  // envoie un fichier vers la sortie uart
                break;
            case CMD_RCV:  // re�oit un fichier du uart
                break;
            case CMD_CP:   // copie un fichier
                break;
            case CMD_CLEAR: // efface l'�cran
                clear_screen();
                break;
            case CMD_REBOOT: // red�marrage � froid.
                asm("lui $t0, 0xbfc0");
                asm("j  $t0");
                break;
        }
    }
}// parse_execute()

const char *prompt="\r#";

void shell(void){
    print(comm_channel,"VPC-32 shell\r16384 bytes free RAM.\r");
    while (1){
        print(comm_channel,prompt);
        cmd_line.first=0;
        cmd_line.next=0;
        cmd_line.len=readline(comm_channel,cmd_line.buff,CHAR_PER_LINE);
        if (cmd_line.len){
            parse_execute();
        }
    }//while(1)
}//shell()

