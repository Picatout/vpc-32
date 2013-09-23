/*********************************************************************
 *
 *                  UART buffered
 *                  DUINOMITE - DIOS Forth
 *
 * v.1.0 - 19.12.2012, UART, INT, FIFO, L.Pekny
 ********************************************************************/

#include <plib.h>
#include "..\HardwareProfile.h"
#include "UartBuf.h"


UARTblk UART[6];


void UARTopen(char Ux)  // Config UART, clear buf, set interrupts
{

        UARTConfigure(Ux, UART_ENABLE_PINS_TX_RX_ONLY | UART_ENABLE_HIGH_SPEED);
	UARTSetFifoMode(Ux, UART_INTERRUPT_ON_TX_NOT_FULL | UART_INTERRUPT_ON_RX_NOT_EMPTY);
	UARTSetLineControl(Ux, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
	UARTSetDataRate(Ux, GetPeripheralClock(), DEF_BAUDRATE); UART[Ux].Speed=DEF_BAUDRATE;
	UARTEnable(Ux, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));

	UARTinit(Ux);

 	INTSetVectorPriority(INT_VECTOR_UART(Ux), INT_PRIORITY_LEVEL_3);
 	INTSetVectorSubPriority(INT_VECTOR_UART(Ux), INT_SUB_PRIORITY_LEVEL_0);
	INTClearFlag(INT_SOURCE_UART_RX(Ux)); INTClearFlag(INT_SOURCE_UART_TX(Ux));
	INTEnable(INT_SOURCE_UART_RX(Ux), INT_ENABLED);

	delay_us(10000);
}


void UARTclose(char Ux)  // Disable UART and interrupts, clear buf
{
	INTEnable(INT_SOURCE_UART_RX(Ux)|INT_SOURCE_UART_TX(Ux), INT_DISABLED);
	UARTEnable(Ux, UART_DISABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));
	delay_us(5000);
}


BYTE UARTread(char Ux)  // Data? -> read from RXbuf
{
	BYTE value;

	INTEnable(INT_SOURCE_UART_RX(Ux), INT_DISABLED);
	if (UART[Ux].RXrd!=UART[Ux].RXwr)  // Is data?
	  { UART[Ux].RXrd++; UART[Ux].RXrd&=UartCntMask; }
	value=*(UART[Ux].pRXbuf+UART[Ux].RXrd);
	INTEnable(INT_SOURCE_UART_RX(Ux), INT_ENABLED);
	return value;
}


void UARTwrite(char Ux, BYTE value)  // Write to TXbuf, interrupt en
{
	INTEnable(INT_SOURCE_UART_TX(Ux), INT_DISABLED);
	UART[Ux].TXwr++; UART[Ux].TXwr&=UartCntMask;
	*(UART[Ux].pTXbuf+UART[Ux].TXwr)=value;

	if (UART[Ux].TXwr==UART[Ux].TXrd)  // Full?
	{
	  UART[Ux].TXrd++; UART[Ux].TXrd&=UartCntMask;
	  UART[Ux].TXerr|=1;  // Full, replace char
	}
	INTEnable(INT_SOURCE_UART_TX(Ux), INT_ENABLED);
}


void UARTspeed(char Ux, UINT value)  // Set baudrate
{
	UARTSetDataRate(Ux, GetPeripheralClock(), value);
	UART[Ux].Speed=value;
}


void UARTmode(char Ux, UINT value)  // Set parity, size, stopbits
{
	UARTSetLineControl(Ux, value);
}


BYTE isUARTread(char Ux)  // Data in RXbuf?
{
	BYTE value;

	value=(UART[Ux].RXrd!=UART[Ux].RXwr);
	return value;
}


BYTE isUARTwrite(char Ux)  // No full TXbuf?
{
	WORD Delta;

	Delta=(UART[Ux].TXrd-UART[Ux].TXwr)&UartCntMask;
	return (BYTE)(Delta!=1);
}


void UARTinit(char Ux)  // Clear buf
{
	WORD i;

	UART[Ux].RXwr=0; UART[Ux].RXrd=0; UART[Ux].RXerr=0;
	UART[Ux].TXwr=0; UART[Ux].TXrd=0; UART[Ux].TXerr=0;
	UART[Ux].pRXbuf=&UART[Ux].RXbuf[0];
	UART[Ux].pTXbuf=&UART[Ux].TXbuf[0];

	for (i=0; i<UartBufSize; i++) {UART[Ux].RXbuf[i]=0; UART[Ux].TXbuf[i]=0;}
}


