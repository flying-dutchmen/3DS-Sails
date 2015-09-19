#include "Canvas.h"

void SetCanvasPixel(u8* screen, int x, int y, u32 colour)
{
	int height=240;
	u32 v=(height-y+x*height)*3;
	screen[v]=colour & 0xFF;
	screen[v+1]=(colour>>8) & 0xFF;
	screen[v+2]=(colour>>16) & 0xFF;
}

u32 * GetCanvasPixel(u8* screen, int x, int y)
{
	int colour, height=240;
	u32 v=(height-y+x*height)*3;
	colour = (screen[v]) & 0xFF;
	colour =+ (screen[v+1]>>8) & 0xFF;
	colour =+ (screen[v+2]>>16) & 0xFF;
        return colour;
}

void ClearCanvas(u8* screen, u32 colour)
{ 
	int height=240;
	int width;
//top
   if (screen = (gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL) || gfxGetFramebuffer(GFX_TOP, GFX_RIGHT, NULL, NULL))) width = 400;

//bottom
   else if (screen = gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL)) width = 340;
	int i, j;
	for(i=1;i<width;i++)
           for(j=1;j<height;j++)
		SetCanvasPixel(screen,i,j,colour);
}

void SetRecRe(u8* screen, int Top, int Left, int Height, int Width, u32* Region)
{ 
	int MaxWidth, MaxHeight=240;
//top
   if (screen = (gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL) || gfxGetFramebuffer(GFX_TOP, GFX_RIGHT, NULL, NULL))) MaxWidth = 400;
//bottom
   else if (screen = gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL)) MaxWidth = 340;
	int i, j;
	for(i=1;i<Width;i++)
           for(j=1;j<Height;j++)
    //linare section of screen gfxbuffer todo...
		SetCanvasPixel(screen[j+Top*MaxWidth+i+Left],i+Left,j+Top, Region[i,j]);
}
