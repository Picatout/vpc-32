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


#include "hardware/HardwareProfile.h"
#include "hardware/ntsc.h"
#include "hardware/serial_comm.h"
#include "hardware/keyboard.h"
#include "hardware/Pinguino/diskio.h"
//#include "console.h"

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



const char *msg2=" test video ntsc ";
const char *msg1="01234567890123456789012345678901234567890123456789012"; // 53 caractères par ligne


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
    print(msg1);
    print(msg2);
}//test_pattern()


void main(void) {
    int code;
    HardwareInit();
    UartInit(STDIO,9600,DEFAULT_LINE_CTRL);
    UartPrint(STDOUT,"initialisation video\r");
    ln_cnt=0;
    video=0;
    test_pattern();
    VideoInit();
    UartPrint(STDOUT,"initialisation clavier\r");
    delay_ms(750);
    if ((code=KeyboardInit())==1){
        _status_on();
        SetKbdLeds(F_NUM);
        delay_ms(300);
        SetKbdLeds(F_CAPS);
        delay_ms(300);
        SetKbdLeds(F_SCROLL);
        delay_ms(300);
        SetKbdLeds(0);
        _status_off();
    }else{
        UartPrint(STDOUT,"erreur initialisation clavier\r");
    }
    unsigned int cpos;
    UartPrint(STDOUT,"initialisation SPI2 (carte SD)\r");
    initSD();
    UartPrint(STDOUT,"initialisation carte SD\r");
    if (disk_initialize(0)==STA_NOINIT){
        UartPrint(STDOUT,"erreur d'initialisation carte SD\r");
        _status_on();
    }else{
        unsigned char buff[BLK_SIZE];
        int i;
        UartPrint(STDOUT,"lecture secteur 0 de la carte SD\r");
        clear_screen();
        if (disk_read(0,buff,0,1)==RES_OK){
            for (i=0;i<(BLK_SIZE);i++){
                print_hex(buff[i],2);
                if ((i+1)%18){
                    cursor_right();
                }
//                cpos=get_curpos();
//                if ((cpos>>16)>0){cursor_right();}
            }
        }
    }
    short scancode,key;
    UartPrint(STDOUT,"OK\r");
    while(1){
        if ((scancode=GetScancode())){
            if (scancode>0){
                switch (scancode){
                    case LSHIFT:
                    case RSHIFT:
                        rx_flags |= F_SHIFT;
                        break;
                    case BKSP:
                        cursor_left();
                        put_char(32);
                        break;
                    case ENTER:
                        put_char(CR);
                        break;
                    default:
                        put_char(GetKey(scancode)&127);
                }
            }else switch (scancode&511){
                case LSHIFT:
                case RSHIFT:
                    rx_flags &= ~F_SHIFT;
                    break;
                case LCTRL:
                case RCTRL:
                    break;
                case LALT:
                case RALT:
                    break;
                case NUM_LOCK:
                    kbd_leds ^= F_NUM;
                    SetKbdLeds(kbd_leds);
                    break;
                case CAPS_LOCK:
                    kbd_leds ^= F_CAPS;
                    rx_flags ^= F_CAPS;
                    SetKbdLeds(kbd_leds);
                    break;

            }
        }
    }
}

