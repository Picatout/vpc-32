/*
 *  Code adapté de Microchip note applicative http://www.microchip.com/forums/download.axd?file=0;366322
 *
 *  SPI2/SDI    RA4    (entrée)
 *  SPI2/SDO    RB8    (sortie)
 *  SPI2/SCLK   RB15   (sortie)
 *  ~CS         RB2    (sortie)
 *
 */
#ifndef _spi
#define _spi


#define mSPI_MasterInitialize() { \
 TRISAbits.TRISA4=1;             \
 TRISBbits.TRISB8=0;             \
 LATBbits.LATB8=0;               \
 TRISBbits.TRISB15=0;             \
 LATBbits.LATB15=0;             \
 TRISBbits.TRISB2=0;             \
 LATBbits.LATB2=0;               \
}

void writespi(unsigned char data_out);
unsigned char readspi(void);

#endif

