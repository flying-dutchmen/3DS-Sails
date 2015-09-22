//hit from the 3DSPong
//
//
//
//

#include "Canvas.h"

//Revised from --> http://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C
void line(u8* screen, int x0, int y0, int x1, int y1, u32 colour) {
  int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
  int err = (dx>dy ? dx : -dy)/2, e2;
 
  for(;;){
    SetCanvasPixel(x0,y0,colour);
    if (x0==x1 && y0==y1) break;
    e2 = err;
    if (e2 >-dx) { err -= dy; x0 += sx; }
    if (e2 < dy) { err += dx; y0 += sy; }
  }
}

void LineHV(u8* screen, int x1, int y1, int x2, int y2, u32 colour)
{
	int x, y;
	if (x1 == x2){
		if (y1<y2) for (y = y1; y < y2; y++) SetCanvasPixel(x1,y,colour);
		else for (y = y2; y < y1; y++) SetCanvasPixel(x1,y,colour);
	} else {
		if (x1<x2) for (x = x1; x < x2; x++) SetCanvasPixel(x,y1,colour);
		else for (x = x2; x < x1; x++) SetCanvasPixel(x,y1,colour);
	}
}

void box(u8* screen, int x1, int y1, int x2, int y2, u32 colour)
{
	LineHV(screen, x1, y1, x2, y1, colour);
	LineHV(screen, x2, y1, x2, y2, colour);
	LineHV(screen, x1, y2, x2, y2, colour);
	LineHV(screen x1, y1, x1, y2, colour);
}

void Circle(u8* screen, int xCen, int yCen, int radius, u32 colour)
{
	int x = 0;
	int y = radius;
	int p = (5 - radius*4)/4;
	Circum(screen, xCen, yCen, x, ycolour);
	while(x < y){
		x++;
		if(p < 0){
			p += 2*x+1;
		}
		else{
			y--;
			p += 2*(x-y)+1;
		}
		Circum(screen, xCen, yCen, x, ycolour);
	}
}

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


//Revised from -->http://www.programming-techniques.com/2012/01/drawing-ellipse-with-mid-point-ellipse.html
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