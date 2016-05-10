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
 * tranlation.h
 *
 *  Created on: 2013-07-24
 *      Author: Jacques Deschênes
 *      Description: table de tanscription des codes clavier vers code ASCII pour clavier QWERTY
 *
 *------------------------------------------------
 *  Références:
 *  	http://retired.beyondlogic.org/keyboard/keybrd.htm#1
 *  	http://www.computer-engineering.org/
 *  	http://en.wikipedia.org/wiki/PS/2_keyboard
 *  	http://wiki.osdev.org/PS2_Keyboard
 *------------------------------------------------
 */

#ifndef QWERTY_H_
#define QWERTY_H_

#include "keyboard.h"

//touches virtuelles
#define VK_BACK 8
#define VK_TAB 9
#define VK_ESC 27
#define VK_ENTER '\r'
#define VK_F1 128
#define VK_F2 129
#define VK_F3 130
#define VK_F4 131
#define VK_F5 132
#define VK_F6 133
#define VK_F7 134
#define VK_F8 135
#define VK_F9 136
#define VK_F10 138
#define VK_F11 139
#define VK_F12 140
#define VK_UP  141
#define VK_DOWN 142
#define VK_LEFT 143
#define VK_RIGHT 144
#define VK_HOME 145
#define VK_END  146
#define VK_PGUP 147
#define VK_PGDN 148
#define VK_INSERT 149
#define VK_DELETE 150
#define VK_PAUSE 151


extern const t_scan2key qwerty[];
extern const t_scan2key qwerty_shifted_key[];
extern const t_scan2key qwerty_alt_char[];
extern const t_scan2key qwerty_xt_char[];



#endif /* QWERTY_H_ */
