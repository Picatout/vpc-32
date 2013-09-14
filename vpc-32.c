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
#include "console.h"
#include "hardware/Pinguino/ff.h"
#include "vpForth/opcodes.h"
#include "vpForth/vpForth.h"

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



const char *msg1=" test video ntsc\r";
const char *msg2="01234567890123456789012345678901234567890123456789012"; // 53 caractères par ligne


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


void main(void) {
    int code;
    short key;
    unsigned char buff[BLK_SIZE];
    int i;
    unsigned int size;

    HardwareInit();
    UartInit(STDIO,115200,DEFAULT_LINE_CTRL);
    ln_cnt=0;
    video=0;
    test_pattern();
    UartPrint(STDOUT,"initialisation video\r");
    VideoInit();
    delay_ms(500);
    UartPrint(STDOUT,"initialisation clavier: ");
    if (KeyboardInit()){
        UartPrint(STDOUT,"OK\r");
        comm_channel=LOCAL_CON;
    }else{
        UartPrint(STDOUT,"erreur clavier\r");
        UartPrint(STDOUT,"Utilisation du lien sériel.\r");
        comm_channel=SERIAL_CON;
    }
    text_coord_t cpos;
    UartPrint(STDOUT,"initialisation peripherique carte SD.\r");
    UartPrint(STDOUT,"initialisation carte SD: ");
    if (!mount(0)){
        UartPrint(STDOUT,"Echec\r");
    }
    UartPrint(STDOUT, "OK\r");
//    FIL *fp;
//    FILINFO *fo;
//    fp=malloc(sizeof(FIL));
//    fo=malloc(sizeof(FILINFO));
//    if (!f_open(fp,"readme.txt",FA_READ)){
//        f_stat("readme.txt",fo);
//        if (!f_read(fp,buff,fo->fsize,(UINT*)&i)){
//            clear_screen();
//            buff[fo->fsize]=0;
//            print("file size: ");
//            print_int(i,10);
//            put_char('\r');
//            print(buff);
//            f_close(fp);
//            if (i==40){
//                f_open(fp,"readme.txt",FA_WRITE);
//                f_lseek(fp,i);
//                f_write(fp,"test ecriture\r",14,(UINT*)&i);
//                f_stat("readme.txt",fo);
//                print("grandeur apres ecriture: ");
//                print_int(fo->fsize,10);
//                f_close(fp);
//            }
//            free(fp);
//            free(fo);
//        }
//    }
//    UartPrint(STDOUT,"lecture du registre CSD\r");
//    clear_screen();
//    size=disk_ioctl(0,GET_SECTOR_SIZE,buff);
//    print_int(*(unsigned short *)buff,10);
//    put_char('\r');
//    size=disk_ioctl(0,GET_SECTOR_COUNT,buff);
//    print_int(*(int*)buff,10);
//    put_char('\r');
//    size=disk_ioctl(0,GET_BLOCK_SIZE,buff);
//    print_int(*(int*)buff,10);
//    put_char('\r');
//    for (i=511;i;i--){
//        buff[i]=255;
//    }
//    while (1);
//    delay_ms(2000);
    tone(400,1000);
    UartPrint(STDOUT,"initialisation completee.\r");
    set_cursor(CR_BLOCK);
    clear_screen();
    while(1){
        vpForth();
    } // while(1)
} // main()


