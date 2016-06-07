/*
* Copyright 2013,2014,2015 Jacques Deschênes
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
 * File:   console.h
 * Description: text console for ntsc video output
 * Author: Jacques Descênes
 *
 * Created on 6 septembre 2013, 16:28
 */

#ifndef CONSOLE_H
#define	CONSOLE_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <GenericTypeDefs.h>
#include "font.h"
#include "hardware/tvout/ntsc.h"
#include "hardware/HardwareProfile.h"
    
#define LINE_PER_SCREEN ((int)VRES/CHAR_HEIGHT)
#define CHAR_PER_LINE ((int)(HRES/CHAR_WIDTH))
#define TAB_WIDTH 4

#define ESC 27
#define CR  13
#define LF  10
#define TAB 9
#define SPACE 32
#define FF 12

typedef  char dev_t;

    typedef struct{
        unsigned short x;
        unsigned short y;
    } text_coord_t;
    
    typedef enum _CURSOR_SHAPE {CR_UNDER=0,CR_BLOCK} cursor_t;

extern unsigned char comm_channel;

// fonctions de l'interface
    void clear_screen(void); // efface l'écran et positionne le curseur à {0,0}
    void clear_eol(void); // efface la fin de la ligne à partir du curseur.
    void scroll_up(void); // fait glissé le texte une ligne vers le haut.
    void scroll_down(void); // fait glissé le texte une ligne vers le bas.
    text_coord_t get_curpos(); // retourne position curseur texte.
    void set_curpos(unsigned short x, unsigned short y); // positionne le curseur
    void put_char(dev_t channel, char c); //affiche le caractère à la position courante
    void print(dev_t channel, const char *str); // imprime un chaîne à la position courante
    void print_hex(dev_t channel, unsigned int i, unsigned char width); // imprime un nombre hexadécimal à la position courante
    void print_int(dev_t channel, int number, unsigned short width); // imprime un entier à la position courante.
    void cursor_right(void); // avance le curseur, retour à la ligne si nécessaire.
    void cursor_left(void); // recule le curseur d'une position, va à la fin de la ligne précédente si nécessaire.
    void cursor_down(void); // descend le curseur à la ligne suivante, scroll_up() si nécessaire
    void cursor_up(void); // monte le curseur à la ligne précédente, scroll_down() si nécessaire
    void set_tab_width(unsigned char width); // défini la largeur des colonnes de tabulation.
    void invert_char(void); // inverse les pixel du caractère à la position du curseur.
    void show_cursor(BOOL); // affiche ou masque le curseur texte
    void set_cursor(cursor_t shape); // défini la  forme du curseur
    void crlf(void); // déplace le curseur à la ligne suivante
    unsigned char get_key(dev_t channel); // lecture touches clavier
    unsigned char wait_key(dev_t channel); // attend qu'une touche soit enfoncée.
    unsigned char readline(dev_t channel, unsigned char *ibuff,unsigned char max_char); // lit une ligne au clavier, retourne la longueur de texte.
    void invert_video(unsigned char invert); // inverse vidéo des caractèrs noir/blanc
    void println(dev_t channel, const char *str);
    void uppercase(char *str);// in situ uppercase
#ifdef	__cplusplus
}
#endif

#endif	/* CONSOLE_H */

