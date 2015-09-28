#include "Canvas_Shapes.h"

//with exception of poly && unless stated/commented in the above otherwise source were cited -->
//https://github.com/shinyquagsire23/DCPU-3DS
//https://github.com/AlbertoSONIC/3DS_Paint

//Revised --> http://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C
void line(u8* screen, int x0, int y0, int x1, int y1, u32 colour) {
  int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
  int err = (dx>dy ? dx : -dy)/2, e2;
 
  for(;;){
    SetCanvasPixel(screen,x0,y0,colour);
    if (x0==x1 && y0==y1) break;
    e2 = err;
    if (e2 >-dx) { err -= dy; x0 += sx; }
    if (e2 < dy) { err += dx; y0 += sy; }
  }
}

void box(u8* screen, int x, int y, int w, int h, u32 colour)
{
	line(screen, x, y, x+w, y, colour); //top
	line(screen, x, y, x, y+h, colour); //left
	line(screen, x, y+h, x+w, y+h, colour); //bottom
	line(screen, x+w, y, x+w, y+h, colour); //right
}

//plot nth number vertex -->  tested &or draftfinal
void poly(u8* screen, int ** argv, int argc, u32 colour)
//void poly(u8* screen, int * argv, int argc, u32 colour)
//void poly(u8* screen, int *argv[], int argc, u32 colour)
{ 
    int i;
    for(i =0;i<argc-2;i+=2) //roll through all vertices && increment 2 positions
      	line(screen, argv[i], argv[i+1], argv[i+2], argv[i+3], colour);
//Close polygon      	
      	line(screen, argv[0], argv[1], argv[argc-1], argv[argc], colour);
}

//altered --> https://bitbucket.org/xerpi
void rectfill(u8* screen, int x, int y, int w, int h, u32 colour)
{
	int i, j;
	for (i = 0; i < w; ++i) {
		for (j = 0; j < h; ++j) {
			SetCanvasPixel(screen, x+i, y+j, colour);
		}
	}
}

/*
#define Circle(screen, cx, cy, radius, colour) (Ellipse(screen, cx, cy, radius, radius, colour));
void Circle(u8* screen, int xCen, int yCen, int radius, u32 colour)
{
	int x = 0;
	int y = radius;
	int p = (5 - radius*4)/4;
	Circum(screen, xCen, yCen, x, y, colour);
	while(x < y){
		x++;
		if(p < 0){
			p += 2*x+1;
		}
		else{
			y--;
			p += 2*(x-y)+1;
		}
		Circum(screen, xCen, yCen, x, y, colour);
	}
}
*/

void CircleFill(u8* screen, int xCen, int yCen, int radius, u32 colour)
{
	Circle(screen, xCen, yCen, radius, colour);
	int x,y;
	for(y = -radius;y <= radius;y++){
		for(x = -radius;x <= radius;x++)
			if(x*x + y*y <= radius*radius + radius * .8f)
				SetCanvasPixel(screen, xCen + x, yCen + y, colour);
	}
}

/*
void Circum(u8* screen, int cx, int cy, int x, int y, u32 colour)
{

	if(x == 0){
		SetCanvasPixel(screen, cx, cy + y, colour);
		SetCanvasPixel(screen, cx, cy - y, colour);
		SetCanvasPixel(screen, cx + y, cy, colour);
		SetCanvasPixel(screen, cx - y, cy, colour);
	}
	if(x == y){
		SetCanvasPixel(screen, cx + x, cy + y, colour);
		SetCanvasPixel(screen, cx - x, cy + y, colour);
		SetCanvasPixel(screen, cx + x, cy - y, colour);
		SetCanvasPixel(screen, cx - x, cy - y, colour);
	}
	if(x < y){
		SetCanvasPixel(screen, cx + x, cy + y, colour);
		SetCanvasPixel(screen, cx - x, cy + y, colour);
		SetCanvasPixel(screen, cx + x, cy - y, colour);
		SetCanvasPixel(screen, cx - x, cy - y, colour);
		SetCanvasPixel(screen, cx + y, cy + x, colour);
		SetCanvasPixel(screen, cx - y, cy + x, colour);
		SetCanvasPixel(screen, cx + y, cy - x, colour);
		SetCanvasPixel(screen, cx - y, cy - x, colour);
	}
}
*/ 

