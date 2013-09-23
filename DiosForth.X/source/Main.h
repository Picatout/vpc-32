/*********************************************************************
 *
 *                  Main.h
 *                  DUINOMITE - DIOS Forth
 *
 * v.1.0 - 20.12.2012, L.Pekny
 ********************************************************************/

#ifndef __MAIN_H__
#define __MAIN_H__



#define StrFW "\r\n PIC32MX150F128B-40MHz\r\n"

typedef struct
{
	void *dlink;	// Device link
	void *dcall;	// Device functions	
	char *dname;	// Device name
	int  info;      // Other info
} TDEVICE;

TDEVICE DevTab[];

void *DevHead;


#endif
		

