#include <plib.h>
#include <xc.h>
#include "spi4sdcard.h"

#pragma code

void writespi(unsigned char data_out)
{
   SPI2BUF = data_out;        // Send the data
   while(!SPI2STATbits.SPIRBF);   // While emtpy wait
   data_out = SPI2BUF;        // Essential avoid overflow i.e. receive dummy byte
   while(SPI2STATbits.SPIRBF);    // While full wait
   _nop();
   _nop();
   _nop();
   _nop();
   _nop();
   _nop();
   _nop();
   _nop();
   _nop();
}

unsigned char readspi(void)
{
 unsigned char TempVar;
   IFS1bits.SPI2TXIF = 0;       // Make certain interrupt flag is  clear
   TempVar = SPI2BUF;         // Make certain BF is clear.
   SPI2BUF = 0xff;            // initiate bus cycle
   while(!IFS1bits.SPI2TXIF);   // wait until bus cycle complete
   while(!IFS1bits.SPI2TXIF);   // wait until bus cycle complete
   while(!IFS1bits.SPI2TXIF);   // wait until bus cycle complete
   while(!IFS1bits.SPI2TXIF);   // wait until bus cycle complete
   while(!IFS1bits.SPI2TXIF);   // wait until bus cycle complete
   while(!IFS1bits.SPI2TXIF);   // wait until bus cycle complete
   while(!IFS1bits.SPI2TXIF);   // wait until bus cycle complete
   while(!IFS1bits.SPI2TXIF);   // wait until bus cycle complete
   while(!IFS1bits.SPI2TXIF);   // wait until bus cycle complete
 return(SPI2BUF);             // return with byte read

}

