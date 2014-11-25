/*	----------------------------------------------------------------------------
	FILE:			fileio.c
	PROJECT:		pinguino32
	PURPOSE:		SD Card file system functions
	AUTHORS:		Alfred Broda <alfredbroda@gmail.com>
					Mark Harper <markfh@f2s.com>
					Regis Blanchot <rblanchot@gmail.com>
	FIRST RELEASE:	23 dec. 2011
	LAST RELEASE:	06 jan. 2012
	----------------------------------------------------------------------------
	based on original code by Regis Blanchot and FatFS example for PIC24
	----------------------------------------------------------------------------
 	[30-03-12][hgmvanbeek@gmail.com][Some cards have no card detect and no write protect]
	07 May 2012	As part of providing support for PIC32 Pinguino Micro and
					potentially other cards removed #if defined (PIC32_Pinguino) etc
					and #endif in function mount() so that SDCS is set via mount 
					for all cards.
*/

#ifndef __FILEIO_C__
#define __FILEIO_C__

// standard C libraries used
#include <ctype.h>      		// toupper...
#include <string.h>     		// memcpy...
//#include <malloc.h>				// malloc, free?
#include <GenericTypeDefs.h>

#include "fileio.h"
#include "sdmmc.h"
#include "ff.h"					// Fat Filesystem
#include "diskio.h"				// card access functions
#include "../serial_comm/serial_comm.h"
#include "../../console.h"

//#define SD_DEBUG

//#ifdef SD_DEBUG
//    #include <__cdc.c>          // USB CDC functions
//#endif

/*	----------------------------------------------------------------------------
 mount
 initializes a MEDIA structure for FILEIO access
 will mount only the first partition on the disk/card
 --------------------------------------------------------------------------*/

BOOL SDCardReady;

static FATFS _Fat;

char mount(unsigned char pin) {
	int flag, i;
	FRESULT r;

	SDCS = pin;

	// 0. init the I/Os
	initSD();

#ifdef SD_DEBUG
	UartPrint(STDOUT,"Looking for SD slot... ");
#endif
	// 1. check if the card is in the slot
	if (!getCD()) {
		FError = FE_NOT_PRESENT;
#ifdef SD_DEBUG
		UartPrint(STDOUT, "Failed!\r");
#endif
		return FALSE;
	}
#ifdef SD_DEBUG
	UartPrint(STDOUT,"OK\r");
#endif

	// 2. initialize the card
#ifdef SD_DEBUG
	UartPrint(STDOUT,"Initializing SD card... ");
#endif
	initMedia();
        if (disk_initialize(0)==STA_NOINIT){
#ifdef SD_DEBUG
            UartPrint(STDOUT,"failed!\r");
#endif
            return 0;
        };
#ifdef SD_DEBUG
        UartPrint(STDOUT,"OK\r");
#endif
	// We're skipping the old step 3 because there's no need for malloc
	// This takes 6k off the code size if malloc is not used elsewhere.
	// Instead, just point it to our _Fat var.
	// The FATFS struct takes only 560 bytes of mem.
	Fat = &_Fat;

	// Mount media
#ifdef SD_DEBUG
	UartPrint(STDOUT,"Mounting FAT filesystem... ");
#endif
	r = f_mount(0, Fat);
	if (r != FR_OK) {
		FError = r;
#ifdef SD_DEBUG
		UartPrint(STDOUT,"Failed!\r");
#endif
		//free(Fat);
		return FALSE;
	}
#ifdef SD_DEBUG
	UartPrint(STDOUT, "OK\r");
#endif

#ifdef SD_DEBUG
	UartPrint(STDOUT,"Checking FAT filesystem... ");
#endif
	const TCHAR * pth = "/";
	r = chk_mounted(&pth, &Fat, 0);
	if (r != FR_OK) {
		FError = r;
#ifdef SD_DEBUG
		UartPrint(STDOUT, "Failed!\r");
                 //put_rc(r);
#endif
		unmount();
		return FALSE;
	}
#ifdef SD_DEBUG
	UartPrint(STDOUT, "OK\r");
#endif

	return TRUE;
} // mount

/*	----------------------------------------------------------------------------
 unmount    initializes a MEDIA structure for FILEIO access
 --------------------------------------------------------------------------*/

