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
#include "hardware/HardwareProfile.h"
#include "hardware/serial_comm/serial_comm.h"
#include "hardware/ps2_kbd/keyboard.h"


// indicateurs booléens
#define CUR_SHOW 1  // curseur actif
#define CUR_VIS  2  // curseur visible
#define INV_VID  4  // inverse vidéo


static unsigned short cx=0, cy=0;  // coordonnée courante du curseur texte en pixels.
static unsigned char tab_width=TAB_WIDTH;
static cursor_t cur_shape=CR_UNDER;
static unsigned short flags=0;

unsigned char comm_channel=LOCAL_CON;


void scroll_up(void){
    char *src, *dst;
    dst = (char*)video_bmp;
    src = (char*)video_bmp +(CHAR_HEIGHT)*HRES/8;
    memmove(dst,src,(LINE_PER_SCREEN-1)*CHAR_HEIGHT*HRES/8);
    dst= (char*)video_bmp+(CHAR_HEIGHT*(LINE_PER_SCREEN-1))*HRES/8;
    memset(dst,0,HRES/8*CHAR_HEIGHT);
}//scroll_up();

void scroll_down(void){
    char *src, *dst;
    src = (char*)video_bmp;
    dst = (char*)video_bmp+(CHAR_HEIGHT)*HRES/8;
    memmove(dst,src,(LINE_PER_SCREEN-1)*CHAR_HEIGHT*HRES/8);
    dst=(char*)video_bmp;
    memset(dst,0,HRES/8*CHAR_HEIGHT);
}//scroll_down()


void cursor_right(void){
    cx += CHAR_WIDTH;
    if (cx>=(CHAR_PER_LINE*CHAR_WIDTH)){
        cx = 0;
        cy += CHAR_HEIGHT;
        if (cy>=((LINE_PER_SCREEN-1)*CHAR_HEIGHT)){
            scroll_up();
            cy -= CHAR_HEIGHT;
        }
    }
} // cursor_right()

void cursor_left(void){
    if (cx>=(CHAR_WIDTH)){
        cx -= CHAR_WIDTH;
    }else{
        cx = (CHAR_PER_LINE-1);
        if (cy>=CHAR_HEIGHT){
            cy -= CHAR_HEIGHT;
        }else{
            scroll_down();
        }
    }
}// cursor_left()

void cursor_up(void){
    if (cy>=CHAR_HEIGHT){
        cy -= CHAR_HEIGHT;
    }else{
        scroll_down();
    }
}// cursor_up()

void cursor_down(void){
    if (cy<=((CHAR_HEIGHT*(LINE_PER_SCREEN-2)))){
        cy += CHAR_HEIGHT;
    }else{
        scroll_up();
    }
}//cursor_down()

void crlf(void){
    cx=0;
    if (cy==((LINE_PER_SCREEN-1)*CHAR_HEIGHT)){
        scroll_up();
    }else{
        cy += CHAR_HEIGHT;
    }
}//crlf()

void put_char(dev_t channel, char c){
    register int i,l,r,b,x,y;
    x=cx;
    y=cy;
    if (channel==LOCAL_CON){
        switch (c){
            case CR:
            case NL:
                crlf();
                break;
            case TAB:
                cx += (cx%tab_width);
                if (cx>=(CHAR_PER_LINE*CHAR_WIDTH)){
                    cx = 0;
                    if (cy==((LINE_PER_SCREEN-1)*CHAR_HEIGHT)){
                        scroll_up();
                    }else{
                        cy += CHAR_HEIGHT;
                    }
                }
                break;
            case '\b':
                cursor_left();
                break;
            default:
                if ((c<32) || (c>(FONT_SIZE+32))) break;
                c -=32;
                b=x>>5; // position index ligne video_bmp
                r=0;
                l=(32-CHAR_WIDTH)-(x&0x1f); // décalage  à l'intérieur de l'entier
                if (l<0){
                    r=-l;
                }
                for (i=0;i<8;i++){
                    if (r){
                        if (flags & INV_VID){
                            video_bmp[y][b] |= (0x3f>>r);
                            video_bmp[y][b] &=~(font6x8[c][i]>>r);
                            video_bmp[y][b+1] |= (0x3f<<32-r);
                            video_bmp[y][b+1] &= ~(font6x8[c][i]<<(32-r));
                        }else{
                            video_bmp[y][b] &= ~(0x3f>>r);
                            video_bmp[y][b] |= font6x8[c][i]>>r;
                            video_bmp[y][b+1] &= ~(0x3f<<32-r);
                            video_bmp[y][b+1] |= font6x8[c][i]<<(32-r);
                        }
                        y++;
                    } else{
                        if (flags & INV_VID){
                            video_bmp[y][b] |= (0x3f<<l);
                            video_bmp[y++][b] &=~(font6x8[c][i]<<l);
                        }else{
                            video_bmp[y][b] &= ~(0x3f<<l);
                            video_bmp[y++][b] |= font6x8[c][i]<<l;
                        }
                    }
                }
                cursor_right();
        }//switch(c)
    }else{
        UartPutch(channel,c);
    }
}//put_char()

void clear_screen(){
    memset(video_bmp,0,HRES/8*VRES);
    cx=0;
    cy=0;
} // clear_screen()

void print(dev_t channel, const char *text){
    if (channel==LOCAL_CON){
        while (*text){
            put_char(channel, *text++);
        }
    }else{
        UartPrint(channel,text);
    }
}// print()

