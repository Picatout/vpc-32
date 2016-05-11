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
#include <stdint.h>
#include <stdbool.h>
#include "../HardwareProfile.h"
#include "keyboard.h"
#include "QWERTY.h"

#define KBD_CLK 1  // SET/CLR bit
#define KBD_DAT 2  // SET/CLR bit

#define _wait_key() while (ps2_head==ps2_tail)

#define PS2_QUEUE_SIZE (32)
#define KBD_QUEUE_SIZE (16)
static  uint8_t ps2_queue[PS2_QUEUE_SIZE]; // file circulaire pour les codes reçus du clavier.
static  uint8_t kbd_queue[KBD_QUEUE_SIZE]; // file codes ascii
volatile static unsigned char ps2_head=0, ps2_tail=0; // tête et queue de la file ps2_queue
volatile static unsigned char kbd_head=0, kbd_tail=0; // tête et queue file kbd_queue
volatile static unsigned char rx_flags=0, kbd_leds=0;
volatile unsigned short key_state=0; // état des touches d'alteration: shift, ctrl, alt,etc.

// envoie une commande au clavier
static int KbdSend(char cmd){
    register unsigned int dly;
    unsigned char bit_cnt,parity;
    unsigned int t0;
    bit_cnt=0;
	parity=0;
	IEC0CLR=_IEC0_INT4IE_MASK; // désactive les interruptions sur KBD_CLK
    rx_flags&=~F_ACK;
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
    IFS0CLR=_IFS0_INT4IF_MASK;
	IEC0SET = _IEC0_INT4IE_MASK; // réactivation interruption
    t0=ticks()+100;
    while(ticks()!=t0 && !(rx_flags&F_ACK));
    return rx_flags&F_ACK;
} // KbdSend()


static void KbdReset(void){
    unsigned int t0;
    rx_flags = 0;
    KbdSend(KBD_RESET);
    t0=ticks()+750;
    while (ticks()<t0 && !(rx_flags&F_BATOK));
}//KbdReset()

int SetKbdLeds(){ // contrôle l'état des LEDS du clavier
    return (KbdSend(KBD_LED) && KbdSend(kbd_leds));
} // SetKbdLeds()

void KeyboardInit(){ // initialisation E/S et RAZ clavier
    TRISASET=KBD_CLK|KBD_DAT;
    INTCONbits.INT4EP=0; // interruption sur transition descendante
    IPC4bits.INT4IP=6; // priorité 6
    IPC4bits.INT4IS=3;  // sous-priorité 3.
    while (!PORTAbits.RA0);
    //INT4 utilisée comme interruuption externe clavier ps2
    IFS0bits.INT4IF=0; // RAZ indicateur interruption
    IEC0SET = _IEC0_INT4IE_MASK; // activation interruption externe 4 (KBD_CLK)
    // timer pour processing ps2_queue
    KBD_TMR_CON=(1<<15)|(1<<4); //diviseur 1:8
    KBD_PR=(SYSCLK/800); // 40Mhz/800 période 10msec
    KBD_IPCbits.T1IP=2;
    KBD_IPCbits.T1IS=0;
    KBD_IFSbits.T1IF=0;
    KBD_IECbits.T1IE=1;
    KbdReset();
} //KeyboardInit()



// retourne la transcription du scancode en ASCII
unsigned char KbdKey(){
    unsigned char key;
    
    if (kbd_head==kbd_tail) return 0;
    key=kbd_queue[kbd_head++];
    kbd_head&=KBD_QUEUE_SIZE-1;
    return key;
} // GetKey()

static void key_release(short code){
    int i;
    
    switch (code){
        case 0x7c: //PrtSc
            for (i=3;i;i--){ // élimine les 3 codes suivants.
                _wait_key();
                ps2_head++;
                ps2_head &= PS2_QUEUE_SIZE-1;
            }
            break;
        case SC_LSHIFT:
            key_state &=~F_LSHIFT;
            break;
        case SC_RSHIFT:
            key_state &=~F_RSHIFT;
            break;
        case SC_LCTRL:
            key_state &=~F_LCTRL;
            break;
        case SC_RCTRL:
            key_state &=~F_RCTRL;
            break;
        case SC_RALT:
            key_state &=~F_ALTCHAR;
            break;
        case SC_LALT:
            key_state &=~F_LALT;
            break;
        case SC_NUMLOCK:
            kbd_leds ^= F_NUM;
            key_state ^= F_NUM;
            SetKbdLeds();
            break;
        case SC_CAPS:
            kbd_leds ^= F_CAPS;
            key_state ^= F_CAPS;
            SetKbdLeds();
            break;
        case SC_SCROLLOCK:
            kbd_leds ^= F_SCROLL;
            key_state ^= F_SCROLL;
            SetKbdLeds();
            break;
        default:;            
    }//switch
}//f

