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
 *  Name: console.c
 *  Description: text console for NTSC video output
 *  Author: Jacques Deschênes
 *  Date Created: 2013-09-06
 */

#include "console.h"
#include "hardware/serial_comm.h"

#define X_OFS  ((HRES%CHAR_WIDTH)/2)  // offset vidéo position curseur x
#define Y_OFS  ((VRES%LINE_SPACING)/2)  // offset vidéo position curseur y

static text_coord_t curpos={X_OFS, Y_OFS};

static unsigned char tab_width=TAB_WIDTH;

void scroll_up(void){
    memcpy(video_bmp,&video_bmp[LINE_SPACING][0],(LINE_PER_SCREEN-1)*LINE_SPACING*HRES/8);
    memset(&video_bmp[LINE_SPACING*(LINE_PER_SCREEN-1)][0],0,HRES/8*LINE_SPACING);
}//scroll_up();

void scroll_down(void){
    memcpy(&video_bmp[LINE_SPACING][0],video_bmp,(LINE_PER_SCREEN-1)*LINE_SPACING*HRES/8);
    memset(video_bmp,0,HRES/8*LINE_SPACING);
}//scroll_down()

void cursor_right(void){
    curpos.x += CHAR_WIDTH;
    if (curpos.x>(CHAR_PER_LINE*CHAR_WIDTH)){
        curpos.x = X_OFS;
        curpos.y += LINE_SPACING;
        if (curpos.y>(LINE_PER_SCREEN*LINE_SPACING)){
            scroll_up();
            curpos.y -= LINE_SPACING;
        }
    }
} // cursor_right()

void cursor_left(void){
}// cursor_left()

void cursor_up(void){
}// cursor_up()

void cursor_down(void){
}//cursor_down()

void put_char(char c){
    register int i,l,r,b,x,y;
    x=curpos.x;
    y=curpos.y;
    switch (c){
        case CR:
            curpos.x=X_OFS;
            curpos.y += CHAR_HEIGHT+1;
            break;
        case TAB:
            curpos.x += (curpos.x%tab_width);
            if (curpos.x>(CHAR_PER_LINE*CHAR_WIDTH)){
                curpos.x = X_OFS;
                curpos.y += LINE_SPACING;
                if (curpos.y>(LINE_PER_SCREEN*LINE_SPACING)){
                    scroll_up();
                }
            }
            break;
        default:
            if ((c<32) || (c>(FONT_SIZE+32))) break;
            c -=32;
            b=x>>5;
            r=0;
            l=27-(x&0x1f);
            if (l<0){
                r=-l;
            }
            for (i=0;i<7;i++){
                if (y>=VRES) break;
                if (r){
                    video_bmp[y][b] &= ~(0x1f>>r);
                    video_bmp[y][b] |= font5x7[c][i]>>r;
                    video_bmp[y][b+1] &= ~(0x1f<<32-r);
                    video_bmp[y][b+1] |= font5x7[c][i]<<(32-r);
                    y++;
                } else{
                    video_bmp[y][b] &= ~(0x1f<<l);
                    video_bmp[y++][b] |= font5x7[c][i]<<l;
                }
            }
            cursor_right();
    }//switch(c)
}//put)char()

void clear_screen(){
    memset(video_bmp,0,HRES/8*VRES);
    curpos.x=X_OFS;
    curpos.y=Y_OFS;
} // clear_screen()

void print(const char *text){
    while (*text){
        put_char(*text++);
    }
}// print()

void print_hex(unsigned int hex, unsigned char width){
    char c[12], *d;
    int i;
    c[11]=0;
    d= &c[10];
    for(i=width;i;i--){
        *d=(hex%16);
        if (*d<10)
            *d += '0';
        else
            *d += 'A'-10;
        hex /=16;
        d--;
    }
    print(&c[11-width]);
} // print_hex()

void print_decimal(int number){
}// print_decimal()

void set_tab_width(unsigned char width){
    tab_width=width;
}// set_tab_width()

void clear_eol(void){
    memset(&video_bmp[curpos.y][curpos.x],0,HRES/8*LINE_SPACING);
}// clear_eol()

text_coord_t get_curpos(){
    text_coord_t cpos;
    cpos.x = (curpos.x-X_OFS)/CHAR_WIDTH;
    cpos.y = (curpos.y-Y_OFS)/LINE_SPACING;
    return curpos;
} // get_cursor_pos()

void set_curpos(unsigned short x, unsigned short y){
    if (x>(CHAR_PER_LINE-1) || y>(LINE_PER_SCREEN-1))
        return;
    curpos.x=x*CHAR_WIDTH+X_OFS;
    curpos.y=y*LINE_SPACING+Y_OFS;
}//set_curpos()

void show_cursor(cursor_t shape){
}// show_cursor()
