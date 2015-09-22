#include "Canvas.h"
// the Pro-liter-riot's, Sklaven (a turned-out &or bent FSF/Open-Source) 
// [act 1, scene 1] 
// Eg. u'r "Cash-in-Hand?", "live?", "or still friends with Dr. Nelson Mandal";
//
// Citizen-Ken: 
//                Well we're going on a guilt trip too israil. & Do you need IT/Marketing in 3-D? 
// A-Thesis-on: 
//                it's comprized of FreePascal, can't U c/c++ U'r time is drawing niegh 
//                 to obtain your companies, very, very own corprate, dirigible listings? 
//
//PS: Oblivious too! Also some portions are un-tested & Revamped from sources listed bellow 
//
// ShinyQuagsire23 --> https://github.com/shinyquagsire23/DCPU-3DS
//& AlbertoSONIC --> https://github.com/AlbertoSONIC/3DS_Paint
//& xerpi --> https://github.com/xerpi 
//& steveice10 --> https://github.com/Steveice10/FBI
//& Lua-Player-Plus --> https://github.com/Rinnegatamante
//& smealum --> https://github.com/smealum/ctrulib


void SetCanvasPixel(u8* screen, int x, int y, u32 colour)
{
	int height=240;
	u32 v=(height-1-y+x*height)*3;
	screen[v]=colour & 0xFF;
	screen[v+1]=(colour>>8) & 0xFF;
	screen[v+2]=(colour>>16) & 0xFF;
}

//found @ https://github.com/Lectem/3Damnesic 
#define ABGR8(r, g, b, a) ((((r)&0xFF)<<24) | (((g)&0xFF)<<16) | (((b)&0xFF)<<8) | (((a)&0xFF)<<0))
//& found //gs.h --> crtulib nintendo 3ds
//#define RGBA8(r,g,b,a) ((((r)&0xFF)<<24) | (((g)&0xFF)<<16) | (((b)&0xFF)<<8) | (((a)&0xFF)<<0))

u32 GetCanvasPixel(u8* screen, int x, int y)
{
        int height=240;
	u32 v=(height-1-y+x*height)*3;

//GetPixel  Lua-Player-Plus
//	u32 colour = (screen[idx] & 0x00FFFFFF) | (0xFFFFFFFF & 0xFF000000);

        return ABGR8(screen[v],screen[v+1],screen[v+2],0xFF);
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


#define Dual_for(j,i, Width, Height, into) \
//	do { \
           for(i=0;i<Width;i++)for(j=0;j<Height;j++){into}; 
//	} while (0);

void ClearCanvas(u8* screen, u32 colour)
{ 
	int height=240;
	int width = MaxWidth(screen);
	int i, j;

	for(i=0;i<width;i++)
           for(j=0;j<height;j++)
		SetCanvasPixel(screen,i,j,colour);
}

void SetRecRe(u8* screen, int Top, int Left, int Height, int Width, u8* Region)
{ 
//	int CanvasWidth = MaxWidth(screen);
	int i, j;
	for(i=Left;i<Width;i++)
           for(j=Top;j<Height;j++)
 {
	    int si = (240-1-j+i*240) * 3;
	    int di = (Height-1-(Left-j)+((Top-i)*Height)) * 3;

             screen[si++] = Region[di++];
             screen[si++] = Region[di++];
             screen[si++] = Region[di++];
 }
//		SetCanvasPixel(screen[j+Top*CanvasWidth+i+Left],i+Left,j+Top, Region[i,j]);
}

//Re-tooled --> StapleButter :: blargSnes :: main.c :: "bool TakeScreenshot(char* path)"
u8 * GetRecRe(u8* screen, int Top, int Left, int Height, int Width)
{  
//	u32* Region;
	u32 bitmapsize = Width*Height*3;
	u8* Region = (u8*)MemAlloc(bitmapsize);
	memset(Region, 0, bitmapsize);
//	int CanvasWidth = MaxWidth(screen); ???

	int i, j;
	for(i=Left;i<Width;i++)
           for(j=Top;j<Height;j++)
{
			int si = (240-1-j+i*240) * 3;
			int di = (Height-1-(Left-j)+((Top-i)*Height)) * 3;
			
			Region[di++] = screen[si++];
			Region[di++] = screen[si++];
			Region[di++] = screen[si++];

// inverted linare section of screen with out repeated gfxbuffer todo... (Height-1-j+i*Height)*3 ?
//	  Region[i,j] = GetCanvasPixel(screen[j+Top*CanvasWidth+i+Left],i+Left,j+Top);

}
	return Region;	
}

//the Pro-liter-riot's Sklaven "kennyd-lee" 
//present's "long sex-live the Pro-liter-riot, &  3 nude lcd's 1/2 of in 3-D"
//also (unmolested Citrus, GPU, GIMP, openGL &or Nanox or any & all other that "got bent Mozilla" that have endure some form of rape)?
