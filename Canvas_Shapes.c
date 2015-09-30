#include "Canvas_Shapes.h"

//with exception of poly && Ellipse && unless stated/commented in the above otherwise source were cited elsewear-->
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

//tested &or finaldraft
void box(u8* screen, int x, int y, int w, int h, u32 colour)
{
	line(screen, x, y, x+w, y, colour); //top
	line(screen, x, y, x, y+h, colour); //left
	line(screen, x, y+h, x+w, y+h, colour); //bottom
	line(screen, x+w, y, x+w, y+h, colour); //right
}

//plot nth number vertex -->  tested &or finaldraft
void poly(u8* screen, int ** argv, int argc, u32 colour)
//void poly(u8* screen, int * argv, int argc, u32 colour)
//void poly(u8* screen, int *argv[], int argc, u32 colour)
//in the begining there was void poly(u8* screen, int x1, int y1, int x2, int y2, int x3, int y3, u32 colour)
{
    int i;
//roll through the array
    for(i=0;i<argc-2;i+=2)
      	line(screen, argv[i], argv[i+1], argv[i+2], argv[i+3], colour); 
//automatically Close polygon      	
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

//#define Circle(screen, cx, cy, radius, colour) (Ellipse(screen, cx, cy, radius, radius, 0, colour));
//not at all happy with LCD output, too be nixxed &or deprecated for floodfill
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

//pillaged --> http://www.uwgb.edu/dutchs/Geometry/HTMLCanvas/ObliqueEllipses5.HTM
void Ellipse(u8* screen, int xctr, int yctr, int majaxis, int minaxis, int rotnrad, u32 colour){
	//Draw ellipse of arbitrary orientation
	float t = 0; //Parametric equation variable
	int x0 = xctr + majaxis*cos(rotnrad);// initial x-val
	int y0 = yctr - majaxis*sin(rotnrad);// initial y-val
	int x1 = x0;// final x-val
	int y1 = y0;// final y-val
		
	for (t=0; t <= 6.3; t = t + 0.1)
	{
	x0=x1;
	y0=y1;
	x1= xctr + majaxis*cos(t)*cos(rotnrad) - minaxis*sin(t)*sin(rotnrad);
	y1= yctr - majaxis*cos(t)*sin(rotnrad) - minaxis*sin(t)*cos(rotnrad);
	line (screen,x0,y0,x1,y1,colour);
	}//close for loop
	
	//Draw Cartesian Axes
//	line(screen,0,yctr,2*xctr,yctr,colour);
//	line(screen,xctr,0,xctr,2*yctr,colour);

	//Draw Major and Minor Axes
//	line(screen,xctr+majaxis*cos(rotnrad),yctr-majaxis*sin(rotnrad),xctr-majaxis*cos(rotnrad),yctr+majaxis*sin(rotnrad),colour);
//	line(screen,xctr+minaxis*sin(rotnrad),yctr+minaxis*cos(rotnrad),xctr-minaxis*sin(rotnrad),yctr-minaxis*cos(rotnrad),colour);
}//close Ellipse()
