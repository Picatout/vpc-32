/* 
 * File:   HardwareProfile.h
 * Author: Jacques
 * Description: configuration hardware spécifique
 * Created on 17 avril 2013, 14:41
 */

#ifndef HARDWAREPROFILE_H
#define	HARDWAREPROFILE_H

#include <p32xxxx.h>

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
#define CLK_PER_USEC SYSCLK/1000000L

volatile unsigned long  sys_tick; // compteur pour les milli-secondes

void HardwareInit();
long ticks(void);
void delay_ms(int msec);
void delay_us(int usec);

#endif	/* HARDWAREPROFILE_H */

