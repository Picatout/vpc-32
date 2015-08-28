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

#define RAM_SIZE (65536)
//#define FREE_RAM (49152)

#define LOCAL_CON 0  // console vidéo locale
#define SERIAL_CON STDIO // console rs-232


#define TX BIT_10 // UART2 TX sur PB10
#define RX BIT_11 // UART2 RX sur PB11

//#define SYSCLK  37500000L  // 37,5Mhz pour obtenir des pixels plus larges donne 51,2µSec pour 320pixels
#define SYSCLK 40000000L
#define PBCLK   SYSCLK

#define mGetSystemClock() (SYSCLK)
#define mGetInstructionClock()	(mGetSystemClock()/1)	//
#define mGetPeripheralClock() (mGetSystemClock())
#define CORE_TICK_RATE (mGetSystemClock()/2/1000) // system tick 1msec
#define CLK_PER_USEC (SYSCLK/1000000L)

#define FLASH_BEG			 	0x9D000000
#define FLASH_END			 	0x9D000000+BMXPFMSZ-1
#define RAM_BEG				 	0xA0000000
#define RAM_END				 	0xA0000000+BMXDRMSZ-1

#define CS_RAM
#define STATUS_LED  BIT_3
//#define _status_on()  (PORTB |=STATUS_LED)
//#define _status_off() (PORTB &=~STATUS_LED)
#define _write_on() _status_on()
#define _write_off() _status_off()
#define _read_on() _status_on()
#define _read_off() _status_off()


#define USE_CORE_TIMER  // mettre en commentaire si on n'utilise pas le core timer

//SDcard and SPIRAM share same SPI channel
//SPI2 used by storage devices 
#define STORE_SPICON SPI2CON
#define STORE_SPIBRG SPI2BRG
#define STORE_SPISTATbits SPI2STATbits
#define STORE_SPIBUF SPI2BUF

//storage devices select on PORTB
#define STORE_PORT PORTB
#define STORE_PORTCLR PORTBCLR
#define STORE_PORTSET PORTBSET
#define STORE_TRIS TRISB
//bit used for select lines
#define SRAM_SEL  BIT_1
#define SDC_SEL BIT_2
// storage device activity LED
#define STATUS_LED  BIT_3
#define _status_on() PORTBSET = STATUS_LED
#define _status_off() PORTBCLR = STATUS_LED

unsigned int heap_size;

void HardwareInit();
unsigned free_heap();


#ifdef USE_CORE_TIMER
unsigned int ticks(void);
#endif
void delay_us(unsigned int usec);
void delay_ms(unsigned int msec);

#endif	/* HARDWAREPROFILE_H */

