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
 * File:   test-ntsc.c
 * Author: Jacques Deschênes
 * Description: test génération signal vidéo NTSC sur PIC32MX150F128B.
 * Created on 20 août 2013, 08:48
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/attribs.h>
#include <plib.h>
#include "../HardwareProfile.h"
#include "ntsc.h"

/*
 *  la fréquence PBCLK influence la largeur des pixels vidéo. Comme PBCLK==SYSCLK
 *  on peut allonger les pixels en diminuant la fréquence légèrement.
 *  pour une résolution horizontale de 320 pixels et une fréquence de 40Mhz le
 *  meilleur diviseur pour le générateur BGR du SPI est de 2 ce qui donne
 *  une fréquence de pixel de  40e6/(2*(2+1)=6,67Mhz et la durée total d'une
 *  ligne vidéo est donc de 320/6,67e6= 48µSec.
 *  En principe pour le standard NTSC on dispose de 52µSec par ligne vidéo.
 *  En pratique on est plutôt à 51µSec. Donc ici on a 3µSec inutilisées.
 *  en résuidant SYSCLK à 37,5Mhz on a 51,2µSec par ligne vidéo. Les pixels
 *  sont donc moins compressé horizontalement. Mais sur certains téléviseurs
 *  on peut perdre quelques pixels en bout de ligne.
 *  la constante SPI_DLY doit-être ajustée en fonction de PBCLK pour que les
 *  320 pixels horizontals soient visibles.
 */

#define PWM_PERIOD (SYSCLK/15748)-1
#define HSYNC  (SYSCLK/212766)  // 4,7µSec
#define FIRST_LINE 27   //première ligne de scan visible
#define LAST_LINE  (FIRST_LINE+VRES)  // dernière ligne de scan visible
#define BITCLK HRES * 1000000L/52 // 52µSec c'est le temps que dure 1 ligne vidéo.
#if SYSCLK==40000000L
#define SPI_DLY HSYNC+90 // délais en début de ligne avant l'envoie du signal vidéo.
#else
#define SPI_DLY HSYNC+32 // délais en début de ligne avant l'envoie du signal vidéo.
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
    //configuration PWM sur OC1 puor délais vidéo, utilisation TIMER2
    OC1CONbits.OCM = 5; //mode train d'impulsion
    OC1RS=0;
    OC1R=HSYNC+SPI_DLY;
    OC1CONbits.ON=1;
    // configuration SPI pour sortie vidéo
    // configure le DMA
    DmaChnOpen(0,0,DMA_OPEN_DEFAULT);
    DmaChnSetEventControl(0,DMA_EV_START_IRQ_EN|
                          DMA_EV_START_IRQ(_SPI1_TX_IRQ));
    DmaChnSetTxfer(0,(void *)DmaSrc,(void *)&SPI1BUF,HRES/8,4,4);
    // configuration du SPI canal 1
    SPI1CONbits.DISSDI=1; // SDI n'est pas utilisé.
    SPI1CONbits.FRMEN=1; // frame mode
    SPI1CONbits.FRMCNT=5; // 32 octets par frame.
    SPI1CONbits.FRMPOL=1; // sync sur transition montante
    SPI1CONbits.FRMSYNC=1; // synchronisation esclave
    SPI1CONbits.MSTEN=1; // SPI maître
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
        case 1:  // début vsync
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
                DCH0CON |=128; // remplace DmaChnEnable(0); réduit le temps de latence
            }
    }//switch (ln_cnt)
    mT2ClearIntFlag();
}//tmr2_isr()

