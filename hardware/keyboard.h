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
 * File:   keyboard.h
 * Author: Jacques Desch�nes
 *
 * Created on 26 ao�t 2013, 10:44
 */

#ifndef KEYBOARD_H
#define	KEYBOARD_H

#ifdef	__cplusplus
extern "C" {
#endif
// structure pour la table de transcription des codes clavier
typedef struct scan2key{
	unsigned short code:8;
	unsigned short ascii:8;
}t_scan2key;


// commandes contr�le clavier
#define KBD_RESET 0xFF  // commande RAZ clavier
#define KBD_LED 0xED    // commande de contr�le des LEDS

#define XT_BIT	(1<<8)  // indicateur code �tendu
#define REL_BIT (1<<15) // indicateur touche rel�ch�e

// touches sp�ciales
#define CAPS_LOCK		0x58
#define NUM_LOCK		0x77
#define SCROLL_LOCK		0x7E
#define LSHIFT			0x12
#define LCTRL			0x14
#define LALT			0x11
#define KEY_ESC			0x76
#define F1			0x05
#define F2			0x06
#define F3			0x04
#define F4			0x0c
#define F5			0x03
#define F6			0x0b
#define F7			0x83
#define F8			0x0a
#define F9			0x01
#define F10			0x09
#define F11			0x78
#define F12			0x07
#define PRN			XT_BIT|0x12   // touche PrtSc enfonc�e 4 codes: 0xe012e07c, rel�ch�e 6 codes: 0xe0f07ce0f012
#define PAUSE			XT_BIT|0xe1   // touche Pause s�quence de 8 codes 0xe11477e1f014f077
#define UP_ARROW		XT_BIT|0x75
#define DOWN_ARROW		XT_BIT|0x72
#define LEFT_ARROW		XT_BIT|0x6B
#define RIGHT_ARROW		XT_BIT|0x74
#define INSERT			XT_BIT|0x70
#define HOME			XT_BIT|0x6c
#define	PGUP			XT_BIT|0x7d
#define PGDN			XT_BIT|0x7a
#define	DEL			XT_BIT|0x71
#define END			XT_BIT|0x69
#define KPDIV			XT_BIT|0x4a
#define KPMUL			0x7c
#define KPMINUS			0x7b
#define KPPLUS			0x79
#define KPENTER			XT_BIT|0x5a
#define KPDOT			0x71
#define KP0			0x70
#define KP1			0x69
#define KP2			0x72
#define KP3			0x7a
#define KP4			0x6b
#define KP5			0x73
#define KP6			0x74
#define KP7			0x6c
#define KP8			0x75
#define KP9			0x7d

// r�ponses du clavier aux commandes
#define KBD_ACK 0xFA // ACK confirmation envoy� par le clavier
#define KBD_RSND 0xFE // le clavier demande de renvoyer la commande
#define BAT_OK 0xAA // test clavier r�ussi (Basic Assurance Test)
#define BAT_ERROR 0xFC // �chec test clavier


#define KEY_REL 0xF0 // code touche rel�ch�e
#define XTD_KEY 0xE0 // code �tendu

#define F_ERROR 1 // erreur parit�e dans rx_flags
#define F_RCVD 2 // signal r�ception d'un octet du clavier dans rx_flags


#define F_SCROLL 1 // bit indicateur scroll_lock dans kbd_leds
#define F_NUM  2 // bit indicateur numlock dans kbd_leds
#define F_CAPS 4 // bit indicateur capslock dans kbd_leds
#define F_SHIFT 8  // bit indicateur shift dans kbd_leds

extern volatile unsigned char rx_flags, kbd_leds;


// keyboard API
int KeyboardInit();  // initialisation E/S et RAZ clavier
short GetScancode();  // obtient le code clavier en t�te de la file
short GetKey(short scancode);  // obtient la transcription du code en ASCII
int KbdSend(char cmd);  // envoie une commande au clavier
int SetKbdLeds(unsigned int leds_state); // contr�le l'�tat des LEDS du clavier

#ifdef	__cplusplus
}
#endif

#endif	/* KEYBOARD_H */