void unmount(void) {
	f_mount(0, NULL);
	//free(Fat);
	SPI2CONCLR = 0x8000; // SPI2 OFF
} // unmount

/*	----------------------------------------------------------------------------
 present   test if a SD card is present
 --------------------------------------------------------------------------*/

char SD_present(unsigned char pin) {
	if (mount(pin)) {
		unmount();
		return TRUE;
	} else {
		return FALSE;
	}
}

/*	----------------------------------------------------------------------------
 Scans the current disk and compiles a list of files with a given extension
 list     array of file names max * 8
 max      number of entries
 ext      file extension we are searching for
 return   number of files found
 --------------------------------------------------------------------------*/

//unsigned listTYPE(char *listname, long *listsize, int max, const char *ext )
unsigned listTYPE(DIRTABLE *list, int max, const char *ext)
//unsigned listTYPE(char *list, int max, const char *ext )
{
	//TODO: implement

	return 0;
} // listTYPE

/* Prints the directory contents */
unsigned listDir(const char *path) {
	//TODO: remove all CDC references
	long p1;
	PF_BYTE res, b;
	UINT s1, s2;
	DIR dir; /* Directory object */
        char * fmt;
        dir.fs=Fat;
	res = f_opendir(&dir, path);
        if (!res) {
            p1 = s1 = s2 = 0;
            fmt=malloc(64);
            if (!fmt) {
                res=-1;
            }else{
                sprintf(fmt,"\rreading dirctory: ('%s')\r", path);
                print(comm_channel,fmt);
            }
        }
	while (!res) {
		res = f_readdir(&dir, &Finfo);
#ifdef SD_DEBUG
//		put_rc(res);
#endif
		if ((res != FR_OK) || !Finfo.fname[0]) {
			break;
		}
		if (Finfo.fattrib & AM_DIR) {
			s2++;
		} else {
			s1++;
			p1 += Finfo.fsize;
		}
// what about other outputs ?
		sprintf(fmt,"%c%c%c%c%c ",
                (Finfo.fattrib & AM_DIR) ? 'D' : '-',
				(Finfo.fattrib & AM_RDO) ? 'R' : '-',
				(Finfo.fattrib & AM_HID) ? 'H' : '-',
				(Finfo.fattrib & AM_SYS) ? 'S' : '-',
				(Finfo.fattrib & AM_ARC) ? 'A' : '-');
                print(comm_channel, fmt);
		sprintf(fmt,"%u/%02u/%02u %02u:%02u ",
                (Finfo.fdate >> 9) + 1980,
				(Finfo.fdate >> 5) & 15, Finfo.fdate & 31, (Finfo.ftime >> 11),
				(Finfo.ftime >> 5) & 63);
                print(comm_channel, fmt);
                sprintf(fmt," %9u ", Finfo.fsize);
                print(comm_channel, fmt);
		sprintf(fmt, " %-12s %s\r", Finfo.fname,
#if _USE_LFN
				Lfname);
#else
				"");
#endif
                print(comm_channel,fmt);
	}
        if (!res){
            sprintf(fmt, "\rfile count %d, directory count %d,  total size %d\r",s1,s2,p1);
            print(comm_channel, fmt);
        }else{
            if (fmt){
                    sprintf(fmt,"file i/o error code: %d\r", res);
                    print(comm_channel, fmt);
            }else{
                print(comm_channel,"Memory allocation error.\r");
            }
        }
        free(fmt);
	return s1;
} // listDir

char isDirectory(FILINFO file) {
	if (file.fattrib & AM_DIR) {
		return TRUE;
	} else {
		return FALSE;
	}
}

char isReadOnly(FILINFO file) {
	if (file.fattrib & AM_RDO) {
		return TRUE;
	} else {
		return FALSE;
	}
}

char isHidden(FILINFO file) {
	if (file.fattrib & AM_HID) {
		return TRUE;
	} else {
		return FALSE;
	}
}

char isSystem(FILINFO file) {
	if (file.fattrib & AM_SYS) {
		return TRUE;
	} else {
		return FALSE;
	}
}

char isArchive(FILINFO file) {
	if (file.fattrib & AM_ARC) {
		return TRUE;
	} else {
		return FALSE;
	}
}
#endif /* __FILEIO_C__ */

