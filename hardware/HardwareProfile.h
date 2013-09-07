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
 * File:   HardwareProfile.h
 * Author: Jacques Deschênes
 * Description: configuration hardware spécifique
 * Created on 17 avril 2013, 14:41
 */

#ifndef HARDWAREPROFILE_H
#define	HARDWAREPROFILE_H

#include <p32xxxx.h>

#define VPC_32

#define STDIO   UART2
#define STDOUT  STDIO
#define STDIN   STDIO
#define STDERR  STDIO

#define FREE_RAM 16384

#define TX BIT_10 // UART2 TX sur PB10
#define RX BIT_11 // UART2 RX sur PB11

#define SYSCLK  37500000L  // 37,5Mhz pour obtenir des pixels plus larges donne 51,2µSec pour 320pixels
#define PBCLK   SYSCLK

#define mGetSystemClock() (SYSCLK)
#define mGetPeripheralClock() (mGetSystemClock())
#define CORE_TICK_RATE (mGetSystemClock()/2/1000) // system tick 1msec
#define CLK_PER_USEC (SYSCLK/1000000L)

#define STATUS_LED  BIT_3
#define _status_on()  (PORTB |=STATUS_LED)
#define _status_off() (PORTB &=~STATUS_LED)
#define _write_on() _status_on()
#define _write_off() _status_off()
#define _read_on() _status_on()
#define _read_off() _status_off()


#define USE_CORE_TIMER  // metter en commentaire si on n'utilise pas le core timer


void HardwareInit();
#ifdef USE_CORE_TIMER
unsigned int ticks(void);
#endif
void delay_us(unsigned int usec);
void delay_ms(unsigned int msec);

#endif	/* HARDWAREPROFILE_H */

