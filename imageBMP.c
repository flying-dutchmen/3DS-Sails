//Revised for use nintendo 3ds, Kenny D. Lee 
//large portions used --> Andreas Hartl :: http://tipsandtricks.runicsoft.com/Cpp/BitmapTutorial.html#chapter4
//& portions used --> Mans Rullgard :: FFmpeg :: libavcodec :: bmp.c

#include <3ds.h>
#include <stdio.h>
#include "imageBMP.h"

#include "Canvas_font.h"

#define BI_RGB 0
#define BI_BITFIELDS 3

typedef struct tagBITMAPFILEHEADER 
{
   u16   bfType;        // must be 'BM' 
   u32   bfSize;        // size of the whole .bmp file
   u16   bfReserved1;   // must be 0
   u16   bfReserved2;   // must be 0
   u32   bfOffBits;     
} __attribute__((packed)) BITMAPFILEHEADER; 

typedef struct tagBITMAPINFOHEADER
{
   u32   biSize;            // size of the structure
   s32   biWidth;           // image width
   s32   biHeight;          // image height //abs(biHeight) bottom .. top
   u16   biPlanes;          // bitplanes
   u16   biBitCount;        // resolution 
   u32   biCompression;     // compression
   u32   biSizeImage;       // size of the image
   s32   biXPelsPerMeter;   // pixels per meter X
   s32   biYPelsPerMeter;   // pixels per meter Y
   u32   biClrUsed;         // colors used
   u32   biClrImportant;    // important colors
} __attribute__((packed)) BITMAPINFOHEADER;
   
void BITMAPINFOHEADERdebug(u8* screen, BITMAPINFOHEADER *image)
{
char* str[256];
sprintf(str, "biSize: %u", image->biSize);
CanvasString(screen, str, 0, 60, WHITE);
sprintf(str, "biWidth %d \n", image->biWidth);
CanvasString(screen, str, 0, 70, WHITE);
sprintf(str, "biHeight %d \n", image->biHeight);
CanvasString(screen, str, 0, 80, WHITE);
sprintf(str, "biPlanes %u \n", image->biPlanes);
CanvasString(screen, str, 0, 90, WHITE);
sprintf(str, "biBitCount %u", image->biBitCount);
CanvasString(screen, str, 0, 100, WHITE);
sprintf(str, "biCompression %u", image->biCompression);
CanvasString(screen, str, 0, 110, WHITE);
sprintf(str, "biSizeImage %u", image->biSizeImage);
CanvasString(screen, str, 0, 120, WHITE);
sprintf(str, "biXPelsPerMeter %u", image->biXPelsPerMeter);
CanvasString(screen, str, 0, 130, WHITE);
sprintf(str, "biYPelsPerMeter %u", image->biYPelsPerMeter);
CanvasString(screen, str, 0, 140, WHITE);
sprintf(str, "biClrUsed %u", image->biClrUsed);
CanvasString(screen, str, 0, 150, WHITE);
sprintf(str, "biClrImportant %u", image->biClrImportant);
CanvasString(screen, str, 0, 160, WHITE);
}

void BITMAPFILEHEADERdebug(u8* screen, BITMAPFILEHEADER *image)
{
char* str[256];
sprintf(str, "bfType: %u", image->bfType);
CanvasString(screen, str, 0, 10, WHITE);
sprintf(str, "bfSize %u \n", image->bfSize);
CanvasString(screen, str, 0, 20, WHITE);
sprintf(str, "bfReserved1 %u \n", image->bfReserved1);
CanvasString(screen, str, 0, 30, WHITE);
sprintf(str, "bfReserved2 %u \n", image->bfReserved2);
CanvasString(screen, str, 0, 40, WHITE);
sprintf(str, "bfOffBits %u", image->bfOffBits);
CanvasString(screen, str, 0, 50, WHITE);
}

