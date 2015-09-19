#include "Canvas.h"
//Revamped from the sources of (Shiny Quagsire & AlbertoSONIC & xerpi & Lua-Player-Plus)
//& portions untested

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
	//GetPixel  Lua-Player-Plus
	//colour = (screen[v] & 0x00FFFFFF) | (0xFFFFFFFF & 0xFF000000);
	colour = (screen[v]) & 0xFF;
	colour =+ (screen[v+1]>>8) & 0xFF;
	colour =+ (screen[v+2]>>16) & 0xFF;
        return colour;
}

//todo
//LED-Memory * flick through FB[1,2,3] Vram/Cram/FSrom/sdmc buffer transfer speeds > (6 / 1.5) && old128 || new256?
int MaxWidth(u8* screen)
{ 
//top
   if (screen = (gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL) || gfxGetFramebuffer(GFX_TOP, GFX_RIGHT, NULL, NULL))) return 400;
//bottom
   else if (screen = gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL)) return 340;
}

void ClearCanvas(u8* screen, u32 colour)
{ 
	int height=240;
	int width = MaxWidth(screen);
	int i, j;
	for(i=1;i<width;i++)
           for(j=1;j<height;j++)
		SetCanvasPixel(screen,i,j,colour);
}

void SetRecRe(u8* screen, int Top, int Left, int Height, int Width, u32* Region)
{ 
	int CanvasWidth = MaxWidth(screen);
	int i, j;
	for(i=1;i<Width;i++)
           for(j=1;j<Height;j++)
    //linare section of screen gfxbuffer todo... (height-y+x*height)*3 ?
		SetCanvasPixel(screen[j+Top*CanvasWidth+i+Left],i+Left,j+Top, Region[i,j]);
}

u32* GetRecRe(u8* screen, int Top, int Left, int Height, int Width)
{  
	u32* Region;
	int CanvasWidth = MaxWidth(screen);
	int i, j;
	for(i=1;i<Width;i++)
           for(j=1;j<Height;j++)
    //linare section of screen gfxbuffer todo... (height-y+x*height)*3 ?
	  Region[i,j] = GetCanvasPixel(screen[j+Top*CanvasWidth+i+Left],i+Left,j+Top);
	return Region;	
}
