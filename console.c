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

#define X_OFS  ((HRES%CWIDTH)/2)  // offset vidéo position curseur x
#define Y_OFS  ((VRES%CHEIGHT)/2)  // offset vidéo position curseur y

// indicateurs booléens
#define CUR_SHOW 1  // curseur actif
#define CUR_VIS  2  // curseur visible


static unsigned short cx=X_OFS, cy=Y_OFS;  // coordonnée courante du curseur texte en pixels.
static unsigned char tab_width=TAB_WIDTH;
static cursor_t cur_shape=CR_UNDER;
static unsigned short flags=0;


void scroll_up(void){
    char *src, *dst;
    dst = (char*)video_bmp+Y_OFS*HRES/8;
    src = (char*)video_bmp +(CHEIGHT+Y_OFS)*HRES/8;
    memmove(dst,src,(LINE_PER_SCREEN-1)*CHEIGHT*HRES/8);
    dst= (char*)video_bmp+(CHEIGHT*(LINE_PER_SCREEN-1)+Y_OFS)*HRES/8;
    memset(dst,0,HRES/8*CHEIGHT);
}//scroll_up();

void scroll_down(void){
    char *src, *dst;
    src = (char*)video_bmp+Y_OFS*HRES/8;
    dst = (char*)video_bmp+(CHEIGHT+Y_OFS)*HRES/8;
    memmove(dst,src,(LINE_PER_SCREEN-1)*CHEIGHT*HRES/8);
    dst=(char*)video_bmp+Y_OFS*HRES/8;
    memset(dst,0,HRES/8*CHEIGHT);
}//scroll_down()

void cursor_right(void){
    cx += CWIDTH;
    if (cx>(CHAR_PER_LINE*CWIDTH)){
        cx = X_OFS;
        cy += CHEIGHT;
        if (cy>(LINE_PER_SCREEN*CHEIGHT)){
            scroll_up();
            cy -= CHEIGHT;
        }
    }
} // cursor_right()

void cursor_left(void){
    if (cx>=(X_OFS+CWIDTH)){
        cx -= CWIDTH;
    }else{
        cx = X_OFS+CWIDTH*(CHAR_PER_LINE-1);
        if (cy>=(Y_OFS+CHEIGHT)){
            cy -= CHEIGHT;
        }else{
            scroll_down();
        }
    }
}// cursor_left()

void cursor_up(void){
    if (cy>=(Y_OFS+CHEIGHT)){
        cy -= CHEIGHT;
    }else{
        scroll_down();
    }
}// cursor_up()

void cursor_down(void){
    if (cy<=(Y_OFS+(CHEIGHT*(LINE_PER_SCREEN-2)))){
        cy += CHEIGHT;
    }else{
        scroll_up();
    }
}//cursor_down()

void put_char(char c){
    register int i,l,r,b,x,y;
    x=cx;
    y=cy;
    switch (c){
        case CR:
            cx=X_OFS;
            cy += CHAR_HEIGHT+1;
            break;
        case TAB:
            cx += (cx%tab_width);
            if (cx>(CHAR_PER_LINE*CWIDTH)){
                cx = X_OFS;
                cy += CHEIGHT;
                if (cy>(LINE_PER_SCREEN*CHEIGHT)){
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
    cx=X_OFS;
    cy=Y_OFS;
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
    print(++d);
} // print_hex()

void print_decimal(int number, unsigned short width){ // imprime entier,width inclus le signe
    int sign=0, i;
    char str[14], *d;
    str[13]=0;
    d=&str[12];
    if (number<0){sign=1;}
    for (i=--width;i;i--){
        *d--=(number%10)+'0';
        number /= 10;
    }
    if (sign){*d='-';}else{*d=' ';}
    print(d);
}// print_decimal()

void set_tab_width(unsigned char width){
    tab_width=width;
}// set_tab_width()

void clear_eol(void){
    int i;
    for (i=CHEIGHT;i;i--){
        memset(&video_bmp[cy+CHEIGHT-i][cx],0,HRES/8);
    }
}// clear_eol()

text_coord_t get_curpos(){
    text_coord_t cpos;
    cpos.x = (cx-X_OFS)/CWIDTH;
    cpos.y = (cy-Y_OFS)/CHEIGHT;
    return cpos;
} // get_cursor_pos()

void set_curpos(unsigned short x, unsigned short y){// {x,y} coordonnée caractère
    if (x>(CHAR_PER_LINE-1) || y>(LINE_PER_SCREEN-1))
        return;
    cx=x*CWIDTH+X_OFS;
    cy=y*CHEIGHT+Y_OFS;
}//set_curpos()

void invert_char(void){// inverse vidéo du caractère à la position courante
    register int i,l,r,b,x,y;
    x=cx;
    y=cy;
    b=x>>5;
    r=0;
    l=27-(x&0x1f);
    if (l<0){
        r=-l;
    }
    for (i=8;i;i--){
        if (r){
            video_bmp[y][b] ^= (0x1f>>r);
            video_bmp[y][b+1] ^= (0x1f<<32-r);
            y++;
        } else{
            video_bmp[y++][b] ^= (0x1f<<l);
        }
    }
}//invert_char()

static void toggle_underscore(void){
    register int l,r,b,x;
    x=cx;
    b=x>>5;
    r=0;
    l=27-(x&0x1f);
    if (l<0){
        r=-l;
    }
    if (r){
        video_bmp[cy+CHEIGHT-1][b] ^= (0x1f>>r);
        video_bmp[cy+CHEIGHT-1][b+1] ^= (0x1f<<32-r);
    } else{
        video_bmp[cy+CHEIGHT-1][b] ^= (0x1f<<l);
    }
}//toggle_underscore()

void show_cursor(BOOL show){
    if (show){
        flags |= CUR_SHOW;
        flags ^= CUR_VIS;
        if (cur_shape==CR_BLOCK){
            invert_char();
        }else{
            toggle_underscore();
        }
    }else{
        if (flags & CUR_VIS){
            if (cur_shape==CR_BLOCK){
                invert_char();
            }else{
                toggle_underscore();
            }
            flags &= ~CUR_VIS;
        }
        flags &= ~CUR_SHOW;
    }

}// show_cursor()

void set_cursor(cursor_t shape){
    if (flags & CUR_VIS){
        show_cursor(FALSE);
        cur_shape=shape;
        show_cursor(TRUE);
    }else{
        cur_shape=shape;
    }
}// set_cursor()
