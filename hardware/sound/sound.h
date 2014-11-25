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
 * File:   sound.h
 * Author: Jacques Deschênes
 *
 * Created on 13 septembre 2013, 20:42
 */

#ifndef SOUND_H
#define	SOUND_H

#ifdef	__cplusplus
extern "C" {
#endif

#define TONE_ON  1
#define PLAY_TUNE 2

#define mTone_off() (OC3CONbits.ON=0)
#define mTone_on()  (OC3CONbits.ON=1)

extern volatile unsigned char fSound; // indicateurs booléins
extern volatile unsigned int duration;


void tone(unsigned int freq, unsigned int duration);
void tune(const unsigned int *buffer);



#ifdef	__cplusplus
}
#endif

#endif	/* SOUND_H */

