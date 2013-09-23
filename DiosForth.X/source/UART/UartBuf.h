/*********************************************************************
 *
 *                  UartBuf.h
 *                  DUINOMITE - DIOS Forth
 *
 * v.1.0 - 19.12.2012, L.Pekny
 ********************************************************************/

#ifndef __UARTBUF_H__
#define __UARTBUF_H__


#define DEF_BAUDRATE	115200
#define UARTcon			UART2	// Console

#define UartBufSize		64	    // 2^n
#define UartCntMask		0x3F	// Counter mask

typedef struct
{
	BYTE	RXbuf[UartBufSize];
	BYTE	TXbuf[UartBufSize];
	BYTE	*pRXbuf;			// Pointer to RXbuf
	BYTE	*pTXbuf;			// Pointer to TXbuf
	WORD	RXrd, RXwr;			// Counter for RXbuf
	WORD	TXrd, TXwr;			// Counter for TXbuf
	BYTE	RXerr,TXerr;		// Error flags
	UINT	Speed;				// Baudrate
} UARTblk;

extern UARTblk UART[6];

void UARTopen(char Ux);
void UARTclose(char Ux);
BYTE UARTread(char Ux);
void UARTwrite(char Ux, BYTE value);
BYTE isUARTread(char Ux);
BYTE isUARTwrite(char Ux);
void UARTinit(char Ux);
void UARTspeed(char Ux, UINT value);
void UARTmode(char Ux, UINT value);
void UARTwriteS(char Ux, const char *string);

const void* DevTabCOM[];


#endif
		

