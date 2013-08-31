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
 * tranlation.h
 *
 *  Created on: 2013-07-24
 *      Author: Jacques Desch�nes
 *      Description: table de tanscription des codes clavier vers code ASCII pour clavier QWERTY
 *
 *------------------------------------------------
 *  R�f�rences:
 *  	http://retired.beyondlogic.org/keyboard/keybrd.htm#1
 *  	http://www.computer-engineering.org/
 *  	http://en.wikipedia.org/wiki/PS/2_keyboard
 *  	http://wiki.osdev.org/PS2_Keyboard
 *------------------------------------------------
 */

#ifndef QWERTY_H_
#define QWERTY_H_

#include "keyboard.h"

const t_scan2key translate[]={  // table de correspondance codes clavier -> ASCII (clavier QWERTY)
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
		{0x0d,'\t'},
		{0x54,'['},
		{0x5b,']'},
		{0x4c,';'},
		{0x41,','},
		{0x49,'.'},
		{0x4a,'/'},
		{0x66,8}, // BACKSPACE
		{0x0d,9}, // TAB
		{0x5a,'\r'}, // CR
		{0x76,27}, // ESC
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
		{KPENTER,'\r'},
		{KPDOT,'.'},
		{0,0}
};

const t_scan2key shifted_key[]={
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

const t_scan2key alt_char[]={
		{0,0}
};

const t_scan2key xt_char[]={
		{0x4a,'/'}, // keypad '/'
		{0x5a,'\r'},// keypad ENTER
		{0,0}
};

#endif /* QWERTY_H_ */
