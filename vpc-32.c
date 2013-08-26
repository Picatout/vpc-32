/* 
 * File:   vpc-32.c
 * Author: Jacques Deschênes
 *
 * Created on 18 août 2013, 21:37
 */


#include <stdio.h>
#include <stdlib.h>
#include <plib.h>

#define SYSCLK  30000000L



// PIC32MX150F128B Configuration Bit Settings

#include <xc.h>

// DEVCFG3
// USERID = No Setting
#pragma config PMDL1WAY = ON            // Peripheral Module Disable Configuration (Allow only one reconfiguration)
#pragma config IOL1WAY = ON             // Peripheral Pin Select Configuration (Allow only one reconfiguration)
#pragma config FUSBIDIO = ON            // USB USID Selection (Controlled by the USB Module)
#pragma config FVBUSONIO = ON           // USB VBUS ON Selection (Controlled by USB Module)

// DEVCFG2
#pragma config FPLLIDIV = DIV_5         // PLL Input Divider (5x Divider)
#pragma config FPLLMUL = MUL_15         // PLL Multiplier (15x Multiplier)
#pragma config FPLLODIV = DIV_1         // System PLL Output Clock Divider (PLL Divide by 1)

// DEVCFG1
#pragma config FNOSC = PRIPLL           // Oscillator Selection Bits (Primary Osc w/PLL (XT+,HS+,EC+PLL))
#pragma config FSOSCEN = OFF            // Secondary Oscillator Enable (Disabled)
#pragma config IESO = OFF               // Internal/External Switch Over (Disabled)
#pragma config POSCMOD = OFF            // Primary Oscillator Configuration (Primary osc disabled)
#pragma config OSCIOFNC = OFF            // CLKO Output Signal Active on the OSCO Pin (Enabled)
#pragma config FPBDIV = DIV_1           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/8)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
#pragma config WDTPS = PS1048576        // Watchdog Timer Postscaler (1:1048576)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable (Watchdog Timer is in Non-Window Mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))
#pragma config FWDTWINSZ = WISZ_25      // Watchdog Timer Window Size (Window Size is 25%)

// DEVCFG0
#pragma config JTAGEN = OFF             // JTAG Enable (JTAG Disabled)
#pragma config ICESEL = ICS_PGx1        // ICE/ICD Comm Channel Select (Communicate on PGEC1/PGED1)
#pragma config PWP = OFF                // Program Flash Write Protect (Disable)
#pragma config BWP = OFF                // Boot Flash Write Protect bit (Protection Disabled)
#pragma config CP = OFF                 // Code Protect (Protection Disabled)







#define PWM_PERIOD 1904
#define HSYNC 141

volatile unsigned short ln_cnt=0;

/*
 * 
 */
void main() {
  /*
    SYSTEMConfigPerformance(SYSCLK);
    INTEnableSystemMultiVectoredInt();
    RPB7Rbits.RPB7R=0x5;  // OC1  sortie PWM pour SYNC
    RPB6Rbits.RPB6R=0x3;  // SDO1  sortie vidéo
    //configuration PWM sur OC1, utilisation TIMER2
    T2CON = 0;
    PR2=PWM_PERIOD;
    OC1CON = 6; // mode PWM sans protection
    OC1RS = PWM_PERIOD-HSYNC;
    OC1R = PWM_PERIOD-HSYNC;
    IFS0bits.T2IF=0;
    IEC0bits.T2IE=1;
    IPC2SET = (7<<2);// niveau de priorité interrution 7
    OC1CONSET=0x8000;  // activation
    T2CONSET=0x8000;  // activation
*/
     TRISBCLR = 1<<5;
     while(1){
         TRISBINV = 1<<5;
     };
}//main()

/*
void __ISR(_TIMER_2_VECTOR,IPL7SRS) tmr2_isr(void){
    ln_cnt++;
    switch (ln_cnt){
        case 3:
            OC2RS=HSYNC;
            break;
        case 263:
            OC2RS=PWM_PERIOD-HSYNC;
            ln_cnt=0;
            break;
    }//switch (ln_cnt)
    IFS0CLR = IFS0bits.OC2IF;
}//tmr2_isr()
*/