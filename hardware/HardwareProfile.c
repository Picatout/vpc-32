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
 * File:   HardwareProfile.h
 * Author: Jacques Desch�nes
 * Description: configuration hardware sp�cifique, assignation des p�riph�riques
 *     UART2 assign� � PB10,PB11
 * Created on 17 avril 2013, 14:41
 *
 */

#include "HardwareProfile.h"
#include <plib.h>

volatile unsigned int  sys_tick; // compteur pour les milli-secondes


void HardwareInit(){
   SYSTEMConfig(mGetSystemClock(), SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
   INTEnableSystemMultiVectoredInt();
   // activation du PROXIMITY TIMER pour les interruption inf�rieures � IPL=7
//   INTCONbits.TPC=0;
//   IPTMRCLR=0xFFFFFFFF;
//   INTCONbits.TPC=7;
//   IPTMR=CORE_TICK_RATE/2;
    // configure coretimer pour le sys_tick
#ifdef USE_CORE_TIMER
   OpenCoreTimer(CORE_TICK_RATE);
   mConfigIntCoreTimer((CT_INT_ON | CT_INT_PRIOR_1 | CT_INT_SUB_PRIOR_0));
#endif
   // d�sactivation de toutes les entr�e analogiques.
   ANSELBCLR=0xFFFFFFFF;
   ANSELACLR=0xFFFFFFFF;
    // configuration PB10 sortie et PB11 entr�e
   mPORTBSetBits(TX); // mettre � Vdd pour ne pas g�n�r� un faux start bit.
   mPORTBSetPinsDigitalOut(TX);      // sortie PB10(Tx), transmission UART2
   mPORTBSetPinsDigitalIn (RX);      // entr�e PB11(Rx), r�ception UART2
   // assignation des p�riph�riques aux broches
   // l'information se trouve dans pps.h
   PPSUnLock;                     // d�verrouillage configuration assignation p�riph�rique
   //clavier
   I2C1CONbits.DISSLW=1; // voir pic32mx1xxx/2xxx-errata.pdf rev. E, point 9
   RPA0R=0;  // pas de p�riph�rique sur RA0 (keyboard clock)
   RPA1R=0;  // pas de p�riph�rique sur RA1 (keyboard data)
   RPB3R=0; //  pas de p�riph�rique sur RB3 (status LED)
   TRISBCLR=STATUS_LED; // broche status LED en sortie
   _status_off();
   PPSOutput(4, RPB10, U2TX);  // assignation U2TX sur PB10, (groupe, pin, fonction)
   PPSInput (2, U2RX, RPB11);  // assignation U2RX sur PB11, (groupe, fonction, pin)
   PPSOutput(2,RPB5,OC2); // 5=OC2  sur PB5 sortie synchronistaiton ntsc
   PPSOutput(1,RPB4,OC1);  // 5=OC1 impulsion d�clenchant SPI , Fsync vid�o
   PPSOutput(3,RPB6,SDO1); // 3=SDO1 sortie SPI1 (vid�o)
   PPSInput(1,SS1,RPB7); // entr�e Fsync sur RPB7
   PPSLock;                       // reverrouille pour �viter assignation accidentelle.
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
    while (sys_tick<t0);
#else
    while (msec--)
        delay_us(1000);
#endif
} // delay_ms()

#ifdef USE_CORE_TIMER
  //d�claration du gestionnaire d'interruption
   void __ISR(_CORE_TIMER_VECTOR, IPL1SOFT)  CoreTimerHandler(void){
       sys_tick++;
       __asm__("addiu $v0, $zero, 0");
       __asm__("mtc0 $v0, $9");
       __asm__("addiu $v0,$zero,%0"::"I"(CORE_TICK_RATE));
       __asm__("mtc0 $v0, $11");
       mCTClearIntFlag();
   };
#endif

