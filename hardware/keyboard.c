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
#include "hardwareProfile.h"
#include "keyboard.h"
#include "QWERTY.h"

#define KBD_CLK 1  // SET/CLR bit
#define KBD_DAT 2  // SET/CLR bit

volatile unsigned char kbd_queue[32]; // file circulaire pour les codes reçus du clavier.
volatile unsigned char head=0, tail=0; // tête et queue de la file
volatile unsigned char  in_byte=0, bit_cnt=0, parity=0, rx_flags=0, kbd_leds=0;

int KeyboardInit(){ // initialisation E/S et RAZ clavier
    unsigned char c;

    head=0;
    tail=0;
    in_byte=0;
    bit_cnt=0;
    parity=0;
    rx_flags=0;
    kbd_leds=0;
    TRISASET=KBD_CLK|KBD_DAT;
    INTCONbits.INT4EP=0; // interruption sur transition descendante
    IPC4bits.INT4IP=6; // priorité 4
    IPC4bits.INT4IS=3;  // sous-priorité 3.
    while (!PORTAbits.RA0);
    IFS0bits.INT4IF=0; // RAZ indicateur interruption
    IEC0SET = _IEC0_INT4IE_MASK; // activation interruption externe 4 (KBD_CLK)
    if (!KbdSend(KBD_RESET)){
            return -1;
    }
    while ((rx_flags & F_ERROR+F_RCVD)==0); // attend résultat BAT
    if (rx_flags & F_ERROR)
            return -2;
    c=GetScancode();
    if (c!=BAT_OK)
            return -c;
    return 1;
} //KeyboardInit()

#define COMPLETED 1
#define RELEASE 2
#define EXTENDED 4
#define PRN_KEY 8
#define PAUSE_KEY 16

short GetScancode(){  // obtient le code clavier en tête de la file
	unsigned int i, flags;
	short code;
	code = 0;
	flags=0;
	while (!(flags & COMPLETED)){
            if (head!=tail){
                code = kbd_queue[head];
                head++;
                head &= 31;
                if (code==XTD_KEY){
                        flags |= EXTENDED;
                }else if (code==KEY_REL){
                        flags |= RELEASE;
                }else if (code==0xE1){ // PAUSE
                        for (i=7;i;i--){     // élimine les 7 prochains caractères
                                while (head==tail);
                                head++;
                                head &= 31;
                        }
                        flags = COMPLETED+PAUSE_KEY;
                }else if ((flags&EXTENDED)&& (code==0x12)){ // touche PRINT SCREEN enfoncée
                        for (i=2;i;i--){ // élimine les 2 codes suivants
                                while (head==tail);
                                head++;
                        }
                        flags = COMPLETED+PRN_KEY;
                }else if ((flags&EXTENDED)&& (code==0x7c)){ // touche PRINT SCREEN relâchée
                        for (i=4;i;i--){ // élimine les 4 codes suivants
                                while (head==tail);
                                head++;
                        }
                        flags = COMPLETED+PRN_KEY+RELEASE;
                }else{
                        flags |=COMPLETED;
                }
                if (!(flags & COMPLETED)){
                        while (head==tail); // attend touche suivante
                }
            }else{
                    break;
            }
	}
	if (flags & PAUSE_KEY){
		code = PAUSE;
	}else if (flags & PRN_KEY){
		code = PRN;
	}
	if (flags & RELEASE){
		code |= REL_BIT; // négatif pour touche relâchée
	}
	if (flags & EXTENDED){
		code |= XT_BIT; //
	}
        IEC0bits.INT4IE=0; // section critique désactive interruption
	if (head==tail){
		rx_flags &= ~F_RCVD;
	}
        IEC0bits.INT4IE=1; // fin section critique réactive interruption
	return code;
}// GetScancode()

extern const t_scan2key translate[],alt_char[],xt_char[],shifted_key[];

short GetKey(short scancode){  // obtient la transcription du code en ASCII
	int a,i;
	a=0;
	if (scancode & XT_BIT){
		i=0;
		while (xt_char[i].code){
			if (xt_char[i].code==scancode){
				a=xt_char[i].ascii;
				break;
			}
			i++;
		} // while (xt_char[i].code)
	}else if (rx_flags & F_SHIFT){
		i=0;
		while (shifted_key[i].code){
			if (shifted_key[i].code==(scancode&0xff)){
				a=shifted_key[i].ascii;
				break;
			}
			i++;
		}// while (shifted_key.code)
		if (!a){
			i=0;
			while (translate[i].code){
				if (translate[i].code==(scancode&0xff)){
					a=translate[i].ascii;
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
		while (translate[i].code){
			if (translate[i].code==(scancode&0xff)){
				a=translate[i].ascii;
				break;
			}
			i++;
		}// while (translate.code)
		if (a>='a' && a<='z' && (rx_flags & F_CAPS)){
			a -=32;
		}
	}
	return a|(scancode&0xff00);
} // GetKey()

int KbdSend(char cmd){  // envoie une commande au clavier
    register unsigned int dly;
        bit_cnt=0;
	parity=0;
	IEC0CLR=_IEC0_INT4IE_MASK; // désactive les interruptions sur KBD_CLK
        TRISACLR = KBD_CLK; // MCU prend le contrôle de la ligne KBD_CLK
        LATACLR = KBD_CLK; //  mis à 0  KBD_CLK
        // délais minimum 100µsec
        for (dly=(100/3*CLK_PER_USEC);dly;dly--);
        TRISACLR = KBD_DAT;	// prend le contrôle de la ligne KBD_DAT
	LATACLR = KBD_DAT;   	// met KBD_DAT à zéro
	TRISASET = KBD_CLK; 	// libère la ligne clock
        while (!(PORTAbits.RA0)); // attend que la ligne revienne à 1
        while (bit_cnt<8){      // envoie les 8 bits, le moins significatif en premier.
		while (PORTAbits.RA0);   // attend clock à 0
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
        while ((rx_flags & F_ERROR+F_RCVD)==0); // attend keyboard ACK
	if ((rx_flags & F_ERROR) || (GetScancode()!=KBD_ACK)){
		return 0;
	}else{
		return 1;
	}
} // KbdSend()

int SetKbdLeds(unsigned int leds_state){ // contrôle l'état des LEDS du clavier
	if (!KbdSend(KBD_LED)){
		return 0;
	}
	if (!KbdSend(leds_state)){
		return 0;
	}
	return 1;
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


