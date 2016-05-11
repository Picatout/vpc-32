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
 * File:   keyboard.h
 * Author: Jacques Deschênes
 *
 * Created on 26 août 2013, 10:44
 */

#ifndef KEYBOARD_H
#define	KEYBOARD_H

#ifdef	__cplusplus
extern "C" {
#endif
// structure pour la table de transcription des codes clavier
typedef struct scan2key{
	 short code;
	 unsigned char ascii;
}t_scan2key;


// commandes contrôle clavier
#define KBD_RESET 0xFF  // commande RAZ clavier
#define KBD_LED 0xED    // commande de contrôle des LEDS

#define XT_BIT	(1<<8)  // indicateur code étendu
#define REL_BIT (1<<15) // indicateur touche relâchée

// scancodes clavier QWERTY 
//REF: http://www.computer-engineering.org/ps2keyboard/scancodes2.html
#define SC_DIG_0        0x45
#define SC_DIG_1        0x16
#define SC_DIG_2        0x1E
#define SC_DIG_3        0x26
#define SC_DIG_4        0x25
#define SC_DIG_5        0x2E
#define SC_DIG_6        0x36
#define SC_DIG_7        0x3D
#define SC_DIG_8        0x3E
#define SC_DIG_9        0x46
#define SC_LTR_A        0x1C
#define SC_LTR_B        0x32
#define SC_LTR_C        0x21
#define SC_LTR_D        0x23
#define SC_LTR_E        0x24
#define SC_LTR_F        0x2B
#define SC_LTR_G        0x34
#define SC_LTR_H        0x33
#define SC_LTR_I        0x43
#define SC_LTR_J        0x3B
#define SC_LTR_K        0x42
#define SC_LTR_L        0x4B
#define SC_LTR_M        0x3A
#define SC_LTR_N        0x31
#define SC_LTR_O        0x44
#define SC_LTR_P        0x4D
#define SC_LTR_Q        0x15
#define SC_LTR_R        0x2D
#define SC_LTR_S        0x1B
#define SC_LTR_T        0x2C
#define SC_LTR_U        0x3C
#define SC_LTR_V        0x2A
#define SC_LTR_W        0x1D
#define SC_LTR_X        0x22
#define SC_LTR_Y        0x35
#define SC_LTR_Z        0x1A
#define SC_A_ACUTE      0x0E
#define SC_SEMICOL      0x4C
#define SC_PERIOD       0x49
#define SC_COMMA        0x41
#define SC_SQUOTE       0x52
#define SC_DASH         0x4E
#define SC_EQUAL        0x55
#define SC_BKSLASH      0x5D
#define SC_SLASH        0X4A
#define SC_SPACE        0X29
#define SC_TAB          0X0D
#define SC_LBRACKET     0x54
#define SC_RBRACKET     0x5B
#define SC_ENTER        0x5A
#define SC_CAPS 		0x58
#define SC_NUMLOCK		0x77
#define SC_SCROLLOCK    0x7E
#define SC_LSHIFT		0x12
#define SC_RSHIFT       0x59
#define SC_LCTRL		0x14
#define SC_RCTRL        (XT_BIT|0x14)
#define SC_LALT         0x11
#define SC_RALT         (XT_BIT|0x11)
#define SC_BKSP         0x66
#define SC_ESC  	    0x76
#define SC_F1			0x05
#define SC_F2			0x06
#define SC_F3			0x04
#define SC_F4			0x0c
#define SC_F5			0x03
#define SC_F6			0x0b
#define SC_F7			0x83
#define SC_F8			0x0a
#define SC_F9			0x01
#define SC_F10			0x09
#define SC_F11			0x78
#define SC_F12			0x07
#define SC_PRN			(XT_BIT|0x12) // touche PrtSc enfoncée 4 codes: 0xe0,0x12,0xe0,0x7c 
                                      // relâchée 6 codes: 0xe0,0xf0,0x7c,0xe0,0xf0,0x12
#define SC_PAUSE		0xe1 // touche Pause séquence de 8 codes 0xe1,0x14,0x77,0xe1,0xf0,0x14,0xf0,0x77
#define SC_UP_ARROW		(XT_BIT|0x75)
#define SC_DOWN_ARROW	(XT_BIT|0x72)
#define SC_LEFT_ARROW	(XT_BIT|0x6B)
#define SC_RIGHT_ARROW	(XT_BIT|0x74)
#define SC_INSERT		(XT_BIT|0x70)
#define SC_HOME			(XT_BIT|0x6c)
#define	SC_PGUP			(XT_BIT|0x7d)
#define SC_PGDN			(XT_BIT|0x7a)
#define	SC_DEL			(XT_BIT|0x71)
#define SC_END			(XT_BIT|0x69)
#define SC_KPDIV		(XT_BIT|0x4a)
#define SC_KPMUL		0x7c
#define SC_KPMINUS		0x7b
#define SC_KPPLUS		0x79
#define SC_KPENTER		(XT_BIT|0x5a)
#define SC_KPDOT		0x71
#define SC_KP0			0x70
#define SC_KP1			0x69
#define SC_KP2			0x72
#define SC_KP3			0x7a
#define SC_KP4			0x6b
#define SC_KP5			0x73
#define SC_KP6			0x74
#define SC_KP7			0x6c
#define SC_KP8			0x75
#define SC_KP9			0x7d

// réponses du clavier aux commandes
#define KBD_ACK   0xFA   // ACK confirmation envoyé par le clavier
#define KBD_RSND  0xFE  // le clavier demande de renvoyer la commande
#define BAT_OK    0xAA    // test clavier réussi (Basic Assurance Test)
#define BAT_ERROR 0xFC // échec test clavier


#define KEY_REL 0xF0 // code touche relâchée
#define XTD_KEY 0xE0 // code étendu

// code réception clavier ps2
#define F_ERROR 1 // erreur paritée dans rx_flags
#define F_RCVD 2 // signal réception d'un octet du clavier dans rx_flags
#define F_BATOK 4 // succès BAT
#define F_ACK   8 // le clavier à reconnu la commande

#define F_SCROLL 1 // bit indicateur scroll_lock dans kbd_leds
#define F_NUM  2 // bit indicateur numlock dans kbd_leds
#define F_CAPS 4 // bit indicateur capslock dans kbd_leds
#define F_LSHIFT 8  // bit indicateur shift dans kbd_leds
#define F_RSHIFT 16 // bit indicateur touche contrôle 
#define F_SHIFT (F_LSHIFT|F_RSHIFT)
#define F_LCTRL 32  // bit indiateur touche alt enfoncée
#define F_RCTRL 64
#define F_CTRL (F_LCTRL|F_RCTRL)
#define F_LALT 128
#define F_ALTCHAR  256
#define F_ALT (F_LALT|F_ALTCHAR)

extern volatile unsigned short key_state; // état des touches d'alteration: shift, ctrl ,alt


// keyboard API
void KeyboardInit();  // initialisation E/S et RAZ clavier
unsigned char KbdKey();  // obtient la transcription du code en ASCII

#ifdef	__cplusplus
}
#endif

#endif	/* KEYBOARD_H */

