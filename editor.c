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
 * File:   editor.h
 * Author: Jacques Deschênes
 *
 * Created on 18 septembre 2013, 07:18
 * Description: un éditeur simple pour le VPC-32
 *
 */


#include "hardware/HardwareProfile.h"
#include "hardware/keyboard.h"
#include "console.h"


void ed(char *file_name){
    if (comm_channel!=LOCAL_CON){
        print(comm_channel, "Cet editeur fonctionne seulement sur la console locale\r");
        return;
    }

} // ed()



