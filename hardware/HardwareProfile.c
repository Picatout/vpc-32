/*
 * File:   HardwareProfile.h
 * Author: Jacques
 * Description: configuration hardware spécifique, assignation des périphériques
 *     UART2 assigné à PB10,PB11
 * Created on 17 avril 2013, 14:41
 *
 */

#include "HardwareProfile.h"
#include <plib.h>

void HardwareInit(){
    SYSTEMConfig(mGetSystemClock(), SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
    // configure coretimer pour le sys_tick
   OpenCoreTimer(CORE_TICK_RATE);
   mConfigIntCoreTimer((CT_INT_ON | CT_INT_PRIOR_2 | CT_INT_SUB_PRIOR_0));
   INTEnableSystemMultiVectoredInt();

    // configuration PB10 sortie et PB11 entrée
   mPORTBSetBits(TX); // mettre à Vdd pour ne pas généré un faux start bit.
   mPORTBSetPinsDigitalOut(TX);      // sortie PB10(Tx), transmission UART2
   mPORTBSetPinsDigitalIn (RX);      // entrée PB11(Rx), réception UART2
   // assignation des périphériques aux broches
   // l'information se trouve dans pps.h
   PPSUnLock;                     // déverrouillage configuration assignation périphérique
   PPSOutput(4, RPB10, U2TX);  // assignation U2TX sur PB10, (groupe, pin, fonction)
   PPSInput (2, U2RX, RPB11);  // assignation U2RX sur PB11, (groupe, fonction, pin)
   PPSOutput(2,RPB5,OC2); // 5=OC2  sur PB5 sortie synchronistaiton ntsc
   PPSOutput(1,RPB4,OC1);  // 5=OC1 impulsion déclenchant SPI , Fsync vidéo
   PPSOutput(3,RPB6,SDO1); // 3=SDO1 sortie SPI1 (vidéo)
   PPSInput(1,SS1,RPB7); // entrée Fsync sur RPB7
   PPSLock;                       // reverrouille pour éviter assignation accidentelle.
}

inline long ticks(void){
    return sys_tick;
} //ticks()

void delay_ms(int msec){
    long t0;
    t0=ticks()+msec;
    while (ticks<t0);
} // delay_ms()

inline void delay_us(int usec){
    for(usec=(usec*CLK_PER_USEC)>>1;usec;usec--);
} // delay_us()

  //déclaration du gestionnaire d'interruption
   void __ISR(_CORE_TIMER_VECTOR, ipl2auto) CoreTimerHandler(void){
       sys_tick++;
       UpdateCoreTimer(CORE_TICK_RATE);
       mCTClearIntFlag();
   };
