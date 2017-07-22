/*
* Copyright 2013,2016 Jacques Deschênes
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
*
 *  *  * ***  LICENCE ****
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

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>

//#include "Hardware/hardware.h"
#include "console.h"
#include "hardware/tvout/ntsc.h"
#include "hardware/spiram/spiram.h"
#include "hardware/ps2_kbd/keyboard.h"
#include "hardware/Pinguino/fileio.h"
#include "hardware/sound/sound.h"
#include "graphics.h"
#include "font.h"
#include "reader.h"
#include "vpcBASIC/vm.h"
#include "hardware/ps2_kbd/QWERTY.h"

#define LAST_COL (CHAR_PER_LINE-1)
#define LINE_MAX_LEN (CHAR_PER_LINE-1)
#define LAST_LINE (LINE_PER_SCREEN-1)
#define SCREEN_SIZE (LINE_PER_SCREEN*CHAR_PER_LINE)
#define ED_BUFF_SIZE (SRAM_SIZE-2)
#define MAX_SIZE (ED_BUFF_SIZE)

#define refused()  tone(500,100);

#define MODE_INSERT 1
#define MODE_OVERWR 0
#define NAME_MAX_LEN 32

//contient 1'écran texte
uint8_t screen[LINE_PER_SCREEN][CHAR_PER_LINE];

typedef struct editor_state{
    uint32_t fsize; // grandeur du fichier en octets.
    uint32_t scr_first; //premier caractère visible
    uint32_t gap_first; //position début gap
    uint32_t tail; // position fin gap
    int8_t scr_line; // ligne du curseur
    int8_t scr_col; //colonne du curseur
    struct {
      uint8_t insert:1;
      uint8_t modified:1;
      uint8_t new:1;
    }flags;
} ed_state_t;



static ed_state_t state;

static char fname[32]; // nom du fichier en édition

typedef struct search_struct{
   bool ignore_case;
   bool loop;
   bool found;
   uint8_t col;
   char target[CHAR_PER_LINE];
} search_t;

static search_t search_info;

static void open_file();
static void load_file(const char *name);
static void save_file();
static void save_file_as();
static void line_up();
static void line_down();
static void char_left();
static void char_right();
static void delete_left();
static void delete_at();
static void delete_to_end();
static void delete_to_start();
static void insert_char(char c);
static void replace_char(char c);
static void line_home();
static void line_end();
static void enter();
static void file_home();
static void file_end();
static void update_display();
static void page_up();
static void page_down();
static void word_right();
static void word_left();
static void goto_line();
static uint8_t get_line_forward(uint8_t* line, uint32_t from);
static uint8_t get_line_back(uint8_t* line, uint32_t from);
static void jump_to_line(uint16_t line_no);


static void print_line(uint8_t line){
    set_curpos(0,line);
    clear_eol();
    print(comm_channel,(char*)&screen[line]);
}

static void prompt_continue(){
    print(comm_channel,"\nany key...");
    wait_key(comm_channel);
}

static void ed_error(const char *msg, int code){
    println(comm_channel,msg);
    print(comm_channel,"error code: ");
    print_int(comm_channel,code,0);
    if (comm_channel==LOCAL_CON){
        prompt_continue();
        clear_screen();
    }
}

//si fichier modifié confirme 
//avant de continuer
static bool ask_confirm(){
    char key;
    bool answer=state.flags.modified;
    if (answer){
        tone(1000,100);
        invert_video(true);
        set_curpos(0,0);
        clear_eol();
        print(comm_channel,"file unsaved! continue (y/n)?");
        key= wait_key(comm_channel);
        answer=(key=='y')||(key=='Y');
        invert_video(false);
        set_curpos(0,0);
        clear_eol();
        print_line(0);
        set_curpos(state.scr_col,state.scr_line);
    }else{
        answer=true;
    }
    return answer;
}

static void new_file(){
    if (ask_confirm()){
        invert_video(true);
        clear_screen();
        sram_clear_block(0,ED_BUFF_SIZE);
        memset(&state,0,sizeof(ed_state_t));
        memset(screen,0,SCREEN_SIZE);
        state.flags.insert=MODE_INSERT;
        state.flags.new=1;
        state.flags.modified=false;
        state.tail=MAX_SIZE;
        fname[0]=0;
        search_info.target[0]=0;
        search_info.ignore_case=false;
        search_info.loop=false;
        search_info.found=false;
        invert_video(false);
        update_display();
    }
}//f()

static bool quit;

static void leave_editor(){
    if (ask_confirm()){
        set_cursor(CR_UNDER);
        quit=true;
    }
}
 
static void list_files(){
    invert_video(true);
    clear_screen();
    listDir(".");
    prompt_continue();
    invert_video(false);
    update_display();
}//f

//initialement le curseur est au début de la cible
static void mark_target(){
    int i,len;

    len=strlen(search_info.target);
    set_curpos(state.scr_col,state.scr_line);
    invert_video(true);
    for (i=0;i<len;i++) put_char(comm_channel,screen[state.scr_line][state.scr_col+i]);
    invert_video(false);
    set_curpos(state.scr_col,state.scr_line);
}//f

static int search_line(char *line, char *target, int from){
    int i,j=0;
    for (i=from;line[i];i++){
        if (search_info.target[j]==line[i]){
            j++;
            if (j==strlen(search_info.target)){
                i++;
                break;
            }
        }else{
             j=0; 
        }
    }
    return j==strlen(search_info.target)?i-j:-1;
}//f

static void search_next(){
    char line[CHAR_PER_LINE];
    int len,lcount=0,pos;
    uint16_t saddr;
    
    if (!search_info.target[0] || (!search_info.loop && (state.gap_first>=(state.fsize-1)))){
        refused();
        return;
    }
    search_info.found=false;
    strcpy(line,(char*)&screen[state.scr_line]);
    if (search_info.ignore_case) uppercase(line);
    pos=search_line(line,search_info.target,state.scr_col+1);
    if (pos>state.scr_col){
        search_info.col=pos;
        state.scr_col=pos;
        mark_target();
        return;
    }
    len=strlen((char*)&screen[state.scr_line]);
    saddr=state.tail+(len-state.scr_col)+1;
    while (saddr<ED_BUFF_SIZE){
        lcount++;
        len=get_line_forward((uint8_t*)line,saddr);
        if (search_info.ignore_case) uppercase(line);
        pos=search_line(line,search_info.target,0);
        if (pos>-1){
            search_info.found=true;
            search_info.col=(uint8_t)pos;
            break;
        }
        saddr+=len+1;
    }//while
    if (search_info.found){
        pos=state.scr_line+lcount;
        line_down();
        saddr=state.tail;
        state.scr_col=search_info.col;
        while (lcount){
            len=get_line_forward((uint8_t*)line,saddr);
            lcount--;
            if (lcount){
                sram_write_block(state.gap_first,(uint8_t*)line,len+1);
                state.gap_first+=len+1;
                sram_clear_block(state.tail,len+1);
                state.tail+=len+1;
                saddr=state.tail;
            }else{
                sram_write_block(state.gap_first,(uint8_t*)line,search_info.col);
                state.gap_first+=search_info.col;
                sram_clear_block(state.tail,search_info.col);
                state.tail+=search_info.col;
            }
        }//while
        if (pos>LAST_LINE){
            state.scr_first=state.gap_first-search_info.col;
            state.scr_line=0;
            update_display();
        }else{
            state.scr_line=pos;
            set_curpos(state.scr_col,state.scr_line);
        }
        mark_target();
    }else if (search_info.loop){
        saddr=0;
        lcount=1;
        while (saddr<state.gap_first){
            len=get_line_forward((uint8_t*)line,saddr);
            if (search_info.ignore_case) uppercase(line);
            pos=search_line(line,search_info.target,0);
            if (pos>-1){
                search_info.found=true;
                search_info.col=(uint8_t)pos;
                break;
            }
            lcount++;
            saddr+=len+1;
        }//while
        if (search_info.found){
            jump_to_line(lcount);
            while (state.scr_col<search_info.col) char_right();
            mark_target();
        }else{
            refused();
        }
    }else{
        refused();
    }
}//f

static bool parse_search_line(){
    int pos=0;
    char *str, prev=0;
    int options=0;
    
    str=search_info.target;
    while (str[pos]){
        if (str[pos]=='-' && prev==' '){
            str[pos-1]=0;
            prev='-';
            pos++;
            break;
        }
        prev=str[pos];
        pos++;
    }    
    while (str[pos] && options!=3){
        switch(str[pos]){
            case '-':
            case ' ':
                break;
            case 'l':
            case 'L':
                if (prev!='-'){
                    refused();
                    return false;
                }
                search_info.loop=true;
                options|=1; 
                if (str[pos+1] && str[pos+1]!=' '){
                    refused();
                    return false;
                }
                break;
            case 'i':
            case 'I':
                if (prev!='-'){
                    refused();
                    return false;
                }
                search_info.ignore_case=true;
                options|=2;
                uppercase(str);
                if (str[pos+1] && str[pos+1]!=' '){
                    refused();
                    return false;
                }
                break;
            default:
                refused();
                return false;
        }//switch
        prev=str[pos];
        pos++;
    }//while
    return true;
}//f

static void search(){
    int len;
    invert_video(true);
    clear_screen();
    print(comm_channel,"USAGE: target [-I] [-L]\n");
    search_info.found=false;
    search_info.loop=false;
    search_info.ignore_case=false;
    print(comm_channel,"? ");
    len=readline(comm_channel,search_info.target,CHAR_PER_LINE);
    if (search_info.target[len-1]=='\n'){
        search_info.target[len-1]=0;
        len--;
    }
    invert_video(false);
    update_display();
    if (len && parse_search_line()){
        search_next();
    }
}//f

static bool get_file_name(char *name){
    int len;
    
    crlf();
    print(comm_channel,"file name? ");
    len=readline(comm_channel,name,NAME_MAX_LEN);
    if (name[len-1]=='\n')name[len-1]=0;
    uppercase(name);
    return len;
}//f()

static void load_file(const char *name){
    uint32_t saddr=0,fsize;
    FRESULT result;
    FIL fh;
    int count=0; 
    unsigned char  c=0, buffer[CHAR_PER_LINE];
    bool line_break=false;
    reader_t r;

//    put_char(STDIO,FF);// debug only
   
    new_file();
    strcpy(fname,name);
    result=f_open(&fh,fname,FA_READ);
    if (result){
        ed_error("File load failed!\n",result);
        return;
    }
    if (fh.fsize>MAX_SIZE){
        ed_error("File too big!\n",0);
        return;
    }
    fsize=fh.fsize;
    state.tail=MAX_SIZE;
    print(comm_channel,"loading file...\n");
    print_int(comm_channel,fsize,0);
    print(comm_channel," bytes");
    reader_init(&r,eDEV_SDCARD,&fh);
    while (!r.eof){
        c=reader_getc(&r);
        switch(c){
            case 255:  
                break;
            case CR:
            case LF:
                line_break=true;
                break;
            default: 
                if ((c>=32) && (c<(32+FONT_SIZE))){
                    buffer[count++]=c;
                   // put_char(STDIO,c); // debug only
                }else {
                    buffer[count++]=' ';
                   // put_char(STDIO,' '); //debug only
                    print_int(STDIO,c,0); // debug only
                }
                if (count==(CHAR_PER_LINE-1)) line_break=true;
        }//switch(c)
        if (line_break ){
            //put_char(STDIO,LF); //debug only
            buffer[count++]=0;
            sram_write_block(saddr,(uint8_t*)buffer,count);
            saddr+=count;
            count=0;
            line_break=false;
        };
    }//while(!r.eof)
    if (count){
        sram_write_block(saddr,(uint8_t*)buffer,count);
        saddr+=count;
        //line_no++;
    }
    f_close(&fh);
    invert_video(false);
    state.fsize=saddr;
    state.gap_first=saddr;
    state.scr_first=saddr;
    file_home();
}//f()

static void open_file(){
    FIL fh;
    FRESULT result;
    char name[32];

    if (ask_confirm()){
        invert_video(true);
        clear_screen();    
        print(comm_channel,"open file\n");
        if (get_file_name(name)){
            if (!(result=f_open(&fh,name,FA_READ))){
                f_close(&fh);
                load_file(name);
                return;
            }else{
                ed_error("faile to open file.",result);
            }
        }
        invert_video(false);
        clear_screen();
        update_display();
    }
}//f()

inline static void replace_nulls(uint8_t *buffer,int len){
    int i;
    for(i=0;i<=len;i++) if (!buffer[i]) buffer[i]=CR;
}//f

static void save_file(){
#define BUFFER_SIZE 128
    uint8_t buffer[BUFFER_SIZE];
    int size;
    uint32_t saddr=0;
    FRESULT result;
    FIL fh;

    if ((result=f_open(&fh,fname,FA_WRITE+FA_CREATE_ALWAYS))){
        ed_error("failed to create file!\n",result);
        return;
    }
    invert_video(true);
    clear_screen();
    print(comm_channel,"saving file...\n");
    while ((result==FR_OK) && (saddr < state.gap_first)){
        size=min(BUFFER_SIZE,state.gap_first-saddr);
        sram_read_block(saddr,buffer,size);
        saddr+=size;
        replace_nulls(buffer,size);
        result=f_write(&fh,buffer,size,&size);
    }
    saddr=state.tail;
    while((result==FR_OK) && saddr < ED_BUFF_SIZE){
        size=min(BUFFER_SIZE,ED_BUFF_SIZE-saddr);
        sram_read_block(saddr,buffer,size);
        saddr+=size;
        replace_nulls(buffer,size);
        result=f_write(&fh,buffer,size,&size);
    }
    f_close(&fh);
    if (result){
        ed_error("diksI/O error...\n",result);
        return;
    }
    state.flags.modified=false;
    state.flags.new=0;
    crlf();
    print(comm_channel,fname);
    print(comm_channel," saved, size: ");
    print_int(comm_channel,state.fsize,0);
    prompt_continue();
    invert_video(false);
    update_display();
}//f()

static void save_file_as(){
    char name[32];
    invert_video(true);
    clear_screen();
    if (get_file_name(name)){
        if (strlen(name)){
            strcpy(fname,name);
            save_file();
        }
    }
    invert_video(false);
    update_display();
}//f()

static void file_info(){
    char line[CHAR_PER_LINE];
    set_curpos(0,0);
    invert_video(true);
    clear_eol();
    if (strlen(fname)){
        sprintf(line,"file%c: %s, size: %d",state.flags.modified?'*':' ',fname,state.fsize);
    }else{
        sprintf(line,"no name%c, size: %d",state.flags.modified?'*':' ',state.fsize);
    }
    print(comm_channel,line);
    prompt_continue();
    invert_video(false);
    print_line(0);
    print_line(1);
    set_curpos(state.scr_col,state.scr_line);
}


const char* hkeys[]={
  "<CTRL-DEL> delete to end of line\n",
  "<CTRL-A> save as\n",
  "<CTRL-END> file end\n",
  "<CTRL-F> list SDcard files\n",
  "<CTRL-G> goto line...\n",
  "<CTRL-H> display hotkeys\n",
  "<CTRL-HOME> file start\n",
  "<CTRL_LEFT> word left\n",
  "<CTRL-N> new file...\n",
  "<CTRL-O> open file...\n",
  "<CTRL-Q> Quit editor\n",
  "<CTRL-RIGHT> word right\n",
  "<CTRL-S> save file\n",
  "<F1> file name and size\n"
  "<F3> set search criterion\n",
  "<SHIFT-F3> search next\n",
  ""
};

//affiche les touches d'action
static void hot_keys(){
    int i=0;

    invert_video(true);
    clear_screen();
    while (strlen(hkeys[i])){    
        print(comm_channel,hkeys[i++]);
    }
    crlf();
    prompt_continue();
    invert_video(false);
    update_display();
}

static void editor_init(){
    invert_video(false);
    set_cursor(CR_UNDER);
    clear_screen();
    fname[0]=0;
    new_file();
}//f()


void editor(const char* name){
    unsigned short key;
    FATFS fh;
    FRESULT result;
    
    state.flags.modified=false;
    editor_init();
    if (name){
        load_file(name);
    }
    quit=false;
    while(!quit){
        key=wait_key(comm_channel);
        switch(key){
            case VK_UP:
                line_up();
                break;
            case VK_DOWN:
                line_down();
                break;
            case VK_LEFT:
                if (key_state & F_CTRL){
                    word_left();
                }else{
                    char_left();
                }
                break;
            case VK_RIGHT:
                if (key_state & F_CTRL){
                    word_right();
                }else{
                    char_right();
                }
                break;
            case VK_HOME:
                if ((key_state & F_CTRL))
                    file_home();
                else
                    line_home();
                break;
            case VK_END:
                if ((key_state & F_CTRL))
                    file_end();
                else
                    line_end();
                break;
            case VK_PGUP:
                page_up();
                break;
            case VK_PGDN:
                page_down();
                break;
            case VK_INSERT:
                state.flags.insert=~state.flags.insert;
                if (state.flags.insert){
                    set_cursor(CR_UNDER);
                }else{
                    set_cursor(CR_BLOCK);
                }
                break;
            case VK_DELETE:
                if ((key_state & F_LCTRL)){
                    delete_to_end();
                }else{
                    delete_at();
                }
                break;
            case VK_BACK:
                if (key_state & F_LCTRL){
                    delete_to_start();
                }else{
                    delete_left();
                }
                break;
            case VK_ENTER:
                enter();
                break;
            case VK_F1:
                file_info();
                break;
            case VK_F3:
                if (key_state & F_SHIFT) search_next();else search();
                break;
            default:
                if (key_state & F_CTRL){
                    switch (key){
                        case 'a': // sauvegarde sous...
                            save_file_as();
                            break;
                        case 'f': // liste des fichiers
                            list_files();
                            break;
                        case 'g':// va à la ligne
                            goto_line();
                            break;
                        case 'h': // affiche hot keys
                            hot_keys();
                            break;
                        case 'n': // nouveau fichier
                            new_file();
                            break;
                        case 'o': // ouvrir un fichier
                            open_file();
                            break;
                        case 'q': // quitter
                            leave_editor();
                            break;
                        case 's': // sauvegarde
                            invert_video(true);
                            clear_screen();
                            if (strlen(fname)) save_file(); else save_file_as();
                            invert_video(false);
                            update_display();
                            break;
                        default:
                            refused();
                            
                    }//switch
                }else  if ((key>=32)&(key<FONT_SIZE+32)){
                    if (state.flags.insert){
                        insert_char(key);
                    }else{
                        replace_char(key);
                    }
                }
            }//switch
    }
    invert_video(false);
    clear_screen();
}//f()


// cherche ligne avant from
// condition: from doit pointé le premier caractère d'une ligne
// argurments:
//   line-> buffer recevant la ligne
//   from -> adresse SRAM début ligne courante
// retourne la longueur de la ligne
uint8_t get_line_back(uint8_t *line, uint32_t from){
    int j=0,size;

    if (from==0){
        line[0]=0;
        return 0;
    }
    if (from<=state.gap_first){
        size=min(CHAR_PER_LINE,from);
        sram_read_block(from-size,line,size);
    }else{
        size=min(from-state.tail,CHAR_PER_LINE);
        if (size<CHAR_PER_LINE){
            j=min(CHAR_PER_LINE-size,state.gap_first);
            sram_read_block(state.gap_first-j,line,j);
        }
        sram_read_block(from-size,line+j,size);
        size+=j;
    }
    if (size<CHAR_PER_LINE) line[size]=0;
    j=size-1;
    if (!line[j]){
        j--;
    }
    while (j>0 && line[j]) j--;
    if (!line[j]) strcpy((char*)line,(char*)&line[j+1]);
    return strlen((char*)line);
}//f()


//cherche ligne débutant a partir de from
//condition: from pointe le premier caractère de la ligne courante
// arguments:
//   from -> adresse SRAM début de ligne
//   line -> buffer recevant la ligne
// retourne la longueur de la ligne
uint8_t get_line_forward(uint8_t *line, uint32_t from){
    int j,size;
    
    if (from <= state.gap_first){
        size=min(CHAR_PER_LINE-1, state.gap_first-from);
        sram_read_block(from,line,size);
        if (size<(CHAR_PER_LINE-1) && state.tail<MAX_SIZE){
            from=state.tail;
            j=min(CHAR_PER_LINE-size-1,MAX_SIZE-from);
            sram_read_block(from,&line[size],j);
            size+=j;
        }
    }else{
        size=min(CHAR_PER_LINE-1,MAX_SIZE-from);
        sram_read_block(from,line,size);
    }
    line[size]=0;
    size=strlen((char*)line);
    j=size;
    while (j<CHAR_PER_LINE) line[j++]=0;
    return size;
}//f()


static void update_display(){
    uint32_t from;
    uint8_t scr_line=0, llen=0;
    
    clear_screen();
    memset(screen,0,SCREEN_SIZE);
    from=state.scr_first;
    while (from < state.gap_first && scr_line<LINE_PER_SCREEN){
        llen=get_line_forward((uint8_t*)&screen[scr_line],from);
        print_line(scr_line);
        scr_line++;
        from+=llen+1;
    }
    if (from<state.tail) from=state.tail;
    while (from<MAX_SIZE && scr_line<LINE_PER_SCREEN){
        llen=get_line_forward((uint8_t*)&screen[scr_line],from); //print(STDIO,&screen[scr_line]);put_char(STDIO,LF);
        if (!llen) break;
        print_line(scr_line);
        scr_line++;
        from+=llen+1;
    }
    set_curpos(state.scr_col,state.scr_line);
}//f();


static void line_up(){
    uint8_t llen, line[CHAR_PER_LINE];
    
    line_home();
    if (state.gap_first){ 
        llen=get_line_back(line,state.gap_first);
        state.gap_first-=llen+1;
        sram_clear_block(state.gap_first,llen);
        state.tail-=llen+1;
        sram_write_block(state.tail,line,llen+1);
        state.scr_col=0;
        if (state.gap_first < state.scr_first){
            state.scr_first=state.gap_first;
            scroll_down();
            memmove(&screen[1],screen, SCREEN_SIZE-CHAR_PER_LINE);
            memmove(screen,line,CHAR_PER_LINE);
            print_line(0);
        }else{
            state.scr_line--;
            memmove(&screen[state.scr_line],line,CHAR_PER_LINE);
            print_line(state.scr_line);
            
        }
        set_curpos(state.scr_col,state.scr_line);
    }
}//f()

static void line_down(){
    uint8_t llen,line[CHAR_PER_LINE];
    
    line_end();
    if (state.tail==MAX_SIZE){
        return;
    }
    sram_write_byte(state.tail++,0);
    sram_write_byte(state.gap_first++,0);
    state.scr_col=0;
    state.scr_line++;
    if (state.scr_line==LINE_PER_SCREEN){
        state.scr_line--;
        state.scr_first+=strlen((char*)&screen[0])+1;
        scroll_up();
        memmove(screen,(void*)&screen[1],SCREEN_SIZE-CHAR_PER_LINE);
        llen=get_line_forward(line,state.tail);
        strcpy((char*)&screen[LAST_LINE],(char*)line);
        print_line(LAST_LINE);
    }
    set_curpos(state.scr_col,state.scr_line);
}//f()

//déplace le curseur vers la gauche d'un caractère
static void char_left(){
    if (state.scr_col){
        state.scr_col--;
        sram_write_byte(--state.tail,screen[state.scr_line][state.scr_col]);
        sram_write_byte(state.gap_first--,0);
        set_curpos(state.scr_col,state.scr_line);
    }else if (state.gap_first){
        line_up();
        line_end();
    }
}//f()

//déplace le curseur vers la droite d'un caractère
static void char_right(){
    uint8_t c;
    
    if (state.gap_first==state.fsize) return;
    if (state.scr_col<strlen((char*)&screen[state.scr_line])){
        c=sram_read_byte(state.tail);
        sram_write_byte(state.gap_first++,c);
        sram_write_byte(state.tail++,0);
        state.scr_col++;
        set_curpos(state.scr_col,state.scr_line);
    }else{
        line_down();
    }
}//f()

static void delete_left(){
    if (state.gap_first){
        char_left();
        delete_at();
    }
} //f()

static void delete_at(){
//    int llen,i;
//    char c;
//    uint8_t line[CHAR_PER_LINE];
    
    if (state.tail==MAX_SIZE) return;
    sram_write_byte(state.tail++,0);
    state.fsize--;
    state.flags.modified=true;
    if (state.scr_col<strlen((char*)&screen[state.scr_line])){
        strcpy((char*)&screen[state.scr_line][state.scr_col],
                (char*)&screen[state.scr_line][state.scr_col+1]);
        print_line(state.scr_line);
        set_curpos(state.scr_col,state.scr_line);
    }else{
        update_display();
    }
}//f()

//efface tous les caractères après le cursor
//jusqu'à la fin de ligne.
//si le curseur est au début de ligne
//la ligne est supprimée au complet '\n' inclus.
static void delete_to_end(){
    uint8_t llen,c;
    
    if (state.fsize==0) return;
    llen=strlen((char*)&screen[state.scr_line]);
    if (state.scr_col){
        memset(&screen[state.scr_line][state.scr_col],0,CHAR_PER_LINE-state.scr_col);
        c=llen-state.scr_col;
        sram_clear_block(state.tail,c);
        state.fsize-=c;
        state.tail+=c;
        print_line(state.scr_line);
    }else{
        memset(&screen[state.scr_line],0,CHAR_PER_LINE);
        sram_clear_block(state.tail,llen);
        state.tail+=llen;
        state.fsize-=llen;
        if (state.tail<ED_BUFF_SIZE){
            c=sram_read_byte(state.tail);
            if (c==0){
                sram_write_byte(state.tail,0);
                state.tail++;
                state.fsize--;
            }
        }
        update_display();
    }
    state.flags.modified=true;
}//f()

static void delete_to_start(){
    uint8_t buffer[CHAR_PER_LINE];
    
    if (state.scr_col){
        memset(buffer,0,CHAR_PER_LINE);
        state.gap_first-=state.scr_col;
        sram_write_block(state.gap_first,buffer,state.scr_col);
        memmove(&screen[state.scr_line],
                &screen[state.scr_line][state.scr_col],CHAR_PER_LINE-state.scr_col);
        state.fsize-=state.scr_col;
        state.scr_col=0;
        print_line(state.scr_line);
        state.flags.modified=true;
    }
}//f()

static void insert_char(char c){
    if (state.fsize==MAX_SIZE){
        refused();
        return;
    }
    if (strlen((char*)&screen[state.scr_line])<LINE_MAX_LEN){
        sram_write_byte(state.gap_first++,c);
        state.fsize++;
        memmove(&screen[state.scr_line][state.scr_col+1],
                &screen[state.scr_line][state.scr_col],CHAR_PER_LINE-state.scr_col-1);
        screen[state.scr_line][state.scr_col]=c;
        print_line(state.scr_line);
        state.scr_col++;
        set_curpos(state.scr_col,state.scr_line);
    }else{
        sram_write_byte(state.gap_first++,0);
        sram_write_byte(state.gap_first++,c);
        state.fsize+=2;
        state.scr_col=1;
        state.scr_line++;
        if (state.scr_line>LAST_LINE){
            state.scr_first+=strlen((char*)&screen[0])+1;
            state.scr_line=LAST_LINE;
        }
        update_display();
    }
    state.flags.modified=true;
}//f()

static void replace_char(char c){
    int llen;
    llen=strlen((char*)&screen[state.scr_line]);
    if (state.scr_col<llen){
        sram_write_byte(state.gap_first++,c);
        sram_write_byte(state.tail++,0);
        screen[state.scr_line][state.scr_col]=c;
        put_char(comm_channel,c);
        state.scr_col++;
        state.flags.modified=true;
        set_curpos(state.scr_col,state.scr_line);
    }else{
        insert_char(c);
    }
}//f();

static void line_home(){
    if (state.scr_col){
        state.tail-=state.scr_col;
        sram_write_block(state.tail,
                (uint8_t*)&screen[state.scr_line],state.scr_col);
        state.gap_first-=state.scr_col;
        sram_clear_block(state.gap_first,state.scr_col);
        state.scr_col=0;
        set_curpos(0,state.scr_line);
    }
}//f()

static void line_end(){
    uint8_t llen,forward;
    
    llen=strlen((char*)&screen[state.scr_line]);
    forward=llen-state.scr_col;
    if (forward){
        sram_write_block(state.gap_first,(uint8_t*)&screen[state.scr_line][state.scr_col],forward);
        state.gap_first+=forward;
        sram_clear_block(state.tail,forward);
        state.tail+=forward;
        state.scr_col=llen;
        set_curpos(state.scr_col,state.scr_line);
    }
}//f()

 static void enter(){
    if ((state.flags.insert) && (state.fsize==MAX_SIZE)){
        refused();
        return;
    }
    if (state.flags.insert){
        state.fsize++;
    }else{
        sram_write_byte(state.tail++,0);
    }
    sram_write_byte(state.gap_first++,0);
    state.scr_col=0;
    state.scr_line++;
    if (state.scr_line==LINE_PER_SCREEN){
        state.scr_first+=strlen((char*)&screen[0])+1;
        state.scr_line--;
    }
    update_display();
}//f()

static void file_home(){
    uint8_t size, maxlen, line[CHAR_PER_LINE];
    if (state.fsize==MAX_SIZE){
        state.tail=0;
    }else{
        maxlen=min(CHAR_PER_LINE,state.tail - state.gap_first);
        while (state.gap_first){
            size=min(maxlen,state.gap_first);
            state.gap_first-=size;
            sram_read_block(state.gap_first,line,size);
            sram_clear_block(state.gap_first,size);
            state.tail-=size;
            sram_write_block(state.tail,line,size);
        }
    }
    state.scr_first=0;
    state.gap_first=0;
    state.scr_col=0;
    state.scr_line=0;
    update_display();
}//f()

static void file_end(){
    int size;
    uint8_t maxlen, line[CHAR_PER_LINE];

    if (state.gap_first==state.fsize) return;
    if (state.fsize==MAX_SIZE){
        state.scr_first=MAX_SIZE-1;
        state.gap_first=MAX_SIZE;
        state.tail=MAX_SIZE;
    }else{
        maxlen=min(CHAR_PER_LINE,state.tail - state.gap_first); 
        while (state.gap_first < state.fsize){
            size=min(maxlen,MAX_SIZE-state.tail);
            sram_read_block(state.tail,line,size);
            sram_clear_block(state.tail,size);
            sram_write_block(state.gap_first,line,size);
            state.tail+=size;
            state.gap_first+=size;
        }
        state.scr_first=state.gap_first;
        size=get_line_back(line,state.gap_first);
        if (size){
            state.scr_first-=size;
            state.gap_first-=size;
            sram_clear_block(state.gap_first,size);
            state.tail-=size;
            sram_write_block(state.tail,line,size);
        }
        state.scr_col=0;
        state.scr_line=0;
        
    }
    update_display();
}//f()

static void page_up(){
    int i=0;

    while (state.scr_line) line_up();
    i=LINE_PER_SCREEN;
    while (state.scr_first && i){
        line_up();
        i--;
    }
}

static void page_down(){
     while (state.gap_first<(state.fsize-1) && state.scr_line<LAST_LINE){
         line_down();
     }
    if ((state.scr_line==LAST_LINE) && (state.scr_col==0)){
        state.scr_first=state.gap_first;
        state.scr_line=0;
    }
    update_display();
}


static void word_right(){
    uint8_t c, scr_col;
    uint8_t line[CHAR_PER_LINE];
    
    if (state.scr_col==strlen((char*)&screen[state.scr_line])){
        line_down();
    }else{
        scr_col=state.scr_col;
        c=screen[state.scr_line][scr_col++];
        if (!isalnum(c)){
            while (c && !isalnum(c)){
                c=screen[state.scr_line][scr_col++];;
            }
        }else{
            while (c  && isalnum(c)){
                c=screen[state.scr_line][scr_col++];
            }    
        }
        scr_col--;
        if (scr_col>state.scr_col){
            sram_read_block(state.tail,line,scr_col-state.scr_col);
            sram_clear_block(state.tail,scr_col-state.scr_col);
            state.tail+=scr_col-state.scr_col;
            sram_write_block(state.gap_first,line,scr_col-state.scr_col);
            state.gap_first+=scr_col-state.scr_col;
            state.scr_col=scr_col;
            set_curpos(scr_col,state.scr_line);
        }
    }
}

static void word_left(){
    uint8_t c, scr_col, diff;
    uint8_t line[CHAR_PER_LINE];
    
    if (!state.scr_col){
        line_up();
        line_end();
    }else{
        scr_col=state.scr_col-1;
        c=screen[state.scr_line][scr_col];
        if (!isalnum(c)){
            while (scr_col && !isalnum(c)){
               --scr_col;
               c=screen[state.scr_line][scr_col];
            }
        }else{
            while (scr_col && isalnum(c)){
               --scr_col;
               c=screen[state.scr_line][scr_col];

            }
        }
        if (scr_col) scr_col++;
        if (scr_col<state.scr_col){
            diff=state.scr_col-scr_col;
            state.gap_first-=diff;
            sram_read_block(state.gap_first,line,diff);
            sram_clear_block(state.gap_first,diff);
            state.tail-=diff;
            sram_write_block(state.tail,line,diff);
            state.scr_col=scr_col;
            set_curpos(scr_col,state.scr_line);
        }
    }
}

//move cursor to line_no
static void jump_to_line(uint16_t line_no){
    uint16_t llen,count;
    uint8_t line[CHAR_PER_LINE];
    
    file_home();
    if (!line_no) return;
    line_no--;
    if (line_no<LINE_PER_SCREEN){
        while(state.scr_line<line_no) line_down();
        return;
    }
    count=0;
    while ((state.tail<MAX_SIZE) && (count<line_no)){
        llen=get_line_forward(line,state.tail);
        sram_write_block(state.gap_first,line,llen+1);
        state.gap_first+=llen+1;
        sram_clear_block(state.tail,llen+1);
        state.tail+=llen+1;
        count++;
    }
    state.scr_first=state.gap_first;
    set_curpos(0,0);
    update_display();
}//f

// ask for target line number
static void goto_line(){
    uint8_t llen;
    char line[16];
    uint16_t line_no=0;
    
    invert_video(true);
    set_curpos(0,0);
    clear_eol();
    print(comm_channel,"goto line: ");
    llen=readline(comm_channel,line,16);
    invert_video(false);
    if (llen){
        line_no=atoi((const char*)line);
        jump_to_line(line_no);
    }else{
        print_line(0);
    }
}