static short update_key_state(short code){
    int i;
    switch (code){ // les touches d'alteration sont traitées ici.
        case SC_LSHIFT:
            key_state |= F_LSHIFT;
            code=0;
            break;
        case SC_RSHIFT:
            key_state |= F_RSHIFT;
            code=0;
            break;
        case SC_LCTRL:
            key_state |= F_LCTRL;
            code=0;
            break;
        case SC_RCTRL:
            key_state |= F_RCTRL;
            code=0;
            break;
        case SC_RALT:
            key_state |= F_ALTCHAR;
            code=0;
            break;
        case SC_LALT:
            key_state |= F_LALT;
            code=0;
            break;
        case SC_NUMLOCK:
        case SC_CAPS:
        case SC_SCROLLOCK:
            code=0;
            break;
        case 0xE1: // PAUSE jette les 7 codes suivants
            for (i=7;i;i--){
                _wait_key();
                ps2_head++;
                ps2_head&=PS2_QUEUE_SIZE-1;
            }
            code=0;
            break;
        default:;
    }//switch(code)
    return code;
}//f

short search_code(const t_scan2key table[], short code){
    int i=0;
    
    while (table[i].code){
        if (table[i].code==code) return table[i].ascii;
        i++;
    }
    return 0;
}//f

void translate_code(uint16_t code){
    short ascii;
    short shift=key_state&F_SHIFT;

#define letter(a) ((a>='a') && (a<='z'))
#define xor(a,b) ((a)&& !(b) || !(a) && (b))
    
    if (code==SC_DEL && (key_state&(F_LCTRL|F_LALT))==(F_LCTRL|F_LALT)){
        __asm__("xor $0,$0,$2\nlui $2,0x9DC0\nj $2\n");
    }
	if (code & XT_BIT){
		ascii=search_code(qwerty_xt_char,code);
	}else if (shift){
        ascii=search_code(qwerty_shifted_key,code);
		if (!ascii){
            ascii=search_code(qwerty,code);
		} // if (!a)
	}else{
        ascii=search_code(qwerty,code);
	}
    if (letter(ascii) && xor((key_state&F_CAPS),(key_state&F_SHIFT))){
        ascii-=32;
    }else if (!ascii){
        ascii=code&255;
    }
    kbd_queue[kbd_tail++]=ascii;
    kbd_tail&=KBD_QUEUE_SIZE-1;
}//f

void __ISR(_TIMER_1_VECTOR,IPL2SOFT) kbd_timer_isr(void){
    uint16_t code;
    static unsigned short flags=0;

    while (ps2_head!=ps2_tail){
        code=ps2_queue[ps2_head++];
        ps2_head&=PS2_QUEUE_SIZE-1;
        switch (code){
            case XTD_KEY:
                flags |= XT_BIT;
                break;
            case KEY_REL:
                flags |= REL_BIT;
                break;
            default:
                if (flags & REL_BIT){
                    key_release(code|(flags&XT_BIT));
                }else{
                    code=update_key_state(code|flags);
                    if (code) translate_code(code);
                }
                flags=0;
        }//switch(code)
    }//while
    IFS0bits.T1IF=0;
}//_ISR
 
void __ISR(_EXTERNAL_4_VECTOR,IPL6SOFT) ps2_clk_isr(void){
    volatile  static unsigned char  in_byte=0,bit_cnt=0, parity=0;
    
    switch (bit_cnt){
	case 0:   // start bit
        rx_flags&=~(F_ERROR|F_RCVD);
        in_byte=0;
		if (!(PORTA & KBD_DAT)){
            parity=0;
            bit_cnt++;
        }
		break;
	case 9:   // paritée
		if (PORTA & KBD_DAT) parity++;
		if (!(parity & 1)){
			rx_flags |= F_ERROR;
		}
		bit_cnt++;
		break;
	case 10:  // stop bit
        switch(in_byte){
            case KBD_ACK:
                rx_flags|=F_ACK;
                break;
            case BAT_OK:
                rx_flags|=F_BATOK;
                break;
            case BAT_ERROR:
                rx_flags&=~F_BATOK;
                break;
            case KBD_RSND:
                rx_flags&=~F_ACK;
                break;
            default:
            if (!(rx_flags&F_ERROR)){
                ps2_queue[ps2_tail++]=in_byte;
                ps2_tail &=PS2_QUEUE_SIZE-1;
                rx_flags |= F_RCVD;
            }
        }
        bit_cnt=0;
		break;
	default:
		in_byte >>=1;
		if(PORTA & KBD_DAT){
			in_byte |=128;
			parity++;
		}
		bit_cnt++;
	}
    IFS0bits.INT4IF=0;
} // kbd_clk_isr()


