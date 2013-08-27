/* 
 * File:   uart.h
 * Author: Jacques
 * Description: fonctions de bas niveau pour communication UART avec le PC
 * Created on 17 avril 2013, 14:48
 */

#ifndef UART_H
#define	UART_H
#include <p32xxxx.h>
#include <plib.h>

#define NL  10  // nouvelle ligne
#define CR  13  // retour à la ligne
#define ESC 27  // échappement
#define BS 8   // back space
#define SPC 32  // espace
#define TAB 9   // caractère tabulation

#define DEFAULT_LINE_CTRL   UART_DATA_SIZE_8_BITS|UART_PARITY_NONE|UART_STOP_BITS_1

void UartInit( UART_MODULE channel, int baudrate, UART_LINE_CONTROL_MODE LineCtrl);
char UartGetch(UART_MODULE channel);
char UartWaitch(UART_MODULE channel, int delay);
void UartPutch(UART_MODULE channel,char c);
void UartPrint(UART_MODULE channel, const char* str);
int UartReadln(UART_MODULE channel, char * buffer, int buff_len);
int UartReceivedBreak(UART_MODULE channel);
#endif	/* UART_H */

