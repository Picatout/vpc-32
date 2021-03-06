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
 * Name: keyboard.c
 * Author: Jacques Desch�nes
 * Description:  interface avec clavier PS/2
 * Date: 2013-08-26
 */

#include <plib.h>
#include "../HardwareProfile.h"
#include "keyboard.h"
#include "QWERTY.h"

#define KBD_CLK 1  // SET/CLR bit
#define KBD_DAT 2  // SET/CLR bit

#define _wait_key() while (head==tail)


static  unsigned char kbd_queue[32]; // file circulaire pour les codes re�us du clavier.
volatile  static unsigned char head=0, tail=0; // t�te et queue de la file
volatile  static unsigned char  in_byte=0, bit_cnt=0, parity=0;
volatile unsigned char rx_flags=0, kbd_leds=0;
volatile unsigned short key_state; // �tat des touches d'alteration: shift, ctrl, alt,etc.

#define FN_COUNT 32
const short fn_keys[FN_COUNT]={CAPS_LOCK,SCROLL_LOCK,NUM_LOCK,LSHIFT,RSHIFT,LCTRL,
RCTRL,LALT,RALT,F1,F2,F3,F4,F5,F6,F7,F8,F9,F10,F11,F12,PRN,PAUSE,UP_ARROW,DOWN_ARROW,
LEFT_ARROW,RIGHT_ARROW,INSERT,HOME,PGUP,PGDN,END
};

static int KbdReset(void);

int KeyboardInit(){ // initialisation E/S et RAZ clavier
    unsigned char c;
    unsigned int t0;
    t0=ticks()+750;
    head=0;
    tail=0;
    in_byte=0;
    bit_cnt=0;
    parity=0;
    rx_flags=0;
    kbd_leds=0;
    key_state=0;
    TRISASET=KBD_CLK|KBD_DAT;
    INTCONbits.INT4EP=0; // interruption sur transition descendante
    IPC4bits.INT4IP=6; // priorit� 4
    IPC4bits.INT4IS=3;  // sous-priorit� 3.
    while (!PORTAbits.RA0);
    IFS0bits.INT4IF=0; // RAZ indicateur interruption
    IEC0SET = _IEC0_INT4IE_MASK; // activation interruption externe 4 (KBD_CLK)
    while ((ticks()<t0) && ((rx_flags & F_ERROR)==0)){
        c=KbdScancode();
        if (c==BAT_OK){
            return 1;
        }
    }
    if (KbdReset())
        return 1;
    else
        return 0;
} //KeyboardInit()

static int KbdReset(void){
    unsigned int t0;
    short c;
    rx_flags = 0;
    KbdSend(KBD_RESET);
    t0=ticks()+750;
    c=0;
    while ((ticks()<t0) && ((rx_flags & F_ERROR)==0) && !c){
            c=KbdScancode();
            if (c==KBD_ACK){
                c=0;
            }
    }
    if ((rx_flags & F_ERROR) || (c!=BAT_OK))
        return 0;
    else
        return 1;
}//KbdReset()

typedef enum  _STATE {ST0, ST_EXT0, ST_RELEASE, ST_COMPLETED} state_t;

