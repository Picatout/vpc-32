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
 * File:   ntsc.h
 * Author: Jacques Deschênes
 *
 * Created on 26 août 2013, 07:48
 */

#ifndef NTSC_H
#define	NTSC_H

#ifdef	__cplusplus
extern "C" {
#endif

#define VRES 224  // résolution verticale
#define HRES 320  // résolution horizontale
#define BMP_SIZE (224*320/8) // grosseur de video_bmp en octets

extern unsigned int video_bmp[VRES][HRES/32];
extern volatile unsigned int ln_cnt;
extern volatile unsigned int video;

// protoypes fonction interface
    void VideoInit(void);

#ifdef	__cplusplus
}
#endif

#endif	/* NTSC_H */