bool SaveBMP ( // BYTE* Buffer, int width, int height, long paddedsize, LPCTSTR bmpfile )
const char* path, imagebuff * image)
{
	Handle file;
	FS_path filePath;
        FS_archive sdmcArchive;
	filePath.type = PATH_CHAR;
	filePath.size = strlen(path) + 1;
	filePath.data = (u8*)path;

	// declare bmp structures 
	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER info;
	
	// andinitialize them to zero
	memset ( &bmfh, 0, sizeof (BITMAPFILEHEADER ) );
	memset ( &info, 0, sizeof (BITMAPINFOHEADER ) );
	
	// fill the infoheader

	info.biSize = sizeof(BITMAPINFOHEADER);
	info.biWidth = image->width;
	info.biHeight = image->height;
	info.biPlanes = 1;			// we only have one bitplane
	info.biBitCount = 24;		// RGB mode is 24 bits
	info.biCompression = BI_RGB;	
	info.biSizeImage = 0;		// can be 0 for 24 bit images 

// Calculating the size of a bitmap found --> https://msdn.microsoft.com/en-us/library/ms969901.aspx
//	info.biSizeImage = ((((info.biWidth * info.biBitCount) + 31) & ~31) >> 3) * info.biHeight;
	info.biXPelsPerMeter = 0x0ec4;     // paint and PSP use this values
	info.biYPelsPerMeter = 0x0ec4;     
	info.biClrUsed = 0;			// we are in RGB mode and have no palette
	info.biClrImportant = 0;    // all colors are important

	// fill the fileheader with data

	bmfh.bfType = 0x4d42;       // 0x4d42 = 'BM'
	bmfh.bfReserved1 = 0;
	bmfh.bfReserved2 = 0;
	bmfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + info.biSizeImage; // paddedsize;
	bmfh.bfOffBits = 0x36;		// number of bytes to start of bitmap bits
	
	// now we open the file to write to

	Result res = FSUSER_OpenFile(NULL, &file, sdmcArchive, filePath, FS_OPEN_CREATE | FS_OPEN_WRITE, FS_ATTRIBUTE_NONE);
	if (res)
		return false;

	
	// write file header
	unsigned long bwritten;
	if (FSFILE_Write(file, &bwritten, 0, &bmfh, sizeof ( BITMAPFILEHEADER ), FS_WRITE_NOFLUSH) == false )
         //WriteFile ( file, &bmfh, sizeof ( BITMAPFILEHEADER ), &bwritten, NULL ) == false )
	{	
		FSFILE_Close (file);
  		svcCloseHandle(file);
		return false;
	}
	// write infoheader
	if (FSFILE_Write(file, &bwritten, 0, &info, sizeof ( BITMAPINFOHEADER ), FS_WRITE_NOFLUSH) == false )
         // WriteFile ( file, &info, sizeof ( BITMAPINFOHEADER ), &bwritten, NULL ) == false )
	{	
		FSFILE_Close (file);
  		svcCloseHandle(file);
		return false;
	}
	// write image data
	if (FSFILE_Write(file, &bwritten, 0, &image->data, info.biSizeImage, FS_WRITE_FLUSH) == false )
        // WriteFile ( file, Buffer, paddedsize, &bwritten, NULL ) == false )
	{	
		FSFILE_Close (file);
  		svcCloseHandle(file);
		return false;
	}
	
	// and close file
	FSFILE_Close (file);
	svcCloseHandle(file);

	return true;
}