short KbdScancode(){  // obtient le code clavier en t�te de la file
	unsigned int i, flags;
        state_t state;
	short code;
        if (head==tail){
            if (rx_flags & F_ERROR){
                _status_on();
                delay_ms(400);
                _status_off();
                delay_ms(400);
                rx_flags &= ~F_ERROR;
                IEC0bits.INT4IE=1;
            }
            return 0; // tampon vide
        }
	code = 0;
	flags=0;
        state=ST0;
	while (state!=ST_COMPLETED){
            _wait_key();
            code=kbd_queue[head];
            head++;
            head &= 31;
            switch (state){
                case ST0:
                    switch (code){
                        case KEY_REL:
                            flags |= REL_BIT;
                            state = ST_RELEASE;
                            break;
                        case XTD_KEY:
                            flags |= XT_BIT;
                            state = ST_EXT0;
                            break;
                        case 0xE1:
                            for (i=7;i;i--){     // touche PAUSE �limine les 7 prochains caract�res
                                    _wait_key();
                                    head++;
                                    head &= 31;
                            }
                            code = PAUSE;
                        default:
                            state=ST_COMPLETED;
                    }//switch(code)
                    break;
                case ST_EXT0:
                    if (code==KEY_REL){
                        flags |= REL_BIT;
                        state = ST_RELEASE;
                    }else{
                        if ((flags & XT_BIT) && (code==0x12)){ // touche PrtSc enfonc�e.
                            for (i=2;i;i--){ // �limine les 2 codes suivants
                                _wait_key();
                                head++;
                                head &=31;
                            }
                            code = PRN;
                        }
                        state = ST_COMPLETED;
                    }
                    break;
                case ST_RELEASE:
                    if ((flags & XT_BIT) && (code==0x7c)){ //touche PrtSc rel�ch�e.
                        for (i=3;i;i--){ // �limine les 3 codes suivants.
                            _wait_key();
                            head++;
                            head &= 31;
                        }
                        code = PRN;
                    }
                    state=ST_COMPLETED;
                    break;
                default:;
            }//switch(state)
        }//while(state...)
        IEC0bits.INT4IE=0; // section critique d�sactive interruption
	if (head==tail){
		rx_flags &= ~F_RCVD;
	}
        IEC0bits.INT4IE=1; // fin section critique r�active interruption
        if ((code==KBD_ACK)||(code==KBD_RSND)||(code==BAT_OK)||(code==BAT_ERROR)){
            return code;
        }
        code |= flags;
        for (i=0;i<FN_COUNT;i++){
            if ((code&0x1ff)==fn_keys[i]){
                code |= FN_BIT;
                switch (code&0x1ff){ // les touches d'alteration sont trait�es ici.
                    case LSHIFT:
                        if (flags & REL_BIT){
                            key_state &=~F_LSHIFT;
                        }else{
                            key_state |= F_LSHIFT;
                        }
                        code=0;
                        break;
                    case RSHIFT:
                        if (flags & REL_BIT){
                            key_state &=~F_RSHIFT;
                        }else{
                            key_state |= F_RSHIFT;
                        }
                        code=0;
                        break;
                    case LCTRL:
                        if (flags & REL_BIT){
                            key_state &=~F_LCTRL;
                        }else{
                            key_state |= F_LCTRL;
                        }
                        code=0;
                        break;
                    case RCTRL:
                        if (flags & REL_BIT){
                            key_state &=~F_RCTRL;
                        }else{
                            key_state |= F_RCTRL;
                        }
                        code=0;
                        break;
                    case RALT:
                        if (flags & REL_BIT){
                            key_state &=~F_ALTCHAR;
                        }else{
                            key_state |= F_ALTCHAR;
                        }
                        code=0;
                        break;
                    case LALT:
                        if (flags & REL_BIT){
                            key_state &=~F_LALT;
                        }else{
                            key_state |= F_LALT;
                        }
                        code=0;
                        break;
                    case NUM_LOCK:
                        if (flags & REL_BIT){
                            kbd_leds ^= F_NUM;
                            key_state ^= F_NUM;
                            SetKbdLeds(kbd_leds);
                        }
                        code=0;
                        break;
                    case CAPS_LOCK:
                        if (flags & REL_BIT){
                            kbd_leds ^= F_CAPS;
                            key_state ^= F_CAPS;
                            SetKbdLeds(kbd_leds);
                        }
                        code=0;
                        break;
                    case SCROLL_LOCK:
                        if (flags & REL_BIT){
                            kbd_leds ^= F_SCROLL;
                            key_state ^= F_SCROLL;
                            SetKbdLeds(kbd_leds);
                        }
                        code=0;
                        break;
                    default:;
                }//switch(code)
                break;
            }// if
        }//for
        if ((code < 0) && !(code & FN_BIT)) // ne retourne pas les rel�chement de touche pour les touches caract�res.
            return 0;
        else
            return code;
}// GetScancode()


short KbdKey(short scancode){  // obtient la transcription du code en ASCII
	int a,i;
	a=0;
	if (scancode & XT_BIT){
		i=0;
		while (qwerty_xt_char[i].code){
			if (qwerty_xt_char[i].code==scancode){
				a=qwerty_xt_char[i].ascii;
				break;
			}
			i++;
		} // while (xt_char[i].code)
	}else if (key_state & F_SHIFT){
		i=0;
		while (qwerty_shifted_key[i].code){
			if (qwerty_shifted_key[i].code==(scancode&0xff)){
				a=qwerty_shifted_key[i].ascii;
				break;
			}
			i++;
		}// while (shifted_key.code)
		if (!a){
			i=0;
			while (qwerty[i].code){
				if (qwerty[i].code==(scancode&0xff)){
					a=qwerty[i].ascii;
					break;
				}
				i++;
			}// while (translate.code)
			if (a>='a' && a<='z'){
				a -=32;
			}
		} // if (!a)
	}else{
		i=0;
		while (qwerty[i].code){
			if (qwerty[i].code==(scancode&0xff)){
				a=qwerty[i].ascii;
				break;
			}
			i++;
		}// while (translate.code)
		if (a>='a' && a<='z' && (key_state & F_CAPS)){
			a -=32;
		}
	}
	return a|(scancode&0xff00);
} // GetKey()

