/*
* Copyright 2013,2014 Jacques Deschênes
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
 * File:   store_spi.c
 * Author: jacques Deschênes
 * Description: shared SPI interface used by SDcard and SPIRAM
 *
 * Created on 25 novembre 2014, 15:40
 */

#include <peripheral/ports.h>
#include "store_spi.h"

unsigned char store_initialized=0;

void store_spi_init(){
    STORE_PORT |= SRAM_SEL|SDC_SEL;  // both devices disabled
    STORE_PORT &= ~STATUS_LED ; // status LED off
    STORE_TRIS &= ~(SRAM_SEL|SDC_SEL|STATUS_LED); // CS1, CS2 and STATUS_LED as output
    STORE_SPICON = 0x8120;   // ON (0x8000), CKE=1 (0x100), CKP=0, Master mode (0x20)
    //SPI2 at maximum speed, this 20Mhz for 23LC1024
    if (mGetPeripheralClock()<40000000L){
        STORE_SPIBRG=0;
    }else{
        STORE_SPIBRG = (mGetPeripheralClock() / (2 * 20000000)) - 1; // 20Mbits/sec
    }
    store_initialized=1;
}


// send one byte of data and receive one back at the same time
unsigned char writeSPI(unsigned char b)
{

	STORE_SPIBUF = b;
	while(!STORE_SPISTATbits.SPIRBF);	// wait transfer complete
	return STORE_SPIBUF;			// read the received value

}	// writeSPI

