/*
 * Name: keyboard.c
 * Author: Jacques Desch�nes
 * Description:  interface avec clavier PS/2
 * Date: 2013-08-26
 */

#include <plib.h>
#include "hardwareProfile.h"
#include "keyboard.h"
#include "QWERTY.h"

#define KBD_CLK 1  // SET/CLR bit
#define KBD_DAT 2  // SET/CLR bit

volatile unsigned char kbd_queue[32]; // file circulaire pour les codes re�us du clavier.
volatile unsigned char head, tail; // t�te et queue de la file
volatile unsigned char  in_byte, bit_cnt, parity, rx_flags, kbd_leds;

int KeyboardInit(){ // initialisation E/S et RAZ clavier
    char c;

    head=0;
    tail=0;
    in_byte=0;
    bit_cnt=0;
    parity=0;
    rx_flags=0;
    kbd_leds=0;
    IFS0bits.INT4IF=0; // RAZ indicateur interruption
    INTCONbits.INT4EP=0; // interruption sur transition descendante
    IPC4bits.INT4IP=6; // priorit� 6
    IPC4bits.INT4IS=3;  // sous-priorit� 3.
    IEC0bits.INT4IE=1; // activation interruption externe 4 (KBD_CLK)
    if (!KbdSend(KBD_RESET)){
            return 0;
    }
    while ((rx_flags & F_ERROR+F_RCVD)==0); // attend r�sultat BAT
    if (rx_flags & F_ERROR)
            return 0;
    c=GetScancode();
    if (c!=BAT_OK)
            return 0;
    return 1;
} //KeyboardInit()

#define COMPLETED 1
#define RELEASE 2
#define EXTENDED 4
#define PRN_KEY 8
#define PAUSE_KEY 16

short GetScancode(){  // obtient le code clavier en t�te de la file
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
                            for (i=7;i;i--){     // �limine les 7 prochains caract�res
                                    while (head==tail);
                                    head++;
                                    head &= 31;
                            }
                            flags = COMPLETED+PAUSE_KEY;
                    }else if ((flags&EXTENDED)&& (code==0x12)){ // touche PRINT SCREEN enfonc�e
                            for (i=2;i;i--){ // �limine les 2 codes suivants
                                    while (head==tail);
                                    head++;
                            }
                            flags = COMPLETED+PRN_KEY;
                    }else if ((flags&EXTENDED)&& (code==0x7c)){ // touche PRINT SCREEN rel�ch�e
                            for (i=4;i;i--){ // �limine les 4 codes suivants
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
		code |= REL_BIT; // n�gatif pour touche rel�ch�e
	}
	if (flags & EXTENDED){
		code |= XT_BIT; //
	}
        IEC0bits.INT4IE=0; // section critique d�sactive interruption
	if (head==tail){
		rx_flags &= ~F_RCVD;
	}
        IEC0bits.INT4IE=1; // fin section critique
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
	}else if (rx_flags & F_SHIFT|F_CAPS){
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
		if (a>='a' && a<='z'){
			a -=32;
		}
	}
	return a|(scancode&0xff00);
} // GetKey()

int KbdSend(char cmd){  // envoie une commande au clavier
	bit_cnt=0;
	parity=0;
	IEC0bits.INT4IE=0; // d�sactive les interruptions sur KBD_CLK
	TRISASET = KBD_CLK; // MCU prend le contr�le de la ligne KBD_CLK
	PORTACLR = KBD_CLK;   	//  mis � 0  KBD_CLK
	delay_us(150); 	// d�lais minimum 100�sec marge 50�sec
	TRISASET = KBD_DAT;		// prend le contr�le de la ligne KBD_DAT
	PORTACLR = KBD_DAT;   	// met KBD_DAT � z�ro
	TRISACLR = KBD_CLK; 	// lib�re la ligne clock
	while (!(PORTAbits.RA0)); // attend que le clavier mette la ligne clock � 1
	while (bit_cnt<8){      // envoie des 8 bits, moins significatif en premier.
		while (PORTAbits.RA0);   // attend clock � 0
		if (cmd&1){
			PORTASET = KBD_DAT;
			parity++;
		}else{
			PORTACLR = KBD_DAT;
		}
		cmd >>= 1;
		while (!(PORTAbits.RA0)); // attend clock � 1
		bit_cnt++;				  // un bit de plus envoy�.
	}
	while (PORTAbits.RA0);   // attend clock � 0
	if (!(parity & 1)){
		PORTASET = KBD_DAT;
	}else{
		PORTACLR = KBD_DAT;
	}
	while (!(PORTAbits.RA0)); // attend clock � 1
	while (PORTAbits.RA0);   // attend clock � 0
	TRISACLR = KBD_DAT;  		// lib�re la ligne data
	while (!(PORTAbits.RA0)); // attend clock � 1
	while (PORTA & (KBD_DAT+KBD_CLK)); 	// attend que le clavier mette data et clock � 0
	while (!((PORTA & (KBD_DAT+KBD_CLK))==(KBD_DAT+KBD_CLK))); // attend que les 2 lignes reviennent � 1.
	bit_cnt=0;
        IFS0bits.INT4IF=0;
	IEC0bits.INT4IE = 1;
	while ((rx_flags & F_ERROR+F_RCVD)==0); // attend keyboard ACK
	if ((rx_flags & F_ERROR) || (GetScancode()!=KBD_ACK)){
		return 0;
	}else{
		return 1;
	}
} // KbdSend()

int SetKbdLeds(unsigned int leds_state){ // contr�le l'�tat des LEDS du clavier
	if (!KbdSend(KBD_LED)){
		return 0;
	}
	if (!KbdSend(leds_state)){
		return 0;
	}
	return 1;
} // SetKbdLeds()

void __ISR(_EXTERNAL_4_VECTOR,IPL6) kbd_clk_isr(void){
	switch (bit_cnt){
	case 0:   // start bit
		if (PORTAbits.RA1)
			return; // ce n'est pas un start bit
		parity=0;
		bit_cnt++;
		break;
	case 9:   // parit�e
		if (PORTAbits.RA1)
			parity++;
		if (!(parity & 1)){
			rx_flags |= F_ERROR;
                        IEC0bits.INT4IE=0; // d�sactive l'interruption
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
		if(PORTAbits.RA1){
			in_byte |=128;
			parity++;
		}
		bit_cnt++;
        mINT4ClearIntFlag();
	}
} // kbd_clk_isr()


