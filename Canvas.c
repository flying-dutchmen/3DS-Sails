#include "Canvas.h"
#include "Canvas_Pallet.h"
// the Pro-liter-riot's, Sklaven (a turned-out &or bent FSF/Open-Source) 
// [act 1, scene 1] 
// Eg. "Cash-in-Hand?", " U'r live?", "or still friends with Dr. Nelson Mandal";
//
// Citizen-Ken: 
//                We're going on a guilt trip too israil. & Do you need IT/Marketing in 3-D? 
// A-Thesis-on: 
//                Well it's comprized mostly of FreePascal & can't U c/c++ U'r time is drawing niegh 
//                 to obtain your companies, very, very own corprate, dirigible listings? 
//
//PS: Oblivious too the device buffers & GPU is estranged! & Revamped from sources listed bellow 
//
// ShinyQuagsire23 --> https://github.com/shinyquagsire23/DCPU-3DS
//& StapleButter --> https://github.com/StapleButter/blargSnes
//& AlbertoSONIC --> https://github.com/AlbertoSONIC/3DS_Paint
//& Steveice10 --> https://github.com/Steveice10/FBI
//& Lua-Player-Plus --> https://github.com/Rinnegatamante
//& smealum --> https://github.com/smealum/ctrulib

//Too be later included graphic file formats, special thanks too 
//+They have the second coolest avitar on the github? I beleve there logo "says it all"!!
//mtheall --> https://github.com/mtheall/screenshot_png/blob/master/source/screenshot_png.c :: get_pixel
//& xerpi --> https://github.com/xerpi 

//portions revised --> Lua-Player-Plus :: Graphics.cpp :: DrawAlphaPixel
void SetCanvasPixel(u8* screen, int x, int y, u32 colour)
{
       u8 alpha = (((colour) >> 24) & 0xFF); //alpha
       int height=240;
       u32 v=(height-1-y+x*height)*3;
     if (alpha = 0xFF)
      {	
        screen[v]=colour & 0xFF;         //blue
        screen[v+1]=(colour>>8) & 0xFF;  //green
        screen[v+2]=(colour>>16) & 0xFF; //red
      } 
      else 
      { //this alpha is slow, needs bitwise &= ~ re-written
//	float ratio = alpha / 255.0f;
//	screen[v] = ((colour & 0xFF) * ratio) + (screen[v] * (1.0 - ratio));               //blue
//	screen[v+1] = ((((colour) >> 8) & 0xFF) * ratio) + (screen[v+1] * (1.0 - ratio));  //green
//	screen[v+2] = ((((colour) >> 16) & 0xFF) * ratio) + (screen[v+2] * (1.0 - ratio)); //red 
	
	//the !float version should be faster
	//revised --> smealum :: 3ds_hb_menu :: gfx.c :: gfxDrawSpriteAlpha
	screen[v] = (((colour & 0xFF) * alpha) + (screen[v] * (255 - alpha))) >> 8;// div 256;
	screen[v+1] = (((((colour) >> 8) & 0xFF)* alpha) + (screen[v+1] * (255 - alpha))) / 256;
	screen[v+2] = (((((colour) >> 16) & 0xFF)* alpha) + (screen[v+2] * (255 - alpha))) / 256;
      }
}

//found @ https://github.com/Lectem/3Damnesic 
#define ABGR8(r,g,b,a) ((((r)&0xFF)<<24) | (((g)&0xFF)<<16) | (((b)&0xFF)<<8) | (((a)&0xFF)<<0))
//& found //gs.h --> crtulib nintendo 3ds
//#define RGBA8(r,g,b,a) ((((r)&0xFF)<<24) | (((g)&0xFF)<<16) | (((b)&0xFF)<<8) | (((a)&0xFF)<<0))