void UARTwriteS(char Ux, const char *string)
{
  while (*string != '\0')
    {
	//  while (!isUARTwrite(Ux));		// froze
	  if (!isUARTwrite(Ux)) {delay_us(20000000/UART[Ux].Speed);}
 	  UARTwrite(Ux,*string);
      string++;
    }
}


void RXuart(char Ux)  // wr++, buf[wr]=RX
{
  while (UARTReceivedDataIsAvailable(Ux))  // Read HW FIFO
  {
	UART[Ux].RXwr++; UART[Ux].RXwr&=UartCntMask;
	if (UART[Ux].RXwr==UART[Ux].RXrd)  // Full?
	{
	  UART[Ux].RXrd++; UART[Ux].RXrd&=UartCntMask;
	  UART[Ux].RXerr|=1;  // Full, replace char
	}
	*(UART[Ux].pRXbuf+UART[Ux].RXwr)=UARTGetDataByte(Ux);
  }
}


void TXuart(char Ux)  // rd++, TX=buf[rd], rd=wr? -> done, no interrupt
{
  while (UARTTransmitterIsReady(Ux))  // Write to HW FIFO
  {
	UART[Ux].TXrd++; UART[Ux].TXrd&=UartCntMask;
	UARTSendDataByte(Ux, *(UART[Ux].pTXbuf+UART[Ux].TXrd));
	if (UART[Ux].TXrd==UART[Ux].TXwr)
	{
	  INTEnable(INT_SOURCE_UART_TX(Ux), INT_DISABLED);
	  break;  // TX done
	}
  }
}


// UART1 interrupt handler
void __ISR(_UART_1_VECTOR, IPL3) IntUart1Handler(void)
{
  // RX interrupt?
  if (INTGetFlag(INT_SOURCE_UART_RX(UART1)))
  {
	if (INTGetEnable(INT_SOURCE_UART_RX(UART1))) {RXuart(UART1);}
	INTClearFlag(INT_SOURCE_UART_RX(UART1));  // Clear the RX interrupt Flag
  }

  // TX interrupt?
  if ( INTGetFlag(INT_SOURCE_UART_TX(UART1)) )
  {
	if (INTGetEnable(INT_SOURCE_UART_TX(UART1))) {TXuart(UART1);}
	INTClearFlag(INT_SOURCE_UART_TX(UART1));
  }
}


// UART2 interrupt handler
void __ISR(_UART_2_VECTOR, IPL3) IntUart2Handler(void)
{
  // RX interrupt?
  if (INTGetFlag(INT_SOURCE_UART_RX(UART2)))
  {
	if (INTGetEnable(INT_SOURCE_UART_RX(UART2))) {RXuart(UART2);}
	INTClearFlag(INT_SOURCE_UART_RX(UART2));  // Clear the RX interrupt Flag
  }

  // TX interrupt?
  if ( INTGetFlag(INT_SOURCE_UART_TX(UART2)) )
  {
	if (INTGetEnable(INT_SOURCE_UART_TX(UART2))) {TXuart(UART2);}
	INTClearFlag(INT_SOURCE_UART_TX(UART2));
  }
}

