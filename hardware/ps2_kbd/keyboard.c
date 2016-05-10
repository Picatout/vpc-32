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
 * Name: keyboard.c
 * Author: Jacques Deschênes
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


static  unsigned char kbd_queue[32]; // file circulaire pour les codes reçus du clavier.
volatile  static unsigned char head=0, tail=0; // tête et queue de la file
volatile  static unsigned char  in_byte=0, bit_cnt=0, parity=0;
volatile unsigned char rx_flags=0, kbd_leds=0;
volatile unsigned short key_state; // état des touches d'alteration: shift, ctrl, alt,etc.


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
    IPC4bits.INT4IP=6; // priorité 4
    IPC4bits.INT4IS=3;  // sous-priorité 3.
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

static void key_release(short code){
    int i;
    
    switch (code){
        case 0x7c: //PrtSc
            for (i=3;i;i--){ // élimine les 3 codes suivants.
                _wait_key();
                head++;
                head &= 31;
            }
            break;
        case LSHIFT:
            key_state &=~F_LSHIFT;
            break;
        case RSHIFT:
            key_state &=~F_RSHIFT;
            break;
        case LCTRL:
            key_state &=~F_LCTRL;
            break;
        case RCTRL:
            key_state &=~F_RCTRL;
            break;
        case RALT:
            key_state &=~F_ALTCHAR;
            break;
        case LALT:
            key_state &=~F_LALT;
            break;
        case NUM_LOCK:
            kbd_leds ^= F_NUM;
            key_state ^= F_NUM;
            SetKbdLeds(kbd_leds);
            break;
        case CAPS_LOCK:
            kbd_leds ^= F_CAPS;
            key_state ^= F_CAPS;
            SetKbdLeds(kbd_leds);
            break;
        case SCROLL_LOCK:
            kbd_leds ^= F_SCROLL;
            key_state ^= F_SCROLL;
            SetKbdLeds(kbd_leds);
            break;
        default:;            
    }//switch
}//f

static short update_key_state(short code){
    int i;
    switch (code){ // les touches d'alteration sont traitées ici.
        case LSHIFT:
            key_state |= F_LSHIFT;
            code=0;
            break;
        case RSHIFT:
            key_state |= F_RSHIFT;
            code=0;
            break;
        case LCTRL:
            key_state |= F_LCTRL;
            code=0;
            break;
        case RCTRL:
            key_state |= F_RCTRL;
            code=0;
            break;
        case RALT:
            key_state |= F_ALTCHAR;
            code=0;
            break;
        case LALT:
            key_state |= F_LALT;
            code=0;
            break;
        case NUM_LOCK:
        case CAPS_LOCK:
        case SCROLL_LOCK:
            code=0;
            break;
        case 0xE1: // PAUSE jette les 7 codes suivants
            for (i=0;i<7;i++){
                _wait_key();
                head++;
                head&=0x31;
            }
            code|=XT_BIT;
            break;
        default:;
    }//switch(code)
    return code;
}//f

enum STATE {ST0, ST_XTDKEY, ST_RELKEY, ST_COMPLETED};
//retourne le scancode avec les bits REL_BIT et XT_BIT
short KbdScancode(){  // obtient le code clavier en tête de la file
	unsigned int i;
	short code,flag;
    int state;
    


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
    }//if
    state=ST0;
    while (state!=ST_COMPLETED){
        _wait_key();
        code=kbd_queue[head++];
        head &= 31;
        switch(state){
            case ST0:
                if (code==XTD_KEY){
                    flag=XT_BIT;
                    state=ST_XTDKEY;
                }else if (code==KEY_REL){
                    state=ST_RELKEY;
                }else{
                    state=ST_COMPLETED;
                }
                break;
            case ST_XTDKEY:
                if (code==KEY_REL){
                    state=ST_RELKEY;
                }else{
                    code|=flag;
                    state=ST_COMPLETED;
                }
                break;
            case ST_RELKEY:
                key_release(code|flag);
                code=0;
                state=ST_COMPLETED;
                break;
        }//switch
    }//while
    IEC0bits.INT4IE=0; // section critique désactive interruption
	if (head==tail){
		rx_flags &= ~F_RCVD;
	}
    IEC0bits.INT4IE=1; // fin section critique réactive interruption
    code=update_key_state(code);
    return code;
}// GetScancode()

// retourne la transcription du scancode en ASCII
short KbdKey(short scancode){
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
	return a|(scancode&0xff00); // on conserve les indicateurs REL_BIT et XT_BIT
} // GetKey()

// envoie une commande au clavier
void KbdSend(char cmd){
    register unsigned int dly;
    unsigned int t0;
        bit_cnt=0;
	parity=0;
	IEC0CLR=_IEC0_INT4IE_MASK; // désactive les interruptions sur KBD_CLK
        TRISACLR = KBD_CLK; // MCU prend le contrôle de la ligne KBD_CLK
        LATACLR = KBD_CLK; //  mis à 0  KBD_CLK
        // délais minimum 100µsec
        delay_us(100);
        TRISACLR = KBD_DAT;	// prend le contrôle de la ligne KBD_DAT
	LATACLR = KBD_DAT;   	// met KBD_DAT à zéro
	TRISASET = KBD_CLK; 	// libère la ligne clock
        t0=ticks()+100;
        while (!(PORTAbits.RA0)); // attend que la ligne revienne à 1
        while (bit_cnt<8){      // envoie les 8 bits, le moins significatif en premier.
		while ((ticks()<t0) && PORTAbits.RA0);   // attend clock à 0
                if (ticks()>=t0) return; // pas de réponse du clavier. (pas de clavier!!)
                if (cmd&1){
			LATASET = KBD_DAT;
			parity++;
		}else{
			LATACLR = KBD_DAT;
		}
		cmd >>= 1;
		while (!(PORTAbits.RA0)); // attend clock à 1
		bit_cnt++;				  // un bit de plus envoyé.
	}
        while (PORTAbits.RA0);   // attend clock à 0
	if (!(parity & 1)){
		LATASET = KBD_DAT;
	}else{
		LATACLR = KBD_DAT;
	}
	while (!(PORTAbits.RA0)); // attend clock à 1
	while (PORTAbits.RA0);   // attend clock à 0
	TRISASET = KBD_DAT;  		// libère la ligne data
	while (!(PORTAbits.RA0)); // attend clock à 1
	while (PORTA & (KBD_DAT+KBD_CLK)); 	// attend que le clavier mette data et clock à 0
	while (!((PORTA & (KBD_DAT+KBD_CLK))==(KBD_DAT+KBD_CLK))); // attend que les 2 lignes reviennent à 1.
	bit_cnt=0;
    IFS0CLR=_IFS0_INT4IF_MASK;
	IEC0SET = _IEC0_INT4IE_MASK; // réactivation interruption
} // KbdSend()

int SetKbdLeds(unsigned char leds_state){ // contrôle l'état des LEDS du clavier
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
	case 9:   // paritée
		if (PORTA & KBD_DAT)
			parity++;
		if (!(parity & 1)){
			rx_flags |= F_ERROR;
                        IEC0CLR = _IEC0_INT4IE_MASK; // désactive l'interruption
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