void KbdSend(char cmd){  // envoie une commande au clavier
    register unsigned int dly;
    unsigned int t0;
        bit_cnt=0;
	parity=0;
	IEC0CLR=_IEC0_INT4IE_MASK; // d�sactive les interruptions sur KBD_CLK
        TRISACLR = KBD_CLK; // MCU prend le contr�le de la ligne KBD_CLK
        LATACLR = KBD_CLK; //  mis � 0  KBD_CLK
        // d�lais minimum 100�sec
        delay_us(100);
        TRISACLR = KBD_DAT;	// prend le contr�le de la ligne KBD_DAT
	LATACLR = KBD_DAT;   	// met KBD_DAT � z�ro
	TRISASET = KBD_CLK; 	// lib�re la ligne clock
        t0=ticks()+100;
        while (!(PORTAbits.RA0)); // attend que la ligne revienne � 1
        while (bit_cnt<8){      // envoie les 8 bits, le moins significatif en premier.
		while ((ticks()<t0) && PORTAbits.RA0);   // attend clock � 0
                if (ticks()>=t0) return; // pas de r�ponse du clavier. (pas de clavier!!)
                if (cmd&1){
			LATASET = KBD_DAT;
			parity++;
		}else{
			LATACLR = KBD_DAT;
		}
		cmd >>= 1;
		while (!(PORTAbits.RA0)); // attend clock � 1
		bit_cnt++;				  // un bit de plus envoy�.
	}
        while (PORTAbits.RA0);   // attend clock � 0
	if (!(parity & 1)){
		LATASET = KBD_DAT;
	}else{
		LATACLR = KBD_DAT;
	}
	while (!(PORTAbits.RA0)); // attend clock � 1
	while (PORTAbits.RA0);   // attend clock � 0
	TRISASET = KBD_DAT;  		// lib�re la ligne data
	while (!(PORTAbits.RA0)); // attend clock � 1
	while (PORTA & (KBD_DAT+KBD_CLK)); 	// attend que le clavier mette data et clock � 0
	while (!((PORTA & (KBD_DAT+KBD_CLK))==(KBD_DAT+KBD_CLK))); // attend que les 2 lignes reviennent � 1.
	bit_cnt=0;
        IFS0CLR=_IFS0_INT4IF_MASK;
	IEC0SET = _IEC0_INT4IE_MASK; // r�activation interruption
} // KbdSend()

int SetKbdLeds(unsigned char leds_state){ // contr�le l'�tat des LEDS du clavier
    unsigned int t0;
    short c;
    t0=ticks()+100;
    KbdSend(KBD_LED);
    c=0;
    while (!(ticks()==t0) && !(rx_flags & F_ERROR) && !c){
        c=KbdScancode();
    }
    if (c==KBD_ACK){
        t0=ticks()+100;
        KbdSend(leds_state);
        c=0;
        while (!(ticks()==t0) && !(rx_flags & F_ERROR) && !c){
            c=KbdScancode();
        }
    }
    if (c=KBD_ACK)
        return 1;
    else
        return 0;
} // SetKbdLeds()

void __ISR(_EXTERNAL_4_VECTOR,IPL6SOFT) kbd_clk_isr(void){
	switch (bit_cnt){
	case 0:   // start bit
		if (!(PORTA & KBD_DAT)){
                    parity=0;
                    bit_cnt++;
                }
		break;
	case 9:   // parit�e
		if (PORTA & KBD_DAT)
			parity++;
		if (!(parity & 1)){
			rx_flags |= F_ERROR;
                        IEC0CLR = _IEC0_INT4IE_MASK; // d�sactive l'interruption
		}
		bit_cnt++;
		break;
	case 10:  // stop bit
		kbd_queue[tail]=in_byte;
		tail++;
		tail &=31;
		bit_cnt=0;
		rx_flags |= F_RCVD;
		break;
	default:
		in_byte >>=1;
		if(PORTA & KBD_DAT){
			in_byte |=128;
			parity++;
		}
		bit_cnt++;
	}
        mINT4ClearIntFlag();
} // kbd_clk_isr()


