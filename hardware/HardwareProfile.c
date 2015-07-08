/*
* Copyright 2013,2014 Jacques Deschênes
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
 * File:   HardwareProfile.h
 * Author: Jacques Deschênes
 * Description: configuration hardware spécifique, assignation des périphériques
 *     UART2 assigné à PB10,PB11
 * Created on 17 avril 2013, 14:41
 *
 */

#include "HardwareProfile.h"
#include <plib.h>
#include "sound/sound.h"

volatile unsigned int  sys_tick; // compteur pour les milli-secondes


void HardwareInit(){
   SYSTEMConfig(mGetSystemClock(), SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
   INTEnableSystemMultiVectoredInt();
    // configure coretimer pour le sys_tick
#ifdef USE_CORE_TIMER
   OpenCoreTimer(CORE_TICK_RATE);
   mConfigIntCoreTimer((CT_INT_ON | CT_INT_PRIOR_1 | CT_INT_SUB_PRIOR_0));
#endif
   // désactivation de toutes les entrée analogiques.
   ANSELBCLR=0xFFFFFFFF;
   ANSELACLR=0xFFFFFFFF;
    // configuration PB10 sortie et PB11 entrée
   mPORTBSetBits(TX); // mettre à Vdd pour ne pas généré un faux start bit.
   mPORTBSetPinsDigitalOut(TX);      // sortie PB10(Tx), transmission UART2
   mPORTBSetPinsDigitalIn (RX);      // entrée PB11(Rx), réception UART2
   // assignation des périphériques aux broches
   // l'information se trouve dans pps.h
   PPSUnLock;                     // déverrouillage configuration assignation périphérique
   //clavier
   I2C1CONbits.DISSLW=1; // voir pic32mx1xxx/2xxx-errata.pdf rev. E, point 9
   RPA0R=0;  // pas de périphérique sur RA0 (keyboard clock)
   RPA1R=0;  // pas de périphérique sur RA1 (keyboard data)
   RPB1R=0;  // pas de périphérique sur RPB1 ~CS1 SRAM
   RPB2R=0;  // pas de périphérique sur RPB2 ~CS2 SDCARD
   RPB3R=0; //  pas de périphérique sur RB3 (status LED)
   RPB15R=0; // pad de périphérique sur RB15
   TRISBCLR=STATUS_LED; // broche status LED en sortie
   _status_off();
   PPSOutput(4, RPB10, U2TX);  // assignation U2TX sur PB10, (groupe, pin, fonction)
   PPSInput (2, U2RX, RPB11);  // assignation U2RX sur PB11, (groupe, fonction, pin)
   PPSOutput(2,RPB5,OC2); // 5=OC2  sur PB5 sortie synchronistaiton ntsc
   PPSOutput(1,RPB4,OC1);  // 5=OC1 impulsion déclenchant SPI , Fsync vidéo
   PPSOutput(3,RPB6,SDO1); // 3=SDO1 sortie SPI1 (vidéo)
   PPSOutput(4,RPB9,OC3); // OC3 sortie audio.
   PPSInput(1,SS1,RPB7); // entrée Fsync sur RPB7
<<<<<<< HEAD
   PPSInput(3,SDI2,RPA4); // entrée SDI pour carte SD
   PPSOutput(2,RPB8,SDO2); // sortie commande carte SD
   PPSOutput(2,RPB1,NULL); // sortie GPIO RB1, ~CS1  (SPIRAM)
   TRISBCLR=1|(1<<14); // RB0 et RB14 sortie GPIO
   PPSOutput(4,RPB0,NULL); // sortie GPIO RB0, snes_clock
   PPSOutput(4,RPB14,NULL); // sortie GPIO RB14, snes_latch
   PPSLock;                // reverrouille pour éviter assignation accidentelle.
=======
   PPSInput(3,SDI2,RPA4); // entrée SDI pour carte SD et SPIRAM
   PPSOutput(2,RPB8,SDO2); // sortie commande carte SD et SPIRAM
   PPSLock;                       // reverrouille pour éviter assignation accidentelle.
>>>>>>> 1abd89f007cb013a29040570370025f6a1d9c7ce
}

inline unsigned int ticks(void){
    return sys_tick;
} //ticks()

inline void delay_us(unsigned int usec){
    for (usec=usec*(CLK_PER_USEC/3);usec;usec--);
}//delay_us()

void delay_ms(unsigned int msec){
#ifdef USE_CORE_TIMER
    unsigned int t0;
    t0=sys_tick+msec;
    while (sys_tick!=t0);
#else
    while (msec--)
        delay_us(1000);
#endif
} // delay_ms()

// détermine la mémoire disponible
// sur le heap
// essais successifs par division binaire
unsigned free_heap(){
    unsigned toobig,size,tmp;
    void *ptr=NULL;

    size=RAM_SIZE;
    while (!ptr){
        ptr=malloc(size);
        if (!ptr){
            toobig=size;
            size >>=1;
        }
    }

    while (toobig-size>16){
        if (ptr) free(ptr);
        tmp=size;
        size+=(toobig-size)>>1;
        ptr=malloc(size);
        if (!ptr){
            toobig=size;
            size=tmp;
        }
    }
    if (ptr) free(ptr);
    return size;
}

#ifdef USE_CORE_TIMER
  //déclaration du gestionnaire d'interruption
   void __ISR(_CORE_TIMER_VECTOR, IPL1SOFT)  CoreTimerHandler(void){
       sys_tick++;
       __asm__("mfc0 $v0, $11");
       __asm__("addiu $v0,$v0,%0"::"I"(CORE_TICK_RATE));
       __asm__("mtc0 $v0, $11");
       mCTClearIntFlag();
       if ((fSound & TONE_ON) && !(--duration)){
           fSound &= ~TONE_ON;
           mTone_off();
       }
   };
#endif

