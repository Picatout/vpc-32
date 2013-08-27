/* 
 * File:   vpc-32.c
 * Author: Jacques
 *
 * Created on 26 août 2013, 07:38
 */

#include <stdio.h>
#include <stdlib.h>
#include <plib.h>
#include "hardware/HardwareProfile.h"
#include "hardware/ntsc.h"
#include "hardware/font.h"
#include "hardware/serial_comm.h"

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


void put_char(int x, int y, char c){
    register int i,l,r,b;
    if (c>='a'){
        c -= 32;
    }
    c -=32;
    b=x>>5;
    r=0;
    l=27-(x&0x1f);
    if (l<0){
        r=-l;
    }
    for (i=0;i<7;i++){
        if (r){
            video_bmp[y][b] |= font[c][i]>>r;
            video_bmp[y][b+1] |= font[c][i]<<(32-r);
            y++;
        } else{
            video_bmp[y++][b] |= font[c][i]<<l;
        }
    }
}//put)char()

const char *msg2=" test video ntsc ";
const char *msg1="01234567890123456789012345678901234567890123456789012"; // 53 caractères par ligne

void print(int x, int y, const char *text){
    while (*text){
        put_char(x, y, *text++);
        x += 6;
        if (x>HRES-6) break;
    }
}// print()

void test_pattern(void){
    int i,j;
    for (i=0;i<VRES;i++){
        video_bmp[i][0]=0x80000000;
        video_bmp[i][HRES/32-1]=1;
    }
    for (i=0;i<HRES/32;i++){
        video_bmp[0][i]=0xffffffff;
        video_bmp[VRES-1][i]=0xffffffff;
    }
    for (i=VRES/4;i<VRES/2+VRES/4;i++){
        video_bmp[i][2]=0xFF00FF00;
        video_bmp[i][3]=0xF0F0F0F0;
        video_bmp[i][4]=0xcccccccc;
        video_bmp[i][5]=0xaaaaaaaa;
    }//i
    print(2,3,msg1);
    print(2,12,msg2);
}//test_pattern()


void main(void) {
    HardwareInit();
    UartInit(STDIO,9600,DEFAULT_LINE_CTRL);
    KeyboardInit();
    ln_cnt=0;
    video=0;
    test_pattern();
    VideoInit();
    while(1){
    }
}