//ellipsePlotPoints && Ellipse && ROUND
//Revised --> http://www.programming-techniques.com/2012/01/drawing-ellipse-with-mid-point-ellipse.html
#define ROUND(a) ((int) (a + 0.5))

void ellipsePlotPoints(u8* screen, int xCenter, int yCenter, int x, int y, u32 colour){
    SetCanvasPixel(screen, xCenter + x, yCenter + y, colour);
    SetCanvasPixel(screen, xCenter - x, yCenter + y, colour);
    SetCanvasPixel(screen, xCenter + x, yCenter - y, colour);
    SetCanvasPixel(screen, xCenter - x, yCenter - y, colour);
}
void Ellipse(u8* screen, int xCenter, int yCenter, int Rx, int Ry, u32 colour){
    int Rx2 = Rx*Rx;
    int Ry2 = Ry*Ry;
    int twoRx2 = 2 * Rx2;
    int twoRy2 = 2 * Ry2;
    int p;
    int x = 0;
    int y = Ry;
    int px = 0;
    int py = twoRx2 * y;

    ellipsePlotPoints(screen, xCenter, yCenter, x, y, colour);

    /* For Region 1 */
    p = ROUND(Ry2 - (Rx2*Ry) + (0.25) * Rx2);
    while(px < py){
        x++;
        px += twoRy2;
        if(p < 0){
            p += Ry2 + px;
        }else{
            y--;
            py -= twoRx2;
            p += Ry2 + px - py;
        }
        ellipsePlotPoints(screen, xCenter, yCenter, x, y, colour);
    }

    /* For Region 2*/
    p = ROUND(Ry2 * (x + 0.5)*(x + 0.5) + Rx2 * (y - 1)*(y - 1) - Rx2 * Ry2);
    while(y > 0){
        y--;
        py -= twoRx2;
        if(p > 0){
            p += Rx2 - py;
        }else{
            x++;
            px += twoRy2;
            p += Rx2 - py + px;
        }
        ellipsePlotPoints(screen, xCenter, yCenter, x, y, colour);
    }

}

