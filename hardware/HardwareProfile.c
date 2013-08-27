/*
 * File:   HardwareProfile.h
 * Author: Jacques
 * Description: configuration hardware sp�cifique, assignation des p�riph�riques
 *     UART2 assign� � PB10,PB11
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

    // configuration PB10 sortie et PB11 entr�e
   mPORTBSetBits(TX); // mettre � Vdd pour ne pas g�n�r� un faux start bit.
   mPORTBSetPinsDigitalOut(TX);      // sortie PB10(Tx), transmission UART2
   mPORTBSetPinsDigitalIn (RX);      // entr�e PB11(Rx), r�ception UART2
   // assignation des p�riph�riques aux broches
   // l'information se trouve dans pps.h
   PPSUnLock;                     // d�verrouillage configuration assignation p�riph�rique
   PPSOutput(4, RPB10, U2TX);  // assignation U2TX sur PB10, (groupe, pin, fonction)
   PPSInput (2, U2RX, RPB11);  // assignation U2RX sur PB11, (groupe, fonction, pin)
   PPSOutput(2,RPB5,OC2); // 5=OC2  sur PB5 sortie synchronistaiton ntsc
   PPSOutput(1,RPB4,OC1);  // 5=OC1 impulsion d�clenchant SPI , Fsync vid�o
   PPSOutput(3,RPB6,SDO1); // 3=SDO1 sortie SPI1 (vid�o)
   PPSInput(1,SS1,RPB7); // entr�e Fsync sur RPB7
   PPSLock;                       // reverrouille pour �viter assignation accidentelle.
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

  //d�claration du gestionnaire d'interruption
   void __ISR(_CORE_TIMER_VECTOR, ipl2auto) CoreTimerHandler(void){
       sys_tick++;
       UpdateCoreTimer(CORE_TICK_RATE);
       mCTClearIntFlag();
   };