//#define GetCanvasPixel(screen, x, y) (GetCanvasPixelEx(screen, GSP_BGR8_OES, x, y))
u32 GetCanvasPixelEx(u8* screen, GSP_FramebufferFormats format, int x, int y)
{
        int height=240;
	u32 v=(height-1-y+x*height)*3;
	u32 colour =screen[v];
	colour += (screen[v+1] << 8);
	colour += (screen[v+2] << 16);
	if (bytes_per_pixel(format) == 3)  // 24bit 
         colour += (0xFF << 24); 
          else colour += (screen[v+3] << 24); // 32bit using an alpha
        return colour;
//       (bytes_per_pixel(format) == 3) ? return ABGR8(screen[v],screen[v+1],screen[v+2],0xFF):
//        return ABGR8(screen[v],screen[v+1],screen[v+2],screen[v+3])
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

	for(i=0;i<width;i++)
           for(j=0;j<height;j++)
		SetCanvasPixel(screen,i,j,colour);
}

#define Dual_for(j, i, Top, Left, Width, Height, into) \
        for(i=Left;i<Width+Left;i++)  \
           for(j=Top;j<Height+Top;j++){into;}; 

//u32 transparent;
//#define SetRecRe(screen, Top, Left, Height, Width, Region) (SetRecEx(screen, Top, Left, Height, Width, Region, 1, linear))
void SetRecEx(u8* screen, int Top, int Left, int Height, int Width, u8* Region, bool CleanUp, RamType RamLocal)
{ 
//	int CanvasWidth = MaxWidth(screen);
	int i, j;
        u32 Pixel;
	for(i=Left;i<Width+Left;i++)
           for(j=Top;j<Height+Top;j++)
		{
		Pixel = GetCanvasPixel(Region,i-Left,j-Top);
		if (Pixel != transparent) SetCanvasPixel(screen, i, j, Pixel);
                }

  if (CleanUp) switch(RamLocal){
       case linear: linearFree(Region); break;  
       case vram: vramFree(Region); break;  
       default: free(Region); 
}

//in the begin there was --> SetCanvasPixel(screen[j+Top*CanvasWidth+i+Left],i+Left,j+Top, Region[i,j])
}

// & portions modify --> xerpi :: libsf2d :: sf2d_texture.c :: sf2d_create_texture
//#define GetRecRe(screen, Top, Left, Height, Width) (GetRecEx(screen, Top, Left, Height, Width, linear))
u8 * GetRecEx(u8* screen, int Top, int Left, int Height, int Width, RamType RamLocal)
{  
//	int CanvasWidth = MaxWidth(screen); ???
	u32 bitmapsize = Width*Height*3; 
        u8* Region;
        
switch(RamLocal){
case linear: Region = (u8*)linearMemAlign(bitmapsize, 0x8);
             memset(Region, 0, bitmapsize);
             break;

case vram:   Region = (u8*)vramMemAlign(bitmapsize, 0x8); 
             GX_SetMemoryFill(NULL, Region, 0x00000000, (u32*)&(Region)[bitmapsize], GX_FILL_TRIGGER | GX_FILL_32BIT_DEPTH,NULL, 0x00000000, NULL, 0);
	     gspWaitForPSC0();
             break;
default: 
	     Region = (u8*)malloc(bitmapsize);
             memset(Region, 0, bitmapsize);
}	
	int i, j;
	for(i=Left;i<Width+Left;i++)
           for(j=Top;j<Height+Top;j++)
			SetCanvasPixel(Region, i-Left, j-Top, GetCanvasPixel(screen,i,j));

// goal reached "section screen with out repeated gfxbuffer"
//	  Region[i,j] = GetCanvasPixel(screen[j+Top*CanvasWidth+i+Left],i+Left,j+Top);

	return Region;	
}

//the Pro-liter-riot's Sklaven "kennyd-lee" 
//present's "long sex-live the Pro-liter-riot, &  3 nude lcd's 1/2 of in 3-D"
//also (unmolested Citrus, GPU, openGL, GIMP, VLC, &or Nanox, or any & all other that "got bent Mozilla" that have endured some form of rape)?
