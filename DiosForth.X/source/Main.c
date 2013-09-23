/*********************************************************************
 *
 *                  Main PIC32
 *                  DUINOMITE - DIOS Forth
 *
 * v.1.0 - 16.03.2012-20.12.2012, UART, I/O, ADC, PWM, Timer, L.Pekny
 ********************************************************************/

// Configuring the Device
// SYSCLK = 80 MHz (8MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
// PBCLK  = 80 MHz
// WDT OFF
// BootQCOM - vectors 0x9D003000, reset 0x9D004000

//#define  __32MX795F512H__
//#include <p32xxxx.h>

// Configuring the Device Configuration Registers
// SYSCLK = 80 MHz (8MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
// PBCLK  = 80 MHz
// Primary Osc w/PLL (XT+,HS+,EC+PLL)
// Boot write protect

//        #pragma config UPLLEN   = OFF            // USB PLL Enabled
//        #pragma config UPLLIDIV = DIV_2         // USB PLL Input Divider
        #pragma config FPLLMUL  = MUL_16        // PLL Multiplier
        #pragma config FPLLIDIV = DIV_2         // PLL Input Divider
        #pragma config FPLLODIV = DIV_2         // PLL Output Divider
        #pragma config FPBDIV   = DIV_1         // Peripheral Clock divisor
        #pragma config FWDTEN   = OFF           // Watchdog Timer
        #pragma config WDTPS    = PS1           // Watchdog Timer Postscale
        #pragma config FCKSM    = CSDCMD        // Clock Switching & Fail Safe Clock Monitor
        #pragma config OSCIOFNC = OFF           // CLKO Enable
        #pragma config POSCMOD  = HS            // Primary Oscillator
        #pragma config IESO     = OFF           // Internal/External Switch-over
        #pragma config FSOSCEN  = OFF           // Secondary Oscillator Enable (KLO was off)
        #pragma config FNOSC    = PRIPLL        // Oscillator Selection
        #pragma config CP       = OFF           // Code Protect
        #pragma config BWP      = ON            // Boot Flash Write Protect
        #pragma config PWP      = OFF           // Program Flash Write Protect
        #pragma config ICESEL   = ICS_PGx1      // ICE/ICD Comm Channel Select
        #pragma config FUSBIDIO = OFF           // USBID I/O PORT
        #pragma config FVBUSONIO= ON			// VBUS ON Selection
//        #pragma config FMIIEN = OFF, FETHIO = OFF	// external PHY in RMII/alternate configuration


#include <stdlib.h>
#include <plib.h>
#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "Main.h"
#include "UART\UartBuf.h"
#include "PIN\Pin.h"
#include "TIMER\MTimer.h"
#include "OCmp\OCmp.h"
#include "NVMem\NVMem.h"
#include "VM\VMword.h"


const char Str1[] = StrFW;


// ********** DIOS **********
TDEVICE DevTab[] =
{
	{0,         DevTabTIM1,"TIM1",0x01},
	{&DevTab[0],DevTabCOM, "COM", 0x01},
	{&DevTab[1],DevTabPIN, "PIN", 0x01},
	{&DevTab[2],DevTabADC, "ADC", 0x01},
	{&DevTab[3],DevTabPWM, "PWM", 0x01},
};
void *DevHead=&DevTab[4];
// **************************

#define TX BIT_10 // UART2 TX sur PB10
#define RX BIT_11 // UART2 RX sur PB11


WORD ADCvalue;
BYTE KeyU;

INT main(void)
{
	UINT pbClk;

	delay_us(10000);
	pbClk=SYSTEMConfig(SYS_FREQ,SYS_CFG_WAIT_STATES|SYS_CFG_PCACHE|SYS_CFG_PB_BUS);
	INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);  //multi-vectored mode
	INTEnableInterrupts();                              // enable interrupts

//	PINopen();
    // configuration PB10 sortie et PB11 entrée
   mPORTBSetBits(TX); // mettre à Vdd pour ne pas généré un faux start bit.
   mPORTBSetPinsDigitalOut(TX);      // sortie PB10(Tx), transmission UART2
   mPORTBSetPinsDigitalIn (RX);      // entrée PB11(Rx), réception UART2
   PPSOutput(4, RPB10, U2TX);  // assignation U2TX sur PB10, (groupe, pin, fonction)
   PPSInput (2, U2RX, RPB11);  // assignation U2RX sur PB11, (groupe, fonction, pin)

	UARTclose(UARTcon); UARTopen(UARTcon);
	UARTwriteS(UARTcon,Str1);

	ADCopen(); ADCread(); ADCread();

	PWMopen();

	TIM1open();

        mPORTBSetPinsDigitalOut(BIT_3);

	PINwrite(pSTATUS_LED,1); delay_us(300000); PINwrite(pSTATUS_LED,0);
	delay_us(300000);
	PINwrite(pSTATUS_LED,1); delay_us(300000); PINwrite(pSTATUS_LED,0);

	while(1)
	{
	  cold();
	}
	return 0;
}			

