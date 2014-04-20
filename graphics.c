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
 *  File: graphics.c
 *  Author: Jacques Deschênes
 *  Created on 2 novembre 2013
 *  Description: fonctions graphiques.
 *  REF: http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
 *  REF: http://members.chello.at/~easyfilter/bresenham.html
 */
#include "hardware/ntsc.h"
#include <math.h>

int getPixel(int x, int y){
    int h,ofs;
    h=x/32;
    ofs=31-x%32;
    return (video_bmp[y][h]&(1<<ofs))>>ofs;
}//getPixel()

void setPixel(int x, int y){
    int h,ofs;
    if (y>(VRES-1)) return; // hors limites
    h= x/32;
    if (h>HRES/32) return; // hors limites
    ofs = 31 - x%32;
    video_bmp[y][h]|= (1<<ofs);
} // setPixel()

void clearPixel(int x, int y){
    int h,ofs;
    if (y>(VRES-1)) return; // hors limites
    h= x/32;
    if (h>HRES/32) return; // hors limites
    ofs = 31 - x%32;
    video_bmp[y][h]&= ~(1<<ofs);
}//clearPixel()

//  REF: http://members.chello.at/~easyfilter/bresenham.html
void line(int x0, int y0, int x1, int y1)
{
   int dx =  abs(x1-x0), sx = x0<x1 ? 1 : -1;
   int dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1;
   int err = dx+dy, e2; /* error value e_xy */

   for(;;){  /* loop */
      setPixel(x0,y0);
      if (x0==x1 && y0==y1) break;
      e2 = 2*err;
      if (e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
      if (e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
   }
}//line()

void rectangle(int x1, int y1, int x2, int y2){
    line(x1,y1,x1,y2);
    line(x2,y1,x2,y2);
    line(x1,y1,x2,y1);
    line(x1,y2,x2,y2);
}//rectangle()

//  REF: http://members.chello.at/~easyfilter/bresenham.html
void circle(int xc, int yc, int r)
{
   int x = -r, y = 0, err = 2-2*r; /* II. Quadrant */
   do {
      setPixel(xc-x, yc+y); /*   I. Quadrant */
      setPixel(xc-y, yc-x); /*  II. Quadrant */
      setPixel(xc+x, yc-y); /* III. Quadrant */
      setPixel(xc+y, yc+x); /*  IV. Quadrant */
      r = err;
      if (r <= y) err += ++y*2+1;           /* e_xy+e_y < 0 */
      if (r > x || err > y) err += ++x*2+1; /* e_xy+e_x > 0 or no 2nd y-step */
   } while (x < 0);
}//circle()

/* REF: http://members.chello.at/~easyfilter/bresenham.html
 * dessine une ellipse circoncrite par un rectangle
 */
void ellipse (int x0, int y0, int x1, int y1){
   int a = abs(x1-x0), b = abs(y1-y0), b1 = b&1; /* values of diameter */
   long dx = 4*(1-a)*b*b, dy = 4*(b1+1)*a*a; /* error increment */
   long err = dx+dy+b1*a*a, e2; /* error of 1.step */

   if (x0 > x1) { x0 = x1; x1 += a; } /* if called with swapped points */
   if (y0 > y1) y0 = y1; /* .. exchange them */
   y0 += (b+1)/2; y1 = y0-b1;   /* starting pixel */
   a *= 8*a; b1 = 8*b*b;

   do {
       setPixel(x1, y0); /*   I. Quadrant */
       setPixel(x0, y0); /*  II. Quadrant */
       setPixel(x0, y1); /* III. Quadrant */
       setPixel(x1, y1); /*  IV. Quadrant */
       e2 = 2*err;
       if (e2 <= dy) { y0++; y1--; err += dy += a; }  /* y step */
       if (e2 >= dx || 2*err > dy) { x0++; x1--; err += dx += b1; } /* x step */
   } while (x0 <= x1);

   while (y0-y1 < b) {  /* too early stop of flat ellipses a=1 */
       setPixel(x0-1, y0); /* -> finish tip of ellipse */
       setPixel(x1+1, y0++);
       setPixel(x0-1, y1);
       setPixel(x1+1, y1--);
   }
}//ellipse()

/*
 * points[]={x1,y1,x2,y2,x3,y3,...}
 * vertices est le nombre de points
 */
void polygon(const int points[], int vertices){
    int i;
    for(i=0;i<(2*vertices-2);i+=2){
        line(points[i],points[i+1],points[i+2],points[i+3]);
    }
    line(points[0],points[1],points[i],points[i+1]);
}//polygon()

//  REF: http://members.chello.at/~easyfilter/bresenham.html
void besiez(int x0, int y0, int x1, int y1, int x2, int y2){
  int sx = x2-x1, sy = y2-y1;
  long xx = x0-x1, yy = y0-y1, xy;         /* relative values for checks */
  double dx, dy, err, cur = xx*sy-yy*sx;                    /* curvature */

  //if(xx*sx <= 0 && yy*sy <= 0) return;  /* sign of gradient must not change */

  if (sx*(long)sx+sy*(long)sy > xx*xx+yy*yy) { /* begin with longer part */
    x2 = x0; x0 = sx+x1; y2 = y0; y0 = sy+y1; cur = -cur;  /* swap P0 P2 */
  }
  if (cur != 0) {                                    /* no straight line */
    xx += sx; xx *= sx = x0 < x2 ? 1 : -1;           /* x step direction */
    yy += sy; yy *= sy = y0 < y2 ? 1 : -1;           /* y step direction */
    xy = 2*xx*yy; xx *= xx; yy *= yy;          /* differences 2nd degree */
    if (cur*sx*sy < 0) {                           /* negated curvature? */
      xx = -xx; yy = -yy; xy = -xy; cur = -cur;
    }
    dx = 4.0*sy*cur*(x1-x0)+xx-xy;             /* differences 1st degree */
    dy = 4.0*sx*cur*(y0-y1)+yy-xy;
    xx += xx; yy += yy; err = dx+dy+xy;                /* error 1st step */
    do {
      setPixel(x0,y0);                                     /* setPixel curve */
      if (x0 == x2 && y0 == y2) return;  /* last pixel -> curve finished */
      y1 = 2*err < dx;                  /* save value for test of y step */
      if (2*err > dy) { x0 += sx; dx -= xy; err += dy += yy; } /* x step */
      if (    y1    ) { y0 += sy; dy -= xy; err += dx += xx; } /* y step */
    } while (dy < dx );           /* gradient negates -> algorithm fails */
  }
  line(x0,y0, x2,y2);                  /* setPixel remaining part to end */
}//besiez()


void fill(int x, int y){
}//fill()