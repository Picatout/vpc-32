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
 * File:   graphics.h
 * Author: Jacques Deschênes
 *
 * Created on 2 novembre 2013, 19:57
 */

#ifndef GRAPHICS_H
#define	GRAPHICS_H

int getPixel(int x, int y);
void setPixel(int x, int y);
void clearPixel(int x, int y);
void line(int x0, int y0, int x1, int y1);
void rectangle(int x0, int y0, int x1, int y1);
void circle(int xc, int yc, int r);
void ellipse (int x0, int y0, int x1, int y1);
void polygon(const int points[],int vertices);
void bezier(int x0, int y0, int x1, int y1, int x2, int y2);
void fill(int x, int y);

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* GRAPHICS_H */

