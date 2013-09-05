#include "sdcard.h"
#include "../spi/spi.h"


unsigned char SendSDCCmd(unsigned char cmd, unsigned long long address, unsigned char crc, unsigned char more)
{
  unsigned long timeout=0;
  unsigned char response=0;

   CARDSELECT=0;
   writespi(0xFF);
   writespi(0xFF);
   writespi(0xFF);

   writespi(cmd);
   writespi(((address&0xFF000000)>>24));
   writespi(((address&0x00FF0000)>>16));
   writespi(((address&0x0000FF00)>>8));
   writespi((address&0x000000FF));
   writespi(crc);

   timeout=8;
   do {
     response=readspi();
     timeout--;
   }while((response&0x80)&&(timeout!=0));

   writespi(0xFF);

   if(!more) {
     CARDSELECT=1;
     writespi(0xFF);
     writespi(0xFF);
     writespi(0xFF);
   }

 return(response);
}

void MediaInitialize(void)
{
  unsigned long timeout=0;
  unsigned char response=0;

    TRIS_CARDSELECT=0;
    CARDSELECT=1;

    mSPI_MasterInitialize();
    OpenSPI(SPI_FOSC_64, MODE_00, SMPMID);

    for(timeout=0;timeout<=9;timeout++) { writespi(0xFF); }

    timeout=0xFFFF;
    do {
      response=SendSDCCmd(0x40,0x0,0x95,0);
      timeout--;
    }while((response!=0x01)&&(timeout!=0));

    timeout=0xFFFF;
    do {
      response=SendSDCCmd(0x41,0x0,0xF9,0);
      timeout--;
    }while((response!=0x00)&&(timeout!=0));

    timeout=0xFFFF;
    do {
      response=SendSDCCmd(0x50,0x00000200,0xFF,0);
      timeout--;
    }while((response!=0x00)&&(timeout!=0));
}

void SECTORread(unsigned long long sector_addr, unsigned char* buffer)
{
  unsigned long timeout=0;
  unsigned char response=0;

    SendSDCCmd(0x51,(sector_addr<<9),0xFF,1);

    timeout=0xFFFF;
    do {
      response=readspi();
      timeout--;
    }while((response!=0xFE)&&(timeout!=0));

    for(timeout=0;timeout<512;timeout++) {
      *buffer=readspi();
      buffer++;
    }
    writespi(0xFF);
    writespi(0xFF);

    writespi(0xFF);

    CARDSELECT=1;
    writespi(0xFF);
    writespi(0xFF);
    writespi(0xFF);
}