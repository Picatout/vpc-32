#ifndef _sdcard
#define _sdcard
#include <plib.h>


#define  TRIS_CARDSELECT    TRISBbits.TRISB2
#define  CARDSELECT         LATBbits.LATB2

unsigned char SendSDCCmd(unsigned char cmd, unsigned long long address, unsigned char crc, unsigned char more);
void MediaInitialize(void);
void SECTORread(unsigned long long sector_addr, unsigned char* buffer);

#endif

