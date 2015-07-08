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
 * File: spiram.c
 * Author: Jacques Deschênes
 * Description: interface SPI mémoire RAM
 * Created on 2014-06-02
 *
 */

#include "HardwareProfile.h"
#include <peripheral/ports.h>
#include "spiram.h"

#define SPIRAM_CSEL  (2)

#define enable_spiram() LATBCLR=SPIRAM_CSEL
#define disable_spiram() LATBSET=SPIRAM_CSEL

void spiram_init(){ // peripheral initialisation
    spiram_write_mode(SPIRAM_MODE_SEQ);
}//f()

void spiram_access(int access){ // set access mode
}//f()

void spiram_read_mode(){// read mode register
}//f()

void spiram_write_mode(int mode){ // set mode register
   enable_spiram();
   
   disable_spiram();
}//f()

void spiram_write(unsigned address, unsigned count, const char* data){ // write to SPIRAM
}//f()

void spiram_read(unsigned address,unsigned count, char* buffer){ // read from SPIRAM
}//f()