void println(dev_t channel, const char *str){
    print(channel, str);
    if (channel==LOCAL_CON){
        crlf();
    }else{
        UartPutch(channel,'\r');
    }
}

void print_hex(dev_t channel, unsigned int hex, unsigned char width){
    char c[12], *d;
    int i;
    c[11]=0;
    d= &c[10];
    for(i=width;i>=0||hex;i--){
        *d=(hex%16);
        if (*d<10)
            *d += '0';
        else
            *d += 'A'-10;
        hex /=16;
        d--;
    }
    print(channel, ++d);
} // print_hex()

void print_int(dev_t channel, int number, unsigned short width){ // imprime entier,width inclus le signe
    int sign=0, i;
    char str[14], *d;
    str[13]=0;
    d=&str[12];
    if (width>13){width=13;}
    if (number<0){
        sign=1;
        number = -number;
    }
    //for (i=--width;i>=0||number;i--){
    i=width;
    while (number>0){
       *d--=(number%10)+'0';
        number /= 10;
        i--;
    }
    if (i==width){
        *d--='0';
        i--;
    }
    while (i>0){
        *d--=' ';
        i--;
    }
    if (sign){*d='-';}else{*d=' ';}
    
    print(channel, d);
}// print_int()

void set_tab_width(unsigned char width){
    tab_width=width;
}// set_tab_width()

void clear_eol(void){
    int x,y;
    x=cx;
    y=cy;
    while (cx<(CHAR_WIDTH*(CHAR_PER_LINE-2))){
        put_char(LOCAL_CON, 32);
    }
    put_char(LOCAL_CON, 32);
    cx=x;
    cy=y;
}// clear_eol()

text_coord_t get_curpos(){
    text_coord_t cpos;
    cpos.x = cx/CHAR_WIDTH;
    cpos.y = cy/CHAR_HEIGHT;
    return cpos;
} // get_cursor_pos()

void set_curpos(unsigned short x, unsigned short y){// {x,y} coordonnée caractère
    if (x>(CHAR_PER_LINE-1) || y>(LINE_PER_SCREEN-1))
        return;
    cx=x*CHAR_WIDTH;
    cy=y*CHAR_HEIGHT;
}//set_curpos()

void invert_char(void){// inverse vidéo du caractère à la position courante
    register int i,l,r,b,x,y;
    x=cx;
    y=cy;
    b=x>>5;
    r=0;
    l=(32-CHAR_WIDTH)-(x&0x1f);
    if (l<0){
        r=-l;
    }
    for (i=9;i;i--){
        if (r){
            video_bmp[y][b] ^= (0x3f>>r);
            video_bmp[y][b+1] ^= (0x3f<<32-r);
            y++;
        } else{
            video_bmp[y++][b] ^= (0x3f<<l);
        }
    }
}//invert_char()

static void toggle_underscore(void){
    register int l,r,b,x;
    x=cx;
    b=x>>5;
    r=0;
    l=(32-CHAR_WIDTH)-(x&0x1f);
    if (l<0){
        r=-l;
    }
    if (r){
        video_bmp[cy+CHAR_HEIGHT-1][b] ^= (0x3f>>r);
        video_bmp[cy+CHAR_HEIGHT-1][b+1] ^= (0x3f<<32-r);
    } else{
        video_bmp[cy+CHAR_HEIGHT-1][b] ^= (0x3f<<l);
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

unsigned short get_key(dev_t channel){ // lecture touche clavier, retourne 0 s'il n'y a pas de touche ou touche relâchée.
    char key;
    if (channel==LOCAL_CON){
        key=KbdKey();
    }else{
        key=UartGetch(STDIN);
        if (key==-1){
            key=0;
        }
    }
    return key;
}//get_key()

unsigned short wait_key(dev_t channel){ // attend qu'une touche soit enfoncée et retourne sa valeur.
    unsigned short key;
    unsigned int t0;
    t0=ticks()+500;
    if (channel==LOCAL_CON){
        while (!(key=get_key(channel))){
            if (ticks()==t0){
                if (flags & CUR_SHOW){
                    show_cursor(FALSE);
                }else{
                    show_cursor(TRUE);
                }
                t0=ticks()+500;
            }
        };
        show_cursor(FALSE);
    }else{
        key=UartWaitch(STDIN,0);
    }
    return key;
}//wait_key()

unsigned char readline(dev_t channel, unsigned char *ibuff,unsigned char max_char){ // lit une ligne au clavier, retourne la longueur de texte.
    unsigned char c=0, count=0;
    if (channel==LOCAL_CON){
        while ((c!='\r') && (count<=max_char)){
            c=wait_key(channel);
            if (c==CR){
                put_char(channel,'\r');
                break;
            }else if (c==BS){
                if (count){
                    ibuff--;
                    count--;
                    print(channel,"\b \b");
                }
            }else if (c >=32 && c<=127){
                *ibuff++=c;
                count++;
                put_char(channel, c);
            }
        }// while
        *ibuff=(char)0;
    }else{
        count=UartReadln(STDIN,ibuff,max_char);
    }
    return count;
} // readline()

void invert_video(unsigned char invert){
    if (invert){
        flags |= INV_VID;
    }else{
        flags &= ~INV_VID;
    }
}//invert_video()


void uppercase(char *str){// in situ uppercase
    while (*str){
        if (*str>='a' && *str<='z') *str-=32;
        str++;
    }
}