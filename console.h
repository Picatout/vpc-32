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
 * File:   console.h
 * Description: text console for ntsc video output
 * Author: Jacques Desc�nes
 *
 * Created on 6 septembre 2013, 16:28
 */

#ifndef CONSOLE_H
#define	CONSOLE_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "hardware/font.h"
#include "hardware/ntsc.h"

#define CHAR_PER_LINE 53
#define LINE_PER_SCREEN 28
#define LINE_SPACING (CHAR_HEIGHT+1)
#define TAB_WIDTH 4

    typedef struct{
        unsigned short x;
        unsigned short y;
    } text_coord_t;
    
    typedef enum _CURSOR_SHAPE {CR_HIDE=0,CR_UNDER,CR_BLOCK} cursor_t;
    
// fonctions de l'interface
    void clear_screen(void); // efface l'�cran et positionne le curseur � {0,0}
    void clear_eol(void); // efface la fin de la ligne � partir du curseur.
    void scroll_up(void); // fait gliss� le texte une ligne vers le haut.
    void scroll_down(void); // fait gliss� le texte une ligne vers le bas.
    void scroll_left(unsigned short line, unsigned short count);// fait la ligne de count position vers la gauche
    void scroll_right(unsigned short line, unsigned short count); // fait gliss� la ligne de count position vers la droite.
    text_coord_t get_curpos(); // retourne position curseur texte.
    void set_curpos(unsigned short x, unsigned short y); // positionne le curseur
    void show_cursor(cursor_t shape); // affiche ou masque le curseur texte
    void put_char(char c); //affiche le caract�re � la position courante
    void print(const char *str); // imprime un cha�ne � la position courante
    void print_hex(unsigned int i, unsigned char width); // imprime un nombre hexad�cimal � la position courante
    void print_int(int number); // imprime un entier � la position courante.
    void cursor_right(void); // avance le curseur, retour � la ligne si n�cessaire.
    void cursor_left(void); // recule le curseur d'une position, va � la fin de la ligne pr�c�dente si n�cessaire.
    void cursor_down(void); // descend le curseur � la ligne suivante, scroll_up() si n�cessaire
    void cursor_up(void); // monte le curseur � la ligne pr�c�dente, scroll_down() si n�cessaire
    void set_tab_width(unsigned char width); // d�fini la largeur des colonnes de tabulation.
#ifdef	__cplusplus
}
#endif

#endif	/* CONSOLE_H */