//QuadRationalBezierSeg &&  plotRotatedEllipseRect &&  RotatedEllipse 
//revised --> http://members.chello.at/easyfilter/bresenham.js
QuadRationalBezierSeg(u8* screen, int x0, int y0, int x1, int y1, int x2, int y2, int w, u32 colour)
{                   /* plot a limited rational Bezier segment, squared weight */
  int sx = x2-x1, sy = y2-y1;                   /* relative values for checks */
  int dx = x0-x2, dy = y0-y2, xx = x0-x1, yy = y0-y1;
  int xy = xx*sy+yy*sx, cur = xx*sy-yy*sx, err;                  /* curvature */

//  assert(xx*sx <= 0.0 && yy*sy <= 0.0);   /* sign of gradient must not change */

  if (cur != 0.0 && w > 0.0) {                            /* no straight line */
    if (sx*sx+sy*sy > xx*xx+yy*yy) {               /* begin with shorter part */
      x2 = x0; x0 -= dx; y2 = y0; y0 -= dy; cur = -cur;         /* swap P0 P2 */
    }
    xx = 2.0*(4.0*w*sx*xx+dx*dx);                   /* differences 2nd degree */
    yy = 2.0*(4.0*w*sy*yy+dy*dy);
    sx = x0 < x2 ? 1 : -1;                                /* x step direction */
    sy = y0 < y2 ? 1 : -1;                                /* y step direction */
    xy = -2.0*sx*sy*(2.0*w*xy+dx*dy);

    if (cur*sx*sy < 0.0) {                              /* negated curvature? */
      xx = -xx; yy = -yy; xy = -xy; cur = -cur;
    }
    dx = 4.0*w*(x1-x0)*sy*cur+xx/2.0+xy;            /* differences 1st degree */
    dy = 4.0*w*(y0-y1)*sx*cur+yy/2.0+xy;

    if (w < 0.5 && (dy > xy || dx < xy)) {   /* flat ellipse, algorithm fails */
       cur = (w+1.0)/2.0; w = sqrt(w); xy = 1.0/(w+1.0);
       sx = floor((x0+2.0*w*x1+x2)*xy/2.0+0.5);/*subdivide curve in half */
       sy = floor((y0+2.0*w*y1+y2)*xy/2.0+0.5);
       dx = floor((w*x1+x0)*xy+0.5); dy = floor((y1*w+y0)*xy+0.5);
       QuadRationalBezierSeg(screen, x0,y0, dx,dy, sx,sy, cur,colour);/* plot separately */
       dx = floor((w*x1+x2)*xy+0.5); dy = floor((y1*w+y2)*xy+0.5);
       QuadRationalBezierSeg(screen, sx,sy, dx,dy, x2,y2, cur,colour);
       return;
    }
    err = dx+dy-xy;                                           /* error 1.step */
    do {
      SetCanvasPixel(screen,x0,y0,colour);                            /* plot curve */
      if (x0 == x2 && y0 == y2) return;       /* last pixel -> curve finished */
      x1 = 2*err > dy; y1 = 2*(err+yy) < -dy;/* save value for test of x step */
      if (2*err < dx || y1) { y0 += sy; dy += xy; err += dx += xx; }/* y step */
      if (2*err > dx || x1) { x0 += sx; dx += xy; err += dy += yy; }/* x step */
    } while (dy <= xy && dx >= xy);    /* gradient negates -> algorithm fails */
  }
  line(screen,x0,y0, x2,y2,colour);                     /* plot remaining needle to end */
}

plotRotatedEllipseRect(u8* screen,int x0, int y0, int x1, int y1, int zd, u32 colour)
{                  /* rectangle enclosing the ellipse, integer rotation angle */
   int xd = x1-x0, yd = y1-y0, w = xd*yd;
 //  if (zd == 0) return plotEllipseRect(x0,y0, x1,y1);          /* looks nicer */
   if (w != 0.0) w = (w-zd)/(w+w);                    /* squared weight of P1 */
 //  assert(w <= 1.0 && w >= 0.0);                /* limit angle to |zd|<=xd*yd */
   xd = floor(xd*w+0.5); yd = floor(yd*w+0.5);       /* snap to int */
   QuadRationalBezierSeg(screen,x0,y0+yd, x0,y0, x0+xd,y0, 1.0-w,colour);
   QuadRationalBezierSeg(screen,x0,y0+yd, x0,y1, x1-xd,y1, w,colour);
   QuadRationalBezierSeg(screen,x1,y1-yd, x1,y1, x1-xd,y1, 1.0-w,colour);
   QuadRationalBezierSeg(screen,x1,y1-yd, x1,y0, x0+xd,y0, w,colour);
}

RotatedEllipse(u8* screen,int x, int y, int a, int b, int angle, u32 colour)
{                                   /* plot ellipse rotated by angle (radian) */
   int xd = a*a, yd = b*b;
   int s = sin(angle), zd = (xd-yd)*s;               /* ellipse rotation */
   xd = sqrt(xd-zd*s), yd = sqrt(yd+zd*s);      /* surrounding rect */
   a = floor(xd+0.5); b = floor(yd+0.5); zd = zd*a*b/(xd*yd);  
   plotRotatedEllipseRect(screen, x-a,y-b, x+a,y+b, (4*zd*cos(angle)),colour);
}
