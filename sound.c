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

#include "hardware/HardwareProfile.h"

volatile unsigned char tone_on=0;
volatile unsigned int duration;

void tone(unsigned int freq, // fréquence en hertz
          unsigned int msec){ // durée en  millisecondes
    //configuration PWM sur OC1 puor délais vidéo, utilisation TIMER2
    OC3CONbits.OCM = 5; //mode pwm
    OC3CONbits.OCTSEL=1; // timer 3
    T3CON=0;
    PR3=(SYSCLK/8/freq)-1; // rapport cyclique 50%
    OC3RS=0;
    OC3R=SYSCLK/16/freq;
    OC3CONbits.ON=1;
    duration=msec;
    tone_on=1;
    T3CONbits.TCKPS=3;
    T3CONbits.ON=1;
} //tone();

