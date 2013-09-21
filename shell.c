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
#include "hardware/keyboard.h"
#include "console.h"
#include "hardware/Pinguino/ff.h"
#include <plib.h>

#define MAX_LINE_LEN 80
typedef struct{
    char buff[MAX_LINE_LEN]; // chaîne saisie par l'utilisateur.
    unsigned char len;  // longueur de la chaîne.
    unsigned char first; // position du premier caractère du mot
    unsigned char next; // position du du curseur de l'analyseur.
} input_buff_t;

static input_buff_t cmd_line;

typedef enum CMDS {CMD_LS,CMD_RM,CMD_MV,CMD_ED,CMD_AS,CMD_RUN,
                   CMD_CP,CMD_SND,CMD_RCV,CMD_FORTH} cmds_t;

#define CMD_LEN 10
const char *commands[CMD_LEN]={
    "ls","rm","mv","ed","as","run","cp","snd","rcv","forth"};

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

void parse_execute(void){
    while (next_token()){
        switch (cmd_search(&cmd_line.buff[cmd_line.first])){
            case CMD_LS:
                break;
            case CMD_RM:
                break;
            case CMD_MV:
                break;
            case CMD_ED:
                break;
            case CMD_AS:
                break;
            case CMD_FORTH:
                vpForth();
                break;
            case CMD_RUN:
                break;
            case CMD_SND:
                break;
            case CMD_RCV:
                break;
            case CMD_CP:
                break;
        }
    }
}// parse_execute()

const char *prompt="\r>";

void shell(void){
    int len;
    print(comm_channel,"VPC-32 shell\r");
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

