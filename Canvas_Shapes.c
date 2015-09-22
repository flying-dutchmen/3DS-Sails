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
