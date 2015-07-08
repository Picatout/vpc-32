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
 * File:   sound.c
 * Author: Jacques Deschênes
 *
 * Created on 13 septembre 2013, 20:42
 */

#include <plib.h>
#include "../HardwareProfile.h"
#include "sound.h"



volatile unsigned char fSound=0; // indicateurs booléins
volatile unsigned int duration;
volatile unsigned int *tones_list;



void tone(unsigned int freq, // fréquence en hertz
          unsigned int msec){ // durée en  millisecondes
    //configuration PWM sur OC1 puor délais vidéo, utilisation TIMER2
    OC3CONbits.OCM = 5; //mode pwm
    OC3CONbits.OCTSEL=1; // timer 3
    OC3RS=0;
    T3CON=0;
    T3CONbits.TCKPS=3;
    PR3=(SYSCLK/8/freq)-1; // rapport cyclique 50%
    OC3R=SYSCLK/16/freq;
    duration=msec;
    fSound |=TONE_ON;
    mTone_on();
    T3CONbits.ON=1;
} //tone();

// joue une mélodie en arrière plan
<<<<<<< HEAD:sound.c
void tune(unsigned int *buffer){
    tones_list=buffer;
=======
void tune(const unsigned int *buffer){
    tones_list=(unsigned *)buffer;
>>>>>>> 1abd89f007cb013a29040570370025f6a1d9c7ce:hardware/sound/sound.c
    if (*tones_list && *(tones_list+1)){
        fSound |= PLAY_TUNE;
        IPC3bits.T3IP=2;
        IPC3bits.T3IS=3;
        IFS0bits.T3IF=0;
        IEC0bits.T3IE=1;
        tone(*tones_list++,*tones_list++);
    }
}//tune()



void __ISR(_TIMER_3_VECTOR, IPL2SOFT)  T3Handler(void){
    unsigned int f,d;
       mT3ClearIntFlag();
       if (fSound==PLAY_TUNE){
           f=*tones_list++;
           d=*tones_list++;
           if (d){
                if (f){
                    tone(f,d);
                }else{
                    duration=(*tones_list);
                    fSound |= TONE_ON;
                }
           }else{
               fSound=0;
               IEC0bits.T3IE=0;
               T3CONbits.ON=0;
           } // if 
       }//if 
}// T3Handler