//revised --> AlbertoSONIC :: 3DS_Paint :: bool saveDrawing(char* path)
bool _saveBMP(const char* path, imagebuff * image)
{
	int x, y;

	Handle file;
	FS_path filePath;
        FS_archive sdmcArchive;
	filePath.type = PATH_CHAR;
	filePath.size = strlen(path) + 1;
	filePath.data = (u8*)path;

	Result res = FSUSER_OpenFile(NULL, &file, sdmcArchive, filePath, FS_OPEN_CREATE | FS_OPEN_WRITE, FS_ATTRIBUTE_NONE);
	if (res)
		return false;

	u32 byteswritten;

	u32 bitmapsize = image->width * image->height * 3;
	u8* tempbuf = (u8*)malloc(0x36 + bitmapsize);
	memset(tempbuf, 0, 0x36 + bitmapsize);

	FSFILE_SetSize(file, (u16)(0x36 + bitmapsize));

	*(u16*)&tempbuf[0x0] = 0x4D42;
	*(u32*)&tempbuf[0x2] = 0x36 + bitmapsize;
	*(u32*)&tempbuf[0xA] = 0x36;
	*(u32*)&tempbuf[0xE] = 0x28;
	*(u32*)&tempbuf[0x12] = image->width; // width
	*(u32*)&tempbuf[0x16] = image->height; // height
	*(u32*)&tempbuf[0x1A] = 0x00180001;
	*(u32*)&tempbuf[0x22] = bitmapsize;

	for (y = 0; y < image->height; y++)
	{
		for (x = 0; x < image->width; x++)
		{
			int si = (((image->height-1) - y) + (x * image->height)) * image->depth;
			int di = 0x36 + (x + ((image->height-1) - y) * image->width) * image->depth;

			tempbuf[di++] = image->data[si++];
			tempbuf[di++] = image->data[si++];
			tempbuf[di++] = image->data[si++];
		}
	}

	FSFILE_Write(file, &byteswritten, 0, (u32*)tempbuf, 0x36 + bitmapsize, 0x10001);

	FSFILE_Close(file);
	free(tempbuf);
	return true;
}

//found --> http://tipsandtricks.runicsoft.com/Cpp/BitmapTutorial.html#chapter4
u8* ConvertBMPToRGBBuffer(u8* Buffer, int width, int height )
{
 int x, y;
	// first make sure the parameters are valid
	if ( ( NULL == Buffer ) || ( width == 0 ) || ( height == 0 ) )
		return NULL;

	// find the number of padding bytes
		
	int padding = 0;
	int scanlinebytes = width * 3;
	while ( ( scanlinebytes + padding ) % 4 != 0 )     // DWORD = 4 bytes
		padding++;
	// get the padded scanline width
	int psw = scanlinebytes + padding;

	// create new buffer
        u8* newbuf = (u8*)malloc(width*height*3);
//	u8* newbuf = new BYTE[width*height*3];
	
	// now we loop trough all bytes of the original buffer, 
	// swap the R and B bytes and the scanlines
	long bufpos = 0;   
	long newpos = 0;
	for (y = 0; y < height; y++ )
		for (x = 0; x < 3 * width; x+=3 )
		{
			newpos = y * 3 * width + x;     
			bufpos = ( height - y - 1 ) * psw + x;

			newbuf[newpos] = Buffer[bufpos + 2];       
			newbuf[newpos + 1] = Buffer[bufpos+1]; 
			newbuf[newpos + 2] = Buffer[bufpos];     
		}

	return newbuf;
}

