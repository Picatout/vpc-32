/* 
 * File:   eForth_test.c
 * Author: Jacques
 *
 * Created on 14 septembre 2013, 08:07
 */

#include <stdio.h>
#include <stdlib.h>
#include "../../../hardware/HardwareProfile.h"
#include "../../../hardware/serial_comm.h"

// PIC32MX150F128B Configuration Bit Settings
#include <xc.h>

// DEVCFG3
// USERID = No Setting
#pragma config PMDL1WAY = OFF           // permet plusieurs configurations des périphériques.
#pragma config IOL1WAY = OFF            // permet plusieurs configuration des broches.

// DEVCFG2
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (2x Divider)
#if SYSCLK==40000000L
#pragma config FPLLMUL = MUL_16         // SYSCLK=40Mhz
#else
#pragma config FPLLMUL = MUL_15          // PLL Multiplier (15x Multiplier) SYSCLK=37,5Mhz
#endif
#pragma config FPLLODIV = DIV_2         // System PLL Output Clock Divider (PLL Divide by 2)

// DEVCFG1
#pragma config FNOSC = PRIPLL           // Oscillator Selection Bits (Primary Osc w/PLL (XT+,HS+,EC+PLL))
#pragma config FSOSCEN = OFF            // Secondary Oscillator Enable (Disabled)
#pragma config IESO = OFF               // Internal/External Switch Over (Disabled)
#pragma config POSCMOD = HS             // Primary Oscillator Configuration (XT osc mode)
#pragma config OSCIOFNC = OFF           // CLKO Output Signal Active on the OSCO Pin (Disabled)
#pragma config FPBDIV = DIV_1           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/1)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))

// DEVCFG0
#pragma config JTAGEN = OFF             // JTAG Enable (JTAG Disabled)
#pragma config ICESEL = ICS_PGx1        // ICE/ICD Comm Channel Select (Communicate on PGEC1/PGED1)
#pragma config PWP = OFF                // Program Flash Write Protect (Disable)
#pragma config BWP = OFF                // Boot Flash Write Protect bit (Protection Disabled)
#pragma config CP = OFF                 // Code Protect (Protection Disabled)

extern void eForth(void);

char *text[80];
void debug_print(char *fmt, unsigned int a, unsigned int b, unsigned int c){
    sprintf((char*)text, fmt, a,b,c);
    UartPrint(STDOUT, (char*)text);
} // debug_print

/*
 * 
 */
void main(void) {
    HardwareInit();
    UartInit(STDIO,115200,DEFAULT_LINE_CTRL);
    while (1){
       eForth();
       UartPrint(STDOUT,"VM crashed!\r restarting it.\r");
    }
}

