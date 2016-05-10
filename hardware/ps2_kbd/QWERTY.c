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
 * Fichier: QWERTY.c
 * Description: table de transcription scancode clavier pour clavier QWERTY
 * Auteur: Jacques Deschênes
 * Date: 2013-09-08
 */

#include "QWERTY.h"

const t_scan2key qwerty[]={  // table de correspondance codes clavier -> ASCII (clavier QWERTY)
		{0x1c,'a'},
		{0x32,'b'},
		{0x21,'c'},
		{0x23,'d'},
		{0x24,'e'},
		{0x2b,'f'},
		{0x34,'g'},
		{0x33,'h'},
		{0x43,'i'},
		{0x3b,'j'},
		{0x42,'k'},
		{0x4b,'l'},
		{0x3a,'m'},
		{0x31,'n'},
		{0x44,'o'},
		{0x4d,'p'},
		{0x15,'q'},
		{0x2d,'r'},
		{0x1b,'s'},
		{0x2c,'t'},
		{0x3c,'u'},
		{0x2a,'v'},
		{0x1d,'w'},
		{0x22,'x'},
		{0x35,'y'},
		{0x1a,'z'},
		{0x45,'0'},
		{0x16,'1'},
		{0x1e,'2'},
		{0x26,'3'},
		{0x25,'4'},
		{0x2e,'5'},
		{0x36,'6'},
		{0x3d,'7'},
		{0x3e,'8'},
		{0x46,'9'},
		{0x29,' '},
		{0x4e,'-'},
		{0x55,'='},
		{0x0e,'`'},
		{0x0d,VK_TAB},
		{0x54,'['},
		{0x5b,']'},
		{0x4c,';'},
		{0x41,','},
		{0x49,'.'},
		{0x4a,'/'},
		{0x66,VK_BACK}, // BACKSPACE
		{0x5a,VK_ENTER}, // CR
		{0x76,VK_ESC}, // ESC
		{KP0,'0'},
		{KP1,'1'},
		{KP2,'2'},
		{KP3,'3'},
		{KP4,'4'},
		{KP5,'5'},
		{KP6,'6'},
		{KP7,'7'},
		{KP8,'8'},
		{KP9,'9'},
		{KPDIV,'/'},
		{KPMUL,'*'},
		{KPMINUS,'-'},
		{KPPLUS,'+'},
		{KPDOT,'.'},
        {F1,VK_F1},
        {F2,VK_F2},
        {F3,VK_F3},
        {F4,VK_F4},
        {F5,VK_F5},
        {F6,VK_F6},
        {F7,VK_F7},
        {F8,VK_F8},
        {F9,VK_F9},
        {F10,VK_F10},
        {F11,VK_F11},
        {F12,VK_F12},
		{0,0}
};

const t_scan2key qwerty_shifted_key[]={
		{0x0e,'~'},
		{0x16,'!'},
		{0x1e,'@'},
		{0x26,'#'},
		{0x25,'$'},
		{0x2e,'%'},
		{0x36,'^'},
		{0x3d,'&'},
		{0x3e,'*'},
		{0x46,'('},
		{0x45,')'},
		{0x4e,'_'},
		{0x55,'+'},
		{0x5d,'\\'},
		{0x4c,':'},
		{0x52,'"'},
		{0x41,'<'},
		{0x49,'>'},
		{0x4a,'?'},
		{0,0}
};

const t_scan2key qwerty_alt_char[]={
		{0,0}
};

const t_scan2key qwerty_xt_char[]={
		{0x4a,'/'}, // keypad '/'
		{KPENTER,VK_ENTER},// keypad ENTER
		{UP_ARROW,VK_UP},
        {DOWN_ARROW,VK_DOWN},
        {LEFT_ARROW,VK_LEFT},
        {RIGHT_ARROW,VK_RIGHT},
        {PGUP, VK_PGUP},
        {PGDN, VK_PGDN},
        {INSERT,VK_INSERT},
        {HOME,VK_HOME},
        {END,VK_END},
        {DEL,VK_DELETE},
        {0,0}
};

