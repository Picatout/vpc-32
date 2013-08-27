/*
 * File:   uart.h
 * Author: Jacques
 * Description: fonctions de bas niveau pour communication UART avec le PC
 * Created on 17 avril 2013, 14:48
 */

#include <plib.h>
#include "serial_comm.h"
#include "HardwareProfile.h"

static char unget;

void UartInit(UART_MODULE channel, int baudrate, UART_LINE_CONTROL_MODE LineCtrl){
/*initialise le périphérique UART*/
   // Configure UART, information dans uart.h
   UARTConfigure(channel, UART_ENABLE_PINS_TX_RX_ONLY); // pas de contrôle de flux matériel.
   UARTSetLineControl(channel, LineCtrl);
   UARTSetDataRate(channel, mGetPeripheralClock(), baudrate);
   // activation du périphérique
   UARTEnable(channel, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));
   unget=-1;
};

char UartGetch(UART_MODULE channel){
/*lecture d'un caractère retourne -1 si aucun disponible*/
    char ch;
    if (!unget==-1) {
        ch=unget;
        unget=-1;
        return ch;
    }else{
        if (UARTReceivedDataIsAvailable (channel)){
               return UARTGetDataByte(channel);
        }else{
            return -1;
        }
    }
};

void UartPutch(UART_MODULE channel, char c){
/*Envoie d'un caractère*/
    while(!UARTTransmitterIsReady(channel));
      UARTSendDataByte(channel, c);
};

char UartWaitch(UART_MODULE channel, int delay){
/*attend un caractère pour un temps spécifié par delay
*retourne -1 si aucun reçu avant expiration du délais
*/
    int t;
    char ch;
    if (!unget==-1){
        ch=unget;
        unget=-1;
        return unget;
    }
    if (!delay) while (1) if (UARTReceivedDataIsAvailable(channel)) return UARTGetDataByte(channel);
    t=sys_tick;
    while (sys_tick-t<delay){
       if (UARTReceivedDataIsAvailable(channel)) return UARTGetDataByte(channel);
    }
    return -1;
}

void UartPrint(UART_MODULE channel, const char* str){
/* envoie la chaine de caractère str via le UART */
   while(*str != (char)0)
   {
      while(!UARTTransmitterIsReady(channel));
      UARTSendDataByte(channel, *str++);
   }
   while(!UARTTransmissionHasCompleted(channel));
};

int UartReadln(UART_MODULE channel, char *buffer, int buff_len){
/* lecture d'une chaine se terminant par '\r'
 *  complète la chaine avec un zéro
 *  retourne le nombre de caractère dans le buffer.
 */
    int count=0;
    char c;
    if (!unget==-1){
        c=unget;
        unget=-1;
        *buffer++=c;
        if (c==CR) return;
    }
    while (count < (buff_len-1)){
        if (UARTReceivedDataIsAvailable(channel)){
            c = UARTGetDataByte(channel);
            if (c==CR) break;
            if (c==BS){
                if (count){
                    buffer--;
                    count--;
                    UartPrint(channel,"\b \b");
                }
            }else{
                *buffer++=c;
                count++;
                UartPutch(channel,c);
            }
            
        }
    }
    if (count) *buffer = (char)0;
    UartPutch(channel,'\r');
    return count;
}

int UartReceivedBreak(UART_MODULE channel){
   char ch;
   if (unget==-1 && UARTReceivedDataIsAvailable(channel)) {
       ch=UARTGetDataByte(channel);
       if (ch==ESC) return 1; else unget=ch;
   }
   return 0;
}

