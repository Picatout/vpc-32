#include "sdcard.h"
#include "spi4sdcard.h"
#include "HardwareProfile.h"

unsigned char SendSDCCmd(unsigned char cmd, unsigned int address, unsigned char crc, unsigned char more)
{
  unsigned int timeout=0;
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
  unsigned int timeout=0;
  unsigned char response=0;

    _status_on();
    TRIS_CARDSELECT=0;
    CARDSELECT=1;

    mSPI_MasterInitialize();
    SpiChnOpen(SPI_CHANNEL2,SPI_OPEN_MSTEN|SPI_OPEN_MODE8|SPI_OPEN_ON,PBCLK/(2*400000L));
    for(timeout=0;timeout<=9;timeout++) { writespi(0xFF); }

    timeout=0xFFFFF;
    do {
      response=SendSDCCmd(0x40,0x0,0x95,0);
      timeout--;
    }while((response!=0x01)&&(timeout!=0));

    timeout=0xFFFFF;
    do {
      response=SendSDCCmd(0x41,0x0,0xF9,0);
      timeout--;
    }while((response!=0x00)&&(timeout!=0));

    timeout=0xFFFFF;
    do {
      response=SendSDCCmd(0x50,0x00000200,0xFF,0);
      timeout--;
    }while((response!=0x00)&&(timeout!=0));
    if (!response) {
        SPI2BRG=0;
        _status_off();
    }
}

void SECTORread(unsigned long long sector_addr, unsigned char* buffer)
{
  unsigned int timeout=0;
  unsigned char response=0;

    SendSDCCmd(0x51,(sector_addr<<9),0xFF,1);

    timeout=0xFFFFF;
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