//revised --> Rinnegatamante :: Lua-Player-Plus :: Graphics.cpp :: Bitmap* decodeBMPfile(const char* fname)
//& portions used --> http://tipsandtricks.runicsoft.com/Cpp/BitmapTutorial.html#chapter4 :: ConvertBMPToRGBBuffer
imagebuff * loadBMP(const char* path)
{
	int x, y;

        u32 rgb[3];
	// declare bitmap structures
	BITMAPFILEHEADER bmpheader;
	BITMAPINFOHEADER bmpinfo;

	Handle file;
	u32 bytesRead;
	u64 size;
	FS_path filePath = FS_makePath(PATH_CHAR, path);
	FS_archive archive = (FS_archive) { ARCH_SDMC, (FS_path) { PATH_EMPTY, 1, (u8*)"" }};
	FSUSER_OpenFileDirectly(NULL, &file, archive, filePath, FS_OPEN_READ, FS_ATTRIBUTE_NONE);

	FSFILE_GetSize(file, &size);
	imagebuff* result = (imagebuff*)malloc(sizeof(imagebuff));
	
	if(!result) {
		FSFILE_Close(file);
		svcCloseHandle(file);
		return 0;
	}

	// read file header
	FSFILE_Read(file, &bytesRead, 0, &bmpheader,sizeof(BITMAPFILEHEADER));
        BITMAPFILEHEADERdebug(screenBottom, &bmpheader);
	//read bitmap info
	FSFILE_Read(file, &bytesRead, 0x0e, &bmpinfo,sizeof(BITMAPINFOHEADER));

        // Calculating the size of a bitmap found --> https://msdn.microsoft.com/en-us/library/ms969901.aspx
//        if (bmpinfo.biSizeImage == 0) bmpinfo.biSizeImage = ((((bmpinfo.biWidth * bmpinfo.biBitCount) + 31) & ~31) >> 3) * bmpinfo.biHeight;
        BITMAPINFOHEADERdebug(screenBottom, &bmpinfo);

/*
  if(bmpinfo.biCompression == BI_BITFIELDS){
        u32 bitmasks = bytesRead;
	FSFILE_Read(file, &bytesRead, bitmasks, &rgb[0],4); //red
	FSFILE_Read(file, &bytesRead, bitmasks+4, &rgb[1],4); //green
	FSFILE_Read(file, &bytesRead, bitmasks+8, &rgb[2],4); //blue
	FSFILE_Read(file, &bytesRead, bitmasks+12, &rgb[2],4); //alpha
     }

    switch(bmpinfo.biBitCount){
     case 32:
         if(bmpinfo.biCompression == BI_BITFIELDS){
             rgb[0] = (rgb[0] >> 15) & 3;
             rgb[1] = (rgb[1] >> 15) & 3;
             rgb[2] = (rgb[2] >> 15) & 3;
 
             if(rgb[0] + rgb[1] + rgb[2] != 3 ||
                rgb[0] == rgb[1] || rgb[0] == rgb[2] || rgb[1] == rgb[2]){
                 break;
             }
         } else {
             rgb[0] = 2;
             rgb[1] = 1;
             rgb[2] = 0;
         }
      default: break;
}
*/
	result->width = bmpinfo.biWidth;
	result->height = abs(bmpinfo.biHeight);
	result->depth = bmpinfo.biBitCount >> 3;  //works for 8, 16, 24, 32 & !monchrome 1

	result->data = (u8*)malloc(result->height * result->width * result->depth); //24bit canvas buffer
	memset (result->data, 0, result->height * result->width * result->depth);

	u8* tempbuf = (u8*)malloc(bmpinfo.biSizeImage); 
	memset (tempbuf, 0, bmpinfo.biSizeImage);

	FSFILE_Read(file, &bytesRead, bmpheader.bfOffBits, tempbuf, bmpinfo.biSizeImage);

	FSFILE_Close(file);
	svcCloseHandle(file);

	int padding = 0;
	int scanlinebytes = result->width * result->depth;
	while ((scanlinebytes + padding) % 4 != 0)     // DWORD = 4 bytes
		padding++;
	// get the padded scanline width
	int psw = scanlinebytes + padding;

//char* str[256];
//sprintf(str, "scanlinebytes: %d", scanlinebytes);
//CanvasString(screenBottom, str, 0, 170, WHITE);

//sprintf(str, "psw: %d", psw);
//CanvasString(screenBottom, str, 0, 180, WHITE);

	// swap the R and B bytes and the scanlines
	u32 bufpos, newpos;
	for (y = 0; y < result->height; y++ )
		for (x = 0; x < result->width; x++)//for 24bit is kind of 1/2 crap ok, but wii need %100 of image?
		{
		   newpos = (result->height-1-y+x*result->height) * result->depth; //map pixels --> nintendo 3ds canvas 
		if (bmpinfo.biHeight > 0) 
			bufpos = ((result->height - 1 - y) * psw) + (result->depth * x); // run through scanlines starting @ bottom work back to the top
		   else
			bufpos =  (y * psw) + (result->depth * x); // padded scanlines starting @ top work to the bottom 

			result->data[newpos] = tempbuf[bufpos + 2];       
			result->data[newpos + 1] = tempbuf[bufpos+1]; 
			result->data[newpos + 2] = tempbuf[bufpos];  
			if (bmpinfo.biBitCount==32) result->data[newpos + 3] = tempbuf[bufpos + 3]; //if our rastermap, uses an alpha byte    
		}
	free(tempbuf);
        result->used == 1;
	return result;
}
