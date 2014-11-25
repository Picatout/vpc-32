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
 * File:   test-ntsc.c
 * Author: Jacques Desch�nes
 * Description: test g�n�ration signal vid�o NTSC sur PIC32MX150F128B.
 * Created on 20 ao�t 2013, 08:48
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/attribs.h>
#include <plib.h>
#include "../HardwareProfile.h"
#include "ntsc.h"

/*
 *  la fr�quence PBCLK influence la largeur des pixels vid�o. Comme PBCLK==SYSCLK
 *  on peut allonger les pixels en diminuant la fr�quence l�g�rement.
 *  pour une r�solution horizontale de 320 pixels et une fr�quence de 40Mhz le
 *  meilleur diviseur pour le g�n�rateur BGR du SPI est de 2 ce qui donne
 *  une fr�quence de pixel de  40e6/(2*(2+1)=6,67Mhz et la dur�e total d'une
 *  ligne vid�o est donc de 320/6,67e6= 48�Sec.
 *  En principe pour le standard NTSC on dispose de 52�Sec par ligne vid�o.
 *  En pratique on est plut�t � 51�Sec. Donc ici on a 3�Sec inutilis�es.
 *  en r�suidant SYSCLK � 37,5Mhz on a 51,2�Sec par ligne vid�o. Les pixels
 *  sont donc moins compress� horizontalement. Mais sur certains t�l�viseurs
 *  on peut perdre quelques pixels en bout de ligne.
 *  la constante SPI_DLY doit-�tre ajust�e en fonction de PBCLK pour que les
 *  320 pixels horizontals soient visibles.
 */

#define PWM_PERIOD (SYSCLK/15748)-1
#define HSYNC  (SYSCLK/212766)  // 4,7�Sec
#define FIRST_LINE 27   //premi�re ligne de scan visible
#define LAST_LINE  (FIRST_LINE+VRES)  // derni�re ligne de scan visible
#define BITCLK HRES * 1000000L/52 // 52�Sec c'est le temps que dure 1 ligne vid�o.
#if SYSCLK==40000000L
#define SPI_DLY HSYNC+90 // d�lais en d�but de ligne avant l'envoie du signal vid�o.
#else
#define SPI_DLY HSYNC+32 // d�lais en d�but de ligne avant l'envoie du signal vid�o.
#endif
#define _enable_video_out()  SPI1CONSET =(1<<15)
#define _disable_video_out() SPI1CONCLR =(1<<15)

unsigned int video_bmp[VRES][HRES/32]; // video bitmap 7168 octets
volatile unsigned int ln_cnt;
volatile unsigned int video;
volatile int *DmaSrc;



void VideoInit(void){
    //configuration PWM sur OC2 pour SYNC, utilisation TIMER2
    T2CON = 0;
    PR2=PWM_PERIOD;
    OC2CONbits.OCM = 5; // mode train d'impulsion
    OC2RS = 0; 
    OC2R = HSYNC;
    IFS0bits.T2IF=0;
    IEC0bits.T2IE=1;
    IPC2bits.T2IP=7;
    IPC2bits.T2IS=3;
    OC2CONbits.ON =1;  // activation
    T2CONbits.ON=1;  // activation
    //configuration PWM sur OC1 puor d�lais vid�o, utilisation TIMER2
    OC1CONbits.OCM = 5; //mode train d'impulsion
    OC1RS=0;
    OC1R=HSYNC+SPI_DLY;
    OC1CONbits.ON=1;
    // configuration SPI pour sortie vid�o
    // configure le DMA
    DmaChnOpen(0,0,DMA_OPEN_DEFAULT);
    DmaChnSetEventControl(0,DMA_EV_START_IRQ_EN|
                          DMA_EV_START_IRQ(_SPI1_TX_IRQ));
    DmaChnSetTxfer(0,(void *)DmaSrc,(void *)&SPI1BUF,HRES/8,4,4);
    // configuration du SPI canal 1
    SPI1CONbits.DISSDI=1; // SDI n'est pas utilis�.
    SPI1CONbits.FRMEN=1; // frame mode
    SPI1CONbits.FRMCNT=5; // 32 octets par frame.
    SPI1CONbits.FRMPOL=1; // sync sur transition montante
    SPI1CONbits.FRMSYNC=1; // synchronisation esclave
    SPI1CONbits.MSTEN=1; // SPI ma�tre
   // SPI1CONbits.SPIFE=1; // synchronisation sur clock premier bit.
    SPI1CONbits.MODE32=1; // mode 32 bits
    SPI1CONbits.STXISEL=1; // interruption sur TBE
    SpiChnSetBitRate(SPI_CHANNEL1, PBCLK, BITCLK);
    SPI1CONbits.ON=1;
}//init_video()



void __ISR(_TIMER_2_VECTOR,IPL7AUTO) tmr2_isr(void){
    _disable_video_out();
    ln_cnt++;
    switch (ln_cnt){
        case 1:  // d�but vsync
            OC2R=PWM_PERIOD-HSYNC;
            break;
        case 4: // fin vsync
            OC2R=HSYNC;
            break;
        case 262:  //fin du frame
            ln_cnt=0;
            break;
  
        case FIRST_LINE-1:
            video=1;
            DmaSrc=(void*)&video_bmp[0];
            break;
        case LAST_LINE:
            video=0;
            break;
        default:
            if (video){
                _enable_video_out();
                IFS1bits.SPI1TXIF=1;
                DCH0SSA=KVA_TO_PA((void *)DmaSrc);
                DmaSrc +=HRES/32;
                DCH0CON |=128; // remplace DmaChnEnable(0); r�duit le temps de latence
            }
    }//switch (ln_cnt)
    mT2ClearIntFlag();
}//tmr2_isr()

