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
		{SC_LTR_A,'a'},
		{SC_LTR_B,'b'},
		{SC_LTR_C,'c'},
		{SC_LTR_D,'d'},
		{SC_LTR_E,'e'},
		{SC_LTR_F,'f'},
		{SC_LTR_G,'g'},
		{SC_LTR_H,'h'},
		{SC_LTR_I,'i'},
		{SC_LTR_J,'j'},
		{SC_LTR_K,'k'},
		{SC_LTR_L,'l'},
		{SC_LTR_M,'m'},
		{SC_LTR_N,'n'},
		{SC_LTR_O,'o'},
		{SC_LTR_P,'p'},
		{SC_LTR_Q,'q'},
		{SC_LTR_R,'r'},
		{SC_LTR_S,'s'},
		{SC_LTR_T,'t'},
		{SC_LTR_U,'u'},
		{SC_LTR_V,'v'},
		{SC_LTR_W,'w'},
		{SC_LTR_X,'x'},
		{SC_LTR_Y,'y'},
		{SC_LTR_Z,'z'},
		{SC_DIG_0,'0'},
		{SC_DIG_1,'1'},
		{SC_DIG_2,'2'},
		{SC_DIG_3,'3'},
		{SC_DIG_4,'4'},
		{SC_DIG_5,'5'},
		{SC_DIG_6,'6'},
		{SC_DIG_7,'7'},
		{SC_DIG_8,'8'},
		{SC_DIG_9,'9'},
		{SC_SPACE,' '},
		{SC_DASH,'-'},
		{SC_EQUAL,'='},
		{SC_A_ACUTE,'`'},
		{SC_TAB,VK_TAB},
		{SC_LBRACKET,'['},
		{SC_RBRACKET,']'},
		{SC_SEMICOL,';'},
		{SC_COMMA,','},
		{SC_PERIOD,'.'},
		{SC_SLASH,'/'},
		{SC_BKSP,VK_BACK}, // BACKSPACE
		{SC_ENTER,VK_ENTER}, // CR
		{SC_ESC,VK_ESC}, // ESC
		{SC_KP0,'0'},
		{SC_KP1,'1'},
		{SC_KP2,'2'},
		{SC_KP3,'3'},
		{SC_KP4,'4'},
		{SC_KP5,'5'},
		{SC_KP6,'6'},
		{SC_KP7,'7'},
		{SC_KP8,'8'},
		{SC_KP9,'9'},
        {SC_BKSLASH,'\\'},
		{SC_KPDIV,'/'},
		{SC_KPMUL,'*'},
		{SC_KPMINUS,'-'},
		{SC_KPPLUS,'+'},
		{SC_KPDOT,'.'},
        {SC_SQUOTE,'\''},
        {SC_F1,VK_F1},
        {SC_F2,VK_F2},
        {SC_F3,VK_F3},
        {SC_F4,VK_F4},
        {SC_F5,VK_F5},
        {SC_F6,VK_F6},
        {SC_F7,VK_F7},
        {SC_F8,VK_F8},
        {SC_F9,VK_F9},
        {SC_F10,VK_F10},
        {SC_F11,VK_F11},
        {SC_F12,VK_F12},
		{0,0}
};

const t_scan2key qwerty_shifted_key[]={
		{SC_A_ACUTE,'~'},
		{SC_DIG_1,'!'},
		{SC_DIG_2,'@'},
		{SC_DIG_3,'#'},
		{SC_DIG_4,'$'},
		{SC_DIG_5,'%'},
		{SC_DIG_6,'^'},
		{SC_DIG_7,'&'},
		{SC_DIG_8,'*'},
		{SC_DIG_9,'('},
		{SC_DIG_0,')'},
		{SC_DASH,'_'},
		{SC_EQUAL,'+'},
		{SC_BKSLASH,'|'},
		{SC_SEMICOL,':'},
		{SC_SQUOTE,'"'},
		{SC_COMMA,'<'},
		{SC_PERIOD,'>'},
		{SC_SLASH,'?'},
        {SC_LBRACKET,'{'},
        {SC_RBRACKET,'}'},
		{0,0}
};

const t_scan2key qwerty_alt_char[]={
		{0,0}
};

const t_scan2key qwerty_xt_char[]={
		{SC_KPDIV,'/'}, // keypad '/'
		{SC_KPENTER,VK_ENTER},// keypad ENTER
		{SC_UP_ARROW,VK_UP},
        {SC_DOWN_ARROW,VK_DOWN},
        {SC_LEFT_ARROW,VK_LEFT},
        {SC_RIGHT_ARROW,VK_RIGHT},
        {SC_PGUP, VK_PGUP},
        {SC_PGDN, VK_PGDN},
        {SC_INSERT,VK_INSERT},
        {SC_HOME,VK_HOME},
        {SC_END,VK_END},
        {SC_DEL,VK_DELETE},
        {0,0}
};

