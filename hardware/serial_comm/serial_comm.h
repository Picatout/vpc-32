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
 * File:   uart.h
 * Author: Jacques Desch�nes
 * Description: fonctions de bas niveau pour communication UART avec le PC
 * Created on 17 avril 2013, 14:48
 */

#ifndef UART_H
#define	UART_H
#include <p32xxxx.h>
#include <plib.h>

#define NL  10  // nouvelle ligne
#define CR  13  // retour � la ligne
#define ESC 27  // �chappement
#define BS 8   // back space
#define SPC 32  // espace
#define TAB 9   // caract�re tabulation

#define DEFAULT_LINE_CTRL   UART_DATA_SIZE_8_BITS|UART_PARITY_NONE|UART_STOP_BITS_1

void UartInit( UART_MODULE channel, int baudrate, UART_LINE_CONTROL_MODE LineCtrl);
char UartGetch(UART_MODULE channel);
char UartWaitch(UART_MODULE channel, int delay);
void UartPutch(UART_MODULE channel,char c);
void UartPrint(UART_MODULE channel, const char* str);
int UartReadln(UART_MODULE channel, char * buffer, int buff_len);
int UartReceivedBreak(UART_MODULE channel);
#endif	/* UART_H */

