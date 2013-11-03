/*
* Copyright 2013, Jacques Deschênes
* This file is part of VPC-32.
*
*     VPC-32 is free software: you can redistribute it and/or modify
*     it under the terms of the GNU General Public License as published by
*     the Free Software Foundation, either version 3 of the License, or
*     (at your option) any later version.
*
*     VPC-32 is distributed in the hope that it will be useful,
*     but WITHOUT ANY WARRANTY; without even the implied warranty of
*     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*     GNU General Public License for more details.
*
*     You should have received a copy of the GNU General Public License
*     along with VPC-32.  If not, see <http://www.gnu.org/licenses/>.
*/

/* 
 * File:   vpc-32.c
 * Author: Jacques Deschênes
 *
 * Created on 26 août 2013, 07:38
 */

#include <stdio.h>
#include <stdlib.h>
#include <plib.h>
#include "graphics.h"

#include "hardware/HardwareProfile.h"
#include "hardware/ntsc.h"
#include "hardware/serial_comm.h"
#include "hardware/keyboard.h"
#include "hardware/Pinguino/diskio.h"
#include "console.h"
#include "hardware/Pinguino/ff.h"
#include "vpForth/opcodes.h"
#include "vpForth/vpForth.h"
#include "sound.h"

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



const char *msg1=" ntsc video target\r";
const char *msg2="0123456789012345678901234567890123456789012345678901"; // 52 caractères par ligne


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
    print(LOCAL_CON,msg1);
    print(LOCAL_CON,msg2);
}//test_pattern()

const int pts[6]={HRES/2,VRES/2,HRES/2+HRES/3,VRES/2+VRES/3,HRES/2-HRES/3,VRES/2+VRES/3};

void graphics_test(){ // test des fonctions graphiques
    int i;
    rectangle(50,50,200,200);
//    polygon(pts,3);
//    line(HRES/2,0,HRES/2,VRES-1);
//    line(0,VRES/2,HRES-1,VRES/2);
   // for (i=0;i<100;i++){
   //     ellipse(HRES/3+i,VRES/3+i,50,30);
   // }
}//graphics_test

const unsigned int e3k[]={ // rencontre du 3ième type
784,500, // sol4
880,500,// la4
698,500,// fa4
349,500,// fa3
523,500,// do4
0,0
};

void main(void) {
    HardwareInit();
    UartInit(STDIO,115200,DEFAULT_LINE_CTRL);
    ln_cnt=0;
    video=0;
    test_pattern();
    UartPrint(STDOUT,"video initialization\r");
    VideoInit();
    delay_ms(500);
    UartPrint(STDOUT,"keyboard initialization: ");
    if (KeyboardInit()){
        UartPrint(STDOUT,"OK\r");
        comm_channel=LOCAL_CON;
    }else{
        UartPrint(STDOUT,"keyboard error\r");
        UartPrint(STDOUT,"Using uart2 channel.\r");
        comm_channel=SERIAL_CON;
    }
    text_coord_t cpos;
    UartPrint(STDOUT,"SD initialization: ");
    if (!mount(0)){
        UartPrint(STDOUT,"Failed\r");
    }
    UartPrint(STDOUT,"sound initialization.\r");
    tune(&e3k[0]);
    UartPrint(STDOUT,"initialization completed.\r");
    set_cursor(CR_BLOCK);
    clear_screen();
    graphics_test();
    shell();
} // main()


