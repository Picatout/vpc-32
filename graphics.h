/*
* Copyright 2013, Jacques Desch�nes
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
 * File:   graphics.h
 * Author: Jacques Desch�nes
 *
 * Created on 2 novembre 2013, 19:57
 */

#ifndef GRAPHICS_H
#define	GRAPHICS_H

void plot(int x, int y);
void line(int x1, int y1, int x2, int y2);
void rectangle(int x1, int y1, int x2, int y2);
void ellipse (int xc, int yc, int rx, int ry);
void polygon(int points[],int vertices);

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* GRAPHICS_H */