/*
// UART3 interrupt handler
void __ISR(_UART_3_VECTOR, IPL3) IntUart3Handler(void)
{
  // RX interrupt?
  if (INTGetFlag(INT_SOURCE_UART_RX(UART3)))
  {
	if (INTGetEnable(INT_SOURCE_UART_RX(UART3))) {RXuart(UART3);}
	INTClearFlag(INT_SOURCE_UART_RX(UART3));  // Clear the RX interrupt Flag
  }

  // TX interrupt?
  if ( INTGetFlag(INT_SOURCE_UART_TX(UART3)) )
  {
	if (INTGetEnable(INT_SOURCE_UART_TX(UART3))) {TXuart(UART3);}
	INTClearFlag(INT_SOURCE_UART_TX(UART3));
  }
}


// UART4 interrupt handler
void __ISR(_UART_4_VECTOR, IPL3) IntUart4Handler(void)
{
  // RX interrupt?
  if (INTGetFlag(INT_SOURCE_UART_RX(UART4)))
  {
	if (INTGetEnable(INT_SOURCE_UART_RX(UART4))) {RXuart(UART4);}
	INTClearFlag(INT_SOURCE_UART_RX(UART4));  // Clear the RX interrupt Flag
  }

  // TX interrupt?
  if ( INTGetFlag(INT_SOURCE_UART_TX(UART4)) )
  {
	if (INTGetEnable(INT_SOURCE_UART_TX(UART4))) {TXuart(UART4);}
	INTClearFlag(INT_SOURCE_UART_TX(UART4));
  }
}


// UART5 interrupt handler
void __ISR(_UART_5_VECTOR, IPL3) IntUart5Handler(void)
{
  // RX interrupt?
  if (INTGetFlag(INT_SOURCE_UART_RX(UART5)))
  {
	if (INTGetEnable(INT_SOURCE_UART_RX(UART5))) {RXuart(UART5);}
	INTClearFlag(INT_SOURCE_UART_RX(UART5));  // Clear the RX interrupt Flag
  }

  // TX interrupt?  if ( INTGetFlag(INT_SOURCE_UART_TX(UART5)) )
  {
	if (INTGetEnable(INT_SOURCE_UART_TX(UART5))) {TXuart(UART5);}
	INTClearFlag(INT_SOURCE_UART_TX(UART5));
  }
}


// UART6 interrupt handler
void __ISR(_UART_6_VECTOR, IPL3) IntUart6Handler(void)
{
  // RX interrupt?
  if (INTGetFlag(INT_SOURCE_UART_RX(UART6)))
  {
	if (INTGetEnable(INT_SOURCE_UART_RX(UART6))) {RXuart(UART6);}
	INTClearFlag(INT_SOURCE_UART_RX(UART6));  // Clear the RX interrupt Flag
  }

  // TX interrupt?
  if ( INTGetFlag(INT_SOURCE_UART_TX(UART6)) )
  {
	if (INTGetEnable(INT_SOURCE_UART_TX(UART6))) {TXuart(UART6);}
	INTClearFlag(INT_SOURCE_UART_TX(UART6));
  }
}

*/

void PutCharacter(const char character)  // No interrupt
{
	while (!UARTTransmitterIsReady(UARTcon));
	UARTSendDataByte(UARTcon, character);
	while (!UARTTransmissionHasCompleted(UARTcon));
}


// ********** DIOS **********

void fhCOMinit(void *addr)
{
	int Ux=*(int *)addr;		// Ux=TOS
	UARTinit(Ux);
}


void fhCOMopen(void *addr)
{
	int Ux=*(int *)addr;		// Ux=TOS
	UARTopen(Ux);
}


void fhCOMclose(void *addr)
{
	int Ux=*(int *)addr;		// Ux=TOS
	UARTclose(Ux);
}


void fhCOMread(void *addr)
{
	int Ux=*(int *)addr;		// Ux=TOS
	*(int *)addr=UARTread(Ux);	// TOS=@READ
}


void fhCOMwrite(void *addr)
{
	int value,Ux;
	
	Ux=*(int *)addr;			// Ux=TOS
	value=*(int *)(addr-4);		// value=NOS
	UARTwrite(Ux,value);
}


void fhisCOMread(void *addr)
{
	int Ux=*(int *)addr;			// Ux=TOS
	*(int *)addr=isUARTread(Ux);	// TOS=@?READ
}


void fhisCOMwrite(void *addr)
{
	int Ux=*(int *)addr;			// Ux=TOS
	*(int *)addr=isUARTwrite(Ux);	// TOS=@?WRITE
}


void fhCOMwriteS(void *addr)
{
	int value,Ux;
	
	Ux=*(int *)addr;			// Ux=TOS
	value=*(int *)(addr-4);		// value=NOS
	UARTwriteS(Ux,(const char *)value);
}


void fhCOMspeed(void *addr)
{
	int value,Ux;
	
	Ux=*(int *)addr;			// Ux=TOS
	value=*(int *)(addr-4);		// value=NOS
	UARTspeed(Ux,value);
}


void fhCOMmode(void *addr)
{
	int value,Ux;
	
	Ux=*(int *)addr;		// Ux=TOS
	value=*(int *)(addr-4);		// value=NOS
	UARTmode(Ux,value);
}


const void* DevTabCOM[] =
{
	fhCOMinit, 0,           fhCOMopen, fhCOMclose,		// init, release, open, close
	fhCOMread, fhCOMwrite, fhisCOMread, fhisCOMwrite,	// read, write, ?read, ?write
	0,         fhCOMwriteS, fhCOMspeed, fhCOMmode
};
